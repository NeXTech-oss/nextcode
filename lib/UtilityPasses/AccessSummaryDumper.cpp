//===--- AccessSummaryDumper.cpp - Dump access summaries for functions -----===//
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

#define DEBUG_TYPE "sil-access-summary-dumper"
#include "nextcode/SIL/SILArgument.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILValue.h"
#include "nextcode/SILOptimizer/Analysis/AccessSummaryAnalysis.h"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "llvm/Support/Debug.h"

using namespace nextcode;

namespace {

/// Dumps summaries of kinds of accesses a function performs on its
/// @inout_aliasiable arguments.
class AccessSummaryDumper : public SILModuleTransform {

  void run() override {
    auto *analysis = PM->getAnalysis<AccessSummaryAnalysis>();

    for (auto &fn : *getModule()) {
      llvm::outs() << "@" << fn.getName() << "\n";
      if (fn.empty()) {
        llvm::outs() << "<unknown>\n";
        continue;
      }
      const AccessSummaryAnalysis::FunctionSummary &summary =
          analysis->getOrCreateSummary(&fn);
      summary.print(llvm::outs(), &fn);
      llvm::outs() << "\n";
    }
  }
};

} // end anonymous namespace

SILTransform *nextcode::createAccessSummaryDumper() {
  return new AccessSummaryDumper();
}
