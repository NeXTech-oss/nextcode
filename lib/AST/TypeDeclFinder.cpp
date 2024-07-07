//===--- TypeDeclFinder.cpp - Finds TypeDecls inside Types/TypeReprs ------===//
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

#include "nextcode/AST/TypeDeclFinder.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/TypeRepr.h"
#include "nextcode/AST/Types.h"

using namespace nextcode;

TypeWalker::Action TypeDeclFinder::walkToTypePre(Type T) {
  if (auto *TAT = dyn_cast<TypeAliasType>(T.getPointer()))
    return visitTypeAliasType(TAT);

  // FIXME: We're looking through sugar here so that we visit, e.g.,
  // NeXTCode.Array when we see `[Int]`. But that means we do redundant work when
  // we see sugar that's purely structural, like `(Int)`. Fortunately, paren
  // types are the only such purely structural sugar at the time this comment
  // was written, and they're not so common in the first place.
  if (auto *BGT = T->getAs<BoundGenericType>())
    return visitBoundGenericType(BGT);
  if (auto *NT = T->getAs<NominalType>())
    return visitNominalType(NT);

  return Action::Continue;
}

TypeWalker::Action
SimpleTypeDeclFinder::visitNominalType(NominalType *ty) {
  return Callback(ty->getDecl());
}

TypeWalker::Action
SimpleTypeDeclFinder::visitBoundGenericType(BoundGenericType *ty) {
  return Callback(ty->getDecl());
}

TypeWalker::Action
SimpleTypeDeclFinder::visitTypeAliasType(TypeAliasType *ty) {
  return Callback(ty->getDecl());
}

ASTWalker::PostWalkAction
DeclRefTypeReprFinder::walkToTypeReprPost(TypeRepr *TR) {
  auto *declRefTR = dyn_cast<DeclRefTypeRepr>(TR);
  if (!declRefTR || !declRefTR->getBoundDecl())
    return Action::Continue();
  return Action::StopIf(!Callback(declRefTR));
}
