//===--- FreestandingMacroExpansion.cpp -----------------------------------===//
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

#include "nextcode/AST/FreestandingMacroExpansion.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/Expr.h"
#include "nextcode/AST/MacroDiscriminatorContext.h"
#include "nextcode/Basic/Assertions.h"

using namespace nextcode;

SourceRange MacroExpansionInfo::getSourceRange() const {
  SourceLoc endLoc;
  if (ArgList && !ArgList->isImplicit())
    endLoc = ArgList->getEndLoc();
  else if (RightAngleLoc.isValid())
    endLoc = RightAngleLoc;
  else
    endLoc = MacroNameLoc.getEndLoc();

  return SourceRange(SigilLoc, endLoc);
}

#define FORWARD_VARIANT(NAME)                                                  \
  switch (getFreestandingMacroKind()) {                                        \
  case FreestandingMacroKind::Expr:                                            \
    return cast<MacroExpansionExpr>(this)->NAME();                             \
  case FreestandingMacroKind::Decl:                                            \
    return cast<MacroExpansionDecl>(this)->NAME();                             \
  }

DeclContext *FreestandingMacroExpansion::getDeclContext() const {
  FORWARD_VARIANT(getDeclContext);
}
SourceRange FreestandingMacroExpansion::getSourceRange() const {
  FORWARD_VARIANT(getSourceRange);
}
unsigned FreestandingMacroExpansion::getDiscriminator() const {
  auto info = getExpansionInfo();
  if (info->Discriminator != MacroExpansionInfo::InvalidDiscriminator)
    return info->Discriminator;

  auto mutableThis = const_cast<FreestandingMacroExpansion *>(this);
  auto dc = getDeclContext();
  ASTContext &ctx = dc->getASTContext();
  auto discriminatorContext =
      MacroDiscriminatorContext::getParentOf(mutableThis);
  info->Discriminator = ctx.getNextMacroDiscriminator(
      discriminatorContext, getMacroName().getBaseName());

  assert(info->Discriminator != MacroExpansionInfo::InvalidDiscriminator);
  return info->Discriminator;
}

unsigned FreestandingMacroExpansion::getRawDiscriminator() const {
  return getExpansionInfo()->Discriminator;
}

ASTNode FreestandingMacroExpansion::getASTNode() {
  switch (getFreestandingMacroKind()) {
  case FreestandingMacroKind::Expr:
    return cast<MacroExpansionExpr>(this);
  case FreestandingMacroKind::Decl:
    return cast<MacroExpansionDecl>(this);
  }
}
