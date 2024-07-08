//===--- TangentBuilder.cpp - Tangent SIL builder ------------*- C++ -*----===//
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
//
//===----------------------------------------------------------------------===//
//
// This file defines a helper class for emitting tangent code for automatic
// differentiation.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "differentiation"

#include "nextcode/Basic/Assertions.h"
#include "nextcode/SILOptimizer/Differentiation/TangentBuilder.h"
#include "nextcode/SILOptimizer/Differentiation/ADContext.h"

namespace nextcode {
namespace autodiff {

void TangentBuilder::emitZeroIntoBuffer(SILLocation loc, SILValue buffer,
                                        IsInitialization_t isInit) {
  if (!isInit)
    emitDestroyAddr(loc, buffer);
  if (auto tupleType = buffer->getType().getAs<TupleType>()) {
    for (unsigned i : range(tupleType->getNumElements())) {
      auto *eltAddr = createTupleElementAddr(loc, buffer, i);
      emitZeroIntoBuffer(loc, eltAddr, IsInitialization);
    }
    return;
  }
  auto *nextcodeMod = getModule().getNeXTCodeModule();
  // Look up conformance to `AdditiveArithmetic`.
  auto *additiveArithmeticProto = adContext.getAdditiveArithmeticProtocol();
  auto astType = buffer->getType().getASTType();
  auto confRef = nextcodeMod->lookupConformance(astType, additiveArithmeticProto);
  assert(!confRef.isInvalid() && "Missing conformance to `AdditiveArithmetic`");
  SILDeclRef accessorDeclRef(adContext.getAdditiveArithmeticZeroGetter(),
                             SILDeclRef::Kind::Func);
  auto silFnType = getModule().Types.getConstantType(
      getTypeExpansionContext(), accessorDeclRef);
  // %wm = witness_method ...
  auto *getter = createWitnessMethod(
      loc, astType, confRef, accessorDeclRef, silFnType);
  // %metatype = metatype $T
  auto metatypeType = CanMetatypeType::get(astType,
                                           MetatypeRepresentation::Thick);
  auto metatype = createMetatype(
      loc, SILType::getPrimitiveObjectType(metatypeType));
  auto subMap = SubstitutionMap::getProtocolSubstitutions(
      additiveArithmeticProto, astType, confRef);
  createApply(loc, getter, subMap, {buffer, metatype});
  emitDestroyValueOperation(loc, getter);
}

SILValue TangentBuilder::emitZero(SILLocation loc, CanType type) {
  auto silType = getModule().Types.getLoweredLoadableType(
      type, TypeExpansionContext::minimal(), getModule());
  auto tempAllocLoc = RegularLocation::getAutoGeneratedLocation();
  auto *alloc = createAllocStack(tempAllocLoc, silType);
  emitZeroIntoBuffer(loc, alloc, IsInitialization);
  auto zeroValue = emitLoadValueOperation(
      loc, alloc, LoadOwnershipQualifier::Take);
  createDeallocStack(loc, alloc);
  return zeroValue;
}

void TangentBuilder::emitInPlaceAdd(
    SILLocation loc, SILValue destinationBuffer, SILValue operand) {
  assert(destinationBuffer->getType().isAddress());
  auto type = destinationBuffer->getType();
  if (auto tupleType = type.getAs<TupleType>()) {
    for (unsigned i : range(tupleType->getNumElements())) {
      auto *eltDestAddr = createTupleElementAddr(loc, destinationBuffer, i);
      switch (operand->getType().getCategory()) {
      case SILValueCategory::Address: {
        auto *eltOperand = createTupleElementAddr(loc, operand, i);
        emitInPlaceAdd(loc, eltDestAddr, eltOperand);
        break;
      }
      case SILValueCategory::Object: {
        auto borrowedOp = emitBeginBorrowOperation(loc, operand);
        auto eltOperand = emitTupleExtract(loc, borrowedOp, i);
        emitInPlaceAdd(loc, eltDestAddr, eltOperand);
        emitEndBorrowOperation(loc, borrowedOp);
        break;
      }
      }
    }
    return;
  }
  // Call the combiner function and return.
  auto *nextcodeMod = getModule().getNeXTCodeModule();
  auto astType = type.getASTType();
  auto confRef = nextcodeMod->lookupConformance(
      astType, adContext.getAdditiveArithmeticProtocol());
  assert(!confRef.isInvalid() &&
         "Missing conformance to `AdditiveArithmetic`");
  SILDeclRef declRef(adContext.getPlusEqualDecl(), SILDeclRef::Kind::Func);
  auto silFnTy = getModule().Types.getConstantType(
      getTypeExpansionContext(), declRef);
  // %0 = witness_method @+=
  auto witnessMethod =
      createWitnessMethod(loc, astType, confRef, declRef, silFnTy);
  auto subMap = SubstitutionMap::getProtocolSubstitutions(
      adContext.getAdditiveArithmeticProtocol(), astType, confRef);
  // %1 = metatype $T.Type
  auto metatypeType =
      CanMetatypeType::get(astType, MetatypeRepresentation::Thick);
  auto metatypeSILType = SILType::getPrimitiveObjectType(metatypeType);
  auto metatype = createMetatype(loc, metatypeSILType);
  // %2 = apply $0(%lhs, %rhs, %1)
  createApply(loc, witnessMethod, subMap,
              {destinationBuffer, operand, metatype});
  emitDestroyValueOperation(loc, witnessMethod);
}

void TangentBuilder::emitAddIntoBuffer(SILLocation loc,
                                       SILValue destinationBuffer,
                                       SILValue lhsAddress,
                                       SILValue rhsAddress) {
  assert(lhsAddress->getType().getASTType() ==
             rhsAddress->getType().getASTType() &&
         "Adjoint values must have same type!");
  assert(lhsAddress->getType().isAddress() &&
         rhsAddress->getType().isAddress() &&
         "Adjoint values must both have address types!");
  auto type = lhsAddress->getType();
  if (auto tupleType = type.getAs<TupleType>()) {
    for (unsigned i : range(tupleType->getNumElements())) {
      auto *destAddr = createTupleElementAddr(loc, destinationBuffer, i);
      auto *eltAddrLHS = createTupleElementAddr(loc, lhsAddress, i);
      auto *eltAddrRHS = createTupleElementAddr(loc, rhsAddress, i);
      emitAddIntoBuffer(loc, destAddr, eltAddrLHS, eltAddrRHS);
    }
    return;
  }
  auto astType = type.getASTType();
  auto *proto = adContext.getAdditiveArithmeticProtocol();
  auto *combinerFuncDecl = adContext.getPlusDecl();
  // Call the combiner function and return.
  auto *nextcodeMod = getModule().getNeXTCodeModule();
  auto confRef = nextcodeMod->lookupConformance(astType, proto);
  assert(!confRef.isInvalid() &&
         "Missing conformance to `AdditiveArithmetic`");
  SILDeclRef declRef(combinerFuncDecl, SILDeclRef::Kind::Func);
  auto silFnTy = getModule().Types.getConstantType(
      getTypeExpansionContext(), declRef);
  // %0 = witness_method @+
  auto witnessMethod =
      createWitnessMethod(loc, astType, confRef, declRef, silFnTy);
  auto subMap =
      SubstitutionMap::getProtocolSubstitutions(proto, astType, confRef);
  // %1 = metatype $T.Type
  auto metatypeType =
      CanMetatypeType::get(astType, MetatypeRepresentation::Thick);
  auto metatypeSILType = SILType::getPrimitiveObjectType(metatypeType);
  auto metatype = createMetatype(loc, metatypeSILType);
  // %2 = apply %0(%result, %new, %old, %1)
  createApply(loc, witnessMethod, subMap,
              {destinationBuffer, rhsAddress, lhsAddress, metatype});
  emitDestroyValueOperation(loc, witnessMethod);
}

SILValue TangentBuilder::emitAdd(SILLocation loc, SILValue lhs, SILValue rhs) {
  LLVM_DEBUG(getADDebugStream() << "Emitting adjoint accumulation for lhs: "
                                << lhs << " and rhs: " << rhs);
  assert(lhs->getType() == rhs->getType() && "Adjoints must have equal types!");
  assert(lhs->getType().isObject() && rhs->getType().isObject() &&
         "Adjoint types must be both object types!");
  auto type = lhs->getType();
  auto lhsCopy = emitCopyValueOperation(loc, lhs);
  auto rhsCopy = emitCopyValueOperation(loc, rhs);
  // Allocate buffers for inputs and output.
  auto tempAllocLoc = RegularLocation::getAutoGeneratedLocation();
  auto *resultBuf = createAllocStack(tempAllocLoc, type);
  auto *lhsBuf = createAllocStack(tempAllocLoc, type);
  auto *rhsBuf = createAllocStack(tempAllocLoc, type);
  // Initialize input buffers.
  emitStoreValueOperation(loc, lhsCopy, lhsBuf,
                          StoreOwnershipQualifier::Init);
  emitStoreValueOperation(loc, rhsCopy, rhsBuf,
                          StoreOwnershipQualifier::Init);
  emitAddIntoBuffer(loc, resultBuf, lhsBuf, rhsBuf);
  emitDestroyAddr(loc, lhsBuf);
  emitDestroyAddr(loc, rhsBuf);
  // Deallocate input buffers.
  createDeallocStack(loc, rhsBuf);
  createDeallocStack(loc, lhsBuf);
  auto val = emitLoadValueOperation(loc, resultBuf,
                                    LoadOwnershipQualifier::Take);
  // Deallocate result buffer.
  createDeallocStack(loc, resultBuf);
  return val;
}

} // end namespace autodiff
} // end namespace nextcode
