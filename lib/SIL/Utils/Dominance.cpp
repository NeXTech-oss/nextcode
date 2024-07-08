//===--- Dominance.cpp - SIL basic block dominance analysis ---------------===//
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
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILBasicBlock.h"
#include "nextcode/SIL/SILArgument.h"
#include "nextcode/SIL/Dominance.h"
#include "llvm/Support/GenericDomTreeConstruction.h"

using namespace nextcode;

template class llvm::DominatorTreeBase<SILBasicBlock, false>;
template class llvm::DominatorTreeBase<SILBasicBlock, true>;
template class llvm::DomTreeNodeBase<SILBasicBlock>;

namespace llvm {
namespace DomTreeBuilder {
template void Calculate<SILDomTree>(SILDomTree &DT);
template void Calculate<SILPostDomTree>(SILPostDomTree &DT);
} // namespace DomTreeBuilder
} // namespace llvm

/// Compute the immediate-dominators map.
DominanceInfo::DominanceInfo(SILFunction *F)
    : DominatorTreeBase() {
  assert(!F->isExternalDeclaration() &&
         "Make sure the function is a definition and not a declaration.");
  recalculate(*F);
}

DominanceInfo::~DominanceInfo() {
}

bool DominanceInfo::properlyDominates(SILInstruction *a, SILInstruction *b) {
  auto aBlock = a->getParent(), bBlock = b->getParent();

  // If the blocks are different, it's as easy as whether A's block
  // dominates B's block.
  if (aBlock != bBlock)
    return properlyDominates(a->getParent(), b->getParent());

  // Otherwise, they're in the same block, and we just need to check
  // whether B comes after A.  This is a non-strict computation.
  auto aIter = a->getIterator();
  auto bIter = b->getIterator();
  auto fIter = aBlock->begin();
  while (bIter != fIter) {
    --bIter;
    if (aIter == bIter)
      return true;
  }

  return false;
}

/// Does value A properly dominate instruction B?
bool DominanceInfo::properlyDominates(SILValue a, SILInstruction *b) {
  if (auto *Inst = a->getDefiningInstruction()) {
    return properlyDominates(Inst, b);
  }
  if (auto *Arg = dyn_cast<SILArgument>(a)) {
    return dominates(Arg->getParent(), b->getParent());
  }
  return false;
}

SILBasicBlock *DominanceInfo::getLeastCommonAncestorOfUses(SILValue value) {
  SILBasicBlock *lca = nullptr;
  for (auto *use : value->getUses()) {
    auto *block = use->getParentBlock();
    lca = lca ? findNearestCommonDominator(lca, block) : block;
  }
  return lca;
}

void DominanceInfo::verify() const {
  // Recompute.
  auto *F = getRoot()->getParent();
  DominanceInfo OtherDT(F);

  // And compare.
  if (errorOccurredOnComparison(OtherDT)) {
    llvm::errs() << "DominatorTree is not up to date!\nComputed:\n";
    print(llvm::errs());
    llvm::errs() << "\nActual:\n";
    OtherDT.print(llvm::errs());
    abort();
  }
}

/// Compute the immediate-post-dominators map.
PostDominanceInfo::PostDominanceInfo(SILFunction *F)
   : PostDominatorTreeBase() {
  assert(!F->isExternalDeclaration() &&
         "Cannot construct a post dominator tree for a declaration");
  recalculate(*F);
}

bool
PostDominanceInfo::
properlyDominates(SILInstruction *I1, SILInstruction *I2) {
  SILBasicBlock *BB1 = I1->getParent(), *BB2 = I2->getParent();

  // If the blocks are different, it's as easy as whether BB1 post dominates
  // BB2.
  if (BB1 != BB2)
    return properlyDominates(BB1, BB2);

  // Otherwise, they're in the same block, and we just need to check
  // whether A comes after B.
  for (auto II = I1->getIterator(), IE = BB1->end(); II != IE; ++II) {
    if (&*II == I2) {
      return false;
    }
  }

  return true;
}

bool PostDominanceInfo::properlyDominates(SILValue A, SILInstruction *B) {
  if (auto *Inst = A->getDefiningInstruction()) {
    return properlyDominates(Inst, B);
  }
  if (auto *Arg = dyn_cast<SILArgument>(A)) {
    return dominates(Arg->getParent(), B->getParent());
  }
  return false;
}

void PostDominanceInfo::verify() const {
  // Recompute.
  //
  // Even though at the SIL level we have "one" return function, we can have
  // multiple exits provided by no-return functions.
  auto *F = (*root_begin())->getParent();
  PostDominanceInfo OtherDT(F);

  // And compare.
  if (errorOccurredOnComparison(OtherDT)) {
    llvm::errs() << "PostDominatorTree is not up to date!\nComputed:\n";
    print(llvm::errs());
    llvm::errs() << "\nActual:\n";
    OtherDT.print(llvm::errs());
    abort();
  }
}

void nextcode::computeDominatedBoundaryBlocks(
    SILBasicBlock *root, DominanceInfo *domTree,
    SmallVectorImpl<SILBasicBlock *> &boundary) {
  assert(boundary.empty());

  DominanceOrder domOrder(root, domTree);
  while (SILBasicBlock *block = domOrder.getNext()) {
    DominanceInfoNode *domNode = domTree->getNode(block);
    if (!domNode->isLeaf()) {
      domOrder.pushChildren(block);
      continue;
    }
    if (block->getNumSuccessors() == 0) {
      boundary.push_back(block);
      continue;
    }
    auto *succ = block->getSingleSuccessorBlock();
    if (!domTree->properlyDominates(root, succ)) {
      boundary.push_back(block);
    }
  }
}