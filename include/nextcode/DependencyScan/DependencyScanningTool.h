//===-------------- DependencyScanningTool.h - NeXTCode Compiler -------------===//
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

#ifndef NEXTCODE_DEPENDENCY_SCANNING_TOOL_H
#define NEXTCODE_DEPENDENCY_SCANNING_TOOL_H

#include "nextcode-c/DependencyScan/DependencyScan.h"
#include "nextcode/Frontend/Frontend.h"
#include "nextcode/AST/ModuleDependencies.h"
#include "nextcode/DependencyScan/ScanDependencies.h"
#include "nextcode/Frontend/PrintingDiagnosticConsumer.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/StringSaver.h"

namespace nextcode {
namespace dependencies {
class DependencyScanningTool;
class DependencyScanDiagnosticCollector;

struct ScanQueryInstance {
  std::unique_ptr<CompilerInstance> ScanInstance;
  std::unique_ptr<DependencyScanDiagnosticCollector> ScanDiagnostics;
};

/// Diagnostic consumer that simply collects the diagnostics emitted so-far
class DependencyScanDiagnosticCollector : public DiagnosticConsumer {
private:
  struct ScannerDiagnosticInfo {
    std::string Message;
    llvm::SourceMgr::DiagKind Severity;
    std::optional<ScannerImportStatementInfo::ImportDiagnosticLocationInfo> ImportLocation;
  };
  std::vector<ScannerDiagnosticInfo> Diagnostics;

  void handleDiagnostic(SourceManager &SM, const DiagnosticInfo &Info) override;

protected:
  virtual void addDiagnostic(SourceManager &SM, const DiagnosticInfo &Info);

public:
  friend DependencyScanningTool;
  DependencyScanDiagnosticCollector() {}
  void reset() { Diagnostics.clear(); }
  const std::vector<ScannerDiagnosticInfo> &getDiagnostics() const {
    return Diagnostics;
  }
};

/// Locking variant of the above diagnostic collector that guards accesses to
/// its state with a lock.
class LockingDependencyScanDiagnosticCollector
    : public DependencyScanDiagnosticCollector {
private:
  void addDiagnostic(SourceManager &SM, const DiagnosticInfo &Info) override;
  llvm::sys::SmartMutex<true> ScanningDiagnosticConsumerStateLock;

public:
  friend DependencyScanningTool;
  LockingDependencyScanDiagnosticCollector() {}
};

/// Given a set of arguments to a print-target-info frontend tool query, produce the
/// JSON target info.
llvm::ErrorOr<nextcodescan_string_ref_t> getTargetInfo(ArrayRef<const char *> Command,
                                                    const char *main_executable_path);

/// The high-level implementation of the dependency scanner that runs on
/// an individual worker thread.
class DependencyScanningTool {
public:
  /// Construct a dependency scanning tool.
  DependencyScanningTool();

  /// Collect the full module dependency graph for the input, ignoring any
  /// placeholder modules.
  ///
  /// \returns a \c StringError with the diagnostic output if errors
  /// occurred, \c nextcodescan_dependency_result_t otherwise.
  llvm::ErrorOr<nextcodescan_dependency_graph_t>
  getDependencies(ArrayRef<const char *> Command,
                  const llvm::StringSet<> &PlaceholderModules,
                  StringRef WorkingDirectory);

  /// Collect the set of imports for the input module
  ///
  /// \returns a \c StringError with the diagnostic output if errors
  /// occurred, \c nextcodescan_prescan_result_t otherwise.
  llvm::ErrorOr<nextcodescan_import_set_t>
  getImports(ArrayRef<const char *> Command, StringRef WorkingDirectory);

  /// Collect the full module dependency graph for the input collection of
  /// module names (batch inputs) and output them to the
  /// BatchScanInput-specified output locations.
  ///
  /// \returns a \c std::error_code if errors occurred during scan.
  std::vector<llvm::ErrorOr<nextcodescan_dependency_graph_t>>
  getDependencies(ArrayRef<const char *> Command,
                  const std::vector<BatchScanInput> &BatchInput,
                  const llvm::StringSet<> &PlaceholderModules,
                  StringRef WorkingDirectory);

  /// Writes the current `SharedCache` instance to a specified FileSystem path.
  void serializeCache(llvm::StringRef path);
  /// Loads an instance of a `NeXTCodeDependencyScanningService` to serve as the `SharedCache`
  /// from a specified FileSystem path.
  bool loadCache(llvm::StringRef path);
  /// Discard the tool's current `SharedCache` and start anew.
  void resetCache();
  /// Query diagnostics consumed so far.
  std::vector<DependencyScanDiagnosticCollector::ScannerDiagnosticInfo> getDiagnostics();
  /// Discared the collection of diagnostics encountered so far.
  void resetDiagnostics();

  /// Using the specified invocation command, instantiate a CompilerInstance
  /// that will be used for this scan.
  llvm::ErrorOr<ScanQueryInstance>
  initCompilerInstanceForScan(ArrayRef<const char *> Command,
                              StringRef WorkingDirectory);

private:
  /// Using the specified invocation command, initialize the scanner instance
  /// for this scan. Returns the `CompilerInstance` that will be used.
  llvm::ErrorOr<ScanQueryInstance>
  initScannerForAction(ArrayRef<const char *> Command,
                       StringRef WorkingDirectory);

  /// Shared cache of module dependencies, re-used by individual full-scan queries
  /// during the lifetime of this Tool.
  std::unique_ptr<NeXTCodeDependencyScanningService> ScanningService;

  /// Shared cache of compiler instances created during batch scanning, corresponding to
  /// command-line options specified in the batch scan input entry.
  std::unique_ptr<CompilerArgInstanceCacheMap> VersionedPCMInstanceCacheCache;

  /// Shared state mutual-exclusivity lock
  llvm::sys::SmartMutex<true> DependencyScanningToolStateLock;

  /// A shared consumer that accumulates encountered diagnostics.
  LockingDependencyScanDiagnosticCollector CDC;
  llvm::BumpPtrAllocator Alloc;
  llvm::StringSaver Saver;
};

} // end namespace dependencies
} // end namespace nextcode

#endif // NEXTCODE_DEPENDENCY_SCANNING_TOOL_H
