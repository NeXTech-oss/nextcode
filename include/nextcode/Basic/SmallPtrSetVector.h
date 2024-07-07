//===--- SmallPtrSetVector.h ----------------------------------------------===//
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

#ifndef NEXTCODE_BASIC_SMALLPTRSETVECTOR_H
#define NEXTCODE_BASIC_SMALLPTRSETVECTOR_H

#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"

namespace nextcode {

/// A SetVector that performs no allocations if smaller than a certain
/// size. Uses a SmallPtrSet/SmallVector internally.
template <typename T, unsigned VectorSize, unsigned SetSize = VectorSize>
class SmallPtrSetVector : public llvm::SetVector<T, SmallVector<T, VectorSize>,
                                                 SmallPtrSet<T, SetSize>> {
public:
  SmallPtrSetVector() = default;

  /// Initialize a SmallPtrSetVector with a range of elements
  template <typename It> SmallPtrSetVector(It Start, It End) {
    this->insert(Start, End);
  }
};

} // namespace nextcode

namespace std {

/// Implement std::swap in terms of SmallSetVector swap.
///
/// This matches llvm's implementation for SmallSetVector.
template <typename T, unsigned VectorSize, unsigned SetSize = VectorSize>
inline void swap(nextcode::SmallPtrSetVector<T, VectorSize, SetSize> &LHS,
                 nextcode::SmallPtrSetVector<T, VectorSize, SetSize> &RHS) {
  LHS.swap(RHS);
}

} // end namespace std

#endif
