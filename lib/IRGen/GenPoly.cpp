//===--- GenPoly.cpp - NeXTCode IR Generation for Polymorphism ---------------===//
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
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//
//
//  This file implements IR generation for polymorphic operations in NeXTCode.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/ASTVisitor.h"
#include "nextcode/AST/Types.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/GenericEnvironment.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SIL/SILType.h"
#include "llvm/IR/DerivedTypes.h"

#include "Explosion.h"
#include "IRGenFunction.h"
#include "IRGenModule.h"
#include "LoadableTypeInfo.h"
#include "TypeVisitor.h"
#include "GenTuple.h"
#include "GenPoly.h"
#include "GenType.h"

using namespace nextcode;
using namespace irgen;

static SILType applyPrimaryArchetypes(IRGenFunction &IGF,
                                      SILType type) {
  if (!type.hasTypeParameter()) {
    return type;
  }

  auto substType =
    IGF.IGM.getGenericEnvironment()->mapTypeIntoContext(type.getASTType())
      ->getCanonicalType();
  return SILType::getPrimitiveType(substType, type.getCategory());
}

/// Given a substituted explosion, re-emit it as an unsubstituted one.
///
/// For example, given an explosion which begins with the
/// representation of an (Int, Float), consume that and produce the
/// representation of an (Int, T).
///
/// The substitutions must carry origTy to substTy.
void irgen::reemitAsUnsubstituted(IRGenFunction &IGF,
                                  SILType expectedTy, SILType substTy,
                                  Explosion &in, Explosion &out) {
  expectedTy = applyPrimaryArchetypes(IGF, expectedTy);

  ExplosionSchema expectedSchema;
  cast<LoadableTypeInfo>(IGF.IGM.getTypeInfo(expectedTy))
    .getSchema(expectedSchema);

#ifndef NDEBUG
  auto &substTI = IGF.IGM.getTypeInfo(applyPrimaryArchetypes(IGF, substTy));
  assert(expectedSchema.size() ==
         cast<LoadableTypeInfo>(substTI).getExplosionSize());
#endif

  for (ExplosionSchema::Element &elt : expectedSchema) {
    llvm::Value *value = in.claimNext();
    assert(elt.isScalar());

    // The only type differences we expect here should be due to
    // substitution of class archetypes.
    if (value->getType() != elt.getScalarType()) {
      value = IGF.Builder.CreateBitCast(value, elt.getScalarType(),
                                        value->getName() + ".asUnsubstituted");
    }
    out.add(value);
  }
}
