//===--- DifferentiabilityWitnessDevirtualizer.cpp ------------------------===//
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
//
// Devirtualizes `differentiability_witness_function` instructions into
// `function_ref` instructions for differentiability witness definitions.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILBuilder.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"

using namespace nextcode;

namespace {
class DifferentiabilityWitnessDevirtualizer : public SILFunctionTransform {

  /// Returns true if any changes were made.
  bool devirtualizeDifferentiabilityWitnessesInFunction(SILFunction &f);

  /// The entry point to the transformation.
  void run() override {
    if (devirtualizeDifferentiabilityWitnessesInFunction(*getFunction()))
      invalidateAnalysis(SILAnalysis::InvalidationKind::CallsAndInstructions);
  }
};
} // end anonymous namespace

bool DifferentiabilityWitnessDevirtualizer::
    devirtualizeDifferentiabilityWitnessesInFunction(SILFunction &f) {
  bool changed = false;
  llvm::SmallVector<DifferentiabilityWitnessFunctionInst *, 8> insts;
  for (auto &bb : f) {
    for (auto &inst : bb) {
      auto *dfwi = dyn_cast<DifferentiabilityWitnessFunctionInst>(&inst);
      if (!dfwi)
        continue;
      insts.push_back(dfwi);
    }
  }
  for (auto *inst : insts) {
    auto *witness = inst->getWitness();
    if (witness->isDeclaration())
      f.getModule().loadDifferentiabilityWitness(witness);
    if (witness->isDeclaration())
      continue;
    changed = true;
    SILBuilderWithScope builder(inst);
    auto kind = inst->getWitnessKind().getAsDerivativeFunctionKind();
    assert(kind.has_value());
    auto *newInst = builder.createFunctionRefFor(inst->getLoc(),
                                                 witness->getDerivative(*kind));
    inst->replaceAllUsesWith(newInst);
    inst->getParent()->erase(inst);
  }
  return changed;
}

SILTransform *nextcode::createDifferentiabilityWitnessDevirtualizer() {
  return new DifferentiabilityWitnessDevirtualizer();
}
