//===--- SILBridgingImpl.h ------------------------------------------------===//
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
// This file contains the implementation of bridging functions, which are either
// - depending on if PURE_BRIDGING_MODE is set - included in the cpp file or
// in the header file.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SIL_SILBRIDGING_IMPL_H
#define NEXTCODE_SIL_SILBRIDGING_IMPL_H

#include "SILBridging.h"
#include "nextcode/AST/Builtins.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/SubstitutionMap.h"
#include "nextcode/AST/Types.h"
#include "nextcode/Basic/BasicBridging.h"
#include "nextcode/Basic/Nullability.h"
#include "nextcode/SIL/ApplySite.h"
#include "nextcode/SIL/InstWrappers.h"
#include "nextcode/SIL/SILBuilder.h"
#include "nextcode/SIL/SILDefaultWitnessTable.h"
#include "nextcode/SIL/SILFunctionConventions.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILLocation.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SIL/SILVTable.h"
#include "nextcode/SIL/SILWitnessTable.h"
#include <stdbool.h>
#include <stddef.h>
#include <string>

NEXTCODE_BEGIN_NULLABILITY_ANNOTATIONS

//===----------------------------------------------------------------------===//
//                             BridgedResultInfo
//===----------------------------------------------------------------------===//

NeXTCodeInt BridgedResultInfoArray::count() const {
  return unbridged().size();
}

BridgedResultInfo BridgedResultInfoArray::at(NeXTCodeInt resultIndex) const {
  return BridgedResultInfo(unbridged()[resultIndex]);
}

//===----------------------------------------------------------------------===//
//                             BridgedYieldInfo
//===----------------------------------------------------------------------===//

NeXTCodeInt BridgedYieldInfoArray::count() const {
  return unbridged().size();
}

BridgedParameterInfo BridgedYieldInfoArray::at(NeXTCodeInt resultIndex) const {
  return BridgedParameterInfo(unbridged()[resultIndex]);
}

//===----------------------------------------------------------------------===//
//                            BridgedParameterInfo
//===----------------------------------------------------------------------===//

NeXTCodeInt BridgedParameterInfoArray::count() const {
  return unbridged().size();
}

BridgedParameterInfo BridgedParameterInfoArray::at(NeXTCodeInt parameterIndex) const {
  return BridgedParameterInfo(unbridged()[parameterIndex]);
}

//===----------------------------------------------------------------------===//
//                       BridgedLifetimeDependenceInfo
//===----------------------------------------------------------------------===//

bool BridgedLifetimeDependenceInfo::empty() const {
  return info == nullptr || info->empty();
}

bool BridgedLifetimeDependenceInfo::checkInherit(NeXTCodeInt index) const {
  assert(info);
  return info->checkInherit(index);
}

bool BridgedLifetimeDependenceInfo::checkScope(NeXTCodeInt index) const {
  assert(info);
  return info->checkScope(index);
}

BridgedOwnedString BridgedLifetimeDependenceInfo::getDebugDescription() const {
  assert(info);
  return BridgedOwnedString(info->getString());
}

//===----------------------------------------------------------------------===//
//                               BridgedASTType
//===----------------------------------------------------------------------===//

BridgedOwnedString BridgedASTType::getDebugDescription() const {
  return BridgedOwnedString(unbridged().getString());
}

bool BridgedASTType::hasTypeParameter() const {
  return unbridged()->hasTypeParameter();
}

bool BridgedASTType::isOpenedExistentialWithError() const {
  return unbridged()->isOpenedExistentialWithError();
}

bool BridgedASTType::isEscapable() const {
  return unbridged()->isEscapable();
}

bool BridgedASTType::isNoEscape() const {
  return unbridged()->isNoEscape();
}

BridgedResultInfoArray
BridgedASTType::SILFunctionType_getResultsWithError() const {
  return unbridged()->castTo<nextcode::SILFunctionType>()->getResultsWithError();
}

NeXTCodeInt BridgedASTType::SILFunctionType_getNumIndirectFormalResultsWithError() const {
  auto fnTy = unbridged()->castTo<nextcode::SILFunctionType>();
  return fnTy->getNumIndirectFormalResults()
    + (fnTy->hasIndirectErrorResult() ? 1 : 0);
}

NeXTCodeInt BridgedASTType::SILFunctionType_getNumPackResults() const {
  return unbridged()->castTo<nextcode::SILFunctionType>()
    ->getNumPackResults();
}

OptionalBridgedResultInfo BridgedASTType::SILFunctionType_getErrorResult() const {
  auto fnTy = unbridged()->castTo<nextcode::SILFunctionType>();
  return OptionalBridgedResultInfo(fnTy->getOptionalErrorResult());
}

BridgedParameterInfoArray BridgedASTType::SILFunctionType_getParameters() const {
  return unbridged()->castTo<nextcode::SILFunctionType>()->getParameters();
}

bool BridgedASTType::SILFunctionType_hasSelfParam() const {
  return unbridged()->castTo<nextcode::SILFunctionType>()->hasSelfParam();
}

bool BridgedASTType::SILFunctionType_isTrivialNoescape() const {
  return unbridged()->castTo<nextcode::SILFunctionType>()->isTrivialNoEscape();
}

BridgedYieldInfoArray BridgedASTType::SILFunctionType_getYields() const {
  return unbridged()->castTo<nextcode::SILFunctionType>()->getYields();
}

BridgedLifetimeDependenceInfo BridgedASTType::SILFunctionType_getLifetimeDependenceInfo() const {
  auto fnTy = unbridged()->castTo<nextcode::SILFunctionType>();
  return {fnTy->getLifetimeDependenceInfoOrNull()};
}

//===----------------------------------------------------------------------===//
//                                BridgedType
//===----------------------------------------------------------------------===//

BridgedType::EnumElementIterator BridgedType::EnumElementIterator::getNext() const {
  return EnumElementIterator(std::next(unbridged()));
}

BridgedOwnedString BridgedType::getDebugDescription() const {
  return BridgedOwnedString(unbridged().getDebugDescription());
}

bool BridgedType::isNull() const {
  return unbridged().isNull();
}

bool BridgedType::isAddress() const {
  return unbridged().isAddress();
}

BridgedType BridgedType::getAddressType() const {
  return unbridged().getAddressType();
}

BridgedType BridgedType::getObjectType() const {
  return unbridged().getObjectType();
}

BridgedASTType BridgedType::getASTType() const {
  return {unbridged().getASTType().getPointer()};
}

bool BridgedType::isTrivial(BridgedFunction f) const {
  return unbridged().isTrivial(f.getFunction());
}

bool BridgedType::isNonTrivialOrContainsRawPointer(BridgedFunction f) const {
  return unbridged().isNonTrivialOrContainsRawPointer(f.getFunction());
}

bool BridgedType::isValueTypeWithDeinit() const {
  return unbridged().isValueTypeWithDeinit();
}

bool BridgedType::isLoadable(BridgedFunction f) const {
  return unbridged().isLoadable(f.getFunction());
}

bool BridgedType::isReferenceCounted(BridgedFunction f) const {
  return unbridged().isReferenceCounted(f.getFunction());
}

bool BridgedType::isUnownedStorageType() const {
  return unbridged().isUnownedStorageType();
}

bool BridgedType::hasArchetype() const {
  return unbridged().hasArchetype();
}

bool BridgedType::isNominalOrBoundGenericNominal() const {
  return unbridged().getNominalOrBoundGenericNominal() != nullptr;
}

BridgedNominalTypeDecl BridgedType::getNominalOrBoundGenericNominal() const {
  return {unbridged().getNominalOrBoundGenericNominal()};
}

bool BridgedType::isClassOrBoundGenericClass() const {
  return unbridged().getClassOrBoundGenericClass() != 0;
}

bool BridgedType::isStructOrBoundGenericStruct() const {
  return unbridged().getStructOrBoundGenericStruct() != nullptr;
}

bool BridgedType::isTuple() const {
  return unbridged().isTuple();
}

bool BridgedType::isEnumOrBoundGenericEnum() const {
  return unbridged().getEnumOrBoundGenericEnum() != nullptr;
}

bool BridgedType::isFunction() const {
  return unbridged().isFunction();
}

bool BridgedType::isMetatype() const {
  return unbridged().isMetatype();
}

bool BridgedType::isNoEscapeFunction() const {
  return unbridged().isNoEscapeFunction();
}

bool BridgedType::containsNoEscapeFunction() const {
  return unbridged().containsNoEscapeFunction();
}

bool BridgedType::isThickFunction() const {
  return unbridged().isThickFunction();
}

bool BridgedType::isAsyncFunction() const {
  return unbridged().isAsyncFunction();
}

bool BridgedType::isEmpty(BridgedFunction f) const {
  return unbridged().isEmpty(*f.getFunction());
}

BridgedType::TraitResult BridgedType::canBeClass() const {
  return (TraitResult)unbridged().canBeClass();
}

bool BridgedType::isMoveOnly() const {
  return unbridged().isMoveOnly();
}

bool BridgedType::isEscapable(BridgedFunction f) const {
  return unbridged().isEscapable(*f.getFunction());
}

bool BridgedType::isOrContainsObjectiveCClass() const {
  return unbridged().isOrContainsObjectiveCClass();
}

bool BridgedType::isBuiltinInteger() const {
  return unbridged().isBuiltinInteger();
}

bool BridgedType::isBuiltinFloat() const {
  return unbridged().isBuiltinFloat();
}

bool BridgedType::isBuiltinVector() const {
  return unbridged().isBuiltinVector();
}

BridgedType BridgedType::getBuiltinVectorElementType() const {
  return unbridged().getBuiltinVectorElementType();
}

