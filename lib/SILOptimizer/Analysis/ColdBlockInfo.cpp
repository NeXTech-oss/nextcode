//===--- ColdBlockInfo.cpp - Fast/slow path analysis for the SIL CFG ------===//
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

#include "nextcode/SILOptimizer/Analysis/ColdBlockInfo.h"
#include "nextcode/SILOptimizer/Analysis/DominanceAnalysis.h"
#include "nextcode/SIL/SILArgument.h"
#include "nextcode/AST/SemanticAttrs.h"

using namespace nextcode;

/// Peek through an extract of Bool.value.
static SILValue getCondition(SILValue C) {
  if (auto *SEI = dyn_cast<StructExtractInst>(C)) {
    if (auto *Struct = dyn_cast<StructInst>(SEI->getOperand()))
      return Struct->getFieldValue(SEI->getField());
    return SEI->getOperand();
  }
  return C;
}

/// \return a BranchHint if this call is a builtin branch hint.
ColdBlockInfo::BranchHint ColdBlockInfo::getBranchHint(SILValue Cond,
                                                       int recursionDepth) {
  // Handle the fully inlined Builtin.
  if (auto *BI = dyn_cast<BuiltinInst>(Cond)) {
    if (BI->getIntrinsicInfo().ID == llvm::Intrinsic::expect) {
      // peek through an extract of Bool.value.
      SILValue ExpectedValue = getCondition(BI->getArguments()[1]);
      if (auto *Literal = dyn_cast<IntegerLiteralInst>(ExpectedValue)) {
        return (Literal->getValue() == 0)
          ? BranchHint::LikelyFalse : BranchHint::LikelyTrue;
      }
    }
    return BranchHint::None;
  }

  if (auto *Arg = dyn_cast<SILArgument>(Cond)) {
    llvm::SmallVector<std::pair<SILBasicBlock *, SILValue>, 4> InValues;
    if (!Arg->getIncomingPhiValues(InValues))
      return BranchHint::None;

    if (recursionDepth > RecursionDepthLimit)
      return BranchHint::None;

    BranchHint Hint = BranchHint::None;

    // Check all predecessor values which come from non-cold blocks.
    for (auto Pair : InValues) {
      if (isCold(Pair.first, recursionDepth + 1))
        continue;

      auto *IL = dyn_cast<IntegerLiteralInst>(Pair.second);
      if (!IL)
        return BranchHint::None;
      // Check if we have a consistent value for all non-cold predecessors.
      if (IL->getValue().getBoolValue()) {
        if (Hint == BranchHint::LikelyFalse)
          return BranchHint::None;
        Hint = BranchHint::LikelyTrue;
      } else {
        if (Hint == BranchHint::LikelyTrue)
          return BranchHint::None;
        Hint = BranchHint::LikelyFalse;
      }
    }
    return Hint;
  }
  
  // Handle the @semantic functions used for branch hints.
  auto AI = dyn_cast<ApplyInst>(Cond);
  if (!AI)
    return BranchHint::None;

  if (auto *F = AI->getReferencedFunctionOrNull()) {
    if (F->hasSemanticsAttrs()) {
      // fastpath/slowpath attrs are untested because the inliner luckily
      // inlines them before the downstream calls.
      if (F->hasSemanticsAttr(semantics::SLOWPATH))
        return BranchHint::LikelyFalse;
      else if (F->hasSemanticsAttr(semantics::FASTPATH))
        return BranchHint::LikelyTrue;
    }
  }
  return BranchHint::None;
}

/// \return true if the CFG edge FromBB->ToBB is directly gated by a _slowPath
/// branch hint.
bool ColdBlockInfo::isSlowPath(const SILBasicBlock *FromBB,
                               const SILBasicBlock *ToBB,
                               int recursionDepth) {
  auto *CBI = dyn_cast<CondBranchInst>(FromBB->getTerminator());
  if (!CBI)
    return false;

  SILValue C = getCondition(CBI->getCondition());

  BranchHint hint = getBranchHint(C, recursionDepth);
  if (hint == BranchHint::None)
    return false;

  const SILBasicBlock *ColdTarget =
    (hint == BranchHint::LikelyTrue) ? CBI->getFalseBB() : CBI->getTrueBB();

  return ToBB == ColdTarget;
}

/// \return true if the given block is dominated by a _slowPath branch hint.
///
/// Cache all blocks visited to avoid introducing quadratic behavior.
bool ColdBlockInfo::isCold(const SILBasicBlock *BB, int recursionDepth) {
  auto I = ColdBlockMap.find(BB);
  if (I != ColdBlockMap.end())
    return I->second;

  typedef llvm::DomTreeNodeBase<SILBasicBlock> DomTreeNode;
  DominanceInfo *DT = DA->get(const_cast<SILFunction*>(BB->getParent()));
  DomTreeNode *Node = DT->getNode(const_cast<SILBasicBlock*>(BB));
  // Always consider unreachable code cold.
  if (!Node)
    return true;

  std::vector<const SILBasicBlock*> DomChain;
  DomChain.push_back(BB);
  bool IsCold = false;
  Node = Node->getIDom();
  while (Node) {
    if (isSlowPath(Node->getBlock(), DomChain.back(), recursionDepth)) {
      IsCold = true;
      break;
    }
    auto I = ColdBlockMap.find(Node->getBlock());
    if (I != ColdBlockMap.end()) {
      IsCold = I->second;
      break;
    }
    DomChain.push_back(Node->getBlock());
    Node = Node->getIDom();
  }
  for (auto *ChainBB : DomChain)
    ColdBlockMap[ChainBB] = IsCold;
  return IsCold;
}
