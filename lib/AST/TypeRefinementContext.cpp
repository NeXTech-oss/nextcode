//===--- TypeRefinementContext.cpp - NeXTCode Refinement Context -------------===//
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
//
// This file implements the TypeRefinementContext class.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/Stmt.h"
#include "nextcode/AST/Expr.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/AST/TypeCheckRequests.h"
#include "nextcode/AST/TypeRefinementContext.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/SourceManager.h"

using namespace nextcode;

TypeRefinementContext::TypeRefinementContext(ASTContext &Ctx, IntroNode Node,
                                             TypeRefinementContext *Parent,
                                             SourceRange SrcRange,
                                             const AvailabilityContext &Info,
                                             const AvailabilityContext &ExplicitInfo)
    : Node(Node), SrcRange(SrcRange),
      AvailabilityInfo(Info), ExplicitAvailabilityInfo(ExplicitInfo) {
  if (Parent) {
    assert(SrcRange.isValid());
    Parent->addChild(this);
    assert(Info.isContainedIn(Parent->getAvailabilityInfo()));
  }
  Ctx.addDestructorCleanup(Children);
}

TypeRefinementContext *
TypeRefinementContext::createForSourceFile(SourceFile *SF,
                                           const AvailabilityContext &Info) {
  assert(SF);

  ASTContext &Ctx = SF->getASTContext();

  SourceRange range;
  TypeRefinementContext *parentContext = nullptr;
  AvailabilityContext availabilityContext = Info;
  switch (SF->Kind) {
  case SourceFileKind::MacroExpansion:
  case SourceFileKind::DefaultArgument: {
    // Look up the parent context in the enclosing file that this file's
    // root context should be nested under.
    if (auto parentTRC =
            SF->getEnclosingSourceFile()->getTypeRefinementContext()) {
      auto charRange = Ctx.SourceMgr.getRangeForBuffer(*SF->getBufferID());
      range = SourceRange(charRange.getStart(), charRange.getEnd());
      auto originalNode = SF->getNodeInEnclosingSourceFile();
      parentContext =
          parentTRC->findMostRefinedSubContext(originalNode.getStartLoc(), Ctx);
      if (parentContext)
        availabilityContext = parentContext->getAvailabilityInfo();
    }
    break;
  }
  case SourceFileKind::Library:
  case SourceFileKind::Main:
  case SourceFileKind::Interface:
    break;
  case SourceFileKind::SIL:
    llvm_unreachable("unexpected SourceFileKind");
  }

  return new (Ctx)
      TypeRefinementContext(Ctx, SF, parentContext, range,
                            availabilityContext,
                            AvailabilityContext::alwaysAvailable());
}

TypeRefinementContext *
TypeRefinementContext::createForDecl(ASTContext &Ctx, Decl *D,
                                     TypeRefinementContext *Parent,
                                     const AvailabilityContext &Info,
                                     const AvailabilityContext &ExplicitInfo,
                                     SourceRange SrcRange) {
  assert(D);
  assert(Parent);
  return new (Ctx)
      TypeRefinementContext(Ctx, D, Parent, SrcRange, Info, ExplicitInfo);
}

TypeRefinementContext *TypeRefinementContext::createForDeclImplicit(
    ASTContext &Ctx, Decl *D, TypeRefinementContext *Parent,
    const AvailabilityContext &Info, SourceRange SrcRange) {
  assert(D);
  assert(Parent);
  return new (Ctx) TypeRefinementContext(
      Ctx, IntroNode(D, Reason::DeclImplicit), Parent, SrcRange, Info,
      AvailabilityContext::alwaysAvailable());
}

TypeRefinementContext *
TypeRefinementContext::createForIfStmtThen(ASTContext &Ctx, IfStmt *S,
                                           TypeRefinementContext *Parent,
                                           const AvailabilityContext &Info) {
  assert(S);
  assert(Parent);
  return new (Ctx)
      TypeRefinementContext(Ctx, IntroNode(S, /*IsThen=*/true), Parent,
                            S->getThenStmt()->getSourceRange(),
                            Info, /* ExplicitInfo */Info);
}

