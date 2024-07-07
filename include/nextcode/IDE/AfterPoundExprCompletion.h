//===--- AfterPoundExprCompletion.h ---------------------------------------===//
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

#ifndef NEXTCODE_IDE_AFTERPOUNDEXPRCOMPLETION_H
#define NEXTCODE_IDE_AFTERPOUNDEXPRCOMPLETION_H

#include "nextcode/IDE/CodeCompletionConsumer.h"
#include "nextcode/IDE/CodeCompletionContext.h"
#include "nextcode/IDE/TypeCheckCompletionCallback.h"

namespace nextcode {
namespace ide {

/// Used to collect and store information needed to perform unresolved member
/// completion (\c CompletionKind::UnresolvedMember ) from the solutions
/// formed during expression type-checking.
class AfterPoundExprCompletion : public TypeCheckCompletionCallback {
  struct Result {
    Type ExpectedTy;
    bool IsImpliedResult;

    /// Whether the surrounding context is async and thus calling async
    /// functions is supported.
    bool IsInAsyncContext;
  };

  CodeCompletionExpr *CompletionExpr;
  DeclContext *DC;
  std::optional<StmtKind> ParentStmtKind;

  SmallVector<Result, 4> Results;

  void sawSolutionImpl(const constraints::Solution &solution) override;

public:
  AfterPoundExprCompletion(CodeCompletionExpr *CompletionExpr, DeclContext *DC,
                           std::optional<StmtKind> ParentStmtKind)
      : CompletionExpr(CompletionExpr), DC(DC), ParentStmtKind(ParentStmtKind) {
  }

  void collectResults(ide::CodeCompletionContext &CompletionCtx);
};

} // end namespace ide
} // end namespace nextcode

#endif // NEXTCODE_IDE_AFTERPOUNDEXPRCOMPLETION_H
