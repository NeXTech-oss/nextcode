//===--- ConcreteDeclRef.cpp - Reference to a concrete decl ---------------===//
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
// This file implements the ConcreteDeclRef class, which provides a reference to
// a declaration that is potentially specialized.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/ConcreteDeclRef.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/GenericSignature.h"
#include "nextcode/AST/ProtocolConformance.h"
#include "nextcode/AST/SubstitutionMap.h"
#include "nextcode/AST/Types.h"
#include "nextcode/Basic/Assertions.h"
#include "llvm/Support/raw_ostream.h"
using namespace nextcode;

ConcreteDeclRef ConcreteDeclRef::getOverriddenDecl() const {
  auto *derivedDecl = getDecl();
  auto *baseDecl = derivedDecl->getOverriddenDecl();

  auto baseSig = baseDecl->getInnermostDeclContext()
      ->getGenericSignatureOfContext();
  auto derivedSig = derivedDecl->getInnermostDeclContext()
      ->getGenericSignatureOfContext();

  SubstitutionMap subs;
  if (baseSig) {
    subs = SubstitutionMap::getOverrideSubstitutions(baseDecl, derivedDecl);
    if (derivedSig)
      subs = subs.subst(getSubstitutions());
  }
  return ConcreteDeclRef(baseDecl, subs);
}

ConcreteDeclRef ConcreteDeclRef::getOverriddenDecl(ValueDecl *baseDecl) const {
  auto *derivedDecl = getDecl();
  if (baseDecl == derivedDecl) return *this;

#ifndef NDEBUG
  {
    auto cur = derivedDecl;
    for (; cur && cur != baseDecl; cur = cur->getOverriddenDecl()) {}
    assert(cur && "decl is not an indirect override of baseDecl");
  }
#endif

  if (!baseDecl->getInnermostDeclContext()->isGenericContext()) {
    return ConcreteDeclRef(baseDecl);
  }

  auto subs = SubstitutionMap::getOverrideSubstitutions(baseDecl, derivedDecl);
  if (auto derivedSubs = getSubstitutions()) {
    subs = subs.subst(derivedSubs);
  }
  return ConcreteDeclRef(baseDecl, subs);
}

void ConcreteDeclRef::dump(raw_ostream &os) const {
  if (!getDecl()) {
    os << "**NULL**";
    return;
  }

  getDecl()->dumpRef(os);

  // If specialized, dump the substitutions.
  if (isSpecialized()) {
    os << " [with ";
    getSubstitutions().dump(os, SubstitutionMap::DumpStyle::Minimal);
    os << ']';
  }
}

void ConcreteDeclRef::dump() const {
  dump(llvm::errs());
}
