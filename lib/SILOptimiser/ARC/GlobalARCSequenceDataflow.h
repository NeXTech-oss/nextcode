//===- GlobalARCSequenceDataflow.h - ARC Sequence Flow Analysis -*- C++ -*-===//
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

#ifndef NEXTCODE_SILOPTIMIZER_PASSMANAGER_ARC_GLOBALARCSEQUENCEDATAFLOW_H
#define NEXTCODE_SILOPTIMIZER_PASSMANAGER_ARC_GLOBALARCSEQUENCEDATAFLOW_H

#include "RefCountState.h"
#include "nextcode/Basic/BlotMapVector.h"
#include "nextcode/Basic/NullablePtr.h"
#include "nextcode/SILOptimizer/Analysis/PostOrderAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/ProgramTerminationAnalysis.h"
#include "llvm/ADT/MapVector.h"
#include <optional>

namespace nextcode {

class SILFunction;
class AliasAnalysis;

} // end nextcode namespace

namespace nextcode {

/// A class that implements the ARC sequence data flow.
class ARCSequenceDataflowEvaluator {
public:
  // Forward declaration of private classes that are opaque in this header.
  class ARCBBStateInfoHandle;
  class ARCBBStateInfo;
  class ARCBBState;

private:
  /// The SILFunction that we are applying the dataflow to.
  SILFunction &F;

  /// The alias analysis that we are using for alias queries.
  AliasAnalysis *AA;

  /// The post order analysis we are using for computing post orders, reverse
  /// post orders.
  PostOrderAnalysis *POA;

  /// An analysis which computes the identity root of a SILValue(), i.e. the
  /// dominating origin SILValue of the reference count that by retaining or
  /// releasing this value one is affecting.
  RCIdentityFunctionInfo *RCIA;

  /// An analysis to get the epilogue ARC instructions. 
  EpilogueARCFunctionInfo *EAFI;

  /// The map from dataflow terminating decrements -> increment dataflow state.
  BlotMapVector<SILInstruction *, TopDownRefCountState> &DecToIncStateMap;

  /// The map from dataflow terminating increment -> decrement dataflow state.
  BlotMapVector<SILInstruction *, BottomUpRefCountState> &IncToDecStateMap;

  llvm::BumpPtrAllocator Allocator;
  ImmutablePointerSetFactory<SILInstruction *> SetFactory;

  /// Stashed BB information.
  std::unique_ptr<ARCBBStateInfo> BBStateInfo;

public:
  ARCSequenceDataflowEvaluator(
      SILFunction &F, AliasAnalysis *AA, PostOrderAnalysis *POA,
      RCIdentityFunctionInfo *RCIA, EpilogueARCFunctionInfo *EAFI,
      ProgramTerminationFunctionInfo *PTFI,
      BlotMapVector<SILInstruction *, TopDownRefCountState> &DecToIncStateMap,
      BlotMapVector<SILInstruction *, BottomUpRefCountState> &IncToDecStateMap);
  ~ARCSequenceDataflowEvaluator();

  /// Run the dataflow evaluator.
  bool run(bool FreezePostDomReleases);

  /// Clear all of the states we are tracking for the various basic blocks.
  void clear();

  SILFunction *getFunction() const { return &F; }

private:
  /// Perform the bottom up data flow.
  bool processBottomUp(bool freezePostDomReleases);

  /// Perform the top down dataflow.
  bool processTopDown();

  /// Merge in the BottomUp state of any successors of DataHandle.getBB() into
  /// DataHandle.getState().
  void mergeSuccessors(ARCBBStateInfoHandle &DataHandle);

  /// Merge in the TopDown state of any predecessors of DataHandle.getBB() into
  /// DataHandle.getState().
  void mergePredecessors(ARCBBStateInfoHandle &DataHandle);

  bool processBBBottomUp(ARCBBState &BBState,
                         bool FreezeOwnedArgEpilogueReleases);
  bool processBBTopDown(ARCBBState &BBState);
  void computePostDominatingConsumedArgMap();

  std::optional<ARCBBStateInfoHandle> getBottomUpBBState(SILBasicBlock *BB);
  std::optional<ARCBBStateInfoHandle> getTopDownBBState(SILBasicBlock *BB);

  void dumpDataflowResults();
};

} // end nextcode namespace

#endif