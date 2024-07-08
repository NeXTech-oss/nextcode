//===--- CompletionContextFinder.cpp --------------------------------------===//
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

#include "nextcode/Sema/CompletionContextFinder.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Parse/Lexer.h"
#include "nextcode/Sema/SyntacticElementTarget.h"

using namespace nextcode;
using namespace constraints;
using Fallback = CompletionContextFinder::Fallback;

CompletionContextFinder::CompletionContextFinder(
    SyntacticElementTarget target, DeclContext *DC)
    : InitialExpr(target.getAsExpr()), InitialDC(DC) {
  assert(DC);
  target.walk(*this);
}

ASTWalker::PreWalkResult<Expr *>
CompletionContextFinder::walkToExprPre(Expr *E) {
  if (auto *closure = dyn_cast<ClosureExpr>(E)) {
    // NOTE: We're querying hasSingleExpressionBody before the single-expression
    // body transform has happened, so this won't take e.g SingleValueStmtExprs
    // into account.
    Contexts.push_back({closure->hasSingleExpressionBody()
                            ? ContextKind::SingleStmtClosure
                            : ContextKind::MultiStmtClosure,
                        closure});
  }

  if (isa<InterpolatedStringLiteralExpr>(E)) {
    Contexts.push_back({ContextKind::StringInterpolation, E});
  }

  if (isa<ApplyExpr>(E) || isa<SequenceExpr>(E) ||
      isa<SingleValueStmtExpr>(E)) {
    Contexts.push_back({ContextKind::FallbackExpression, E});
  }

  if (auto *Error = dyn_cast<ErrorExpr>(E)) {
    Contexts.push_back({ContextKind::ErrorExpression, E});
    if (auto *OrigExpr = Error->getOriginalExpr()) {
      OrigExpr->walk(*this);
      if (hasCompletionExpr())
        return Action::Stop();
    }
  }

  if (auto *CCE = dyn_cast<CodeCompletionExpr>(E)) {
    CompletionNode = CCE;
    return Action::Stop();
  }
  if (auto *KeyPath = dyn_cast<KeyPathExpr>(E)) {
    for (auto &component : KeyPath->getComponents()) {
      if (component.getKind() == KeyPathExpr::Component::Kind::CodeCompletion) {
        CompletionNode = KeyPath;
        return Action::Stop();
      }
    }
    // Code completion in key paths is modelled by a code completion component
    // Don't walk the key path's parsed expressions.
    return Action::SkipNode(E);
  }

  return Action::Continue(E);
}

ASTWalker::PostWalkResult<Expr *>
CompletionContextFinder::walkToExprPost(Expr *E) {
  if (isa<ClosureExpr>(E) || isa<InterpolatedStringLiteralExpr>(E) ||
      isa<ApplyExpr>(E) || isa<SequenceExpr>(E) || isa<ErrorExpr>(E) ||
      isa<SingleValueStmtExpr>(E)) {
    assert(Contexts.back().E == E);
    Contexts.pop_back();
  }
  return Action::Continue(E);
}

size_t CompletionContextFinder::getKeyPathCompletionComponentIndex() const {
  size_t ComponentIndex = 0;
  auto Components = getKeyPathContainingCompletionComponent()->getComponents();
  for (auto &Component : Components) {
    if (Component.getKind() == KeyPathExpr::Component::Kind::CodeCompletion) {
      break;
    } else {
      ComponentIndex++;
    }
  }
  assert(ComponentIndex < Components.size() &&
         "No completion component in the key path?");
  return ComponentIndex;
}

std::optional<Fallback>
CompletionContextFinder::getFallbackCompletionExpr() const {
  if (!hasCompletionExpr()) {
    // Creating a fallback expression only makes sense if we are completing in
    // an expression, not when we're completing in a key path.
    return std::nullopt;
  }

  std::optional<Fallback> fallback;
  bool separatePrecheck = false;
  DeclContext *fallbackDC = InitialDC;

  // Find the outermost fallback expression within the innermost error
  // expression or multi-statement closure, keeping track of its decl context.
  for (auto context : Contexts) {
    switch (context.Kind) {
    case ContextKind::StringInterpolation:
      LLVM_FALLTHROUGH;
    case ContextKind::FallbackExpression:
      if (!fallback && context.E != InitialExpr)
        fallback = Fallback{context.E, fallbackDC, separatePrecheck};
      continue;

    case ContextKind::SingleStmtClosure:
      if (!fallback && context.E != InitialExpr)
        fallback = Fallback{context.E, fallbackDC, separatePrecheck};
      fallbackDC = cast<AbstractClosureExpr>(context.E);
      continue;

    case ContextKind::MultiStmtClosure:
      fallbackDC = cast<AbstractClosureExpr>(context.E);
      LLVM_FALLTHROUGH;
    case ContextKind::ErrorExpression:;
      fallback = std::nullopt;
      separatePrecheck = true;
      continue;
    }
  }

  if (fallback)
    return fallback;

  if (getCompletionExpr() != InitialExpr)
    return Fallback{getCompletionExpr(), fallbackDC, separatePrecheck};
  return std::nullopt;
}

bool nextcode::containsIDEInspectionTarget(SourceRange range,
                                        const SourceManager &SourceMgr) {
  if (range.isInvalid())
    return false;
  auto charRange = Lexer::getCharSourceRangeFromSourceRange(SourceMgr, range);
  return SourceMgr.rangeContainsIDEInspectionTarget(charRange);
}