TypeRefinementContext *
TypeRefinementContext::createForIfStmtElse(ASTContext &Ctx, IfStmt *S,
                                           TypeRefinementContext *Parent,
                                           const AvailabilityContext &Info) {
  assert(S);
  assert(Parent);
  return new (Ctx)
      TypeRefinementContext(Ctx, IntroNode(S, /*IsThen=*/false), Parent,
                            S->getElseStmt()->getSourceRange(),
                            Info, /* ExplicitInfo */Info);
}

TypeRefinementContext *
TypeRefinementContext::createForConditionFollowingQuery(ASTContext &Ctx,
                                 PoundAvailableInfo *PAI,
                                 const StmtConditionElement &LastElement,
                                 TypeRefinementContext *Parent,
                                 const AvailabilityContext &Info) {
  assert(PAI);
  assert(Parent);
  SourceRange Range(PAI->getEndLoc(), LastElement.getEndLoc());
  return new (Ctx) TypeRefinementContext(Ctx, PAI, Parent, Range,
                                         Info, /* ExplicitInfo */Info);
}

TypeRefinementContext *
TypeRefinementContext::createForGuardStmtFallthrough(ASTContext &Ctx,
                                  GuardStmt *RS,
                                  BraceStmt *ContainingBraceStmt,
                                  TypeRefinementContext *Parent,
                                  const AvailabilityContext &Info) {
  assert(RS);
  assert(ContainingBraceStmt);
  assert(Parent);
  SourceRange Range(RS->getEndLoc(), ContainingBraceStmt->getEndLoc());
  return new (Ctx) TypeRefinementContext(Ctx,
                                         IntroNode(RS, /*IsFallthrough=*/true),
                                         Parent, Range, Info, /* ExplicitInfo */Info);
}

TypeRefinementContext *
TypeRefinementContext::createForGuardStmtElse(ASTContext &Ctx, GuardStmt *RS,
                                              TypeRefinementContext *Parent,
                                              const AvailabilityContext &Info) {
  assert(RS);
  assert(Parent);
  return new (Ctx)
      TypeRefinementContext(Ctx, IntroNode(RS, /*IsFallthrough=*/false), Parent,
                            RS->getBody()->getSourceRange(), Info, /* ExplicitInfo */Info);
}

TypeRefinementContext *
TypeRefinementContext::createForWhileStmtBody(ASTContext &Ctx, WhileStmt *S,
                                              TypeRefinementContext *Parent,
                                              const AvailabilityContext &Info) {
  assert(S);
  assert(Parent);
  return new (Ctx) TypeRefinementContext(
      Ctx, S, Parent, S->getBody()->getSourceRange(), Info, /* ExplicitInfo */Info);
}

/// Determine whether the child location is somewhere within the parent
/// range.
static bool rangeContainsTokenLocWithGeneratedSource(
    SourceManager &sourceMgr, SourceRange parentRange, SourceLoc childLoc) {
  auto parentBuffer = sourceMgr.findBufferContainingLoc(parentRange.Start);
  auto childBuffer = sourceMgr.findBufferContainingLoc(childLoc);
  while (parentBuffer != childBuffer) {
    auto info = sourceMgr.getGeneratedSourceInfo(childBuffer);
    if (!info)
      return false;

    childLoc = info->originalSourceRange.getStart();
    if (childLoc.isInvalid())
      return false;

    childBuffer = sourceMgr.findBufferContainingLoc(childLoc);
  }

  return sourceMgr.rangeContainsTokenLoc(parentRange, childLoc);
}

