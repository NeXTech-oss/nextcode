//===--- LoopInfoPrinter.cpp - Print SIL Loop Info ------------------------===//
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

#include "nextcode/Basic/Assertions.h"
#include "nextcode/SILOptimizer/Analysis/LoopAnalysis.h"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SIL/SILVisitor.h"
#include "llvm/ADT/Statistic.h"

using namespace nextcode;

namespace {

class LoopInfoPrinter : public SILModuleTransform {

  /// The entry point to the transformation.
  void run() override {
    SILLoopAnalysis *LA = PM->getAnalysis<SILLoopAnalysis>();
    assert(LA && "Invalid LoopAnalysis");
    for (auto &Fn : *getModule()) {
      if (Fn.isExternalDeclaration()) continue;

      SILLoopInfo *LI = LA->get(&Fn);
      assert(LI && "Invalid loop info for function");


      if (LI->empty()) {
        llvm::errs() << "No loops in " << Fn.getName() << "\n";
        return;
      }

      llvm::errs() << "Loops in " << Fn.getName() << "\n";
      for (auto *LoopIt : *LI) {
        LoopIt->dump();
      }
    }
  }
};

} // end anonymous namespace


SILTransform *nextcode::createLoopInfoPrinter() {
  return new LoopInfoPrinter();
}

