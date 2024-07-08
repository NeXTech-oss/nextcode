//===--- OwnershipDumper.cpp ----------------------------------------------===//
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
/// This is a simple utility pass that dumps the ValueOwnershipKind of all
/// SILValue in a module. It is meant to trigger assertions and verification of
/// these values.
///
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"

using namespace nextcode;

//===----------------------------------------------------------------------===//
//                               Implementation
//===----------------------------------------------------------------------===//

static void dumpInstruction(SILInstruction &ii) {
  llvm::outs() << "Visiting: " << ii;

  auto ops = ii.getAllOperands();
  if (!ops.empty()) {
    llvm::outs() << "Ownership Constraint:\n";
    for (const auto &op : ops) {
      llvm::outs() << "Op #: " << op.getOperandNumber() << "\n"
                      "Constraint: " << op.getOwnershipConstraint() << "\n";
    }
  }

  // If the instruction doesn't have any results, bail.
  auto results = ii.getResults();
  if (!results.empty()) {
    llvm::outs() << "Results Ownership Kinds:\n";
    for (auto v : results) {
      auto kind = v->getOwnershipKind();
      llvm::outs() << "Result: " << v;
      llvm::outs() << "Kind: " << kind << "\n";
    }
  }
}

//===----------------------------------------------------------------------===//
//                            Top Level Entrypoint
//===----------------------------------------------------------------------===//

namespace {

class OwnershipDumper : public SILFunctionTransform {
  void run() override {
    SILFunction *f = getFunction();
    llvm::outs() << "*** Dumping Function: '" << f->getName() << "'\n";
    for (auto &bb : *f) {
      // We only dump instructions right now.
      for (auto &ii : bb) {
        dumpInstruction(ii);
      }
    }
  }
};

} // end anonymous namespace

SILTransform *nextcode::createOwnershipDumper() { return new OwnershipDumper(); }