TypeRefinementContext *
TypeRefinementContext::findMostRefinedSubContext(SourceLoc Loc,
                                                 ASTContext &Ctx) {
  assert(Loc.isValid());

  if (SrcRange.isValid() &&
      !rangeContainsTokenLocWithGeneratedSource(Ctx.SourceMgr, SrcRange, Loc))
    return nullptr;

  auto expandedChildren = evaluateOrDefault(
      Ctx.evaluator, ExpandChildTypeRefinementContextsRequest{this}, {});

  // For the moment, we perform a linear search here, but we can and should
  // do something more efficient.
  for (TypeRefinementContext *Child : expandedChildren) {
    if (auto *Found = Child->findMostRefinedSubContext(Loc, Ctx)) {
      return Found;
    }
  }

  // Loc is in this context's range but not in any child's, so this context
  // must be the inner-most context.
  return this;
}

void TypeRefinementContext::dump(SourceManager &SrcMgr) const {
  dump(llvm::errs(), SrcMgr);
}

void TypeRefinementContext::dump(raw_ostream &OS, SourceManager &SrcMgr) const {
  print(OS, SrcMgr, 0);
  OS << '\n';
}

SourceLoc TypeRefinementContext::getIntroductionLoc() const {
  switch (getReason()) {
  case Reason::Decl:
  case Reason::DeclImplicit:
    return Node.getAsDecl()->getLoc();

  case Reason::IfStmtThenBranch:
  case Reason::IfStmtElseBranch:
    return Node.getAsIfStmt()->getIfLoc();

  case Reason::ConditionFollowingAvailabilityQuery:
    return Node.getAsPoundAvailableInfo()->getStartLoc();

  case Reason::GuardStmtFallthrough:
  case Reason::GuardStmtElseBranch:
    return Node.getAsGuardStmt()->getGuardLoc();


  case Reason::WhileStmtBody:
    return Node.getAsWhileStmt()->getStartLoc();

  case Reason::Root:
    return SourceLoc();
  }

  llvm_unreachable("Unhandled Reason in switch.");
}

static SourceRange
getAvailabilityConditionVersionSourceRange(const PoundAvailableInfo *PAI,
                                           PlatformKind Platform,
                                           const llvm::VersionTuple &Version) {
  SourceRange Range;
  for (auto *S : PAI->getQueries()) {
    if (auto *V = dyn_cast<PlatformVersionConstraintAvailabilitySpec>(S)) {
      if (V->getPlatform() == Platform && V->getVersion() == Version) {
        // More than one: return invalid range, no unique choice.
        if (Range.isValid())
          return SourceRange();
        else
          Range = V->getVersionSrcRange();
      }
    }
  }
  return Range;
}

static SourceRange
getAvailabilityConditionVersionSourceRange(
    const MutableArrayRef<StmtConditionElement> &Conds,
    PlatformKind Platform,
    const llvm::VersionTuple &Version) {
  SourceRange Range;
  for (auto const& C : Conds) {
    if (C.getKind() == StmtConditionElement::CK_Availability) {
      SourceRange R = getAvailabilityConditionVersionSourceRange(
        C.getAvailability(), Platform, Version);
      // More than one: return invalid range.
      if (Range.isValid())
        return SourceRange();
      else
        Range = R;
    }
  }
  return Range;
}

static SourceRange
getAvailabilityConditionVersionSourceRange(const DeclAttributes &DeclAttrs,
                                           PlatformKind Platform,
                                           const llvm::VersionTuple &Version) {
  SourceRange Range;
  for (auto *Attr : DeclAttrs) {
    if (auto *AA = dyn_cast<AvailableAttr>(Attr)) {
      if (AA->Introduced.has_value() &&
          AA->Introduced.value() == Version &&
          AA->Platform == Platform) {

        // More than one: return invalid range.
        if (Range.isValid())
          return SourceRange();
        else
          Range = AA->IntroducedRange;
      }
    }
  }
  return Range;
}