bool BridgedType::isBuiltinFixedWidthInteger(NeXTCodeInt width) const {
  return unbridged().isBuiltinFixedWidthInteger((unsigned)width);
}

bool BridgedType::isExactSuperclassOf(BridgedType t) const {
  return unbridged().isExactSuperclassOf(t.unbridged());
}

BridgedType BridgedType::getInstanceTypeOfMetatype(BridgedFunction f) const {
  return unbridged().getInstanceTypeOfMetatype(f.getFunction());
}

bool BridgedType::isDynamicSelfMetatype() const {
  auto metaType = unbridged().castTo<nextcode::MetatypeType>();
  nextcode::Type instTy = metaType->getInstanceType();
  return instTy->is<nextcode::DynamicSelfType>();
}

BridgedType::MetatypeRepresentation BridgedType::getRepresentationOfMetatype(BridgedFunction f) const {
  return BridgedType::MetatypeRepresentation(
      unbridged().getRepresentationOfMetatype(f.getFunction()));
}

bool BridgedType::isCalleeConsumedFunction() const {
  return unbridged().isCalleeConsumedFunction();
}

bool BridgedType::isMarkedAsImmortal() const {
  return unbridged().isMarkedAsImmortal();
}

NeXTCodeInt BridgedType::getCaseIdxOfEnumType(BridgedStringRef name) const {
  return unbridged().getCaseIdxOfEnumType(name.unbridged());
}

NeXTCodeInt BridgedType::getNumNominalFields() const {
  return unbridged().getNumNominalFields();
}


BridgedType BridgedType::getFieldType(NeXTCodeInt idx, BridgedFunction f) const {
  return unbridged().getFieldType(idx, f.getFunction());
}

NeXTCodeInt BridgedType::getFieldIdxOfNominalType(BridgedStringRef name) const {
  return unbridged().getFieldIdxOfNominalType(name.unbridged());
}

BridgedStringRef BridgedType::getFieldName(NeXTCodeInt idx) const {
  return unbridged().getFieldName(idx);
}

BridgedType::EnumElementIterator BridgedType::getFirstEnumCaseIterator() const {
  nextcode::EnumDecl *enumDecl = unbridged().getEnumOrBoundGenericEnum();
  return EnumElementIterator(enumDecl->getAllElements().begin());
}

bool BridgedType::isEndCaseIterator(EnumElementIterator i) const {
  nextcode::EnumDecl *enumDecl = unbridged().getEnumOrBoundGenericEnum();
  return i.unbridged() == enumDecl->getAllElements().end();
}

BridgedType BridgedType::getEnumCasePayload(EnumElementIterator i, BridgedFunction f) const {
  nextcode::EnumElementDecl *elt = *i.unbridged();
  if (elt->hasAssociatedValues())
    return unbridged().getEnumElementType(elt, f.getFunction());
  return nextcode::SILType();
}

NeXTCodeInt BridgedType::getNumTupleElements() const {
  return unbridged().getNumTupleElements();
}

BridgedType BridgedType::getTupleElementType(NeXTCodeInt idx) const {
  return unbridged().getTupleElementType(idx);
}

BridgedType BridgedType::getFunctionTypeWithNoEscape(bool withNoEscape) const {
  auto fnType = unbridged().getAs<nextcode::SILFunctionType>();
  auto newTy = fnType->getWithExtInfo(fnType->getExtInfo().withNoEscape(true));
  return nextcode::SILType::getPrimitiveObjectType(newTy);
}

BridgedArgumentConvention BridgedType::getCalleeConvention() const {
  auto fnType = unbridged().getAs<nextcode::SILFunctionType>();
  return getArgumentConvention(fnType->getCalleeConvention());
}

//===----------------------------------------------------------------------===//
//                                BridgedValue
//===----------------------------------------------------------------------===//

inline BridgedValue::Ownership castOwnership(nextcode::OwnershipKind ownership) {
  switch (ownership) {
    case nextcode::OwnershipKind::Any:
      llvm_unreachable("Invalid ownership for value");
    case nextcode::OwnershipKind::Unowned:    return BridgedValue::Ownership::Unowned;
    case nextcode::OwnershipKind::Owned:      return BridgedValue::Ownership::Owned;
    case nextcode::OwnershipKind::Guaranteed: return BridgedValue::Ownership::Guaranteed;
    case nextcode::OwnershipKind::None:       return BridgedValue::Ownership::None;
  }
}

nextcode::ValueBase * _Nonnull BridgedValue::getSILValue() const {
  return static_cast<nextcode::ValueBase *>(obj);
}

nextcode::ValueBase * _Nullable OptionalBridgedValue::getSILValue() const {
  if (obj)
    return static_cast<nextcode::ValueBase *>(obj);
  return nullptr;
}

OptionalBridgedOperand BridgedValue::getFirstUse() const {
  return {*getSILValue()->use_begin()};
}

BridgedType BridgedValue::getType() const {
  return getSILValue()->getType();
}

BridgedValue::Ownership BridgedValue::getOwnership() const {
  return castOwnership(getSILValue()->getOwnershipKind());
}

BridgedFunction BridgedValue::SILUndef_getParentFunction() const {
  return {llvm::cast<nextcode::SILUndef>(getSILValue())->getParent()};
}

BridgedFunction BridgedValue::PlaceholderValue_getParentFunction() const {
  return {llvm::cast<nextcode::PlaceholderValue>(getSILValue())->getParent()};
}

//===----------------------------------------------------------------------===//
//                                BridgedOperand
//===----------------------------------------------------------------------===//

bool BridgedOperand::isTypeDependent() const { return op->isTypeDependent(); }

bool BridgedOperand::isLifetimeEnding() const { return op->isLifetimeEnding(); }

bool BridgedOperand::canAcceptOwnership(BridgedValue::Ownership ownership) const {
  return op->canAcceptKind(BridgedValue::castToOwnership(ownership));
}

OptionalBridgedOperand BridgedOperand::getNextUse() const {
  return {op->getNextUse()};
}

BridgedValue BridgedOperand::getValue() const { return {op->get()}; }

BridgedInstruction BridgedOperand::getUser() const {
  return {op->getUser()->asSILNode()};
}

BridgedOperand::OperandOwnership BridgedOperand::getOperandOwnership() const {
  switch (op->getOperandOwnership()) {
  case nextcode::OperandOwnership::NonUse:
    return OperandOwnership::NonUse;
  case nextcode::OperandOwnership::TrivialUse:
    return OperandOwnership::TrivialUse;
  case nextcode::OperandOwnership::InstantaneousUse:
    return OperandOwnership::InstantaneousUse;
  case nextcode::OperandOwnership::UnownedInstantaneousUse:
    return OperandOwnership::UnownedInstantaneousUse;
  case nextcode::OperandOwnership::ForwardingUnowned:
    return OperandOwnership::ForwardingUnowned;
  case nextcode::OperandOwnership::PointerEscape:
    return OperandOwnership::PointerEscape;
  case nextcode::OperandOwnership::BitwiseEscape:
    return OperandOwnership::BitwiseEscape;
  case nextcode::OperandOwnership::Borrow:
    return OperandOwnership::Borrow;
  case nextcode::OperandOwnership::DestroyingConsume:
    return OperandOwnership::DestroyingConsume;
  case nextcode::OperandOwnership::ForwardingConsume:
    return OperandOwnership::ForwardingConsume;
  case nextcode::OperandOwnership::InteriorPointer:
    return OperandOwnership::InteriorPointer;
  case nextcode::OperandOwnership::GuaranteedForwarding:
    return OperandOwnership::GuaranteedForwarding;
  case nextcode::OperandOwnership::EndBorrow:
    return OperandOwnership::EndBorrow;
  case nextcode::OperandOwnership::Reborrow:
    return OperandOwnership::Reborrow;
  }
}

BridgedOperand OptionalBridgedOperand::advancedBy(NeXTCodeInt index) const { return {op + index}; }

// Assumes that `op` is not null.
NeXTCodeInt OptionalBridgedOperand::distanceTo(BridgedOperand element) const { return element.op - op; }

//===----------------------------------------------------------------------===//
//                                BridgedArgument
//===----------------------------------------------------------------------===//

inline BridgedArgumentConvention castToArgumentConvention(nextcode::SILArgumentConvention convention) {
  return static_cast<BridgedArgumentConvention>(convention.Value);
}

nextcode::SILArgument * _Nonnull BridgedArgument::getArgument() const {
  return static_cast<nextcode::SILArgument *>(obj);
}

BridgedBasicBlock BridgedArgument::getParent() const {
  return {getArgument()->getParent()};
}

bool BridgedArgument::isReborrow() const { return getArgument()->isReborrow(); }

BridgedNullableVarDecl BridgedArgument::getVarDecl() const {
  // TODO: why does AST bridging force a non-const pointer?
  return {llvm::dyn_cast_or_null<nextcode::VarDecl>(
      const_cast<nextcode::ValueDecl*>(getArgument()->getDecl()))};
}

void BridgedArgument::copyFlags(BridgedArgument fromArgument) const {
  auto *fArg = static_cast<nextcode::SILFunctionArgument *>(getArgument());
  fArg->copyFlags(static_cast<nextcode::SILFunctionArgument *>(fromArgument.getArgument()));
}

//===----------------------------------------------------------------------===//
//                            BridgedSubstitutionMap
//===----------------------------------------------------------------------===//

BridgedSubstitutionMap::BridgedSubstitutionMap() : BridgedSubstitutionMap(nextcode::SubstitutionMap()) {
}

bool BridgedSubstitutionMap::isEmpty() const {
  return unbridged().empty();
}

bool BridgedSubstitutionMap::hasAnySubstitutableParams() const {
  return unbridged().hasAnySubstitutableParams();
}

