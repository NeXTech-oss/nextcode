//===--- PostOrderAnalysis.h - SIL POT and RPOT Analysis --------*- C++ -*-===//
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

#ifndef NEXTCODE_SILOPTIMIZER_ANALYSIS_POSTORDERANALYSIS_H
#define NEXTCODE_SILOPTIMIZER_ANALYSIS_POSTORDERANALYSIS_H

#include "nextcode/Basic/Range.h"
#include "nextcode/SIL/CFG.h"
#include "nextcode/SIL/PostOrder.h"
#include "nextcode/SIL/SILBasicBlock.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SILOptimizer/Analysis/Analysis.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/iterator_range.h"

namespace nextcode {

class SILBasicBlock;
class SILFunction;

/// This class is a simple wrapper around the POT iterator provided by LLVM. It
/// lazily re-evaluates the post order when it is invalidated so that we do not
/// reform the post order over and over again (it can be expensive).
class PostOrderAnalysis : public FunctionAnalysisBase<PostOrderFunctionInfo> {
protected:
  virtual std::unique_ptr<PostOrderFunctionInfo>
  newFunctionAnalysis(SILFunction *F) override {
    return std::make_unique<PostOrderFunctionInfo>(F);
  }

  virtual bool shouldInvalidate(SILAnalysis::InvalidationKind K) override {
    return K & InvalidationKind::Branches;
  }

public:
  PostOrderAnalysis()
      : FunctionAnalysisBase<PostOrderFunctionInfo>(
            SILAnalysisKind::PostOrder) {}

  // This is a cache and shouldn't be copied around.
  PostOrderAnalysis(const PostOrderAnalysis &) = delete;
  PostOrderAnalysis &operator=(const PostOrderAnalysis &) = delete;

  static bool classof(const SILAnalysis *S) {
    return S->getKind() == SILAnalysisKind::PostOrder;
  }
};

} // end namespace nextcode

#endif
