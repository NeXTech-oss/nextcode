//===--- AADumper.cpp - Compare all values in Function with AA ------------===//
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
/// This pass collects all values in a function and applies alias analysis to
/// them. The purpose of this is to enable unit tests for SIL Alias Analysis
/// implementations independent of any other passes.
///
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "sil-aa-evaluator"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SIL/SILArgument.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILValue.h"
#include "nextcode/SILOptimizer/Analysis/AliasAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/Analysis.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "llvm/Support/Debug.h"

using namespace nextcode;

//===----------------------------------------------------------------------===//
//                               Value Gatherer
//===----------------------------------------------------------------------===//

// Return a list of all instruction values in Fn. Returns true if we have at
// least two values to compare.
static bool gatherValues(SILFunction &Fn, std::vector<SILValue> &Values) {
  for (auto &BB : Fn) {
    for (auto *Arg : BB.getArguments())
      Values.push_back(SILValue(Arg));
    for (auto &II : BB)
      for (auto result : II.getResults())
        Values.push_back(result);
  }
  return Values.size() > 1;
}

//===----------------------------------------------------------------------===//
//                              Top Level Driver
//===----------------------------------------------------------------------===//

namespace {

/// Dumps the alias relations between all instructions of a function.
class SILAADumper : public SILModuleTransform {

  void run() override {
    for (auto &Fn: *getModule()) {
      llvm::outs() << "@" << Fn.getName() << "\n";
      // Gather up all Values in Fn.
      std::vector<SILValue> Values;
      if (!gatherValues(Fn, Values))
        continue;

      AliasAnalysis *AA = PM->getAnalysis<AliasAnalysis>(&Fn);

      // A cache
      llvm::DenseMap<uint64_t, AliasAnalysis::AliasResult> Results;

      // Emit the N^2 alias evaluation of the values.
      unsigned PairCount = 0;
      for (unsigned i1 = 0, e1 = Values.size(); i1 != e1; ++i1) {
        for (unsigned i2 = 0, e2 = Values.size(); i2 != e2; ++i2) {
          auto V1 = Values[i1];
          auto V2 = Values[i2];

          auto Result =
              AA->alias(V1, V2, computeTBAAType(V1), computeTBAAType(V2));

          // Results should always be the same. But if they are different print
          // it out so we find the error. This should make our test results less
          // verbose.
          uint64_t Key = uint64_t(i1) | (uint64_t(i2) << 32);
          uint64_t OpKey = uint64_t(i2) | (uint64_t(i1) << 32);
          auto R = Results.find(OpKey);
          if (R != Results.end() && R->second == Result)
            continue;

          Results[Key] = Result;
          llvm::outs() << "PAIR #" << PairCount++ << ".\n" << V1 << V2 << Result
                      << "\n";
        }
      }
          llvm::outs() << "\n";
    }
  }

};
        
} // end anonymous namespace

SILTransform *nextcode::createAADumper() { return new SILAADumper(); }
