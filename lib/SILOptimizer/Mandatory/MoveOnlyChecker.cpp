//===--- MoveOnlyChecker.cpp ----------------------------------------------===//
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

#define DEBUG_TYPE "sil-move-only-checker"

#include "nextcode/AST/AccessScope.h"
#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/AST/DiagnosticsSIL.h"
#include "nextcode/AST/SemanticAttrs.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/Debug.h"
#include "nextcode/Basic/Defer.h"
#include "nextcode/Basic/FrozenMultiMap.h"
#include "nextcode/Basic/SmallBitVector.h"
#include "nextcode/SIL/ApplySite.h"
#include "nextcode/SIL/BasicBlockBits.h"
#include "nextcode/SIL/BasicBlockData.h"
#include "nextcode/SIL/BasicBlockDatastructures.h"
#include "nextcode/SIL/BasicBlockUtils.h"
#include "nextcode/SIL/Consumption.h"
#include "nextcode/SIL/DebugUtils.h"
#include "nextcode/SIL/FieldSensitivePrunedLiveness.h"
#include "nextcode/SIL/InstructionUtils.h"
#include "nextcode/SIL/MemAccessUtils.h"
#include "nextcode/SIL/OSSALifetimeCompletion.h"
#include "nextcode/SIL/OwnershipUtils.h"
#include "nextcode/SIL/PrunedLiveness.h"
#include "nextcode/SIL/SILArgument.h"
#include "nextcode/SIL/SILArgumentConvention.h"
#include "nextcode/SIL/SILBasicBlock.h"
#include "nextcode/SIL/SILBuilder.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILUndef.h"
#include "nextcode/SIL/SILValue.h"
#include "nextcode/SILOptimizer/Analysis/ClosureScope.h"
#include "nextcode/SILOptimizer/Analysis/DeadEndBlocksAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/DominanceAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/NonLocalAccessBlockAnalysis.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "nextcode/SILOptimizer/Utils/CanonicalizeOSSALifetime.h"
#include "nextcode/SILOptimizer/Utils/InstructionDeleter.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/MapVector.h"
#include "llvm/ADT/PointerIntPair.h"
#include "llvm/ADT/PointerUnion.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallBitVector.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"

#include "MoveOnlyAddressCheckerUtils.h"
#include "MoveOnlyDiagnostics.h"
#include "MoveOnlyObjectCheckerUtils.h"
#include "MoveOnlyUtils.h"

using namespace nextcode;
using namespace nextcode::siloptimizer;

//===----------------------------------------------------------------------===//
//                     MARK: Top Level Object Entrypoint
//===----------------------------------------------------------------------===//

namespace {

struct MoveOnlyChecker {
  DiagnosticEmitter diagnosticEmitter;
  SILFunction *fn;
  DominanceInfo *domTree;
  PostOrderAnalysis *poa;
  bool madeChange = false;
  borrowtodestructure::IntervalMapAllocator allocator;

  MoveOnlyChecker(SILFunction *fn, DominanceInfo *domTree,
                  PostOrderAnalysis *poa)
      : diagnosticEmitter(fn), fn(fn), domTree(domTree), poa(poa) {
  }

  void checkObjects();
  void completeObjectLifetimes(ArrayRef<MarkUnresolvedNonCopyableValueInst *>);
  void checkAddresses();
};

} // namespace

void MoveOnlyChecker::checkObjects() {
  llvm::SmallSetVector<MarkUnresolvedNonCopyableValueInst *, 32>
      moveIntroducersToProcess;
  unsigned diagCount = diagnosticEmitter.getDiagnosticCount();
  madeChange |= searchForCandidateObjectMarkUnresolvedNonCopyableValueInsts(
      fn, moveIntroducersToProcess, diagnosticEmitter);

  LLVM_DEBUG(
      llvm::dbgs()
      << "Emitting diagnostic when checking for mark must check inst: "
      << (diagCount != diagnosticEmitter.getDiagnosticCount() ? "yes" : "no")
      << '\n');

  if (moveIntroducersToProcess.empty()) {
    LLVM_DEBUG(llvm::dbgs()
               << "No move introducers found?! Returning early?!\n");
    return;
  }

  completeObjectLifetimes(moveIntroducersToProcess.getArrayRef());

  MoveOnlyObjectChecker checker{diagnosticEmitter, domTree, poa, allocator};
  madeChange |= checker.check(moveIntroducersToProcess);
}

