//===--- DeadEndBlocksAnalysis.cpp ----------------------------------------===//
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

#include "nextcode/SILOptimizer/Analysis/DeadEndBlocksAnalysis.h"
#include "nextcode/SILOptimizer/OptimizerBridging.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILFunction.h"

using namespace nextcode;

void DeadEndBlocksAnalysis::verify(DeadEndBlocks *deBlocks) const {
  // If the passed in deBlocks has not computed, there is nothing to check.
  if (!deBlocks->isComputed())
    return;

  // Then create our new dead end blocks instance so we can check the internal
  // state of our input against it.
  auto *fn = deBlocks->getFunction();
  DeadEndBlocks newBlocks(fn);

  // Make sure that all values that deBlocks thinks is unreachable are
  // actually unreachable.
  //
  // NOTE: We verify like this b/c DeadEndBlocks looks up state lazily so we
  // can only check the work we have done so far.
  for (auto &block : *fn) {
    if (deBlocks->isDeadEnd(&block)) {
      if (!newBlocks.isDeadEnd(&block)) {
        llvm::errs() << "DeadEndBlocksAnalysis Error! Found dead end block "
                        "that is no longer a dead end block?!";
        llvm_unreachable("standard error assertion");
      }
    } else {
      if (newBlocks.isDeadEnd(&block)) {
        llvm::errs() << "DeadEndBlocksAnalysis Error! Found reachable block "
                        "that is no longer reachable?!";
        llvm_unreachable("standard error assertion");
      }
    }
  }
}

//===----------------------------------------------------------------------===//
//                              Main Entry Point
//===----------------------------------------------------------------------===//

SILAnalysis *nextcode::createDeadEndBlocksAnalysis(SILModule *) {
  return new DeadEndBlocksAnalysis();
}
