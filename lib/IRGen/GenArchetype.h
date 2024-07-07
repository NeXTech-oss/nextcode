//===--- GenArchetype.h - NeXTCode IR generation for archetypes ----*- C++ -*-===//
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
//  This file provides the private interface to the archetype emission code.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_GENARCHETYPE_H
#define NEXTCODE_IRGEN_GENARCHETYPE_H

#include "nextcode/AST/Types.h"
#include "llvm/ADT/STLExtras.h"

namespace llvm {
  class Value;
}

namespace nextcode {
  class AssociatedType;
  class ProtocolDecl;
  class SILType;

namespace irgen {
  class Address;
  class IRGenFunction;
  class DynamicMetadataRequest;
  class MetadataResponse;

  using GetTypeParameterInContextFn =
    llvm::function_ref<CanType(CanType type)>;

  /// Emit a type metadata reference for an archetype.
  MetadataResponse emitArchetypeTypeMetadataRef(IRGenFunction &IGF,
                                                CanArchetypeType archetype,
                                                DynamicMetadataRequest request);

  /// Emit a witness table reference.
  llvm::Value *emitArchetypeWitnessTableRef(IRGenFunction &IGF,
                                            CanArchetypeType archetype,
                                            ProtocolDecl *protocol);

  /// Emit a metadata reference for an associated type of an archetype.
  MetadataResponse emitAssociatedTypeMetadataRef(IRGenFunction &IGF,
                                                 CanArchetypeType origin,
                                                 AssociatedType association,
                                                 DynamicMetadataRequest request);

  /// Emit a dynamic metatype lookup for the given archetype.
  llvm::Value *emitDynamicTypeOfOpaqueArchetype(IRGenFunction &IGF,
                                                Address archetypeAddr,
                                                SILType archetypeType);
  
  /// Emit a lookup for an opaque result type's metadata.
  MetadataResponse emitOpaqueTypeMetadataRef(IRGenFunction &IGF,
                                             CanOpaqueTypeArchetypeType archetype,
                                             DynamicMetadataRequest request);
  /// Emit a lookup for an opaque result type's protocol conformance.
  llvm::Value *emitOpaqueTypeWitnessTableRef(IRGenFunction &IGF,
                                             CanOpaqueTypeArchetypeType archetype,
                                             ProtocolDecl *protocol);
} // end namespace irgen
} // end namespace nextcode

#endif