void MoveOnlyChecker::completeObjectLifetimes(
    ArrayRef<MarkUnresolvedNonCopyableValueInst *> insts) {
// TODO: Delete once OSSALifetimeCompletion is run as part of SILGenCleanup.
  OSSALifetimeCompletion completion(fn, domTree);

  // Collect all values derived from each mark_unresolved_non_copyable_value
  // instruction via ownership instructions and phis.
  ValueWorklist transitiveValues(fn);
  for (auto *inst : insts) {
    transitiveValues.push(inst);
  }
  while (auto value = transitiveValues.pop()) {
    for (auto *use : value->getUses()) {
      auto *user = use->getUser();
      switch (user->getKind()) {
      case SILInstructionKind::BeginBorrowInst:
      case SILInstructionKind::CopyValueInst:
      case SILInstructionKind::MoveValueInst:
        transitiveValues.pushIfNotVisited(cast<SingleValueInstruction>(user));
        break;
      case SILInstructionKind::BranchInst: {
        PhiOperand po(use);
        transitiveValues.pushIfNotVisited(po.getValue());
        break;
      }
      default: {
        auto forward = ForwardingOperation(user);
        if (!forward)
          continue;
        forward.visitForwardedValues([&transitiveValues](auto forwarded) {
          transitiveValues.pushIfNotVisited(forwarded);
          return true;
        });
        break;
      }
      }
    }
  }
  // Complete the lifetime of each collected value.  This is a subset of the
  // work that SILGenCleanup will do.
  for (auto *block : poa->get(fn)->getPostOrder()) {
    for (SILInstruction &inst : reverse(*block)) {
      for (auto result : inst.getResults()) {
        if (!transitiveValues.isVisited(result))
          continue;
        if (completion.completeOSSALifetime(
                result, OSSALifetimeCompletion::Boundary::Availability) ==
            LifetimeCompletion::WasCompleted) {
          madeChange = true;
        }
      }
    }
    for (SILArgument *arg : block->getArguments()) {
      assert(!arg->isReborrow() && "reborrows not legal at this SIL stage");
      if (!transitiveValues.isVisited(arg))
        continue;
      if (completion.completeOSSALifetime(
              arg, OSSALifetimeCompletion::Boundary::Availability) ==
          LifetimeCompletion::WasCompleted) {
        madeChange = true;
      }
    }
  }
}

void MoveOnlyChecker::checkAddresses() {
  unsigned diagCount = diagnosticEmitter.getDiagnosticCount();
  llvm::SmallSetVector<MarkUnresolvedNonCopyableValueInst *, 32>
      moveIntroducersToProcess;
  searchForCandidateAddressMarkUnresolvedNonCopyableValueInsts(
      fn, poa, moveIntroducersToProcess, diagnosticEmitter);

  LLVM_DEBUG(
      llvm::dbgs()
      << "Emitting diagnostic when checking for mark must check inst: "
      << (diagCount != diagnosticEmitter.getDiagnosticCount() ? "yes" : "no")
      << '\n');

  if (moveIntroducersToProcess.empty()) {
    LLVM_DEBUG(llvm::dbgs()
               << "No move introducers found?! Returning early?!\n");
    return;
  }

  MoveOnlyAddressChecker checker{fn, diagnosticEmitter, allocator, domTree,
                                 poa};
  madeChange |= checker.completeLifetimes();
  madeChange |= checker.check(moveIntroducersToProcess);
}

//===----------------------------------------------------------------------===//
//                         MARK: Top Level Entrypoint
//===----------------------------------------------------------------------===//

namespace {

static bool canonicalizeLoadBorrows(SILFunction *F) {
  bool changed = false;
  for (auto &block : *F) {
    for (auto &inst : block) {
      if (auto *lbi = dyn_cast<LoadBorrowInst>(&inst)) {
        if (lbi->isUnchecked()) {
          changed = true;
          lbi->setUnchecked(false);
        }
      }
    }
  }
  
  return changed;
}

class MoveOnlyCheckerPass : public SILFunctionTransform {
  void run() override {
    auto *fn = getFunction();

    // Don't rerun diagnostics on deserialized functions.
    if (getFunction()->wasDeserializedCanonical())
      return;

    assert(fn->getModule().getStage() == SILStage::Raw &&
           "Should only run on Raw SIL");

    // If an earlier pass told use to not emit diagnostics for this function,
    // clean up any copies, invalidate the analysis, and return early.
    if (fn->hasSemanticsAttr(semantics::NO_MOVEONLY_DIAGNOSTICS)) {
      bool didChange = canonicalizeLoadBorrows(fn);
      didChange |= cleanupNonCopyableCopiesAfterEmittingDiagnostic(getFunction());
      if (didChange) {
        invalidateAnalysis(SILAnalysis::InvalidationKind::Instructions);
      }
      return;
    }

    LLVM_DEBUG(llvm::dbgs()
               << "===> MoveOnly Checker. Visiting: " << fn->getName() << '\n');

    MoveOnlyChecker checker(
        fn, getAnalysis<DominanceAnalysis>()->get(fn),
        getAnalysis<PostOrderAnalysis>());

    checker.checkObjects();
    checker.checkAddresses();

    // If we did not emit any diagnostics, emit an error on any copies that
    // remain. If we emitted a diagnostic, we just want to rewrite all of the
    // non-copyable copies into explicit variants below and let the user
    // recompile.
    if (!checker.diagnosticEmitter.emittedDiagnostic()) {
      emitCheckerMissedCopyOfNonCopyableTypeErrors(fn,
                                                   checker.diagnosticEmitter);
    }

    // Remaining borrows
    // should be correctly immutable. We can canonicalize any remaining
    // `load_borrow [unchecked]` instructions.
    checker.madeChange |= canonicalizeLoadBorrows(fn);

    checker.madeChange |=
        cleanupNonCopyableCopiesAfterEmittingDiagnostic(fn);

    if (checker.madeChange)
      invalidateAnalysis(SILAnalysis::InvalidationKind::Instructions);
  }
};

} // namespace

SILTransform *nextcode::createMoveOnlyChecker() {
  return new MoveOnlyCheckerPass();
}
