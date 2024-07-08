//===--- MoveOnlyAddressCheckerTester.cpp ---------------------------------===//
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
#include "MoveOnlyBorrowToDestructureUtils.h"
#include "MoveOnlyDiagnostics.h"
#include "MoveOnlyObjectCheckerUtils.h"
#include "MoveOnlyUtils.h"

#include <utility>

using namespace nextcode;
using namespace nextcode::siloptimizer;

//===----------------------------------------------------------------------===//
//                         MARK: Top Level Entrypoint
//===----------------------------------------------------------------------===//

namespace {

class MoveOnlyAddressCheckerTesterPass : public SILFunctionTransform {
  void run() override {
    auto *fn = getFunction();

    // Don't rerun diagnostics on deserialized functions.
    if (getFunction()->wasDeserializedCanonical())
      return;

    assert(fn->getModule().getStage() == SILStage::Raw &&
           "Should only run on Raw SIL");
    LLVM_DEBUG(llvm::dbgs() << "===> MoveOnly Addr Checker. Visiting: "
                            << fn->getName() << '\n');
    auto *dominanceAnalysis = getAnalysis<DominanceAnalysis>();
    DominanceInfo *domTree = dominanceAnalysis->get(fn);
    auto *poa = getAnalysis<PostOrderAnalysis>();

    DiagnosticEmitter diagnosticEmitter(fn);
    llvm::SmallSetVector<MarkUnresolvedNonCopyableValueInst *, 32>
        moveIntroducersToProcess;
    searchForCandidateAddressMarkUnresolvedNonCopyableValueInsts(
        fn, getAnalysis<PostOrderAnalysis>(), moveIntroducersToProcess,
        diagnosticEmitter);

    LLVM_DEBUG(llvm::dbgs()
               << "Emitting diagnostic when checking for mark must check inst: "
               << (diagnosticEmitter.getDiagnosticCount() ? "yes" : "no")
               << '\n');

    bool madeChange = false;
    if (moveIntroducersToProcess.empty()) {
      LLVM_DEBUG(llvm::dbgs() << "No move introducers found?!\n");
    } else {
      borrowtodestructure::IntervalMapAllocator allocator;
      MoveOnlyAddressChecker checker{getFunction(), diagnosticEmitter,
                                     allocator, domTree, poa};
      madeChange = checker.completeLifetimes();
      madeChange |= checker.check(moveIntroducersToProcess);
    }

    // If we did not emit any diagnostics, emit a diagnostic if we missed any
    // copies.
    if (!diagnosticEmitter.emittedDiagnostic()) {
      emitCheckerMissedCopyOfNonCopyableTypeErrors(getFunction(),
                                                   diagnosticEmitter);
    }

    // Then cleanup any copies we left behind for either reason and emit an
    // error.
    madeChange |=
        cleanupNonCopyableCopiesAfterEmittingDiagnostic(getFunction());

    if (madeChange) {
      invalidateAnalysis(SILAnalysis::InvalidationKind::Instructions);
    }
  }
};

} // anonymous namespace

SILTransform *nextcode::createMoveOnlyAddressChecker() {
  return new MoveOnlyAddressCheckerTesterPass();
}