SourceRange
TypeRefinementContext::getAvailabilityConditionVersionSourceRange(
    PlatformKind Platform,
    const llvm::VersionTuple &Version) const {
  switch (getReason()) {
  case Reason::Decl:
    return ::getAvailabilityConditionVersionSourceRange(
      Node.getAsDecl()->getAttrs(), Platform, Version);

  case Reason::IfStmtThenBranch:
  case Reason::IfStmtElseBranch:
    return ::getAvailabilityConditionVersionSourceRange(
      Node.getAsIfStmt()->getCond(), Platform, Version);

  case Reason::ConditionFollowingAvailabilityQuery:
    return ::getAvailabilityConditionVersionSourceRange(
      Node.getAsPoundAvailableInfo(), Platform, Version);

  case Reason::GuardStmtFallthrough:
  case Reason::GuardStmtElseBranch:
    return ::getAvailabilityConditionVersionSourceRange(
      Node.getAsGuardStmt()->getCond(), Platform, Version);

  case Reason::WhileStmtBody:
    return ::getAvailabilityConditionVersionSourceRange(
      Node.getAsWhileStmt()->getCond(), Platform, Version);

  case Reason::Root:
  case Reason::DeclImplicit:
    return SourceRange();
  }

  llvm_unreachable("Unhandled Reason in switch.");
}

void TypeRefinementContext::print(raw_ostream &OS, SourceManager &SrcMgr,
                                  unsigned Indent) const {
  OS.indent(Indent);
  OS << "(" << getReasonName(getReason());

  OS << " versions=" << AvailabilityInfo.getOSVersion().getAsString();

  if (getReason() == Reason::Decl || getReason() == Reason::DeclImplicit) {
    Decl *D = Node.getAsDecl();
    OS << " decl=";
    if (auto VD = dyn_cast<ValueDecl>(D)) {
      OS << VD->getName();
    } else if (auto *ED = dyn_cast<ExtensionDecl>(D)) {
      OS << "extension." << ED->getExtendedType().getString();
    } else if (isa<TopLevelCodeDecl>(D)) {
      OS << "<top-level-code>";
    } else if (auto PBD = dyn_cast<PatternBindingDecl>(D)) {
      if (auto VD = PBD->getAnchoringVarDecl(0)) {
        OS << VD->getName();
      }
    }
  }

  auto R = getSourceRange();
  if (R.isValid()) {
    OS << " src_range=";
    R.print(OS, SrcMgr, /*PrintText=*/false);
  }

  if (!ExplicitAvailabilityInfo.isAlwaysAvailable())
    OS << " explicit_versions="
       << ExplicitAvailabilityInfo.getOSVersion().getAsString();

  for (TypeRefinementContext *Child : Children) {
    OS << '\n';
    Child->print(OS, SrcMgr, Indent + 2);
  }
  OS.indent(Indent);
  OS << ")";
}

TypeRefinementContext::Reason TypeRefinementContext::getReason() const {
  return Node.getReason();
}

StringRef TypeRefinementContext::getReasonName(Reason R) {
  switch (R) {
  case Reason::Root:
    return "root";

  case Reason::Decl:
    return "decl";

  case Reason::DeclImplicit:
    return "decl_implicit";

  case Reason::IfStmtThenBranch:
    return "if_then";

  case Reason::IfStmtElseBranch:
    return "if_else";

  case Reason::ConditionFollowingAvailabilityQuery:
    return "condition_following_availability";

  case Reason::GuardStmtFallthrough:
    return "guard_fallthrough";

  case Reason::GuardStmtElseBranch:
    return "guard_else";

  case Reason::WhileStmtBody:
    return "while_body";
  }

  llvm_unreachable("Unhandled Reason in switch.");
}

void nextcode::simple_display(
    llvm::raw_ostream &out, const TypeRefinementContext *trc) {
  out << "TRC @" << trc;
}

std::optional<std::vector<TypeRefinementContext *>>
ExpandChildTypeRefinementContextsRequest::getCachedResult() const {
  auto *TRC = std::get<0>(getStorage());
  if (TRC->getNeedsExpansion())
    return std::nullopt;
  return TRC->Children;
}

void ExpandChildTypeRefinementContextsRequest::cacheResult(
    std::vector<TypeRefinementContext *> children) const {
  auto *TRC = std::get<0>(getStorage());
  TRC->Children = children;
  TRC->setNeedsExpansion(false);
}
