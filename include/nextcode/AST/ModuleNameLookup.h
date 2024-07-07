//===--- ModuleNameLookup.h - Name lookup within a module -------*- C++ -*-===//
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
// This file defines interfaces for performing top-level name lookup into a
// set of imported modules.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_MODULE_NAME_LOOKUP_H
#define NEXTCODE_AST_MODULE_NAME_LOOKUP_H

#include "llvm/ADT/SmallVector.h"
#include "nextcode/AST/Identifier.h"
#include "nextcode/AST/Module.h"
#include "nextcode/Basic/SourceLoc.h"

namespace nextcode {
class ValueDecl;

namespace namelookup {

enum class ResolutionKind {
  /// Lookup can match any number of decls, as long as they are all
  /// overloadable.
  ///
  /// If non-overloadable decls are returned, this indicates ambiguous lookup.
  Overloadable,

  /// Lookup should match a single decl that declares a type.
  TypesOnly,

  /// Lookup should only match macros.
  MacrosOnly,
};

void simple_display(llvm::raw_ostream &out, ResolutionKind kind);

/// Performs a lookup into the given module and it's imports.
///
/// If 'moduleOrFile' is a ModuleDecl, we search the module and it's
/// public imports. If 'moduleOrFile' is a SourceFile, we search the
/// file's parent module, the module's public imports, and the source
/// file's private imports.
///
/// \param moduleOrFile The module or file unit whose imports to search.
/// \param name The name to look up.
/// \param[out] decls Any found decls will be added to this vector.
/// \param lookupKind Whether this lookup is qualified or unqualified.
/// \param resolutionKind What sort of decl is expected.
/// \param moduleScopeContext The top-level context from which the lookup is
///        being performed, for checking access. This must be either a
///        FileUnit or a Module.
/// \param options name lookup options. Currently only used to communicate the
/// NL_IncludeUsableFromInline option.
void lookupInModule(const DeclContext *moduleOrFile,
                    DeclName name, SmallVectorImpl<ValueDecl *> &decls,
                    NLKind lookupKind, ResolutionKind resolutionKind,
                    const DeclContext *moduleScopeContext,
                    SourceLoc loc, NLOptions options);

/// Performs a qualified lookup into the given module and, if necessary, its
/// reexports, observing proper shadowing rules.
void
lookupVisibleDeclsInModule(const DeclContext *moduleOrFile,
                           ImportPath::Access accessPath,
                           SmallVectorImpl<ValueDecl *> &decls,
                           NLKind lookupKind,
                           ResolutionKind resolutionKind,
                           const DeclContext *moduleScopeContext);

} // end namespace namelookup

} // end namespace nextcode

#endif
