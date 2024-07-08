//===--- ScanDependencies.cpp -- Scans the dependencies of a module -------===//
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

#include "nextcode/Basic/PrettyStackTrace.h"

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/AST/DiagnosticsDriver.h"
#include "nextcode/AST/DiagnosticsFrontend.h"
#include "nextcode/AST/DiagnosticsSema.h"
#include "nextcode/AST/FileSystem.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/ModuleDependencies.h"
#include "nextcode/AST/ModuleLoader.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/Defer.h"
#include "nextcode/Basic/FileTypes.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/Basic/STLExtras.h"
#include "nextcode/ClangImporter/ClangImporter.h"
#include "nextcode/DependencyScan/DependencyScanImpl.h"
#include "nextcode/DependencyScan/DependencyScanJSON.h"
#include "nextcode/DependencyScan/DependencyScanningTool.h"
#include "nextcode/DependencyScan/ModuleDependencyScanner.h"
#include "nextcode/DependencyScan/ScanDependencies.h"
#include "nextcode/DependencyScan/SerializedModuleDependencyCacheFormat.h"
#include "nextcode/DependencyScan/StringUtils.h"
#include "nextcode/Frontend/CachingUtils.h"
#include "nextcode/Frontend/CompileJobCacheKey.h"
#include "nextcode/Frontend/CompileJobCacheResult.h"
#include "nextcode/Frontend/Frontend.h"
#include "nextcode/Frontend/FrontendOptions.h"
#include "nextcode/Frontend/ModuleInterfaceLoader.h"
#include "nextcode/Strings.h"
#include "clang/Basic/Module.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SetOperations.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/CAS/ActionCache.h"
#include "llvm/CAS/CASReference.h"
#include "llvm/CAS/ObjectStore.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/StringSaver.h"
#include "llvm/Support/VirtualOutputBackend.h"
#include "llvm/Support/YAMLParser.h"
#include "llvm/Support/YAMLTraits.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <algorithm>

using namespace nextcode;
using namespace nextcode::dependencies;
using namespace nextcode::c_string_utils;
using namespace llvm::yaml;

