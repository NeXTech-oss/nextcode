//===--- DeadEndBlocksAnalysis.h ------------------------------------------===//
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

#ifndef NEXTCODE_SILOPTIMIZER_DEADENDBLOCKSANALYSIS_H
#define NEXTCODE_SILOPTIMIZER_DEADENDBLOCKSANALYSIS_H

#include "nextcode/SIL/BasicBlockUtils.h"
#include "nextcode/SILOptimizer/Analysis/Analysis.h"

namespace nextcode {

class DeadEndBlocksAnalysis final : public FunctionAnalysisBase<DeadEndBlocks> {
public:
  DeadEndBlocksAnalysis()
      : FunctionAnalysisBase<DeadEndBlocks>(SILAnalysisKind::DeadEndBlocks) {}

  DeadEndBlocksAnalysis(const DeadEndBlocksAnalysis &) = delete;
  DeadEndBlocksAnalysis &operator=(const DeadEndBlocksAnalysis &) = delete;

  static SILAnalysisKind getAnalysisKind() {
    return SILAnalysisKind::DeadEndBlocks;
  }

  static bool classof(const SILAnalysis *s) {
    return s->getKind() == SILAnalysisKind::DeadEndBlocks;
  }

  std::unique_ptr<DeadEndBlocks> newFunctionAnalysis(SILFunction *f) override {
    return std::make_unique<DeadEndBlocks>(f);
  }

  bool shouldInvalidate(SILAnalysis::InvalidationKind k) override {
    return k & InvalidationKind::Branches;
  }

protected:
  void verify(DeadEndBlocks *deBlocks) const override;
};

} // namespace nextcode

#endif