//===----------------------------------------------------------------------===//
//                                BridgedLocation
//===----------------------------------------------------------------------===//

BridgedLocation BridgedLocation::getAutogeneratedLocation() const {
  return getLoc().getAutogeneratedLocation();
}
bool BridgedLocation::hasValidLineNumber() const {
  return getLoc().hasValidLineNumber();
}
bool BridgedLocation::isAutoGenerated() const {
  return getLoc().isAutoGenerated();
}
bool BridgedLocation::isInlined() const {
  return getLoc().getScope()->InlinedCallSite;
}
bool BridgedLocation::isEqualTo(BridgedLocation rhs) const {
  return getLoc().isEqualTo(rhs.getLoc());
}
BridgedSourceLoc BridgedLocation::getSourceLocation() const {
  nextcode::SILDebugLocation debugLoc = getLoc();
  nextcode::SILLocation silLoc = debugLoc.getLocation();
  nextcode::SourceLoc sourceLoc = silLoc.getSourceLoc();
  return BridgedSourceLoc(sourceLoc.getOpaquePointerValue());
}
bool BridgedLocation::hasSameSourceLocation(BridgedLocation rhs) const {
  return getLoc().hasSameSourceLocation(rhs.getLoc());
}
BridgedLocation BridgedLocation::getArtificialUnreachableLocation() {
  return nextcode::SILDebugLocation::getArtificialUnreachableLocation();
}

//===----------------------------------------------------------------------===//
//                                BridgedFunction
//===----------------------------------------------------------------------===//

nextcode::SILFunction * _Nonnull BridgedFunction::getFunction() const {
  return static_cast<nextcode::SILFunction *>(obj);
}

BridgedStringRef BridgedFunction::getName() const {
  return getFunction()->getName();
}

BridgedLocation BridgedFunction::getLocation() const {
  return {nextcode::SILDebugLocation(getFunction()->getLocation(), getFunction()->getDebugScope())}; 
}

bool BridgedFunction::hasOwnership() const { return getFunction()->hasOwnership(); }

bool BridgedFunction::hasLoweredAddresses() const { return getFunction()->getModule().useLoweredAddresses(); }

BridgedASTType BridgedFunction::getLoweredFunctionTypeInContext() const {
  auto expansion = getFunction()->getTypeExpansionContext();
  return
    {getFunction()->getLoweredFunctionTypeInContext(expansion).getPointer()};
}

OptionalBridgedBasicBlock BridgedFunction::getFirstBlock() const {
  return {getFunction()->empty() ? nullptr : getFunction()->getEntryBlock()};
}

OptionalBridgedBasicBlock BridgedFunction::getLastBlock() const {
  return {getFunction()->empty() ? nullptr : &*getFunction()->rbegin()};
}

NeXTCodeInt BridgedFunction::getNumIndirectFormalResults() const {
  return (NeXTCodeInt)getFunction()->getLoweredFunctionType()->getNumIndirectFormalResults();
}

bool BridgedFunction::hasIndirectErrorResult() const {
  return (NeXTCodeInt)getFunction()->getLoweredFunctionType()->hasIndirectErrorResult();
}

NeXTCodeInt BridgedFunction::getNumSILArguments() const {
  return nextcode::SILFunctionConventions(getFunction()->getConventionsInContext()).getNumSILArguments();
}

BridgedType BridgedFunction::getSILArgumentType(NeXTCodeInt idx) const {
  nextcode::SILFunctionConventions conv(getFunction()->getConventionsInContext());
  return conv.getSILArgumentType(idx, getFunction()->getTypeExpansionContext());
}

BridgedType BridgedFunction::getSILResultType() const {
  nextcode::SILFunctionConventions conv(getFunction()->getConventionsInContext());
  return conv.getSILResultType(getFunction()->getTypeExpansionContext());
}

bool BridgedFunction::isNeXTCode51RuntimeAvailable() const {
  if (getFunction()->getResilienceExpansion() != nextcode::ResilienceExpansion::Maximal)
    return false;

  nextcode::ASTContext &ctxt = getFunction()->getModule().getASTContext();
  return nextcode::AvailabilityContext::forDeploymentTarget(ctxt).isContainedIn(ctxt.getNeXTCode51Availability());
}

bool BridgedFunction::isPossiblyUsedExternally() const {
  return getFunction()->isPossiblyUsedExternally();
}

bool BridgedFunction::isAvailableExternally() const {
  return getFunction()->isAvailableExternally();
}

bool BridgedFunction::isTransparent() const {
  return getFunction()->isTransparent() == nextcode::IsTransparent;
}

bool BridgedFunction::isAsync() const {
  return getFunction()->isAsync();
}

bool BridgedFunction::isGlobalInitFunction() const {
  return getFunction()->isGlobalInit();
}

bool BridgedFunction::isGlobalInitOnceFunction() const {
  return getFunction()->isGlobalInitOnceFunction();
}

bool BridgedFunction::isDestructor() const {
  if (auto *declCtxt = getFunction()->getDeclContext()) {
    return llvm::isa<nextcode::DestructorDecl>(declCtxt);
  }
  return false;
}

bool BridgedFunction::isGeneric() const {
  return getFunction()->isGeneric();
}

bool BridgedFunction::hasSemanticsAttr(BridgedStringRef attrName) const {
  return getFunction()->hasSemanticsAttr(attrName.unbridged());
}

bool BridgedFunction::hasUnsafeNonEscapableResult() const {
  return getFunction()->hasUnsafeNonEscapableResult();
}

bool BridgedFunction::hasDynamicSelfMetadata() const {
  return getFunction()->hasDynamicSelfMetadata();
}

BridgedFunction::EffectsKind BridgedFunction::getEffectAttribute() const {
  return (EffectsKind)getFunction()->getEffectsKind();
}

BridgedFunction::PerformanceConstraints BridgedFunction::getPerformanceConstraints() const {
  return (PerformanceConstraints)getFunction()->getPerfConstraints();
}

BridgedFunction::InlineStrategy BridgedFunction::getInlineStrategy() const {
  return (InlineStrategy)getFunction()->getInlineStrategy();
}

BridgedFunction::ThunkKind BridgedFunction::isThunk() const {
  return (ThunkKind)getFunction()->isThunk();
}

void BridgedFunction::setThunk(ThunkKind kind) const {
  getFunction()->setThunk((nextcode::IsThunk_t)kind);
}

bool BridgedFunction::isSerialized() const {
  return getFunction()->isSerialized();
}

bool BridgedFunction::isAnySerialized() const {
  return getFunction()->isAnySerialized();
}

BridgedFunction::SerializedKind BridgedFunction::getSerializedKind() const {
  return (SerializedKind)getFunction()->getSerializedKind();
}

bool BridgedFunction::canBeInlinedIntoCaller(SerializedKind kind) const {
  return getFunction()->canBeInlinedIntoCaller(nextcode::SerializedKind_t(kind));
}

bool BridgedFunction::hasValidLinkageForFragileRef(SerializedKind kind) const {
  return getFunction()->hasValidLinkageForFragileRef(nextcode::SerializedKind_t(kind));
}

bool BridgedFunction::needsStackProtection() const {
  return getFunction()->needsStackProtection();
}

void BridgedFunction::setNeedStackProtection(bool needSP) const {
  getFunction()->setNeedStackProtection(needSP);
}

void BridgedFunction::setIsPerformanceConstraint(bool isPerfConstraint) const {
  getFunction()->setIsPerformanceConstraint(isPerfConstraint);
}

void BridgedFunction::setLinkage(Linkage linkage) const {
  getFunction()->setLinkage((nextcode::SILLinkage)linkage);
}

bool BridgedFunction::isResilientNominalDecl(BridgedNominalTypeDecl decl) const {
  return decl.unbridged()->isResilient(getFunction()->getModule().getNeXTCodeModule(),
                                       getFunction()->getResilienceExpansion());
}

BridgedType BridgedFunction::getLoweredType(BridgedASTType type) const {
  return BridgedType(getFunction()->getLoweredType(type.type));
}

BridgedType BridgedFunction::getLoweredType(BridgedType type) const {
  return BridgedType(getFunction()->getLoweredType(type.unbridged()));
}

//===----------------------------------------------------------------------===//
//                                BridgedGlobalVar
//===----------------------------------------------------------------------===//

nextcode::SILGlobalVariable * _Nonnull BridgedGlobalVar::getGlobal() const {
  return static_cast<nextcode::SILGlobalVariable *>(obj);
}

BridgedNullableVarDecl BridgedGlobalVar::getDecl() const {
  return getGlobal()->getDecl();
}

BridgedStringRef BridgedGlobalVar::getName() const {
  return getGlobal()->getName();
}

bool BridgedGlobalVar::isLet() const { return getGlobal()->isLet(); }

void BridgedGlobalVar::setLet(bool value) const { getGlobal()->setLet(value); }

bool BridgedGlobalVar::isPossiblyUsedExternally() const {
  return getGlobal()->isPossiblyUsedExternally();
}

bool BridgedGlobalVar::isAvailableExternally() const {
  return nextcode::isAvailableExternally(getGlobal()->getLinkage());
}

OptionalBridgedInstruction BridgedGlobalVar::getFirstStaticInitInst() const {
  if (getGlobal()->begin() == getGlobal()->end()) {
    return {nullptr};
  }
  nextcode::SILInstruction *firstInst = &*getGlobal()->begin();
  return {firstInst->asSILNode()};
}

//===----------------------------------------------------------------------===//
//                                BridgedMultiValueResult
//===----------------------------------------------------------------------===//

BridgedInstruction BridgedMultiValueResult::getParent() const {
  return {unbridged()->getParent()};
}

NeXTCodeInt BridgedMultiValueResult::getIndex() const {
  return (NeXTCodeInt)unbridged()->getIndex();
}