namespace {

static std::string getScalaNodeText(Node *N) {
  SmallString<32> Buffer;
  return cast<ScalarNode>(N)->getValue(Buffer).str();
}

/// Parse an entry like this, where the "platforms" key-value pair is optional:
///  {
///     "nextcodeModuleName": "Foo",
///     "arguments": "-target 10.15",
///     "output": "../Foo.json"
///  },
static bool parseBatchInputEntries(ASTContext &Ctx, llvm::StringSaver &saver,
                                   Node *Node,
                                   std::vector<BatchScanInput> &result) {
  auto *SN = cast<SequenceNode>(Node);
  if (!SN)
    return true;
  for (auto It = SN->begin(); It != SN->end(); ++It) {
    auto *MN = cast<MappingNode>(&*It);
    BatchScanInput entry;
    std::optional<std::set<int8_t>> Platforms;
    for (auto &Pair : *MN) {
      auto Key = getScalaNodeText(Pair.getKey());
      auto *Value = Pair.getValue();
      if (Key == "clangModuleName") {
        entry.moduleName = saver.save(getScalaNodeText(Value));
        entry.isNeXTCode = false;
      } else if (Key == "nextcodeModuleName") {
        entry.moduleName = saver.save(getScalaNodeText(Value));
        entry.isNeXTCode = true;
      } else if (Key == "arguments") {
        entry.arguments = saver.save(getScalaNodeText(Value));
      } else if (Key == "output") {
        entry.outputPath = saver.save(getScalaNodeText(Value));
      } else {
        // Future proof.
        continue;
      }
    }
    if (entry.moduleName.empty())
      return true;
    if (entry.outputPath.empty())
      return true;
    result.emplace_back(std::move(entry));
  }
  return false;
}

static std::optional<std::vector<BatchScanInput>>
parseBatchScanInputFile(ASTContext &ctx, StringRef batchInputPath,
                        llvm::StringSaver &saver) {
  assert(!batchInputPath.empty());
  namespace yaml = llvm::yaml;
  std::vector<BatchScanInput> result;

  // Load the input file.
  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> FileBufOrErr =
      llvm::MemoryBuffer::getFile(batchInputPath);
  if (!FileBufOrErr) {
    ctx.Diags.diagnose(SourceLoc(), diag::batch_scan_input_file_missing,
                       batchInputPath);
    return std::nullopt;
  }
  StringRef Buffer = FileBufOrErr->get()->getBuffer();

  // Use a new source manager instead of the one from ASTContext because we
  // don't want the Json file to be persistent.
  SourceManager SM;
  yaml::Stream Stream(llvm::MemoryBufferRef(Buffer, batchInputPath),
                      SM.getLLVMSourceMgr());
  for (auto DI = Stream.begin(); DI != Stream.end(); ++DI) {
    assert(DI != Stream.end() && "Failed to read a document");
    yaml::Node *N = DI->getRoot();
    assert(N && "Failed to find a root");
    if (parseBatchInputEntries(ctx, saver, N, result)) {
      ctx.Diags.diagnose(SourceLoc(), diag::batch_scan_input_file_corrupted,
                         batchInputPath);
      return std::nullopt;
    }
  }
  return result;
}

static llvm::Expected<llvm::cas::ObjectRef>
updateModuleCacheKey(ModuleDependencyInfo &depInfo,
                     ModuleDependenciesCache &cache,
                     llvm::cas::ObjectStore &CAS) {
  auto commandLine = depInfo.getCommandline();
  std::vector<const char *> Args;
  if (commandLine.size() > 1)
    for (auto &c : ArrayRef<std::string>(commandLine).drop_front(1))
      Args.push_back(c.c_str());

  auto base = createCompileJobBaseCacheKey(CAS, Args);
  if (!base)
    return base.takeError();

  std::string InputPath;
  if (auto *dep = depInfo.getAsClangModule())
    InputPath = dep->moduleMapFile;
  else if (auto *dep = depInfo.getAsNeXTCodeInterfaceModule())
    InputPath = dep->nextcodeInterfaceFile;
  else
    llvm_unreachable("Unhandled dependency kind");

  if (cache.getScanService().hasPathMapping())
    InputPath = cache.getScanService().remapPath(InputPath);

  // Module compilation commands always have only one input and the input
  // index is always 0.
  auto key = createCompileJobCacheKeyForOutput(CAS, *base, /*InputIndex=*/0);
  if (!key)
    return key.takeError();

  depInfo.updateModuleCacheKey(CAS.getID(*key).toString());
  return *key;
}

static llvm::Error resolveExplicitModuleInputs(
    ModuleDependencyID moduleID,
    const std::set<ModuleDependencyID> &dependencies,
    ModuleDependenciesCache &cache, CompilerInstance &instance,
    std::optional<std::set<ModuleDependencyID>> bridgingHeaderDeps) {
  // Only need to resolve dependency for following dependencies.
  if (moduleID.Kind == ModuleDependencyKind::NeXTCodePlaceholder)
    return llvm::Error::success();

  auto &resolvingDepInfo = cache.findKnownDependency(moduleID);
  // If the dependency is already finalized, nothing needs to be done.
  if (resolvingDepInfo.isFinalized())
    return llvm::Error::success();

  auto &service = cache.getScanService();
  auto remapPath = [&](StringRef path) { return service.remapPath(path); };
  std::vector<std::string> rootIDs;
  std::vector<std::string> includeTrees;
  if (auto ID = resolvingDepInfo.getClangIncludeTree())
    includeTrees.push_back(*ID);

  auto tracker = cache.getScanService().createNeXTCodeDependencyTracker();
  auto addBridgingHeaderDeps =
      [&](const ModuleDependencyInfo &depInfo) -> llvm::Error {
    auto sourceDepDetails = depInfo.getAsNeXTCodeSourceModule();
    if (!sourceDepDetails)
      return llvm::Error::success();

    if (sourceDepDetails->textualModuleDetails
            .CASBridgingHeaderIncludeTreeRootID.empty()) {
      if (!sourceDepDetails->textualModuleDetails.bridgingSourceFiles.empty()) {
        if (tracker) {
          tracker->startTracking();
          for (auto &file :
               sourceDepDetails->textualModuleDetails.bridgingSourceFiles)
            tracker->trackFile(file);
          auto bridgeRoot = tracker->createTreeFromDependencies();
          if (!bridgeRoot)
            return bridgeRoot.takeError();
          rootIDs.push_back(bridgeRoot->getID().toString());
        }
      }
    } else
      includeTrees.push_back(sourceDepDetails->textualModuleDetails
                                 .CASBridgingHeaderIncludeTreeRootID);
    return llvm::Error::success();
  };
  if (auto E = addBridgingHeaderDeps(resolvingDepInfo))
    return E;

  std::vector<std::string> commandLine = resolvingDepInfo.getCommandline();
  auto dependencyInfoCopy = resolvingDepInfo;
  for (const auto &depModuleID : dependencies) {
    const auto &depInfo = cache.findKnownDependency(depModuleID);
    switch (depModuleID.Kind) {
    case nextcode::ModuleDependencyKind::NeXTCodeInterface: {
      auto interfaceDepDetails = depInfo.getAsNeXTCodeInterfaceModule();
      assert(interfaceDepDetails && "Expected NeXTCode Interface dependency.");
      auto &path = interfaceDepDetails->moduleCacheKey.empty()
                       ? interfaceDepDetails->moduleOutputPath
                       : interfaceDepDetails->moduleCacheKey;
      commandLine.push_back("-nextcode-module-file=" + depModuleID.ModuleName + "=" +
                            path);
      // Add the exported macro from interface into current module.
      llvm::for_each(
          interfaceDepDetails->textualModuleDetails.macroDependencies,
          [&](const auto &entry) {
            dependencyInfoCopy.addMacroDependency(entry.first(),
                                                  entry.second.LibraryPath,
                                                  entry.second.ExecutablePath);
          });
    } break;
    case nextcode::ModuleDependencyKind::NeXTCodeBinary: {
      auto binaryDepDetails = depInfo.getAsNeXTCodeBinaryModule();
      assert(binaryDepDetails && "Expected NeXTCode Binary Module dependency.");
      auto &path = binaryDepDetails->moduleCacheKey.empty()
                       ? binaryDepDetails->compiledModulePath
                       : binaryDepDetails->moduleCacheKey;
      commandLine.push_back("-nextcode-module-file=" + depModuleID.ModuleName + "=" +
                            path);
      // If this binary module was built with a header, the header's module
      // dependencies must also specify a .modulemap to the compilation, in
      // order to resolve the header's own header include directives.
      for (const auto &bridgingHeaderDepName :
           binaryDepDetails->headerModuleDependencies) {
        auto optionalBridgingHeaderDepModuleInfo = cache.findKnownDependency(
            {bridgingHeaderDepName, ModuleDependencyKind::Clang});
        const auto bridgingHeaderDepModuleDetails =
          optionalBridgingHeaderDepModuleInfo.getAsClangModule();
	commandLine.push_back("-Xcc");
        commandLine.push_back(
            "-fmodule-map-file=" +
            remapPath(bridgingHeaderDepModuleDetails->moduleMapFile));
      }
    } break;
    case nextcode::ModuleDependencyKind::NeXTCodePlaceholder: {
      auto placeholderDetails = depInfo.getAsPlaceholderDependencyModule();
      assert(placeholderDetails && "Expected NeXTCode Placeholder dependency.");
      commandLine.push_back("-nextcode-module-file=" + depModuleID.ModuleName + "=" +
                            placeholderDetails->compiledModulePath);
    } break;
    case nextcode::ModuleDependencyKind::Clang: {
      auto clangDepDetails = depInfo.getAsClangModule();
      assert(clangDepDetails && "Expected Clang Module dependency.");
      if (!resolvingDepInfo.isClangModule()) {
        commandLine.push_back("-Xcc");
        commandLine.push_back("-fmodule-file=" + depModuleID.ModuleName + "=" +
                              clangDepDetails->mappedPCMPath);
      }
      if (!clangDepDetails->moduleCacheKey.empty()) {
        commandLine.push_back("-Xcc");
        commandLine.push_back("-fmodule-file-cache-key");
        commandLine.push_back("-Xcc");
        commandLine.push_back(clangDepDetails->mappedPCMPath);
        commandLine.push_back("-Xcc");
        commandLine.push_back(clangDepDetails->moduleCacheKey);
      }

      // Only need to merge the CASFS from clang importer.
      if (auto ID = depInfo.getCASFSRootID())
        rootIDs.push_back(*ID);
      if (auto ID = depInfo.getClangIncludeTree())
        includeTrees.push_back(*ID);
    } break;
    case nextcode::ModuleDependencyKind::NeXTCodeSource: {
      if (auto E = addBridgingHeaderDeps(depInfo))
        return E;
      break;
    }
    default:
      llvm_unreachable("Unhandled dependency kind.");
    }
  }

  // Update the dependency in the cache with the modified command-line.
  if (resolvingDepInfo.isNeXTCodeInterfaceModule() ||
      resolvingDepInfo.isClangModule()) {
    if (service.hasPathMapping())
      commandLine = remapPathsFromCommandLine(commandLine, remapPath);
    dependencyInfoCopy.updateCommandLine(commandLine);
  }

  // Handle CAS options.
  if (instance.getInvocation().getCASOptions().EnableCaching) {
    // Merge CASFS from clang dependency.
    auto &CASFS = cache.getScanService().getSharedCachingFS();
    auto &CAS = CASFS.getCAS();

    assert(tracker && "no caching tracker is available");
    // Compute the CASFS root ID for the resolving dependency.
    if (auto *sourceDep = resolvingDepInfo.getAsNeXTCodeSourceModule()) {
      tracker->startTracking();
      tracker->addCommonSearchPathDeps(
          instance.getInvocation().getSearchPathOptions());
      llvm::for_each(
          sourceDep->sourceFiles,
          [&tracker](const std::string &file) { tracker->trackFile(file); });
      llvm::for_each(
          sourceDep->auxiliaryFiles,
          [&tracker](const std::string &file) { tracker->trackFile(file); });
      llvm::for_each(sourceDep->textualModuleDetails.macroDependencies,
                     [&tracker](const auto &entry) {
                       tracker->trackFile(entry.second.LibraryPath);
                     });
      auto root = tracker->createTreeFromDependencies();
      if (!root)
        return root.takeError();
      auto rootID = root->getID().toString();
      dependencyInfoCopy.updateCASFileSystemRootID(rootID);
      rootIDs.push_back(rootID);
    } else if (auto *textualDep =
                   resolvingDepInfo.getAsNeXTCodeInterfaceModule()) {
      tracker->startTracking();
      tracker->addCommonSearchPathDeps(
          instance.getInvocation().getSearchPathOptions());
      tracker->trackFile(textualDep->nextcodeInterfaceFile);
      llvm::for_each(
          textualDep->auxiliaryFiles,
          [&tracker](const std::string &file) { tracker->trackFile(file); });
      auto root = tracker->createTreeFromDependencies();
      if (!root)
        return root.takeError();
      auto rootID = root->getID().toString();
      dependencyInfoCopy.updateCASFileSystemRootID(rootID);
      rootIDs.push_back(rootID);
    }

    // Update build command line.
    if (resolvingDepInfo.isNeXTCodeInterfaceModule() ||
        resolvingDepInfo.isNeXTCodeSourceModule()) {
      // Update with casfs option.
      std::vector<std::string> newCommandLine =
          dependencyInfoCopy.getCommandline();
      for (auto rootID : rootIDs) {
        newCommandLine.push_back("-cas-fs");
        newCommandLine.push_back(rootID);
      }

      for (auto tree : includeTrees) {
        newCommandLine.push_back("-clang-include-tree-root");
        newCommandLine.push_back(tree);
      }
      dependencyInfoCopy.updateCommandLine(newCommandLine);
    }

    if (bridgingHeaderDeps) {
      std::vector<std::string> newCommandLine =
          dependencyInfoCopy.getBridgingHeaderCommandline();
      for (auto bridgingDep : *bridgingHeaderDeps) {
        auto &dep = cache.findKnownDependency(bridgingDep);
        auto *clangDep = dep.getAsClangModule();
        assert(clangDep && "wrong module dependency kind");
        if (!clangDep->moduleCacheKey.empty()) {
          newCommandLine.push_back("-Xcc");
          newCommandLine.push_back("-fmodule-file-cache-key");
          newCommandLine.push_back("-Xcc");
          newCommandLine.push_back(clangDep->mappedPCMPath);
          newCommandLine.push_back("-Xcc");
          newCommandLine.push_back(clangDep->moduleCacheKey);
        }
      }
      dependencyInfoCopy.updateBridgingHeaderCommandLine(newCommandLine);
    }

    // Compute and update module cache key.
    auto setupBinaryCacheKey = [&](StringRef path) -> llvm::Error {
      // For binary module, we need to make sure the lookup key is setup here in
      // action cache. We just use the CASID of the binary module itself as key.
      auto Ref = CASFS.getObjectRefForFileContent(path);
      if (!Ref)
        return llvm::errorCodeToError(Ref.getError());
      assert(*Ref && "Binary module should be loaded into CASFS already");
      dependencyInfoCopy.updateModuleCacheKey(CAS.getID(**Ref).toString());

      nextcode::cas::CompileJobCacheResult::Builder Builder;
      Builder.addOutput(file_types::ID::TY_NeXTCodeModuleFile, **Ref);
      auto Result = Builder.build(CAS);
      if (!Result)
        return Result.takeError();
      if (auto E = instance.getActionCache().put(CAS.getID(**Ref),
                                                 CAS.getID(*Result)))
        return E;
      return llvm::Error::success();
    };

    if (resolvingDepInfo.isClangModule() ||
        resolvingDepInfo.isNeXTCodeInterfaceModule()) {
      auto Key = updateModuleCacheKey(dependencyInfoCopy, cache, CAS);
      if (!Key)
        return Key.takeError();
    } else if (auto *binaryDep = dependencyInfoCopy.getAsNeXTCodeBinaryModule()) {
      if (auto E = setupBinaryCacheKey(binaryDep->compiledModulePath))
        return E;
    }
  }

  dependencyInfoCopy.setIsFinalized(true);
  cache.updateDependency(moduleID, dependencyInfoCopy);

  return llvm::Error::success();
}

static llvm::Error
pruneUnusedVFSOverlays(ModuleDependencyID moduleID,
                       const std::set<ModuleDependencyID> &dependencies,
                       ModuleDependenciesCache &cache,
                       CompilerInstance &instance) {
  // Pruning of unused VFS overlay options for Clang dependencies
  // is performed by the Clang dependency scanner.
  if (moduleID.Kind == ModuleDependencyKind::Clang)
    return llvm::Error::success();

  auto isVFSOverlayFlag = [](StringRef arg) {
    return arg == "-ivfsoverlay" || arg == "-vfsoverlay";
  };
  auto isXCCArg = [](StringRef arg) {
    return arg == "-Xcc";
  };

  auto &resolvingDepInfo = cache.findKnownDependency(moduleID);
  // If this NeXTCode dependency contains any VFS overlay paths,
  // then attempt to prune the ones not used by any of the Clang dependencies.
  if (!llvm::any_of(resolvingDepInfo.getCommandline(),
                    [&isVFSOverlayFlag](const std::string &arg) {
                      return isVFSOverlayFlag(arg);
                    }))
    return llvm::Error::success();

  // 1. For each Clang dependency, gather its ivfsoverlay path arguments
  // to keep track of which overlays are actually used and were not
  // pruned by the Clang dependency scanner.
  llvm::StringSet<> usedVFSOverlayPaths;
  for (const auto &depModuleID : dependencies) {
    const auto &depInfo = cache.findKnownDependency(depModuleID);
    if (auto clangDepDetails = depInfo.getAsClangModule()) {
      const auto &depCommandLine = clangDepDetails->buildCommandLine;
      // true if the previous argument was the dash-option of an option pair
      bool getNext = false;
      for (const auto &A : depCommandLine) {
        StringRef arg(A);
        if (isXCCArg(arg))
          continue;
        if (getNext) {
          getNext = false;
          usedVFSOverlayPaths.insert(arg);
        } else if (isVFSOverlayFlag(arg))
          getNext = true;
      }
    }
  }

  // 2. Each -Xcc VFS overlay path on the resolving command-line which is not used by
  // any of the Clang dependencies can be removed from the command-line.
  const std::vector<std::string> &currentCommandLine =
      resolvingDepInfo.getCommandline();
  std::vector<std::string> resolvedCommandLine;
  size_t skip = 0;
  for (auto it = currentCommandLine.begin(), end = currentCommandLine.end();
       it != end; it++) {
    if (skip) {
      skip--;
      continue;
    }
    // If this VFS overlay was not used across any of the dependencies, skip it.
    if ((it+1) != end && isXCCArg(*it) && isVFSOverlayFlag(*(it + 1))) {
      assert(it + 2 != end); // Extra -Xcc
      assert(it + 3 != end); // Actual VFS overlay path argument
      if (!usedVFSOverlayPaths.contains(*(it + 3))) {
        skip = 3;
        continue;
      }
    }
    resolvedCommandLine.push_back(*it);
  }

  // 3. Update the dependency in the cache if the command-line has been modified.
  if (currentCommandLine.size() != resolvedCommandLine.size()) {
    auto dependencyInfoCopy = resolvingDepInfo;
    dependencyInfoCopy.updateCommandLine(resolvedCommandLine);

    // Update the CAS cache key for the new command-line
    if (instance.getInvocation().getCASOptions().EnableCaching) {
      auto &CAS = cache.getScanService().getSharedCachingFS().getCAS();
      auto Key = updateModuleCacheKey(dependencyInfoCopy, cache, CAS);
      if (!Key)
        return Key.takeError();
    }
    cache.updateDependency(moduleID, dependencyInfoCopy);
  }

  return llvm::Error::success();
}

static bool writePrescanJSONToOutput(DiagnosticEngine &diags,
                                     llvm::vfs::OutputBackend &backend,
                                     StringRef path,
                                     nextcodescan_import_set_t importSet) {
  return withOutputPath(diags, backend, path, [&](llvm::raw_pwrite_stream &os) {
    writePrescanJSON(os, importSet);
    return false;
  });
}

static bool writeJSONToOutput(DiagnosticEngine &diags,
                              llvm::vfs::OutputBackend &backend, StringRef path,
                              nextcodescan_dependency_graph_t dependencies) {
  return withOutputPath(diags, backend, path, [&](llvm::raw_pwrite_stream &os) {
    writeJSON(os, dependencies);
    return false;
  });
}

static void bridgeDependencyIDs(const ArrayRef<ModuleDependencyID> dependencies,
                                std::vector<std::string> &bridgedDependencyNames) {
  for (const auto &dep : dependencies) {
    std::string dependencyKindAndName;
    switch (dep.Kind) {
    case ModuleDependencyKind::NeXTCodeInterface:
    case ModuleDependencyKind::NeXTCodeSource:
      dependencyKindAndName = "nextcodeTextual";
      break;
    case ModuleDependencyKind::NeXTCodeBinary:
      dependencyKindAndName = "nextcodeBinary";
      break;
    case ModuleDependencyKind::NeXTCodePlaceholder:
      dependencyKindAndName = "nextcodePlaceholder";
      break;
    case ModuleDependencyKind::Clang:
      dependencyKindAndName = "clang";
      break;
    default:
      llvm_unreachable("Unhandled dependency kind.");
    }
    dependencyKindAndName += ":";
    dependencyKindAndName += dep.ModuleName;
    bridgedDependencyNames.push_back(dependencyKindAndName);
  }
}

static nextcodescan_diagnostic_set_t *mapCollectedDiagnosticsForOutput(
    const SourceManager &SM,
    const DependencyScanDiagnosticCollector *diagnosticCollector) {
  auto collectedDiagnostics = diagnosticCollector->getDiagnostics();
  auto numDiagnostics = collectedDiagnostics.size();
  nextcodescan_diagnostic_set_t *diagnosticOutput = new nextcodescan_diagnostic_set_t;
  diagnosticOutput->count = numDiagnostics;
  diagnosticOutput->diagnostics =
      new nextcodescan_diagnostic_info_t[numDiagnostics];
  for (size_t i = 0; i < numDiagnostics; ++i) {
    const auto &Diagnostic = collectedDiagnostics[i];
    nextcodescan_diagnostic_info_s *diagnosticInfo =
        new nextcodescan_diagnostic_info_s;
    diagnosticInfo->message =
        nextcode::c_string_utils::create_clone(Diagnostic.Message.c_str());
    switch (Diagnostic.Severity) {
    case llvm::SourceMgr::DK_Error:
      diagnosticInfo->severity = NEXTCODESCAN_DIAGNOSTIC_SEVERITY_ERROR;
      break;
    case llvm::SourceMgr::DK_Warning:
      diagnosticInfo->severity = NEXTCODESCAN_DIAGNOSTIC_SEVERITY_WARNING;
      break;
    case llvm::SourceMgr::DK_Note:
      diagnosticInfo->severity = NEXTCODESCAN_DIAGNOSTIC_SEVERITY_NOTE;
      break;
    case llvm::SourceMgr::DK_Remark:
      diagnosticInfo->severity = NEXTCODESCAN_DIAGNOSTIC_SEVERITY_REMARK;
      break;
    }

    if (Diagnostic.ImportLocation.has_value()) {
      auto importLocation = Diagnostic.ImportLocation.value();
      nextcodescan_source_location_s *sourceLoc = new nextcodescan_source_location_s;
      if (importLocation.bufferIdentifier.empty())
        sourceLoc->buffer_identifier = nextcode::c_string_utils::create_null();
      else
        sourceLoc->buffer_identifier = nextcode::c_string_utils::create_clone(
            importLocation.bufferIdentifier.c_str());
      sourceLoc->line_number = importLocation.lineNumber;
      sourceLoc->column_number = importLocation.columnNumber;
      diagnosticInfo->source_location = sourceLoc;
    } else {
      diagnosticInfo->source_location = nullptr;
    }

    diagnosticOutput->diagnostics[i] = diagnosticInfo;
  }
  return diagnosticOutput;
}

static nextcodescan_dependency_graph_t
generateFullDependencyGraph(const CompilerInstance &instance,
                            const DependencyScanDiagnosticCollector *diagnosticCollector,
                            const ModuleDependenciesCache &cache,
                            const ArrayRef<ModuleDependencyID> allModules) {
  if (allModules.empty()) {
    return nullptr;
  }

  std::string mainModuleName = allModules.front().ModuleName;
  nextcodescan_dependency_set_t *dependencySet = new nextcodescan_dependency_set_t;
  dependencySet->count = allModules.size();
  dependencySet->modules =
      new nextcodescan_dependency_info_t[dependencySet->count];

  for (size_t i = 0; i < allModules.size(); ++i) {
    const auto &module = allModules[i];
    auto &moduleDeps = cache.findKnownDependency(module);
    // Collect all the required pieces to build a ModuleInfo
    auto nextcodePlaceholderDeps = moduleDeps.getAsPlaceholderDependencyModule();
    auto nextcodeTextualDeps = moduleDeps.getAsNeXTCodeInterfaceModule();
    auto nextcodeSourceDeps = moduleDeps.getAsNeXTCodeSourceModule();
    auto nextcodeBinaryDeps = moduleDeps.getAsNeXTCodeBinaryModule();
    auto clangDeps = moduleDeps.getAsClangModule();

    // ModulePath
    const char *modulePathSuffix =
        moduleDeps.isNeXTCodeModule() ? ".codemodule" : ".pcm";
    std::string modulePath;
    if (nextcodeTextualDeps)
      modulePath = nextcodeTextualDeps->moduleOutputPath;
    else if (nextcodePlaceholderDeps)
      modulePath = nextcodePlaceholderDeps->compiledModulePath;
    else if (nextcodeBinaryDeps)
      modulePath = nextcodeBinaryDeps->compiledModulePath;
    else if (clangDeps)
      modulePath = clangDeps->pcmOutputPath;
    else
      modulePath = module.ModuleName + modulePathSuffix;

    // SourceFiles
    std::vector<std::string> sourceFiles;
    if (nextcodeSourceDeps) {
      sourceFiles = nextcodeSourceDeps->sourceFiles;
    } else if (clangDeps) {
      sourceFiles = clangDeps->fileDependencies;
    }

    auto &depInfo = cache.findKnownDependency(module);
    auto directDependencies = depInfo.getDirectModuleDependencies();

    // Generate a nextcodescan_clang_details_t object based on the dependency kind
    auto getModuleDetails = [&]() -> nextcodescan_module_details_t {
      nextcodescan_module_details_s *details = new nextcodescan_module_details_s;
      if (nextcodeTextualDeps) {
        nextcodescan_string_ref_t moduleInterfacePath =
            create_clone(nextcodeTextualDeps->nextcodeInterfaceFile.c_str());
        nextcodescan_string_ref_t bridgingHeaderPath =
            nextcodeTextualDeps->textualModuleDetails.bridgingHeaderFile
                    .has_value()
                ? create_clone(
                      nextcodeTextualDeps->textualModuleDetails.bridgingHeaderFile
                          .value()
                          .c_str())
                : create_null();
        details->kind = NEXTCODESCAN_DEPENDENCY_INFO_NEXTCODE_TEXTUAL;
        // Create an overlay dependencies set according to the output format
        std::vector<std::string> bridgedOverlayDependencyNames;
        bridgeDependencyIDs(nextcodeTextualDeps->nextcodeOverlayDependencies,
                            bridgedOverlayDependencyNames);

        details->nextcode_textual_details = {
            moduleInterfacePath,
            create_set(nextcodeTextualDeps->compiledModuleCandidates),
            bridgingHeaderPath,
            create_set(
                nextcodeTextualDeps->textualModuleDetails.bridgingSourceFiles),
            create_set(nextcodeTextualDeps->textualModuleDetails
                           .bridgingModuleDependencies),
            create_set(bridgedOverlayDependencyNames),
            create_set(nextcodeTextualDeps->textualModuleDetails.buildCommandLine),
            /*bridgingHeaderBuildCommand*/ create_set({}),
            create_set(nextcodeTextualDeps->textualModuleDetails.extraPCMArgs),
            create_clone(nextcodeTextualDeps->contextHash.c_str()),
            nextcodeTextualDeps->isFramework,
            nextcodeTextualDeps->isStatic,
            create_clone(nextcodeTextualDeps->textualModuleDetails
                             .CASFileSystemRootID.c_str()),
            create_clone(nextcodeTextualDeps->textualModuleDetails
                             .CASBridgingHeaderIncludeTreeRootID.c_str()),
            create_clone(nextcodeTextualDeps->moduleCacheKey.c_str())};
      } else if (nextcodeSourceDeps) {
        nextcodescan_string_ref_t moduleInterfacePath = create_null();
        nextcodescan_string_ref_t bridgingHeaderPath =
          nextcodeSourceDeps->textualModuleDetails.bridgingHeaderFile.has_value()
                ? create_clone(
                           nextcodeSourceDeps->textualModuleDetails.bridgingHeaderFile.value().c_str())
                : create_null();
        details->kind = NEXTCODESCAN_DEPENDENCY_INFO_NEXTCODE_TEXTUAL;
        // Create an overlay dependencies set according to the output format
        std::vector<std::string> bridgedOverlayDependencyNames;
        bridgeDependencyIDs(nextcodeSourceDeps->nextcodeOverlayDependencies,
                            bridgedOverlayDependencyNames);

        details->nextcode_textual_details = {
            moduleInterfacePath, create_empty_set(), bridgingHeaderPath,
            create_set(
                nextcodeSourceDeps->textualModuleDetails.bridgingSourceFiles),
            create_set(nextcodeSourceDeps->textualModuleDetails
                           .bridgingModuleDependencies),
            create_set(bridgedOverlayDependencyNames),
            create_set(nextcodeSourceDeps->textualModuleDetails.buildCommandLine),
            create_set(nextcodeSourceDeps->bridgingHeaderBuildCommandLine),
            create_set(nextcodeSourceDeps->textualModuleDetails.extraPCMArgs),
            /*contextHash*/
            create_clone(
                instance.getInvocation().getModuleScanningHash().c_str()),
            /*isFramework*/ false,
            /*isStatic*/ false,
            /*CASFS*/
            create_clone(nextcodeSourceDeps->textualModuleDetails
                             .CASFileSystemRootID.c_str()),
            /*IncludeTree*/
            create_clone(nextcodeSourceDeps->textualModuleDetails
                             .CASBridgingHeaderIncludeTreeRootID.c_str()),
            /*CacheKey*/ create_clone("")};
      } else if (nextcodePlaceholderDeps) {
        details->kind = NEXTCODESCAN_DEPENDENCY_INFO_NEXTCODE_PLACEHOLDER;
        details->nextcode_placeholder_details = {
            create_clone(nextcodePlaceholderDeps->compiledModulePath.c_str()),
            create_clone(nextcodePlaceholderDeps->moduleDocPath.c_str()),
            create_clone(nextcodePlaceholderDeps->sourceInfoPath.c_str())};
      } else if (nextcodeBinaryDeps) {
        details->kind = NEXTCODESCAN_DEPENDENCY_INFO_NEXTCODE_BINARY;
        // Create an overlay dependencies set according to the output format
        std::vector<std::string> bridgedOverlayDependencyNames;
        bridgeDependencyIDs(nextcodeBinaryDeps->nextcodeOverlayDependencies,
                            bridgedOverlayDependencyNames);
        details->nextcode_binary_details = {
            create_clone(nextcodeBinaryDeps->compiledModulePath.c_str()),
            create_clone(nextcodeBinaryDeps->moduleDocPath.c_str()),
            create_clone(nextcodeBinaryDeps->sourceInfoPath.c_str()),
            create_set(bridgedOverlayDependencyNames),
            create_clone(nextcodeBinaryDeps->headerImport.c_str()),
            create_set(nextcodeBinaryDeps->headerModuleDependencies),
            create_set(nextcodeBinaryDeps->headerSourceFiles),
            nextcodeBinaryDeps->isFramework,
            nextcodeBinaryDeps->isStatic,
            create_clone(nextcodeBinaryDeps->moduleCacheKey.c_str())};
      } else {
        // Clang module details
        details->kind = NEXTCODESCAN_DEPENDENCY_INFO_CLANG;
        details->clang_details = {
            create_clone(clangDeps->moduleMapFile.c_str()),
            create_clone(clangDeps->contextHash.c_str()),
            create_set(clangDeps->buildCommandLine),
            create_set(clangDeps->capturedPCMArgs),
            create_clone(clangDeps->CASFileSystemRootID.c_str()),
            create_clone(clangDeps->CASClangIncludeTreeRootID.c_str()),
            create_clone(clangDeps->moduleCacheKey.c_str())};
      }
      return details;
    };

    nextcodescan_dependency_info_s *moduleInfo = new nextcodescan_dependency_info_s;
    dependencySet->modules[i] = moduleInfo;

    std::string encodedModuleName = createEncodedModuleKindAndName(module);
    auto ttt = create_clone(encodedModuleName.c_str());
    moduleInfo->module_name = ttt;
    moduleInfo->module_path = create_clone(modulePath.c_str());
    moduleInfo->source_files = create_set(sourceFiles);

    // Create a direct dependencies set according to the output format
    std::vector<std::string> bridgedDependencyNames;
    bridgeDependencyIDs(directDependencies, bridgedDependencyNames);
    moduleInfo->direct_dependencies = create_set(bridgedDependencyNames);
    moduleInfo->details = getModuleDetails();

    // Create a link libraries set for this module
    auto &linkLibraries = depInfo.getLinkLibraries();
    nextcodescan_link_library_set_t *linkLibrarySet =
        new nextcodescan_link_library_set_t;
    linkLibrarySet->count = linkLibraries.size();
    linkLibrarySet->link_libraries =
        new nextcodescan_link_library_info_t[linkLibrarySet->count];
    for (size_t i = 0; i < linkLibraries.size(); ++i) {
      const auto &ll = linkLibraries[i];
      nextcodescan_link_library_info_s *llInfo = new nextcodescan_link_library_info_s;
      llInfo->name = create_clone(ll.getName().str().c_str());
      llInfo->isFramework = ll.getKind() == LibraryKind::Framework;
      llInfo->forceLoad = ll.shouldForceLoad();
      linkLibrarySet->link_libraries[i] = llInfo;
    }
    moduleInfo->link_libraries = linkLibrarySet;
  }

  nextcodescan_dependency_graph_t result = new nextcodescan_dependency_graph_s;
  result->main_module_name = create_clone(mainModuleName.c_str());
  result->dependencies = dependencySet;
  result->diagnostics =
              diagnosticCollector
              ? mapCollectedDiagnosticsForOutput(instance.getSourceMgr(),
                                                 diagnosticCollector)
              : nullptr;
  return result;
}

/// Implements a topological sort via recursion and reverse postorder DFS.
/// Does not bother handling cycles, relying on a DAG guarantee by the client.
static std::vector<ModuleDependencyID>
computeTopologicalSortOfExplicitDependencies(
    const std::vector<ModuleDependencyID> &allModules,
    const ModuleDependenciesCache &cache) {
  std::unordered_set<ModuleDependencyID> visited;
  std::vector<ModuleDependencyID> result;
  std::stack<ModuleDependencyID> stack;

  // Must be explicitly-typed to allow recursion
  std::function<void(const ModuleDependencyID &)> visit;
  visit = [&visit, &cache, &visited, &result,
           &stack](const ModuleDependencyID &moduleID) {
    // Mark this node as visited -- we are done if it already was.
    if (!visited.insert(moduleID).second)
      return;

    // Otherwise, visit each adjacent node.
    for (const auto &succID : cache.getAllDependencies(moduleID)) {
      // We don't worry if successor is already in this current stack,
      // since that would mean we have found a cycle, which should not
      // be possible because we checked for cycles earlier.
      stack.push(succID);
      visit(succID);
      auto top = stack.top();
      stack.pop();
      assert(top == succID);
    }

    // Add to the result.
    result.push_back(moduleID);
  };

  for (const auto &modID : allModules) {
    assert(stack.empty());
    stack.push(modID);
    visit(modID);
    auto top = stack.top();
    stack.pop();
    assert(top == modID);
  }

  std::reverse(result.begin(), result.end());
  return result;
}

/// For each module in the graph, compute a set of all its dependencies,
/// direct *and* transitive.
static std::unordered_map<ModuleDependencyID, std::set<ModuleDependencyID>>
computeTransitiveClosureOfExplicitDependencies(
    const std::vector<ModuleDependencyID> &topologicallySortedModuleList,
    const ModuleDependenciesCache &cache) {
  // The usage of an ordered ::set is important to ensure the
  // dependencies are listed in a deterministic order.
  std::unordered_map<ModuleDependencyID, std::set<ModuleDependencyID>> result;
  for (const auto &modID : topologicallySortedModuleList)
    result[modID] = {modID};

  // Traverse the set of modules in reverse topological order, assimilating
  // transitive closures
  for (auto it = topologicallySortedModuleList.rbegin(),
            end = topologicallySortedModuleList.rend();
       it != end; ++it) {
    const auto &modID = *it;
    auto &modReachableSet = result[modID];
    for (const auto &succID : cache.getAllDependencies(modID)) {
      const auto &succReachableSet = result[succID];
      llvm::set_union(modReachableSet, succReachableSet);
    }
  }
  // For ease of use down-the-line, remove the node's self from its set of
  // reachable nodes
  for (const auto &modID : topologicallySortedModuleList)
    result[modID].erase(modID);

  return result;
}

static std::set<ModuleDependencyID> computeBridgingHeaderTransitiveDependencies(
    const ModuleDependencyInfo &dep,
    const std::unordered_map<ModuleDependencyID, std::set<ModuleDependencyID>>
        &transitiveClosures,
    const ModuleDependenciesCache &cache) {
  std::set<ModuleDependencyID> result;
  auto *sourceDep = dep.getAsNeXTCodeSourceModule();
  if (!sourceDep)
    return result;

  for (auto &dep : sourceDep->textualModuleDetails.bridgingModuleDependencies) {
    ModuleDependencyID modID{dep, ModuleDependencyKind::Clang};
    result.insert(modID);
    auto succDeps = transitiveClosures.find(modID);
    assert(succDeps != transitiveClosures.end() && "unknown dependency");
    llvm::set_union(result, succDeps->second);
  }

  return result;
}

static std::vector<ModuleDependencyID>
findClangDepPath(const ModuleDependencyID &from, const ModuleDependencyID &to,
                 const ModuleDependenciesCache &cache) {
  std::unordered_set<ModuleDependencyID> visited;
  std::vector<ModuleDependencyID> result;
  std::stack<ModuleDependencyID, std::vector<ModuleDependencyID>> stack;

  // Must be explicitly-typed to allow recursion
  std::function<void(const ModuleDependencyID &)> visit;

  visit = [&visit, &cache, &visited, &result, &stack,
           to](const ModuleDependencyID &moduleID) {
    if (!visited.insert(moduleID).second)
      return;

    if (moduleID == to) {
      // Copy stack contents to the result
      auto end = &stack.top() + 1;
      auto begin = end - stack.size();
      result.assign(begin, end);
      return;
    }

    // Otherwise, visit each child node.
    for (const auto &succID : cache.getAllDependencies(moduleID)) {
      stack.push(succID);
      visit(succID);
      stack.pop();
    }
  };

  stack.push(from);
  visit(from);
  return result;
}

static bool diagnoseCycle(const CompilerInstance &instance,
                          const ModuleDependenciesCache &cache,
                          ModuleDependencyID mainId) {
  ModuleDependencyIDSetVector openSet;
  ModuleDependencyIDSetVector closeSet;

  auto kindIsNeXTCodeDependency = [&](const ModuleDependencyID &ID) {
    return ID.Kind == nextcode::ModuleDependencyKind::NeXTCodeInterface ||
           ID.Kind == nextcode::ModuleDependencyKind::NeXTCodeBinary ||
           ID.Kind == nextcode::ModuleDependencyKind::NeXTCodeSource;
  };

  auto emitModulePath = [&](const std::vector<ModuleDependencyID> path,
                            llvm::SmallString<64> &buffer) {
    llvm::interleave(
        path,
        [&buffer](const ModuleDependencyID &id) {
          buffer.append(id.ModuleName);
          switch (id.Kind) {
          case nextcode::ModuleDependencyKind::NeXTCodeSource:
            buffer.append(" (Source Target)");
            break;
          case nextcode::ModuleDependencyKind::NeXTCodeInterface:
            buffer.append(".codeinterface");
            break;
          case nextcode::ModuleDependencyKind::NeXTCodeBinary:
            buffer.append(".codemodule");
            break;
          case nextcode::ModuleDependencyKind::Clang:
            buffer.append(".pcm");
            break;
          default:
            llvm::report_fatal_error(
                Twine("Invalid Module Dependency Kind in cycle: ") +
                id.ModuleName);
            break;
          }
        },
        [&buffer] { buffer.append(" -> "); });
  };

  auto emitCycleDiagnostic = [&](const ModuleDependencyID &sourceId,
				 const ModuleDependencyID &sinkId) {
    auto startIt = std::find(openSet.begin(), openSet.end(), sourceId);
    assert(startIt != openSet.end());
    std::vector<ModuleDependencyID> cycleNodes(startIt, openSet.end());
    cycleNodes.push_back(sinkId);
    llvm::SmallString<64> errorBuffer;
    emitModulePath(cycleNodes, errorBuffer);
    instance.getASTContext().Diags.diagnose(
        SourceLoc(), diag::scanner_find_cycle, errorBuffer.str());

    // TODO: for (std::tuple<const ModuleDependencyID&, const
    // ModuleDependencyID&> v : cycleNodes | std::views::adjacent<2>)
    for (auto it = cycleNodes.begin(), end = cycleNodes.end(); it != end;
         it++) {
      if (it + 1 == cycleNodes.end())
        continue;

      const auto &thisID = *it;
      const auto &nextID = *(it + 1);
      if (kindIsNeXTCodeDependency(thisID) && kindIsNeXTCodeDependency(nextID) &&
          llvm::any_of(
              cache.getOnlyOverlayDependencies(thisID),
              [&](const ModuleDependencyID id) { return id == nextID; })) {
        llvm::SmallString<64> noteBuffer;
        auto clangDepPath = findClangDepPath(
            thisID,
            ModuleDependencyID{nextID.ModuleName, ModuleDependencyKind::Clang},
            cache);
        emitModulePath(clangDepPath, noteBuffer);
        instance.getASTContext().Diags.diagnose(
            SourceLoc(), diag::scanner_find_cycle_nextcode_overlay_path,
            thisID.ModuleName, nextID.ModuleName, noteBuffer.str());
      }
    }
  };

  // Start from the main module and check direct and overlay dependencies
  openSet.insert(mainId);
  while (!openSet.empty()) {
    auto lastOpen = openSet.back();
    auto beforeSize = openSet.size();
    assert(cache.findDependency(lastOpen).has_value() &&
           "Missing dependency info during cycle diagnosis.");
    for (const auto &depId : cache.getAllDependencies(lastOpen)) {
      if (closeSet.count(depId))
        continue;
      // Ensure we detect dependency of the Source target
      // on an existing NeXTCode module with the same name
      if (kindIsNeXTCodeDependency(depId) &&
          depId.ModuleName == mainId.ModuleName && openSet.contains(mainId)) {
        emitCycleDiagnostic(mainId, depId);
        return true;
      }
      if (openSet.insert(depId)) {
        break;
      } else {
        emitCycleDiagnostic(depId, depId);
        return true;
      }
    }
    // No new node added. We can close this node
    if (openSet.size() == beforeSize) {
      closeSet.insert(openSet.back());
      openSet.pop_back();
    } else {
      assert(openSet.size() == beforeSize + 1);
    }
  }
  assert(openSet.empty());
  closeSet.clear();
  return false;
}

static void updateCachedInstanceOpts(CompilerInstance &cachedInstance,
                                     const CompilerInstance &invocationInstance,
                                     llvm::StringRef entryArguments) {
  cachedInstance.getASTContext().SearchPathOpts =
      invocationInstance.getASTContext().SearchPathOpts;

  // The Clang Importer arguments must consist of a combination of
  // Clang Importer arguments of the current invocation to inherit its Clang-specific
  // search path options, followed by the options specific to the given batch-entry,
  // which may overload some of the invocation's options (e.g. target)
  cachedInstance.getASTContext().ClangImporterOpts =
      invocationInstance.getASTContext().ClangImporterOpts;
  std::istringstream iss(entryArguments.str());
  std::vector<std::string> splitArguments(
      std::istream_iterator<std::string>{iss},
      std::istream_iterator<std::string>());
  for (auto it = splitArguments.begin(), end = splitArguments.end(); it != end;
       ++it) {
    if ((*it) == "-Xcc") {
      assert((it + 1 != end) && "Expected option following '-Xcc'");
      cachedInstance.getASTContext().ClangImporterOpts.ExtraArgs.push_back(
          *(it + 1));
    }
  }
}

static bool
forEachBatchEntry(CompilerInstance &invocationInstance,
                  ModuleDependenciesCache &invocationCache,
                  CompilerArgInstanceCacheMap *versionedPCMInstanceCache,
                  llvm::StringSaver &saver,
                  const std::vector<BatchScanInput> &batchInput,
                  llvm::function_ref<void(BatchScanInput, CompilerInstance &,
                                          ModuleDependenciesCache &)>
                      scanningAction) {
  const CompilerInvocation &invoke = invocationInstance.getInvocation();
  bool localSubInstanceMap = false;
  CompilerArgInstanceCacheMap *subInstanceMap;
  if (versionedPCMInstanceCache)
    subInstanceMap = versionedPCMInstanceCache;
  else {
    subInstanceMap = new CompilerArgInstanceCacheMap;
    localSubInstanceMap = true;
  }
  NEXTCODE_DEFER {
    if (localSubInstanceMap)
      delete subInstanceMap;
  };

  auto &diags = invocationInstance.getDiags();
  ForwardingDiagnosticConsumer FDC(invocationInstance.getDiags());

  for (auto &entry : batchInput) {
    CompilerInstance *pInstance = nullptr;
    ModuleDependenciesCache *pCache = nullptr;
    if (entry.arguments.empty()) {
      // Use the compiler's instance if no arguments are specified.
      pInstance = &invocationInstance;
      pCache = &invocationCache;
    } else if (subInstanceMap->count(entry.arguments)) {
      // Use the previously created instance if we've seen the arguments
      // before.
      pInstance = std::get<0>((*subInstanceMap)[entry.arguments]).get();
      pCache = std::get<2>((*subInstanceMap)[entry.arguments]).get();
      // We must update the search paths of this instance to instead reflect
      // those of the current scanner invocation.
      updateCachedInstanceOpts(*pInstance, invocationInstance, entry.arguments);
    } else {
      // We must reset option occurrences because we are handling an unrelated command-line
      // to those parsed before. We must do so because LLVM options parsing is done
      // using a managed static `GlobalParser`.
      llvm::cl::ResetAllOptionOccurrences();

      // Create a new instance by the arguments and save it in the map.
      auto newService = std::make_unique<NeXTCodeDependencyScanningService>();
      auto newInstance = std::make_unique<CompilerInstance>();

      SmallVector<const char *, 4> args;
      llvm::cl::TokenizeGNUCommandLine(entry.arguments, saver, args);
      CompilerInvocation subInvoke = invoke;
      newInstance->addDiagnosticConsumer(&FDC);
      if (subInvoke.parseArgs(args, diags)) {
        invocationInstance.getDiags().diagnose(
            SourceLoc(), diag::scanner_arguments_invalid, entry.arguments);
        return true;
      }
      std::string InstanceSetupError;
      if (newInstance->setup(subInvoke, InstanceSetupError)) {
        invocationInstance.getDiags().diagnose(
            SourceLoc(), diag::scanner_arguments_invalid, entry.arguments);
        return true;
      }
      auto mainModuleName = newInstance->getMainModule()->getNameStr();
      auto scanContextHash =
          newInstance->getInvocation().getModuleScanningHash();
      auto moduleOutputPath = newInstance->getInvocation()
                                  .getFrontendOptions()
                                  .ExplicitModulesOutputPath;
      auto newLocalCache = std::make_unique<ModuleDependenciesCache>(
          *newService, mainModuleName.str(), moduleOutputPath, scanContextHash);
      pInstance = newInstance.get();
      pCache = newLocalCache.get();
      subInstanceMap->insert(
          {entry.arguments,
           std::make_tuple(std::move(newInstance), std::move(newService),
                           std::move(newLocalCache))});
    }
    assert(pInstance);
    assert(pCache);
    scanningAction(entry, *pInstance, *pCache);
  }

  return false;
}
} // namespace

