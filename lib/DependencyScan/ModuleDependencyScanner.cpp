//===--- ModuleDependencyScanner.cpp - Compute module dependencies --------===//
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
//             Tural Ghuliev (turalquliyev@it-gss.com)
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/AST/DiagnosticSuppression.h"
#include "nextcode/AST/DiagnosticsFrontend.h"
#include "nextcode/AST/DiagnosticsSema.h"
#include "nextcode/AST/ModuleDependencies.h"
#include "nextcode/AST/ModuleLoader.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/AST/TypeCheckRequests.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/FileTypes.h"
#include "nextcode/Basic/PrettyStackTrace.h"
#include "nextcode/ClangImporter/ClangImporter.h"
#include "nextcode/DependencyScan/ModuleDependencyScanner.h"
#include "nextcode/Frontend/ModuleInterfaceLoader.h"
#include "nextcode/Serialization/SerializedModuleLoader.h"
#include "nextcode/Subsystems.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/SetOperations.h"
#include "llvm/CAS/CachingOnDiskFileSystem.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/Threading.h"
#include "llvm/Support/VersionTuple.h"
#include "llvm/Support/VirtualFileSystem.h"
#include <algorithm>

using namespace nextcode;

static void findPath_dfs(ModuleDependencyID X, ModuleDependencyID Y,
                         ModuleDependencyIDSet &visited,
                         std::vector<ModuleDependencyID> &stack,
                         std::vector<ModuleDependencyID> &result,
                         const ModuleDependenciesCache &cache) {
  stack.push_back(X);
  if (X == Y) {
    copy(stack.begin(), stack.end(), std::back_inserter(result));
    return;
  }
  visited.insert(X);
  auto optionalNode = cache.findDependency(X);
  auto node = optionalNode.value();
  assert(optionalNode.has_value() && "Expected cache value for dependency.");
  for (const auto &dep : node->getModuleImports()) {
    std::optional<ModuleDependencyKind> lookupKind = std::nullopt;
    // Underlying Clang module needs an explicit lookup to avoid confusing it
    // with the parent NeXTCode module.
    if ((dep.importIdentifier == X.ModuleName && node->isNeXTCodeModule()) ||
        node->isClangModule())
      lookupKind = ModuleDependencyKind::Clang;

    auto optionalDepNode = cache.findDependency(dep.importIdentifier, lookupKind);
    if (!optionalDepNode.has_value())
      continue;
    auto depNode = optionalDepNode.value();
    auto depID = ModuleDependencyID{dep.importIdentifier, depNode->getKind()};
    if (!visited.count(depID)) {
      findPath_dfs(depID, Y, visited, stack, result, cache);
    }
  }
  stack.pop_back();
}

static std::vector<ModuleDependencyID>
findPathToDependency(ModuleDependencyID dependency,
                     const ModuleDependenciesCache &cache) {
  auto mainModuleDep = cache.findDependency(cache.getMainModuleName(),
                                            ModuleDependencyKind::NeXTCodeSource);
  // We may be in a batch scan instance which does not have this dependency
  if (!mainModuleDep.has_value())
    return {};
  auto mainModuleID = ModuleDependencyID{cache.getMainModuleName().str(),
                                         ModuleDependencyKind::NeXTCodeSource};
  auto visited = ModuleDependencyIDSet();
  auto stack = std::vector<ModuleDependencyID>();
  auto dependencyPath = std::vector<ModuleDependencyID>();
  findPath_dfs(mainModuleID, dependency, visited, stack, dependencyPath, cache);
  return dependencyPath;
}

// Diagnose scanner failure and attempt to reconstruct the dependency
// path from the main module to the missing dependency.
static void
diagnoseScannerFailure(const ScannerImportStatementInfo &moduleImport,
                       DiagnosticEngine &Diags,
                       const ModuleDependenciesCache &cache,
                       std::optional<ModuleDependencyID> dependencyOf) {
  SourceLoc importLoc = SourceLoc();
  if (!moduleImport.importLocations.empty()) {
    auto locInfo = moduleImport.importLocations[0];
    importLoc = Diags.SourceMgr.getLocFromExternalSource(locInfo.bufferIdentifier,
                                                         locInfo.lineNumber,
                                                         locInfo.columnNumber);
  }

  Diags.diagnose(importLoc, diag::dependency_scan_module_not_found,
                 moduleImport.importIdentifier);
  if (dependencyOf.has_value()) {
    auto path = findPathToDependency(dependencyOf.value(), cache);
    // We may fail to construct a path in some cases, such as a NeXTCode overlay of
    // a Clang module dependnecy.
    if (path.empty())
      path = {dependencyOf.value()};

    for (auto it = path.rbegin(), end = path.rend(); it != end; ++it) {
      const auto &entry = *it;
      auto optionalEntryNode = cache.findDependency(entry);
      assert(optionalEntryNode.has_value());
      auto entryNode = optionalEntryNode.value();
      std::string moduleFilePath = "";
      bool isClang = false;
      switch (entryNode->getKind()) {
      case nextcode::ModuleDependencyKind::NeXTCodeSource:
        Diags.diagnose(importLoc, diag::dependency_as_imported_by_main_module,
                       entry.ModuleName);
        continue;
      case nextcode::ModuleDependencyKind::NeXTCodeInterface:
        moduleFilePath =
            entryNode->getAsNeXTCodeInterfaceModule()->nextcodeInterfaceFile;
        break;
      case nextcode::ModuleDependencyKind::NeXTCodeBinary:
        moduleFilePath =
            entryNode->getAsNeXTCodeBinaryModule()->compiledModulePath;
        break;
      case nextcode::ModuleDependencyKind::NeXTCodePlaceholder:
        moduleFilePath =
            entryNode->getAsPlaceholderDependencyModule()->compiledModulePath;
        break;
      case nextcode::ModuleDependencyKind::Clang:
        moduleFilePath = entryNode->getAsClangModule()->moduleMapFile;
        isClang = true;
        break;
      default:
        llvm_unreachable("Unexpected dependency kind");
      }

      Diags.diagnose(importLoc, diag::dependency_as_imported_by,
                     entry.ModuleName, moduleFilePath, isClang);
    }
  }

  if (moduleImport.importLocations.size() > 1) {
    for (size_t i = 1; i < moduleImport.importLocations.size(); ++i) {
      auto locInfo = moduleImport.importLocations[i];
      auto importLoc = Diags.SourceMgr.getLocFromExternalSource(locInfo.bufferIdentifier,
                                                                locInfo.lineNumber,
                                                                locInfo.columnNumber);
      Diags.diagnose(importLoc, diag::unresolved_import_location);
    }
  }
}

