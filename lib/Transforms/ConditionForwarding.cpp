//===--- ConditionForwarding.cpp - Forwards conditions --------------------===//
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

#define DEBUG_TYPE "condbranch-forwarding"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/BasicBlockBits.h"
#include "nextcode/SIL/DebugUtils.h"
#include "nextcode/SIL/OwnershipUtils.h"
#include "nextcode/SIL/SILArgument.h"
#include "nextcode/SIL/SILBuilder.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILUndef.h"
#include "nextcode/SILOptimizer/Utils/OwnershipOptUtils.h"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"

using namespace nextcode;

//===----------------------------------------------------------------------===//
//                              Top Level Driver
//===----------------------------------------------------------------------===//

namespace {

/// Moves a condition down to a switch_enum and performs jump threading.
/// Example:
///
///   cond_br %c, bb1, bb2
/// bb1:
///   ... // instructions without relevant side-effects
///   %e1 = enum E.caseA
///   br bb3(%e1)
/// bb2:
///   ... // instructions without relevant side-effects
///   %e2 = enum E.caseB
///   br bb3(%e2)
/// bb3(%e : $Enum):
///   ...
///   ... // Any code, including control flow
///   ...
///   switch_enum %e, case E.caseA : bb4, case E.caseB : bb5
/// bb4:
///   ... // bb4 code
/// bb5:
///   ... // bb5 code
///
/// is optimized to
///
///   br bb3
/// bb3(%e : $Enum):
///   ...
///   ... // Any code, including control flow
///   ...
///   cond_br %c, bb1, bb2
/// bb1:
///   ... // instructions without relevant side-effects
///   %e1 = enum E.caseA
///   br bb4(%e1)
/// bb2:
///   ... // instructions without relevant side-effects
///   %e2 = enum E.caseB
///   br bb5(%e2)
/// bb4(%e3 : $Enum):
///   ... // bb4 code
/// bb5(%e4 : $Enum):
///   ... // bb5 code
///
/// A subsequence run of SimplifyCFG can then optimize it to:
///
///   ...
///   ... // Any code, including control flow
///   ...
///   cond_br %c, bb1, bb2
/// bb1:
///   ... // instructions without relevant side-effects
///   %e1 = enum E.caseA
///   ... // bb4 code
/// bb2:
///   ... // instructions without relevant side-effects
///   %e2 = enum E.caseB
///   ... // bb5 code
///
/// This eliminates the switch_enum. Such a pattern occurs when using
/// closed-range iteration, e.g.
///   for i in 0...n { }
///
class ConditionForwarding : public SILFunctionTransform {

public:
  ConditionForwarding() {}

private:

  bool tryOptimize(SwitchEnumInst *SEI);

