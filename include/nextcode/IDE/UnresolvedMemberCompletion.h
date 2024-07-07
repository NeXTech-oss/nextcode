//===--- UnresolvedMemberCompletion.h -------------------------------------===//
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

#ifndef NEXTCODE_IDE_UNRESOLVEDMEMBERCOMPLETION_H
#define NEXTCODE_IDE_UNRESOLVEDMEMBERCOMPLETION_H

#include "nextcode/IDE/CodeCompletionConsumer.h"
#include "nextcode/IDE/CodeCompletionContext.h"
#include "nextcode/IDE/TypeCheckCompletionCallback.h"

namespace nextcode {
namespace ide {

/// Used to collect and store information needed to perform unresolved member
/// completion (\c CompletionKind::UnresolvedMember ) from the solutions
/// formed during expression type-checking.
class UnresolvedMemberTypeCheckCompletionCallback
    : public TypeCheckCompletionCallback {
  struct Result {
    Type ExpectedTy;
    bool IsImpliedResult;

    /// Whether the surrounding context is async and thus calling async
    /// functions is supported.
    bool IsInAsyncContext;

    /// Attempts to merge this result with \p Other, returning \c true if
    /// successful, else \c false.
    bool tryMerge(const Result &Other, DeclContext *DC);
  };

  CodeCompletionExpr *CompletionExpr;
  DeclContext *DC;

  SmallVector<Result, 4> ExprResults;
  SmallVector<Result, 1> EnumPatternTypes;

  /// Add a result to \c Results, merging it with an existing result, if
  /// possible.
  void addExprResult(const Result &Res);

  void sawSolutionImpl(const constraints::Solution &solution) override;

public:
  UnresolvedMemberTypeCheckCompletionCallback(
      CodeCompletionExpr *CompletionExpr, DeclContext *DC)
      : CompletionExpr(CompletionExpr), DC(DC) {}

  void collectResults(DeclContext *DC, SourceLoc DotLoc,
                      ide::CodeCompletionContext &CompletionCtx);
};

} // end namespace ide
} // end namespace nextcode

#endif // NEXTCODE_IDE_UNRESOLVEDMEMBERCOMPLETION_H