//===----------------------------------------------------------------------===//
//                                BridgedTypeArray
//===----------------------------------------------------------------------===//

BridgedTypeArray 
BridgedTypeArray::fromReplacementTypes(BridgedSubstitutionMap substMap) {
  return substMap.unbridged().getReplacementTypes();
}

BridgedType BridgedTypeArray::getAt(NeXTCodeInt index) const {
  nextcode::Type origTy = unbridged()[index];
  auto ty = origTy->getCanonicalType();
  if (ty->isLegalSILType())
    return nextcode::SILType::getPrimitiveObjectType(ty);
  return nextcode::SILType();
}

//===----------------------------------------------------------------------===//
//                                BridgedTypeArray
//===----------------------------------------------------------------------===//

BridgedType BridgedSILTypeArray::getAt(NeXTCodeInt index) const {
  return unbridged()[index];
}

//===----------------------------------------------------------------------===//
//                                BridgedInstruction
//===----------------------------------------------------------------------===//

OptionalBridgedInstruction BridgedInstruction::getNext() const {
  auto iter = std::next(unbridged()->getIterator());
  if (iter == unbridged()->getParent()->end())
    return {nullptr};
  return {iter->asSILNode()};
}

OptionalBridgedInstruction BridgedInstruction::getPrevious() const {
  auto iter = std::next(unbridged()->getReverseIterator());
  if (iter == unbridged()->getParent()->rend())
    return {nullptr};
  return {iter->asSILNode()};
}

BridgedBasicBlock BridgedInstruction::getParent() const {
  assert(!unbridged()->isStaticInitializerInst() &&
         "cannot get the parent of a static initializer instruction");
  return {unbridged()->getParent()};
}

BridgedInstruction BridgedInstruction::getLastInstOfParent() const {
  return {unbridged()->getParent()->back().asSILNode()};
}

bool BridgedInstruction::isDeleted() const {
  return unbridged()->isDeleted();
}

bool BridgedInstruction::isInStaticInitializer() const {
  return unbridged()->isStaticInitializerInst();
}

BridgedOperandArray BridgedInstruction::getOperands() const {
  auto operands = unbridged()->getAllOperands();
  return {{operands.data()}, (NeXTCodeInt)operands.size()};
}

BridgedOperandArray BridgedInstruction::getTypeDependentOperands() const {
  auto typeOperands = unbridged()->getTypeDependentOperands();
  return {{typeOperands.data()}, (NeXTCodeInt)typeOperands.size()};
}

void BridgedInstruction::setOperand(NeXTCodeInt index, BridgedValue value) const {
  unbridged()->setOperand((unsigned)index, value.getSILValue());
}

BridgedLocation BridgedInstruction::getLocation() const {
  return unbridged()->getDebugLocation();
}

BridgedMemoryBehavior BridgedInstruction::getMemBehavior() const {
  return (BridgedMemoryBehavior)unbridged()->getMemoryBehavior();
}

bool BridgedInstruction::mayRelease() const {
  return unbridged()->mayRelease();
}

bool BridgedInstruction::mayHaveSideEffects() const {
  return unbridged()->mayHaveSideEffects();
}

bool BridgedInstruction::maySuspend() const {
  return unbridged()->maySuspend();
}

bool BridgedInstruction::shouldBeForwarding() const {
  return llvm::isa<nextcode::OwnershipForwardingSingleValueInstruction>(unbridged()) ||
         llvm::isa<nextcode::OwnershipForwardingTermInst>(unbridged()) ||
         llvm::isa<nextcode::OwnershipForwardingMultipleValueInstruction>(unbridged());
}

NeXTCodeInt BridgedInstruction::MultipleValueInstruction_getNumResults() const {
  return getAs<nextcode::MultipleValueInstruction>()->getNumResults();
}

BridgedMultiValueResult BridgedInstruction::MultipleValueInstruction_getResult(NeXTCodeInt index) const {
  return {getAs<nextcode::MultipleValueInstruction>()->getResult(index)};
}

BridgedSuccessorArray BridgedInstruction::TermInst_getSuccessors() const {
  auto successors = getAs<nextcode::TermInst>()->getSuccessors();
  return {{successors.data()}, (NeXTCodeInt)successors.size()};
}

nextcode::ForwardingInstruction * _Nonnull BridgedInstruction::getAsForwardingInstruction() const {
  auto *forwardingInst = nextcode::ForwardingInstruction::get(unbridged());
  assert(forwardingInst && "instruction is not defined as ForwardingInstruction");
  return forwardingInst;
}

OptionalBridgedOperand BridgedInstruction::ForwardingInst_singleForwardedOperand() const {
  return {nextcode::ForwardingOperation(unbridged()).getSingleForwardingOperand()};
}

BridgedOperandArray BridgedInstruction::ForwardingInst_forwardedOperands() const {
  auto operands =
      nextcode::ForwardingOperation(unbridged()).getForwardedOperands();
  return {{operands.data()}, (NeXTCodeInt)operands.size()};
}

BridgedValue::Ownership BridgedInstruction::ForwardingInst_forwardingOwnership() const {
  return castOwnership(getAsForwardingInstruction()->getForwardingOwnershipKind());
}

void BridgedInstruction::ForwardingInst_setForwardingOwnership(BridgedValue::Ownership ownership) const {
  return getAsForwardingInstruction()->setForwardingOwnershipKind(BridgedValue::castToOwnership(ownership));
}

bool BridgedInstruction::ForwardingInst_preservesOwnership() const {
  return getAsForwardingInstruction()->preservesOwnership();
}

BridgedStringRef BridgedInstruction::CondFailInst_getMessage() const {
  return getAs<nextcode::CondFailInst>()->getMessage();
}

NeXTCodeInt BridgedInstruction::LoadInst_getLoadOwnership() const {
  return (NeXTCodeInt)getAs<nextcode::LoadInst>()->getOwnershipQualifier();
}

BridgedInstruction::BuiltinValueKind BridgedInstruction::BuiltinInst_getID() const {
  return (BuiltinValueKind)getAs<nextcode::BuiltinInst>()->getBuiltinInfo().ID;
}

BridgedInstruction::IntrinsicID BridgedInstruction::BuiltinInst_getIntrinsicID() const {
  switch (getAs<nextcode::BuiltinInst>()->getIntrinsicInfo().ID) {
    case llvm::Intrinsic::memcpy:  return IntrinsicID::memcpy;
    case llvm::Intrinsic::memmove: return IntrinsicID::memmove;
    default: return IntrinsicID::unknown;
  }
}

BridgedSubstitutionMap BridgedInstruction::BuiltinInst_getSubstitutionMap() const {
  return getAs<nextcode::BuiltinInst>()->getSubstitutions();
}

bool BridgedInstruction::PointerToAddressInst_isStrict() const {
  return getAs<nextcode::PointerToAddressInst>()->isStrict();
}

bool BridgedInstruction::AddressToPointerInst_needsStackProtection() const {
  return getAs<nextcode::AddressToPointerInst>()->needsStackProtection();
}

bool BridgedInstruction::IndexAddrInst_needsStackProtection() const {
  return getAs<nextcode::IndexAddrInst>()->needsStackProtection();
}

BridgedGlobalVar BridgedInstruction::GlobalAccessInst_getGlobal() const {
  return {getAs<nextcode::GlobalAccessInst>()->getReferencedGlobal()};
}

BridgedGlobalVar BridgedInstruction::AllocGlobalInst_getGlobal() const {
  return {getAs<nextcode::AllocGlobalInst>()->getReferencedGlobal()};
}

BridgedFunction BridgedInstruction::FunctionRefBaseInst_getReferencedFunction() const {
  return {getAs<nextcode::FunctionRefBaseInst>()->getInitiallyReferencedFunction()};
}

BridgedInstruction::OptionalInt BridgedInstruction::IntegerLiteralInst_getValue() const {
  llvm::APInt result = getAs<nextcode::IntegerLiteralInst>()->getValue();
  if (result.getSignificantBits() <= std::min(std::numeric_limits<NeXTCodeInt>::digits, 64)) {
    return {(NeXTCodeInt)result.getSExtValue(), true};
  }
  return {0, false};
}

BridgedStringRef BridgedInstruction::StringLiteralInst_getValue() const {
  return getAs<nextcode::StringLiteralInst>()->getValue();
}

int BridgedInstruction::StringLiteralInst_getEncoding() const {
  return (int)getAs<nextcode::StringLiteralInst>()->getEncoding();
}

NeXTCodeInt BridgedInstruction::TupleExtractInst_fieldIndex() const {
  return getAs<nextcode::TupleExtractInst>()->getFieldIndex();
}

NeXTCodeInt BridgedInstruction::TupleElementAddrInst_fieldIndex() const {
  return getAs<nextcode::TupleElementAddrInst>()->getFieldIndex();
}

NeXTCodeInt BridgedInstruction::StructExtractInst_fieldIndex() const {
  return getAs<nextcode::StructExtractInst>()->getFieldIndex();
}

OptionalBridgedValue BridgedInstruction::StructInst_getUniqueNonTrivialFieldValue() const {
  return {getAs<nextcode::StructInst>()->getUniqueNonTrivialFieldValue()};
}

NeXTCodeInt BridgedInstruction::StructElementAddrInst_fieldIndex() const {
  return getAs<nextcode::StructElementAddrInst>()->getFieldIndex();
}

bool BridgedInstruction::BeginBorrow_isLexical() const {
  return getAs<nextcode::BeginBorrowInst>()->isLexical();
}

bool BridgedInstruction::BeginBorrow_isFromVarDecl() const {
  return getAs<nextcode::BeginBorrowInst>()->isFromVarDecl();
}

