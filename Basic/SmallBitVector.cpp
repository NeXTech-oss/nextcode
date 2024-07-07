//===--- SmallBitVector.cpp -----------------------------------------------===//
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
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/SmallBitVector.h"
#include "llvm/Support/Debug.h"

using namespace llvm;

/// Debug dump a location bit vector.
void nextcode::printBitsAsArray(raw_ostream &OS, const SmallBitVector &bits,
                             bool bracketed) {
  if (!bracketed) {
    for (unsigned i = 0, e = bits.size(); i != e; ++i)
      OS << (bits[i] ? '1' : '0');
  }
  const char *separator = "";
  OS << '[';
  for (int idx = bits.find_first(); idx >= 0; idx = bits.find_next(idx)) {
    OS << separator << idx;
    separator = ",";
  }
  OS << ']';
}

void nextcode::dumpBits(const SmallBitVector &bits) { dbgs() << bits << '\n'; }
