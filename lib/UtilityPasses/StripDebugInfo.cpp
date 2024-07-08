//===--- StripDebugInfo.cpp - Strip debug info from SIL -------------------===//
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

#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SIL/SILFunction.h"


using namespace nextcode;

static void stripFunction(SILFunction *F) {
  for (auto &BB : *F)
    for (auto II = BB.begin(), IE = BB.end(); II != IE;) {
      SILInstruction *Inst = &*II;
      ++II;

      if (!isa<DebugValueInst>(Inst))
        continue;

      Inst->eraseFromParent();
    }
}

namespace {
class StripDebugInfo : public nextcode::SILFunctionTransform {
  ~StripDebugInfo() override {}

  /// The entry point to the transformation.
  void run() override {
    stripFunction(getFunction());
    invalidateAnalysis(SILAnalysis::InvalidationKind::Instructions);
  }

};
} // end anonymous namespace


SILTransform *nextcode::createStripDebugInfo() {
  return new StripDebugInfo();
}
