//===--- SimplifyUnreachableContainingBlocks.cpp --------------------------===//
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
///
/// This file contains a simple utility pass that simplifies blocks that contain
/// unreachables by eliminating all other instructions. This includes
/// instructions with side-effects and no-return functions. It is only intended
/// to be used to simplify IR for testing or exploratory purposes.
///
//===----------------------------------------------------------------------===//

#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SIL/SILBasicBlock.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"

using namespace nextcode;

namespace {

class SimplifyUnreachableContainingBlocks : public SILFunctionTransform {
  void run() override {
    // For each block...
    for (auto &BB : *getFunction()) {
      // If the block does not contain an unreachable, just continue. There is
      // no further work to do.
      auto *UI = dyn_cast<UnreachableInst>(BB.getTerminator());
      if (!UI)
        continue;

      // Otherwise, eliminate all other instructions in the block.
      for (auto II = BB.begin(); &*II != UI;) {
        // Avoid iterator invalidation.
        auto *I = &*II;
        ++II;

        I->replaceAllUsesOfAllResultsWithUndef();
        I->eraseFromParent();
      }
    }
  }
};

} // end anonymous namespace

//===----------------------------------------------------------------------===//
//                           Top Level Entry Point
//===----------------------------------------------------------------------===//

SILTransform *nextcode::createSimplifyUnreachableContainingBlocks() {
  return new SimplifyUnreachableContainingBlocks();
}
