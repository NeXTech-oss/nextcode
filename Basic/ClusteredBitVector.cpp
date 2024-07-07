//===--- ClusteredBitVector.cpp - Out-of-line code for the bit vector -----===//
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
//  This file implements support code for ClusteredBitVector.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/ClusteredBitVector.h"

#include "llvm/Support/raw_ostream.h"

using namespace nextcode;

void ClusteredBitVector::dump() const {
  print(llvm::errs());
}

/// Pretty-print the vector.
void ClusteredBitVector::print(llvm::raw_ostream &out) const {
  // Print in 8 clusters of 8 bits per row.
  if (!Bits) {
    return;
  }
  auto &v = Bits.value();
  for (size_t i = 0, e = size(); ; ) {
    out << (v[i++] ? '1' : '0');
    if (i == e) {
      return;
    } else if ((i & 64) == 0) {
      out << '\n';
    } else if ((i & 8) == 0) {
      out << ' ';
    }
  }
}
