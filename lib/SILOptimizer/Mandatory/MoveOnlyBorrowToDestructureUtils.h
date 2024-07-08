//===--- MoveOnlyBorrowToDestructure.h ------------------------------------===//
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

#ifndef NEXTCODE_SILOPTIMIZER_MANDATORY_MOVEONLYBORROWTODESTRUCTURE_H
#define NEXTCODE_SILOPTIMIZER_MANDATORY_MOVEONLYBORROWTODESTRUCTURE_H

#include "nextcode/Basic/FrozenMultiMap.h"
#include "nextcode/SIL/FieldSensitivePrunedLiveness.h"
#include "nextcode/SIL/StackList.h"
#include "nextcode/SILOptimizer/Analysis/PostOrderAnalysis.h"

#include "llvm/ADT/IntervalMap.h"

namespace nextcode {
namespace siloptimizer {

class DiagnosticEmitter;

namespace borrowtodestructure {

class IntervalMapAllocator {
public:
  using Map = llvm::IntervalMap<
      unsigned, SILValue,
      llvm::IntervalMapImpl::NodeSizer<unsigned, SILValue>::LeafSize,
      llvm::IntervalMapHalfOpenInfo<unsigned>>;

  using Allocator = Map::Allocator;

private:
  /// Lazily initialized allocator.
  std::optional<Allocator> allocator;

public:
  Allocator &get() {
    if (!allocator)
      allocator.emplace();
    return *allocator;
  }
};

struct Implementation;

} // namespace borrowtodestructure

class BorrowToDestructureTransform {
  friend borrowtodestructure::Implementation;

  using IntervalMapAllocator = borrowtodestructure::IntervalMapAllocator;

  IntervalMapAllocator &allocator;
  MarkUnresolvedNonCopyableValueInst *mmci;
  SILValue rootValue;
  DiagnosticEmitter &diagnosticEmitter;
  PostOrderAnalysis *poa;
  PostOrderFunctionInfo *pofi = nullptr;
  SmallVector<SILInstruction *, 8> createdDestructures;
  SmallVector<SILPhiArgument *, 8> createdPhiArguments;

public:
  BorrowToDestructureTransform(IntervalMapAllocator &allocator,
                               MarkUnresolvedNonCopyableValueInst *mmci,
                               SILValue rootValue,
                               DiagnosticEmitter &diagnosticEmitter,
                               PostOrderAnalysis *poa)
      : allocator(allocator), mmci(mmci), rootValue(rootValue),
        diagnosticEmitter(diagnosticEmitter), poa(poa) {}

  bool transform();

private:
  PostOrderFunctionInfo *getPostOrderFunctionInfo() {
    if (!pofi)
      pofi = poa->get(mmci->getFunction());
    return pofi;
  }
};

} // namespace siloptimizer
} // namespace nextcode

#endif
