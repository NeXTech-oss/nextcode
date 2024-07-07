//===--- NonLocalAccessBlockAnalysis.h - Nonlocal end_access ----*- C++ -*-===//
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
///
/// Cache the set of blocks that contain a non-local end_access, which is a rare
/// occurrence. Optimizations that are driven by a known-use analysis, such as
/// CanonicalOSSA, don't need to scan instructions that are unrelated to the SSA
/// def-use graph. However, they may still need to be aware of unrelated access
/// scope boundaries. By querying this analysis, they can avoid scanning all
/// instructions just to deal with the extremely rare case of an end_access that
/// spans blocks within the relevant SSA lifetime.
///
/// By default, this analysis is invalidated whenever instructions or blocks are
/// changed, but it should ideally be preserved by passes that invalidate
/// instructions but don't create any new access scopes or move end_access
/// across blocks, which is unusual.
///
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SILOPTIMIZER_ANALYSIS_NONLOCALACCESSBLOCKS_H
#define NEXTCODE_SILOPTIMIZER_ANALYSIS_NONLOCALACCESSBLOCKS_H

#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/Compiler.h"
#include "nextcode/SILOptimizer/Analysis/Analysis.h"
#include "llvm/ADT/SmallPtrSet.h"

namespace nextcode {

class SILBasicBlock;
class SILFunction;

class NonLocalAccessBlocks {
  friend class NonLocalAccessBlockAnalysis;

  SILFunction *function;
  llvm::SmallPtrSet<SILBasicBlock *, 4> accessBlocks;

public:
  NonLocalAccessBlocks(SILFunction *function) : function(function) {}

  SILFunction *getFunction() const { return function; }

  bool containsNonLocalEndAccess(SILBasicBlock *block) const {
    return accessBlocks.count(block);
  }

  /// Perform NonLocalAccessBlockAnalysis for this function. Populate
  /// this->accessBlocks with all blocks containing a non-local end_access.
  void compute();
};

class NonLocalAccessBlockAnalysis
    : public FunctionAnalysisBase<NonLocalAccessBlocks> {
public:
  static bool classof(const SILAnalysis *S) {
    return S->getKind() == SILAnalysisKind::NonLocalAccessBlock;
  }
  NonLocalAccessBlockAnalysis()
      : FunctionAnalysisBase<NonLocalAccessBlocks>(
          SILAnalysisKind::NonLocalAccessBlock) {}

  NonLocalAccessBlockAnalysis(const NonLocalAccessBlockAnalysis &) = delete;

  NonLocalAccessBlockAnalysis &
  operator=(const NonLocalAccessBlockAnalysis &) = delete;

protected:
  virtual std::unique_ptr<NonLocalAccessBlocks>
  newFunctionAnalysis(SILFunction *function) override {
    auto result = std::make_unique<NonLocalAccessBlocks>(function);
    result->compute();
    return result;
  }

  virtual bool shouldInvalidate(SILAnalysis::InvalidationKind kind) override {
    return kind & InvalidationKind::BranchesAndInstructions;
  }

  NEXTCODE_ASSERT_ONLY_DECL(
      virtual void verify(NonLocalAccessBlocks *accessBlocks) const override {
        NonLocalAccessBlocks checkAccessBlocks(accessBlocks->function);
        checkAccessBlocks.compute();
        assert(llvm::all_of(checkAccessBlocks.accessBlocks,
                            [&](SILBasicBlock *bb) {
                              return accessBlocks->accessBlocks.count(bb);
                            }));
      })
};

} // end namespace nextcode

#endif