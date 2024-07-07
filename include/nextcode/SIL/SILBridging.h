//===--- SILBridging.h - header for the nextcode SILBridging module ----------===//
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

#ifndef NEXTCODE_SIL_SILBRIDGING_H
#define NEXTCODE_SIL_SILBRIDGING_H

// Do not add other C++/llvm/nextcode header files here!
// Function implementations should be placed into SILBridgingImpl.h or SILBridging.cpp and
// required header files should be added there.
//
// Pure bridging mode does not permit including any C++/llvm/nextcode headers.
// See also the comments for `BRIDGING_MODE` in the top-level CMakeLists.txt file.
//
#include "nextcode/AST/ASTBridging.h"

#ifdef USED_IN_CPP_SOURCE
#include "llvm/ADT/ArrayRef.h"
#include "nextcode/SIL/SILBuilder.h"
#include "nextcode/SIL/SILDebugVariable.h"
#include "nextcode/SIL/SILDefaultWitnessTable.h"
#include "nextcode/SIL/SILFunctionConventions.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILWitnessTable.h"
#endif

NEXTCODE_BEGIN_NULLABILITY_ANNOTATIONS

struct BridgedInstruction;
struct OptionalBridgedInstruction;
struct OptionalBridgedOperand;
struct OptionalBridgedSuccessor;
struct BridgedFunction;
struct BridgedBasicBlock;
struct BridgedSuccessorArray;
struct OptionalBridgedBasicBlock;

namespace nextcode {
class ValueBase;
class Operand;
class ForwardingInstruction;
class SILFunction;
class SILBasicBlock;
class SILSuccessor;
class SILGlobalVariable;
class SILInstruction;
class SILArgument;
class MultipleValueInstructionResult;
class SILVTableEntry;
class SILVTable;
class SILWitnessTable;
class SILDefaultWitnessTable;
class NominalTypeDecl;
class VarDecl;
class TypeBase;
class NeXTCodePassInvocation;
class GenericSpecializationInformation;
class LifetimeDependenceInfo;
}

bool nextcodeModulesInitialized();
void registerBridgedClass(BridgedStringRef className, NeXTCodeMetatype metatype);

enum class BridgedResultConvention {
  Indirect,
  Owned,
  Unowned,
  UnownedInnerPointer,
  Autoreleased,
  Pack
};

struct BridgedResultInfo {
  nextcode::TypeBase * _Nonnull type;
  BridgedResultConvention convention;

#ifdef USED_IN_CPP_SOURCE
  inline static BridgedResultConvention
  castToResultConvention(nextcode::ResultConvention convention) {
    return static_cast<BridgedResultConvention>(convention);
  }

  BridgedResultInfo(nextcode::SILResultInfo resultInfo):
    type(resultInfo.getInterfaceType().getPointer()),
    convention(castToResultConvention(resultInfo.getConvention()))
  {}
#endif
};

struct OptionalBridgedResultInfo {
  nextcode::TypeBase * _Nullable type = nullptr;
  BridgedResultConvention convention = BridgedResultConvention::Indirect;

#ifdef USED_IN_CPP_SOURCE
  OptionalBridgedResultInfo(std::optional<nextcode::SILResultInfo> resultInfo) {
    if (resultInfo) {
      type = resultInfo->getInterfaceType().getPointer();
      convention =
        BridgedResultInfo::castToResultConvention(resultInfo->getConvention());
    }
  }
#endif
};

struct BridgedResultInfoArray {
  BridgedArrayRef resultInfoArray;

#ifdef USED_IN_CPP_SOURCE
  BridgedResultInfoArray(llvm::ArrayRef<nextcode::SILResultInfo> results)
    : resultInfoArray(results) {}

  llvm::ArrayRef<nextcode::SILResultInfo> unbridged() const {
    return resultInfoArray.unbridged<nextcode::SILResultInfo>();
  }
#endif

  BRIDGED_INLINE NeXTCodeInt count() const;

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE
  BridgedResultInfo at(NeXTCodeInt resultIndex) const;
};

// Unfortunately we need to take a detour over this enum.
// Currently it's not possible to switch over `SILArgumentConvention::ConventionType`,
// because it's not a class enum.
enum class BridgedArgumentConvention {
  Indirect_In,
  Indirect_In_Guaranteed,
  Indirect_Inout,
  Indirect_InoutAliasable,
  Indirect_In_CXX,
  Indirect_Out,
  Direct_Owned,
  Direct_Unowned,
  Direct_Guaranteed,
  Pack_Owned,
  Pack_Inout,
  Pack_Guaranteed,
  Pack_Out
};

#ifdef USED_IN_CPP_SOURCE
inline nextcode::ParameterConvention getParameterConvention(BridgedArgumentConvention convention) {
  switch (convention) {
    case BridgedArgumentConvention::Indirect_In:             return nextcode::ParameterConvention::Indirect_In;
    case BridgedArgumentConvention::Indirect_In_Guaranteed:  return nextcode::ParameterConvention::Indirect_In_Guaranteed;
    case BridgedArgumentConvention::Indirect_Inout:          return nextcode::ParameterConvention::Indirect_Inout;
    case BridgedArgumentConvention::Indirect_InoutAliasable: return nextcode::ParameterConvention::Indirect_InoutAliasable;
    case BridgedArgumentConvention::Indirect_In_CXX:         return nextcode::ParameterConvention::Indirect_In_CXX;
    case BridgedArgumentConvention::Indirect_Out:            break;
    case BridgedArgumentConvention::Direct_Owned:            return nextcode::ParameterConvention::Direct_Owned;
    case BridgedArgumentConvention::Direct_Unowned:          return nextcode::ParameterConvention::Direct_Unowned;
    case BridgedArgumentConvention::Direct_Guaranteed:       return nextcode::ParameterConvention::Direct_Guaranteed;
    case BridgedArgumentConvention::Pack_Owned:              return nextcode::ParameterConvention::Pack_Owned;
    case BridgedArgumentConvention::Pack_Inout:              return nextcode::ParameterConvention::Pack_Inout;
    case BridgedArgumentConvention::Pack_Guaranteed:         return nextcode::ParameterConvention::Pack_Guaranteed;
    case BridgedArgumentConvention::Pack_Out:                break;
  }
  llvm_unreachable("invalid parameter convention");
}

inline BridgedArgumentConvention getArgumentConvention(nextcode::ParameterConvention convention) {
  switch (convention) {
    case nextcode::ParameterConvention::Indirect_In:              return BridgedArgumentConvention::Indirect_In;
    case nextcode::ParameterConvention::Indirect_In_Guaranteed:   return BridgedArgumentConvention::Indirect_In_Guaranteed;
    case nextcode::ParameterConvention::Indirect_Inout:           return BridgedArgumentConvention::Indirect_Inout;
    case nextcode::ParameterConvention::Indirect_InoutAliasable:  return BridgedArgumentConvention::Indirect_InoutAliasable;
    case nextcode::ParameterConvention::Indirect_In_CXX:          return BridgedArgumentConvention::Indirect_In_CXX;
    case nextcode::ParameterConvention::Direct_Owned:             return BridgedArgumentConvention::Direct_Owned;
    case nextcode::ParameterConvention::Direct_Unowned:           return BridgedArgumentConvention::Direct_Unowned;
    case nextcode::ParameterConvention::Direct_Guaranteed:        return BridgedArgumentConvention::Direct_Guaranteed;
    case nextcode::ParameterConvention::Pack_Owned:               return BridgedArgumentConvention::Pack_Owned;
    case nextcode::ParameterConvention::Pack_Inout:               return BridgedArgumentConvention::Pack_Inout;
    case nextcode::ParameterConvention::Pack_Guaranteed:          return BridgedArgumentConvention::Pack_Guaranteed;
  }
  llvm_unreachable("invalid parameter convention");
}
#endif

