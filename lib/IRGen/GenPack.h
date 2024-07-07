//===--- GenPack.h - NeXTCode IR Generation For Variadic Generics --*- C++ -*-===//
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
//  This file implements IR generation for type and value packs in NeXTCode.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_GENPACK_H
#define NEXTCODE_IRGEN_GENPACK_H

#include "IRGen.h"
#include "nextcode/AST/Types.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"

namespace llvm {

class Value;

} // end namespace llvm

namespace nextcode {

namespace irgen {
class Address;
class IRGenFunction;
class IRGenModule;
class DynamicMetadataRequest;
class MetadataResponse;
class StackAddress;

MetadataResponse
emitPackArchetypeMetadataRef(IRGenFunction &IGF,
                             CanPackArchetypeType type,
                             DynamicMetadataRequest request);

std::pair<StackAddress, llvm::Value *>
emitTypeMetadataPack(IRGenFunction &IGF, CanPackType packType,
                     DynamicMetadataRequest request);

MetadataResponse
emitTypeMetadataPackRef(IRGenFunction &IGF,
                        CanPackType packType,
                        DynamicMetadataRequest request);

/// Given a pointer to a potentially heap-allocated pack of metadata/wtables,
/// mask off the bit that indicates whether it is heap allocated.
llvm::Value *maskMetadataPackPointer(IRGenFunction &IGF, llvm::Value *);

void bindOpenedElementArchetypesAtIndex(IRGenFunction &IGF,
                                        GenericEnvironment *env,
                                        llvm::Value *index);

llvm::Value *
emitTypeMetadataPackElementRef(IRGenFunction &IGF, CanPackType packType,
                               ArrayRef<ProtocolConformanceRef> conformances,
                               llvm::Value *index,
                               DynamicMetadataRequest request,
                               llvm::SmallVectorImpl<llvm::Value *> &wtables);

void cleanupTypeMetadataPack(IRGenFunction &IGF, StackAddress pack,
                             llvm::Value *shape);

std::pair<StackAddress, llvm::Value *>
emitWitnessTablePack(IRGenFunction &IGF, CanPackType packType,
                     PackConformance *conformance);

llvm::Value *emitWitnessTablePackRef(IRGenFunction &IGF, CanPackType packType,
                                     PackConformance *conformance);

void cleanupWitnessTablePack(IRGenFunction &IGF, StackAddress pack,
                             llvm::Value *shape);

/// An on-stack pack metadata/wtable allocation.
///
/// Includes the stack address, the element count, and the kind of requirement
/// (a GenericRequirement::Kind represented as a raw uint8_t).
using StackPackAlloc =
    std::tuple<StackAddress, /*shape*/ llvm::Value *, /*kind*/ uint8_t>;

/// Emits cleanups for an array of on-stack pack metadata/wtable allocations in
/// reverse order.
void cleanupStackAllocPacks(IRGenFunction &IGF,
                            ArrayRef<StackPackAlloc> allocs);

/// Emit the dynamic index of a particular structural component
/// of the given pack type.  If the component is a pack expansion, this
/// is the index of the first element of the pack (or where it would be
/// if it had any elements).
llvm::Value *emitIndexOfStructuralPackComponent(IRGenFunction &IGF,
                                                CanPackType packType,
                                                unsigned componentIndex);

/// Emit the address that stores the given pack element.
///
/// For indirect packs, note that this is the address of the pack
/// array element, not the address stored in the pack array element.
Address emitStorageAddressOfPackElement(IRGenFunction &IGF, Address pack,
                                        llvm::Value *index, SILType elementType,
                                        CanSILPackType packType);

Size getPackElementSize(IRGenModule &, CanSILPackType ty);

StackAddress allocatePack(IRGenFunction &IGF, CanSILPackType packType);

void deallocatePack(IRGenFunction &IGF, StackAddress addr, CanSILPackType packType);

std::optional<StackAddress>
emitDynamicTupleTypeLabels(IRGenFunction &IGF, CanTupleType tupleType,
                           CanPackType packType, llvm::Value *shapeExpression);

StackAddress
emitDynamicFunctionParameterFlags(IRGenFunction &IGF,
                                  AnyFunctionType::CanParamArrayRef params,
                                  CanPackType packType,
                                  llvm::Value *shapeExpression);

std::pair<StackAddress, llvm::Value *>
emitInducedTupleTypeMetadataPack(
    IRGenFunction &IGF, llvm::Value *tupleMetadata);

MetadataResponse
emitInducedTupleTypeMetadataPackRef(
    IRGenFunction &IGF, CanPackType packType,
    llvm::Value *tupleMetadata);

} // end namespace irgen
} // end namespace nextcode

#endif
