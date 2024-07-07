//===--- ColdBlockInfo.h - Fast/slow path analysis for SIL CFG --*- C++ -*-===//
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

#ifndef NEXTCODE_SILOPTIMIZER_ANALYSIS_COLDBLOCKS_H
#define NEXTCODE_SILOPTIMIZER_ANALYSIS_COLDBLOCKS_H

#include "llvm/ADT/DenseMap.h"
#include "nextcode/SIL/SILValue.h"

namespace nextcode {
class DominanceAnalysis;
class SILBasicBlock;

/// Cache a set of basic blocks that have been determined to be cold or hot.
///
/// This does not inherit from SILAnalysis because it is not worth preserving
/// across passes.
class ColdBlockInfo {
  DominanceAnalysis *DA;

  /// Each block in this map has been determined to be either cold or hot.
  llvm::DenseMap<const SILBasicBlock*, bool> ColdBlockMap;

  // This is a cache and shouldn't be copied around.
  ColdBlockInfo(const ColdBlockInfo &) = delete;
  ColdBlockInfo &operator=(const ColdBlockInfo &) = delete;

  /// Tri-value return code for checking branch hints.
  enum BranchHint : unsigned {
    None,
    LikelyTrue,
    LikelyFalse
  };

  enum {
    RecursionDepthLimit = 3
  };

  BranchHint getBranchHint(SILValue Cond, int recursionDepth);

  bool isSlowPath(const SILBasicBlock *FromBB, const SILBasicBlock *ToBB,
                  int recursionDepth);

  bool isCold(const SILBasicBlock *BB,
              int recursionDepth);

public:
  ColdBlockInfo(DominanceAnalysis *DA): DA(DA) {}

  bool isSlowPath(const SILBasicBlock *FromBB, const SILBasicBlock *ToBB) {
    return isSlowPath(FromBB, ToBB, 0);
  }

  bool isCold(const SILBasicBlock *BB) { return isCold(BB, 0); }
};
} // end namespace nextcode

#endif
