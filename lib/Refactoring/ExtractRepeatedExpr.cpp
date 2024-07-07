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

bool RefactoringActionExtractRepeatedExpr::isApplicable(
    const ResolvedRangeInfo &Info, DiagnosticEngine &Diag) {
  switch (Info.Kind) {
  case RangeKind::SingleExpression:
    return checkExtractConditions(Info, Diag)
        .success({CannotExtractReason::Literal});
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

bool RefactoringActionExtractRepeatedExpr::performChange() {
  return RefactoringActionExtractExprBase(TheFile, RangeInfo, DiagEngine, true,
                                          PreferredName, EditConsumer)
      .performChange();
}
