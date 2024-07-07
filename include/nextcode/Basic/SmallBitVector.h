//===--- SmallBitVector.h -------------------------------------------------===//
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

#ifndef NEXTCODE_BASIC_SMALLBITVECTOR_H
#define NEXTCODE_BASIC_SMALLBITVECTOR_H

#include "llvm/ADT/SmallBitVector.h"
#include "llvm/Support/raw_ostream.h"

namespace nextcode {

void printBitsAsArray(llvm::raw_ostream &OS, const llvm::SmallBitVector &bits,
                      bool bracketed);

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &OS,
                                     const llvm::SmallBitVector &bits) {
  printBitsAsArray(OS, bits, /*bracketed=*/false);
  return OS;
}

void dumpBits(const llvm::SmallBitVector &bits);

} // namespace nextcode

#endif
