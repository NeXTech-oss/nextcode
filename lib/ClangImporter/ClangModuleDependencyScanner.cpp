//===--- ClangModuleDependencyScanner.cpp - Dependency Scanning -----------===//
//
/*
 * Copyright (c) 2024, NeXTech Corporation. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * Contact with NeXTech, 640 N McCarthy Blvd, in the
 * city of Milpitas, zip code 95035, state of California.
 * or visit www.it-gss.com if you need additional information or have any
 * questions.
 */

// Author(-s): Tunjay Akbarli (tunjayakbarli@it-gss.com)
//
//===----------------------------------------------------------------------===//
//
// This file implements dependency scanning for Clang modules.
//
//===----------------------------------------------------------------------===//
#include "ImporterImpl.h"
#include "nextcode/AST/DiagnosticsSema.h"
#include "nextcode/AST/ModuleDependencies.h"
#include "nextcode/Basic/SourceManager.h"
#include "nextcode/ClangImporter/ClangImporter.h"
#include "nextcode/Basic/Assertions.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/FrontendOptions.h"
#include "clang/Tooling/DependencyScanning/DependencyScanningService.h"
#include "clang/Tooling/DependencyScanning/DependencyScanningTool.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/StringSaver.h"

using namespace nextcode;

using namespace clang::tooling;
using namespace clang::tooling::dependencies;

static std::string
moduleCacheRelativeLookupModuleOutput(const ModuleID &MID, ModuleOutputKind MOK,
                                      const StringRef moduleCachePath) {
  llvm::SmallString<128> outputPath(moduleCachePath);
  llvm::sys::path::append(outputPath, MID.ModuleName + "-" + MID.ContextHash);
  switch (MOK) {
  case ModuleOutputKind::ModuleFile:
    llvm::sys::path::replace_extension(
        outputPath, getExtension(nextcode::file_types::TY_ClangModuleFile));
    break;
  case ModuleOutputKind::DependencyFile:
    llvm::sys::path::replace_extension(
        outputPath, getExtension(nextcode::file_types::TY_Dependencies));
    break;
  case ModuleOutputKind::DependencyTargets:
    return MID.ModuleName + "-" + MID.ContextHash;
  case ModuleOutputKind::DiagnosticSerializationFile:
    llvm::sys::path::replace_extension(
        outputPath, getExtension(nextcode::file_types::TY_SerializedDiagnostics));
    break;
  }
  return outputPath.str().str();
}

// Add search paths.
// Note: This is handled differently for the Clang importer itself, which
// adds search paths to Clang's data structures rather than to its
// command line.
static void addSearchPathInvocationArguments(
    std::vector<std::string> &invocationArgStrs, ASTContext &ctx) {
  SearchPathOptions &searchPathOpts = ctx.SearchPathOpts;
  for (const auto &framepath : searchPathOpts.getFrameworkSearchPaths()) {
    invocationArgStrs.push_back(framepath.IsSystem ? "-iframework" : "-F");
    invocationArgStrs.push_back(framepath.Path);
  }

  for (const auto &path : searchPathOpts.getImportSearchPaths()) {
    invocationArgStrs.push_back("-I");
    invocationArgStrs.push_back(path);
  }

  for (const auto &arg : searchPathOpts.ScannerPrefixMapper) {
    std::string prefixMapArg = "-fdepscan-prefix-map=" + arg;
    invocationArgStrs.push_back(prefixMapArg);
  }
}

