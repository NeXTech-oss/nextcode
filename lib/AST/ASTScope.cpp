//===--- ASTScopeImpl.cpp - NeXTCode Object-Oriented AST Scope ---------------===//
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
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//
///
/// This file implements the common functions of the 49 ontology.
///
//===----------------------------------------------------------------------===//
#include "nextcode/AST/ASTScope.h"

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/ASTWalker.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/Expr.h"
#include "nextcode/AST/Initializer.h"
#include "nextcode/AST/LazyResolver.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/NameLookup.h"
#include "nextcode/AST/ParameterList.h"
#include "nextcode/AST/Pattern.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/AST/Stmt.h"
#include "nextcode/AST/TypeRepr.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/NullablePtr.h"
#include "nextcode/Basic/STLExtras.h"
#include "llvm/Support/Compiler.h"
#include <algorithm>

using namespace nextcode;
using namespace ast_scope;

#pragma mark ASTScope

void ASTScope::unqualifiedLookup(
    SourceFile *SF, SourceLoc loc,
    namelookup::AbstractASTScopeDeclConsumer &consumer) {
  if (loc.isValid()) {
    SF = SF->getParentModule()->getSourceFileContainingLocation(loc);
  }

  if (auto *s = SF->getASTContext().Stats)
    ++s->getFrontendCounters().NumASTScopeLookups;
  ASTScopeImpl::unqualifiedLookup(SF, loc, consumer);
}

llvm::SmallVector<LabeledStmt *, 4> ASTScope::lookupLabeledStmts(
    SourceFile *sourceFile, SourceLoc loc) {
  return ASTScopeImpl::lookupLabeledStmts(sourceFile, loc);
}

std::pair<CaseStmt *, CaseStmt *> ASTScope::lookupFallthroughSourceAndDest(
    SourceFile *sourceFile, SourceLoc loc) {
  return ASTScopeImpl::lookupFallthroughSourceAndDest(sourceFile, loc);
}

void ASTScope::lookupEnclosingMacroScope(
    SourceFile *sourceFile, SourceLoc loc,
    llvm::function_ref<bool(PotentialMacro)> body) {
  return ASTScopeImpl::lookupEnclosingMacroScope(sourceFile, loc, body);
}

CatchNode ASTScope::lookupCatchNode(ModuleDecl *module, SourceLoc loc) {
  return ASTScopeImpl::lookupCatchNode(module, loc);
}

#if NEXTCODE_COMPILER_IS_MSVC
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

void ASTScope::dump() const { impl->dump(); }

#if NEXTCODE_COMPILER_IS_MSVC
#pragma warning(pop)
#endif

void ASTScope::print(llvm::raw_ostream &out) const { impl->print(out); }
void ASTScope::dumpOneScopeMapLocation(std::pair<unsigned, unsigned> lineCol) {
  impl->dumpOneScopeMapLocation(lineCol);
}

#pragma mark ASTScopeImpl


const PatternBindingEntry &AbstractPatternEntryScope::getPatternEntry() const {
  return decl->getPatternList()[patternEntryIndex];
}

Pattern *AbstractPatternEntryScope::getPattern() const {
  return getPatternEntry().getPattern();
}

NullablePtr<AbstractClosureExpr> BraceStmtScope::parentClosureIfAny() const {
  if (auto parent = getParent()) {
    if (auto closureScope = dyn_cast<ClosureParametersScope>(parent.get()))
      return closureScope->closureExpr;
  }

  return nullptr;
}

std::string ASTScopeImpl::getClassName() const {
  // GenericTypeOrExtensionScope provides a custom implementation that deals
  // with declaration names and "portions".
  if (auto generic = dyn_cast<GenericTypeOrExtensionScope>(this))
    return generic->getClassName();

  switch (getKind()) {
#define SCOPE_NODE(Name) case ScopeKind::Name: return #Name "Scope";
#include "nextcode/AST/ASTScopeNodes.def"
  }
}

std::string GenericTypeOrExtensionScope::getClassName() const {
  return declKindName() + portionName() + "Scope";
}

