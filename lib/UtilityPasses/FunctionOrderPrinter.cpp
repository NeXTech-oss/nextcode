//===--- FunctionOrderPrinter.cpp - Function ordering test pass -----------===//
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
// This pass prints a bottom-up ordering of functions in the module (in the
// sense that each function is printed before functions that call it).
//
//===----------------------------------------------------------------------===//

#include "nextcode/SILOptimizer/Analysis/FunctionOrder.h"
#include "nextcode/Demangling/Demangle.h"
#include "nextcode/SILOptimizer/Analysis/BasicCalleeAnalysis.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "llvm/Support/raw_ostream.h"

using namespace nextcode;

#define DEBUG_TYPE "function-order-printer"

namespace {

class FunctionOrderPrinterPass : public SILModuleTransform {
  BasicCalleeAnalysis *BCA;

  /// The entry point to the transformation.
  void run() override {
    BCA = getAnalysis<BasicCalleeAnalysis>();
    auto &M = *getModule();
    BottomUpFunctionOrder Orderer(M, BCA);

    llvm::outs() << "Bottom up function order:\n";
    auto SCCs = Orderer.getSCCs();
    for (auto &SCC : SCCs) {
      std::string Indent;

      if (SCC.size() != 1) {
        llvm::outs() << "Non-trivial SCC:\n";
        Indent = std::string(2, ' ');
      }

      for (auto *F : SCC) {
        llvm::outs() << Indent
                     << Demangle::demangleSymbolAsString(F->getName())
                     << "\n";
      }
    }
    llvm::outs() << "\n";
  }

};

} // end anonymous namespace

SILTransform *nextcode::createFunctionOrderPrinter() {
  return new FunctionOrderPrinterPass();
}