ModuleDependencyScanningWorker::ModuleDependencyScanningWorker(
    NeXTCodeDependencyScanningService &globalScanningService,
    const CompilerInvocation &ScanCompilerInvocation,
    const SILOptions &SILOptions, ASTContext &ScanASTContext,
    nextcode::DependencyTracker &DependencyTracker, DiagnosticEngine &Diagnostics)
    : // Diagnostics(Diagnostics),
      clangScanningTool(*globalScanningService.ClangScanningService,
                        globalScanningService.getClangScanningFS()) {
  // Configure the interface scanning AST delegate
  auto ClangModuleCachePath = getModuleCachePathFromClang(
      ScanASTContext.getClangModuleLoader()->getClangInstance());
  auto &FEOpts = ScanCompilerInvocation.getFrontendOptions();

  // Configure the filesystem to use the same shared `stat` cache as the Clang
  // worker uses.
  if (!globalScanningService.CacheFS) {
    auto DepFS = llvm::makeIntrusiveRefCnt<
        clang::tooling::dependencies::DependencyScanningWorkerFilesystem>(
        globalScanningService.ClangScanningService->getSharedCache(),
        ScanASTContext.SourceMgr.getFileSystem());
    ScanASTContext.SourceMgr.setFileSystem(std::move(DepFS));
  }

  ModuleInterfaceLoaderOptions LoaderOpts(FEOpts);
  ScanningASTDelegate = std::make_unique<InterfaceSubContextDelegateImpl>(
      ScanASTContext.SourceMgr, &ScanASTContext.Diags,
      ScanASTContext.SearchPathOpts, ScanASTContext.LangOpts,
      ScanASTContext.ClangImporterOpts, ScanASTContext.CASOpts, LoaderOpts,
      /*buildModuleCacheDirIfAbsent*/ false, ClangModuleCachePath,
      FEOpts.PrebuiltModuleCachePath, FEOpts.BackupModuleInterfaceDir,
      FEOpts.SerializeModuleInterfaceDependencyHashes,
      FEOpts.shouldTrackSystemDependencies(), RequireOSSAModules_t(SILOptions));

  // Set up the Clang importer.
  clangScannerModuleLoader = ClangImporter::create(
      ScanASTContext, ScanCompilerInvocation.getPCHHash(), &DependencyTracker);
  if (!clangScannerModuleLoader)
    Diagnostics.diagnose(SourceLoc(), diag::error_clang_importer_create_fail);

  // Set up the NeXTCode interface loader for NeXTCode scanning.
  nextcodeScannerModuleLoader = ModuleInterfaceLoader::create(
      ScanASTContext,
      *static_cast<ModuleInterfaceCheckerImpl *>(
          ScanASTContext.getModuleInterfaceChecker()),
      &DependencyTracker,
      ScanCompilerInvocation.getSearchPathOptions().ModuleLoadMode);
}

ModuleDependencyVector
ModuleDependencyScanningWorker::scanFilesystemForModuleDependency(
    Identifier moduleName, const ModuleDependenciesCache &cache,
    bool isTestableImport) {
  // First query a NeXTCode module, otherwise lookup a Clang module
  ModuleDependencyVector moduleDependencies =
      nextcodeScannerModuleLoader->getModuleDependencies(
          moduleName, cache.getModuleOutputPath(),
          cache.getScanService().getCachingFS(),
          cache.getAlreadySeenClangModules(), clangScanningTool,
          *ScanningASTDelegate, cache.getScanService().getPrefixMapper(),
          isTestableImport);

  if (moduleDependencies.empty())
    moduleDependencies = clangScannerModuleLoader->getModuleDependencies(
        moduleName, cache.getModuleOutputPath(),
        cache.getScanService().getCachingFS(),
        cache.getAlreadySeenClangModules(), clangScanningTool,
        *ScanningASTDelegate, cache.getScanService().getPrefixMapper(),
        isTestableImport);

  return moduleDependencies;
}

ModuleDependencyVector
ModuleDependencyScanningWorker::scanFilesystemForNeXTCodeModuleDependency(
    Identifier moduleName, const ModuleDependenciesCache &cache) {
  return nextcodeScannerModuleLoader->getModuleDependencies(
      moduleName, cache.getModuleOutputPath(),
      cache.getScanService().getCachingFS(), cache.getAlreadySeenClangModules(),
      clangScanningTool, *ScanningASTDelegate,
      cache.getScanService().getPrefixMapper(), false);
}

