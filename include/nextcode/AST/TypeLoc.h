//===--- TypeLoc.h - NeXTCode Language Type Locations --------------*- C++ -*-===//
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
// This file defines the TypeLoc struct and related structs.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_TYPELOC_H
#define NEXTCODE_TYPELOC_H

#include "nextcode/Basic/SourceLoc.h"
#include "nextcode/AST/Type.h"
#include "nextcode/AST/TypeAlignments.h"
#include "llvm/ADT/PointerIntPair.h"

namespace nextcode {

class ASTContext;
class TypeRepr;

/// TypeLoc - Provides source location information for a parsed type.
/// A TypeLoc is stored in AST nodes which use an explicitly written type.
class alignas(1 << TypeReprAlignInBits) TypeLoc {
  Type Ty;
  TypeRepr *TyR = nullptr;

public:
  TypeLoc() {}
  TypeLoc(TypeRepr *TyR) : TyR(TyR) {}
  TypeLoc(TypeRepr *TyR, Type Ty) : TyR(TyR) {
    setType(Ty);
  }

  bool wasValidated() const { return !Ty.isNull(); }
  bool isError() const;

  // FIXME: We generally shouldn't need to build TypeLocs without a location.
  static TypeLoc withoutLoc(Type T) {
    TypeLoc result;
    result.Ty = T;
    return result;
  }

  /// Get the representative location of this type, for diagnostic
  /// purposes.
  /// This location is not necessarily the start location of the type repr.
  SourceLoc getLoc() const;
  SourceRange getSourceRange() const;

  bool hasLocation() const { return TyR != nullptr; }
  TypeRepr *getTypeRepr() const { return TyR; }
  Type getType() const { return Ty; }

  bool isNull() const { return getType().isNull() && TyR == nullptr; }

  void setType(Type Ty);

  friend llvm::hash_code hash_value(const TypeLoc &owner) {
    return llvm::hash_combine(owner.Ty.getPointer(), owner.TyR);
  }

  friend bool operator==(const TypeLoc &lhs,
                         const TypeLoc &rhs) {
    return lhs.Ty.getPointer() == rhs.Ty.getPointer()
        && lhs.TyR == rhs.TyR;
  }

  friend bool operator!=(const TypeLoc &lhs,
                         const TypeLoc &rhs) {
    return !(lhs == rhs);
  }
};

} // end namespace llvm

#endif