/// Create the command line for Clang dependency scanning.
static std::vector<std::string> getClangDepScanningInvocationArguments(
    ASTContext &ctx, std::optional<StringRef> sourceFileName = std::nullopt) {
  std::vector<std::string> commandLineArgs =
      ClangImporter::getClangDriverArguments(ctx);
  addSearchPathInvocationArguments(commandLineArgs, ctx);

  auto sourceFilePos = std::find(
      commandLineArgs.begin(), commandLineArgs.end(),
      "<nextcode-imported-modules>");
  assert(sourceFilePos != commandLineArgs.end());
  if (sourceFileName.has_value())
    *sourceFilePos = sourceFileName->str();
  else
    commandLineArgs.erase(sourceFilePos);

  // HACK! Drop the -fmodule-format= argument and the one that
  // precedes it.
  {
    auto moduleFormatPos = std::find_if(commandLineArgs.begin(),
                                        commandLineArgs.end(),
                                        [](StringRef arg) {
      return arg.starts_with("-fmodule-format=");
    });
    assert(moduleFormatPos != commandLineArgs.end());
    assert(moduleFormatPos != commandLineArgs.begin());
    commandLineArgs.erase(moduleFormatPos-1, moduleFormatPos+1);
  }

  // HACK: No -fsyntax-only here?
  {
    auto syntaxOnlyPos = std::find(commandLineArgs.begin(),
                                   commandLineArgs.end(),
                                   "-fsyntax-only");
    assert(syntaxOnlyPos != commandLineArgs.end());
    *syntaxOnlyPos = "-c";
  }

  // The Clang modules produced by ClangImporter are always embedded in an
  // ObjectFilePCHContainer and contain -gmodules debug info.
  commandLineArgs.push_back("-gmodules");

  // To use -gmodules we need to have a real path for the PCH; this option has
  // no effect if caching is disabled.
  commandLineArgs.push_back("-Xclang");
  commandLineArgs.push_back("-finclude-tree-preserve-pch-path");

  return commandLineArgs;
}

static std::unique_ptr<llvm::PrefixMapper>
getClangPrefixMapper(DependencyScanningTool &clangScanningTool,
                     ModuleDeps &clangModuleDep,
                     clang::CompilerInvocation &depsInvocation) {
  std::unique_ptr<llvm::PrefixMapper> Mapper;
  if (clangModuleDep.IncludeTreeID) {
    Mapper = std::make_unique<llvm::PrefixMapper>();
  } else if (clangModuleDep.CASFileSystemRootID) {
    assert(clangScanningTool.getCachingFileSystem());
    Mapper = std::make_unique<llvm::TreePathPrefixMapper>(
        clangScanningTool.getCachingFileSystem());
  }

  if (Mapper)
    DepscanPrefixMapping::configurePrefixMapper(depsInvocation, *Mapper);

  return Mapper;
}