struct BridgedParameterInfo {
  nextcode::TypeBase * _Nonnull type;
  BridgedArgumentConvention convention;
  uint8_t options;

  BridgedParameterInfo(nextcode::TypeBase * _Nonnull type, BridgedArgumentConvention convention, uint8_t options) :
    type(type), convention(convention), options(options) {}

#ifdef USED_IN_CPP_SOURCE
  inline static BridgedArgumentConvention
  castToArgumentConvention(nextcode::ParameterConvention convention) {
    return static_cast<BridgedArgumentConvention>(
      nextcode::SILArgumentConvention(convention).Value);
  }

  BridgedParameterInfo(nextcode::SILParameterInfo parameterInfo):
    type(parameterInfo.getInterfaceType().getPointer()),
    convention(castToArgumentConvention(parameterInfo.getConvention())),
    options(parameterInfo.getOptions().toRaw())
  {}

  nextcode::SILParameterInfo unbridged() const {
    return nextcode::SILParameterInfo(nextcode::CanType(type), getParameterConvention(convention),
                                   nextcode::SILParameterInfo::Options(options));
  }
#endif
};

struct BridgedParameterInfoArray {
  BridgedArrayRef parameterInfoArray;

#ifdef USED_IN_CPP_SOURCE
  BridgedParameterInfoArray(llvm::ArrayRef<nextcode::SILParameterInfo> parameters)
    : parameterInfoArray(parameters) {}

  llvm::ArrayRef<nextcode::SILParameterInfo> unbridged() const {
    return parameterInfoArray.unbridged<nextcode::SILParameterInfo>();
  }
#endif

  BRIDGED_INLINE NeXTCodeInt count() const;

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE
  BridgedParameterInfo at(NeXTCodeInt parameterIndex) const;
};

struct BridgedYieldInfoArray {
  BridgedArrayRef yieldInfoArray;

#ifdef USED_IN_CPP_SOURCE
  BridgedYieldInfoArray(llvm::ArrayRef<nextcode::SILYieldInfo> yields)
    : yieldInfoArray(yields) {}

  llvm::ArrayRef<nextcode::SILYieldInfo> unbridged() const {
    return yieldInfoArray.unbridged<nextcode::SILYieldInfo>();
  }
#endif

  BRIDGED_INLINE NeXTCodeInt count() const;

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE
  BridgedParameterInfo at(NeXTCodeInt resultIndex) const;
};

struct BridgedLifetimeDependenceInfo {
  const nextcode::LifetimeDependenceInfo * _Nullable info = nullptr;

  BRIDGED_INLINE bool empty() const;
  BRIDGED_INLINE bool checkInherit(NeXTCodeInt index) const;
  BRIDGED_INLINE bool checkScope(NeXTCodeInt index) const;

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedOwnedString getDebugDescription() const;
};

// Temporary access to the AST type within SIL until ASTBridging provides it.
struct BridgedASTType {
  nextcode::TypeBase * _Nullable type;

#ifdef USED_IN_CPP_SOURCE
  nextcode::Type unbridged() const {
    return type;
  }
#endif

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedOwnedString getDebugDescription() const;

  BRIDGED_INLINE bool hasTypeParameter() const;

  BRIDGED_INLINE bool isOpenedExistentialWithError() const;

  BRIDGED_INLINE bool isEscapable() const;

  BRIDGED_INLINE bool isNoEscape() const;

  inline bool mayEscape() const { return !isNoEscape() && isEscapable(); }

  // =========================================================================//
  //                              SILFunctionType
  // =========================================================================//

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE
  BridgedResultInfoArray SILFunctionType_getResultsWithError() const;

  BRIDGED_INLINE NeXTCodeInt
  SILFunctionType_getNumIndirectFormalResultsWithError() const;

  BRIDGED_INLINE NeXTCodeInt SILFunctionType_getNumPackResults() const;

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedResultInfo SILFunctionType_getErrorResult() const;

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE
  BridgedParameterInfoArray SILFunctionType_getParameters() const;

  BRIDGED_INLINE bool SILFunctionType_hasSelfParam() const;

  BRIDGED_INLINE bool SILFunctionType_isTrivialNoescape() const;

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE
  BridgedYieldInfoArray SILFunctionType_getYields() const;

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedLifetimeDependenceInfo SILFunctionType_getLifetimeDependenceInfo() const;
};

struct BridgedType {
  void * _Nullable opaqueValue;

  enum class MetatypeRepresentation {
    Thin,
    Thick,
    ObjC
  };

  enum class TraitResult {
    IsNot,
    CanBe,
    Is
  };

  struct EnumElementIterator {
    uint64_t storage[4];

#ifdef USED_IN_CPP_SOURCE
    EnumElementIterator(nextcode::EnumDecl::ElementRange::iterator i) {
      static_assert(sizeof(EnumElementIterator) >= sizeof(nextcode::EnumDecl::ElementRange::iterator));
      *reinterpret_cast<nextcode::EnumDecl::ElementRange::iterator *>(&storage) = i;
    }
    nextcode::EnumDecl::ElementRange::iterator unbridged() const {
      return *reinterpret_cast<const nextcode::EnumDecl::ElementRange::iterator *>(&storage);
    }
#endif

    NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE EnumElementIterator getNext() const;
  };

#ifdef USED_IN_CPP_SOURCE
  BridgedType(nextcode::SILType t) : opaqueValue(t.getOpaqueValue()) {}

