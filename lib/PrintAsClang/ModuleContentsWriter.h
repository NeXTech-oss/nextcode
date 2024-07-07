//===--- ModuleContentsWriter.h - Walk module to print ObjC/C++ -*- C++ -*-===//
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

#ifndef NEXTCODE_PRINTASCLANG_MODULECONTENTSWRITER_H
#define NEXTCODE_PRINTASCLANG_MODULECONTENTSWRITER_H

#include "nextcode/AST/AttrKind.h"
#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/PointerUnion.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/StringSet.h"

namespace clang {
  class Module;
}

namespace nextcode {
class Decl;
class ModuleDecl;
class NeXTCodeToClangInteropContext;

using ImportModuleTy = PointerUnion<ModuleDecl*, const clang::Module*>;

/// Prints the declarations of \p M to \p os and collecting imports in
/// \p imports along the way.
void printModuleContentsAsObjC(raw_ostream &os,
                               llvm::SmallPtrSetImpl<ImportModuleTy> &imports,
                               ModuleDecl &M,
                               NeXTCodeToClangInteropContext &interopContext);

struct EmittedClangHeaderDependencyInfo {
    /// The set of imported modules used by this module.
    SmallPtrSet<ImportModuleTy, 8> imports;
    /// True if the printed module depends on types from the Stdlib module.
    bool dependsOnStandardLibrary = false;
};

/// Prints the declarations of \p M to \p os in C++ language mode.
///
/// \returns Dependencies required by this module.
EmittedClangHeaderDependencyInfo printModuleContentsAsCxx(
    raw_ostream &os, ModuleDecl &M, NeXTCodeToClangInteropContext &interopContext,
    bool requiresExposedAttribute, llvm::StringSet<> &exposedModules);

} // end namespace nextcode

#endif

