//===--- LoopAnalysis.h - SIL Loop Analysis ---------------------*- C++ -*-===//
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

#ifndef NEXTCODE_SILOPTIMIZER_ANALYSIS_LOOPINFOANALYSIS_H
#define NEXTCODE_SILOPTIMIZER_ANALYSIS_LOOPINFOANALYSIS_H

#include "nextcode/SIL/CFG.h"
#include "nextcode/SIL/LoopInfo.h"
#include "nextcode/SIL/SILBasicBlock.h"
#include "nextcode/SILOptimizer/Analysis/Analysis.h"

namespace nextcode {
  class DominanceInfo;
  class SILLoop;
  class DominanceAnalysis;
}

namespace nextcode {

/// Computes natural loop information for SIL basic blocks.
class SILLoopAnalysis : public FunctionAnalysisBase<SILLoopInfo> {
  DominanceAnalysis *DA;
public:
  SILLoopAnalysis(SILModule *)
      : FunctionAnalysisBase(SILAnalysisKind::Loop), DA(nullptr) {}

  static bool classof(const SILAnalysis *S) {
    return S->getKind() == SILAnalysisKind::Loop;
  }

  virtual bool shouldInvalidate(SILAnalysis::InvalidationKind K) override {
    return K & InvalidationKind::Branches;
  }

  // Computes loop information for the given function using dominance
  // information.
  virtual std::unique_ptr<SILLoopInfo>
  newFunctionAnalysis(SILFunction *F) override;

  virtual void initialize(SILPassManager *PM) override;
};

} // end namespace nextcode

#endif
