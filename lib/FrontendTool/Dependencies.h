//===--- Dependencies.h -- Unified header for dependency tracing utilities --===//
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

#ifndef NEXTCODE_FRONTENDTOOL_DEPENDENCIES_H
#define NEXTCODE_FRONTENDTOOL_DEPENDENCIES_H

namespace llvm {
namespace vfs {
class OutputBackend;
}
} // namespace llvm

namespace nextcode {

class ASTContext;
class DependencyTracker;
class DiagnosticEngine;
class FrontendOptions;
class InputFile;
class ModuleDecl;

/// Emit the names of the modules imported by \c mainModule.
bool emitImportedModules(ModuleDecl *mainModule, const FrontendOptions &opts,
                         llvm::vfs::OutputBackend &backend);
bool emitMakeDependenciesIfNeeded(DiagnosticEngine &diags,
                                  DependencyTracker *depTracker,
                                  const FrontendOptions &opts,
                                  const InputFile &input,
                                  llvm::vfs::OutputBackend &backend);
bool emitLoadedModuleTraceIfNeeded(ModuleDecl *mainModule,
                                   DependencyTracker *depTracker,
                                   const FrontendOptions &opts,
                                   const InputFile &input);

} // end namespace nextcode

#endif
