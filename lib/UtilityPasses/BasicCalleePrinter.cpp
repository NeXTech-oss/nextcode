//===--- BasicCalleePrinter.cpp - Callee cache printing pass --------------===//
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
// This pass prints the callees of functions as determined by the
// BasicCalleeAnalysis. The pass is strictly for testing that
// analysis.
//
//===----------------------------------------------------------------------===//

#include "nextcode/SILOptimizer/Analysis/BasicCalleeAnalysis.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"

using namespace nextcode;

#define DEBUG_TYPE "basic-callee-printer"

namespace {

class BasicCalleePrinterPass : public SILModuleTransform {
  BasicCalleeAnalysis *BCA;

  void printCallees(FullApplySite FAS) {
    llvm::outs() << "Function call site:\n";
    if (auto *Callee = FAS.getCallee()->getDefiningInstruction())
      llvm::outs() << *Callee;
    llvm::outs() << *FAS.getInstruction();

    auto Callees = BCA->getCalleeList(FAS);
    Callees.print(llvm::outs());
  }

  /// The entry point to the transformation.
  void run() override {
    BCA = getAnalysis<BasicCalleeAnalysis>();
    for (auto &Fn : *getModule()) {
      if (Fn.isExternalDeclaration()) continue;
      for (auto &B : Fn)
        for (auto &I : B)
          if (auto FAS = FullApplySite::isa(&I))
            printCallees(FAS);
    }
  }

};

} // end anonymous namespace

SILTransform *nextcode::createBasicCalleePrinter() {
  return new BasicCalleePrinterPass();
}
