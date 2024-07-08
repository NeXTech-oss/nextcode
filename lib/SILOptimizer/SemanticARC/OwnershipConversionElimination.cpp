//===--- OwnershipConversionElimination.cpp -------------------------------===//
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

#include "SemanticARC/SemanticARCOpts.h"
#include "SemanticARCOptVisitor.h"
#include "nextcode/SIL/LinearLifetimeChecker.h"

using namespace nextcode;
using namespace semanticarc;

//===----------------------------------------------------------------------===//
//                            Top Level Entrypoint
//===----------------------------------------------------------------------===//

bool SemanticARCOptVisitor::visitUncheckedOwnershipConversionInst(
    UncheckedOwnershipConversionInst *uoci) {
  // Return false if we are supposed to only be running guaranteed opts.
  if (ctx.onlyMandatoryOpts)
    return false;

  // Then check if we are running tests and shouldn't perform this optimization
  // since we are testing something else.
  if (!ctx.shouldPerform(ARCTransformKind::OwnershipConversionElimPeephole))
    return false;

  // Otherwise, shrink our state space so that we only consider conversions from
  // owned or guaranteed to unowned. These are always legal and it is sometimes
  // convenient to insert them to avoid forwarding issues when RAUWing
  // values. So we eliminate them here.
  if (uoci->getConversionOwnershipKind() != OwnershipKind::Unowned)
    return false;

  auto op = uoci->getOperand();
  auto opKind = op->getOwnershipKind();
  if (opKind != OwnershipKind::Owned && opKind != OwnershipKind::Guaranteed)
    return false;

  // Ok, we can perform our optimization. First go through all of the uses of
  // uoci and see if they can accept our operand without any changes and that
  // they do not consume values.
  SmallVector<Operand *, 8> newUses;
  for (auto *use : uoci->getUses()) {
    if (use->isLifetimeEnding() || !use->canAcceptKind(opKind))
      return false;
    newUses.push_back(use);
  }

  // Ok, now we need to perform our lifetime check.
  SmallVector<Operand *, 8> consumingUses(op->getConsumingUses());
  LinearLifetimeChecker checker(&ctx.getDeadEndBlocks());
  if (!checker.validateLifetime(op, consumingUses, newUses))
    return false;

  // Otherwise, we can perform our rauw.
  eraseAndRAUWSingleValueInstruction(uoci, op);
  return true;
}
