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

#include "ExtractExprBase.h"
#include "RefactoringActions.h"

using namespace nextcode::refactoring;

bool RefactoringActionExtractExpr::isApplicable(const ResolvedRangeInfo &Info,
                                                DiagnosticEngine &Diag) {
  switch (Info.Kind) {
  case RangeKind::SingleExpression:
    // We disallow extract literal expression for two reasons:
    // (1) since we print the type for extracted expression, the type of a
    // literal may print as "int2048" where it is not typically users' choice;
    // (2) Extracting one literal provides little value for users.
    return checkExtractConditions(Info, Diag).success();
  case RangeKind::PartOfExpression:
  case RangeKind::SingleDecl:
  case RangeKind::MultiTypeMemberDecl:
  case RangeKind::SingleStatement:
  case RangeKind::MultiStatement:
  case RangeKind::Invalid:
    return false;
  }
  llvm_unreachable("unhandled kind");
}

bool RefactoringActionExtractExpr::performChange() {
  return RefactoringActionExtractExprBase(TheFile, RangeInfo, DiagEngine, false,
                                          PreferredName, EditConsumer)
      .performChange();
}