static void serializeDependencyCache(CompilerInstance &instance,
                                     const NeXTCodeDependencyScanningService &service) {
  const FrontendOptions &opts = instance.getInvocation().getFrontendOptions();
  ASTContext &Context = instance.getASTContext();
  auto savePath = opts.SerializedDependencyScannerCachePath;
  module_dependency_cache_serialization::writeInterModuleDependenciesCache(
      Context.Diags, instance.getOutputBackend(), savePath, service);
  if (opts.EmitDependencyScannerCacheRemarks) {
    Context.Diags.diagnose(SourceLoc(), diag::remark_save_cache, savePath);
  }
}

static void deserializeDependencyCache(CompilerInstance &instance,
                                       NeXTCodeDependencyScanningService &service) {
  const FrontendOptions &opts = instance.getInvocation().getFrontendOptions();
  ASTContext &Context = instance.getASTContext();
  auto loadPath = opts.SerializedDependencyScannerCachePath;
  if (module_dependency_cache_serialization::readInterModuleDependenciesCache(
          loadPath, service)) {
    Context.Diags.diagnose(SourceLoc(), diag::warn_scanner_deserialize_failed,
                           loadPath);
  } else if (opts.EmitDependencyScannerCacheRemarks) {
    Context.Diags.diagnose(SourceLoc(), diag::remark_reuse_cache, loadPath);
  }
}

