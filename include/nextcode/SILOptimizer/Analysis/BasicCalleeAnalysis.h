//===- BasicCalleeAnalysis.h - Determine callees per call site --*- C++ -*-===//
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

#ifndef NEXTCODE_SILOPTIMIZER_ANALYSIS_BASICCALLEEANALYSIS_H
#define NEXTCODE_SILOPTIMIZER_ANALYSIS_BASICCALLEEANALYSIS_H

#include "nextcode/SILOptimizer/Analysis/Analysis.h"
#include "nextcode/SIL/CalleeCache.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/ApplySite.h"
#include "nextcode/SIL/SILModule.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/PointerIntPair.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/TinyPtrVector.h"
#include "llvm/Support/Allocator.h"

namespace nextcode {

class BasicCalleeAnalysis : public SILAnalysis {
  SILModule &M;
  std::unique_ptr<CalleeCache> Cache;

public:

  BasicCalleeAnalysis(SILModule *M)
      : SILAnalysis(SILAnalysisKind::BasicCallee), M(*M), Cache(nullptr) {}

  ~BasicCalleeAnalysis();

  static bool classof(const SILAnalysis *S) {
    return S->getKind() == SILAnalysisKind::BasicCallee;
  }

  /// Invalidate all information in this analysis.
  virtual void invalidate() override {
    Cache.reset();
  }

  /// Invalidate all of the information for a specific function.
  virtual void invalidate(SILFunction *F, InvalidationKind K) override {
    // No invalidation needed because the analysis does not cache anything
    // per call-site in functions.
  }

  /// Notify the analysis about a newly created function.
  virtual void notifyAddedOrModifiedFunction(SILFunction *F) override {
    // Nothing to be done because the analysis does not cache anything
    // per call-site in functions.
  }

  /// Notify the analysis about a function which will be deleted from the
  /// module.
  virtual void notifyWillDeleteFunction(SILFunction *F) override {
    // No invalidation needed because the analysis does not cache anything per
    // call-site in functions.
  }

  /// Notify the analysis about changed witness or vtables.
  virtual void invalidateFunctionTables() override {
    Cache.reset();
  }

  NEXTCODE_DEBUG_DUMP;

  void print(llvm::raw_ostream &os) const;

  void updateCache() {
    if (!Cache)
      Cache = std::make_unique<CalleeCache>(M);
  }

  CalleeList getCalleeList(FullApplySite FAS) {
    updateCache();
    return Cache->getCalleeList(FAS);
  }

  CalleeList getCalleeListOfValue(SILValue callee) {
    updateCache();
    return Cache->getCalleeListOfValue(callee);
  }

  CalleeList getDestructors(SILType type, bool isExactType) {
    updateCache();
    return Cache->getDestructors(type, isExactType);
  }

  MemoryBehavior getMemoryBehavior(ApplySite as, bool observeRetains);

  CalleeCache *getCalleeCache() { return Cache.get(); }
};

bool isDeinitBarrier(SILInstruction *const instruction,
                     BasicCalleeAnalysis *bca);

} // end namespace nextcode

#endif
