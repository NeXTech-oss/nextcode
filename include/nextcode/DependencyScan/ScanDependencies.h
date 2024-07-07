//===--- ScanDependencies.h -- Scans the dependencies of a module ------===//
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

#ifndef NEXTCODE_DEPENDENCY_SCANDEPENDENCIES_H
#define NEXTCODE_DEPENDENCY_SCANDEPENDENCIES_H

#include "nextcode-c/DependencyScan/DependencyScan.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/Support/Error.h"

namespace llvm {
class StringSaver;
}

namespace nextcode {

class CompilerInvocation;
class CompilerInstance;
class ModuleDependenciesCache;
class NeXTCodeDependencyScanningService;

namespace dependencies {
class DependencyScanDiagnosticCollector;

using CompilerArgInstanceCacheMap =
    llvm::StringMap<std::tuple<std::unique_ptr<CompilerInstance>,
                               std::unique_ptr<NeXTCodeDependencyScanningService>,
                               std::unique_ptr<ModuleDependenciesCache>>>;

struct BatchScanInput {
  llvm::StringRef moduleName;
  llvm::StringRef arguments;
  llvm::StringRef outputPath;
  bool isNeXTCode;
};

// MARK: FrontendTool dependency scanner entry points
/// Scans the dependencies of the main module of \c instance and writes out
/// the resulting JSON according to the instance's output parameters.
bool scanDependencies(CompilerInstance &instance);

/// Identify all imports in the translation unit's module.
bool prescanDependencies(CompilerInstance &instance);

/// Batch scan the dependencies for modules specified in \c batchInputFile.
bool batchScanDependencies(CompilerInstance &instance,
                           llvm::StringRef batchInputFile);

// MARK: Dependency scanning execution
/// Scans the dependencies of the main module of \c instance.
llvm::ErrorOr<nextcodescan_dependency_graph_t>
performModuleScan(CompilerInstance &instance,
                  DependencyScanDiagnosticCollector *diagnostics,
                  ModuleDependenciesCache &cache);

/// Scans the main module of \c instance for all direct module imports
llvm::ErrorOr<nextcodescan_import_set_t>
performModulePrescan(CompilerInstance &instance,
                     DependencyScanDiagnosticCollector *diagnostics,
                     ModuleDependenciesCache &cache);

/// Batch scan the dependencies for modules specified in \c batchInputFile.
std::vector<llvm::ErrorOr<nextcodescan_dependency_graph_t>>
performBatchModuleScan(CompilerInstance &invocationInstance,
                       DependencyScanDiagnosticCollector *diagnostics,
                       ModuleDependenciesCache &invocationCache,
                       CompilerArgInstanceCacheMap *versionedPCMInstanceCache,
                       llvm::StringSaver &saver,
                       const std::vector<BatchScanInput> &BatchInput);
} // end namespace dependencies
} // end namespace nextcode

#endif
