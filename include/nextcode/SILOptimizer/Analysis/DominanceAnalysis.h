//===--- DominanceAnalysis.h - SIL Dominance Analysis -----------*- C++ -*-===//
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

#ifndef NEXTCODE_SILOPTIMIZER_ANALYSIS_DOMINANCEANALYSIS_H
#define NEXTCODE_SILOPTIMIZER_ANALYSIS_DOMINANCEANALYSIS_H

#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/Dominance.h"
#include "nextcode/SILOptimizer/Analysis/Analysis.h"

namespace nextcode {
class SILModule;
class SILInstruction;

class DominanceAnalysis : public FunctionAnalysisBase<DominanceInfo> {
protected:
  virtual void verify(DominanceInfo *DI) const override {
    if (DI->roots().empty())
      return;
    DI->verify();
  }

public:
  DominanceAnalysis()
      : FunctionAnalysisBase<DominanceInfo>(SILAnalysisKind::Dominance) {}

  DominanceAnalysis(const DominanceAnalysis &) = delete;
  DominanceAnalysis &operator=(const DominanceAnalysis &) = delete;

  static SILAnalysisKind getAnalysisKind() {
    return SILAnalysisKind::Dominance;
  }

  static bool classof(const SILAnalysis *S) {
    return S->getKind() == SILAnalysisKind::Dominance;
  }

  std::unique_ptr<DominanceInfo> newFunctionAnalysis(SILFunction *F) override {
    return std::make_unique<DominanceInfo>(F);
  }

  virtual bool shouldInvalidate(SILAnalysis::InvalidationKind K) override {
    return K & InvalidationKind::Branches;
  }
};

class PostDominanceAnalysis : public FunctionAnalysisBase<PostDominanceInfo> {
protected:
  virtual void verify(PostDominanceInfo *PDI) const override {
    if (PDI->roots().empty())
      return;
    PDI->verify();
  }

public:
  PostDominanceAnalysis()
      : FunctionAnalysisBase<PostDominanceInfo>(
            SILAnalysisKind::PostDominance) {}

  PostDominanceAnalysis(const PostDominanceAnalysis &) = delete;
  PostDominanceAnalysis &operator=(const PostDominanceAnalysis &) = delete;

  static SILAnalysisKind getAnalysisKind() {
    return SILAnalysisKind::PostDominance;
  }

  static bool classof(const SILAnalysis *S) {
    return S->getKind() == SILAnalysisKind::PostDominance;
  }

  std::unique_ptr<PostDominanceInfo>
  newFunctionAnalysis(SILFunction *F) override {
    return std::make_unique<PostDominanceInfo>(F);
  }

  virtual bool shouldInvalidate(SILAnalysis::InvalidationKind K) override {
    return K & InvalidationKind::Branches;
  }
};

} // end namespace nextcode



#endif
