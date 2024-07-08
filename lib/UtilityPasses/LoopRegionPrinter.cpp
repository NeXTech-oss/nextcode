//===--- LoopRegionPrinter.cpp --------------------------------------------===//
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
//
// Simple pass for testing the new loop region dumper analysis. Prints out
// information suitable for checking with filecheck.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "sil-loop-region-printer"

#include "nextcode/SILOptimizer/Analysis/LoopRegionAnalysis.h"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "llvm/Support/CommandLine.h"

using namespace nextcode;

static llvm::cl::opt<std::string>
    SILViewCFGOnlyFun("sil-loop-region-view-cfg-only-function",
                      llvm::cl::init(""),
                      llvm::cl::desc("Only produce a graphviz file for the "
                                     "loop region info of this function"));

static llvm::cl::opt<std::string>
    SILViewCFGOnlyFuns("sil-loop-region-view-cfg-only-functions",
                       llvm::cl::init(""),
                       llvm::cl::desc("Only produce a graphviz file for the "
                                      "loop region info for the functions "
                                      "whose name contains this substring"));

namespace {

class LoopRegionViewText : public SILModuleTransform {
  void run() override {
    invalidateAll();
    auto *lra = PM->getAnalysis<LoopRegionAnalysis>();

    for (auto &fn : *getModule()) {
      if (fn.isExternalDeclaration())
        continue;
      if (!SILViewCFGOnlyFun.empty() && fn.getName() != SILViewCFGOnlyFun)
        continue;
      if (!SILViewCFGOnlyFuns.empty() &&
          !fn.getName().contains(SILViewCFGOnlyFuns))
        continue;

      // Ok, we are going to analyze this function. Invalidate all state
      // associated with it so we recompute the loop regions.
      llvm::outs() << "Start @" << fn.getName() << "@\n";
      lra->get(&fn)->dump();
      llvm::outs() << "End @" << fn.getName() << "@\n";
      llvm::outs().flush();
    }
  }
};

class LoopRegionViewCFG : public SILModuleTransform {
  void run() override {
    invalidateAll();
    auto *lra = PM->getAnalysis<LoopRegionAnalysis>();

    for (auto &fn : *getModule()) {
      if (fn.isExternalDeclaration())
        continue;
      if (!SILViewCFGOnlyFun.empty() && fn.getName() != SILViewCFGOnlyFun)
        continue;
      if (!SILViewCFGOnlyFuns.empty() &&
          !fn.getName().contains(SILViewCFGOnlyFuns))
        continue;

      // Ok, we are going to analyze this function. Invalidate all state
      // associated with it so we recompute the loop regions.
      lra->get(&fn)->viewLoopRegions();
    }
  }
};

} // end anonymous namespace

SILTransform *nextcode::createLoopRegionViewText() {
  return new LoopRegionViewText();
}

SILTransform *nextcode::createLoopRegionViewCFG() {
  return new LoopRegionViewCFG();
}