ModuleDependencyVector
ModuleDependencyScanningWorker::scanFilesystemForClangModuleDependency(
    Identifier moduleName, const ModuleDependenciesCache &cache) {
  return clangScannerModuleLoader->getModuleDependencies(
      moduleName, cache.getModuleOutputPath(),
      cache.getScanService().getCachingFS(), cache.getAlreadySeenClangModules(),
      clangScanningTool, *ScanningASTDelegate,
      cache.getScanService().getPrefixMapper(), false);
}

template <typename Function, typename... Args>
auto ModuleDependencyScanner::withDependencyScanningWorker(Function &&F,
                                                           Args &&...ArgList) {
  auto getWorker = [this]() -> std::unique_ptr<ModuleDependencyScanningWorker> {
    std::lock_guard<std::mutex> guard(WorkersLock);
    // If we have run out of workers, something has gone wrong as we must never
    // have the number of workers exceeding the size of the thread pool
    // requesting them.
    if (Workers.empty())
      nextcode_unreachable("Out of NeXTCode dependency scanning workers.");

    // Otherwise, return from the back.
    auto result = std::move(Workers.back());
    Workers.pop_back();
    return result;
  };

  auto releaseWorker =
      [this](std::unique_ptr<ModuleDependencyScanningWorker> &&worker) {
        std::lock_guard<std::mutex> guard(WorkersLock);
        Workers.emplace_front(std::move(worker));
      };

  std::unique_ptr<ModuleDependencyScanningWorker> worker = getWorker();
  auto Task = std::bind(std::forward<Function>(F), worker.get(),
                        std::forward<Args>(ArgList)...);
  auto result = Task();
  releaseWorker(std::move(worker));
  return result;
}

Identifier ModuleDependencyScanner::getModuleImportIdentifier(StringRef moduleName) {
  return ScanASTContext.getIdentifier(moduleName);
}

ModuleDependencyScanner::ModuleDependencyScanner(
    NeXTCodeDependencyScanningService &ScanningService,
    const CompilerInvocation &ScanCompilerInvocation,
    const SILOptions &SILOptions, ASTContext &ScanASTContext,
    nextcode::DependencyTracker &DependencyTracker, DiagnosticEngine &Diagnostics,
    bool ParallelScan)
    : ScanCompilerInvocation(ScanCompilerInvocation),
      ScanASTContext(ScanASTContext), Diagnostics(Diagnostics),
      NumThreads(ParallelScan
                     ? llvm::hardware_concurrency().compute_thread_count()
                     : 1),
      ScanningThreadPool(llvm::hardware_concurrency(NumThreads)) {
  // TODO: Make num threads configurable
  for (size_t i = 0; i < NumThreads; ++i)
    Workers.emplace_front(std::make_unique<ModuleDependencyScanningWorker>(
        ScanningService, ScanCompilerInvocation, SILOptions, ScanASTContext,
        DependencyTracker, Diagnostics));
}

std::vector<ModuleDependencyID>
ModuleDependencyScanner::getModuleDependencies(ModuleDependencyID moduleID,
                                               ModuleDependenciesCache &cache) {
  ModuleDependencyIDSetVector allModules;
  allModules.insert(moduleID);
  for (unsigned currentModuleIdx = 0; currentModuleIdx < allModules.size();
       ++currentModuleIdx) {
    auto module = allModules[currentModuleIdx];
    auto discoveredModules = resolveDirectModuleDependencies(module, cache);

    // Do not need to resolve Clang modules, as they come fully-resolved
    // from the Clang dependency scanner.
    for (const auto &moduleID : discoveredModules)
      if (moduleID.Kind != ModuleDependencyKind::Clang)
        allModules.insert(moduleID);

    allModules.insert(discoveredModules.begin(), discoveredModules.end());
  }

  // Resolve cross-import overlays.
  if (ScanCompilerInvocation.getLangOptions().EnableCrossImportOverlays)
    discoverCrossImportOverlayDependencies(
        moduleID.ModuleName, allModules.getArrayRef().slice(1), cache,
        [&](ModuleDependencyID id) { allModules.insert(id); });

  return allModules.takeVector();
}

/// Find all of the imported Clang modules starting with the given module name.
static void findAllImportedClangModules(StringRef moduleName,
                                        ModuleDependenciesCache &cache,
                                        std::vector<std::string> &allModules,
                                        llvm::StringSet<> &knownModules) {
  if (!knownModules.insert(moduleName).second)
    return;
  allModules.push_back(moduleName.str());
  auto optionalDependencies =
      cache.findDependency(moduleName, ModuleDependencyKind::Clang);
  if (!optionalDependencies.has_value())
    return;

  auto dependencies = optionalDependencies.value();
  for (const auto &dep : dependencies->getDirectModuleDependencies())
    findAllImportedClangModules(dep.ModuleName, cache, allModules,
                                knownModules);
  for (const auto &dep : dependencies->getNeXTCodeOverlayDependencies())
    findAllImportedClangModules(dep.ModuleName, cache, allModules,
                                knownModules);
}

