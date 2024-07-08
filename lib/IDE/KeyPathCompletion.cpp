//===--- KeyPathCompletion.cpp --------------------------------------------===//
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

#include "nextcode/Basic/Assertions.h"
#include "nextcode/IDE/KeyPathCompletion.h"
#include "nextcode/IDE/CodeCompletion.h"
#include "nextcode/IDE/CompletionLookup.h"
#include "nextcode/Sema/ConstraintSystem.h"

using namespace nextcode;
using namespace nextcode::constraints;
using namespace nextcode::ide;

void KeyPathTypeCheckCompletionCallback::sawSolutionImpl(
    const constraints::Solution &S) {
  // Determine the code completion.
  size_t ComponentIndex = 0;
  for (auto &Component : KeyPath->getComponents()) {
    if (Component.getKind() == KeyPathExpr::Component::Kind::CodeCompletion) {
      break;
    } else {
      ComponentIndex++;
    }
  }
  assert(ComponentIndex < KeyPath->getComponents().size() &&
         "Didn't find a code compleiton component?");

  Type BaseType;
  if (ComponentIndex == 0) {
    // We are completing on the root and need to extract the key path's root
    // type.
    if (auto *rootTy = KeyPath->getExplicitRootType()) {
      BaseType = S.getResolvedType(rootTy);
    } else {
      // The key path doesn't have a root TypeRepr set, so we can't look the key
      // path's root up through it. Build a constraint locator and look the
      // root type up through it.
      // FIXME: Improve the linear search over S.typeBindings when it's possible
      // to look up type variables by their locators.
      auto RootLocator =
          S.getConstraintLocator(KeyPath, {ConstraintLocator::KeyPathRoot});
      auto BaseVariableTypeBinding =
          llvm::find_if(S.typeBindings, [&RootLocator](const auto &Entry) {
            return Entry.first->getImpl().getLocator() == RootLocator;
          });
      if (BaseVariableTypeBinding != S.typeBindings.end()) {
        BaseType = S.simplifyType(BaseVariableTypeBinding->getSecond());
      }
    }
  } else {
    // We are completing after a component. Get the previous component's result
    // type.
    BaseType = S.simplifyType(S.getType(KeyPath, ComponentIndex - 1));
  }
  if (BaseType.isNull()) {
    return;
  }

  // If ExpectedTy is a duplicate of any other result, ignore this solution.
  if (llvm::any_of(Results, [&](const Result &R) {
        return R.BaseType->isEqual(BaseType);
      })) {
    return;
  }
  Results.push_back({BaseType, /*OnRoot=*/(ComponentIndex == 0)});
}

void KeyPathTypeCheckCompletionCallback::collectResults(
    DeclContext *DC, SourceLoc DotLoc,
    ide::CodeCompletionContext &CompletionCtx) {
  ASTContext &Ctx = DC->getASTContext();
  CompletionLookup Lookup(CompletionCtx.getResultSink(), Ctx, DC,
                          &CompletionCtx);

  if (DotLoc.isValid()) {
    Lookup.setHaveDot(DotLoc);
  }
  Lookup.shouldCheckForDuplicates(Results.size() > 1);

  for (auto &Result : Results) {
    Lookup.setIsNeXTCodeKeyPathExpr(Result.OnRoot);
    Lookup.getValueExprCompletions(Result.BaseType);
  }

  collectCompletionResults(CompletionCtx, Lookup, DC, ExpectedTypeContext(),
                           /*CanCurrDeclContextHandleAsync=*/false);
}