  /// The entry point to the transformation.
  void run() override {
    bool Changed = false;

    SILFunction *F = getFunction();

    LLVM_DEBUG(llvm::dbgs()
               << "** ConditionForwarding on " << F->getName() << " **\n");

    for (SILBasicBlock &BB : *F) {
      if (auto *SEI = dyn_cast<SwitchEnumInst>(BB.getTerminator())) {
        Changed |= tryOptimize(SEI);
      }
    }
    if (Changed) {
      updateBorrowedFrom(getPassManager(), F);
      invalidateAnalysis(SILAnalysis::InvalidationKind::BranchesAndInstructions);
    }
  }

};

/// Returns true if all instructions of block \p BB are safe to be moved
/// across other code.
static bool hasNoRelevantSideEffects(SILBasicBlock *BB) {
  for (SILInstruction &I : *BB) {
    if (I.getMemoryBehavior() == MemoryBehavior::None)
      continue;
    if (auto *CF = dyn_cast<CondFailInst>(&I)) {
      // Allow cond_fail if the condition is "produced" by a builtin in the
      // same basic block.
      // Even if we move the whole block across other code, it's still
      // guaranteed that the cond_fail is executed before the result of the
      // builtin is used.
      auto *TEI = dyn_cast<TupleExtractInst>(CF->getOperand());
      if (!TEI)
        return false;
      auto *BI = dyn_cast<BuiltinInst>(TEI->getOperand());
      if (!BI || BI->getParent() != BB)
        return false;
      continue;
    }
    if (isa<BeginBorrowInst>(&I) || isa<EndBorrowInst>(&I)) {
      continue;
    }
    LLVM_DEBUG(llvm::dbgs() << "Bailing out, found inst with side-effects ");
    LLVM_DEBUG(I.dump());
    return false;
  }
  return true;
}

/// Try to move a condition, e.g. a whole if-then-else structure down to the
/// switch_enum instruction \p SEI. If successful, jump thread and replace
/// \p SEI with the condition.
/// Returns true if the a change was made.
bool ConditionForwarding::tryOptimize(SwitchEnumInst *SEI) {
  // The switch_enum argument (an Enum) must be a block argument at the merging
  // point of the condition's destinations.
  auto *Arg = dyn_cast<SILArgument>(lookThroughBorrowedFromDef(SEI->getOperand()));
  if (!Arg)
    return false;

  SILValue argValue = lookThroughBorrowedFromUser(Arg);

  // The switch_enum must be the only use of the Enum, except it may be used in
  // SEI's successors.
  for (Operand *ArgUse : argValue->getUses()) {
    SILInstruction *ArgUser = ArgUse->getUser();
    if (ArgUser == SEI)
      continue;

    if (ArgUser->isDebugInstruction())
      continue;

    if (ArgUser->getParent()->getSinglePredecessorBlock() == SEI->getParent()) {
      continue;
    }
    return false;
  }

  // No other values, beside the Enum, should be passed from the condition's
  // destinations to the merging block.
  SILBasicBlock *BB = Arg->getParent();
  if (BB->getNumArguments() != 1)
    return false;

  llvm::SmallVector<SILBasicBlock *, 4> PredBlocks;

  // Check if all predecessors of the merging block pass an Enum to its argument
  // and have a single predecessor - the block of the condition.
  SILBasicBlock *CommonBranchBlock = nullptr;
  for (SILBasicBlock *Pred : BB->getPredecessorBlocks()) {
    SILBasicBlock *PredPred = Pred->getSinglePredecessorBlock();
    if (!PredPred)
      return false;

    auto *BI = dyn_cast<BranchInst>(Pred->getTerminator());
    if (!BI)
      return false;

    auto *EI = dyn_cast<EnumInst>(BI->getArg(0));
    if (!EI)
      return false;

    if (CommonBranchBlock && PredPred != CommonBranchBlock)
      return false;
    CommonBranchBlock = PredPred;

    // We cannot move the block across other code if it has side-effects.
    if (!hasNoRelevantSideEffects(Pred))
      return false;
    PredBlocks.push_back(Pred);
  }
  // It's important to check this, because only if the merging block has at
  // least 2 predecessors, the predecessors don't have dominator children. This
  // means that all values in the predecessor blocks cannot be used in other
  // blocks.
  if (PredBlocks.size() < 2)
    return false;

  // This optimization works with all kind of terminators, except those which
  // have side-effects, like try_apply.
  TermInst *Condition = CommonBranchBlock->getTerminator();
  if (Condition->getMemoryBehavior() != MemoryBehavior::None)
    return false;

  // Are there any other branch block successors beside the predecessors which
  // we collected?
  if (CommonBranchBlock->getSuccessors().size() != PredBlocks.size())
    return false;

  if (getFunction()->hasOwnership()) {
    // TODO: Currently disabled because this case may need lifetime extension
    // Disabling this conservatively for now.
    assert(Condition->getNumOperands() == 1);
    BorrowedValue conditionOp(Condition->getOperand(0));
    if (conditionOp && conditionOp.isLocalScope()) {
      return false;
    }
  }
  // Now do the transformation!
  // First thing to do is to replace all uses of the Enum (= the merging block
  // argument), as this argument gets deleted.
  BasicBlockSet NeedEnumArg(BB->getParent());
  while (!argValue->use_empty()) {
    Operand *ArgUse = *argValue->use_begin();
    SILInstruction *ArgUser = ArgUse->getUser();
    if (ArgUser->isDebugInstruction()) {
      // Don't care about debug instructions. Just remove them.
      ArgUser->eraseFromParent();
      continue;
    }
    SILBasicBlock *UseBlock = ArgUser->getParent();
    if (UseBlock->getSinglePredecessorBlock() == SEI->getParent()) {
      // The Arg is used in a successor block of the switch_enum. To keep things
      // simple, we just create a new block argument and later (see below) we
      // pass the corresponding enum to the block. This argument will be deleted
      // by a subsequent SimplifyCFG.
      SILArgument *NewArg = nullptr;
      if (NeedEnumArg.insert(UseBlock)) {
        // The first Enum use in this UseBlock.
        NewArg = UseBlock->createPhiArgument(Arg->getType(),
                                             Arg->getOwnershipKind());
      } else {
        // We already inserted the Enum argument for this UseBlock.
        assert(UseBlock->getNumArguments() >= 1);
        NewArg = UseBlock->getArgument(UseBlock->getNumArguments() - 1);
      }
      ArgUse->set(NewArg);
      continue;
    }
    assert(ArgUser == SEI);
    // We delete the SEI later anyway. Just get rid of the Arg use.
    ArgUse->set(SILUndef::get(SEI->getOperand()));
  }

  // Redirect the predecessors of the condition's merging block to the
  // successors of the switch_enum.
  for (SILBasicBlock *Pred : PredBlocks) {
    auto *BI = cast<BranchInst>(Pred->getTerminator());
    auto *EI = cast<EnumInst>(BI->getArg(0));

    SILBasicBlock *SEDest = SEI->getCaseDestination(EI->getElement());
    SILBuilder B(BI);
    llvm::SmallVector<SILValue, 2> BranchArgs;
    unsigned HasEnumArg = NeedEnumArg.contains(SEDest);
    if (SEDest->getNumArguments() == 1 + HasEnumArg) {
      // The successor block has an original argument, which is the Enum's
      // payload.
      BranchArgs.push_back(EI->getOperand());
    }
    if (HasEnumArg) {
      // The successor block has a new argument (which we created above) where
      // we have to pass the Enum.
      assert(!getFunction()->hasOwnership() ||
             EI->getType().isTrivial(*getFunction()));
      BranchArgs.push_back(EI);
    }
    B.createBranch(BI->getLoc(), SEDest, BranchArgs);
    BI->eraseFromParent();
    if (EI->use_empty()) {
      assert(!HasEnumArg);
      EI->eraseFromParent();
    } else {
      // If an @owned EI has uses remaining, ownership fixup is needed.
      // 1. Create a copy_value of EI's operand and
      // use it in the branch to avoid a double-consume.
      // 2. Create a destroy_value of EI, to avoid a leak.
      if (getFunction()->hasOwnership() && EI->hasOperand() &&
          EI->getOwnershipKind() == OwnershipKind::Owned) {
        auto *term = EI->getParent()->getTerminator();
        assert(!HasEnumArg);
        auto *copy = SILBuilderWithScope(EI).createCopyValue(EI->getLoc(),
                                                             EI->getOperand());
        term->getOperandRef(0).set(copy);
        SILBuilderWithScope(term).createDestroyValue(EI->getLoc(), EI);
      }
    }
  }
  if (argValue != Arg) {
    cast<BorrowedFromInst>(argValue)->eraseFromParent();
  }

  // Final step: replace the switch_enum by the condition.
  SILBuilder B(Condition);
  B.createBranch(Condition->getLoc(), BB);
  Condition->moveBefore(SEI);
  SEI->eraseFromParent();
  BB->eraseArgument(0);
  return true;
}

} // end anonymous namespace

SILTransform *nextcode::createConditionForwarding() {
  return new ConditionForwarding();
}