  nextcode::SILType unbridged() const {
    return nextcode::SILType::getFromOpaqueValue(opaqueValue);
  }
#endif

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedOwnedString getDebugDescription() const;
  BRIDGED_INLINE bool isNull() const;
  BRIDGED_INLINE bool isAddress() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedType getAddressType() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedType getObjectType() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedASTType getASTType() const;
  BRIDGED_INLINE bool isTrivial(BridgedFunction f) const;
  BRIDGED_INLINE bool isNonTrivialOrContainsRawPointer(BridgedFunction f) const;
  BRIDGED_INLINE bool isValueTypeWithDeinit() const;
  BRIDGED_INLINE bool isLoadable(BridgedFunction f) const;
  BRIDGED_INLINE bool isReferenceCounted(BridgedFunction f) const;
  BRIDGED_INLINE bool isUnownedStorageType() const;
  BRIDGED_INLINE bool hasArchetype() const;
  BRIDGED_INLINE bool isNominalOrBoundGenericNominal() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedNominalTypeDecl getNominalOrBoundGenericNominal() const;
  BRIDGED_INLINE bool isClassOrBoundGenericClass() const;
  BRIDGED_INLINE bool isStructOrBoundGenericStruct() const;
  BRIDGED_INLINE bool isTuple() const;
  BRIDGED_INLINE bool isEnumOrBoundGenericEnum() const;
  BRIDGED_INLINE bool isFunction() const;
  BRIDGED_INLINE bool isMetatype() const;
  BRIDGED_INLINE bool isNoEscapeFunction() const;
  BRIDGED_INLINE bool containsNoEscapeFunction() const;
  BRIDGED_INLINE bool isThickFunction() const;
  BRIDGED_INLINE bool isAsyncFunction() const;
  BRIDGED_INLINE bool isEmpty(BridgedFunction f) const;
  BRIDGED_INLINE TraitResult canBeClass() const;
  BRIDGED_INLINE bool isMoveOnly() const;
  BRIDGED_INLINE bool isEscapable(BridgedFunction f) const;
  BRIDGED_INLINE bool isOrContainsObjectiveCClass() const;
  BRIDGED_INLINE bool isBuiltinInteger() const;
  BRIDGED_INLINE bool isBuiltinFloat() const;
  BRIDGED_INLINE bool isBuiltinVector() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedType getBuiltinVectorElementType() const;
  BRIDGED_INLINE bool isBuiltinFixedWidthInteger(NeXTCodeInt width) const;
  BRIDGED_INLINE bool isExactSuperclassOf(BridgedType t) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedType getInstanceTypeOfMetatype(BridgedFunction f) const;
  BRIDGED_INLINE bool isDynamicSelfMetatype() const;
  BRIDGED_INLINE MetatypeRepresentation getRepresentationOfMetatype(BridgedFunction f) const;
  BRIDGED_INLINE bool isCalleeConsumedFunction() const;
  BRIDGED_INLINE bool isMarkedAsImmortal() const;
  BRIDGED_INLINE NeXTCodeInt getCaseIdxOfEnumType(BridgedStringRef name) const;
  BRIDGED_INLINE NeXTCodeInt getNumNominalFields() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedType getFieldType(NeXTCodeInt idx, BridgedFunction f) const;
  BRIDGED_INLINE NeXTCodeInt getFieldIdxOfNominalType(BridgedStringRef name) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedStringRef getFieldName(NeXTCodeInt idx) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE EnumElementIterator getFirstEnumCaseIterator() const;
  BRIDGED_INLINE bool isEndCaseIterator(EnumElementIterator i) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedType getEnumCasePayload(EnumElementIterator i, BridgedFunction f) const;
  BRIDGED_INLINE NeXTCodeInt getNumTupleElements() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedType
  getTupleElementType(NeXTCodeInt idx) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedType getFunctionTypeWithNoEscape(bool withNoEscape) const;
  BRIDGED_INLINE BridgedArgumentConvention getCalleeConvention() const;
};

// SIL Bridging

struct BridgedValue {
  NeXTCodeObject obj;

  enum class Kind {
    SingleValueInstruction,
    Argument,
    MultipleValueInstructionResult,
    Undef
  };

  // Unfortunately we need to take a detour over this enum.
  // Currently it's not possible to switch over `OwnershipKind::inntery`, because it's not a class enum.
  enum class Ownership {
    Unowned,
    Owned,
    Guaranteed,
    None
  };

#ifdef USED_IN_CPP_SOURCE
  static nextcode::ValueOwnershipKind castToOwnership(BridgedValue::Ownership ownership) {
    switch (ownership) {
      case BridgedValue::Ownership::Unowned:    return nextcode::OwnershipKind::Unowned;
      case BridgedValue::Ownership::Owned:      return nextcode::OwnershipKind::Owned;
      case BridgedValue::Ownership::Guaranteed: return nextcode::OwnershipKind::Guaranteed;
      case BridgedValue::Ownership::None:       return nextcode::OwnershipKind::None;
    }
  }
#endif

  Kind getKind() const;
  BRIDGED_INLINE nextcode::ValueBase * _Nonnull getSILValue() const;
  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString getDebugDescription() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedOperand getFirstUse() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedType getType() const;
  BRIDGED_INLINE Ownership getOwnership() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedFunction SILUndef_getParentFunction() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedFunction PlaceholderValue_getParentFunction() const;

  bool findPointerEscape() const;
};

struct OptionalBridgedValue {
  OptionalNeXTCodeObject obj;

  BRIDGED_INLINE nextcode::ValueBase * _Nullable getSILValue() const;
};

// This is the layout of a class existential.
struct BridgeValueExistential {
  BridgedValue value;
  void * _Nonnull conformance;
};

struct BridgedValueArray {
  const BridgeValueExistential * _Nullable base;
  size_t count;

#ifdef USED_IN_CPP_SOURCE
  llvm::ArrayRef<nextcode::SILValue> getValues(llvm::SmallVectorImpl<nextcode::SILValue> &storage);
#endif
};

struct BridgedOperand {
  nextcode::Operand * _Nonnull op;

  enum class OperandOwnership {
    NonUse,
    TrivialUse,
    InstantaneousUse,
    UnownedInstantaneousUse,
    ForwardingUnowned,
    PointerEscape,
    BitwiseEscape,
    Borrow,
    DestroyingConsume,
    ForwardingConsume,
    InteriorPointer,
    GuaranteedForwarding,
    EndBorrow,
    Reborrow
  };

  BRIDGED_INLINE bool isTypeDependent() const;
  BRIDGED_INLINE bool isLifetimeEnding() const;
  BRIDGED_INLINE bool canAcceptOwnership(BridgedValue::Ownership ownership) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedOperand getNextUse() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedValue getValue() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction getUser() const;
  BRIDGED_INLINE OperandOwnership getOperandOwnership() const;
};

struct OptionalBridgedOperand {
  nextcode::Operand * _Nullable op;

  // Assumes that `op` is not null.
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE
  BridgedOperand advancedBy(NeXTCodeInt index) const;

  // Assumes that `op` is not null.
  BRIDGED_INLINE NeXTCodeInt distanceTo(BridgedOperand element) const;
};

struct BridgedOperandArray {
  OptionalBridgedOperand base;
  NeXTCodeInt count;
};

enum class BridgedMemoryBehavior {
  None,
  MayRead,
  MayWrite,
  MayReadWrite,
  MayHaveSideEffects
};

struct BridgedLocation {
  uint64_t storage[3];

#ifdef USED_IN_CPP_SOURCE
  BridgedLocation(const nextcode::SILDebugLocation &loc) {
    *reinterpret_cast<nextcode::SILDebugLocation *>(&storage) = loc;
  }
  const nextcode::SILDebugLocation &getLoc() const {
    return *reinterpret_cast<const nextcode::SILDebugLocation *>(&storage);
  }
#endif

  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString getDebugDescription() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedLocation getAutogeneratedLocation() const;
  BRIDGED_INLINE bool hasValidLineNumber() const;
  BRIDGED_INLINE bool isAutoGenerated() const;
  BRIDGED_INLINE bool isInlined() const;
  BRIDGED_INLINE bool isEqualTo(BridgedLocation rhs) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedSourceLoc getSourceLocation() const;
  BRIDGED_INLINE bool hasSameSourceLocation(BridgedLocation rhs) const;
  static BRIDGED_INLINE BridgedLocation getArtificialUnreachableLocation();
};

struct BridgedFunction {
  NeXTCodeObject obj;

  enum class EffectsKind {
    ReadNone,
    ReadOnly,
    ReleaseNone,
    ReadWrite,
    Unspecified,
    Custom
  };

  enum class PerformanceConstraints {
    None = 0,
    NoAllocation = 1,
    NoLocks = 2,
    NoRuntime = 3,
    NoExistentials = 4,
    NoObjCBridging = 5
  };

