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

#include "AsyncRefactoring.h"
#include "RefactoringActions.h"
#include "nextcode/AST/Stmt.h"
#include "nextcode/Basic/Assertions.h"

using namespace nextcode::refactoring;

/// Find the outermost call of the given location
static CallExpr *findOuterCall(ResolvedCursorInfoPtr CursorInfo) {
  auto IncludeInContext = [](ASTNode N) {
    if (auto *E = N.dyn_cast<Expr *>())
      return !E->isImplicit();
    return false;
  };

  // TODO: Bit pointless using the "ContextFinder" here. Ideally we would have
  //       already generated a slice of the AST for anything that contains
  //       the cursor location
  ContextFinder Finder(*CursorInfo->getSourceFile(), CursorInfo->getLoc(),
                       IncludeInContext);
  Finder.resolve();
  auto Contexts = Finder.getContexts();
  if (Contexts.empty())
    return nullptr;

  CallExpr *CE = dyn_cast<CallExpr>(Contexts[0].get<Expr *>());
  if (!CE)
    return nullptr;

  SourceManager &SM = CursorInfo->getSourceFile()->getASTContext().SourceMgr;
  if (!SM.rangeContains(CE->getFn()->getSourceRange(), CursorInfo->getLoc()))
    return nullptr;
  return CE;
}

/// Find the function matching the given location if it is not an accessor and
/// either has a body or is a member of a protocol
static FuncDecl *findFunction(ResolvedCursorInfoPtr CursorInfo) {
  auto IncludeInContext = [](ASTNode N) {
    if (auto *D = N.dyn_cast<Decl *>())
      return !D->isImplicit();
    return false;
  };

  ContextFinder Finder(*CursorInfo->getSourceFile(), CursorInfo->getLoc(),
                       IncludeInContext);
  Finder.resolve();

  auto Contexts = Finder.getContexts();
  if (Contexts.empty())
    return nullptr;

  if (Contexts.back().isDecl(DeclKind::Param))
    Contexts = Contexts.drop_back();

  auto *FD = dyn_cast_or_null<FuncDecl>(Contexts.back().get<Decl *>());
  if (!FD || isa<AccessorDecl>(FD))
    return nullptr;

  auto *Body = FD->getBody();
  if (!Body && !isa<ProtocolDecl>(FD->getDeclContext()))
    return nullptr;

  SourceManager &SM = CursorInfo->getSourceFile()->getASTContext().SourceMgr;
  SourceLoc DeclEnd = Body ? Body->getLBraceLoc() : FD->getEndLoc();
  if (!SM.rangeContains(SourceRange(FD->getStartLoc(), DeclEnd),
                        CursorInfo->getLoc()))
    return nullptr;

  return FD;
}

bool RefactoringActionConvertCallToAsyncAlternative::isApplicable(
    ResolvedCursorInfoPtr CursorInfo, DiagnosticEngine &Diag) {
  using namespace asyncrefactorings;

  // Currently doesn't check that the call is in an async context. This seems
  // possibly useful in some situations, so we'll see what the feedback is.
  // May need to change in the future
  auto *CE = findOuterCall(CursorInfo);
  if (!CE)
    return false;

  auto HandlerDesc = AsyncHandlerParamDesc::find(
      getUnderlyingFunc(CE->getFn()), /*RequireAttributeOrName=*/false);
  return HandlerDesc.isValid();
}

/// Converts a call of a function with a possible async alternative, to use it
/// instead. Currently this is any function that
///   1. has a void return type,
///   2. has a void returning closure as its last parameter, and
///   3. is not already async
///
/// For now the call need not be in an async context, though this may change
/// depending on feedback.
bool RefactoringActionConvertCallToAsyncAlternative::performChange() {
  using namespace asyncrefactorings;

  auto *CE = findOuterCall(CursorInfo);
  assert(CE &&
         "Should not run performChange when refactoring is not applicable");

  // Find the scope this call is in
  ContextFinder Finder(
      *CursorInfo->getSourceFile(), CursorInfo->getLoc(),
      [](ASTNode N) { return N.isStmt(StmtKind::Brace) && !N.isImplicit(); });
  Finder.resolve();
  auto Scopes = Finder.getContexts();
  BraceStmt *Scope = nullptr;
  if (!Scopes.empty())
    Scope = cast<BraceStmt>(Scopes.back().get<Stmt *>());

  AsyncConverter Converter(TheFile, SM, DiagEngine, CE, Scope);
  if (!Converter.convert())
    return true;

  Converter.replace(CE, EditConsumer);
  return false;
}

