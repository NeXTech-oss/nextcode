//===----------------------------------------------------------------------===//
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
//             Tural Ghuliev (turalquliyev@it-gss.com)
//
//===----------------------------------------------------------------------===//

#include "Utils.h"
#include "nextcode/AST/Expr.h"
#include "nextcode/AST/Stmt.h"

using namespace nextcode;

/// Whether or not the given statement starts a new scope. Note that most
/// statements are handled by the \c BraceStmt check. The others listed are
/// a somewhat special case since they can also declare variables in their
/// condition.
bool nextcode::refactoring::asyncrefactorings::startsNewScope(Stmt *S) {
  switch (S->getKind()) {
  case StmtKind::Brace:
  case StmtKind::If:
  case StmtKind::While:
  case StmtKind::ForEach:
  case StmtKind::Case:
    return true;
  default:
    return false;
  }
}

/// A more aggressive variant of \c Expr::getReferencedDecl that also looks
/// through autoclosures created to pass the \c self parameter to a member funcs
ValueDecl *nextcode::refactoring::asyncrefactorings::
    getReferencedDeclLookingThroughAutoclosures(const Expr *Fn) {
  Fn = Fn->getSemanticsProvidingExpr();
  if (auto *DRE = dyn_cast<DeclRefExpr>(Fn))
    return DRE->getDecl();
  if (auto ApplyE = dyn_cast<SelfApplyExpr>(Fn))
    return getReferencedDeclLookingThroughAutoclosures(ApplyE->getFn());
  if (auto *ACE = dyn_cast<AutoClosureExpr>(Fn)) {
    if (auto *Unwrapped = ACE->getUnwrappedCurryThunkExpr())
      return getReferencedDeclLookingThroughAutoclosures(Unwrapped);
  }
  return nullptr;
}

FuncDecl *
nextcode::refactoring::asyncrefactorings::getUnderlyingFunc(const Expr *Fn) {
  return dyn_cast_or_null<FuncDecl>(
      getReferencedDeclLookingThroughAutoclosures(Fn));
}