  enum class InlineStrategy {
    InlineDefault,
    NoInline,
    AlwaysInline
  };

  enum class ThunkKind {
    IsNotThunk,
    IsThunk,
    IsReabstractionThunk,
    IsSignatureOptimizedThunk
  };

  enum class SerializedKind {
    IsNotSerialized,
    IsSerialized,
    IsSerializedForPackage
  };

  enum class Linkage {
    Public,
    PublicNonABI,
    Package,
    PackageNonABI,
    Hidden,
    Shared,
    Private,
    PublicExternal,
    PackageExternal,
    HiddenExternal
  };

  NEXTCODE_NAME("init(obj:)") 
  NEXTCODE_IMPORT_UNSAFE BridgedFunction(NeXTCodeObject obj) : obj(obj) {}
  NEXTCODE_IMPORT_UNSAFE BridgedFunction() {}
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE nextcode::SILFunction * _Nonnull getFunction() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedStringRef getName() const;
  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString getDebugDescription() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedLocation getLocation() const;
  BRIDGED_INLINE bool hasOwnership() const;
  BRIDGED_INLINE bool hasLoweredAddresses() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedASTType getLoweredFunctionTypeInContext() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedBasicBlock getFirstBlock() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedBasicBlock getLastBlock() const;
  BRIDGED_INLINE NeXTCodeInt getNumIndirectFormalResults() const;
  BRIDGED_INLINE bool hasIndirectErrorResult() const;
  BRIDGED_INLINE NeXTCodeInt getNumSILArguments() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedType getSILArgumentType(NeXTCodeInt idx) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedType getSILResultType() const;
  BRIDGED_INLINE bool isNeXTCode51RuntimeAvailable() const;
  BRIDGED_INLINE bool isPossiblyUsedExternally() const;
  BRIDGED_INLINE bool isAvailableExternally() const;
  BRIDGED_INLINE bool isTransparent() const;
  BRIDGED_INLINE bool isAsync() const;
  BRIDGED_INLINE bool isGlobalInitFunction() const;
  BRIDGED_INLINE bool isGlobalInitOnceFunction() const;
  BRIDGED_INLINE bool isDestructor() const;
  BRIDGED_INLINE bool isGeneric() const;
  BRIDGED_INLINE bool hasSemanticsAttr(BridgedStringRef attrName) const;
  BRIDGED_INLINE bool hasUnsafeNonEscapableResult() const;
  BRIDGED_INLINE bool hasDynamicSelfMetadata() const;
  BRIDGED_INLINE EffectsKind getEffectAttribute() const;
  BRIDGED_INLINE PerformanceConstraints getPerformanceConstraints() const;
  BRIDGED_INLINE InlineStrategy getInlineStrategy() const;
  BRIDGED_INLINE bool isSerialized() const;
  BRIDGED_INLINE bool isAnySerialized() const;
  BRIDGED_INLINE SerializedKind getSerializedKind() const;
  BRIDGED_INLINE bool canBeInlinedIntoCaller(SerializedKind) const;
  BRIDGED_INLINE bool hasValidLinkageForFragileRef(SerializedKind) const;
  BRIDGED_INLINE ThunkKind isThunk() const;
  BRIDGED_INLINE void setThunk(ThunkKind) const;
  BRIDGED_INLINE bool needsStackProtection() const;
  BRIDGED_INLINE void setNeedStackProtection(bool needSP) const;
  BRIDGED_INLINE void setIsPerformanceConstraint(bool isPerfConstraint) const;
  BRIDGED_INLINE bool isResilientNominalDecl(BridgedNominalTypeDecl decl) const;
  BRIDGED_INLINE BridgedType getLoweredType(BridgedASTType type) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedType getLoweredType(BridgedType type) const;
  BRIDGED_INLINE void setLinkage(Linkage linkage) const;
  bool isTrapNoReturn() const;
  bool isAutodiffVJP() const;
  NeXTCodeInt specializationLevel() const;

  enum class ParseEffectsMode {
    argumentEffectsFromSource,
    argumentEffectsFromSIL,
    globalEffectsFromSIL,
    multipleEffectsFromSIL
  };

  struct ParsingError {
    const unsigned char * _Nullable message;
    NeXTCodeInt position;
  };

  struct EffectInfo {
    NeXTCodeInt argumentIndex;
    bool isDerived;
    bool isEmpty;
    bool isValid;
  };

  typedef void (* _Nonnull RegisterFn)(BridgedFunction f, void * _Nonnull data, NeXTCodeInt size);
  typedef void (* _Nonnull WriteFn)(BridgedFunction, BridgedOStream, NeXTCodeInt);
  typedef ParsingError (*_Nonnull ParseFn)(BridgedFunction,
                                           BridgedStringRef,
                                           ParseEffectsMode, NeXTCodeInt,
                                           BridgedArrayRef);
  typedef NeXTCodeInt (* _Nonnull CopyEffectsFn)(BridgedFunction, BridgedFunction);
  typedef EffectInfo (* _Nonnull GetEffectInfoFn)(BridgedFunction, NeXTCodeInt);
  typedef BridgedMemoryBehavior (* _Nonnull GetMemBehaviorFn)(BridgedFunction, bool);
  typedef bool (* _Nonnull ArgumentMayReadFn)(BridgedFunction, BridgedOperand, BridgedValue);

  static void registerBridging(NeXTCodeMetatype metatype,
              RegisterFn initFn, RegisterFn destroyFn,
              WriteFn writeFn, ParseFn parseFn,
              CopyEffectsFn copyEffectsFn,
              GetEffectInfoFn effectInfoFn,
              GetMemBehaviorFn memBehaviorFn,
              ArgumentMayReadFn argumentMayReadFn);
};

struct OptionalBridgedFunction {
  OptionalNeXTCodeObject obj;
};

struct BridgedGlobalVar {
  NeXTCodeObject obj;

  BridgedGlobalVar(NeXTCodeObject obj) : obj(obj) {}
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE nextcode::SILGlobalVariable * _Nonnull getGlobal() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedNullableVarDecl getDecl() const;
  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString getDebugDescription() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedStringRef getName() const;
  BRIDGED_INLINE bool isLet() const;
  BRIDGED_INLINE void setLet(bool value) const;
  BRIDGED_INLINE bool isPossiblyUsedExternally() const;
  BRIDGED_INLINE bool isAvailableExternally() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedInstruction getFirstStaticInitInst() const;
  bool canBeInitializedStatically() const;
  bool mustBeInitializedStatically() const;
};

struct OptionalBridgedGlobalVar {
  OptionalNeXTCodeObject obj;
};

struct BridgedMultiValueResult {
  NeXTCodeObject obj;

#ifdef USED_IN_CPP_SOURCE
  nextcode::MultipleValueInstructionResult * _Nonnull unbridged() const {
    return static_cast<nextcode::MultipleValueInstructionResult *>(obj);
  }
#endif

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction getParent() const;
  BRIDGED_INLINE NeXTCodeInt getIndex() const;
};

struct BridgedSubstitutionMap {
  uint64_t storage[1];

#ifdef USED_IN_CPP_SOURCE
  BridgedSubstitutionMap(nextcode::SubstitutionMap map) {
    *reinterpret_cast<nextcode::SubstitutionMap *>(&storage) = map;
  }
  nextcode::SubstitutionMap unbridged() const {
    return *reinterpret_cast<const nextcode::SubstitutionMap *>(&storage);
  }
#endif

