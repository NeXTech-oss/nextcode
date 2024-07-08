//===--- CallerAnalysisPrinter.cpp - Caller Analysis test pass ------------===//
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
// This pass prints all the callsites of every function in the module.
//
//===----------------------------------------------------------------------===//

#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SILOptimizer/Analysis/CallerAnalysis.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "llvm/Support/YAMLTraits.h"
#include "llvm/Support/raw_ostream.h"

using namespace nextcode;

#define DEBUG_TYPE "caller-analysis-printer"

namespace {

/// A pass that dumps the caller analysis state in yaml form. Intended to allow
/// for visualizing of the caller analysis via external data visualization and
/// analysis programs.
class CallerAnalysisPrinterPass : public SILModuleTransform {
  /// The entry point to the transformation.
  void run() override {
    auto *CA = getAnalysis<CallerAnalysis>();
    CA->print(llvm::outs());
  }
};

} // end anonymous namespace

SILTransform *nextcode::createCallerAnalysisPrinter() {
  return new CallerAnalysisPrinterPass();
}