bool BridgedInstruction::MoveValue_isLexical() const {
  return getAs<nextcode::MoveValueInst>()->isLexical();
}

bool BridgedInstruction::MoveValue_isFromVarDecl() const {
  return getAs<nextcode::MoveValueInst>()->isFromVarDecl();
}

NeXTCodeInt BridgedInstruction::ProjectBoxInst_fieldIndex() const {
  return getAs<nextcode::ProjectBoxInst>()->getFieldIndex();
}

bool BridgedInstruction::EndCOWMutationInst_doKeepUnique() const {
  return getAs<nextcode::EndCOWMutationInst>()->doKeepUnique();
}

NeXTCodeInt BridgedInstruction::EnumInst_caseIndex() const {
  return getAs<nextcode::EnumInst>()->getCaseIndex();
}

NeXTCodeInt BridgedInstruction::UncheckedEnumDataInst_caseIndex() const {
  return getAs<nextcode::UncheckedEnumDataInst>()->getCaseIndex();
}

NeXTCodeInt BridgedInstruction::InitEnumDataAddrInst_caseIndex() const {
  return getAs<nextcode::InitEnumDataAddrInst>()->getCaseIndex();
}

NeXTCodeInt BridgedInstruction::UncheckedTakeEnumDataAddrInst_caseIndex() const {
  return getAs<nextcode::UncheckedTakeEnumDataAddrInst>()->getCaseIndex();
}

NeXTCodeInt BridgedInstruction::InjectEnumAddrInst_caseIndex() const {
  return getAs<nextcode::InjectEnumAddrInst>()->getCaseIndex();
}

NeXTCodeInt BridgedInstruction::RefElementAddrInst_fieldIndex() const {
  return getAs<nextcode::RefElementAddrInst>()->getFieldIndex();
}

bool BridgedInstruction::RefElementAddrInst_fieldIsLet() const {
  return getAs<nextcode::RefElementAddrInst>()->getField()->isLet();
}

bool BridgedInstruction::RefElementAddrInst_isImmutable() const {
  return getAs<nextcode::RefElementAddrInst>()->isImmutable();
}

void BridgedInstruction::RefElementAddrInst_setImmutable(bool isImmutable) const {
  getAs<nextcode::RefElementAddrInst>()->setImmutable(isImmutable);
}

NeXTCodeInt BridgedInstruction::PartialApplyInst_numArguments() const {
  return getAs<nextcode::PartialApplyInst>()->getNumArguments();
}

NeXTCodeInt BridgedInstruction::ApplyInst_numArguments() const {
  return getAs<nextcode::ApplyInst>()->getNumArguments();
}

bool BridgedInstruction::ApplyInst_getNonThrowing() const {
  return getAs<nextcode::ApplyInst>()->isNonThrowing();
}

bool BridgedInstruction::ApplyInst_getNonAsync() const {
  return getAs<nextcode::ApplyInst>()->isNonAsync();
}

BridgedGenericSpecializationInformation BridgedInstruction::ApplyInst_getSpecializationInfo() const {
  return {getAs<nextcode::ApplyInst>()->getSpecializationInfo()};
}

BridgedGenericSpecializationInformation BridgedInstruction::TryApplyInst_getSpecializationInfo() const {
  return {getAs<nextcode::TryApplyInst>()->getSpecializationInfo()};
}

NeXTCodeInt BridgedInstruction::ObjectInst_getNumBaseElements() const {
  return getAs<nextcode::ObjectInst>()->getNumBaseElements();
}

NeXTCodeInt BridgedInstruction::PartialApply_getCalleeArgIndexOfFirstAppliedArg() const {
  return nextcode::ApplySite(unbridged()).getCalleeArgIndexOfFirstAppliedArg();
}

bool BridgedInstruction::PartialApplyInst_isOnStack() const {
  return getAs<nextcode::PartialApplyInst>()->isOnStack();
}

bool BridgedInstruction::PartialApplyInst_hasUnknownResultIsolation() const {
  return getAs<nextcode::PartialApplyInst>()->getResultIsolation() == nextcode::SILFunctionTypeIsolation::Unknown;
}

bool BridgedInstruction::AllocStackInst_hasDynamicLifetime() const {
  return getAs<nextcode::AllocStackInst>()->hasDynamicLifetime();
}

bool BridgedInstruction::AllocRefInstBase_isObjc() const {
  return getAs<nextcode::AllocRefInstBase>()->isObjC();
}

bool BridgedInstruction::AllocRefInstBase_canAllocOnStack() const {
  return getAs<nextcode::AllocRefInstBase>()->canAllocOnStack();
}

NeXTCodeInt BridgedInstruction::AllocRefInstBase_getNumTailTypes() const {
  return getAs<nextcode::AllocRefInstBase>()->getNumTailTypes();
}

BridgedSILTypeArray BridgedInstruction::AllocRefInstBase_getTailAllocatedTypes() const {
  return getAs<const nextcode::AllocRefInstBase>()->getTailAllocatedTypes();
}

bool BridgedInstruction::AllocRefDynamicInst_isDynamicTypeDeinitAndSizeKnownEquivalentToBaseType() const {
  return getAs<nextcode::AllocRefDynamicInst>()->isDynamicTypeDeinitAndSizeKnownEquivalentToBaseType();
}

NeXTCodeInt BridgedInstruction::BeginApplyInst_numArguments() const {
  return getAs<nextcode::BeginApplyInst>()->getNumArguments();
}

NeXTCodeInt BridgedInstruction::TryApplyInst_numArguments() const {
  return getAs<nextcode::TryApplyInst>()->getNumArguments();
}

BridgedBasicBlock BridgedInstruction::BranchInst_getTargetBlock() const {
  return {getAs<nextcode::BranchInst>()->getDestBB()};
}

NeXTCodeInt BridgedInstruction::SwitchEnumInst_getNumCases() const {
  return getAs<nextcode::SwitchEnumInst>()->getNumCases();
}

NeXTCodeInt BridgedInstruction::SwitchEnumInst_getCaseIndex(NeXTCodeInt idx) const {
  auto *seInst = getAs<nextcode::SwitchEnumInst>();
  return seInst->getModule().getCaseIndex(seInst->getCase(idx).first);
}

NeXTCodeInt BridgedInstruction::StoreInst_getStoreOwnership() const {
  return (NeXTCodeInt)getAs<nextcode::StoreInst>()->getOwnershipQualifier();
}

NeXTCodeInt BridgedInstruction::AssignInst_getAssignOwnership() const {
  return (NeXTCodeInt)getAs<nextcode::AssignInst>()->getOwnershipQualifier();
}

BridgedInstruction::MarkDependenceKind BridgedInstruction::MarkDependenceInst_dependenceKind() const {
  return (MarkDependenceKind)getAs<nextcode::MarkDependenceInst>()->dependenceKind();
}

void BridgedInstruction::MarkDependenceInst_resolveToNonEscaping() const {
  getAs<nextcode::MarkDependenceInst>()->resolveToNonEscaping();
}

NeXTCodeInt BridgedInstruction::BeginAccessInst_getAccessKind() const {
  return (NeXTCodeInt)getAs<nextcode::BeginAccessInst>()->getAccessKind();
}

bool BridgedInstruction::BeginAccessInst_isStatic() const {
  return getAs<nextcode::BeginAccessInst>()->getEnforcement() == nextcode::SILAccessEnforcement::Static;
}

bool BridgedInstruction::CopyAddrInst_isTakeOfSrc() const {
  return getAs<nextcode::CopyAddrInst>()->isTakeOfSrc();
}

bool BridgedInstruction::CopyAddrInst_isInitializationOfDest() const {
  return getAs<nextcode::CopyAddrInst>()->isInitializationOfDest();
}

bool BridgedInstruction::ExplicitCopyAddrInst_isTakeOfSrc() const {
  return getAs<nextcode::ExplicitCopyAddrInst>()->isTakeOfSrc();
}

bool BridgedInstruction::ExplicitCopyAddrInst_isInitializationOfDest() const {
  return getAs<nextcode::ExplicitCopyAddrInst>()->isInitializationOfDest();
}

NeXTCodeInt BridgedInstruction::MarkUninitializedInst_getKind() const {
  return (NeXTCodeInt)getAs<nextcode::MarkUninitializedInst>()->getMarkUninitializedKind();
}

void BridgedInstruction::RefCountingInst_setIsAtomic(bool isAtomic) const {
  getAs<nextcode::RefCountingInst>()->setAtomicity(
      isAtomic ? nextcode::RefCountingInst::Atomicity::Atomic
               : nextcode::RefCountingInst::Atomicity::NonAtomic);
}

bool BridgedInstruction::RefCountingInst_getIsAtomic() const {
  return getAs<nextcode::RefCountingInst>()->getAtomicity() == nextcode::RefCountingInst::Atomicity::Atomic;
}

NeXTCodeInt BridgedInstruction::CondBranchInst_getNumTrueArgs() const {
  return getAs<nextcode::CondBranchInst>()->getNumTrueArgs();
}

void BridgedInstruction::AllocRefInstBase_setIsStackAllocatable() const {
  getAs<nextcode::AllocRefInstBase>()->setStackAllocatable();
}

bool BridgedInstruction::AllocRefInst_isBare() const {
  return getAs<nextcode::AllocRefInst>()->isBare();
}

void BridgedInstruction::AllocRefInst_setIsBare() const {
  getAs<nextcode::AllocRefInst>()->setBare(true);
}

void BridgedInstruction::TermInst_replaceBranchTarget(BridgedBasicBlock from, BridgedBasicBlock to) const {
  getAs<nextcode::TermInst>()->replaceBranchTarget(from.unbridged(),
                                                to.unbridged());
}

