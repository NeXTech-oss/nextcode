//===--- MoveOnlyObjectCheckerTester.cpp ----------------------------------===//
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

#include "nextcode/AST/DiagnosticsSIL.h"
#include "nextcode/AST/TypeCheckRequests.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/Defer.h"
#include "nextcode/Basic/FrozenMultiMap.h"
#include "nextcode/Basic/STLExtras.h"
#include "nextcode/SIL/BasicBlockBits.h"
#include "nextcode/SIL/BasicBlockUtils.h"
#include "nextcode/SIL/DebugUtils.h"
#include "nextcode/SIL/FieldSensitivePrunedLiveness.h"
#include "nextcode/SIL/InstructionUtils.h"
#include "nextcode/SIL/NodeBits.h"
#include "nextcode/SIL/OwnershipUtils.h"
#include "nextcode/SIL/PostOrder.h"
#include "nextcode/SIL/PrunedLiveness.h"
#include "nextcode/SIL/SILArgument.h"
#include "nextcode/SIL/SILBuilder.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILLocation.h"
#include "nextcode/SIL/SILUndef.h"
#include "nextcode/SIL/SILValue.h"
#include "nextcode/SIL/StackList.h"
#include "nextcode/SILOptimizer/Analysis/ClosureScope.h"
#include "nextcode/SILOptimizer/Analysis/DeadEndBlocksAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/DominanceAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/NonLocalAccessBlockAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/PostOrderAnalysis.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "nextcode/SILOptimizer/Utils/CFGOptUtils.h"
#include "nextcode/SILOptimizer/Utils/CanonicalizeOSSALifetime.h"
#include "nextcode/SILOptimizer/Utils/InstructionDeleter.h"
#include "nextcode/SILOptimizer/Utils/SILSSAUpdater.h"
#include "clang/AST/DeclTemplate.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/IntervalMap.h"
#include "llvm/ADT/PointerIntPair.h"
#include "llvm/ADT/PointerUnion.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallBitVector.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/RecyclingAllocator.h"

#include "MoveOnlyBorrowToDestructureUtils.h"
#include "MoveOnlyDiagnostics.h"
#include "MoveOnlyObjectCheckerUtils.h"
#include "MoveOnlyUtils.h"

using namespace nextcode;
using namespace nextcode::siloptimizer;

//===----------------------------------------------------------------------===//
//                         MARK: Top Level Entrypoint
//===----------------------------------------------------------------------===//

namespace {

class MoveOnlyObjectCheckerTesterPass : public SILFunctionTransform {
  void run() override {
    auto *fn = getFunction();

    // Don't rerun diagnostics on deserialized functions.
    if (getFunction()->wasDeserializedCanonical())
      return;

    assert(fn->getModule().getStage() == SILStage::Raw &&
           "Should only run on Raw SIL");

    LLVM_DEBUG(llvm::dbgs() << "===> MoveOnly Object Checker. Visiting: "
                            << fn->getName() << '\n');

    auto *dominanceAnalysis = getAnalysis<DominanceAnalysis>();
    DominanceInfo *domTree = dominanceAnalysis->get(fn);
    auto *poa = getAnalysis<PostOrderAnalysis>();

    DiagnosticEmitter diagnosticEmitter(fn);
    borrowtodestructure::IntervalMapAllocator allocator;

    unsigned diagCount = diagnosticEmitter.getDiagnosticCount();
    llvm::SmallSetVector<MarkUnresolvedNonCopyableValueInst *, 32>
        moveIntroducersToProcess;
    bool madeChange =
        searchForCandidateObjectMarkUnresolvedNonCopyableValueInsts(
            fn, moveIntroducersToProcess, diagnosticEmitter);

    LLVM_DEBUG(llvm::dbgs()
               << "Emitting diagnostic when checking for mark must check inst: "
               << (diagCount != diagnosticEmitter.getDiagnosticCount() ? "yes"
                                                                       : "no")
               << '\n');

    if (moveIntroducersToProcess.empty()) {
      LLVM_DEBUG(llvm::dbgs()
                 << "No move introducers found?! Returning early?!\n");
    } else {
      diagCount = diagnosticEmitter.getDiagnosticCount();
      MoveOnlyObjectChecker checker{diagnosticEmitter, domTree, poa, allocator};
      madeChange |= checker.check(moveIntroducersToProcess);
    }

    if (diagCount != diagnosticEmitter.getDiagnosticCount()) {
      emitCheckerMissedCopyOfNonCopyableTypeErrors(getFunction(),
                                                   diagnosticEmitter);
    }

    madeChange |=
        cleanupNonCopyableCopiesAfterEmittingDiagnostic(getFunction());

    if (madeChange) {
      invalidateAnalysis(SILAnalysis::InvalidationKind::Instructions);
    }
  }
};

} // anonymous namespace

SILTransform *nextcode::createMoveOnlyObjectChecker() {
  return new MoveOnlyObjectCheckerTesterPass();
}
