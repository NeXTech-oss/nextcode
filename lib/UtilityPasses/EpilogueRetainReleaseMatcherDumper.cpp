//===--- EpilogueRetainReleaseMatcherDumper.cpp - Find Epilogue Releases --===//
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
/// This pass finds the epilogue releases matched to each argument of the
/// function.
///
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "sil-epilogue-release-dumper"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SIL/SILArgument.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILValue.h"
#include "nextcode/SILOptimizer/Analysis/AliasAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/ARCAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/Analysis.h"
#include "nextcode/SILOptimizer/Analysis/RCIdentityAnalysis.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"

using namespace nextcode;

//===----------------------------------------------------------------------===//
//                              Top Level Driver
//===----------------------------------------------------------------------===//

namespace {

/// Find and dump the epilogue release instructions for the arguments.
class SILEpilogueRetainReleaseMatcherDumper : public SILModuleTransform {

  void run() override {
    auto *RCIA = getAnalysis<RCIdentityAnalysis>();
    for (auto &Fn: *getModule()) {
      // Function is not definition.
      if (!Fn.isDefinition())
        continue;

      auto *AA = PM->getAnalysis<AliasAnalysis>(&Fn);

      llvm::outs() << "START: sil @" << Fn.getName() << "\n";

      // Handle @owned return value.
      ConsumedResultToEpilogueRetainMatcher RetMap(RCIA->get(&Fn), AA, &Fn); 
      for (auto &RI : RetMap)
        llvm::outs() << *RI;

      // Handle @owned function arguments.
      ConsumedArgToEpilogueReleaseMatcher RelMap(RCIA->get(&Fn), &Fn); 
      // Iterate over arguments and dump their epilogue releases.
      for (auto Arg : Fn.getArguments()) {
        llvm::outs() << *Arg;
        // Can not find an epilogue release instruction for the argument.
        for (auto &RI : RelMap.getReleasesForArgument(Arg))
          llvm::outs() << *RI;
      }

      llvm::outs() << "END: sil @" << Fn.getName() << "\n";
    }
  }

};
        
} // end anonymous namespace

SILTransform *nextcode::createEpilogueRetainReleaseMatcherDumper() {
  return new SILEpilogueRetainReleaseMatcherDumper();
}
