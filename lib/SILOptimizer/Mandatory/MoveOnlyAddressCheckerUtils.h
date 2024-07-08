//===--- MoveOnlyAddressCheckerUtils.h ------------------------------------===//
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

#ifndef NEXTCODE_SILOPTIMIZER_MANDATORY_MOVEONLYADDRESSCHECKERUTILS_H
#define NEXTCODE_SILOPTIMIZER_MANDATORY_MOVEONLYADDRESSCHECKERUTILS_H

#include "MoveOnlyBorrowToDestructureUtils.h"

namespace nextcode {

class PostOrderAnalysis;

namespace siloptimizer {

class DiagnosticEmitter;

/// Searches for candidate mark must checks.
///
/// NOTE: To see if we emitted a diagnostic, use \p
/// diagnosticEmitter.getDiagnosticCount().
void searchForCandidateAddressMarkUnresolvedNonCopyableValueInsts(
    SILFunction *fn, PostOrderAnalysis *poa,
    llvm::SmallSetVector<MarkUnresolvedNonCopyableValueInst *, 32>
        &moveIntroducersToProcess,
    DiagnosticEmitter &diagnosticEmitter);

struct MoveOnlyAddressChecker {
  SILFunction *fn;
  DiagnosticEmitter &diagnosticEmitter;
  borrowtodestructure::IntervalMapAllocator &allocator;
  DominanceInfo *domTree;
  PostOrderAnalysis *poa;

  /// \returns true if we changed the IR. To see if we emitted a diagnostic, use
  /// \p diagnosticEmitter.getDiagnosticCount().
  bool check(llvm::SmallSetVector<MarkUnresolvedNonCopyableValueInst *, 32>
                 &moveIntroducersToProcess);
  bool completeLifetimes();
};

} // namespace siloptimizer

} // namespace nextcode

#endif
