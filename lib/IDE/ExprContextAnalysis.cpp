//===--- ExprContextAnalysis.cpp - Expession context analysis -------------===//
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

#include "ExprContextAnalysis.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/ASTVisitor.h"
#include "nextcode/AST/ASTWalker.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/DeclContext.h"
#include "nextcode/AST/Expr.h"
#include "nextcode/AST/GenericSignature.h"
#include "nextcode/AST/Initializer.h"
#include "nextcode/AST/LazyResolver.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/NameLookup.h"
#include "nextcode/AST/ParameterList.h"
#include "nextcode/AST/Pattern.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/AST/Stmt.h"
#include "nextcode/AST/Type.h"
#include "nextcode/AST/Types.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/SourceManager.h"
#include "nextcode/IDE/CodeCompletionResult.h"
#include "nextcode/Sema/IDETypeChecking.h"
#include "nextcode/Subsystems.h"
#include "clang/AST/Attr.h"
#include "clang/AST/Decl.h"
#include "llvm/ADT/SmallSet.h"

using namespace nextcode;
using namespace ide;

//===----------------------------------------------------------------------===//
// typeCheckContextAt(DeclContext, SourceLoc)
//===----------------------------------------------------------------------===//

void nextcode::ide::typeCheckContextAt(TypeCheckASTNodeAtLocContext TypeCheckCtx,
                                    SourceLoc Loc) {
  // Make sure the extension has been bound.
  auto DC = TypeCheckCtx.getDeclContext();
  // Even if the extension is invalid (e.g. nested in a function or another
  // type), we want to know the "intended nominal" of the extension so that
  // we can know the type of 'Self'.
  SmallVector<ExtensionDecl *, 1> extensions;
  for (auto typeCtx = DC->getInnermostTypeContext(); typeCtx != nullptr;
       typeCtx = typeCtx->getParent()->getInnermostTypeContext()) {
    if (auto *ext = dyn_cast<ExtensionDecl>(typeCtx))
      extensions.push_back(ext);
  }
  while (!extensions.empty()) {
    extensions.back()->computeExtendedNominal();
    extensions.pop_back();
  }

  // If the completion happens in the inheritance clause of the extension,
  // 'DC' is the parent of the extension. We need to iterate the top level
  // decls to find it. In theory, we don't need the extended nominal in the
  // inheritance clause, but ASTScope lookup requires that. We don't care
  // unless 'DC' is not 'SourceFile' because non-toplevel extensions are
  // 'canNeverBeBound()' anyway.
  if (auto *SF = dyn_cast<SourceFile>(DC)) {
    auto &SM = DC->getASTContext().SourceMgr;
    for (auto *decl : SF->getTopLevelDecls())
      if (auto *ext = dyn_cast<ExtensionDecl>(decl))
        if (SM.rangeContainsTokenLoc(ext->getSourceRange(), Loc))
          ext->computeExtendedNominal();
  }

  nextcode::typeCheckASTNodeAtLoc(TypeCheckCtx, Loc);
}

//===----------------------------------------------------------------------===//
// findParsedExpr(DeclContext, Expr)
//===----------------------------------------------------------------------===//

namespace {
class ExprFinder : public ASTWalker {
  SourceManager &SM;
  SourceRange TargetRange;
  Expr *FoundExpr = nullptr;

  template <typename NodeType> bool isInterstingRange(NodeType *Node) {
    return SM.rangeContains(Node->getSourceRange(), TargetRange);
  }

  bool shouldIgnore(Expr *E) {
    // E.g. instanceOfDerived.methodInBaseReturningSelf().#^HERE^#'
    // When calling a method in a base class returning 'Self', the call
    // expression itself has the type of the base class. That is wrapped with
    // CovariantReturnConversionExpr which downcasts it to the derived class.
    if (isa<CovariantReturnConversionExpr>(E))
      return false;

    // E.g. TypeName(#^HERE^#
    // In this case, we want the type expression instead of a reference to the
    // initializer.
    if (isa<ConstructorRefCallExpr>(E))
      return true;

    // Ignore other implicit expression.
    if (E->isImplicit())
      return true;

    return false;
  }

public:
  ExprFinder(SourceManager &SM, SourceRange TargetRange)
      : SM(SM), TargetRange(TargetRange) {}

  Expr *get() const { return FoundExpr; }

  MacroWalking getMacroWalkingBehavior() const override {
    return MacroWalking::ArgumentsAndExpansion;
  }

  PreWalkResult<Expr *> walkToExprPre(Expr *E) override {
    if (TargetRange == E->getSourceRange() && !shouldIgnore(E)) {
      assert(!FoundExpr && "non-nullptr for found expr");
      FoundExpr = E;
      return Action::Stop();
    }
    return Action::VisitNodeIf(isInterstingRange(E), E);
  }

  PreWalkResult<Pattern *> walkToPatternPre(Pattern *P) override {
    return Action::VisitNodeIf(isInterstingRange(P), P);
  }

  PreWalkResult<Stmt *> walkToStmtPre(Stmt *S) override {
    return Action::VisitNodeIf(isInterstingRange(S), S);
  }

  PreWalkAction walkToTypeReprPre(TypeRepr *T) override {
    return Action::SkipNode();
  }
};
} // anonymous namespace

Expr *nextcode::ide::findParsedExpr(const DeclContext *DC,
                                 SourceRange TargetRange) {
  ExprFinder finder(DC->getASTContext().SourceMgr, TargetRange);
  const_cast<DeclContext *>(DC)->walkContext(finder);
  return finder.get();
}