  BRIDGED_INLINE BridgedSubstitutionMap();
  BRIDGED_INLINE bool isEmpty() const;
  BRIDGED_INLINE bool hasAnySubstitutableParams() const;
};

struct BridgedTypeArray {
  BridgedArrayRef typeArray;

#ifdef USED_IN_CPP_SOURCE
  BridgedTypeArray(llvm::ArrayRef<nextcode::Type> types) : typeArray(types) {}

  llvm::ArrayRef<nextcode::Type> unbridged() const {
    return typeArray.unbridged<nextcode::Type>();
  }
#endif

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE
  static BridgedTypeArray fromReplacementTypes(BridgedSubstitutionMap substMap);

  NeXTCodeInt getCount() const { return NeXTCodeInt(typeArray.Length); }

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE
  BridgedType getAt(NeXTCodeInt index) const;
};

struct BridgedSILTypeArray {
  BridgedArrayRef typeArray;

#ifdef USED_IN_CPP_SOURCE
  BridgedSILTypeArray(llvm::ArrayRef<nextcode::SILType> silTypes)
      : typeArray(silTypes) {}

  llvm::ArrayRef<nextcode::SILType> unbridged() const {
    return typeArray.unbridged<nextcode::SILType>();
  }
#endif

  NeXTCodeInt getCount() const { return NeXTCodeInt(typeArray.Length); }

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE
  BridgedType getAt(NeXTCodeInt index) const;
};

struct BridgedGenericSpecializationInformation {
  const nextcode::GenericSpecializationInformation * _Nullable data = nullptr;
};

struct OptionalBridgedSILDebugVariable {
  uint64_t storage[16];

#ifdef USED_IN_CPP_SOURCE
  using OptionalSILDebugVariable = std::optional<nextcode::SILDebugVariable>;

  OptionalBridgedSILDebugVariable(
    OptionalSILDebugVariable &&debugVariable) {
    static_assert(sizeof(OptionalSILDebugVariable) <= 16*8);
    *reinterpret_cast<OptionalSILDebugVariable *>(&storage) = debugVariable;
  }
  const OptionalSILDebugVariable &getDebugVar() const {
    return *reinterpret_cast<const OptionalSILDebugVariable *>(&storage);
  }
#endif
};

struct BridgedInstruction {
  NeXTCodeObject obj;

#ifdef USED_IN_CPP_SOURCE
  template <class I> I *_Nonnull getAs() const {
    return llvm::cast<I>(static_cast<nextcode::SILNode *>(obj)->castToInstruction());
  }
  nextcode::SILInstruction * _Nonnull unbridged() const {
    return getAs<nextcode::SILInstruction>();
  }
#endif

  BridgedInstruction(NeXTCodeObject obj) : obj(obj) {}
  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString getDebugDescription() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedInstruction getNext() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedInstruction getPrevious() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedBasicBlock getParent() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction getLastInstOfParent() const;
  BRIDGED_INLINE bool isDeleted() const;
  BRIDGED_INLINE bool isInStaticInitializer() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedOperandArray getOperands() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedOperandArray getTypeDependentOperands() const;
  BRIDGED_INLINE void setOperand(NeXTCodeInt index, BridgedValue value) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedLocation getLocation() const;
  BRIDGED_INLINE BridgedMemoryBehavior getMemBehavior() const;
  BRIDGED_INLINE bool mayRelease() const;
  BRIDGED_INLINE bool mayHaveSideEffects() const;
  BRIDGED_INLINE bool maySuspend() const;
  bool mayAccessPointer() const;
  bool mayLoadWeakOrUnowned() const;
  bool maySynchronize() const;
  bool mayBeDeinitBarrierNotConsideringSideEffects() const;
  BRIDGED_INLINE bool shouldBeForwarding() const;

  // =========================================================================//
  //                   Generalized instruction subclasses
  // =========================================================================//
  
  BRIDGED_INLINE NeXTCodeInt MultipleValueInstruction_getNumResults() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedMultiValueResult MultipleValueInstruction_getResult(NeXTCodeInt index) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedSuccessorArray TermInst_getSuccessors() const;

  // =========================================================================//
  //                         Instruction protocols
  // =========================================================================//

  BRIDGED_INLINE nextcode::ForwardingInstruction * _Nonnull getAsForwardingInstruction() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedOperand ForwardingInst_singleForwardedOperand() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedOperandArray ForwardingInst_forwardedOperands() const;
  BRIDGED_INLINE BridgedValue::Ownership ForwardingInst_forwardingOwnership() const;
  BRIDGED_INLINE void ForwardingInst_setForwardingOwnership(BridgedValue::Ownership ownership) const;
  BRIDGED_INLINE bool ForwardingInst_preservesOwnership() const;

  // =========================================================================//
  //                    Specific instruction subclasses
  // =========================================================================//

  enum class BuiltinValueKind {
    None = 0,
#define BUILTIN(Id, Name, Attrs) Id,
#include "nextcode/AST/Builtins.def"
  };

  enum class IntrinsicID {
    memcpy, memmove,
    unknown
  };

  struct OptionalInt {
    NeXTCodeInt value;
    bool hasValue;
  };

  enum class MarkDependenceKind {
    Unresolved, Escaping, NonEscaping
  };

  struct KeyPathFunctionResults {
    enum { maxFunctions = 5 };
    BridgedFunction functions[maxFunctions];
    NeXTCodeInt numFunctions;
  };

  enum class CastConsumptionKind {
    TakeAlways,
    TakeOnSuccess,
    CopyOnSuccess
  };

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedStringRef CondFailInst_getMessage() const;
  BRIDGED_INLINE NeXTCodeInt LoadInst_getLoadOwnership() const ;
  BRIDGED_INLINE BuiltinValueKind BuiltinInst_getID() const;
  BRIDGED_INLINE IntrinsicID BuiltinInst_getIntrinsicID() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedSubstitutionMap BuiltinInst_getSubstitutionMap() const;
  BRIDGED_INLINE bool PointerToAddressInst_isStrict() const;
  BRIDGED_INLINE bool AddressToPointerInst_needsStackProtection() const;
  BRIDGED_INLINE bool IndexAddrInst_needsStackProtection() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedGlobalVar GlobalAccessInst_getGlobal() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedGlobalVar AllocGlobalInst_getGlobal() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedFunction FunctionRefBaseInst_getReferencedFunction() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalInt IntegerLiteralInst_getValue() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedStringRef StringLiteralInst_getValue() const;
  BRIDGED_INLINE int StringLiteralInst_getEncoding() const;
  BRIDGED_INLINE NeXTCodeInt TupleExtractInst_fieldIndex() const;
  BRIDGED_INLINE NeXTCodeInt TupleElementAddrInst_fieldIndex() const;
  BRIDGED_INLINE NeXTCodeInt StructExtractInst_fieldIndex() const;
  BRIDGED_INLINE OptionalBridgedValue StructInst_getUniqueNonTrivialFieldValue() const;
  BRIDGED_INLINE NeXTCodeInt StructElementAddrInst_fieldIndex() const;
  BRIDGED_INLINE bool BeginBorrow_isLexical() const;
  BRIDGED_INLINE bool BeginBorrow_isFromVarDecl() const;
  BRIDGED_INLINE bool MoveValue_isLexical() const;
  BRIDGED_INLINE bool MoveValue_isFromVarDecl() const;

