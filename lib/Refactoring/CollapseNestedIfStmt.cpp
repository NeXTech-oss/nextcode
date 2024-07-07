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
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//

#include "RefactoringActions.h"
#include "nextcode/AST/Stmt.h"

using namespace nextcode::refactoring;

static std::pair<IfStmt *, IfStmt *>
findCollapseNestedIfTarget(ResolvedCursorInfoPtr CursorInfo) {
  auto StmtStartInfo = dyn_cast<ResolvedStmtStartCursorInfo>(CursorInfo);
  if (!StmtStartInfo)
    return {};

  // Ensure the statement is 'if' statement. It must not have 'else' clause.
  IfStmt *OuterIf = dyn_cast<IfStmt>(StmtStartInfo->getTrailingStmt());
  if (!OuterIf)
    return {};
  if (OuterIf->getElseStmt())
    return {};

  // The body must contain a sole inner 'if' statement.
  auto Body = dyn_cast_or_null<BraceStmt>(OuterIf->getThenStmt());
  if (!Body || Body->getNumElements() != 1)
    return {};

  IfStmt *InnerIf =
      dyn_cast_or_null<IfStmt>(Body->getFirstElement().dyn_cast<Stmt *>());
  if (!InnerIf)
    return {};

  // Inner 'if' statement also cannot have 'else' clause.
  if (InnerIf->getElseStmt())
    return {};

  return {OuterIf, InnerIf};
}

bool RefactoringActionCollapseNestedIfStmt::isApplicable(
    ResolvedCursorInfoPtr CursorInfo, DiagnosticEngine &Diag) {
  return findCollapseNestedIfTarget(CursorInfo).first;
}

bool RefactoringActionCollapseNestedIfStmt::performChange() {
  auto Target = findCollapseNestedIfTarget(CursorInfo);
  if (!Target.first)
    return true;
  auto OuterIf = Target.first;
  auto InnerIf = Target.second;

  EditorConsumerInsertStream OS(
      EditConsumer, SM,
      Lexer::getCharSourceRangeFromSourceRange(SM, OuterIf->getSourceRange()));

  OS << tok::kw_if << " ";

  // Emit conditions.
  bool first = true;
  for (auto &C : llvm::concat<StmtConditionElement>(OuterIf->getCond(),
                                                    InnerIf->getCond())) {
    if (first)
      first = false;
    else
      OS << ", ";
    OS << Lexer::getCharSourceRangeFromSourceRange(SM, C.getSourceRange())
              .str();
  }

  // Emit body.
  OS << " ";
  OS << Lexer::getCharSourceRangeFromSourceRange(
            SM, InnerIf->getThenStmt()->getSourceRange())
            .str();
  return false;
}
