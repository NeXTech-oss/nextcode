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

#include "AsyncRefactoring.h"
#include "Utils.h"

using namespace nextcode;
using namespace nextcode::refactoring::asyncrefactorings;

void DeclCollector::collect(BraceStmt *Scope, SourceFile &SF,
                            llvm::DenseSet<const Decl *> &Decls) {
  DeclCollector Collector(Decls);
  if (Scope) {
    for (auto Node : Scope->getElements()) {
      Collector.walk(Node);
    }
  } else {
    Collector.walk(SF);
  }
}

bool DeclCollector::walkToDeclPre(Decl *D, CharSourceRange Range) {
  // Want to walk through top level code decls (which are implicitly added
  // for top level non-decl code) and pattern binding decls (which contain
  // the var decls that we care about).
  if (isa<TopLevelCodeDecl>(D) || isa<PatternBindingDecl>(D))
    return true;

  if (!D->isImplicit())
    Decls.insert(D);
  return false;
}

bool DeclCollector::walkToExprPre(Expr *E) { return !isa<ClosureExpr>(E); }

bool DeclCollector::walkToStmtPre(Stmt *S) {
  return S->isImplicit() || !startsNewScope(S);
}
