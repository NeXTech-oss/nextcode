//===--- PrettyStackTrace.cpp ---------------------------------------------===//
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

#include "nextcode/SILOptimizer/PassManager/PrettyStackTrace.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "llvm/Support/raw_ostream.h"

using namespace nextcode;

PrettyStackTraceSILFunctionTransform::PrettyStackTraceSILFunctionTransform(
  SILFunctionTransform *SFT, unsigned PassNumber):
  PrettyStackTraceSILFunction("Running SIL Function Transform",
                              SFT->getFunction()),
  SFT(SFT), PassNumber(PassNumber) {}

void PrettyStackTraceSILFunctionTransform::print(llvm::raw_ostream &out) const {
  out << "While running pass #" << PassNumber
      << " SILFunctionTransform \"" << SFT->getID()
      << "\" on SILFunction ";
  if (!SFT->getFunction()) {
    out << " <<null>>";
    return;
  }
  printFunctionInfo(out);
}

void PrettyStackTraceSILModuleTransform::print(llvm::raw_ostream &out) const {
  out << "While running pass #" << PassNumber
      << " SILModuleTransform \"" << SMT->getID() << "\".\n";
}
