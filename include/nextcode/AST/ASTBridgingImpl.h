//===--- ASTBridgingImpl.h - header for the nextcode ASTBridging module ------===//
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

#ifndef NEXTCODE_AST_ASTBRIDGINGIMPL_H
#define NEXTCODE_AST_ASTBRIDGINGIMPL_H

#include "nextcode/AST/Decl.h"

NEXTCODE_BEGIN_NULLABILITY_ANNOTATIONS

//===----------------------------------------------------------------------===//
// MARK: BridgedASTContext
//===----------------------------------------------------------------------===//

void * _Nonnull BridgedASTContext_raw(BridgedASTContext bridged) {
  return &bridged.unbridged();
}

BridgedASTContext BridgedASTContext_fromRaw(void * _Nonnull ptr) {
  return *static_cast<nextcode::ASTContext *>(ptr);
}

//===----------------------------------------------------------------------===//
// MARK: BridgedNominalTypeDecl
//===----------------------------------------------------------------------===//

BridgedStringRef BridgedNominalTypeDecl_getName(BridgedNominalTypeDecl decl) {
  return decl.unbridged()->getName().str();
}

bool BridgedNominalTypeDecl_isGlobalActor(BridgedNominalTypeDecl decl) {
  return decl.unbridged()->isGlobalActor();
}

bool BridgedNominalTypeDecl_hasValueDeinit(BridgedNominalTypeDecl decl) {
  return decl.unbridged()->getValueTypeDestructor() != nullptr;
}

//===----------------------------------------------------------------------===//
// MARK: BridgedSubscriptDecl
//===----------------------------------------------------------------------===//

BridgedAbstractStorageDecl
BridgedSubscriptDecl_asAbstractStorageDecl(BridgedSubscriptDecl decl) {
  return decl.unbridged();
}

//===----------------------------------------------------------------------===//
// MARK: BridgedVarDecl
//===----------------------------------------------------------------------===//

BridgedSourceLoc BridgedVarDecl_getSourceLocation(BridgedVarDecl decl) {
  nextcode::SourceLoc sourceLoc = decl.unbridged()->getNameLoc();
  return BridgedSourceLoc(sourceLoc.getOpaquePointerValue());
}

BridgedStringRef BridgedVarDecl_getUserFacingName(BridgedVarDecl decl) {
  return decl.unbridged()->getBaseName().userFacingName();
}

BridgedAbstractStorageDecl
BridgedVarDecl_asAbstractStorageDecl(BridgedVarDecl decl) {
  return decl.unbridged();
}

NEXTCODE_END_NULLABILITY_ANNOTATIONS

#endif // NEXTCODE_AST_ASTBRIDGINGIMPL_H