bool RefactoringActionConvertToAsync::isApplicable(
    ResolvedCursorInfoPtr CursorInfo, DiagnosticEngine &Diag) {
  using namespace asyncrefactorings;

  // As with the call refactoring, should possibly only apply if there's
  // actually calls to async alternatives. At the moment this will just add
  // `async` if there are no calls, which is probably fine.
  return findFunction(CursorInfo);
}

/// Converts a whole function to async, converting any calls to functions with
/// async alternatives as above.
bool RefactoringActionConvertToAsync::performChange() {
  using namespace asyncrefactorings;

  auto *FD = findFunction(CursorInfo);
  assert(FD &&
         "Should not run performChange when refactoring is not applicable");

  auto HandlerDesc =
      AsyncHandlerParamDesc::find(FD, /*RequireAttributeOrName=*/false);
  AsyncConverter Converter(TheFile, SM, DiagEngine, FD, HandlerDesc);
  if (!Converter.convert())
    return true;

  Converter.replace(FD, EditConsumer, FD->getSourceRangeIncludingAttrs().Start);
  return false;
}

bool RefactoringActionAddAsyncAlternative::isApplicable(
    ResolvedCursorInfoPtr CursorInfo, DiagnosticEngine &Diag) {
  using namespace asyncrefactorings;

  auto *FD = findFunction(CursorInfo);
  if (!FD)
    return false;

  auto HandlerDesc =
      AsyncHandlerParamDesc::find(FD, /*RequireAttributeOrName=*/false);
  return HandlerDesc.isValid();
}

/// Adds an async alternative and marks the current function as deprecated.
/// Equivalent to the conversion but
///   1. only works on functions that themselves are a possible async
///      alternative, and
///   2. has extra handling to convert the completion/handler/callback closure
///      parameter to either `return`/`throws`
bool RefactoringActionAddAsyncAlternative::performChange() {
  using namespace asyncrefactorings;

  auto *FD = findFunction(CursorInfo);
  assert(FD &&
         "Should not run performChange when refactoring is not applicable");

  auto HandlerDesc =
      AsyncHandlerParamDesc::find(FD, /*RequireAttributeOrName=*/false);
  assert(HandlerDesc.isValid() &&
         "Should not run performChange when refactoring is not applicable");

  AsyncConverter Converter(TheFile, SM, DiagEngine, FD, HandlerDesc);
  if (!Converter.convert())
    return true;

  // Add a reference to the async function so that warnings appear when the
  // synchronous function is used in an async context
  SmallString<128> AvailabilityAttr = HandlerDesc.buildRenamedAttribute();
  EditConsumer.accept(SM, FD->getAttributeInsertionLoc(false),
                      AvailabilityAttr);

  AsyncConverter LegacyBodyCreator(TheFile, SM, DiagEngine, FD, HandlerDesc);
  if (LegacyBodyCreator.createLegacyBody()) {
    LegacyBodyCreator.replace(FD->getBody(), EditConsumer);
  }

  // Add the async alternative
  Converter.insertAfter(FD, EditConsumer);

  return false;
}

bool RefactoringActionAddAsyncWrapper::isApplicable(
    ResolvedCursorInfoPtr CursorInfo, DiagnosticEngine &Diag) {
  using namespace asyncrefactorings;

  auto *FD = findFunction(CursorInfo);
  if (!FD)
    return false;

  auto HandlerDesc =
      AsyncHandlerParamDesc::find(FD, /*RequireAttributeOrName=*/false);
  return HandlerDesc.isValid();
}

bool RefactoringActionAddAsyncWrapper::performChange() {
  using namespace asyncrefactorings;

  auto *FD = findFunction(CursorInfo);
  assert(FD &&
         "Should not run performChange when refactoring is not applicable");

  auto HandlerDesc =
      AsyncHandlerParamDesc::find(FD, /*RequireAttributeOrName=*/false);
  assert(HandlerDesc.isValid() &&
         "Should not run performChange when refactoring is not applicable");

  AsyncConverter Converter(TheFile, SM, DiagEngine, FD, HandlerDesc);
  if (!Converter.createAsyncWrapper())
    return true;

  // Add a reference to the async function so that warnings appear when the
  // synchronous function is used in an async context
  SmallString<128> AvailabilityAttr = HandlerDesc.buildRenamedAttribute();
  EditConsumer.accept(SM, FD->getAttributeInsertionLoc(false),
                      AvailabilityAttr);

  // Add the async wrapper.
  Converter.insertAfter(FD, EditConsumer);

  return false;
}