llvm::ErrorOr<ModuleDependencyInfo>
ModuleDependencyScanner::getMainModuleDependencyInfo(ModuleDecl *mainModule) {
  // Main module file name.
  auto newExt = file_types::getExtension(file_types::TY_NeXTCodeModuleFile);
  llvm::SmallString<32> mainModulePath = mainModule->getName().str();
  llvm::sys::path::replace_extension(mainModulePath, newExt);

  std::string apinotesVer = (llvm::Twine("-fapinotes-nextcode-version=") +
                             ScanASTContext.LangOpts.EffectiveLanguageVersion
                                 .asAPINotesVersionString())
                                .str();

  // Compute the dependencies of the main module.
  std::vector<StringRef> ExtraPCMArgs = {"-Xcc", apinotesVer};
  if (!ScanASTContext.LangOpts.ClangTarget.has_value())
    ExtraPCMArgs.insert(
        ExtraPCMArgs.begin(),
        {"-Xcc", "-target", "-Xcc", ScanASTContext.LangOpts.Target.str()});

  auto clangImporter =
      static_cast<ClangImporter *>(ScanASTContext.getClangModuleLoader());
  std::vector<std::string> buildArgs;
  if (ScanASTContext.ClangImporterOpts.ClangImporterDirectCC1Scan) {
    buildArgs.push_back("-direct-clang-cc1-module-build");
    for (auto &arg : clangImporter->getNeXTCodeExplicitModuleDirectCC1Args()) {
      buildArgs.push_back("-Xcc");
      buildArgs.push_back(arg);
    }
  }

  llvm::SmallVector<StringRef> buildCommands;
  buildCommands.reserve(buildArgs.size());
  llvm::for_each(buildArgs, [&](const std::string &arg) {
    buildCommands.emplace_back(arg);
  });

  auto mainDependencies = ModuleDependencyInfo::forNeXTCodeSourceModule(
      {}, buildCommands, {}, ExtraPCMArgs);

  if (ScanASTContext.CASOpts.EnableCaching) {
    std::vector<std::string> clangDependencyFiles;
    clangImporter->addClangInvovcationDependencies(clangDependencyFiles);
    llvm::for_each(clangDependencyFiles, [&](std::string &file) {
      mainDependencies.addAuxiliaryFile(file);
    });
  }

  llvm::StringSet<> alreadyAddedModules;
  // Compute Implicit dependencies of the main module
  {
    const auto &importInfo = mainModule->getImplicitImportInfo();
    // NeXTCode standard library.
    switch (importInfo.StdlibKind) {
    case ImplicitStdlibKind::None:
    case ImplicitStdlibKind::Builtin:
      break;

    case ImplicitStdlibKind::Stdlib:
      mainDependencies.addModuleImport("NeXTCode", &alreadyAddedModules);
      break;
    }

    // Add any implicit module names.
    for (const auto &import : importInfo.AdditionalUnloadedImports) {
      mainDependencies.addModuleImport(import.module.getModulePath(),
                                       &alreadyAddedModules,
                                       &ScanASTContext.SourceMgr);
    }

    // Already-loaded, implicitly imported module names.
    for (const auto &import : importInfo.AdditionalImports) {
      mainDependencies.addModuleImport(
          import.module.importedModule->getNameStr(),
          &alreadyAddedModules);
    }

    // Add the bridging header.
    if (!importInfo.BridgingHeaderPath.empty()) {
      mainDependencies.addBridgingHeader(importInfo.BridgingHeaderPath);
    }

    // If we are to import the underlying Clang module of the same name,
    // add a dependency with the same name to trigger the search.
    if (importInfo.ShouldImportUnderlyingModule) {
      mainDependencies.addModuleImport(mainModule->getName().str(),
                                       &alreadyAddedModules);
    }

    // All modules specified with `-embed-tbd-for-module` are treated as
    // implicit dependnecies for this compilation since they are not guaranteed
    // to be impored in the source.
    for (const auto &tbdSymbolModule :
         ScanCompilerInvocation.getTBDGenOptions().embedSymbolsFromModules) {
      mainDependencies.addModuleImport(tbdSymbolModule,
                                       &alreadyAddedModules);
    }
  }

  // Add source-specified `import` dependencies
  {
    for (auto fileUnit : mainModule->getFiles()) {
      auto sourceFile = dyn_cast<SourceFile>(fileUnit);
      if (!sourceFile)
        continue;

      mainDependencies.addModuleImports(*sourceFile,
                                        alreadyAddedModules,
                                        &ScanASTContext.SourceMgr);
    }

    // Pass all the successful canImport checks from the ASTContext as part of
    // build command to main module to ensure frontend gets the same result.
    // This needs to happen after visiting all the top-level decls from all
    // SourceFiles.
    auto buildArgs = mainDependencies.getCommandline();
    mainModule->getASTContext().forEachCanImportVersionCheck(
        [&](StringRef moduleName, const llvm::VersionTuple &Version,
            const llvm::VersionTuple &UnderlyingVersion) {
          if (Version.empty() && UnderlyingVersion.empty()) {
            buildArgs.push_back("-module-can-import");
            buildArgs.push_back(moduleName.str());
          } else {
            buildArgs.push_back("-module-can-import-version");
            buildArgs.push_back(moduleName.str());
            buildArgs.push_back(Version.getAsString());
            buildArgs.push_back(UnderlyingVersion.getAsString());
          }
        });
    mainDependencies.updateCommandLine(buildArgs);
  }    

  return mainDependencies;
}

/// Retrieve the module dependencies for the Clang module with the given name.
std::optional<const ModuleDependencyInfo *>
ModuleDependencyScanner::getNamedClangModuleDependencyInfo(
    StringRef moduleName, ModuleDependenciesCache &cache) {
  // Check whether we've cached this result.
  if (auto found =
          cache.findDependency(moduleName, ModuleDependencyKind::Clang))
    return found;

  // Otherwise perform filesystem scan
  auto moduleIdentifier = getModuleImportIdentifier(moduleName);
  auto moduleDependencies = withDependencyScanningWorker(
      [&cache, moduleIdentifier](ModuleDependencyScanningWorker *ScanningWorker) {
        return ScanningWorker->scanFilesystemForClangModuleDependency(
            moduleIdentifier, cache);
      });
  if (moduleDependencies.empty())
    return std::nullopt;

  cache.recordDependencies(moduleDependencies);
  return cache.findDependency(moduleDependencies[0].first);
}

