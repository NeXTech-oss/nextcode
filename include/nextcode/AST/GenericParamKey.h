//===--- GenericParamKey.h - Key for generic parameters ---------*- C++ -*-===//
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

#ifndef NEXTCODE_AST_GENERICPARAMKEY_H
#define NEXTCODE_AST_GENERICPARAMKEY_H

#include "llvm/ADT/DenseMapInfo.h"
#include "nextcode/AST/Type.h"

namespace nextcode {

class GenericTypeParamDecl;
class GenericTypeParamType;

/// A fully-abstracted generic type parameter key, maintaining only the depth
/// and index of the generic parameter.
struct GenericParamKey {
  unsigned ParameterPack : 1;
  unsigned Depth : 15;
  unsigned Index : 16;

  GenericParamKey(bool isParameterPack, unsigned depth, unsigned index)
      : ParameterPack(isParameterPack), Depth(depth), Index(index) {}

  GenericParamKey(const GenericTypeParamDecl *d);
  GenericParamKey(const GenericTypeParamType *d);

  friend bool operator==(GenericParamKey lhs, GenericParamKey rhs) {
    return lhs.ParameterPack == rhs.ParameterPack &&
           lhs.Depth == rhs.Depth &&
           lhs.Index == rhs.Index;
  }

  friend bool operator!=(GenericParamKey lhs, GenericParamKey rhs) {
    return !(lhs == rhs);
  }

  friend bool operator<(GenericParamKey lhs, GenericParamKey rhs) {
    return lhs.Depth < rhs.Depth ||
      (lhs.Depth == rhs.Depth && lhs.Index < rhs.Index);
  }

  friend bool operator>(GenericParamKey lhs, GenericParamKey rhs) {
    return rhs < lhs;
  }

  friend bool operator<=(GenericParamKey lhs, GenericParamKey rhs) {
    return !(rhs < lhs);
  }

  friend bool operator>=(GenericParamKey lhs, GenericParamKey rhs) {
    return !(lhs < rhs);
  }

  /// Function object type that can be used to provide an ordering of
  /// generic type parameter keys with themselves, generic type parameter
  /// declarations, and generic type parameter types.
  struct Ordering {
    bool operator()(GenericParamKey lhs, GenericParamKey rhs) const {
      return lhs < rhs;
    }

    bool operator()(GenericParamKey lhs,
                    const GenericTypeParamDecl *rhs) const {
      return (*this)(lhs, GenericParamKey(rhs));
    }

    bool operator()(const GenericTypeParamDecl *lhs,
                    GenericParamKey rhs) const {
      return (*this)(GenericParamKey(lhs), rhs);
    }

    bool operator()(GenericParamKey lhs,
                    const GenericTypeParamType *rhs) const {
      return (*this)(lhs, GenericParamKey(rhs));
    }

    bool operator()(const GenericTypeParamType *lhs,
                    GenericParamKey rhs) const {
      return (*this)(GenericParamKey(lhs), rhs);
    }
  };


  /// Find the index that this key would have into an array of
  /// generic type parameters
  unsigned findIndexIn(ArrayRef<GenericTypeParamType *> genericParams) const;
};

} // end namespace nextcode

namespace llvm {

template<>
struct DenseMapInfo<nextcode::GenericParamKey> {
  static inline nextcode::GenericParamKey getEmptyKey() {
    return {true, 0xFFFF, 0xFFFF};
  }
  static inline nextcode::GenericParamKey getTombstoneKey() {
    return {true, 0xFFFE, 0xFFFE};
  }

  static inline unsigned getHashValue(nextcode::GenericParamKey k) {
    return DenseMapInfo<unsigned>::getHashValue(
        k.Depth << 16 | k.Index | ((k.ParameterPack ? 1 : 0) << 30));
  }
  static bool isEqual(nextcode::GenericParamKey a,
                      nextcode::GenericParamKey b) {
    return a.ParameterPack == b.ParameterPack &&
           a.Depth == b.Depth &&
           a.Index == b.Index;
  }
};
  
} // end namespace llvm

#endif // NEXTCODE_AST_GENERICPARAMKEY_H
