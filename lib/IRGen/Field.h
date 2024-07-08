//===--- Field.h - Abstract stored field ------------------------*- C++ -*-===//
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
//  This file provides an abstraction for some sort of stored field
//  in a type.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_FIELD_H
#define NEXTCODE_IRGEN_FIELD_H

#include "nextcode/AST/Decl.h"

namespace nextcode {
class SILType;

namespace irgen {
class IRGenModule;

/// Return the number of fields that will be visited by forEachField,
/// which determines a number of things in the ABI, including the length
/// of the field vector in the type metadata.
///
/// Generally this is the length of the stored properties, but 
/// root default actors have an implicit field for their default-actor
/// storage, and there may also be missing members.
///
/// Even if you're sure that you're in a case where only the stored
/// properties are needed, calling this (and forEachField) instead of
/// directly accessing the stored properties is good practice.
unsigned getNumFields(const NominalTypeDecl *type);

struct Field {
public:
  enum Kind: uintptr_t {
    Var,
    MissingMember,
    DefaultActorStorage,
    NonDefaultDistributedActorStorage,
    FirstArtificial = DefaultActorStorage
  };
  enum : uintptr_t { KindMask = 0x3 };
private:
  uintptr_t declOrKind;
public:
  Field(VarDecl *decl)
      : declOrKind(reinterpret_cast<uintptr_t>(decl) | Var) {
    assert(decl);
    assert(getKind() == Var);
  }
  Field(MissingMemberDecl *decl)
      : declOrKind(reinterpret_cast<uintptr_t>(decl) | MissingMember) {
    assert(decl);
    assert(getKind() == MissingMember);
  }
  Field(Kind kind) : declOrKind(kind) {
    assert(kind >= FirstArtificial);
  }

  Kind getKind() const {
    return Kind(declOrKind & KindMask);
  }
  VarDecl *getVarDecl() const {
    assert(getKind() == Var);
    return reinterpret_cast<VarDecl*>(declOrKind);
  }
  MissingMemberDecl *getMissingMemberDecl() const {
    assert(getKind() == MissingMember);
    return reinterpret_cast<MissingMemberDecl*>(declOrKind & ~KindMask);
  }

  /// Is this a concrete field?  When we're emitting a type, all the
  /// fields should be concrete; non-concrete fields occur only with
  /// imported declarations.
  bool isConcrete() const { return getKind() != MissingMember; }

  /// Return the type of this concrete field.
  SILType getType(IRGenModule &IGM, SILType baseType) const;

  /// Return the interface type of this concrete field.
  Type getInterfaceType(IRGenModule &IGM) const;

  /// Return the nam eof this concrete field.
  llvm::StringRef getName() const;

  bool operator==(Field other) const { return declOrKind == other.declOrKind; }
  bool operator!=(Field other) const { return declOrKind != other.declOrKind; }
};

/// Iterate all the fields of the given struct or class type, including
/// any implicit fields that might be accounted for in
/// getFieldVectorLength.
void forEachField(IRGenModule &IGM, const NominalTypeDecl *typeDecl,
                  llvm::function_ref<void(Field field)> fn);

} // end namespace irgen
} // end namespace nextcode

#endif