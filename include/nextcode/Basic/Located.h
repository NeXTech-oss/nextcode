//===--- Located.h - Source Location and Associated Value ----------*- C++ -*-===//
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
// Provides a currency data type Located<T> that should be used instead
// of std::pair<T, SourceLoc>.
//
//===----------------------------------------------------------------------===//


#ifndef NEXTCODE_BASIC_LOCATED_H
#define NEXTCODE_BASIC_LOCATED_H
#include "nextcode/Basic/Debug.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/Basic/SourceLoc.h"

namespace nextcode {

/// A currency type for keeping track of items which were found in the source code.
/// Several parts of the compiler need to keep track of a `SourceLoc` corresponding
/// to an item, in case they need to report some diagnostics later. For example,
/// the ClangImporter needs to keep track of where imports were originally written.
/// Located makes it easy to do so while making the code more readable, compared to
/// using `std::pair`.
template <typename T>
struct Located {
  /// The main item whose source location is being tracked.
  T Item;

  /// The original source location from which the item was parsed.
  SourceLoc Loc;

  Located() : Item(), Loc() {}

  Located(T Item, SourceLoc loc) : Item(Item), Loc(loc) {}

  NEXTCODE_DEBUG_DUMP;
  void dump(raw_ostream &os) const;
};

template <typename T>
bool operator ==(const Located<T> &lhs, const Located<T> &rhs) {
  return lhs.Item == rhs.Item && lhs.Loc == rhs.Loc;
}

} // end namespace nextcode

namespace llvm {

template <typename T, typename Enable> struct DenseMapInfo;

template<typename T>
struct DenseMapInfo<nextcode::Located<T>> {

  static inline nextcode::Located<T> getEmptyKey() {
    return nextcode::Located<T>(DenseMapInfo<T>::getEmptyKey(),
                             DenseMapInfo<nextcode::SourceLoc>::getEmptyKey());
  }

  static inline nextcode::Located<T> getTombstoneKey() {
    return nextcode::Located<T>(DenseMapInfo<T>::getTombstoneKey(),
                             DenseMapInfo<nextcode::SourceLoc>::getTombstoneKey());
  }

  static unsigned getHashValue(const nextcode::Located<T> &LocatedVal) {
    return combineHashValue(DenseMapInfo<T>::getHashValue(LocatedVal.Item),
                            DenseMapInfo<nextcode::SourceLoc>::getHashValue(LocatedVal.Loc));
  }

  static bool isEqual(const nextcode::Located<T> &LHS, const nextcode::Located<T> &RHS) {
    return DenseMapInfo<T>::isEqual(LHS.Item, RHS.Item) &&
           DenseMapInfo<T>::isEqual(LHS.Loc, RHS.Loc);
  }
};
} // namespace llvm

#endif // NEXTCODE_BASIC_LOCATED_H
