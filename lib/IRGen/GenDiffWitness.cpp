//===--- GenDiffWitness.cpp - IRGen for differentiability witnesses -------===//
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
//
// This file implements IR generation for SIL differentiability witnesses.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/PrettyStackTrace.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILDifferentiabilityWitness.h"

#include "ConstantBuilder.h"
#include "IRGenModule.h"

using namespace nextcode;
using namespace irgen;

void IRGenModule::emitSILDifferentiabilityWitness(
    SILDifferentiabilityWitness *dw) {
  PrettyStackTraceDifferentiabilityWitness _st(
      "emitting differentiability witness for", dw->getKey());
  // Don't emit declarations.
  if (dw->isDeclaration())
    return;
  // Don't emit `public_external` witnesses.
  if (dw->getLinkage() == SILLinkage::PublicExternal)
    return;
  ConstantInitBuilder builder(*this);
  auto diffWitnessContents = builder.beginStruct();
  assert(dw->getJVP() &&
         "Differentiability witness definition should have JVP");
  assert(dw->getVJP() &&
         "Differentiability witness definition should have VJP");
  diffWitnessContents.addBitCast(
      getAddrOfSILFunction(dw->getJVP(), NotForDefinition), Int8PtrTy);
  diffWitnessContents.addBitCast(
      getAddrOfSILFunction(dw->getVJP(), NotForDefinition), Int8PtrTy);
  getAddrOfDifferentiabilityWitness(
      dw, diffWitnessContents.finishAndCreateFuture());
}