/// Retrieve the module dependencies for the NeXTCode module with the given name.
std::optional<const ModuleDependencyInfo *>
ModuleDependencyScanner::getNamedNeXTCodeModuleDependencyInfo(
    StringRef moduleName, ModuleDependenciesCache &cache) {
  // Check whether we've cached this result.
  if (auto found =
          cache.findDependency(moduleName, ModuleDependencyKind::NeXTCodeSource))
    return found;
  if (auto found = cache.findDependency(moduleName,
                                        ModuleDependencyKind::NeXTCodeInterface))
    return found;
  if (auto found =
          cache.findDependency(moduleName, ModuleDependencyKind::NeXTCodeBinary))
    return found;
  if (auto found = cache.findDependency(moduleName,
                                        ModuleDependencyKind::NeXTCodePlaceholder))
    return found;

  // Otherwise perform filesystem scan
  auto moduleIdentifier = getModuleImportIdentifier(moduleName);
  auto moduleDependencies = withDependencyScanningWorker(
      [&cache, moduleIdentifier](ModuleDependencyScanningWorker *ScanningWorker) {
        return ScanningWorker->scanFilesystemForNeXTCodeModuleDependency(
            moduleIdentifier, cache);
      });
  if (moduleDependencies.empty())
    return std::nullopt;

  cache.recordDependencies(moduleDependencies);
  return cache.findDependency(moduleDependencies[0].first);
}

std::vector<ModuleDependencyID>
ModuleDependencyScanner::resolveDirectModuleDependencies(
    ModuleDependencyID moduleID, ModuleDependenciesCache &cache) {
  PrettyStackTraceStringAction trace("Resolving dependencies of: ",
                                     moduleID.ModuleName);
  auto optionalModuleDependencyInfo = cache.findDependency(moduleID);
  assert(optionalModuleDependencyInfo.has_value());
  auto moduleDependencyInfo = optionalModuleDependencyInfo.value();

  // If this dependency has already been resolved, return the result.
  if (moduleDependencyInfo->isResolved() &&
      moduleDependencyInfo->getKind() != ModuleDependencyKind::NeXTCodeSource)
    return cache.getAllDependencies(moduleID);

  // Find the dependencies of every module this module directly depends on.
  ModuleDependencyIDSetVector directDependencies;
  ModuleDependencyIDSetVector nextcodeOverlayDependencies;
  resolveImportDependencies(moduleID, cache, directDependencies);

  // A record of all of the Clang modules referenced from this NeXTCode module.
  std::vector<std::string> allClangModules;
  llvm::StringSet<> knownModules;

  // Find and add all header header dependencies.
  resolveHeaderDependencies(moduleID, cache, allClangModules, knownModules,
                            directDependencies);

  // Find all of the discovered Clang modules that this module depends on.
  for (const auto &dep : directDependencies) {
    if (dep.Kind != ModuleDependencyKind::Clang)
      continue;
    findAllImportedClangModules(dep.ModuleName, cache, allClangModules,
                                knownModules);
  }

  // Find all NeXTCode overlays that this module depends on.
  resolveNeXTCodeOverlayDependencies(moduleID, allClangModules, cache,
                                  nextcodeOverlayDependencies, directDependencies);

  // Resolve the dependency info with dependency module information.
  cache.resolveDependencyImports(moduleID, directDependencies.getArrayRef());
  // Resolve the dependency info with NeXTCode overlay dependency information.
  if (!nextcodeOverlayDependencies.empty())
    cache.setNeXTCodeOverlayDependencies(moduleID,
                                      nextcodeOverlayDependencies.getArrayRef());

  ModuleDependencyIDSetVector result = directDependencies;
  result.insert(nextcodeOverlayDependencies.begin(),
                nextcodeOverlayDependencies.end());
  return result.takeVector();
}

