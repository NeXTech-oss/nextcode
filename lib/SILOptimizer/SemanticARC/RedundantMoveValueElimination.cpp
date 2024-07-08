//===--- RedundantMoveValueElimination.cpp - Delete spurious move_values --===//
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
// A move_value ends an owned lifetime and begins an owned lifetime.
//
// The new lifetime may have the same characteristics as the original lifetime
// with regards to
// - lexicality
// - escaping
//
// If it has the same characteristics, there is no reason to have two separate
// lifetimes--they are redundant.  This optimization deletes such redundant
// move_values.
//===----------------------------------------------------------------------===//

#include "SemanticARC/SemanticARCOpts.h"
#include "SemanticARCOptVisitor.h"
#include "nextcode/SIL/LinearLifetimeChecker.h"

using namespace nextcode;
using namespace semanticarc;

//===----------------------------------------------------------------------===//
//                            Top Level Entrypoint
//===----------------------------------------------------------------------===//

bool SemanticARCOptVisitor::visitMoveValueInst(MoveValueInst *mvi) {
  if (!ctx.shouldPerform(ARCTransformKind::RedundantMoveValueElim))
    return false;

  if (!isRedundantMoveValue(mvi))
    return false;

  // Both characteristics match.
  eraseAndRAUWSingleValueInstruction(mvi, mvi->getOperand());
  return true;
}
