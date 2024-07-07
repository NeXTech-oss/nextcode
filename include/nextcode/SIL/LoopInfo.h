//===--- LoopInfo.h - SIL Loop Analysis -------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_SIL_LOOPINFO_H
#define NEXTCODE_SIL_LOOPINFO_H

#include "nextcode/SIL/CFG.h"
#include "nextcode/SIL/SILBasicBlock.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/ADT/iterator_range.h"

namespace nextcode {
  class DominanceInfo;
  class SILLoop;
  class SILPassManager;
}

// Implementation in LoopInfoImpl.h
#ifdef __GNUC__
__extension__ extern template
class llvm::LoopBase<nextcode::SILBasicBlock, nextcode::SILLoop>;
__extension__ extern template
class llvm::LoopInfoBase<nextcode::SILBasicBlock, nextcode::SILLoop>;
#endif

namespace nextcode {

class SILLoop;

/// Information about a single natural loop.
class SILLoop : public llvm::LoopBase<SILBasicBlock, SILLoop> {
public:
  SILLoop() {}
  void dump() const;

  iterator_range<iterator> getSubLoopRange() const {
    return make_range(begin(), end());
  }

  void getExitingAndLatchBlocks(
    SmallVectorImpl<SILBasicBlock *> &ExitingAndLatchBlocks) const {
    this->getExitingBlocks(ExitingAndLatchBlocks);
    SILBasicBlock *header = getHeader();
    for (auto *predBB : header->getPredecessorBlocks()) {
      if (contains(predBB) && !this->isLoopExiting(predBB))
        ExitingAndLatchBlocks.push_back(predBB);
    }
  }

  SILFunction *getFunction() const { return getHeader()->getParent(); }

private:
  friend class llvm::LoopInfoBase<SILBasicBlock, SILLoop>;

  explicit SILLoop(SILBasicBlock *BB)
    : llvm::LoopBase<SILBasicBlock, SILLoop>(BB) {}
};

/// Information about loops in a function.
class SILLoopInfo {
  friend class llvm::LoopBase<SILBasicBlock, SILLoop>;
  using SILLoopInfoBase = llvm::LoopInfoBase<SILBasicBlock, SILLoop>;

  SILLoopInfoBase LI;
  DominanceInfo *Dominance;

  void operator=(const SILLoopInfo &) = delete;
  SILLoopInfo(const SILLoopInfo &) = delete;

public:
  SILLoopInfo(SILFunction *F, DominanceInfo *DT);

  SILLoopInfoBase &getBase() { return LI; }

  /// Verify loop information. This is very expensive.
  void verify() const;

  /// The iterator interface to the top-level loops in the current
  /// function.
  using iterator = SILLoopInfoBase::iterator;
  iterator begin() const { return LI.begin(); }
  iterator end() const { return LI.end(); }
  bool empty() const { return LI.empty(); }
  iterator_range<iterator> getTopLevelLoops() const {
    return make_range(begin(), end());
  }

  /// Return the inner most loop that BB lives in.  If a basic block is in no
  /// loop (for example the entry node), null is returned.
  SILLoop *getLoopFor(const SILBasicBlock *BB) const {
    return LI.getLoopFor(BB);
  }

  /// Return the inner most loop that BB lives in.  If a basic block is in no
  /// loop (for example the entry node), null is returned.
  const SILLoop *operator[](const SILBasicBlock *BB) const {
    return LI.getLoopFor(BB);
  }

  /// Return the loop nesting level of the specified block.
  unsigned getLoopDepth(const SILBasicBlock *BB) const {
    return LI.getLoopDepth(BB);
  }

  /// True if the block is a loop header node.
  bool isLoopHeader(SILBasicBlock *BB) const {
    return LI.isLoopHeader(BB);
  }

  /// This removes the specified top-level loop from this loop info object. The
  /// loop is not deleted, as it will presumably be inserted into another loop.
  SILLoop *removeLoop(iterator I) { return LI.removeLoop(I); }

  /// Change the top-level loop that contains BB to the specified loop.  This
  /// should be used by transformations that restructure the loop hierarchy
  /// tree.
  void changeLoopFor(SILBasicBlock *BB, SILLoop *L) { LI.changeLoopFor(BB, L); }

  /// Replace the specified loop in the top-level loops list with the indicated
  /// loop.
  void changeTopLevelLoop(SILLoop *OldLoop, SILLoop *NewLoop) {
    LI.changeTopLevelLoop(OldLoop, NewLoop);
  }

  ///  This adds the specified loop to the collection of top-level loops.
  void addTopLevelLoop(SILLoop *New) {
    LI.addTopLevelLoop(New);
  }

  /// This method completely removes BB from all data structures, including all
  /// of the Loop objects it is nested in and our mapping from SILBasicBlocks to
  /// loops.
  void removeBlock(SILBasicBlock *BB) {
    LI.removeBlock(BB);
  }
};

} // end namespace nextcode

#endif
