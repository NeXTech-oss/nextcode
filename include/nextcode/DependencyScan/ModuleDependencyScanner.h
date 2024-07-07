//===--- ModuleDependencyScanner.h - Import NeXTCode modules --------*- C++
//-*-===//
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

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Identifier.h"
#include "nextcode/AST/ModuleDependencies.h"
#include "nextcode/Frontend/ModuleInterfaceLoader.h"
#include "nextcode/Serialization/SerializedModuleLoader.h"
#include "llvm/Support/ThreadPool.h"

namespace nextcode {
class DependencyTracker;
}

namespace nextcode {

/// A dependency scanning worker which performs filesystem lookup
/// of a named module dependency.
class ModuleDependencyScanningWorker {
public:
  ModuleDependencyScanningWorker(
      NeXTCodeDependencyScanningService &globalScanningService,
      const CompilerInvocation &ScanCompilerInvocation,
      const SILOptions &SILOptions, ASTContext &ScanASTContext,
      DependencyTracker &DependencyTracker, DiagnosticEngine &diags);

private:
  /// Retrieve the module dependencies for the module with the given name.
  ModuleDependencyVector
  scanFilesystemForModuleDependency(Identifier moduleName,
                                    const ModuleDependenciesCache &cache,
                                    bool isTestableImport = false);

  /// Retrieve the module dependencies for the Clang module with the given name.
  ModuleDependencyVector
  scanFilesystemForClangModuleDependency(Identifier moduleName,
                                         const ModuleDependenciesCache &cache);

  /// Retrieve the module dependencies for the NeXTCode module with the given name.
  ModuleDependencyVector
  scanFilesystemForNeXTCodeModuleDependency(Identifier moduleName,
                                         const ModuleDependenciesCache &cache);

  // An AST delegate for interface scanning.
  std::unique_ptr<InterfaceSubContextDelegateImpl> ScanningASTDelegate;
  // The Clang scanner tool used by this worker.
  clang::tooling::dependencies::DependencyScanningTool clangScanningTool;
  // NeXTCode and Clang module loaders acting as scanners.
  std::unique_ptr<ModuleInterfaceLoader> nextcodeScannerModuleLoader;
  std::unique_ptr<ClangImporter> clangScannerModuleLoader;
  // Restrict access to the parent scanner class.
  friend class ModuleDependencyScanner;
};

class ModuleDependencyScanner {
public:
  ModuleDependencyScanner(NeXTCodeDependencyScanningService &ScanningService,
                          const CompilerInvocation &ScanCompilerInvocation,
                          const SILOptions &SILOptions,
                          ASTContext &ScanASTContext,
                          DependencyTracker &DependencyTracker,
                          DiagnosticEngine &diags, bool ParallelScan);

  /// Identify the scanner invocation's main module's dependencies
  llvm::ErrorOr<ModuleDependencyInfo>
  getMainModuleDependencyInfo(ModuleDecl *mainModule);

  /// Resolve module dependencies of the given module, computing a full
  /// transitive closure dependency graph.
  std::vector<ModuleDependencyID>
  getModuleDependencies(ModuleDependencyID moduleID,
                        ModuleDependenciesCache &cache);

  /// Query the module dependency info for the Clang module with the given name.
  /// Explicit by-name lookups are useful for batch mode scanning.
  std::optional<const ModuleDependencyInfo *>
  getNamedClangModuleDependencyInfo(StringRef moduleName,
                                    ModuleDependenciesCache &cache);

  /// Query the module dependency info for the NeXTCode module with the given name.
  /// Explicit by-name lookups are useful for batch mode scanning.
  std::optional<const ModuleDependencyInfo *>
  getNamedNeXTCodeModuleDependencyInfo(StringRef moduleName,
                                    ModuleDependenciesCache &cache);

private:
  /// Resolve the direct dependencies of the given module.
  std::vector<ModuleDependencyID>
  resolveDirectModuleDependencies(ModuleDependencyID moduleID,
                                  ModuleDependenciesCache &cache);

  /// Resolve imported module names of a given module to concrete
  /// modules. If `ParallelScan` is enabled, this operation is multithreaded.
  void
  resolveImportDependencies(const ModuleDependencyID &moduleID,
                            ModuleDependenciesCache &cache,
                            ModuleDependencyIDSetVector &directDependencies);

  /// If a module has a bridging header or other header inputs, execute a dependency scan
  /// on it and record the dependencies.
  void resolveHeaderDependencies(
      const ModuleDependencyID &moduleID, ModuleDependenciesCache &cache,
      std::vector<std::string> &allClangModules,
      llvm::StringSet<> &alreadyKnownModules,
      ModuleDependencyIDSetVector &directDependencies);

  /// Resolve all module dependencies comprised of NeXTCode overlays
  /// of this module's Clang module dependencies.
  void resolveNeXTCodeOverlayDependencies(
      const ModuleDependencyID &moduleID,
      const std::vector<std::string> &clangDependencies,
      ModuleDependenciesCache &cache,
      ModuleDependencyIDSetVector &nextcodeOverlayDependencies,
      ModuleDependencyIDSetVector &directDependencies);

  /// Identify all cross-import overlay modules of the specified
  /// dependency set and apply an action for each.
  void discoverCrossImportOverlayDependencies(
      StringRef mainModuleName, ArrayRef<ModuleDependencyID> allDependencies,
      ModuleDependenciesCache &cache,
      llvm::function_ref<void(ModuleDependencyID)> action);

  /// Perform an operation utilizing one of the Scanning workers
  /// available to this scanner.
  template <typename Function, typename... Args>
  auto withDependencyScanningWorker(Function &&F, Args &&...ArgList);

  Identifier getModuleImportIdentifier(StringRef moduleName);

private:
  const CompilerInvocation &ScanCompilerInvocation;
  ASTContext &ScanASTContext;
  DiagnosticEngine &Diagnostics;

  /// The available pool of workers for filesystem module search
  unsigned NumThreads;
  std::list<std::unique_ptr<ModuleDependencyScanningWorker>> Workers;
  llvm::ThreadPool ScanningThreadPool;
  /// Protect worker access.
  std::mutex WorkersLock;
};

} // namespace nextcode