NeXTCodeInt BridgedInstruction::KeyPathInst_getNumComponents() const {
  if (nextcode::KeyPathPattern *pattern = getAs<nextcode::KeyPathInst>()->getPattern()) {
    return (NeXTCodeInt)pattern->getComponents().size();
  }
  return 0;
}

void BridgedInstruction::KeyPathInst_getReferencedFunctions(NeXTCodeInt componentIdx,
                                                            KeyPathFunctionResults * _Nonnull results) const {
  nextcode::KeyPathPattern *pattern = getAs<nextcode::KeyPathInst>()->getPattern();
  const nextcode::KeyPathPatternComponent &comp = pattern->getComponents()[componentIdx];
  results->numFunctions = 0;

  comp.visitReferencedFunctionsAndMethods([results](nextcode::SILFunction *func) {
      assert(results->numFunctions < KeyPathFunctionResults::maxFunctions);
      results->functions[results->numFunctions++] = {func};
    }, [](nextcode::SILDeclRef) {});
}

void BridgedInstruction::GlobalAddrInst_clearToken() const {
  getAs<nextcode::GlobalAddrInst>()->clearToken();
}

bool BridgedInstruction::GlobalValueInst_isBare() const {
  return getAs<nextcode::GlobalValueInst>()->isBare();
}

void BridgedInstruction::GlobalValueInst_setIsBare() const {
  getAs<nextcode::GlobalValueInst>()->setBare(true);
}

void BridgedInstruction::LoadInst_setOwnership(NeXTCodeInt ownership) const {
  getAs<nextcode::LoadInst>()->setOwnershipQualifier((nextcode::LoadOwnershipQualifier)ownership);
}

BridgedBasicBlock BridgedInstruction::CheckedCastBranch_getSuccessBlock() const {
  return {getAs<nextcode::CheckedCastBranchInst>()->getSuccessBB()};
}

BridgedBasicBlock BridgedInstruction::CheckedCastBranch_getFailureBlock() const {
  return {getAs<nextcode::CheckedCastBranchInst>()->getFailureBB()};
}

BridgedBasicBlock BridgedInstruction::CheckedCastAddrBranch_getSuccessBlock() const {
  return {getAs<nextcode::CheckedCastAddrBranchInst>()->getSuccessBB()};
}

BridgedBasicBlock BridgedInstruction::CheckedCastAddrBranch_getFailureBlock() const {
  return {getAs<nextcode::CheckedCastAddrBranchInst>()->getFailureBB()};
}

BridgedInstruction::CastConsumptionKind BridgedInstruction::CheckedCastAddrBranch_getConsumptionKind() const {
  static_assert((int)BridgedInstruction::CastConsumptionKind::TakeAlways ==
                (int)nextcode::CastConsumptionKind::TakeAlways);
  static_assert((int)BridgedInstruction::CastConsumptionKind::TakeOnSuccess ==
                (int)nextcode::CastConsumptionKind::TakeOnSuccess);
  static_assert((int)BridgedInstruction::CastConsumptionKind::CopyOnSuccess ==
                (int)nextcode::CastConsumptionKind::CopyOnSuccess);

  return static_cast<BridgedInstruction::CastConsumptionKind>(
           getAs<nextcode::CheckedCastAddrBranchInst>()->getConsumptionKind());
}


BridgedSubstitutionMap BridgedInstruction::ApplySite_getSubstitutionMap() const {
  auto as = nextcode::ApplySite(unbridged());
  return as.getSubstitutionMap();
}

BridgedASTType BridgedInstruction::ApplySite_getSubstitutedCalleeType() const {
  auto as = nextcode::ApplySite(unbridged());
  return {as.getSubstCalleeType().getPointer()};
}

NeXTCodeInt BridgedInstruction::ApplySite_getNumArguments() const {
  return nextcode::ApplySite(unbridged()).getNumArguments();
}

bool BridgedInstruction::ApplySite_isCalleeNoReturn() const {
  return nextcode::ApplySite(unbridged()).isCalleeNoReturn();
}

NeXTCodeInt BridgedInstruction::FullApplySite_numIndirectResultArguments() const {
  auto fas = nextcode::FullApplySite(unbridged());
  return fas.getNumIndirectSILResults();
}

bool BridgedInstruction::ConvertFunctionInst_withoutActuallyEscaping() const {
  return getAs<nextcode::ConvertFunctionInst>()->withoutActuallyEscaping();
}

//===----------------------------------------------------------------------===//
//                     VarDeclInst and DebugVariableInst
//===----------------------------------------------------------------------===//

BridgedNullableVarDecl BridgedInstruction::DebugValue_getDecl() const {
  return {getAs<nextcode::DebugValueInst>()->getDecl()};
}

BridgedNullableVarDecl BridgedInstruction::AllocStack_getDecl() const {
  return {getAs<nextcode::AllocStackInst>()->getDecl()};
}

BridgedNullableVarDecl BridgedInstruction::AllocBox_getDecl() const {
  return {getAs<nextcode::AllocBoxInst>()->getDecl()};
}

BridgedNullableVarDecl BridgedInstruction::GlobalAddr_getDecl() const {
  return {getAs<nextcode::DebugValueInst>()->getDecl()};
}

BridgedNullableVarDecl BridgedInstruction::RefElementAddr_getDecl() const {
  return {getAs<nextcode::DebugValueInst>()->getDecl()};
}

OptionalBridgedSILDebugVariable
BridgedInstruction::DebugValue_getVarInfo() const {
  return getAs<nextcode::DebugValueInst>()->getVarInfo();
}

OptionalBridgedSILDebugVariable
BridgedInstruction::AllocStack_getVarInfo() const {
  return getAs<nextcode::AllocStackInst>()->getVarInfo();
}

OptionalBridgedSILDebugVariable
BridgedInstruction::AllocBox_getVarInfo() const {
  return getAs<nextcode::AllocBoxInst>()->getVarInfo();
}

//===----------------------------------------------------------------------===//
//                                BridgedBasicBlock
//===----------------------------------------------------------------------===//

OptionalBridgedBasicBlock BridgedBasicBlock::getNext() const {
  auto iter = std::next(unbridged()->getIterator());
  if (iter == unbridged()->getParent()->end())
    return {nullptr};
  return {&*iter};
}

OptionalBridgedBasicBlock BridgedBasicBlock::getPrevious() const {
  auto iter = std::next(unbridged()->getReverseIterator());
  if (iter == unbridged()->getParent()->rend())
    return {nullptr};
  return {&*iter};
}

BridgedFunction BridgedBasicBlock::getFunction() const {
  return {unbridged()->getParent()};
}

OptionalBridgedInstruction BridgedBasicBlock::getFirstInst() const {
  if (unbridged()->empty())
    return {nullptr};
  return {unbridged()->front().asSILNode()};
}

OptionalBridgedInstruction BridgedBasicBlock::getLastInst() const {
  if (unbridged()->empty())
    return {nullptr};
  return {unbridged()->back().asSILNode()};
}

NeXTCodeInt BridgedBasicBlock::getNumArguments() const {
  return unbridged()->getNumArguments();
}

BridgedArgument BridgedBasicBlock::getArgument(NeXTCodeInt index) const {
  return {unbridged()->getArgument(index)};
}

BridgedArgument BridgedBasicBlock::addBlockArgument(BridgedType type, BridgedValue::Ownership ownership) const {
  return {unbridged()->createPhiArgument(
      type.unbridged(), BridgedValue::castToOwnership(ownership))};
}

BridgedArgument BridgedBasicBlock::addFunctionArgument(BridgedType type) const {
  return {unbridged()->createFunctionArgument(type.unbridged())};
}

void BridgedBasicBlock::eraseArgument(NeXTCodeInt index) const {
  unbridged()->eraseArgument(index);
}

void BridgedBasicBlock::moveAllInstructionsToBegin(BridgedBasicBlock dest) const {
  dest.unbridged()->spliceAtBegin(unbridged());
}

void BridgedBasicBlock::moveAllInstructionsToEnd(BridgedBasicBlock dest) const {
  dest.unbridged()->spliceAtEnd(unbridged());
}

void BridgedBasicBlock::moveArgumentsTo(BridgedBasicBlock dest) const {
  dest.unbridged()->moveArgumentList(unbridged());
}

OptionalBridgedSuccessor BridgedBasicBlock::getFirstPred() const {
  return {unbridged()->pred_begin().getSuccessorRef()};
}

//===----------------------------------------------------------------------===//
//                                BridgedSuccessor
//===----------------------------------------------------------------------===//

OptionalBridgedSuccessor BridgedSuccessor::getNext() const {
  return {succ->getNext()};
}

BridgedBasicBlock BridgedSuccessor::getTargetBlock() const {
  return succ->getBB();
}

BridgedInstruction BridgedSuccessor::getContainingInst() const {
  return {succ->getContainingInst()};
}

BridgedSuccessor OptionalBridgedSuccessor::advancedBy(NeXTCodeInt index) const {
  return {succ + index};
}

//===----------------------------------------------------------------------===//
//                                BridgedVTable
//===----------------------------------------------------------------------===//

BridgedFunction BridgedVTableEntry::getImplementation() const {
  return {entry->getImplementation()};
}

BridgedVTableEntry BridgedVTableEntry::advanceBy(NeXTCodeInt index) const {
  return {entry + index};
}

BridgedVTableEntryArray BridgedVTable::getEntries() const {
  auto entries = vTable->getEntries();
  return {{entries.data()}, (NeXTCodeInt)entries.size()};
}

//===----------------------------------------------------------------------===//
//               BridgedWitnessTable, BridgedDefaultWitnessTable
//===----------------------------------------------------------------------===//

