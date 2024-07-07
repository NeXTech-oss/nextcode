//===--- NullablePtr.h - A pointer that allows null -------------*- C++ -*-===//
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
// This file defines and implements the NullablePtr class.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_NULLABLEPTR_H
#define NEXTCODE_BASIC_NULLABLEPTR_H

#include <cassert>
#include <cstddef>
#include <type_traits>
#include "llvm/Support/PointerLikeTypeTraits.h"

namespace nextcode {
/// NullablePtr pointer wrapper - NullablePtr is used for APIs where a
/// potentially-null pointer gets passed around that must be explicitly handled
/// in lots of places.  By putting a wrapper around the null pointer, it makes
/// it more likely that the null pointer case will be handled correctly.
template<class T>
class NullablePtr {
  T *Ptr;
  struct PlaceHolder {};

public:
  NullablePtr(T *P = 0) : Ptr(P) {}

  template<typename OtherT>
  NullablePtr(NullablePtr<OtherT> Other,
              typename std::enable_if<
                std::is_convertible<OtherT *, T *>::value,
                PlaceHolder
              >::type = PlaceHolder()) : Ptr(Other.getPtrOrNull()) {}
  
  bool isNull() const { return Ptr == 0; }
  bool isNonNull() const { return Ptr != 0; }

  /// get - Return the pointer if it is non-null.
  const T *get() const {
    assert(Ptr && "Pointer wasn't checked for null!");
    return Ptr;
  }

  /// get - Return the pointer if it is non-null.
  T *get() {
    assert(Ptr && "Pointer wasn't checked for null!");
    return Ptr;
  }

  T *getPtrOrNull() { return getPtrOr(nullptr); }
  const T *getPtrOrNull() const { return getPtrOr(nullptr); }

  T *getPtrOr(T *defaultValue) { return Ptr ? Ptr : defaultValue; }
  const T *getPtrOr(const T *defaultValue) const {
    return Ptr ? Ptr : defaultValue;
  }

  explicit operator bool() const { return Ptr; }

  bool operator==(const NullablePtr<T> &other) const {
    return other.Ptr == Ptr;
  }

  bool operator!=(const NullablePtr<T> &other) const {
    return !(*this == other);
  }

  bool operator==(const T *other) const { return other == Ptr; }

  bool operator!=(const T *other) const { return !(*this == other); }
};
  
} // end namespace nextcode

namespace llvm {
template <typename T> struct PointerLikeTypeTraits;
template <typename T> struct PointerLikeTypeTraits<nextcode::NullablePtr<T>> {
public:
  static inline void *getAsVoidPointer(nextcode::NullablePtr<T> ptr) {
    return static_cast<void *>(ptr.getPtrOrNull());
  }
  static inline nextcode::NullablePtr<T> getFromVoidPointer(void *ptr) {
    return nextcode::NullablePtr<T>(static_cast<T*>(ptr));
  }
  enum { NumLowBitsAvailable = PointerLikeTypeTraits<T *>::NumLowBitsAvailable };
};

}

#endif // NEXTCODE_BASIC_NULLABLEPTR_H
