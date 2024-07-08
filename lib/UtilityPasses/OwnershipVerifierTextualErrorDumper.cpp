//===--- OwnershipVerifierStateDumper.cpp ---------------------------------===//
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
///
/// \file
///
/// This is a simple utility pass that verifies the ownership of all SILValue in
/// a module with a special flag set that causes the verifier to emit textual
/// errors instead of asserting. This is done one function at a time so that we
/// can number the errors as we emit them so in FileCheck tests, we can be 100%
/// sure we exactly matched the number of errors.
///
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/BasicBlockUtils.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SILOptimizer/Analysis/DeadEndBlocksAnalysis.h"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"

using namespace nextcode;

//===----------------------------------------------------------------------===//
//                            Top Level Entrypoint
//===----------------------------------------------------------------------===//

namespace {

class OwnershipVerifierTextualErrorDumper : public SILFunctionTransform {
  void run() override {
    SILFunction *f = getFunction();
    auto *deBlocksAnalysis = getAnalysis<DeadEndBlocksAnalysis>();
    f->verifyOwnership(f->getModule().getOptions().OSSAVerifyComplete
                           ? nullptr
                           : deBlocksAnalysis->get(f));
  }
};

} // end anonymous namespace

SILTransform *nextcode::createOwnershipVerifierTextualErrorDumper() {
  return new OwnershipVerifierTextualErrorDumper();
}