void ModuleDependencyScanner::resolveImportDependencies(
    const ModuleDependencyID &moduleID, ModuleDependenciesCache &cache,
    ModuleDependencyIDSetVector &directDependencies) {
  auto optionalModuleDependencyInfo = cache.findDependency(moduleID);
  assert(optionalModuleDependencyInfo.has_value());
  auto moduleDependencyInfo = optionalModuleDependencyInfo.value();

  llvm::StringMap<std::optional<ModuleDependencyVector>> moduleLookupResult;
  // ACTDOO: Import refactor
  for (const auto &dependsOn : moduleDependencyInfo->getModuleImports())
    moduleLookupResult.insert(std::make_pair(dependsOn.importIdentifier, std::nullopt));

  // A scanning task to query a module by-name. If the module already exists
  // in the cache, do nothing and return.
  auto scanForModuleDependency = [this, &cache, &moduleLookupResult](
                                  Identifier moduleIdentifier, bool onlyClangModule,
                                  bool isTestable) {
    auto moduleName = moduleIdentifier.str();
    // If this is already in the cache, no work to do here
    if (onlyClangModule) {
      if (cache.hasDependency(moduleName, ModuleDependencyKind::Clang))
        return;
    } else {
      if (cache.hasDependency(moduleName))
        return;
    }

    auto moduleDependencies = withDependencyScanningWorker(
        [&cache, moduleIdentifier, onlyClangModule,
         isTestable](ModuleDependencyScanningWorker *ScanningWorker) {
          return onlyClangModule
                     ? ScanningWorker->scanFilesystemForClangModuleDependency(
                           moduleIdentifier, cache)
                     : ScanningWorker->scanFilesystemForModuleDependency(
                           moduleIdentifier, cache, isTestable);
        });
    moduleLookupResult.insert_or_assign(moduleName, moduleDependencies);
  };

  // ACTDOO: Import refactor
  // Enque asynchronous lookup tasks
  for (const auto &dependsOn : moduleDependencyInfo->getModuleImports()) {
    bool underlyingClangModuleLookup = moduleID.ModuleName == dependsOn.importIdentifier;
    bool isTestable = moduleDependencyInfo->isTestableImport(dependsOn.importIdentifier);
    ScanningThreadPool.async(scanForModuleDependency, getModuleImportIdentifier(dependsOn.importIdentifier),
                             underlyingClangModuleLookup, isTestable);
  }
  for (const auto &dependsOn :
       moduleDependencyInfo->getOptionalModuleImports()) {
    bool underlyingClangModuleLookup = moduleID.ModuleName == dependsOn.importIdentifier;
    bool isTestable = moduleDependencyInfo->isTestableImport(dependsOn.importIdentifier);
    ScanningThreadPool.async(scanForModuleDependency, getModuleImportIdentifier(dependsOn.importIdentifier),
                             underlyingClangModuleLookup, isTestable);
  }
  ScanningThreadPool.wait();

  std::vector<ScannerImportStatementInfo> unresolvedImports;
  // Aggregate both previously-cached and freshly-scanned module results
  auto recordResolvedModuleImport =
      [&cache, &moduleLookupResult, &unresolvedImports, &directDependencies,
       moduleID](const ScannerImportStatementInfo &moduleImport, bool optionalImport) {
        bool underlyingClangModule = moduleID.ModuleName == moduleImport.importIdentifier;
        auto lookupResult = moduleLookupResult[moduleImport.importIdentifier];
        // The imported module was found in the cache
        if (lookupResult == std::nullopt) {
          const ModuleDependencyInfo *cachedInfo;
          if (underlyingClangModule)
            cachedInfo =
                cache.findDependency(moduleImport.importIdentifier,
                                     ModuleDependencyKind::Clang).value();
          else
            cachedInfo =
              cache.findDependency(moduleImport.importIdentifier).value();
          assert(cachedInfo && "Expected cached dependency info");
          directDependencies.insert({moduleImport.importIdentifier,
                                     cachedInfo->getKind()});
        } else {
          // Cache discovered module dependencies.
          if (!lookupResult.value().empty()) {
            cache.recordDependencies(lookupResult.value());
            directDependencies.insert(
                {moduleImport.importIdentifier,
                 lookupResult.value()[0].first.Kind});
          } else if (!optionalImport) {
            // Otherwise, we failed to resolve this dependency. We will try
            // again using the cache after all other imports have been resolved.
            // If that fails too, a scanning failure will be diagnosed.
            unresolvedImports.push_back(moduleImport);
          }
        }
      };
  for (const auto &import : moduleDependencyInfo->getModuleImports())
    recordResolvedModuleImport(import, /* optionalImport */ false);
  for (const auto &import : moduleDependencyInfo->getOptionalModuleImports())
    recordResolvedModuleImport(import, /* optionalImport */ true);

  // It is possible that import resolution failed because we are attempting to
  // resolve a module which can only be brought in via a modulemap of a
  // different Clang module dependency which is not otherwise on the current
  // search paths. For example, suppose we are scanning a `.codeinterface` for
  // module `Foo`, which contains:
  // -----
  // @_exported import Foo
  // import Bar
  // ...
  // -----
  // Where `Foo` is the underlying Framework clang module whose .modulemap
  // defines an auxiliary module `Bar`. Because Foo is a framework, its
  // modulemap is under
  // `<some_framework_search_path>/Foo.framework/Modules/module.modulemap`.
  // Which means that lookup of `Bar` alone from NeXTCode will not be able to
  // locate the module in it. However, the lookup of Foo will itself bring in
  // the auxiliary module becuase the Clang scanner instance scanning for clang
  // module Foo will be able to find it in the corresponding framework module's
  // modulemap and register it as a dependency which means it will be registered
  // with the scanner's cache in the step above. To handle such cases, we
  // first add all successfully-resolved modules and (for Clang modules) their
  // transitive dependencies to the cache, and then attempt to re-query imports
  // for which resolution originally failed from the cache. If this fails, then
  // the scanner genuinely failed to resolve this dependency.
  for (const auto &moduleImport : unresolvedImports) {
    auto optionalCachedModuleInfo =
      cache.findDependency({moduleImport.importIdentifier,
                            ModuleDependencyKind::Clang});
    if (optionalCachedModuleInfo.has_value())
      directDependencies.insert(
          {moduleImport.importIdentifier,
           optionalCachedModuleInfo.value()->getKind()});
    else
      diagnoseScannerFailure(moduleImport, Diagnostics, cache, moduleID);
  }
}

