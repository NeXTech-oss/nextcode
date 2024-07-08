//===--- DebugUtils.cpp ---------------------------------------------------===//
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

#include "nextcode/SIL/DebugUtils.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/STLExtras.h"
#include "nextcode/SIL/SILArgument.h"
#include "nextcode/SIL/SILInstruction.h"
#include "llvm/ADT/PointerIntPair.h"
#include "llvm/ADT/SmallPtrSet.h"

using namespace nextcode;

bool nextcode::hasNonTrivialNonDebugTransitiveUsers(
    PointerUnion<SILInstruction *, SILArgument *> V) {
  llvm::SmallVector<SILInstruction *, 8> Worklist;
  llvm::SmallPtrSet<SILInstruction *, 8> SeenInsts;

  // Initialize our worklist.
  if (auto *A = V.dyn_cast<SILArgument *>()) {
    for (Operand *Op : getNonDebugUses(SILValue(A))) {
      auto *User = Op->getUser();
      if (!SeenInsts.insert(User).second)
        continue;
      Worklist.push_back(User);
    }
  } else {
    auto *I = V.get<SILInstruction *>();
    SeenInsts.insert(I);
    Worklist.push_back(I);
  }

  while (!Worklist.empty()) {
    SILInstruction *U = Worklist.pop_back_val();
    assert(SeenInsts.count(U) &&
           "Worklist should only contain seen instructions?!");

    // If U is a terminator inst, return false.
    if (isa<TermInst>(U))
      return true;

    // If U has side effects...
    if (U->mayHaveSideEffects())
      return true;

    // Otherwise add all non-debug uses of I that we have not seen yet to the
    // worklist.
    for (SILValue Result : U->getResults()) {
      for (Operand *I : getNonDebugUses(Result)) {
        auto *User = I->getUser();
        if (!SeenInsts.insert(User).second)
          continue;
        Worklist.push_back(User);
      }
    }
  }
  return false;
}

DebugVarCarryingInst DebugVarCarryingInst::getFromValue(SILValue value) {
  if (auto *svi = dyn_cast<SingleValueInstruction>(value)) {
    if (auto result = VarDeclCarryingInst(svi)) {
      switch (result.getKind()) {
      case VarDeclCarryingInst::Kind::Invalid:
        llvm_unreachable("ShouldKind have never seen this");
      case VarDeclCarryingInst::Kind::DebugValue:
      case VarDeclCarryingInst::Kind::AllocStack:
      case VarDeclCarryingInst::Kind::AllocBox:
        return DebugVarCarryingInst(svi);
      case VarDeclCarryingInst::Kind::GlobalAddr:
      case VarDeclCarryingInst::Kind::RefElementAddr:
        return DebugVarCarryingInst();
      }
    }
  }

  if (auto *use = getSingleDebugUse(value))
    return DebugVarCarryingInst(use->getUser());

  return DebugVarCarryingInst();
}
