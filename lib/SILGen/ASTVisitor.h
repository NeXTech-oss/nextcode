//===--- ASTVisitor.h - SILGen ASTVisitor specialization --------*- C++ -*-===//
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
//
// This file defines nextcode::Lowering::ASTVisitor.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_LOWERING_ASTVISITOR_H
#define NEXTCODE_LOWERING_ASTVISITOR_H

#include "nextcode/AST/ASTVisitor.h"

namespace nextcode {
namespace Lowering {
  
/// Lowering::ASTVisitor - This is a specialization of
/// nextcode::ASTVisitor which works only on resolved nodes and
/// which automatically ignores certain AST node kinds.
template<typename ImplClass,
         typename ExprRetTy = void,
         typename StmtRetTy = void,
         typename DeclRetTy = void,
         typename PatternRetTy = void,
         typename... Args>
class ASTVisitor : public nextcode::ASTVisitor<ImplClass,
                                            ExprRetTy,
                                            StmtRetTy,
                                            DeclRetTy,
                                            PatternRetTy,
                                            void,
                                            void,
                                            Args...>
{
public:
#define EXPR(Id, Parent)
#define UNCHECKED_EXPR(Id, Parent) \
  ExprRetTy visit##Id##Expr(Id##Expr *E, Args...AA) { \
    llvm_unreachable(#Id "Expr should not survive to SILGen"); \
  }
#include "nextcode/AST/ExprNodes.def"
  
  ExprRetTy visitErrorExpr(ErrorExpr *E, Args...AA) {
    llvm_unreachable("expression kind should not survive to SILGen");
  }

  ExprRetTy visitCodeCompletionExpr(CodeCompletionExpr *E, Args...AA) {
    llvm_unreachable("expression kind should not survive to SILGen");
  }

  ExprRetTy visitDefaultArgumentExpr(DefaultArgumentExpr *E, Args...AA) {
    llvm_unreachable("DefaultArgumentExpr should not appear in this position");
  }

  ExprRetTy visitVarargExpansionExpr(VarargExpansionExpr *E, Args... AA) {
    return static_cast<ImplClass*>(this)->visit(E->getSubExpr(),
                                                std::forward<Args>(AA)...);
  }

  ExprRetTy visitIdentityExpr(IdentityExpr *E, Args...AA) {
    return static_cast<ImplClass*>(this)->visit(E->getSubExpr(),
                                                std::forward<Args>(AA)...);
  }

  ExprRetTy visitTryExpr(TryExpr *E, Args...AA) {
    return static_cast<ImplClass*>(this)->visit(E->getSubExpr(),
                                                std::forward<Args>(AA)...);
  }

  ExprRetTy visitLazyInitializerExpr(LazyInitializerExpr *E, Args...AA) {
    return static_cast<ImplClass*>(this)->visit(E->getSubExpr(),
                                                std::forward<Args>(AA)...);
  }
};

template <typename ImplClass,
          typename ExprRetTy = void,
          typename... Args>
using ExprVisitor = ASTVisitor<ImplClass, ExprRetTy, void, void, void, Args...>;

} // end namespace Lowering
} // end namespace nextcode

#endif