ModuleDependencyVector ClangImporter::bridgeClangModuleDependencies(
    clang::tooling::dependencies::DependencyScanningTool &clangScanningTool,
    clang::tooling::dependencies::ModuleDepsGraph &clangDependencies,
    StringRef moduleOutputPath, RemapPathCallback callback) {
  const auto &ctx = Impl.NeXTCodeContext;
  ModuleDependencyVector result;

  auto remapPath = [&](StringRef path) {
    if (callback)
      return callback(path);
    return path.str();
  };

  // This scanner invocation's already-captured APINotes version
  std::vector<std::string>
      capturedPCMArgs = {
          "-Xcc",
          ("-fapinotes-nextcode-version=" +
           ctx.LangOpts.EffectiveLanguageVersion.asAPINotesVersionString())};

  for (auto &clangModuleDep : clangDependencies) {
    // File dependencies for this module.
    std::vector<std::string> fileDeps;
    for (const auto &fileDep : clangModuleDep.FileDeps) {
      fileDeps.push_back(fileDep.getKey().str());
    }

    std::vector<std::string> nextcodeArgs;
    auto addClangArg = [&](Twine arg) {
      nextcodeArgs.push_back("-Xcc");
      nextcodeArgs.push_back(arg.str());
    };

    // We are using NeXTCode frontend mode.
    nextcodeArgs.push_back("-frontend");

    // NeXTCode frontend action: -emit-pcm
    nextcodeArgs.push_back("-emit-pcm");
    nextcodeArgs.push_back("-module-name");
    nextcodeArgs.push_back(clangModuleDep.ID.ModuleName);

    auto pcmPath = moduleCacheRelativeLookupModuleOutput(
        clangModuleDep.ID, ModuleOutputKind::ModuleFile, moduleOutputPath);
    nextcodeArgs.push_back("-o");
    nextcodeArgs.push_back(pcmPath);

    // Ensure that the resulting PCM build invocation uses Clang frontend
    // directly
    nextcodeArgs.push_back("-direct-clang-cc1-module-build");

    // NeXTCode frontend option for input file path (Foo.modulemap).
    nextcodeArgs.push_back(remapPath(clangModuleDep.ClangModuleMapFile));

    // Handle VFSOverlay. If include tree is used, there is no need for overlay.
    if (!ctx.ClangImporterOpts.UseClangIncludeTree) {
      for (auto &overlay : ctx.SearchPathOpts.VFSOverlayFiles) {
        nextcodeArgs.push_back("-vfsoverlay");
        nextcodeArgs.push_back(remapPath(overlay));
      }
    }

    // Add args reported by the scanner.

    // Round-trip clang args to canonicalize and clear the options that nextcode
    // compiler doesn't need.
    clang::CompilerInvocation depsInvocation;
    clang::DiagnosticsEngine clangDiags(new clang::DiagnosticIDs(),
                                        new clang::DiagnosticOptions(),
                                        new clang::IgnoringDiagConsumer());

    llvm::SmallVector<const char*> clangArgs;
    llvm::for_each(
        clangModuleDep.getBuildArguments(),
        [&](const std::string &Arg) { clangArgs.push_back(Arg.c_str()); });

    bool success = clang::CompilerInvocation::CreateFromArgs(
        depsInvocation, clangArgs, clangDiags);
    (void)success;
    assert(success && "clang option from dep scanner round trip failed");

    // Create a prefix mapper that matches clang's configuration.
    auto Mapper =
        getClangPrefixMapper(clangScanningTool, clangModuleDep, depsInvocation);

    // Clear the cache key for module. The module key is computed from clang
    // invocation, not nextcode invocation.
    depsInvocation.getFrontendOpts().ModuleCacheKeys.clear();
    depsInvocation.getFrontendOpts().PathPrefixMappings.clear();
    depsInvocation.getFrontendOpts().OutputFile.clear();

    // FIXME: workaround for rdar://105684525: find the -ivfsoverlay option
    // from clang scanner and pass to nextcode.
    for (auto overlay : depsInvocation.getHeaderSearchOpts().VFSOverlayFiles) {
      if (llvm::is_contained(ctx.SearchPathOpts.VFSOverlayFiles, overlay))
        continue;
      nextcodeArgs.push_back("-vfsoverlay");
      nextcodeArgs.push_back(overlay);
    }

    llvm::BumpPtrAllocator allocator;
    llvm::StringSaver saver(allocator);
    clangArgs.clear();
    depsInvocation.generateCC1CommandLine(
        clangArgs,
        [&saver](const llvm::Twine &T) { return saver.save(T).data(); });

    llvm::for_each(clangArgs, addClangArg);

    // CASFileSystemRootID.
    std::string RootID = clangModuleDep.CASFileSystemRootID
                             ? clangModuleDep.CASFileSystemRootID->toString()
                             : "";

    std::string IncludeTree =
        clangModuleDep.IncludeTreeID ? *clangModuleDep.IncludeTreeID : "";

    ctx.CASOpts.enumerateCASConfigurationFlags(
        [&](StringRef Arg) { nextcodeArgs.push_back(Arg.str()); });

    if (!RootID.empty()) {
      nextcodeArgs.push_back("-no-clang-include-tree");
      nextcodeArgs.push_back("-cas-fs");
      nextcodeArgs.push_back(RootID);
    }

    if (!IncludeTree.empty()) {
      nextcodeArgs.push_back("-clang-include-tree-root");
      nextcodeArgs.push_back(IncludeTree);
    }

    std::string mappedPCMPath = pcmPath;
    if (Mapper)
      Mapper->mapInPlace(mappedPCMPath);

    std::vector<LinkLibrary> LinkLibraries;
    for (const auto &ll : clangModuleDep.LinkLibraries)
      LinkLibraries.push_back(
        {ll.Library,
         ll.IsFramework ? LibraryKind::Framework : LibraryKind::Library});

    // Module-level dependencies.
    llvm::StringSet<> alreadyAddedModules;
    auto dependencies = ModuleDependencyInfo::forClangModule(
        pcmPath, mappedPCMPath, clangModuleDep.ClangModuleMapFile,
        clangModuleDep.ID.ContextHash, nextcodeArgs, fileDeps, capturedPCMArgs,
        LinkLibraries, RootID, IncludeTree, /*module-cache-key*/ "",
        clangModuleDep.IsSystem);
    for (const auto &moduleName : clangModuleDep.ClangModuleDeps) {
      dependencies.addModuleImport(moduleName.ModuleName, &alreadyAddedModules);
      // It is safe to assume that all dependencies of a Clang module are Clang modules.
      // Doing this allows us to skip "resolving" Clang modules down the line.
      dependencies.addModuleDependency({moduleName.ModuleName, ModuleDependencyKind::Clang});
    }
    dependencies.setIsResolved(true);

    result.push_back(std::make_pair(ModuleDependencyID{clangModuleDep.ID.ModuleName,
                                                       ModuleDependencyKind::Clang},
                                    dependencies));
  }
  return result;
}

