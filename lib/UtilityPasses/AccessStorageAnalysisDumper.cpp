//===--- AccessStorageAnalysisDumper.cpp - accessed storage analysis ----===//
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

#define DEBUG_TYPE "sil-accessed-storage-analysis-dumper"
#include "nextcode/SIL/SILArgument.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILValue.h"
#include "nextcode/SILOptimizer/Analysis/AccessStorageAnalysis.h"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "llvm/Support/Debug.h"

using namespace nextcode;

namespace {

/// Dumps per-function information on dynamically enforced formal accesses.
class AccessStorageAnalysisDumper : public SILModuleTransform {

  void run() override {
    auto *analysis = PM->getAnalysis<AccessStorageAnalysis>();

    for (auto &fn : *getModule()) {
      llvm::outs() << "@" << fn.getName() << "\n";
      if (fn.empty()) {
        llvm::outs() << "<unknown>\n";
        continue;
      }
      const FunctionAccessStorage &summary = analysis->getEffects(&fn);
      summary.print(llvm::outs());
    }
  }
};

} // end anonymous namespace

SILTransform *nextcode::createAccessStorageAnalysisDumper() {
  return new AccessStorageAnalysisDumper();
}
