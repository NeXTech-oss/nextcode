//===--- IfConfigClause.h ---------------------------------------*- C++ -*-===//
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
// This file defines the IfConfigClause.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_IFCONFIGCLAUSE_H
#define NEXTCODE_AST_IFCONFIGCLAUSE_H

#include "llvm/ADT/ArrayRef.h"

namespace nextcode {
  class Expr;
  class SourceLoc;
  struct ASTNode;

/// This represents one part of a #if block.  If the condition field is
/// non-null, then this represents a #if or a #elseif, otherwise it represents
/// an #else block.
struct IfConfigClause {
  /// The location of the #if, #elseif, or #else keyword.
  SourceLoc Loc;
  
  /// The condition guarding this #if or #elseif block.  If this is null, this
  /// is a #else clause.
  Expr *Cond;
  
  /// Elements inside the clause
  ArrayRef<ASTNode> Elements;

  /// True if this is the active clause of the #if block.
  const bool isActive;

  IfConfigClause(SourceLoc Loc, Expr *Cond,
                 ArrayRef<ASTNode> Elements, bool isActive)
    : Loc(Loc), Cond(Cond), Elements(Elements), isActive(isActive) {
  }
};

} // end namespace nextcode

#endif
