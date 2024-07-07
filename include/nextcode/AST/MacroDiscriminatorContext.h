//===--- MacroDiscriminatorContext.h - Macro Discriminators -----*- C++ -*-===//
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

#ifndef NEXTCODE_AST_MACRO_DISCRIMINATOR_CONTEXT_H
#define NEXTCODE_AST_MACRO_DISCRIMINATOR_CONTEXT_H

#include "nextcode/AST/Decl.h"
#include "nextcode/AST/Expr.h"
#include "llvm/ADT/PointerUnion.h"

namespace nextcode {

/// Describes the context of a macro expansion for the purpose of
/// computing macro expansion discriminators.
struct MacroDiscriminatorContext
    : public llvm::PointerUnion<DeclContext *, FreestandingMacroExpansion *> {
  using PointerUnion::PointerUnion;

  static MacroDiscriminatorContext getParentOf(FreestandingMacroExpansion *expansion);
  static MacroDiscriminatorContext getParentOf(
      SourceLoc loc, DeclContext *origDC
  );

  /// Return the innermost declaration context that is suitable for
  /// use in identifying a macro.
  static DeclContext *getInnermostMacroContext(DeclContext *dc);
};

}

#endif // NEXTCODE_AST_MACRO_DISCRIMINATOR_CONTEXT_H