  BRIDGED_INLINE NeXTCodeInt ProjectBoxInst_fieldIndex() const;
  BRIDGED_INLINE bool EndCOWMutationInst_doKeepUnique() const;
  BRIDGED_INLINE NeXTCodeInt EnumInst_caseIndex() const;
  BRIDGED_INLINE NeXTCodeInt UncheckedEnumDataInst_caseIndex() const;
  BRIDGED_INLINE NeXTCodeInt InitEnumDataAddrInst_caseIndex() const;
  BRIDGED_INLINE NeXTCodeInt UncheckedTakeEnumDataAddrInst_caseIndex() const;
  BRIDGED_INLINE NeXTCodeInt InjectEnumAddrInst_caseIndex() const;
  BRIDGED_INLINE NeXTCodeInt RefElementAddrInst_fieldIndex() const;
  BRIDGED_INLINE bool RefElementAddrInst_fieldIsLet() const;
  BRIDGED_INLINE bool RefElementAddrInst_isImmutable() const;
  BRIDGED_INLINE void RefElementAddrInst_setImmutable(bool isImmutable) const;
  BRIDGED_INLINE NeXTCodeInt PartialApplyInst_numArguments() const;
  BRIDGED_INLINE NeXTCodeInt ApplyInst_numArguments() const;
  BRIDGED_INLINE bool ApplyInst_getNonThrowing() const;
  BRIDGED_INLINE bool ApplyInst_getNonAsync() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedGenericSpecializationInformation ApplyInst_getSpecializationInfo() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedGenericSpecializationInformation TryApplyInst_getSpecializationInfo() const;
  BRIDGED_INLINE NeXTCodeInt ObjectInst_getNumBaseElements() const;
  BRIDGED_INLINE NeXTCodeInt PartialApply_getCalleeArgIndexOfFirstAppliedArg() const;
  BRIDGED_INLINE bool PartialApplyInst_isOnStack() const;
  BRIDGED_INLINE bool PartialApplyInst_hasUnknownResultIsolation() const;
  BRIDGED_INLINE bool AllocStackInst_hasDynamicLifetime() const;
  BRIDGED_INLINE bool AllocRefInstBase_isObjc() const;
  BRIDGED_INLINE bool AllocRefInstBase_canAllocOnStack() const;
  BRIDGED_INLINE NeXTCodeInt AllocRefInstBase_getNumTailTypes() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedSILTypeArray AllocRefInstBase_getTailAllocatedTypes() const;
  BRIDGED_INLINE bool AllocRefDynamicInst_isDynamicTypeDeinitAndSizeKnownEquivalentToBaseType() const;
  BRIDGED_INLINE NeXTCodeInt BeginApplyInst_numArguments() const;
  BRIDGED_INLINE NeXTCodeInt TryApplyInst_numArguments() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedBasicBlock BranchInst_getTargetBlock() const;
  BRIDGED_INLINE NeXTCodeInt SwitchEnumInst_getNumCases() const;
  BRIDGED_INLINE NeXTCodeInt SwitchEnumInst_getCaseIndex(NeXTCodeInt idx) const;
  BRIDGED_INLINE NeXTCodeInt StoreInst_getStoreOwnership() const;
  BRIDGED_INLINE NeXTCodeInt AssignInst_getAssignOwnership() const;
  BRIDGED_INLINE MarkDependenceKind MarkDependenceInst_dependenceKind() const;
  BRIDGED_INLINE void MarkDependenceInst_resolveToNonEscaping() const;
  BRIDGED_INLINE NeXTCodeInt BeginAccessInst_getAccessKind() const;
  BRIDGED_INLINE bool BeginAccessInst_isStatic() const;
  BRIDGED_INLINE bool CopyAddrInst_isTakeOfSrc() const;
  BRIDGED_INLINE bool CopyAddrInst_isInitializationOfDest() const;
  BRIDGED_INLINE bool ExplicitCopyAddrInst_isTakeOfSrc() const;
  BRIDGED_INLINE bool ExplicitCopyAddrInst_isInitializationOfDest() const;
  BRIDGED_INLINE NeXTCodeInt MarkUninitializedInst_getKind() const;
  BRIDGED_INLINE void RefCountingInst_setIsAtomic(bool isAtomic) const;
  BRIDGED_INLINE bool RefCountingInst_getIsAtomic() const;
  BRIDGED_INLINE NeXTCodeInt CondBranchInst_getNumTrueArgs() const;
  BRIDGED_INLINE void AllocRefInstBase_setIsStackAllocatable() const;
  BRIDGED_INLINE bool AllocRefInst_isBare() const;
  BRIDGED_INLINE void AllocRefInst_setIsBare() const;
  BRIDGED_INLINE void TermInst_replaceBranchTarget(BridgedBasicBlock from, BridgedBasicBlock to) const;
  BRIDGED_INLINE NeXTCodeInt KeyPathInst_getNumComponents() const;
  BRIDGED_INLINE void KeyPathInst_getReferencedFunctions(NeXTCodeInt componentIdx, KeyPathFunctionResults * _Nonnull results) const;
  BRIDGED_INLINE void GlobalAddrInst_clearToken() const;
  BRIDGED_INLINE bool GlobalValueInst_isBare() const;
  BRIDGED_INLINE void GlobalValueInst_setIsBare() const;
  BRIDGED_INLINE void LoadInst_setOwnership(NeXTCodeInt ownership) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedBasicBlock CheckedCastBranch_getSuccessBlock() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedBasicBlock CheckedCastBranch_getFailureBlock() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedBasicBlock CheckedCastAddrBranch_getSuccessBlock() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedBasicBlock CheckedCastAddrBranch_getFailureBlock() const;
  BRIDGED_INLINE CastConsumptionKind CheckedCastAddrBranch_getConsumptionKind() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedSubstitutionMap ApplySite_getSubstitutionMap() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedASTType ApplySite_getSubstitutedCalleeType() const;
  BRIDGED_INLINE NeXTCodeInt ApplySite_getNumArguments() const;
  BRIDGED_INLINE bool ApplySite_isCalleeNoReturn() const;
  BRIDGED_INLINE NeXTCodeInt FullApplySite_numIndirectResultArguments() const;
  BRIDGED_INLINE bool ConvertFunctionInst_withoutActuallyEscaping() const;

  // =========================================================================//
  //                   VarDeclInst and DebugVariableInst
  // =========================================================================//

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedNullableVarDecl
  DebugValue_getDecl() const;

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedNullableVarDecl
  AllocStack_getDecl() const;

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedNullableVarDecl
  AllocBox_getDecl() const;

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedNullableVarDecl
  GlobalAddr_getDecl() const;

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedNullableVarDecl
  RefElementAddr_getDecl() const;

  BRIDGED_INLINE OptionalBridgedSILDebugVariable DebugValue_getVarInfo() const;

  BRIDGED_INLINE OptionalBridgedSILDebugVariable AllocStack_getVarInfo() const;

  BRIDGED_INLINE OptionalBridgedSILDebugVariable AllocBox_getVarInfo() const;
};