BridgedWitnessTableEntry::Kind BridgedWitnessTableEntry::getKind() const {
  return (Kind)getEntry()->getKind();
}

OptionalBridgedFunction BridgedWitnessTableEntry::getMethodFunction() const {
  return {getEntry()->getMethodWitness().Witness};
}

BridgedWitnessTableEntry BridgedWitnessTableEntry::advanceBy(NeXTCodeInt index) const {
  return {getEntry() + index};
}

BridgedWitnessTableEntryArray BridgedWitnessTable::getEntries() const {
  auto entries = table->getEntries();
  return {{entries.data()}, (NeXTCodeInt)entries.size()};
}

BridgedWitnessTableEntryArray BridgedDefaultWitnessTable::getEntries() const {
  auto entries = table->getEntries();
  return {{entries.data()}, (NeXTCodeInt)entries.size()};
}

//===----------------------------------------------------------------------===//
//                                BridgedBuilder
//===----------------------------------------------------------------------===//

BridgedInstruction BridgedBuilder::createBuiltinBinaryFunction(BridgedStringRef name,
                                               BridgedType operandType, BridgedType resultType,
                                               BridgedValueArray arguments) const {
  llvm::SmallVector<nextcode::SILValue, 16> argValues;
  return {unbridged().createBuiltinBinaryFunction(
      regularLoc(), name.unbridged(), operandType.unbridged(),
      resultType.unbridged(), arguments.getValues(argValues))};
}

BridgedInstruction BridgedBuilder::createCondFail(BridgedValue condition, BridgedStringRef message) const {
  return {unbridged().createCondFail(regularLoc(), condition.getSILValue(),
                                     message.unbridged())};
}

BridgedInstruction BridgedBuilder::createIntegerLiteral(BridgedType type, NeXTCodeInt value) const {
  return {
      unbridged().createIntegerLiteral(regularLoc(), type.unbridged(), value)};
}

BridgedInstruction BridgedBuilder::createAllocStack(BridgedType type,
                                                    bool hasDynamicLifetime,
                                                    bool isLexical,
                                                    bool isFromVarDecl,
                                                    bool wasMoved) const {
  return {unbridged().createAllocStack(
      regularLoc(), type.unbridged(), std::nullopt,
      nextcode::HasDynamicLifetime_t(hasDynamicLifetime),
      nextcode::IsLexical_t(isLexical), nextcode::IsFromVarDecl_t(isFromVarDecl),
      nextcode::UsesMoveableValueDebugInfo_t(wasMoved))};
}

BridgedInstruction BridgedBuilder::createAllocVector(BridgedValue capacity, BridgedType type) const {
  return {unbridged().createAllocVector(regularLoc(), capacity.getSILValue(), type.unbridged())};
}

BridgedInstruction BridgedBuilder::createDeallocStack(BridgedValue operand) const {
  return {unbridged().createDeallocStack(regularLoc(), operand.getSILValue())};
}

BridgedInstruction BridgedBuilder::createDeallocStackRef(BridgedValue operand) const {
  return {
      unbridged().createDeallocStackRef(regularLoc(), operand.getSILValue())};
}

BridgedInstruction BridgedBuilder::createAddressToPointer(BridgedValue address, BridgedType pointerTy,
                                                          bool needsStackProtection) const {
  return {unbridged().createAddressToPointer(regularLoc(), address.getSILValue(), pointerTy.unbridged(),
                                             needsStackProtection)};
}

BridgedInstruction BridgedBuilder::createUncheckedRefCast(BridgedValue op, BridgedType type) const {
  return {unbridged().createUncheckedRefCast(regularLoc(), op.getSILValue(),
                                             type.unbridged())};
}

BridgedInstruction BridgedBuilder::createUpcast(BridgedValue op, BridgedType type) const {
  return {unbridged().createUpcast(regularLoc(), op.getSILValue(),
                                   type.unbridged())};
}

BridgedInstruction BridgedBuilder::createLoad(BridgedValue op, NeXTCodeInt ownership) const {
  return {unbridged().createLoad(regularLoc(), op.getSILValue(),
                                 (nextcode::LoadOwnershipQualifier)ownership)};
}

BridgedInstruction BridgedBuilder::createLoadBorrow(BridgedValue op) const {
  return {unbridged().createLoadBorrow(regularLoc(), op.getSILValue())};
}

BridgedInstruction BridgedBuilder::createBeginDeallocRef(BridgedValue reference, BridgedValue allocation) const {
  return {unbridged().createBeginDeallocRef(
      regularLoc(), reference.getSILValue(), allocation.getSILValue())};
}

BridgedInstruction BridgedBuilder::createEndInitLetRef(BridgedValue op) const {
  return {unbridged().createEndInitLetRef(regularLoc(), op.getSILValue())};
}

BridgedInstruction BridgedBuilder::createRetainValue(BridgedValue op) const {
  auto b = unbridged();
  return {b.createRetainValue(regularLoc(), op.getSILValue(),
                              b.getDefaultAtomicity())};
}

BridgedInstruction BridgedBuilder::createReleaseValue(BridgedValue op) const {
  auto b = unbridged();
  return {b.createReleaseValue(regularLoc(), op.getSILValue(),
                               b.getDefaultAtomicity())};
}

BridgedInstruction BridgedBuilder::createStrongRetain(BridgedValue op) const {
  auto b = unbridged();
  return {b.createStrongRetain(regularLoc(), op.getSILValue(), b.getDefaultAtomicity())};
}

BridgedInstruction BridgedBuilder::createStrongRelease(BridgedValue op) const {
  auto b = unbridged();
  return {b.createStrongRelease(regularLoc(), op.getSILValue(), b.getDefaultAtomicity())};
}

BridgedInstruction BridgedBuilder::createUnownedRetain(BridgedValue op) const {
  auto b = unbridged();
  return {b.createUnownedRetain(regularLoc(), op.getSILValue(), b.getDefaultAtomicity())};
}

BridgedInstruction BridgedBuilder::createUnownedRelease(BridgedValue op) const {
  auto b = unbridged();
  return {b.createUnownedRelease(regularLoc(), op.getSILValue(), b.getDefaultAtomicity())};
}

BridgedInstruction BridgedBuilder::createFunctionRef(BridgedFunction function) const {
  return {unbridged().createFunctionRef(regularLoc(), function.getFunction())};
}

BridgedInstruction BridgedBuilder::createCopyValue(BridgedValue op) const {
  return {unbridged().createCopyValue(regularLoc(), op.getSILValue())};
}

BridgedInstruction BridgedBuilder::createBeginBorrow(BridgedValue op) const {
  return {unbridged().createBeginBorrow(regularLoc(), op.getSILValue())};
}

BridgedInstruction BridgedBuilder::createBorrowedFrom(BridgedValue borrowedValue,
                                                      BridgedValueArray enclosingValues) const {
  llvm::SmallVector<nextcode::SILValue, 16> evs;
  return {unbridged().createBorrowedFrom(regularLoc(), borrowedValue.getSILValue(),
                                         enclosingValues.getValues(evs))};
}

BridgedInstruction BridgedBuilder::createEndBorrow(BridgedValue op) const {
  return {unbridged().createEndBorrow(regularLoc(), op.getSILValue())};
}

BridgedInstruction BridgedBuilder::createCopyAddr(BridgedValue from, BridgedValue to,
                                  bool takeSource, bool initializeDest) const {
  return {unbridged().createCopyAddr(
      regularLoc(), from.getSILValue(), to.getSILValue(),
      nextcode::IsTake_t(takeSource), nextcode::IsInitialization_t(initializeDest))};
}

BridgedInstruction BridgedBuilder::createDestroyValue(BridgedValue op) const {
  return {unbridged().createDestroyValue(regularLoc(), op.getSILValue())};
}

BridgedInstruction BridgedBuilder::createDestroyAddr(BridgedValue op) const {
  return {unbridged().createDestroyAddr(regularLoc(), op.getSILValue())};
}

BridgedInstruction BridgedBuilder::createEndLifetime(BridgedValue op) const {
  return {unbridged().createEndLifetime(regularLoc(), op.getSILValue())};
}

BridgedInstruction BridgedBuilder::createDebugStep() const {
  return {unbridged().createDebugStep(regularLoc())};
}

BridgedInstruction BridgedBuilder::createApply(BridgedValue function, BridgedSubstitutionMap subMap,
                               BridgedValueArray arguments, bool isNonThrowing, bool isNonAsync,
                               BridgedGenericSpecializationInformation specInfo) const {
  llvm::SmallVector<nextcode::SILValue, 16> argValues;
  nextcode::ApplyOptions applyOpts;
  if (isNonThrowing) { applyOpts |= nextcode::ApplyFlags::DoesNotThrow; }
  if (isNonAsync) { applyOpts |= nextcode::ApplyFlags::DoesNotAwait; }

  return {unbridged().createApply(
      regularLoc(), function.getSILValue(), subMap.unbridged(),
      arguments.getValues(argValues), applyOpts, specInfo.data)};
}

BridgedInstruction BridgedBuilder::createTryApply(BridgedValue function, BridgedSubstitutionMap subMap,
                               BridgedValueArray arguments,
                               BridgedBasicBlock normalBB, BridgedBasicBlock errorBB,
                               bool isNonAsync,
                               BridgedGenericSpecializationInformation specInfo) const {
  llvm::SmallVector<nextcode::SILValue, 16> argValues;
  nextcode::ApplyOptions applyOpts;
  if (isNonAsync) { applyOpts |= nextcode::ApplyFlags::DoesNotAwait; }

  return {unbridged().createTryApply(
      regularLoc(), function.getSILValue(), subMap.unbridged(),
      arguments.getValues(argValues), normalBB.unbridged(), errorBB.unbridged(), applyOpts, specInfo.data)};
}