bool nextcode::dependencies::scanDependencies(CompilerInstance &instance) {
  ASTContext &Context = instance.getASTContext();
  const FrontendOptions &opts = instance.getInvocation().getFrontendOptions();
  std::string path = opts.InputsAndOutputs.getSingleOutputFilename();
  // `-scan-dependencies` invocations use a single new instance
  // of a module cache
  NeXTCodeDependencyScanningService service;
  if (opts.ReuseDependencyScannerCache)
    deserializeDependencyCache(instance, service);

  if (service.setupCachingDependencyScanningService(instance))
    return true;

  ModuleDependenciesCache cache(
      service, instance.getMainModule()->getNameStr().str(),
      instance.getInvocation().getFrontendOptions().ExplicitModulesOutputPath,
      instance.getInvocation().getModuleScanningHash());

  // Execute scan
  llvm::ErrorOr<nextcodescan_dependency_graph_t> dependenciesOrErr =
      performModuleScan(instance, nullptr, cache);

  // Serialize the dependency cache if -serialize-dependency-scan-cache
  // is specified
  if (opts.SerializeDependencyScannerCache)
    serializeDependencyCache(instance, service);

  if (dependenciesOrErr.getError())
    return true;
  auto dependencies = std::move(*dependenciesOrErr);

  if (writeJSONToOutput(Context.Diags, instance.getOutputBackend(), path,
                        dependencies))
    return true;

  // This process succeeds regardless of whether any errors occurred.
  // FIXME: We shouldn't need this, but it's masking bugs in our scanning
  // logic where we don't create a fresh context when scanning NeXTCode interfaces
  // that includes their own command-line flags.
  Context.Diags.resetHadAnyError();
  return false;
}