struct OptionalBridgedInstruction {
  OptionalNeXTCodeObject obj;

#ifdef USED_IN_CPP_SOURCE
  nextcode::SILInstruction * _Nullable unbridged() const {
    if (!obj)
      return nullptr;
    return llvm::cast<nextcode::SILInstruction>(static_cast<nextcode::SILNode *>(obj)->castToInstruction());
  }
#endif

  OptionalBridgedInstruction() : obj(nullptr) {}
  OptionalBridgedInstruction(OptionalNeXTCodeObject obj) : obj(obj) {}
};

struct BridgedArgument {
  NeXTCodeObject obj;

  BRIDGED_INLINE nextcode::SILArgument * _Nonnull getArgument() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedBasicBlock getParent() const;
  BRIDGED_INLINE bool isReborrow() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedNullableVarDecl getVarDecl() const;
  BRIDGED_INLINE void copyFlags(BridgedArgument fromArgument) const;
};

struct OptionalBridgedArgument {
  OptionalNeXTCodeObject obj;
};

struct OptionalBridgedBasicBlock {
  OptionalNeXTCodeObject obj;

#ifdef USED_IN_CPP_SOURCE
  nextcode::SILBasicBlock * _Nullable unbridged() const {
    return obj ? static_cast<nextcode::SILBasicBlock *>(obj) : nullptr;
  }
#endif
};

struct BridgedBasicBlock {
  NeXTCodeObject obj;

  BridgedBasicBlock(NeXTCodeObject obj) : obj(obj) {
  }

#ifdef USED_IN_CPP_SOURCE
  BridgedBasicBlock(nextcode::SILBasicBlock * _Nonnull block) : obj(block) {
  }
  nextcode::SILBasicBlock * _Nonnull unbridged() const {
    return static_cast<nextcode::SILBasicBlock *>(obj);
  }
#endif

  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString getDebugDescription() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedBasicBlock getNext() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedBasicBlock getPrevious() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedFunction getFunction() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedInstruction getFirstInst() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedInstruction getLastInst() const;
  BRIDGED_INLINE NeXTCodeInt getNumArguments() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedArgument getArgument(NeXTCodeInt index) const;
  NEXTCODE_IMPORT_UNSAFE  BRIDGED_INLINE BridgedArgument addBlockArgument(BridgedType type, BridgedValue::Ownership ownership) const;
  NEXTCODE_IMPORT_UNSAFE  BRIDGED_INLINE BridgedArgument addFunctionArgument(BridgedType type) const;
  BRIDGED_INLINE void eraseArgument(NeXTCodeInt index) const;
  BRIDGED_INLINE void moveAllInstructionsToBegin(BridgedBasicBlock dest) const;
  BRIDGED_INLINE void moveAllInstructionsToEnd(BridgedBasicBlock dest) const;
  BRIDGED_INLINE void moveArgumentsTo(BridgedBasicBlock dest) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedSuccessor getFirstPred() const;
};

struct BridgedSuccessor {
  const nextcode::SILSuccessor * _Nonnull succ;

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedSuccessor getNext() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedBasicBlock getTargetBlock() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE  BridgedInstruction getContainingInst() const;
};

struct OptionalBridgedSuccessor {
  const nextcode::SILSuccessor * _Nullable succ;

  // Assumes that `succ` is not null.
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedSuccessor advancedBy(NeXTCodeInt index) const;
};

struct BridgedSuccessorArray {
  OptionalBridgedSuccessor base;
  NeXTCodeInt count;
};

struct BridgedVTableEntry {
  const nextcode::SILVTableEntry * _Nonnull entry;

  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString getDebugDescription() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedFunction getImplementation() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedVTableEntry advanceBy(NeXTCodeInt index) const;
};

struct BridgedVTableEntryArray {
  BridgedVTableEntry base;
  NeXTCodeInt count;
};

struct BridgedVTable {
  const nextcode::SILVTable * _Nonnull vTable;

  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString getDebugDescription() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedVTableEntryArray getEntries() const;
};

struct BridgedWitnessTableEntry {
  const void * _Nonnull entry;

  enum class Kind {
    Invalid,
    Method,
    AssociatedType,
    AssociatedTypeProtocol,
    BaseProtocol
  };

#ifdef USED_IN_CPP_SOURCE
  const nextcode::SILWitnessTable::Entry * _Nonnull getEntry() const {
    return (const nextcode::SILWitnessTable::Entry * _Nonnull)entry;
  }
#endif

  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString getDebugDescription() const;
  BRIDGED_INLINE Kind getKind() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedFunction getMethodFunction() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedWitnessTableEntry advanceBy(NeXTCodeInt index) const;
};

struct BridgedWitnessTableEntryArray {
  BridgedWitnessTableEntry base;
  NeXTCodeInt count;
};

struct BridgedWitnessTable {
  const nextcode::SILWitnessTable * _Nonnull table;

  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString getDebugDescription() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedWitnessTableEntryArray getEntries() const;
};

struct OptionalBridgedWitnessTable {
  const nextcode::SILWitnessTable * _Nullable table;
};

struct BridgedDefaultWitnessTable {
  const nextcode::SILDefaultWitnessTable * _Nonnull table;

  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString getDebugDescription() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedWitnessTableEntryArray getEntries() const;
};

struct OptionalBridgedDefaultWitnessTable {
  const nextcode::SILDefaultWitnessTable * _Nullable table;
};

struct BridgedBuilder{

  enum class InsertAt {
    beforeInst, endOfBlock, startOfFunction, intoGlobal
  } insertAt;

