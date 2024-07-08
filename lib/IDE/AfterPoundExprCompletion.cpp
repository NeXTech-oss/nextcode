//===--- AfterPoundExprCompletion.cpp -------------------------------------===//
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

#include "nextcode/IDE/AfterPoundExprCompletion.h"
#include "nextcode/IDE/CodeCompletion.h"
#include "nextcode/IDE/CompletionLookup.h"
#include "nextcode/Sema/CompletionContextFinder.h"
#include "nextcode/Sema/ConstraintSystem.h"
#include "nextcode/Sema/IDETypeChecking.h"

using namespace nextcode;
using namespace nextcode::constraints;
using namespace nextcode::ide;

void AfterPoundExprCompletion::sawSolutionImpl(const constraints::Solution &S) {
  Type ExpectedTy = getTypeForCompletion(S, CompletionExpr);

  bool IsAsync = isContextAsync(S, DC);

  // If ExpectedTy is a duplicate of any other result, ignore this solution.
  auto IsEqual = [&](const Result &R) {
    return R.ExpectedTy->isEqual(ExpectedTy);
  };
  if (!llvm::any_of(Results, IsEqual)) {
    bool IsImpliedResult = isImpliedResult(S, CompletionExpr);
    Results.push_back({ExpectedTy, IsImpliedResult, IsAsync});
  }
}

void AfterPoundExprCompletion::collectResults(
    ide::CodeCompletionContext &CompletionCtx) {
  ASTContext &Ctx = DC->getASTContext();
  CompletionLookup Lookup(CompletionCtx.getResultSink(), Ctx, DC,
                          &CompletionCtx);

  Lookup.shouldCheckForDuplicates(Results.size() > 1);

  // The type context that is being used for global results.
  ExpectedTypeContext UnifiedTypeContext;
  UnifiedTypeContext.setPreferNonVoid(true);

  for (auto &Result : Results) {
    Lookup.setExpectedTypes({Result.ExpectedTy}, Result.IsImpliedResult,
                            /*expectsNonVoid=*/true);
    Lookup.addPoundAvailable(ParentStmtKind);
    Lookup.addObjCPoundKeywordCompletions(/*needPound=*/false);
    Lookup.getMacroCompletions(CodeCompletionMacroRole::Expression);

    UnifiedTypeContext.merge(*Lookup.getExpectedTypeContext());
  }

  collectCompletionResults(CompletionCtx, Lookup, DC, UnifiedTypeContext,
                           /*CanCurrDeclContextHandleAsync=*/false);
}