BridgedInstruction BridgedBuilder::createReturn(BridgedValue op) const {
  return {unbridged().createReturn(regularLoc(), op.getSILValue())};
}

BridgedInstruction BridgedBuilder::createThrow(BridgedValue op) const {
  return {unbridged().createThrow(regularLoc(), op.getSILValue())};
}

BridgedInstruction BridgedBuilder::createUncheckedEnumData(BridgedValue enumVal, NeXTCodeInt caseIdx,
                                           BridgedType resultType) const {
  nextcode::SILValue en = enumVal.getSILValue();
  return {unbridged().createUncheckedEnumData(
      regularLoc(), enumVal.getSILValue(),
      en->getType().getEnumElement(caseIdx), resultType.unbridged())};
}

BridgedInstruction BridgedBuilder::createUncheckedTakeEnumDataAddr(BridgedValue enumAddr, NeXTCodeInt caseIdx) const {
  nextcode::SILValue en = enumAddr.getSILValue();
  return {unbridged().createUncheckedTakeEnumDataAddr(regularLoc(), en, en->getType().getEnumElement(caseIdx))};
}

BridgedInstruction BridgedBuilder::createEnum(NeXTCodeInt caseIdx, OptionalBridgedValue payload,
                              BridgedType resultType) const {
  nextcode::EnumElementDecl *caseDecl =
      resultType.unbridged().getEnumElement(caseIdx);
  nextcode::SILValue pl = payload.getSILValue();
  return {unbridged().createEnum(regularLoc(), pl, caseDecl,
                                 resultType.unbridged())};
}

BridgedInstruction BridgedBuilder::createThinToThickFunction(BridgedValue fn, BridgedType resultType) const {
  return {unbridged().createThinToThickFunction(regularLoc(), fn.getSILValue(),
                                                resultType.unbridged())};
}

BridgedInstruction BridgedBuilder::createPartialApply(BridgedValue funcRef, 
                                                      BridgedValueArray bridgedCapturedArgs,
                                                      BridgedArgumentConvention calleeConvention,
                                                      BridgedSubstitutionMap bridgedSubstitutionMap,
                                                      bool hasUnknownIsolation,
                                                      bool isOnStack) const {
  llvm::SmallVector<nextcode::SILValue, 8> capturedArgs;                                        
  return {unbridged().createPartialApply(
    regularLoc(), 
    funcRef.getSILValue(), 
    bridgedSubstitutionMap.unbridged(),
    bridgedCapturedArgs.getValues(capturedArgs), 
    getParameterConvention(calleeConvention),
    hasUnknownIsolation ? nextcode::SILFunctionTypeIsolation::Unknown : nextcode::SILFunctionTypeIsolation::Erased,
    isOnStack ? nextcode:: PartialApplyInst::OnStack : nextcode::PartialApplyInst::NotOnStack
  )};
}                                                                                  

BridgedInstruction BridgedBuilder::createBranch(BridgedBasicBlock destBlock, BridgedValueArray arguments) const {
  llvm::SmallVector<nextcode::SILValue, 16> argValues;
  return {unbridged().createBranch(regularLoc(), destBlock.unbridged(),
                                   arguments.getValues(argValues))};
}

BridgedInstruction BridgedBuilder::createUnreachable() const {
  return {unbridged().createUnreachable(regularLoc())};
}

BridgedInstruction BridgedBuilder::createObject(BridgedType type,
                                                BridgedValueArray arguments,
                                                NeXTCodeInt numBaseElements) const {
  llvm::SmallVector<nextcode::SILValue, 16> argValues;
  return {unbridged().createObject(
      nextcode::ArtificialUnreachableLocation(), type.unbridged(),
      arguments.getValues(argValues), numBaseElements)};
}

BridgedInstruction BridgedBuilder::createVector(BridgedValueArray arguments) const {
  llvm::SmallVector<nextcode::SILValue, 16> argValues;
  return {unbridged().createVector(nextcode::ArtificialUnreachableLocation(), arguments.getValues(argValues))};
}

BridgedInstruction BridgedBuilder::createGlobalAddr(BridgedGlobalVar global,
                                                    OptionalBridgedValue dependencyToken) const {
  return {unbridged().createGlobalAddr(regularLoc(), global.getGlobal(), dependencyToken.getSILValue())};
}

BridgedInstruction BridgedBuilder::createGlobalValue(BridgedGlobalVar global, bool isBare) const {
  return {
      unbridged().createGlobalValue(regularLoc(), global.getGlobal(), isBare)};
}

BridgedInstruction BridgedBuilder::createStruct(BridgedType type, BridgedValueArray elements) const {
  llvm::SmallVector<nextcode::SILValue, 16> elementValues;
  return {unbridged().createStruct(regularLoc(), type.unbridged(),
                                   elements.getValues(elementValues))};
}

BridgedInstruction BridgedBuilder::createStructExtract(BridgedValue str, NeXTCodeInt fieldIndex) const {
  nextcode::SILValue v = str.getSILValue();
  return {unbridged().createStructExtract(
      regularLoc(), v, v->getType().getFieldDecl(fieldIndex))};
}

BridgedInstruction BridgedBuilder::createStructElementAddr(BridgedValue addr, NeXTCodeInt fieldIndex) const {
  nextcode::SILValue v = addr.getSILValue();
  return {unbridged().createStructElementAddr(
      regularLoc(), v, v->getType().getFieldDecl(fieldIndex))};
}

BridgedInstruction BridgedBuilder::createDestructureStruct(BridgedValue str) const {
  return {unbridged().createDestructureStruct(regularLoc(), str.getSILValue())};
}

BridgedInstruction BridgedBuilder::createTuple(BridgedType type, BridgedValueArray elements) const {
  llvm::SmallVector<nextcode::SILValue, 16> elementValues;
  return {unbridged().createTuple(regularLoc(), type.unbridged(),
                                  elements.getValues(elementValues))};
}

BridgedInstruction BridgedBuilder::createTupleExtract(BridgedValue str, NeXTCodeInt elementIndex) const {
  nextcode::SILValue v = str.getSILValue();
  return {unbridged().createTupleExtract(regularLoc(), v, elementIndex)};
}

BridgedInstruction BridgedBuilder::createTupleElementAddr(BridgedValue addr, NeXTCodeInt elementIndex) const {
  nextcode::SILValue v = addr.getSILValue();
  return {unbridged().createTupleElementAddr(regularLoc(), v, elementIndex)};
}

BridgedInstruction BridgedBuilder::createDestructureTuple(BridgedValue str) const {
  return {unbridged().createDestructureTuple(regularLoc(), str.getSILValue())};
}

BridgedInstruction BridgedBuilder::createStore(BridgedValue src, BridgedValue dst,
                               NeXTCodeInt ownership) const {
  return {unbridged().createStore(regularLoc(), src.getSILValue(),
                                  dst.getSILValue(),
                                  (nextcode::StoreOwnershipQualifier)ownership)};
}

BridgedInstruction BridgedBuilder::createInitExistentialRef(BridgedValue instance,
                                            BridgedType type,
                                            BridgedInstruction useConformancesOf) const {
  auto *src = useConformancesOf.getAs<nextcode::InitExistentialRefInst>();
  return {unbridged().createInitExistentialRef(
      regularLoc(), type.unbridged(), src->getFormalConcreteType(),
      instance.getSILValue(), src->getConformances())};
}

BridgedInstruction BridgedBuilder::createInitExistentialMetatype(BridgedValue metatype,
                                            BridgedType existentialType,
                                            BridgedInstruction useConformancesOf) const {
  auto *src = useConformancesOf.getAs<nextcode::InitExistentialMetatypeInst>();
  return {unbridged().createInitExistentialMetatype(
      regularLoc(), metatype.getSILValue(), existentialType.unbridged(),
      src->getConformances())};
}

BridgedInstruction BridgedBuilder::createMetatype(BridgedType type,
                                                  BridgedType::MetatypeRepresentation representation) const {
  auto *mt =
      nextcode::MetatypeType::get(type.unbridged().getASTType(),
                               (nextcode::MetatypeRepresentation)representation);
  auto t = nextcode::SILType::getPrimitiveObjectType(nextcode::CanType(mt));
  return {unbridged().createMetatype(regularLoc(), t)};
}

BridgedInstruction BridgedBuilder::createEndCOWMutation(BridgedValue instance, bool keepUnique) const {
  return {unbridged().createEndCOWMutation(regularLoc(), instance.getSILValue(),
                                           keepUnique)};
}

BridgedInstruction BridgedBuilder::createMarkDependence(BridgedValue value, BridgedValue base, BridgedInstruction::MarkDependenceKind kind) const {
  return {unbridged().createMarkDependence(regularLoc(), value.getSILValue(), base.getSILValue(), nextcode::MarkDependenceKind(kind))};
}

BridgedInstruction BridgedBuilder::createEndAccess(BridgedValue value) const {
  return {unbridged().createEndAccess(regularLoc(), value.getSILValue(), false)};
}

BridgedInstruction BridgedBuilder::createConvertFunction(BridgedValue originalFunction, BridgedType resultType, bool withoutActuallyEscaping) const {
return {unbridged().createConvertFunction(regularLoc(), originalFunction.getSILValue(), resultType.unbridged(), withoutActuallyEscaping)};
}

BridgedInstruction BridgedBuilder::createConvertEscapeToNoEscape(BridgedValue originalFunction, BridgedType resultType, bool isLifetimeGuaranteed) const {
  return {unbridged().createConvertEscapeToNoEscape(regularLoc(), originalFunction.getSILValue(), resultType.unbridged(), isLifetimeGuaranteed)};
}

NEXTCODE_END_NULLABILITY_ANNOTATIONS

#endif