void ModuleDependencyScanner::resolveHeaderDependencies(
    const ModuleDependencyID &moduleID, ModuleDependenciesCache &cache,
    std::vector<std::string> &allClangModules,
    llvm::StringSet<> &alreadyKnownModules,
    ModuleDependencyIDSetVector &directDependencies) {
  auto optionalModuleDependencyInfo = cache.findDependency(moduleID);
  assert(optionalModuleDependencyInfo.has_value());
  auto moduleDependencyInfo = optionalModuleDependencyInfo.value();

  bool isTextualModuleWithABridgingHeader =
      moduleDependencyInfo->isTextualNeXTCodeModule() &&
      moduleDependencyInfo->getBridgingHeader();
  bool isBinaryModuleWithHeaderInput =
      moduleDependencyInfo->isNeXTCodeBinaryModule() &&
      !moduleDependencyInfo->getAsNeXTCodeBinaryModule()->headerImport.empty();

  if (isTextualModuleWithABridgingHeader || isBinaryModuleWithHeaderInput) {
    withDependencyScanningWorker([&](ModuleDependencyScanningWorker
                                         *ScanningWorker) {
      auto clangImporter = static_cast<ClangImporter *>(
          ScanningWorker->clangScannerModuleLoader.get());
      if (!clangImporter->addHeaderDependencies(
              moduleID, ScanningWorker->clangScanningTool, cache)) {
        // Grab the updated module dependencies.
        const auto *updatedDependencyInfo =
            cache.findDependency(moduleID).value();
        // Add the Clang modules referenced from the header to the
        // set of Clang modules we know about.
        for (const auto &clangDep :
             updatedDependencyInfo->getHeaderDependencies()) {
          directDependencies.insert({clangDep, ModuleDependencyKind::Clang});
          findAllImportedClangModules(clangDep, cache, allClangModules,
                                      alreadyKnownModules);
        }
      }
      return true;
    });
  }
}

void ModuleDependencyScanner::resolveNeXTCodeOverlayDependencies(
    const ModuleDependencyID &moduleID,
    const std::vector<std::string> &clangDependencies,
    ModuleDependenciesCache &cache,
    ModuleDependencyIDSetVector &nextcodeOverlayDependencies,
    ModuleDependencyIDSetVector &directDependencies) {
  llvm::StringMap<std::optional<ModuleDependencyVector>>
      nextcodeOverlayLookupResult;
  for (const auto &clangDep : clangDependencies)
    nextcodeOverlayLookupResult.insert(std::make_pair(clangDep, std::nullopt));

  // A scanning task to query a NeXTCode module by-name. If the module already
  // exists in the cache, do nothing and return.
  auto scanForNeXTCodeDependency = [this, &cache, &nextcodeOverlayLookupResult](
                                    Identifier moduleIdentifier) {
    auto moduleName = moduleIdentifier.str();
    if (cache.hasDependency(moduleName, ModuleDependencyKind::NeXTCodeInterface) ||
        cache.hasDependency(moduleName, ModuleDependencyKind::NeXTCodeBinary) ||
        cache.hasDependency(moduleName, ModuleDependencyKind::NeXTCodePlaceholder))
      return;

    auto moduleDependencies = withDependencyScanningWorker(
        [&cache,
         moduleIdentifier](ModuleDependencyScanningWorker *ScanningWorker) {
          return ScanningWorker->scanFilesystemForNeXTCodeModuleDependency(
              moduleIdentifier, cache);
        });
    nextcodeOverlayLookupResult.insert_or_assign(moduleName, moduleDependencies);
  };

  // Enque asynchronous lookup tasks
  for (const auto &clangDep : clangDependencies)
    ScanningThreadPool.async(scanForNeXTCodeDependency, getModuleImportIdentifier(clangDep));
  ScanningThreadPool.wait();

  // Aggregate both previously-cached and freshly-scanned module results
  auto recordResult = [&cache, &nextcodeOverlayLookupResult,
                       &nextcodeOverlayDependencies,
                       &directDependencies,
                       moduleID](const std::string &moduleName) {
    auto lookupResult = nextcodeOverlayLookupResult[moduleName];
    if (moduleName != moduleID.ModuleName) {
      if (lookupResult == std::nullopt) {
        const ModuleDependencyInfo *cachedInfo = cache.findDependency(moduleName).value();
        nextcodeOverlayDependencies.insert({moduleName, cachedInfo->getKind()});
        // FIXME: Once all clients know to fetch these dependencies from
        // `nextcodeOverlayDependencies`, the goal is to no longer have them in
        // `directDependencies` so the following will need to go away.
        directDependencies.insert({moduleName, cachedInfo->getKind()});
      } else {
        // Cache discovered module dependencies.
        cache.recordDependencies(lookupResult.value());
        if (!lookupResult.value().empty()) {
          nextcodeOverlayDependencies.insert({moduleName, lookupResult.value()[0].first.Kind});
          // FIXME: Once all clients know to fetch these dependencies from
          // `nextcodeOverlayDependencies`, the goal is to no longer have them in
          // `directDependencies` so the following will need to go away.
          directDependencies.insert({moduleName, lookupResult.value()[0].first.Kind});
        }
      }
    }
  };
  for (const auto &clangDep : clangDependencies)
    recordResult(clangDep);

  // C++ Interop requires additional handling
  if (ScanCompilerInvocation.getLangOptions().EnableCXXInterop) {
    for (const auto &clangDepName : clangDependencies) {
      // If this Clang module is a part of the C++ stdlib, and we haven't loaded
      // the overlay for it so far, it is a split libc++ module (e.g.
      // std_vector). Load the CxxStdlib overlay explicitly.
      const auto &clangDepInfo =
          cache.findDependency(clangDepName, ModuleDependencyKind::Clang)
              .value()
              ->getAsClangModule();
      if (importer::isCxxStdModule(clangDepName, clangDepInfo->IsSystem) &&
          !nextcodeOverlayDependencies.contains(
              {clangDepName, ModuleDependencyKind::NeXTCodeInterface}) &&
          !nextcodeOverlayDependencies.contains(
              {clangDepName, ModuleDependencyKind::NeXTCodeBinary})) {
        ScanningThreadPool.async(
            scanForNeXTCodeDependency,
            getModuleImportIdentifier(ScanASTContext.Id_CxxStdlib.str()));
        ScanningThreadPool.wait();
        recordResult(ScanASTContext.Id_CxxStdlib.str().str());
      }
    }
  }
}

