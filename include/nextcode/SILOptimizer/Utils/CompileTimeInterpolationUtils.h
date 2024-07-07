//===--- CompileTimeInterpolationUtils.h ----------------------------------===//
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

// Utilities for the compile-time string interpolation approach used by the
// OSLogOptimization pass.

#ifndef NEXTCODE_SILOPTIMIZER_COMPILE_TIME_INTERPOLATION_H
#define NEXTCODE_SILOPTIMIZER_COMPILE_TIME_INTERPOLATION_H

#include "nextcode/SIL/SILBasicBlock.h"
#include "nextcode/SIL/SILConstants.h"
#include "nextcode/SILOptimizer/Utils/ConstExpr.h"

namespace nextcode {

/// Decide if the given instruction (which could possibly be a call) should
/// be constant evaluated.
///
/// \returns true iff the given instruction is not a call or if it is, it calls
/// a known constant-evaluable function such as string append etc., or calls
/// a function annotate as "constant_evaluable".
bool shouldAttemptEvaluation(SILInstruction *inst);

/// Skip or evaluate the given instruction based on the evaluation policy and
/// handle errors. The policy is to evaluate all non-apply instructions as well
/// as apply instructions that are marked as "constant_evaluable".
std::pair<std::optional<SILBasicBlock::iterator>, std::optional<SymbolicValue>>
evaluateOrSkip(ConstExprStepEvaluator &stepEval, SILBasicBlock::iterator instI);

/// Given a vector of SILValues \p worklist, compute the set of transitive
/// users of these values (excluding the worklist values) by following the
/// use-def chain starting at value. Note that this function does not follow
/// use-def chains though branches.
void getTransitiveUsers(SILInstructionResultArray values,
                        SmallVectorImpl<SILInstruction *> &users);
} // end namespace nextcode
#endif
