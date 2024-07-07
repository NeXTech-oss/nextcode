//===--- RequirementLowering.h - Requirement inference and desugaring -----===//
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

#ifndef NEXTCODE_REQUIREMENTLOWERING_H
#define NEXTCODE_REQUIREMENTLOWERING_H

#include "nextcode/AST/Type.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/SmallVector.h"
#include <vector>
#include "Diagnostics.h"

namespace llvm {
  class raw_ostream;
}

namespace nextcode {

class AssociatedTypeDecl;
class ProtocolDecl;
class ProtocolTypeAlias;
class Requirement;

namespace rewriting {

// Entry points used by AbstractGenericSignatureRequest and
// InferredGenericSignatureRequest; see RequirementLowering.cpp for
// documentation
// comments.

void desugarRequirements(SmallVector<StructuralRequirement, 2> &result,
                         SmallVectorImpl<InverseRequirement> &inverses,
                         SmallVectorImpl<RequirementError> &errors);

void desugarRequirement(Requirement req, SourceLoc loc,
                        SmallVectorImpl<Requirement> &result,
                        SmallVectorImpl<InverseRequirement> &inverses,
                        SmallVectorImpl<RequirementError> &errors);

void inferRequirements(Type type, ModuleDecl *module, DeclContext *dc,
                       SmallVectorImpl<StructuralRequirement> &result);

void realizeRequirement(DeclContext *dc,
                        Requirement req, RequirementRepr *reqRepr,
                        bool shouldInferRequirements,
                        SmallVectorImpl<StructuralRequirement> &result,
                        SmallVectorImpl<RequirementError> &errors);

void realizeInheritedRequirements(TypeDecl *decl, Type type,
                                  bool shouldInferRequirements,
                                  SmallVectorImpl<StructuralRequirement> &result,
                                  SmallVectorImpl<RequirementError> &errors);

void applyInverses(ASTContext &ctx,
                   ArrayRef<Type> gps,
                   ArrayRef<InverseRequirement> inverseList,
                   SmallVectorImpl<StructuralRequirement> &result,
                   SmallVectorImpl<RequirementError> &errors);

// Defined in ConcreteContraction.cpp.
bool performConcreteContraction(
    ArrayRef<StructuralRequirement> requirements,
    SmallVectorImpl<StructuralRequirement> &result,
    SmallVectorImpl<RequirementError> &errors,
    bool debug);

} // end namespace rewriting

} // end namespace nextcode

#endif
