//=----------- Reachability.cpp - Walking from roots to barriers. -----------=//
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
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/Assertions.h"
#include "nextcode/SILOptimizer/Analysis/Reachability.h"

using namespace nextcode;

/// Walks backwards from the specified roots to find barrier instructions, phis,
/// and blocks via the isBarrier predicate.
///
/// Implements IterativeBackwardReachability::Effects
/// Implements IterativeBackwardReachability::findBarriers::Visitor
class FindBarriersBackwardDataflow final {
  using Reachability =
      IterativeBackwardReachability<FindBarriersBackwardDataflow>;
  using Effect = Reachability::Effect;
  ArrayRef<SILInstruction *> const roots;
  function_ref<bool(SILInstruction *)> isBarrier;
  ReachableBarriers &barriers;
  Reachability::Result result;
  Reachability reachability;

public:
  FindBarriersBackwardDataflow(SILFunction &function,
                               ArrayRef<SILInstruction *> roots,
                               ArrayRef<SILBasicBlock *> stopBlocks,
                               ReachableBarriers &barriers,
                               function_ref<bool(SILInstruction *)> isBarrier)
      : roots(roots), isBarrier(isBarrier), barriers(barriers),
        result(&function), reachability(&function, stopBlocks, *this, result) {}
  FindBarriersBackwardDataflow(FindBarriersBackwardDataflow const &) = delete;
  FindBarriersBackwardDataflow &
  operator=(FindBarriersBackwardDataflow const &) = delete;

  void run();

private:
  friend Reachability;

  /// IterativeBackwardReachability::Effects

  auto gens() { return roots; }

  Effect effectForInstruction(SILInstruction *);
  Effect effectForPhi(SILBasicBlock *);

  auto localGens() { return result.localGens; }

  bool isLocalGen(SILInstruction *instruction) {
    return result.localGens.contains(instruction);
  }

  /// IterativeBackwardReachability::findBarriers::Visitor

  void visitBarrierInstruction(SILInstruction *instruction) {
    barriers.instructions.push_back(instruction);
  }

  void visitBarrierPhi(SILBasicBlock *block) { barriers.phis.push_back(block); }

  void visitBarrierBlock(SILBasicBlock *block) {
    barriers.edges.push_back(block);
  }

  void visitInitialBlock(SILBasicBlock *block) {
    barriers.initialBlocks.push_back(block);
  }
};

FindBarriersBackwardDataflow::Effect
FindBarriersBackwardDataflow::effectForInstruction(
    SILInstruction *instruction) {
  if (llvm::is_contained(roots, instruction))
    return Effect::Gen();
  auto barrier = isBarrier(instruction);
  return barrier ? Effect::Kill() : Effect::NoEffect();
}

FindBarriersBackwardDataflow::Effect
FindBarriersBackwardDataflow::effectForPhi(SILBasicBlock *block) {
  assert(llvm::all_of(block->getArguments(),
                      [&](auto argument) { return PhiValue(argument); }));

  bool barrier =
      llvm::any_of(block->getPredecessorBlocks(), [&](auto *predecessor) {
        return isBarrier(predecessor->getTerminator());
      });
  return barrier ? Effect::Kill() : Effect::NoEffect();
}

void FindBarriersBackwardDataflow::run() {
  reachability.initialize();
  reachability.solve();
  reachability.findBarriers(*this);
}

void nextcode::findBarriersBackward(
    ArrayRef<SILInstruction *> roots, ArrayRef<SILBasicBlock *> initialBlocks,
    SILFunction &function, ReachableBarriers &barriers,
    function_ref<bool(SILInstruction *)> isBarrier) {
  FindBarriersBackwardDataflow flow(function, roots, initialBlocks, barriers,
                                    isBarrier);
  flow.run();
}