//===--- CompletionContextFinder.h ----------------------------------------===//
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

#ifndef NEXTCODE_SEMA_COMPLETIONCONTEXTFINDER_H
#define NEXTCODE_SEMA_COMPLETIONCONTEXTFINDER_H

#include "nextcode/AST/ASTNode.h"
#include "nextcode/AST/ASTWalker.h"
#include "nextcode/AST/Expr.h"
#include "nextcode/IDE/TypeCheckCompletionCallback.h"

namespace nextcode {

namespace constraints {
class SyntacticElementTarget;
}

class CompletionContextFinder : public ASTWalker {
  enum class ContextKind {
    FallbackExpression,
    StringInterpolation,
    SingleStmtClosure,
    MultiStmtClosure,
    ErrorExpression
  };

  struct Context {
    ContextKind Kind;
    Expr *E;
  };

  /// Stack of all "interesting" contexts up to code completion expression.
  llvm::SmallVector<Context, 4> Contexts;

  /// If we are completing inside an expression, the \c CodeCompletionExpr that
  /// represents the code completion token.

  /// The AST node that represents the code completion token, either as a
  /// \c CodeCompletionExpr or a \c KeyPathExpr which contains a code completion
  /// component.
  llvm::PointerUnion<CodeCompletionExpr *, const KeyPathExpr *> CompletionNode;

  Expr *InitialExpr = nullptr;
  DeclContext *InitialDC;

public:
  MacroWalking getMacroWalkingBehavior() const override {
    return MacroWalking::Arguments;
  }

  /// Finder for completion contexts within the provided SyntacticElementTarget.
  CompletionContextFinder(constraints::SyntacticElementTarget target,
                          DeclContext *DC);

  /// Finder for completion contexts within the outermost non-closure context of
  /// the code completion expression's direct context.
  CompletionContextFinder(DeclContext *completionDC) : InitialDC(completionDC) {
    while (auto *ACE = dyn_cast<AbstractClosureExpr>(InitialDC))
      InitialDC = ACE->getParent();
    InitialDC->walkContext(*this);
  }

  PreWalkResult<Expr *> walkToExprPre(Expr *E) override;

  PostWalkResult<Expr *> walkToExprPost(Expr *E) override;

  bool locatedInStringInterpolation() const {
    return hasContext(ContextKind::StringInterpolation);
  }

  bool hasCompletionExpr() const {
    return CompletionNode.dyn_cast<CodeCompletionExpr *>() != nullptr;
  }

  CodeCompletionExpr *getCompletionExpr() const {
    assert(hasCompletionExpr());
    return CompletionNode.get<CodeCompletionExpr *>();
  }

  bool hasCompletionKeyPathComponent() const {
    return CompletionNode.dyn_cast<const KeyPathExpr *>() != nullptr;
  }

  bool hasCompletion() const {
    return !CompletionNode.isNull();
  }

  /// If we are completing in a key path, returns the \c KeyPath that contains
  /// the code completion component.
  const KeyPathExpr *getKeyPathContainingCompletionComponent() const {
    assert(hasCompletionKeyPathComponent());
    return CompletionNode.get<const KeyPathExpr *>();
  }

  /// If we are completing in a key path, returns the index at which the key
  /// path has the code completion component.
  size_t getKeyPathCompletionComponentIndex() const;

  struct Fallback {
    Expr *E;               ///< The fallback expression.
    DeclContext *DC;       ///< The fallback expression's decl context.
    bool SeparatePrecheck; ///< True if the fallback may require prechecking.
  };

  /// As a fallback sometimes its useful to not only type-check
  /// code completion expression directly but instead add some
  /// of the enclosing context e.g. when completion is an argument
  /// to a call.
  std::optional<Fallback> getFallbackCompletionExpr() const;

private:
  bool hasContext(ContextKind kind) const {
    return llvm::find_if(Contexts, [&kind](const Context &currContext) {
             return currContext.Kind == kind;
           }) != Contexts.end();
  }
};


/// Returns \c true if \p range is valid and contains the IDE inspection
/// target. This performs the underlying check based on \c CharSourceRange
/// to make sure we correctly return \c true if the ide inspection target
/// is inside a string literal that's the last token in \p range.
bool containsIDEInspectionTarget(SourceRange range,
                                 const SourceManager &SourceMgr);

} // end namespace nextcode

#endif // NEXTCODE_SEMA_COMPLETIONCONTEXTFINDER_H
