//===--- EpilogueARCMatcherDumper.cpp - Find Epilogue Releases ------------===//
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

#define DEBUG_TYPE "sil-epilogue-arc-dumper"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SIL/SILArgument.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILValue.h"
#include "nextcode/SILOptimizer/Analysis/Analysis.h"
#include "nextcode/SILOptimizer/Analysis/AliasAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/EpilogueARCAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/RCIdentityAnalysis.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"

using namespace nextcode;

//===----------------------------------------------------------------------===//
//                              Top Level Driver
//===----------------------------------------------------------------------===//

namespace {

/// Find and dump the epilogue release instructions for the arguments.
class SILEpilogueARCMatcherDumper : public SILModuleTransform {
  void run() override {
    for (auto &F: *getModule()) {
      // Function is not definition.
      if (!F.isDefinition())
        continue;

      // Find the epilogue releases of each owned argument. 
      for (auto Arg : F.getArguments()) {
        auto *EA = PM->getAnalysis<EpilogueARCAnalysis>()->get(&F);
        llvm::outs() <<"START: " <<  F.getName() << "\n";
        llvm::outs() << *Arg;

        // Find the retain instructions for the argument.
        llvm::SmallSetVector<SILInstruction *, 1> RelInsts = 
          EA->computeEpilogueARCInstructions(EpilogueARCContext::EpilogueARCKind::Release,
                                             Arg);
        for (auto I : RelInsts) {
          llvm::outs() << *I << "\n";
        }

        // Find the release instructions for the argument.
        llvm::SmallSetVector<SILInstruction *, 1> RetInsts = 
          EA->computeEpilogueARCInstructions(EpilogueARCContext::EpilogueARCKind::Retain,
                                             Arg);
        for (auto I : RetInsts) {
          llvm::outs() << *I << "\n";
        }

        llvm::outs() <<"FINISH: " <<  F.getName() << "\n";
      }
    }
  }

};
        
} // end anonymous namespace

SILTransform *nextcode::createEpilogueARCMatcherDumper() {
  return new SILEpilogueARCMatcherDumper();
}
