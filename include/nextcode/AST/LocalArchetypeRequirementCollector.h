//===--- LocalArchetypeRequirementCollector.h -------------------*- C++ -*-===//
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
// This file has utility code for extending a generic signature with opened
// existentials and shape classes.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_LOCAL_ARCHETYPE_REQUIREMENT_COLLECTOR_H
#define NEXTCODE_AST_LOCAL_ARCHETYPE_REQUIREMENT_COLLECTOR_H

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/GenericSignature.h"
#include "nextcode/AST/Requirement.h"
#include "nextcode/AST/Types.h"

namespace nextcode {

struct LocalArchetypeRequirementCollector {
  const ASTContext &Context;
  GenericSignature OuterSig;
  unsigned Depth;

  /// The lists of new parameters and requirements to add to the signature.
  SmallVector<GenericTypeParamType *, 2> Params;
  SmallVector<Requirement, 2> Requirements;

  LocalArchetypeRequirementCollector(const ASTContext &ctx, GenericSignature sig);

  void addOpenedExistential(Type constraint);
  void addOpenedElement(CanGenericTypeParamType shapeClass);

  GenericTypeParamType *addParameter();
};

struct MapLocalArchetypesOutOfContext {
  GenericSignature baseGenericSig;
  ArrayRef<GenericEnvironment *> capturedEnvs;

  MapLocalArchetypesOutOfContext(GenericSignature baseGenericSig,
                                 ArrayRef<GenericEnvironment *> capturedEnvs)
      : baseGenericSig(baseGenericSig), capturedEnvs(capturedEnvs) {}

  Type operator()(SubstitutableType *type) const;
};

struct MapIntoLocalArchetypeContext {
  GenericEnvironment *baseGenericEnv;
  ArrayRef<GenericEnvironment *> capturedEnvs;

  MapIntoLocalArchetypeContext(GenericEnvironment *baseGenericEnv,
                               ArrayRef<GenericEnvironment *> capturedEnvs)
      : baseGenericEnv(baseGenericEnv), capturedEnvs(capturedEnvs) {}

  Type operator()(SubstitutableType *type) const;
};

GenericSignature buildGenericSignatureWithCapturedEnvironments(
    ASTContext &ctx,
    GenericSignature sig,
    ArrayRef<GenericEnvironment *> capturedEnvs);

SubstitutionMap buildSubstitutionMapWithCapturedEnvironments(
    SubstitutionMap baseSubMap,
    GenericSignature genericSigWithCaptures,
    ArrayRef<GenericEnvironment *> capturedEnvs);

}

#endif // NEXTCODE_AST_LOCAL_ARCHETYPE_REQUIREMENT_COLLECTOR_H