//===--- LoopAnalysis.cpp - SIL Loop Analysis -----------------------------===//
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

#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/Dominance.h"
#include "nextcode/SILOptimizer/Analysis/DominanceAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/LoopAnalysis.h"
#include "nextcode/SILOptimizer/PassManager/PassManager.h"
#include "llvm/Support/Debug.h"

using namespace nextcode;

std::unique_ptr<SILLoopInfo>
SILLoopAnalysis::newFunctionAnalysis(SILFunction *F) {
  assert(DA != nullptr && "Expect a valid dominance analysis");
  DominanceInfo *DT = DA->get(F);
  assert(DT != nullptr && "Expect a valid dominance information");
  return std::make_unique<SILLoopInfo>(F, DT);
}

void SILLoopAnalysis::initialize(SILPassManager *PM) {
  DA = PM->getAnalysis<DominanceAnalysis>();
}

SILAnalysis *nextcode::createLoopAnalysis(SILModule *M) {
  return new SILLoopAnalysis(M);
}
