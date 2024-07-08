//===--- InstrumenterSupport.h - Instrumenter Support -----------*- C++ -*-===//
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
//  This file implements the supporting functions for writing instrumenters of
//  the NeXTCode AST.
//
//===----------------------------------------------------------------------===//

#include "TypeChecker.h"
#include "nextcode/AST/ASTWalker.h"

namespace nextcode {
namespace instrumenter_support {

template <class E> class Added {
private:
  E Contents;

public:
  Added() {}
  Added(E NewContents) : Contents(NewContents) {}
  Added(const Added<E> &rhs) : Contents(rhs.Contents) {}
  const Added<E> &operator=(const Added<E> &rhs) {
    Contents = rhs.Contents;
    return *this;
  }
  E &operator*() { return Contents; }
  E &operator->() { return Contents; }
};

class InstrumenterBase {

protected:
  ASTContext &Context;
  DeclContext *TypeCheckDC;
  std::optional<DeclNameRef> ModuleIdentifier;
  std::optional<DeclNameRef> FileIdentifier;

  InstrumenterBase(ASTContext &C, DeclContext *DC);
  virtual ~InstrumenterBase() = default;
  virtual void anchor();
  virtual BraceStmt *transformBraceStmt(BraceStmt *BS,
                                        const ParameterList *PL = nullptr,
                                        bool TopLevel = false) = 0;

  /// Create an expression which retrieves a valid ModuleIdentifier or
  /// FileIdentifier, if available.
  Expr *buildIDArgumentExpr(std::optional<DeclNameRef> name, SourceRange SR);

  class ClosureFinder : public ASTWalker {
  private:
    InstrumenterBase &I;

  public:
    ClosureFinder(InstrumenterBase &Inst) : I(Inst) {}

    /// Walk only the expansion of the macro.
    MacroWalking getMacroWalkingBehavior() const override {
      return MacroWalking::Expansion;
    }

    PreWalkResult<Stmt *> walkToStmtPre(Stmt *S) override {
      if (isa<BraceStmt>(S)) {
        return Action::SkipNode(S); // don't walk into brace statements; we
                                    // need to respect nesting!
      } else {
        return Action::Continue(S);
      }
    }
    PreWalkResult<Expr *> walkToExprPre(Expr *E) override {
      if (auto *CE = dyn_cast<ClosureExpr>(E)) {
        BraceStmt *B = CE->getBody();
        if (B) {
          const ParameterList *PL = CE->getParameters();
          BraceStmt *NB = I.transformBraceStmt(B, PL);
          CE->setBody(NB);
          // just with the entry and exit logging this is going to
          // be more than a single expression!
        }
      }
      return Action::Continue(E);
    }
  };

  ClosureFinder CF;

  template <class T>
  bool doTypeCheck(ASTContext &Ctx, DeclContext *DC, Added<T *> &parsedExpr) {
    Expr *E = *parsedExpr;
    bool result = doTypeCheckImpl(Ctx, DC, E);
    parsedExpr = Added<T *>(dyn_cast<T>(E));
    return result;
  }
  
private:
  bool doTypeCheckImpl(ASTContext &Ctx, DeclContext *DC, Expr * &parsedExpr);
};
}
}