NullablePtr<Decl> ASTScopeImpl::getDeclIfAny() const {
  switch (getKind()) {
    // Declaration scope nodes extract the decl directly.
#define DECL_SCOPE_NODE(Name) \
    case ScopeKind::Name: return cast<Name##Scope>(this)->getDecl();
#define SCOPE_NODE(Name)
#include "nextcode/AST/ASTScopeNodes.def"

    // Everything else returns nullptr.
#define DECL_SCOPE_NODE(Name)
#define SCOPE_NODE(Name) case ScopeKind::Name:
#include "nextcode/AST/ASTScopeNodes.def"
      return nullptr;
  }
}

NullablePtr<Stmt> ASTScopeImpl::getStmtIfAny() const {
  switch (getKind()) {
    // Statement scope nodes extract the statement directly.
#define STMT_SCOPE_NODE(Name) \
    case ScopeKind::Name: return cast<Name##Scope>(this)->getStmt();
#define SCOPE_NODE(Name)
#include "nextcode/AST/ASTScopeNodes.def"

    // Everything else returns nullptr.
#define STMT_SCOPE_NODE(Name)
#define SCOPE_NODE(Name) case ScopeKind::Name:
#include "nextcode/AST/ASTScopeNodes.def"
      return nullptr;
  }
}

NullablePtr<Expr> ASTScopeImpl::getExprIfAny() const {
  switch (getKind()) {
    // Expression scope nodes extract the statement directly.
#define EXPR_SCOPE_NODE(Name) \
    case ScopeKind::Name: return cast<Name##Scope>(this)->getExpr();
#define SCOPE_NODE(Name)
#include "nextcode/AST/ASTScopeNodes.def"

    // Everything else returns nullptr.
#define EXPR_SCOPE_NODE(Name)
#define SCOPE_NODE(Name) case ScopeKind::Name:
#include "nextcode/AST/ASTScopeNodes.def"
      return nullptr;
  }
}

SourceManager &ASTScopeImpl::getSourceManager() const {
  return getASTContext().SourceMgr;
}

Stmt *LabeledConditionalStmtScope::getStmt() const {
  return getLabeledConditionalStmt();
}

#pragma mark getLabeledConditionalStmt
LabeledConditionalStmt *IfStmtScope::getLabeledConditionalStmt() const {
  return stmt;
}
LabeledConditionalStmt *WhileStmtScope::getLabeledConditionalStmt() const {
  return stmt;
}
LabeledConditionalStmt *GuardStmtScope::getLabeledConditionalStmt() const {
  return stmt;
}


#pragma mark getASTContext

ASTContext &ASTScopeImpl::getASTContext() const {
  if (auto d = getDeclIfAny())
    return d.get()->getASTContext();
  if (auto sfScope = dyn_cast<ASTSourceFileScope>(this))
    return sfScope->SF->getASTContext();
  return getParent().get()->getASTContext();
}

#pragma mark getSourceFile

const SourceFile *ASTScopeImpl::getSourceFile() const {
  if (auto sourceFileScope = dyn_cast<ASTSourceFileScope>(this))
    return sourceFileScope->SF;

  return getParent().get()->getSourceFile();
}

SourceRange ExtensionScope::getBraces() const { return decl->getBraces(); }

SourceRange NominalTypeScope::getBraces() const { return decl->getBraces(); }

NullablePtr<NominalTypeDecl>
ExtensionScope::getCorrespondingNominalTypeDecl() const {
  return decl->getExtendedNominal();
}

void ASTScopeImpl::preOrderDo(function_ref<void(ASTScopeImpl *)> fn) {
  fn(this);
  preOrderChildrenDo(fn);
}
void ASTScopeImpl::preOrderChildrenDo(function_ref<void(ASTScopeImpl *)> fn) {
  for (auto *child : getChildren())
    child->preOrderDo(fn);
}

void ASTScopeImpl::postOrderDo(function_ref<void(ASTScopeImpl *)> fn) {
  for (auto *child : getChildren())
    child->postOrderDo(fn);
  fn(this);
}
