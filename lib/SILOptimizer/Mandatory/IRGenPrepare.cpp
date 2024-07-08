//===--- IRGenPrepare.cpp -------------------------------------------------===//
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
/// Cleanup SIL to make it suitable for IRGen.
///
/// We perform the following canonicalizations:
///
/// 1. We remove calls to Builtin.poundAssert() and Builtin.staticReport(),
///    which are not needed post SIL.
///
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "sil-cleanup"
#include "nextcode/SIL/SILBuilder.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "nextcode/SILOptimizer/Utils/InstructionDeleter.h"
#include "nextcode/Strings.h"

using namespace nextcode;

static bool cleanFunction(SILFunction &fn) {
  bool madeChange = false;

  for (auto &bb : fn) {
    for (auto i = bb.begin(), e = bb.end(); i != e;) {
      // Make sure there is no iterator invalidation if the inspected
      // instruction gets removed from the block.
      SILInstruction *inst = &*i;
      ++i;

      // Remove calls to Builtin.poundAssert() and Builtin.staticReport().
      auto *bi = dyn_cast<BuiltinInst>(inst);
      if (!bi) {
        continue;
      }

      switch (bi->getBuiltinInfo().ID) {
        case BuiltinValueKind::CondFailMessage: {
          SILBuilderWithScope Builder(bi);
          Builder.createCondFail(bi->getLoc(), bi->getOperand(0),
            "unknown program error");
          LLVM_FALLTHROUGH;
        }
        case BuiltinValueKind::PoundAssert:
        case BuiltinValueKind::StaticReport: {
          // The call to the builtin should get removed before we reach
          // IRGen.
          InstructionDeleter deleter;
          deleter.forceDelete(bi);
          // StaticReport only takes trivial operands, and therefore doesn't
          // require fixing the lifetime of its operands.
          deleter.cleanupDeadInstructions();
          madeChange = true;
          break;
        }
        default:
          break;
      }
    }
  }

  return madeChange;
}

//===----------------------------------------------------------------------===//
//                            Top Level Entrypoint
//===----------------------------------------------------------------------===//

namespace {

class IRGenPrepare : public SILFunctionTransform {
  void run() override {
    SILFunction *F = getFunction();

    if (getOptions().EmbeddedNeXTCode) {
      // In embedded nextcode all the code is generated in the top-level module.
      // Even de-serialized functions must be code-gen'd.
      SILLinkage linkage = F->getLinkage();
      if (isAvailableExternally(linkage)) {
        F->setLinkage(stripExternalFromLinkage(linkage));
      }
    }

    bool shouldInvalidate = cleanFunction(*F);

    if (shouldInvalidate)
      invalidateAnalysis(SILAnalysis::InvalidationKind::Instructions);
  }
};

} // end anonymous namespace


SILTransform *nextcode::createIRGenPrepare() {
  return new IRGenPrepare();
}