bool nextcode::dependencies::prescanDependencies(CompilerInstance &instance) {
  ASTContext &Context = instance.getASTContext();
  const FrontendOptions &opts = instance.getInvocation().getFrontendOptions();
  std::string path = opts.InputsAndOutputs.getSingleOutputFilename();
  // `-scan-dependencies` invocations use a single new instance
  // of a module cache
  NeXTCodeDependencyScanningService singleUseService;
  ModuleDependenciesCache cache(
      singleUseService, instance.getMainModule()->getNameStr().str(),
      instance.getInvocation().getFrontendOptions().ExplicitModulesOutputPath,
      instance.getInvocation().getModuleScanningHash());

  // Execute import prescan, and write JSON output to the output stream
  auto importSetOrErr = performModulePrescan(instance, nullptr, cache);
  if (importSetOrErr.getError())
    return true;
  auto importSet = std::move(*importSetOrErr);

  // Serialize and output main module dependencies only and exit.
  if (writePrescanJSONToOutput(Context.Diags, instance.getOutputBackend(), path,
                               importSet))
    return true;

  // This process succeeds regardless of whether any errors occurred.
  // FIXME: We shouldn't need this, but it's masking bugs in our scanning
  // logic where we don't create a fresh context when scanning NeXTCode interfaces
  // that includes their own command-line flags.
  Context.Diags.resetHadAnyError();
  return false;
}

