//===--- CFGPrinter.cpp - CFG printer pass --------------------------------===//
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
// This file defines external functions that can be called to explicitly
// instantiate the CFG printer.
//
//===----------------------------------------------------------------------===//

#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "nextcode/SIL/CFG.h"
#include "nextcode/SIL/SILBasicBlock.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILFunction.h"
#include "llvm/Support/CommandLine.h"

using namespace nextcode;

//===----------------------------------------------------------------------===//
//                                  Options
//===----------------------------------------------------------------------===//

llvm::cl::opt<std::string> SILViewCFGOnlyFun(
    "sil-view-cfg-only-function", llvm::cl::init(""),
    llvm::cl::desc("Only produce a graphviz file for this function"));

llvm::cl::opt<std::string> SILViewCFGOnlyFuns(
    "sil-view-cfg-only-functions", llvm::cl::init(""),
    llvm::cl::desc("Only produce a graphviz file for the sil for the functions "
                   "whose name contains this substring"));

//===----------------------------------------------------------------------===//
//                              Top Level Driver
//===----------------------------------------------------------------------===//

namespace {

class SILCFGPrinter : public SILFunctionTransform {
  /// The entry point to the transformation.
  void run() override {
    SILFunction *F = getFunction();

    // If we are not supposed to dump view this cfg, return.
    if (!SILViewCFGOnlyFun.empty() && F && F->getName() != SILViewCFGOnlyFun)
      return;
    if (!SILViewCFGOnlyFuns.empty() && F &&
        !F->getName().contains(SILViewCFGOnlyFuns))
      return;

    F->viewCFG();
  }
};

} // end anonymous namespace

SILTransform *nextcode::createCFGPrinter() {
  return new SILCFGPrinter();
}
