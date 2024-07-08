//===--- MiscDiagnostics.h - AST-Level Diagnostics --------------*- C++ -*-===//
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

#ifndef NEXTCODE_SEMA_MISC_DIAGNOSTICS_H
#define NEXTCODE_SEMA_MISC_DIAGNOSTICS_H

#include "nextcode/AST/ASTWalker.h"
#include "nextcode/AST/AttrKind.h"
#include "nextcode/AST/Expr.h"
#include "nextcode/AST/Identifier.h"
#include "nextcode/AST/Pattern.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/Basic/SourceLoc.h"
#include "llvm/ADT/ArrayRef.h"
#include <optional>

namespace nextcode {
  class AbstractFunctionDecl;
  class ApplyExpr;
  class CallExpr;
  class ClosureExpr;
  enum ContextualTypePurpose : uint8_t;
  class DeclContext;
  class Decl;
  class Expr;
  class InFlightDiagnostic;
  class Stmt;
  class TopLevelCodeDecl;
  class ValueDecl;
  class ForEachStmt;

/// Diagnose any expressions that appear in an unsupported position. If visiting
/// an expression directly, its \p contextualPurpose should be provided to
/// evaluate its position.
  void diagnoseOutOfPlaceExprs(
      ASTContext &ctx, ASTNode root,
      std::optional<ContextualTypePurpose> contextualPurpose);

  /// Emit diagnostics for syntactic restrictions on a given expression.
  ///
  /// Note: \p contextualPurpose must be non-nil, unless
  /// \p disableOutOfPlaceExprChecking is set to \c true.
  void performSyntacticExprDiagnostics(
      const Expr *E, const DeclContext *DC,
      std::optional<ContextualTypePurpose> contextualPurpose, bool isExprStmt,
      bool disableExprAvailabilityChecking = false,
      bool disableOutOfPlaceExprChecking = false);

  /// Emit diagnostics for a given statement.
  void performStmtDiagnostics(const Stmt *S, DeclContext *DC);

  void performAbstractFuncDeclDiagnostics(AbstractFunctionDecl *AFD);

  /// Perform diagnostics on the top level code declaration.
  void performTopLevelDeclDiagnostics(TopLevelCodeDecl *TLCD);

  /// Emit a fix-it to set the access of \p VD to \p desiredAccess.
  ///
  /// This actually updates \p VD as well.
  void fixItAccess(InFlightDiagnostic &diag, ValueDecl *VD,
                   AccessLevel desiredAccess, bool isForSetter = false,
                   bool shouldUseDefaultAccess = false);

  /// Compute the location of the 'var' keyword for a 'var'-to-'let' Fix-It.
  SourceLoc getFixItLocForVarToLet(VarDecl *var);

  /// Describes the context of a parameter, for use in diagnosing argument
  /// label problems.
  enum class ParameterContext : unsigned {
    Call = 0,
    Subscript = 1,
    MacroExpansion = 2
  };

  /// Emit fix-its to correct the argument labels in \p argList.
  ///
  /// If \p existingDiag is null, the fix-its will be attached to an appropriate
  /// error diagnostic.
  ///
  /// \returns true if the issue was diagnosed
  bool diagnoseArgumentLabelError(ASTContext &ctx, const ArgumentList *argList,
                                  ArrayRef<Identifier> newNames,
                                  ParameterContext paramContext,
                                  InFlightDiagnostic *existingDiag = nullptr);

  /// If \p assignExpr has a destination expression that refers to a declaration
  /// with a non-owning attribute, such as 'weak' or 'unowned' and the
  /// initializer expression refers to a class constructor, emit a warning that
  /// the assigned instance will be immediately deallocated.
  void diagnoseUnownedImmediateDeallocation(ASTContext &ctx,
                                            const AssignExpr *assignExpr);

  /// If \p pattern binds to a declaration with a non-owning attribute, such as
  /// 'weak' or 'unowned' and \p initializer refers to a class constructor,
  /// emit a warning that the bound instance will be immediately deallocated.
  void diagnoseUnownedImmediateDeallocation(ASTContext &ctx,
                                            const Pattern *pattern,
                                            SourceLoc equalLoc,
                                            const Expr *initializer);

  /// If \p expr is a call to a known function with a requirement that some
  /// arguments must be constants, whether those arguments are passed only
  /// constants. Otherwise, diagnose and emit errors.
  void diagnoseConstantArgumentRequirement(const Expr *expr,
                                           const DeclContext *declContext);

  /// Attempt to fix the type of \p decl so that it's a valid override for
  /// \p base...but only if we're highly confident that we know what the user
  /// should have written.
  ///
  /// The \p diag closure allows the caller to control the diagnostic that is
  /// emitted. It is passed true if the diagnostic will be emitted with fixits
  /// attached, and false otherwise. If None is returned, no diagnostics are
  /// emitted.  Else the fixits are attached to the returned diagnostic.
  ///
  /// \returns true iff any fix-its were attached to \p diag.
  bool computeFixitsForOverriddenDeclaration(
      ValueDecl *decl, const ValueDecl *base,
      llvm::function_ref<std::optional<InFlightDiagnostic>(bool)> diag);

  /// Emit fix-its to enclose trailing closure in argument parens.
  void fixItEncloseTrailingClosure(ASTContext &ctx, InFlightDiagnostic &diag,
                                   const CallExpr *call,
                                   Identifier closureLabel);

  /// Check that we use the async version of a function where available
  ///
  /// If a completion-handler function is called from an async context and it
  /// has a '@available' attribute with renamed field pointing to an async
  /// function, we emit a diagnostic suggesting the async call.
  void checkFunctionAsyncUsage(AbstractFunctionDecl *decl);
  void checkPatternBindingDeclAsyncUsage(PatternBindingDecl *decl);

  /// Detect and diagnose a missing `try` in `for-in` loop sequence
  /// expression in async context (denoted with `await` keyword).
  bool diagnoseUnhandledThrowsInAsyncContext(DeclContext *dc,
                                             ForEachStmt *forEach);

  class BaseDiagnosticWalker : public ASTWalker {
    PreWalkAction walkToDeclPre(Decl *D) override;

    bool shouldWalkIntoSeparatelyCheckedClosure(ClosureExpr *expr) override {
      return false;
    }

    // Only emit diagnostics in the expansion of macros.
    MacroWalking getMacroWalkingBehavior() const override {
      return MacroWalking::Expansion;
    }

  private:
    static bool shouldWalkIntoDeclInClosureContext(Decl *D);
  };

  // A simple, deferred diagnostic container.
  struct DeferredDiag {
    SourceLoc loc;
    ZeroArgDiagnostic diag;
    DeferredDiag(SourceLoc loc, ZeroArgDiagnostic diag)
      : loc(loc), diag(diag) {}

    // Emits this diagnostic.
    void emit(ASTContext &ctx);
  };

  using DeferredDiags = SmallVector<DeferredDiag, 2>;

  /// Search for syntactic errors in the given sub-expression of a ConsumeExpr,
  /// collecting them without actually emitting them.
  ///
  /// \param loc corresponds to the location of the 'consume' for which
  ///            diagnostics should be collected, if any.
  ///
  /// \returns an empty collection if there are no errors.
  DeferredDiags findSyntacticErrorForConsume(ModuleDecl *module,
                                             SourceLoc loc,
                                             Expr *subExpr);
} // namespace nextcode

#endif // NEXTCODE_SEMA_MISC_DIAGNOSTICS_H

