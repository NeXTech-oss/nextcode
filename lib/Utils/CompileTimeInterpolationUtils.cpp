//===--- CompileTimeInterpolationUtils.cpp -------------------------------===//
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

#include "nextcode/SILOptimizer/Utils/CompileTimeInterpolationUtils.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/SIL/SILFunction.h"

using namespace nextcode;

bool nextcode::shouldAttemptEvaluation(SILInstruction *inst) {
  auto *apply = dyn_cast<ApplyInst>(inst);
  if (!apply)
    return true;
  SILFunction *calleeFun = apply->getCalleeFunction();
  if (!calleeFun)
    return false;
  return isConstantEvaluable(calleeFun);
}

std::pair<std::optional<SILBasicBlock::iterator>, std::optional<SymbolicValue>>
nextcode::evaluateOrSkip(ConstExprStepEvaluator &stepEval,
                      SILBasicBlock::iterator instI) {
  SILInstruction *inst = &(*instI);

  // Note that skipping a call conservatively approximates its effects on the
  // interpreter state.
  if (shouldAttemptEvaluation(inst)) {
    return stepEval.tryEvaluateOrElseMakeEffectsNonConstant(instI);
  }
  return stepEval.skipByMakingEffectsNonConstant(instI);
}

void nextcode::getTransitiveUsers(SILInstructionResultArray values,
                               SmallVectorImpl<SILInstruction *> &users) {
  // Collect the instructions that are data dependent on the value using a
  // fix point iteration.
  SmallPtrSet<SILInstruction *, 16> visited;
  SmallVector<SILValue, 16> worklist;
  llvm::copy(values, std::back_inserter(worklist));
  while (!worklist.empty()) {
    SILValue currVal = worklist.pop_back_val();
    for (Operand *use : currVal->getUses()) {
      SILInstruction *user = use->getUser();
      if (visited.count(user))
        continue;
      visited.insert(user);
      llvm::copy(user->getResults(), std::back_inserter(worklist));
    }
  }
  users.append(visited.begin(), visited.end());
}