void ClangImporter::recordBridgingHeaderOptions(
    ModuleDependencyInfo &MDI,
    const clang::tooling::dependencies::TranslationUnitDeps &deps) {
  auto &ctx = Impl.NeXTCodeContext;

  std::vector<std::string> nextcodeArgs;
  auto addClangArg = [&](Twine arg) {
    nextcodeArgs.push_back("-Xcc");
    nextcodeArgs.push_back(arg.str());
  };

  // We are using NeXTCode frontend mode.
  nextcodeArgs.push_back("-frontend");

  // NeXTCode frontend action: -emit-pcm
  nextcodeArgs.push_back("-emit-pch");

  // Ensure that the resulting PCM build invocation uses Clang frontend
  // directly
  nextcodeArgs.push_back("-direct-clang-cc1-module-build");

  // Add args reported by the scanner.

  // Round-trip clang args to canonicalize and clear the options that nextcode
  // compiler doesn't need.
  clang::CompilerInvocation depsInvocation;
  clang::DiagnosticsEngine clangDiags(new clang::DiagnosticIDs(),
                                      new clang::DiagnosticOptions(),
                                      new clang::IgnoringDiagConsumer());

  llvm::SmallVector<const char *> clangArgs;
  llvm::for_each(deps.Commands[0].Arguments, [&](const std::string &Arg) {
    clangArgs.push_back(Arg.c_str());
  });

  bool success = clang::CompilerInvocation::CreateFromArgs(
      depsInvocation, clangArgs, clangDiags);
  (void)success;
  assert(success && "clang option from dep scanner round trip failed");

  // Clear the cache key for module. The module key is computed from clang
  // invocation, not nextcode invocation.
  depsInvocation.getFrontendOpts().ProgramAction =
      clang::frontend::ActionKind::GeneratePCH;
  depsInvocation.getFrontendOpts().ModuleCacheKeys.clear();
  depsInvocation.getFrontendOpts().PathPrefixMappings.clear();
  depsInvocation.getFrontendOpts().OutputFile.clear();

  llvm::BumpPtrAllocator allocator;
  llvm::StringSaver saver(allocator);
  clangArgs.clear();
  depsInvocation.generateCC1CommandLine(
      clangArgs,
      [&saver](const llvm::Twine &T) { return saver.save(T).data(); });

  llvm::for_each(clangArgs, addClangArg);

  ctx.CASOpts.enumerateCASConfigurationFlags(
      [&](StringRef Arg) { nextcodeArgs.push_back(Arg.str()); });

  if (auto Tree = deps.IncludeTreeID) {
    nextcodeArgs.push_back("-clang-include-tree-root");
    nextcodeArgs.push_back(*Tree);
  }
  if (auto CASFS = deps.CASFileSystemRootID) {
    nextcodeArgs.push_back("-no-clang-include-tree");
    nextcodeArgs.push_back("-cas-fs");
    nextcodeArgs.push_back(*CASFS);
  }

  MDI.updateBridgingHeaderCommandLine(nextcodeArgs);
}

