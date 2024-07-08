//===--- LoopCanonicalizer.cpp --------------------------------------------===//
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
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This is a simple pass that can be used to apply loop canonicalizations to a
/// cfg. It also enables loop canonicalizations to be tested via FileCheck.
///
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "sil-loop-canonicalizer"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/Analysis/Analysis.h"
#include "nextcode/SILOptimizer/Analysis/DominanceAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/LoopAnalysis.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "nextcode/SILOptimizer/Utils/LoopUtils.h"

using namespace nextcode;

namespace {

class LoopCanonicalizer : public SILFunctionTransform {

  void run() override {
    SILFunction *F = getFunction();

    LLVM_DEBUG(llvm::dbgs() << "Attempt to canonicalize loops in "
                            << F->getName() << "\n");

    auto *LA = PM->getAnalysis<SILLoopAnalysis>();
    auto *LI = LA->get(F);

    if (LI->empty()) {
      LLVM_DEBUG(llvm::dbgs() << "    No loops to canonicalize!\n");
      return;
    }

    auto *DA = PM->getAnalysis<DominanceAnalysis>();
    auto *DI = DA->get(F);
    if (canonicalizeAllLoops(DI, LI)) {
      // We preserve loop info and the dominator tree.
      DA->lockInvalidation();
      LA->lockInvalidation();
      PM->invalidateAnalysis(F, SILAnalysis::InvalidationKind::FunctionBody);
      DA->unlockInvalidation();
      LA->unlockInvalidation();
    }
  }

};

} // end anonymous namespace

//===----------------------------------------------------------------------===//
//                            Top Level Entrypoint
//===----------------------------------------------------------------------===//

SILTransform *nextcode::createLoopCanonicalizer() {
  return new LoopCanonicalizer();
}
