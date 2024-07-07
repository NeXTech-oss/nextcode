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

#ifndef NEXTCODE_REFACTORING_CONTEXTFINDER_H
#define NEXTCODE_REFACTORING_CONTEXTFINDER_H

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/Basic/SourceManager.h"
#include "nextcode/IDE/SourceEntityWalker.h"

namespace nextcode {
namespace refactoring {

class ContextFinder : public SourceEntityWalker {
  SourceFile &SF;
  ASTContext &Ctx;
  SourceManager &SM;
  SourceRange Target;
  std::function<bool(ASTNode)> IsContext;
  SmallVector<ASTNode, 4> AllContexts;
  bool contains(ASTNode Enclosing) {
    auto Result = SM.rangeContainsRespectingReplacedRanges(
        Enclosing.getSourceRange(), Target);
    if (Result && IsContext(Enclosing)) {
      AllContexts.push_back(Enclosing);
    }
    return Result;
  }

public:
  ContextFinder(
      SourceFile &SF, ASTNode TargetNode,
      std::function<bool(ASTNode)> IsContext = [](ASTNode N) { return true; })
      : SF(SF), Ctx(SF.getASTContext()), SM(Ctx.SourceMgr),
        Target(TargetNode.getSourceRange()), IsContext(IsContext) {}

  ContextFinder(
      SourceFile &SF, SourceLoc TargetLoc,
      std::function<bool(ASTNode)> IsContext = [](ASTNode N) { return true; })
      : SF(SF), Ctx(SF.getASTContext()), SM(Ctx.SourceMgr), Target(TargetLoc),
        IsContext(IsContext) {
    assert(TargetLoc.isValid() && "Invalid loc to find");
  }

  // Only need expansions for the expands refactoring, but we
  // skip nodes that don't contain the passed location anyway.
  virtual MacroWalking getMacroWalkingBehavior() const override {
    return MacroWalking::ArgumentsAndExpansion;
  }

  bool walkToDeclPre(Decl *D, CharSourceRange Range) override {
    return contains(D);
  }
  bool walkToStmtPre(Stmt *S) override { return contains(S); }
  bool walkToExprPre(Expr *E) override { return contains(E); }
  void resolve() { walk(SF); }
  ArrayRef<ASTNode> getContexts() const { return llvm::ArrayRef(AllContexts); }
};

} // namespace refactoring
} // namespace nextcode

#endif
