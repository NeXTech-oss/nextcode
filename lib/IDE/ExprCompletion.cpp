//===--- ExprCompletion.cpp -----------------------------------------------===//
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

#include "nextcode/IDE/ExprCompletion.h"
#include "nextcode/IDE/CodeCompletion.h"
#include "nextcode/IDE/CompletionLookup.h"
#include "nextcode/Sema/ConstraintSystem.h"

using namespace nextcode;
using namespace nextcode::ide;
using namespace nextcode::constraints;

static bool solutionSpecificVarTypesEqual(
    const llvm::SmallDenseMap<const VarDecl *, Type> &LHS,
    const llvm::SmallDenseMap<const VarDecl *, Type> &RHS) {
  if (LHS.size() != RHS.size()) {
    return false;
  }
  for (auto LHSEntry : LHS) {
    auto RHSEntry = RHS.find(LHSEntry.first);
    if (RHSEntry == RHS.end()) {
      // Entry of the LHS doesn't exist in RHS
      return false;
    } else if (!nullableTypesEqual(LHSEntry.second, RHSEntry->second)) {
      return false;
    }
  }
  return true;
}

bool ExprTypeCheckCompletionCallback::Result::operator==(
    const Result &Other) const {
  return IsImpliedResult == Other.IsImpliedResult &&
         IsInAsyncContext == Other.IsInAsyncContext &&
         nullableTypesEqual(UnresolvedMemberBaseType,
                            Other.UnresolvedMemberBaseType) &&
         solutionSpecificVarTypesEqual(SolutionSpecificVarTypes,
                                       Other.SolutionSpecificVarTypes);
}

void ExprTypeCheckCompletionCallback::addExpectedType(Type ExpectedType) {
  auto IsEqual = [&ExpectedType](Type Other) {
    return nullableTypesEqual(ExpectedType, Other);
  };
  if (llvm::any_of(ExpectedTypes, IsEqual)) {
    return;
  }
  ExpectedTypes.push_back(ExpectedType);
}

void ExprTypeCheckCompletionCallback::addResult(
    bool IsImpliedResult, bool IsInAsyncContext, Type UnresolvedMemberBaseType,
    llvm::SmallDenseMap<const VarDecl *, Type> SolutionSpecificVarTypes) {
  if (!AddUnresolvedMemberCompletions) {
    UnresolvedMemberBaseType = Type();
  }
  Result NewResult = {IsImpliedResult, IsInAsyncContext,
                      UnresolvedMemberBaseType, SolutionSpecificVarTypes};
  if (llvm::is_contained(Results, NewResult)) {
    return;
  }
  Results.push_back(NewResult);
}

void ExprTypeCheckCompletionCallback::sawSolutionImpl(
    const constraints::Solution &S) {
  Type ExpectedTy = getTypeForCompletion(S, CompletionExpr);
  bool IsImpliedResult = isImpliedResult(S, CompletionExpr);
  bool IsAsync = isContextAsync(S, DC);

  llvm::SmallDenseMap<const VarDecl *, Type> SolutionSpecificVarTypes;
  getSolutionSpecificVarTypes(S, SolutionSpecificVarTypes);

  addResult(IsImpliedResult, IsAsync, ExpectedTy, SolutionSpecificVarTypes);
  addExpectedType(ExpectedTy);

  if (auto PatternMatchType = getPatternMatchType(S, CompletionExpr)) {
    addResult(IsImpliedResult, IsAsync, PatternMatchType,
              SolutionSpecificVarTypes);
    addExpectedType(PatternMatchType);
  }
}

void ExprTypeCheckCompletionCallback::collectResults(
    SourceLoc CCLoc, ide::CodeCompletionContext &CompletionCtx) {
  ASTContext &Ctx = DC->getASTContext();
  CompletionLookup Lookup(CompletionCtx.getResultSink(), Ctx, DC,
                          &CompletionCtx);
  Lookup.shouldCheckForDuplicates(Results.size() > 1);

  // The type context that is being used for global results.
  ExpectedTypeContext UnifiedTypeContext;
  UnifiedTypeContext.setPreferNonVoid(true);
  bool UnifiedCanHandleAsync = false;

  for (auto &Result : Results) {
    WithSolutionSpecificVarTypesRAII VarTypes(Result.SolutionSpecificVarTypes);

    Lookup.setExpectedTypes(ExpectedTypes, Result.IsImpliedResult);
    Lookup.setCanCurrDeclContextHandleAsync(Result.IsInAsyncContext);
    Lookup.setSolutionSpecificVarTypes(Result.SolutionSpecificVarTypes);

    Lookup.getValueCompletionsInDeclContext(CCLoc);
    Lookup.getSelfTypeCompletionInDeclContext(CCLoc, /*isForDeclResult=*/false);
    if (Result.UnresolvedMemberBaseType) {
      Lookup.getUnresolvedMemberCompletions(Result.UnresolvedMemberBaseType);
    }

    UnifiedTypeContext.merge(*Lookup.getExpectedTypeContext());
    UnifiedCanHandleAsync |= Result.IsInAsyncContext;
  }

  collectCompletionResults(CompletionCtx, Lookup, DC, UnifiedTypeContext,
                           UnifiedCanHandleAsync);
}
