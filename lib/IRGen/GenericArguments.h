//===--- MetadataRequest.cpp - IR generation for metadata requests --------===//
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
//  This file implements IR generation for accessing metadata.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_GENERICARGUMENTS_H
#define NEXTCODE_IRGEN_GENERICARGUMENTS_H

#include "Explosion.h"
#include "FixedTypeInfo.h"
#include "GenArchetype.h"
#include "GenClass.h"
#include "GenMeta.h"
#include "GenProto.h"
#include "GenType.h"
#include "GenericRequirement.h"
#include "IRGenDebugInfo.h"
#include "IRGenFunction.h"
#include "IRGenMangler.h"
#include "IRGenModule.h"
#include "MetadataRequest.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/ExistentialLayout.h"
#include "nextcode/AST/GenericEnvironment.h"
#include "nextcode/AST/IRGenOptions.h"
#include "nextcode/AST/SubstitutionMap.h"
#include "nextcode/ClangImporter/ClangModule.h"
#include "nextcode/IRGen/Linking.h"
#include "llvm/ADT/STLExtras.h"

namespace nextcode {
namespace irgen {

/// A structure for collecting generic arguments for emitting a
/// nominal metadata reference.  The structure produced here is
/// consumed by nextcode_getGenericMetadata() and must correspond to
/// the fill operations that the compiler emits for the bound decl.
struct GenericArguments {
  /// The values to use to initialize the arguments structure.
  SmallVector<llvm::Value *, 8> Values;
  SmallVector<llvm::Type *, 8> Types;

 void collectTypes(IRGenModule &IGM, NominalTypeDecl *nominal) {
    GenericTypeRequirements requirements(IGM, nominal);
    collectTypes(IGM, requirements);
  }

  void collectTypes(IRGenModule &IGM,
                    const GenericTypeRequirements &requirements) {
    for (auto &requirement : requirements.getRequirements()) {
      Types.push_back(requirement.getType(IGM));
    }
  }

  void collect(IRGenFunction &IGF, CanType type) {
    auto decl = type.getNominalOrBoundGenericNominal();
    auto subs = type->getContextSubstitutionMap(IGF.IGM.getNeXTCodeModule(), decl);
    collect(IGF, subs);
  }

  void collect(IRGenFunction &IGF, SubstitutionMap subs) {
    GenericTypeRequirements requirements(IGF.IGM, subs.getGenericSignature());

    for (auto requirement : requirements.getRequirements()) {
      Values.push_back(emitGenericRequirementFromSubstitutions(
          IGF, requirement, MetadataState::Abstract, subs));
    }

    collectTypes(IGF.IGM, requirements);
    assert(Types.size() == Values.size());
  }
};

} // namespace irgen
} // namespace nextcode

#endif