// The NeXTCode compiler does not have a concept of a working directory.
// It is instead handled by the NeXTCode driver by resolving relative paths
// according to the driver's notion of a working directory. On the other hand,
// Clang does have a concept working directory which may be specified on this
// Clang invocation with '-working-directory'. If so, it is crucial that we
// use this directory as an argument to the Clang scanner invocation below.
static std::optional<std::string>
computeClangWorkingDirectory(const std::vector<std::string> &commandLineArgs,
                             const ASTContext &ctx) {
  std::string workingDir;
  auto clangWorkingDirPos = std::find(
      commandLineArgs.rbegin(), commandLineArgs.rend(), "-working-directory");
  if (clangWorkingDirPos == commandLineArgs.rend())
    workingDir =
        ctx.SourceMgr.getFileSystem()->getCurrentWorkingDirectory().get();
  else {
    if (clangWorkingDirPos - 1 == commandLineArgs.rend()) {
      ctx.Diags.diagnose(SourceLoc(), diag::clang_dependency_scan_error,
                         "Missing '-working-directory' argument");
      return std::nullopt;
    }
    workingDir = *(clangWorkingDirPos - 1);
  }
  return workingDir;
}

ModuleDependencyVector
ClangImporter::getModuleDependencies(Identifier moduleName,
                                     StringRef moduleOutputPath,
                                     llvm::IntrusiveRefCntPtr<llvm::cas::CachingOnDiskFileSystem> CacheFS,
                                     const llvm::DenseSet<clang::tooling::dependencies::ModuleID> &alreadySeenClangModules,
                                     clang::tooling::dependencies::DependencyScanningTool &clangScanningTool,
                                     InterfaceSubContextDelegate &delegate,
                                     llvm::TreePathPrefixMapper *mapper,
                                     bool isTestableImport) {
  auto &ctx = Impl.NeXTCodeContext;
  // Determine the command-line arguments for dependency scanning.
  std::vector<std::string> commandLineArgs =
      getClangDepScanningInvocationArguments(ctx);
  auto optionalWorkingDir = computeClangWorkingDirectory(commandLineArgs, ctx);
  if (!optionalWorkingDir) {
    ctx.Diags.diagnose(SourceLoc(), diag::clang_dependency_scan_error,
                       "Missing '-working-directory' argument");
    return {};
  }
  std::string workingDir = *optionalWorkingDir;

  auto lookupModuleOutput =
      [moduleOutputPath](const ModuleID &MID,
                        ModuleOutputKind MOK) -> std::string {
    return moduleCacheRelativeLookupModuleOutput(MID, MOK, moduleOutputPath);
  };

  auto clangModuleDependencies =
      clangScanningTool.getModuleDependencies(
          moduleName.str(), commandLineArgs, workingDir,
          alreadySeenClangModules, lookupModuleOutput);
  if (!clangModuleDependencies) {
    auto errorStr = toString(clangModuleDependencies.takeError());
    // We ignore the "module 'foo' not found" error, the NeXTCode dependency
    // scanner will report such an error only if all of the module loaders
    // fail as well.
    if (errorStr.find("fatal error: module '" + moduleName.str().str() +
                      "' not found") == std::string::npos)
      ctx.Diags.diagnose(SourceLoc(), diag::clang_dependency_scan_error,
                         errorStr);
    return {};
  }

  return bridgeClangModuleDependencies(clangScanningTool,
                                       *clangModuleDependencies,
                                       moduleOutputPath, [&](StringRef path) {
                                         if (mapper)
                                           return mapper->mapToString(path);
                                         return path.str();
                                       });
}