  NeXTCodeObject insertionObj;
  BridgedLocation loc;

#ifdef USED_IN_CPP_SOURCE
  nextcode::SILBuilder unbridged() const {
    switch (insertAt) {
    case BridgedBuilder::InsertAt::beforeInst:
      return nextcode::SILBuilder(BridgedInstruction(insertionObj).unbridged(),
                               loc.getLoc().getScope());
    case BridgedBuilder::InsertAt::endOfBlock:
      return nextcode::SILBuilder(BridgedBasicBlock(insertionObj).unbridged(),
                               loc.getLoc().getScope());
    case BridgedBuilder::InsertAt::startOfFunction:
      return nextcode::SILBuilder(BridgedFunction(insertionObj).getFunction()->getEntryBlock(),
                               loc.getLoc().getScope());
    case BridgedBuilder::InsertAt::intoGlobal:
      return nextcode::SILBuilder(BridgedGlobalVar(insertionObj).getGlobal());
    }
  }
  nextcode::SILLocation regularLoc() const {
    return nextcode::RegularLocation(loc.getLoc().getLocation());
  }
#endif

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createBuiltinBinaryFunction(BridgedStringRef name,
                                          BridgedType operandType, BridgedType resultType,
                                          BridgedValueArray arguments) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createCondFail(BridgedValue condition,
                                                                       BridgedStringRef message) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createIntegerLiteral(BridgedType type, NeXTCodeInt value) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction
  createAllocStack(BridgedType type, bool hasDynamicLifetime, bool isLexical,
                   bool isFromVarDecl, bool wasMoved) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createAllocVector(BridgedValue capacity,
                                                                          BridgedType type) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createDeallocStack(BridgedValue operand) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createDeallocStackRef(BridgedValue operand) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createAddressToPointer(BridgedValue address,
                                                                               BridgedType pointerTy,
                                                                               bool needsStackProtection) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createUncheckedRefCast(BridgedValue op,
                                                                               BridgedType type) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createUpcast(BridgedValue op, BridgedType type) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createLoad(BridgedValue op, NeXTCodeInt ownership) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createLoadBorrow(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createBeginDeallocRef(BridgedValue reference,
                                                                              BridgedValue allocation) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createEndInitLetRef(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction
  createRetainValue(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction
  createReleaseValue(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createStrongRetain(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createStrongRelease(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createUnownedRetain(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createUnownedRelease(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createFunctionRef(BridgedFunction function) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createCopyValue(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createBeginBorrow(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createBorrowedFrom(BridgedValue borrowedValue,
                                                                           BridgedValueArray enclosingValues) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createEndBorrow(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createCopyAddr(BridgedValue from, BridgedValue to,
                                          bool takeSource, bool initializeDest) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createDestroyValue(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createDestroyAddr(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createEndLifetime(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createDebugStep() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createApply(BridgedValue function,
                                          BridgedSubstitutionMap subMap,
                                          BridgedValueArray arguments, bool isNonThrowing, bool isNonAsync,
                                          BridgedGenericSpecializationInformation specInfo) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createTryApply(BridgedValue function,
                                          BridgedSubstitutionMap subMap,
                                          BridgedValueArray arguments,
                                          BridgedBasicBlock normalBB, BridgedBasicBlock errorBB,
                                          bool isNonAsync,
                                          BridgedGenericSpecializationInformation specInfo) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createReturn(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createThrow(BridgedValue op) const;
  NEXTCODE_IMPORT_UNSAFE BridgedInstruction createSwitchEnumInst(BridgedValue enumVal,
                                          OptionalBridgedBasicBlock defaultBlock,
                                          const void * _Nullable enumCases, NeXTCodeInt numEnumCases) const;
  NEXTCODE_IMPORT_UNSAFE BridgedInstruction createSwitchEnumAddrInst(BridgedValue enumAddr,
                                          OptionalBridgedBasicBlock defaultBlock,
                                          const void * _Nullable enumCases, NeXTCodeInt numEnumCases) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createUncheckedEnumData(BridgedValue enumVal,
                                          NeXTCodeInt caseIdx, BridgedType resultType) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createUncheckedTakeEnumDataAddr(BridgedValue enumAddr,
                                                                                        NeXTCodeInt caseIdx) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createEnum(NeXTCodeInt caseIdx, OptionalBridgedValue payload,
                                          BridgedType resultType) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createThinToThickFunction(BridgedValue fn,
                                                                                  BridgedType resultType) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createPartialApply(BridgedValue fn,
                                                                           BridgedValueArray bridgedCapturedArgs,
                                                                           BridgedArgumentConvention calleeConvention,
                                                                           BridgedSubstitutionMap bridgedSubstitutionMap = BridgedSubstitutionMap(),
                                                                           bool hasUnknownIsolation = true,
                                                                           bool isOnStack = false) const;                                                                                  
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createBranch(BridgedBasicBlock destBlock,
                                                                     BridgedValueArray arguments) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createUnreachable() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createObject(BridgedType type, BridgedValueArray arguments,
                                                                     NeXTCodeInt numBaseElements) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createVector(BridgedValueArray arguments) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createGlobalAddr(BridgedGlobalVar global,
                                                                         OptionalBridgedValue dependencyToken) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createGlobalValue(BridgedGlobalVar global,
                                                                          bool isBare) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createStruct(BridgedType type,
                                                                     BridgedValueArray elements) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createStructExtract(BridgedValue str,
                                                                            NeXTCodeInt fieldIndex) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createStructElementAddr(BridgedValue addr,
                                                                                NeXTCodeInt fieldIndex) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createDestructureStruct(BridgedValue str) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createTuple(BridgedType type,
                                                                    BridgedValueArray elements) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createTupleExtract(BridgedValue str,
                                                                           NeXTCodeInt elementIndex) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createTupleElementAddr(BridgedValue addr,
                                                                               NeXTCodeInt elementIndex) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createDestructureTuple(BridgedValue str) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createStore(BridgedValue src, BridgedValue dst,
                                          NeXTCodeInt ownership) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createInitExistentialRef(BridgedValue instance,
                                          BridgedType type,
                                          BridgedInstruction useConformancesOf) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createInitExistentialMetatype(BridgedValue metatype,
                                          BridgedType existentialType,
                                          BridgedInstruction useConformancesOf) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createMetatype(BridgedType type,
                                          BridgedType::MetatypeRepresentation representation) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createEndCOWMutation(BridgedValue instance,
                                                                             bool keepUnique) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createMarkDependence(
    BridgedValue value, BridgedValue base, BridgedInstruction::MarkDependenceKind dependenceKind) const;
    
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createEndAccess(BridgedValue value) const;

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createConvertFunction(BridgedValue originalFunction, BridgedType resultType, bool withoutActuallyEscaping) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedInstruction createConvertEscapeToNoEscape(BridgedValue originalFunction, BridgedType resultType, bool isLifetimeGuaranteed) const;

  NEXTCODE_IMPORT_UNSAFE void destroyCapturedArgs(BridgedInstruction partialApply) const;
};

// Passmanager and Context

namespace nextcode {
  class NeXTCodePassInvocation;
}

struct BridgedChangeNotificationHandler {
  nextcode::NeXTCodePassInvocation * _Nonnull invocation;

  enum class Kind {
    instructionsChanged,
    callsChanged,
    branchesChanged,
    effectsChanged
  };

  void notifyChanges(Kind changeKind) const;
};

namespace nextcode::test {
struct Arguments;
class FunctionTest;
} // namespace nextcode::test

struct BridgedFunctionTest {
  nextcode::test::FunctionTest *_Nonnull test;
};

struct BridgedTestArguments {
  nextcode::test::Arguments *_Nonnull arguments;

  bool hasUntaken() const;
  NEXTCODE_IMPORT_UNSAFE BridgedStringRef takeString() const;
  bool takeBool() const;
  NeXTCodeInt takeInt() const;
  NEXTCODE_IMPORT_UNSAFE BridgedOperand takeOperand() const;
  NEXTCODE_IMPORT_UNSAFE BridgedValue takeValue() const;
  NEXTCODE_IMPORT_UNSAFE BridgedInstruction takeInstruction() const;
  NEXTCODE_IMPORT_UNSAFE BridgedArgument takeArgument() const;
  NEXTCODE_IMPORT_UNSAFE BridgedBasicBlock takeBlock() const;
  NEXTCODE_IMPORT_UNSAFE BridgedFunction takeFunction() const;
};

struct BridgedNeXTCodePassInvocation {
  nextcode::NeXTCodePassInvocation *_Nonnull invocation;
};

using NeXTCodeNativeFunctionTestThunk =
    void (*_Nonnull)(void *_Nonnull, BridgedFunction, BridgedTestArguments,
                     BridgedNeXTCodePassInvocation);

void registerFunctionTestThunk(NeXTCodeNativeFunctionTestThunk);

void registerFunctionTest(BridgedStringRef,
                          void *_Nonnull nativeNeXTCodeInvocation);

NEXTCODE_END_NULLABILITY_ANNOTATIONS

#ifndef PURE_BRIDGING_MODE
// In _not_ PURE_BRIDGING_MODE, bridging functions are inlined and therefore
// included in the header file. This is because they rely on C++ headers that
// we don't want to pull in when using "pure bridging mode".
#include "SILBridgingImpl.h"
#endif

#endif
