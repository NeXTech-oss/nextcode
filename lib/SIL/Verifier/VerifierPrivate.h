//===--- VerifierPrivate.h ------------------------------------------------===//
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

#ifndef NEXTCODE_SIL_VERIFIER_VERIFIERPRIVATE_H
#define NEXTCODE_SIL_VERIFIER_VERIFIERPRIVATE_H

#include "nextcode/Basic/MultiMapCache.h"
#include "nextcode/SIL/MemAccessUtils.h"
#include "nextcode/SIL/SILValue.h"

namespace nextcode {

class BeginAccessInst;
class LoadBorrowInst;
class SILValue;
class Operand;

namespace silverifier {

class LoadBorrowImmutabilityAnalysis {
  SmallMultiMapCache<AccessPath, Operand *> cache;
  DeadEndBlocks *deadEndBlocks = nullptr;

public:
  /// \p deadEndBlocks should be nullptr to enforce complete OSSA lifetimes.
  LoadBorrowImmutabilityAnalysis(DeadEndBlocks *deadEndBlocks,
                                 const SILFunction *f);

  /// Returns true if exhaustively lbi is guaranteed to never be invalidated by
  /// local writes.
  bool isImmutable(LoadBorrowInst *lbi);

private:
  bool isImmutableInScope(LoadBorrowInst *lbi,
                          AccessPathWithBase accessPathWithBase);
};

} // namespace silverifier
} // namespace nextcode

#endif
