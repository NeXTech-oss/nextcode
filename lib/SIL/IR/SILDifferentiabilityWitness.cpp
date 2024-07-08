//===--- SILDifferentiabilityWitness.cpp - Differentiability witnesses ----===//
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

#define DEBUG_TYPE "sil-differentiability-witness"

#include "nextcode/AST/ASTMangler.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILDifferentiabilityWitness.h"
#include "nextcode/SIL/SILModule.h"

using namespace nextcode;

SILDifferentiabilityWitness *SILDifferentiabilityWitness::createDeclaration(
    SILModule &module, SILLinkage linkage, SILFunction *originalFunction,
    DifferentiabilityKind kind, IndexSubset *parameterIndices,
    IndexSubset *resultIndices, GenericSignature derivativeGenSig,
    const DeclAttribute *attribute) {
  auto *diffWitness = new (module) SILDifferentiabilityWitness(
      module, linkage, originalFunction, kind, parameterIndices, resultIndices,
      derivativeGenSig, /*jvp*/ nullptr, /*vjp*/ nullptr,
      /*isDeclaration*/ true, /*isSerialized*/ false, attribute);
  // Register the differentiability witness in the module.
  Mangle::ASTMangler mangler;
  auto mangledKey = mangler.mangleSILDifferentiabilityWitness(
      diffWitness->getOriginalFunction()->getName(),
      diffWitness->getKind(), diffWitness->getConfig());
  assert(!module.DifferentiabilityWitnessMap.count(mangledKey) &&
         "Cannot create duplicate differentiability witness in a module");
  module.DifferentiabilityWitnessMap[mangledKey] = diffWitness;
  module.DifferentiabilityWitnessesByFunction[originalFunction->getName()]
      .push_back(diffWitness);
  module.getDifferentiabilityWitnessList().push_back(diffWitness);
  return diffWitness;
}

SILDifferentiabilityWitness *SILDifferentiabilityWitness::createDefinition(
    SILModule &module, SILLinkage linkage, SILFunction *originalFunction,
    DifferentiabilityKind kind, IndexSubset *parameterIndices,
    IndexSubset *resultIndices, GenericSignature derivativeGenSig,
    SILFunction *jvp, SILFunction *vjp, bool isSerialized,
    const DeclAttribute *attribute) {
  auto *diffWitness = new (module) SILDifferentiabilityWitness(
      module, linkage, originalFunction, kind, parameterIndices, resultIndices,
      derivativeGenSig, jvp, vjp, /*isDeclaration*/ false, isSerialized,
      attribute);
  // Register the differentiability witness in the module.
  Mangle::ASTMangler mangler;
  auto mangledKey = mangler.mangleSILDifferentiabilityWitness(
      diffWitness->getOriginalFunction()->getName(),
      diffWitness->getKind(), diffWitness->getConfig());
  assert(!module.DifferentiabilityWitnessMap.count(mangledKey) &&
         "Cannot create duplicate differentiability witness in a module");
  module.DifferentiabilityWitnessMap[mangledKey] = diffWitness;
  module.DifferentiabilityWitnessesByFunction[originalFunction->getName()]
      .push_back(diffWitness);
  module.getDifferentiabilityWitnessList().push_back(diffWitness);
  return diffWitness;
}

void SILDifferentiabilityWitness::convertToDefinition(SILFunction *jvp,
                                                      SILFunction *vjp,
                                                      bool isSerialized) {
  assert(IsDeclaration);
  IsDeclaration = false;
  JVP = jvp;
  VJP = vjp;
  IsSerialized = isSerialized;
}

SILDifferentiabilityWitnessKey SILDifferentiabilityWitness::getKey() const {
  return {getOriginalFunction()->getName(), getKind(), getConfig()};
}
