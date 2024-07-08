//===--- IVInfoPrinter.cpp - Print SIL IV Info ----------------------------===//
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

#include "nextcode/SILOptimizer/Analysis/IVAnalysis.h"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"

using namespace nextcode;

namespace {

class IVInfoPrinter : public SILModuleTransform {

  void dumpIV(ValueBase *Header, ValueBase *IV) {
    if (IV == Header) {
      llvm::errs() << "IV Header: ";
      IV->dump();
      return;
    }

    llvm::errs() << "IV: ";
    IV->dump();
    llvm::errs() << "with header: ";
    Header->dump();
  }

  /// The entry point to the transformation.
  void run() override {
    auto *IV = PM->getAnalysis<IVAnalysis>();

    for (auto &F : *getModule()) {
      if (F.isExternalDeclaration()) continue;

      auto &Info = *IV->get(&F);

      bool FoundIV = false;

      for (auto &BB : F) {
        for (auto A : BB.getArguments())
          if (Info.isInductionVariable(A)) {
            if (!FoundIV)
              llvm::errs() << "Induction variables for function: " <<
              F.getName() << "\n";

            FoundIV = true;
            dumpIV(Info.getInductionVariableHeader(A), A);
          }

        for (auto &I : BB) {
          auto value = dyn_cast<SingleValueInstruction>(&I);
          if (!value) continue;
          if (!Info.isInductionVariable(value)) continue;
          if (!FoundIV)
            llvm::errs() << "Induction variables for function: " <<
            F.getName() << "\n";

          FoundIV = true;
          dumpIV(Info.getInductionVariableHeader(value), value);
        }
      }
      
      if (FoundIV)
        llvm::errs() << "\n";
    }
  }
};

} // end anonymous namespace

SILTransform *nextcode::createIVInfoPrinter() {
  return new IVInfoPrinter();
}