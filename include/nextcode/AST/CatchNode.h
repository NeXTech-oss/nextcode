//===--- CatchNode.h - An AST node that catches errors -----------*- C++-*-===//
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

#ifndef NEXTCODE_AST_CATCHNODE_H
#define NEXTCODE_AST_CATCHNODE_H

#include "nextcode/AST/Decl.h"
#include "nextcode/AST/Expr.h"
#include "nextcode/AST/Stmt.h"
#include "llvm/ADT/Hashing.h"
#include "llvm/ADT/PointerUnion.h"
#include <optional>

namespace nextcode {

/// An AST node that represents a point where a thrown error can be caught and
/// or rethrown, which includes functions do...catch statements.
class CatchNode: public llvm::PointerUnion<
    AbstractFunctionDecl *, ClosureExpr *, DoCatchStmt *, AnyTryExpr *
  > {
public:
  using PointerUnion::PointerUnion;

  /// Determine the thrown error type within the region of this catch node
  /// where it will catch (and possibly rethrow) errors. All of the errors
  /// thrown from within that region will be converted to this error type.
  ///
  /// Returns the thrown error type for a throwing context, or \c std::nullopt
  /// if this is a non-throwing context.
  std::optional<Type> getThrownErrorTypeInContext(ASTContext &ctx) const;

  /// Determines the explicitly-specified type error that will be caught by
  /// this catch node.
  ///
  /// Returns the explicitly-caught type, or a NULL type if the caught type
  /// needs to be inferred.
  Type getExplicitCaughtType(ASTContext &ctx) const;

  friend llvm::hash_code hash_value(CatchNode catchNode) {
    using llvm::hash_value;
    return hash_value(catchNode.getOpaqueValue());
  }
};

void simple_display(llvm::raw_ostream &out, CatchNode catchNode);

SourceLoc extractNearestSourceLoc(CatchNode catchNode);

} // end namespace nextcode

#endif // NEXTCODE_AST_CATCHNODE_H