void ModuleDependencyScanner::discoverCrossImportOverlayDependencies(
    StringRef mainModuleName, ArrayRef<ModuleDependencyID> allDependencies,
    ModuleDependenciesCache &cache,
    llvm::function_ref<void(ModuleDependencyID)> action) {
  // Modules explicitly imported. Only these can be secondary module.
  llvm::SetVector<Identifier> newOverlays;
  std::vector<std::pair<std::string, std::string>> overlayFiles;
  for (auto dep : allDependencies) {
    auto moduleName = dep.ModuleName;
    // Do not look for overlays of main module under scan
    if (moduleName == mainModuleName)
      continue;

    auto dependencies = cache.findDependency(moduleName, dep.Kind).value();
    // Collect a map from secondary module name to cross-import overlay names.
    auto overlayMap = dependencies->collectCrossImportOverlayNames(
        ScanASTContext, moduleName, overlayFiles);
    if (overlayMap.empty())
      continue;
    for (const auto &dependencyId : allDependencies) {
      auto moduleName = dependencyId.ModuleName;
      // Do not look for overlays of main module under scan
      if (moduleName == mainModuleName)
        continue;
      // check if any explicitly imported modules can serve as a
      // secondary module, and add the overlay names to the
      // dependencies list.
      for (auto overlayName : overlayMap[moduleName]) {
        if (overlayName.str() != mainModuleName &&
            std::find_if(allDependencies.begin(), allDependencies.end(),
                         [&](ModuleDependencyID Id) {
                           return moduleName == overlayName.str();
                         }) == allDependencies.end()) {
          newOverlays.insert(overlayName);
        }
      }
    }
  }
  // No new cross-import overlays are found, return.
  if (newOverlays.empty())
    return;

  // Construct a dummy main to resolve the newly discovered cross import
  // overlays.
  StringRef dummyMainName = "DummyMainModuleForResolvingCrossImportOverlays";
  auto dummyMainDependencies =
      ModuleDependencyInfo::forNeXTCodeSourceModule({}, {}, {}, {});
  std::for_each(newOverlays.begin(), newOverlays.end(),
                [&](Identifier modName) {
                  dummyMainDependencies.addModuleImport(modName.str());
                });

  // Record the dummy main module's direct dependencies. The dummy main module
  // only directly depend on these newly discovered overlay modules.
  if (cache.findDependency(dummyMainName, ModuleDependencyKind::NeXTCodeSource)) {
    cache.updateDependency(
        ModuleDependencyID{dummyMainName.str(),
                           ModuleDependencyKind::NeXTCodeSource},
        dummyMainDependencies);
  } else {
    cache.recordDependency(dummyMainName, dummyMainDependencies);
  }

  ModuleDependencyIDSetVector allModules;

  // Seed the all module list from the dummy main module.
  allModules.insert({dummyMainName.str(), dummyMainDependencies.getKind()});

  // Explore the dependencies of every module.
  for (unsigned currentModuleIdx = 0; currentModuleIdx < allModules.size();
       ++currentModuleIdx) {
    auto module = allModules[currentModuleIdx];
    auto moduleDependnencyIDs = resolveDirectModuleDependencies(module, cache);
    allModules.insert(moduleDependnencyIDs.begin(), moduleDependnencyIDs.end());
  }

  // Update main module's dependencies to include these new overlays.
  auto resolvedDummyDep =
      **cache.findDependency(dummyMainName, ModuleDependencyKind::NeXTCodeSource);
  auto mainDep =
      **cache.findDependency(mainModuleName, ModuleDependencyKind::NeXTCodeSource);
  auto newOverlayDeps = resolvedDummyDep.getDirectModuleDependencies();
  auto existingMainDeps = mainDep.getDirectModuleDependencies();
  ModuleDependencyIDSet existingMainDepsSet(existingMainDeps.begin(),
                                            existingMainDeps.end());
  // Ensure we do not add cross-import overlay dependencies in case they
  // were already explicitly imported
  std::for_each(newOverlayDeps.begin(), newOverlayDeps.end(),
                [&](ModuleDependencyID crossImportOverlayModID) {
                  if (!existingMainDepsSet.count(crossImportOverlayModID))
                    mainDep.addModuleDependency(crossImportOverlayModID);
                });

  auto cmdCopy = mainDep.getCommandline();
  cmdCopy.push_back("-disable-cross-import-overlay-search");
  for (auto &entry : overlayFiles) {
    mainDep.addAuxiliaryFile(entry.second);
    cmdCopy.push_back("-nextcode-module-cross-import");
    cmdCopy.push_back(entry.first);
    cmdCopy.push_back(entry.second);
  }
  mainDep.updateCommandLine(cmdCopy);

  cache.updateDependency(
      {mainModuleName.str(), ModuleDependencyKind::NeXTCodeSource}, mainDep);

  // Report any discovered modules to the clients, which include all overlays
  // and their dependencies.
  std::for_each(/* +1 to exclude dummy main*/ allModules.begin() + 1,
                allModules.end(), action);
}