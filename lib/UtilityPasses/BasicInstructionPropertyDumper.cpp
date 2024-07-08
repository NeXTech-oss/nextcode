//===--- BasicInstructionPropertyDumper.cpp -------------------------------===//
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

#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"

using namespace nextcode;

namespace {

class BasicInstructionPropertyDumper : public SILModuleTransform {
  void run() override {
    for (auto &Fn : *getModule()) {
      unsigned Count = 0;
      llvm::outs() << "@" << Fn.getName() << "\n";
      for (auto &BB : Fn) {
        for (auto &I : BB) {
          llvm::outs() << "Inst #: " << Count++ << "\n    " << I;
          llvm::outs() << "    Mem Behavior: " << I.getMemoryBehavior() << "\n";
          llvm::outs() << "    Release Behavior: " << I.getReleasingBehavior()
                       << "\n";
        }
      }
    }
  }

};

} // end anonymous namespace

SILTransform *nextcode::createBasicInstructionPropertyDumper() {
  return new BasicInstructionPropertyDumper();
}