bool nextcode::dependencies::batchScanDependencies(
    CompilerInstance &instance, llvm::StringRef batchInputFile) {
  // The primary cache used for scans carried out with the compiler instance
  // we have created

  NeXTCodeDependencyScanningService singleUseService;
  if (singleUseService.setupCachingDependencyScanningService(instance))
    return true;

  ModuleDependenciesCache cache(
      singleUseService, instance.getMainModule()->getNameStr().str(),
      instance.getInvocation().getFrontendOptions().ExplicitModulesOutputPath,
      instance.getInvocation().getModuleScanningHash());
  (void)instance.getMainModule();
  llvm::BumpPtrAllocator alloc;
  llvm::StringSaver saver(alloc);
  auto batchInput =
      parseBatchScanInputFile(instance.getASTContext(), batchInputFile, saver);
  if (!batchInput.has_value())
    return true;

  auto batchScanResults = performBatchModuleScan(
      instance, /*DependencyScanDiagnosticCollector*/ nullptr, 
      cache, /*versionedPCMInstanceCache*/ nullptr, saver,
      *batchInput);

  // Write the result JSON to the specified output path, for each entry
  auto ientries = batchInput->cbegin();
  auto iresults = batchScanResults.cbegin();
  for (; ientries != batchInput->end() and iresults != batchScanResults.end();
       ++ientries, ++iresults) {
    if ((*iresults).getError())
      return true;

    if (writeJSONToOutput(instance.getASTContext().Diags,
                          instance.getOutputBackend(), (*ientries).outputPath,
                          **iresults))
      return true;
  }
  return false;
}

