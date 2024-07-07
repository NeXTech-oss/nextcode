//===--- GenExistential.h - IR generation for existentials ------*- C++ -*-===//
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
//  This file provides the private interface to the existential emission code.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_GENEXISTENTIAL_H
#define NEXTCODE_IRGEN_GENEXISTENTIAL_H

#include "Address.h"
#include "nextcode/AST/Types.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/SIL/SILInstruction.h"

namespace llvm {
  class Value;
}

namespace nextcode {
  class ProtocolConformanceRef;
  class SILType;

namespace irgen {
  class Address;
  class Explosion;
  class IRGenFunction;

  /// Emit the metadata and witness table initialization for an allocated
  /// opaque existential container.
  Address emitOpaqueExistentialContainerInit(IRGenFunction &IGF,
                                   Address dest,
                                   SILType destType,
                                   CanType formalSrcType,
                                   SILType loweredSrcType,
                                 ArrayRef<ProtocolConformanceRef> conformances);

  /// Emit an existential metatype container from a metatype value
  /// as an explosion.
  void emitExistentialMetatypeContainer(IRGenFunction &IGF,
                                        Explosion &out,
                                        SILType outType,
                                        llvm::Value *metatype,
                                        SILType metatypeType,
                                 ArrayRef<ProtocolConformanceRef> conformances);
  
  
  /// Emit a class existential container from a class instance value
  /// as an explosion.
  void emitClassExistentialContainer(IRGenFunction &IGF,
                                 Explosion &out,
                                 SILType outType,
                                 llvm::Value *instance,
                                 CanType instanceFormalType,
                                 SILType instanceLoweredType,
                                 ArrayRef<ProtocolConformanceRef> conformances);

  /// Allocate a boxed existential container with uninitialized space to hold a
  /// value of a given type.
  OwnedAddress emitBoxedExistentialContainerAllocation(IRGenFunction &IGF,
                                  SILType destType,
                                  CanType formalSrcType,
                                  ArrayRef<ProtocolConformanceRef> conformances,
                                  GenericSignature sig);
  
  /// Deallocate a boxed existential container with uninitialized space to hold
  /// a value of a given type.
  void emitBoxedExistentialContainerDeallocation(IRGenFunction &IGF,
                                                 Explosion &container,
                                                 SILType containerType,
                                                 CanType valueType);

  /// Allocate the storage for an opaque existential in the existential
  /// container.
  /// If the value is not inline, this will allocate a box for the value and
  /// store the reference to the box in the existential container's buffer.
  Address emitAllocateBoxedOpaqueExistentialBuffer(IRGenFunction &IGF,
                                                   SILType destType,
                                                   SILType valueType,
                                                   Address existentialContainer,
                                                   GenericEnvironment *genEnv,
                                                   bool isOutlined);
  /// Deallocate the storage for an opaque existential in the existential
  /// container.
  /// If the value is not stored inline, this will deallocate the box for the
  /// value.
  void emitDeallocateBoxedOpaqueExistentialBuffer(IRGenFunction &IGF,
                                                  SILType existentialType,
                                                  Address existentialContainer);

  
   /// Free the storage for an opaque existential in the existential
   /// container.
   /// If the value is not stored inline, this will free the box for the
   /// value.
   void emitDestroyBoxedOpaqueExistentialBuffer(IRGenFunction &IGF,
                                                SILType existentialType,
                                                Address existentialContainer);

  Address emitOpaqueBoxedExistentialProjection(
      IRGenFunction &IGF, OpenedExistentialAccess accessKind, Address base,
      SILType existentialType, CanArchetypeType openedArchetype,
      GenericSignature fnSig);

  /// Return the address of the reference values within a class existential.
  Address emitClassExistentialValueAddress(IRGenFunction &IGF,
                                           Address existential,
                                           SILType baseTy);

  /// Extract the instance pointer from a class existential value.
  ///
  /// \param openedArchetype If non-null, the archetype that will capture the
  /// metadata and witness tables produced by projecting the archetype.
  llvm::Value *emitClassExistentialProjection(IRGenFunction &IGF,
                                              Explosion &base,
                                              SILType baseTy,
                                              CanArchetypeType openedArchetype,
                                              GenericSignature sigFn);

  /// Extract the metatype pointer from an existential metatype value.
  ///
  /// \param openedTy If non-null, a metatype of the archetype that
  ///   will capture the metadata and witness tables
  llvm::Value *emitExistentialMetatypeProjection(IRGenFunction &IGF,
                                                 Explosion &base,
                                                 SILType baseTy,
                                                 CanType openedTy);

  /// Project the address of the value inside a boxed existential container.
  ContainedAddress emitBoxedExistentialProjection(IRGenFunction &IGF,
                                                  Explosion &base,
                                                  SILType baseTy,
                                                  CanType projectedType);

  /// Project the address of the value inside a boxed existential container,
  /// and open an archetype to its contained type.
  Address emitOpenExistentialBox(IRGenFunction &IGF,
                                 Explosion &base,
                                 SILType baseTy,
                                 CanArchetypeType openedArchetype);

  /// Emit the existential metatype of an opaque existential value.
  void emitMetatypeOfOpaqueExistential(IRGenFunction &IGF, Address addr,
                                       SILType type, Explosion &out);
  
  /// Emit the existential metatype of a class existential value.
  void emitMetatypeOfClassExistential(IRGenFunction &IGF,
                                      Explosion &value, SILType metatypeType,
                                      SILType existentialType,
                                      GenericSignature fnSig,
                                      Explosion &out);

  /// Emit the existential metatype of a boxed existential value.
  void emitMetatypeOfBoxedExistential(IRGenFunction &IGF, Explosion &value,
                                      SILType type, Explosion &out);

  /// Emit the existential metatype of a metatype.
  void emitMetatypeOfMetatype(IRGenFunction &IGF, Explosion &value,
                              SILType existentialType, Explosion &out);

} // end namespace irgen
} // end namespace nextcode

#endif