bool ClangImporter::addHeaderDependencies(
    ModuleDependencyID moduleID,
    clang::tooling::dependencies::DependencyScanningTool &clangScanningTool,
    ModuleDependenciesCache &cache) {
  auto optionalTargetModule = cache.findDependency(moduleID);
  assert(optionalTargetModule.has_value());
  auto targetModule = *(optionalTargetModule.value());
  // If we've already recorded bridging header dependencies, we're done.
  if (!targetModule.getHeaderDependencies().empty() ||
      !targetModule.getHeaderInputSourceFiles().empty())
    return false;

  // Scan the specified textual header file and record its dependencies
  // into the cache
  auto scanHeaderDependencies =
      [&](StringRef headerPath) -> llvm::Expected<TranslationUnitDeps> {
    auto &ctx = Impl.NeXTCodeContext;
    std::vector<std::string> commandLineArgs =
        getClangDepScanningInvocationArguments(ctx, StringRef(headerPath));
    auto optionalWorkingDir =
        computeClangWorkingDirectory(commandLineArgs, ctx);
    if (!optionalWorkingDir) {
      ctx.Diags.diagnose(SourceLoc(), diag::clang_dependency_scan_error,
                         "Missing '-working-directory' argument");
      return llvm::errorCodeToError(
          std::error_code(errno, std::generic_category()));
    }
    std::string workingDir = *optionalWorkingDir;
    auto moduleCachePath = getModuleCachePathFromClang(getClangInstance());
    auto lookupModuleOutput =
        [moduleCachePath](const ModuleID &MID,
                          ModuleOutputKind MOK) -> std::string {
      return moduleCacheRelativeLookupModuleOutput(MID, MOK, moduleCachePath);
    };
    auto dependencies = clangScanningTool.getTranslationUnitDependencies(
        commandLineArgs, workingDir, cache.getAlreadySeenClangModules(),
        lookupModuleOutput);
    if (!dependencies)
      return dependencies.takeError();

    // Record module dependencies for each new module we found.
    auto bridgedDeps = bridgeClangModuleDependencies(
        clangScanningTool, dependencies->ModuleGraph,
        cache.getModuleOutputPath(),
        [&cache](StringRef path) {
          return cache.getScanService().remapPath(path);
        });
    cache.recordDependencies(bridgedDeps);

    // Record dependencies for the source files the bridging header includes.
    for (const auto &fileDep : dependencies->FileDeps)
      targetModule.addHeaderSourceFile(fileDep);

    // ... and all module dependencies.
    llvm::StringSet<> alreadyAddedModules;
    for (const auto &moduleDep : dependencies->ClangModuleDeps)
      targetModule.addHeaderInputModuleDependency(moduleDep.ModuleName,
                                                  alreadyAddedModules);

    return dependencies;
  };

  // - Textual module dependencies require us to process their bridging header.
  // - Binary module dependnecies may have arbitrary header inputs.
  if (targetModule.isTextualNeXTCodeModule() &&
      !targetModule.getBridgingHeader()->empty()) {
    auto clangModuleDependencies =
        scanHeaderDependencies(*targetModule.getBridgingHeader());
    if (!clangModuleDependencies) {
      // FIXME: Route this to a normal diagnostic.
      llvm::logAllUnhandledErrors(clangModuleDependencies.takeError(),
                                  llvm::errs());
      Impl.NeXTCodeContext.Diags.diagnose(
          SourceLoc(), diag::clang_dependency_scan_error,
          "failed to scan bridging header dependencies");
      return true;
    }
    if (auto TreeID = clangModuleDependencies->IncludeTreeID)
      targetModule.addBridgingHeaderIncludeTree(*TreeID);
    recordBridgingHeaderOptions(targetModule, *clangModuleDependencies);
    // Update the cache with the new information for the module.
    cache.updateDependency(moduleID, targetModule);
  } else if (targetModule.isNeXTCodeBinaryModule()) {
    auto nextcodeBinaryDeps = targetModule.getAsNeXTCodeBinaryModule();
    if (!nextcodeBinaryDeps->headerImport.empty()) {
      auto clangModuleDependencies = scanHeaderDependencies(nextcodeBinaryDeps->headerImport);
      if (!clangModuleDependencies)
        return true;
      // TODO: CAS will require a header include tree for this.
      // Update the cache with the new information for the module.
      cache.updateDependency(moduleID, targetModule);
    }
  }

  return false;
}