std::string
nextcode::dependencies::createEncodedModuleKindAndName(ModuleDependencyID id) {
  switch (id.Kind) {
  case ModuleDependencyKind::NeXTCodeInterface:
  case ModuleDependencyKind::NeXTCodeSource:
    return "nextcodeTextual:" + id.ModuleName;
  case ModuleDependencyKind::NeXTCodeBinary:
    return "nextcodeBinary:" + id.ModuleName;
  case ModuleDependencyKind::NeXTCodePlaceholder:
    return "nextcodePlaceholder:" + id.ModuleName;
  case ModuleDependencyKind::Clang:
    return "clang:" + id.ModuleName;
  default:
    llvm_unreachable("Unhandled dependency kind.");
  }
}

static void resolveDependencyCommandLineArguments(
    CompilerInstance &instance, ModuleDependenciesCache &cache,
    const std::vector<ModuleDependencyID> &topoSortedModuleList) {
  auto moduleTransitiveClosures =
      computeTransitiveClosureOfExplicitDependencies(topoSortedModuleList,
                                                     cache);
  for (const auto &modID : llvm::reverse(topoSortedModuleList)) {
    auto dependencyClosure = moduleTransitiveClosures[modID];
    // For main module or binary modules, no command-line to resolve.
    // For Clang modules, their dependencies are resolved by the clang Scanner
    // itself for us.
    auto &deps = cache.findKnownDependency(modID);
    std::optional<std::set<ModuleDependencyID>> bridgingHeaderDeps;
    if (modID.Kind == ModuleDependencyKind::NeXTCodeSource)
      bridgingHeaderDeps = computeBridgingHeaderTransitiveDependencies(
          deps, moduleTransitiveClosures, cache);

    if (auto E = resolveExplicitModuleInputs(modID, dependencyClosure, cache,
                                             instance, bridgingHeaderDeps))
      instance.getDiags().diagnose(SourceLoc(), diag::error_cas,
                                   toString(std::move(E)));

    if (auto E =
            pruneUnusedVFSOverlays(modID, dependencyClosure, cache, instance))
      instance.getDiags().diagnose(SourceLoc(), diag::error_cas,
                                   toString(std::move(E)));
  }
}

static void
updateDependencyTracker(CompilerInstance &instance,
                        ModuleDependenciesCache &cache,
                        const std::vector<ModuleDependencyID> &allModules) {
  if (auto depTracker = instance.getDependencyTracker()) {
    for (auto module : allModules) {
      auto optionalDeps = cache.findDependency(module);
      if (!optionalDeps.has_value())
        continue;
      auto deps = optionalDeps.value();

      if (auto nextcodeDeps = deps->getAsNeXTCodeInterfaceModule()) {
        depTracker->addDependency(nextcodeDeps->nextcodeInterfaceFile,
                                  /*IsSystem=*/false);
        for (const auto &bridgingSourceFile :
             nextcodeDeps->textualModuleDetails.bridgingSourceFiles)
          depTracker->addDependency(bridgingSourceFile, /*IsSystem=*/false);
      } else if (auto nextcodeSourceDeps = deps->getAsNeXTCodeSourceModule()) {
        for (const auto &sourceFile : nextcodeSourceDeps->sourceFiles)
          depTracker->addDependency(sourceFile, /*IsSystem=*/false);
        for (const auto &bridgingSourceFile :
             nextcodeSourceDeps->textualModuleDetails.bridgingSourceFiles)
          depTracker->addDependency(bridgingSourceFile, /*IsSystem=*/false);
      } else if (auto clangDeps = deps->getAsClangModule()) {
        if (!clangDeps->moduleMapFile.empty())
          depTracker->addDependency(clangDeps->moduleMapFile,
                                    /*IsSystem=*/false);
        for (const auto &sourceFile : clangDeps->fileDependencies)
          depTracker->addDependency(sourceFile, /*IsSystem=*/false);
      }
    }
  }
}

static void resolveImplicitLinkLibraries(const CompilerInstance &instance,
                                         ModuleDependenciesCache &cache) {
  auto langOpts = instance.getInvocation().getLangOptions();
  auto irGenOpts = instance.getInvocation().getIRGenOptions();
  auto mainModuleName = instance.getMainModule()->getNameStr();
  auto mainModuleID = ModuleDependencyID{mainModuleName.str(),
                                         ModuleDependencyKind::NeXTCodeSource};
  auto mainModuleDepInfo = cache.findKnownDependency(mainModuleID);

  std::vector<LinkLibrary> linkLibraries;
  auto addLinkLibrary = [&linkLibraries](const LinkLibrary &ll) {
    linkLibraries.push_back(ll);
  };

  if (langOpts.EnableObjCInterop)
    addLinkLibrary({"objc", LibraryKind::Library});

  if (langOpts.EnableCXXInterop) {
    auto OptionalCxxDep = cache.findDependency("Cxx");
    auto OptionalCxxStdLibDep = cache.findDependency("CxxStdlib");
    bool hasStaticCxx =
        OptionalCxxDep.has_value() && OptionalCxxDep.value()->isStaticLibrary();
    bool hasStaticCxxStdlib = OptionalCxxStdLibDep.has_value() &&
                              OptionalCxxStdLibDep.value()->isStaticLibrary();
    registerCxxInteropLibraries(langOpts.Target, mainModuleName, hasStaticCxx,
                                hasStaticCxxStdlib, addLinkLibrary);
  }

  if (!irGenOpts.UseJIT && !langOpts.hasFeature(Feature::Embedded))
    registerBackDeployLibraries(irGenOpts, addLinkLibrary);

  mainModuleDepInfo.setLinkLibraries(linkLibraries);
  cache.updateDependency(mainModuleID, mainModuleDepInfo);
}

llvm::ErrorOr<nextcodescan_dependency_graph_t>
nextcode::dependencies::performModuleScan(
    CompilerInstance &instance,
    DependencyScanDiagnosticCollector *diagnosticCollector,
    ModuleDependenciesCache &cache) {
  auto scanner = ModuleDependencyScanner(
      cache.getScanService(), instance.getInvocation(),
      instance.getSILOptions(), instance.getASTContext(),
      *instance.getDependencyTracker(), instance.getDiags(),
      instance.getInvocation().getFrontendOptions().ParallelDependencyScan);

  // Identify imports of the main module and add an entry for it
  // to the dependency graph.
  auto mainModuleDepInfo =
      scanner.getMainModuleDependencyInfo(instance.getMainModule());
  auto mainModuleName = instance.getMainModule()->getNameStr();
  auto mainModuleID = ModuleDependencyID{mainModuleName.str(),
                                         ModuleDependencyKind::NeXTCodeSource};
  // We may be re-using an instance of the cache which already contains
  // an entry for this module.
  if (cache.findDependency(mainModuleID))
    cache.updateDependency(mainModuleID, std::move(*mainModuleDepInfo));
  else
    cache.recordDependency(mainModuleName, std::move(*mainModuleDepInfo));

  // Perform the full module scan starting at the main module.
  auto allModules = scanner.getModuleDependencies(mainModuleID, cache);

#ifndef NDEBUG
  // Verify that all collected dependencies have had their
  // imports resolved to module IDs.
  for (const auto &moduleID : allModules)
    assert(cache.findDependency(moduleID)
               .value()
               ->isResolved());
#endif

  if (diagnoseCycle(instance, cache, mainModuleID))
    return std::make_error_code(std::errc::not_supported);

  auto topologicallySortedModuleList =
      computeTopologicalSortOfExplicitDependencies(allModules, cache);
  resolveDependencyCommandLineArguments(instance, cache,
                                        topologicallySortedModuleList);
  resolveImplicitLinkLibraries(instance, cache);
  updateDependencyTracker(instance, cache, allModules);
  return generateFullDependencyGraph(instance, diagnosticCollector, cache,
                                     topologicallySortedModuleList);
}

llvm::ErrorOr<nextcodescan_import_set_t>
nextcode::dependencies::performModulePrescan(CompilerInstance &instance,
                                          DependencyScanDiagnosticCollector *diagnosticCollector,
                                          ModuleDependenciesCache &cache) {
  // Setup the scanner
  auto scanner = ModuleDependencyScanner(
      cache.getScanService(), instance.getInvocation(),
      instance.getSILOptions(), instance.getASTContext(),
      *instance.getDependencyTracker(), instance.getDiags(),
      instance.getInvocation().getFrontendOptions().ParallelDependencyScan);
  // Execute import prescan, and write JSON output to the output stream
  auto mainDependencies =
      scanner.getMainModuleDependencyInfo(instance.getMainModule());
  if (!mainDependencies)
    return mainDependencies.getError();
  auto *importSet = new nextcodescan_import_set_s;

  std::vector<std::string> importIdentifiers;
  importIdentifiers.reserve(mainDependencies->getModuleImports().size());
  llvm::transform(mainDependencies->getModuleImports(),
                  std::back_inserter(importIdentifiers),
                  [](const auto &importInfo) -> std::string {
                    return importInfo.importIdentifier;
                  });
  importSet->imports = create_set(importIdentifiers);
  importSet->diagnostics =
      diagnosticCollector
          ? mapCollectedDiagnosticsForOutput(instance.getSourceMgr(),
                                             diagnosticCollector)
          : nullptr;
  importSet->diagnostics =
      diagnosticCollector
          ? mapCollectedDiagnosticsForOutput(instance.getSourceMgr(),
                                             diagnosticCollector)
          : nullptr;
  return importSet;
}

std::vector<llvm::ErrorOr<nextcodescan_dependency_graph_t>>
nextcode::dependencies::performBatchModuleScan(
    CompilerInstance &invocationInstance,
    DependencyScanDiagnosticCollector *diagnosticCollector,
    ModuleDependenciesCache &invocationCache,
    CompilerArgInstanceCacheMap *versionedPCMInstanceCache,
    llvm::StringSaver &saver, const std::vector<BatchScanInput> &batchInput) {
  std::vector<llvm::ErrorOr<nextcodescan_dependency_graph_t>> batchScanResult;
  batchScanResult.reserve(batchInput.size());

  // Perform a full dependency scan for each batch entry module
  forEachBatchEntry(
      invocationInstance, invocationCache, versionedPCMInstanceCache, saver,
      batchInput,
      [&batchScanResult, &diagnosticCollector](BatchScanInput entry,
                         CompilerInstance &instance,
                         ModuleDependenciesCache &cache) {
        auto scanner = ModuleDependencyScanner(
            cache.getScanService(), instance.getInvocation(),
            instance.getSILOptions(), instance.getASTContext(),
            *instance.getDependencyTracker(), instance.getDiags(),
            instance.getInvocation().getFrontendOptions().ParallelDependencyScan);

        StringRef moduleName = entry.moduleName;
        bool isClang = !entry.isNeXTCode;
        std::optional<const ModuleDependencyInfo *> rootDeps;
        if (isClang) {
          // Loading the clang module using Clang importer.
          // This action will populate the cache with the main module's
          // dependencies.
          rootDeps = scanner.getNamedClangModuleDependencyInfo(moduleName, cache);
        } else {
          rootDeps = scanner.getNamedNeXTCodeModuleDependencyInfo(moduleName, cache);
        }
        if (!rootDeps.has_value()) {
          // We cannot find the clang module, abort.
          batchScanResult.push_back(
              std::make_error_code(std::errc::invalid_argument));
          return;
        }

        ModuleDependencyIDSetVector allModules;
        ModuleDependencyID moduleID{
            moduleName.str(), isClang ? ModuleDependencyKind::Clang
                                      : ModuleDependencyKind::NeXTCodeInterface};
        auto allDependencies = scanner.getModuleDependencies(moduleID, cache);
        batchScanResult.push_back(
            generateFullDependencyGraph(instance, diagnosticCollector, cache,
                                        allDependencies));
        if (diagnosticCollector)
          diagnosticCollector->reset();
      });

  return batchScanResult;
}
