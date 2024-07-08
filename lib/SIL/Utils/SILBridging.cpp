//===--- SILBridging.cpp --------------------------------------------------===//
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

#include "nextcode/SIL/SILBridging.h"

#ifdef PURE_BRIDGING_MODE
// In PURE_BRIDGING_MODE, briding functions are not inlined and therefore inluded in the cpp file.
#include "nextcode/SIL/SILBridgingImpl.h"
#endif

#include "nextcode/AST/Attr.h"
#include "nextcode/AST/SemanticAttrs.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/MemAccessUtils.h"
#include "nextcode/SIL/OwnershipUtils.h"
#include "nextcode/SIL/ParseTestSpecification.h"
#include "nextcode/SIL/SILBuilder.h"
#include "nextcode/SIL/SILGlobalVariable.h"
#include "nextcode/SIL/SILNode.h"
#include "nextcode/SIL/Test.h"
#include <string>
#include <cstring>
#include <stdio.h>

using namespace nextcode;

namespace {

bool nodeMetatypesInitialized = false;

// Filled in by class registration in initializeNeXTCodeModules().
NeXTCodeMetatype nodeMetatypes[(unsigned)SILNodeKind::Last_SILNode + 1];

}

bool nextcodeModulesInitialized() {
  return nodeMetatypesInitialized;
}

// Does return null if initializeNeXTCodeModules() is never called.
NeXTCodeMetatype SILNode::getSILNodeMetatype(SILNodeKind kind) {
  NeXTCodeMetatype metatype = nodeMetatypes[(unsigned)kind];
  if (nodeMetatypesInitialized && !metatype) {
    llvm::errs() << "Instruction " << getSILInstructionName((SILInstructionKind)kind) << " not registered\n";
    abort();
  }
  return metatype;
}

//===----------------------------------------------------------------------===//
//                          Class registration
//===----------------------------------------------------------------------===//

static llvm::StringMap<SILNodeKind> valueNamesToKind;

/// Registers the metatype of a nextcode SIL class.
/// Called by initializeNeXTCodeModules().
void registerBridgedClass(BridgedStringRef bridgedClassName, NeXTCodeMetatype metatype) {
  StringRef className = bridgedClassName.unbridged();
  nodeMetatypesInitialized = true;

  // Handle the important non Node classes.
  if (className == "BasicBlock")
    return SILBasicBlock::registerBridgedMetatype(metatype);
  if (className == "GlobalVariable")
    return SILGlobalVariable::registerBridgedMetatype(metatype);
  if (className == "Argument") {
    nodeMetatypes[(unsigned)SILNodeKind::SILPhiArgument] = metatype;
    return;
  }
  if (className == "FunctionArgument") {
    nodeMetatypes[(unsigned)SILNodeKind::SILFunctionArgument] = metatype;
    return;
  }

  if (valueNamesToKind.empty()) {
#define VALUE(ID, PARENT) \
    valueNamesToKind[#ID] = SILNodeKind::ID;
#define NON_VALUE_INST(ID, NAME, PARENT, MEMBEHAVIOR, MAYRELEASE) \
    VALUE(ID, NAME)
#define ARGUMENT(ID, PARENT) \
    VALUE(ID, NAME)
#define SINGLE_VALUE_INST(ID, NAME, PARENT, MEMBEHAVIOR, MAYRELEASE) \
    VALUE(ID, NAME)
#define MULTIPLE_VALUE_INST(ID, NAME, PARENT, MEMBEHAVIOR, MAYRELEASE) \
    VALUE(ID, NAME)
#include "nextcode/SIL/SILNodes.def"
  }

  std::string prefixedName;
  auto iter = valueNamesToKind.find(className);
  if (iter == valueNamesToKind.end()) {
    // Try again with a "SIL" prefix. For example Argument -> SILArgument.
    prefixedName = std::string("SIL") + std::string(className);
    iter = valueNamesToKind.find(prefixedName);
    if (iter == valueNamesToKind.end()) {
      llvm::errs() << "Unknown bridged node class " << className << '\n';
      abort();
    }
    className = prefixedName;
  }
  SILNodeKind kind = iter->second;
  NeXTCodeMetatype existingTy = nodeMetatypes[(unsigned)kind];
  if (existingTy) {
    llvm::errs() << "Double registration of class " << className << '\n';
    abort();
  }
  nodeMetatypes[(unsigned)kind] = metatype;
}

//===----------------------------------------------------------------------===//
//                                Test
//===----------------------------------------------------------------------===//

void registerFunctionTest(BridgedStringRef name, void *nativeNeXTCodeInvocation) {
  nextcode::test::FunctionTest::createNativeNeXTCodeFunctionTest(
      name.unbridged(), nativeNeXTCodeInvocation);
}

bool BridgedTestArguments::hasUntaken() const {
  return arguments->hasUntaken();
}

BridgedStringRef BridgedTestArguments::takeString() const {
  return arguments->takeString();
}

bool BridgedTestArguments::takeBool() const { return arguments->takeBool(); }

NeXTCodeInt BridgedTestArguments::takeInt() const { return arguments->takeUInt(); }

BridgedOperand BridgedTestArguments::takeOperand() const {
  return {arguments->takeOperand()};
}

BridgedValue BridgedTestArguments::takeValue() const {
  return {arguments->takeValue()};
}

BridgedInstruction BridgedTestArguments::takeInstruction() const {
  return {arguments->takeInstruction()->asSILNode()};
}

BridgedArgument BridgedTestArguments::takeArgument() const {
  return {arguments->takeBlockArgument()};
}

BridgedBasicBlock BridgedTestArguments::takeBlock() const {
  return {arguments->takeBlock()};
}

BridgedFunction BridgedTestArguments::takeFunction() const {
  return {arguments->takeFunction()};
}

//===----------------------------------------------------------------------===//
//                                SILType
//===----------------------------------------------------------------------===//

static_assert((int)BridgedType::MetatypeRepresentation::Thin == (int)nextcode::MetatypeRepresentation::Thin);
static_assert((int)BridgedType::MetatypeRepresentation::Thick == (int)nextcode::MetatypeRepresentation::Thick);
static_assert((int)BridgedType::MetatypeRepresentation::ObjC == (int)nextcode::MetatypeRepresentation::ObjC);

static_assert((int)BridgedType::TraitResult::IsNot == (int)nextcode::TypeTraitResult::IsNot);
static_assert((int)BridgedType::TraitResult::CanBe == (int)nextcode::TypeTraitResult::CanBe);
static_assert((int)BridgedType::TraitResult::Is == (int)nextcode::TypeTraitResult::Is);


//===----------------------------------------------------------------------===//
//                                SILFunction
//===----------------------------------------------------------------------===//

static_assert((int)BridgedFunction::EffectsKind::ReadNone == (int)nextcode::EffectsKind::ReadNone);
static_assert((int)BridgedFunction::EffectsKind::ReadOnly == (int)nextcode::EffectsKind::ReadOnly);
static_assert((int)BridgedFunction::EffectsKind::ReleaseNone == (int)nextcode::EffectsKind::ReleaseNone);
static_assert((int)BridgedFunction::EffectsKind::ReadWrite == (int)nextcode::EffectsKind::ReadWrite);
static_assert((int)BridgedFunction::EffectsKind::Unspecified == (int)nextcode::EffectsKind::Unspecified);
static_assert((int)BridgedFunction::EffectsKind::Custom == (int)nextcode::EffectsKind::Custom);

static_assert((int)BridgedFunction::PerformanceConstraints::None == (int)nextcode::PerformanceConstraints::None);
static_assert((int)BridgedFunction::PerformanceConstraints::NoAllocation == (int)nextcode::PerformanceConstraints::NoAllocation);
static_assert((int)BridgedFunction::PerformanceConstraints::NoLocks == (int)nextcode::PerformanceConstraints::NoLocks);
static_assert((int)BridgedFunction::PerformanceConstraints::NoRuntime == (int)nextcode::PerformanceConstraints::NoRuntime);
static_assert((int)BridgedFunction::PerformanceConstraints::NoExistentials == (int)nextcode::PerformanceConstraints::NoExistentials);
static_assert((int)BridgedFunction::PerformanceConstraints::NoObjCBridging == (int)nextcode::PerformanceConstraints::NoObjCBridging);

static_assert((int)BridgedFunction::InlineStrategy::InlineDefault == (int)nextcode::InlineDefault);
static_assert((int)BridgedFunction::InlineStrategy::NoInline == (int)nextcode::NoInline);
static_assert((int)BridgedFunction::InlineStrategy::AlwaysInline == (int)nextcode::AlwaysInline);

static_assert((int)BridgedFunction::ThunkKind::IsNotThunk == (int)nextcode::IsNotThunk);
static_assert((int)BridgedFunction::ThunkKind::IsThunk == (int)nextcode::IsThunk);
static_assert((int)BridgedFunction::ThunkKind::IsReabstractionThunk == (int)nextcode::IsReabstractionThunk);
static_assert((int)BridgedFunction::ThunkKind::IsSignatureOptimizedThunk == (int)nextcode::IsSignatureOptimizedThunk);

BridgedOwnedString BridgedFunction::getDebugDescription() const {
  std::string str;
  llvm::raw_string_ostream os(str);
  getFunction()->print(os);
  str.pop_back(); // Remove trailing newline.
  return str;
}

//===----------------------------------------------------------------------===//
//                               SILBasicBlock
//===----------------------------------------------------------------------===//

BridgedOwnedString BridgedBasicBlock::getDebugDescription() const {
  std::string str;
  llvm::raw_string_ostream os(str);
  unbridged()->print(os);
  str.pop_back(); // Remove trailing newline.
  return str;
}

//===----------------------------------------------------------------------===//
//                                SILValue
//===----------------------------------------------------------------------===//

BridgedOwnedString BridgedValue::getDebugDescription() const {
  std::string str;
  llvm::raw_string_ostream os(str);
  getSILValue()->print(os);
  str.pop_back(); // Remove trailing newline.
  return str;
}

BridgedValue::Kind BridgedValue::getKind() const {
  SILValue v = getSILValue();
  if (isa<SingleValueInstruction>(v)) {
    return BridgedValue::Kind::SingleValueInstruction;
  } else if (isa<SILArgument>(v)) {
    return BridgedValue::Kind::Argument;
  } else if (isa<MultipleValueInstructionResult>(v)) {
    return BridgedValue::Kind::MultipleValueInstructionResult;
  } else if (isa<SILUndef>(v)) {
    return BridgedValue::Kind::Undef;
  }
  llvm_unreachable("unknown SILValue");
}

ArrayRef<SILValue> BridgedValueArray::getValues(SmallVectorImpl<SILValue> &storage) {
  for (unsigned idx = 0; idx < count; ++idx) {
    storage.push_back(base[idx].value.getSILValue());
  }
  return storage;
}

bool BridgedValue::findPointerEscape() const {
  return nextcode::findPointerEscape(getSILValue());
}

//===----------------------------------------------------------------------===//
//                                SILArgument
//===----------------------------------------------------------------------===//

static_assert((int)BridgedArgumentConvention::Indirect_In == (int)nextcode::SILArgumentConvention::Indirect_In);
static_assert((int)BridgedArgumentConvention::Indirect_In_Guaranteed == (int)nextcode::SILArgumentConvention::Indirect_In_Guaranteed);
static_assert((int)BridgedArgumentConvention::Indirect_Inout == (int)nextcode::SILArgumentConvention::Indirect_Inout);
static_assert((int)BridgedArgumentConvention::Indirect_InoutAliasable == (int)nextcode::SILArgumentConvention::Indirect_InoutAliasable);
static_assert((int)BridgedArgumentConvention::Indirect_Out == (int)nextcode::SILArgumentConvention::Indirect_Out);
static_assert((int)BridgedArgumentConvention::Direct_Owned == (int)nextcode::SILArgumentConvention::Direct_Owned);
static_assert((int)BridgedArgumentConvention::Direct_Unowned == (int)nextcode::SILArgumentConvention::Direct_Unowned);
static_assert((int)BridgedArgumentConvention::Direct_Guaranteed == (int)nextcode::SILArgumentConvention::Direct_Guaranteed);
static_assert((int)BridgedArgumentConvention::Pack_Owned == (int)nextcode::SILArgumentConvention::Pack_Owned);
static_assert((int)BridgedArgumentConvention::Pack_Inout == (int)nextcode::SILArgumentConvention::Pack_Inout);
static_assert((int)BridgedArgumentConvention::Pack_Guaranteed == (int)nextcode::SILArgumentConvention::Pack_Guaranteed);
static_assert((int)BridgedArgumentConvention::Pack_Out == (int)nextcode::SILArgumentConvention::Pack_Out);


//===----------------------------------------------------------------------===//
//                            SILGlobalVariable
//===----------------------------------------------------------------------===//

BridgedOwnedString BridgedGlobalVar::getDebugDescription() const {
  std::string str;
  llvm::raw_string_ostream os(str);
  getGlobal()->print(os);
  str.pop_back(); // Remove trailing newline.
  return str;
}

bool BridgedGlobalVar::canBeInitializedStatically() const {
  SILGlobalVariable *global = getGlobal();
  auto expansion = ResilienceExpansion::Maximal;
  if (hasPublicVisibility(global->getLinkage()))
    expansion = ResilienceExpansion::Minimal;

  auto &tl = global->getModule().Types.getTypeLowering(
      global->getLoweredType(),
      TypeExpansionContext::noOpaqueTypeArchetypesSubstitution(expansion));
  return tl.isLoadable();
}

bool BridgedGlobalVar::mustBeInitializedStatically() const {
  SILGlobalVariable *global = getGlobal();
  return global->mustBeInitializedStatically();
}

//===----------------------------------------------------------------------===//
//                            SILVTable
//===----------------------------------------------------------------------===//

BridgedOwnedString BridgedVTable::getDebugDescription() const {
  std::string str;
  llvm::raw_string_ostream os(str);
  vTable->print(os);
  str.pop_back(); // Remove trailing newline.
  return str;
}

BridgedOwnedString BridgedVTableEntry::getDebugDescription() const {
  std::string str;
  llvm::raw_string_ostream os(str);
  entry->print(os);
  str.pop_back(); // Remove trailing newline.
  return str;
}

//===----------------------------------------------------------------------===//
//                    SILVWitnessTable, SILDefaultWitnessTable
//===----------------------------------------------------------------------===//

static_assert((int)BridgedWitnessTableEntry::Kind::Invalid == (int)nextcode::SILWitnessTable::WitnessKind::Invalid);
static_assert((int)BridgedWitnessTableEntry::Kind::Method == (int)nextcode::SILWitnessTable::WitnessKind::Method);
static_assert((int)BridgedWitnessTableEntry::Kind::AssociatedType == (int)nextcode::SILWitnessTable::WitnessKind::AssociatedType);
static_assert((int)BridgedWitnessTableEntry::Kind::AssociatedTypeProtocol == (int)nextcode::SILWitnessTable::WitnessKind::AssociatedTypeProtocol);
static_assert((int)BridgedWitnessTableEntry::Kind::BaseProtocol == (int)nextcode::SILWitnessTable::WitnessKind::BaseProtocol);

BridgedOwnedString BridgedWitnessTableEntry::getDebugDescription() const {
  std::string str;
  llvm::raw_string_ostream os(str);
  getEntry()->print(os, /*verbose=*/ false, PrintOptions::printSIL());
  str.pop_back(); // Remove trailing newline.
  return str;
}

BridgedOwnedString BridgedWitnessTable::getDebugDescription() const {
  std::string str;
  llvm::raw_string_ostream os(str);
  table->print(os);
  str.pop_back(); // Remove trailing newline.
  return str;
}

BridgedOwnedString BridgedDefaultWitnessTable::getDebugDescription() const {
  std::string str;
  llvm::raw_string_ostream os(str);
  table->print(os);
  str.pop_back(); // Remove trailing newline.
  return str;
}

//===----------------------------------------------------------------------===//
//                               SubstitutionMap
//===----------------------------------------------------------------------===//

static_assert(sizeof(BridgedSubstitutionMap) >= sizeof(nextcode::SubstitutionMap),
              "BridgedSubstitutionMap has wrong size");


//===----------------------------------------------------------------------===//
//                               SILDebugLocation
//===----------------------------------------------------------------------===//

static_assert(sizeof(BridgedLocation) >= sizeof(nextcode::SILDebugLocation),
              "BridgedLocation has wrong size");

BridgedOwnedString BridgedLocation::getDebugDescription() const {
  std::string str;
  llvm::raw_string_ostream os(str);
  SILLocation loc = getLoc().getLocation();
  loc.print(os);
#ifndef NDEBUG
  if (const SILDebugScope *scope = getLoc().getScope()) {
    if (DeclContext *dc = loc.getAsDeclContext()) {
      os << ", scope=";
      scope->print(dc->getASTContext().SourceMgr, os, /*indent*/ 2);
    } else {
      os << ", scope=?";
    }
  }
#endif
  return str;
}

//===----------------------------------------------------------------------===//
//                               SILInstruction
//===----------------------------------------------------------------------===//

static_assert((int)BridgedMemoryBehavior::None == (int)nextcode::MemoryBehavior::None);
static_assert((int)BridgedMemoryBehavior::MayRead == (int)nextcode::MemoryBehavior::MayRead);
static_assert((int)BridgedMemoryBehavior::MayWrite == (int)nextcode::MemoryBehavior::MayWrite);
static_assert((int)BridgedMemoryBehavior::MayReadWrite == (int)nextcode::MemoryBehavior::MayReadWrite);
static_assert((int)BridgedMemoryBehavior::MayHaveSideEffects == (int)nextcode::MemoryBehavior::MayHaveSideEffects);

BridgedOwnedString BridgedInstruction::getDebugDescription() const {
  std::string str;
  llvm::raw_string_ostream os(str);
  unbridged()->print(os);
  str.pop_back(); // Remove trailing newline.
  return str;
}

bool BridgedInstruction::mayAccessPointer() const {
  return ::mayAccessPointer(unbridged());
}

bool BridgedInstruction::mayLoadWeakOrUnowned() const {
  return ::mayLoadWeakOrUnowned(unbridged());
}

bool BridgedInstruction::maySynchronize() const {
  return ::maySynchronize(unbridged());
}

bool BridgedInstruction::mayBeDeinitBarrierNotConsideringSideEffects() const {
  return ::mayBeDeinitBarrierNotConsideringSideEffects(unbridged());
}

//===----------------------------------------------------------------------===//
//                               BridgedBuilder
//===----------------------------------------------------------------------===//

static llvm::SmallVector<std::pair<nextcode::EnumElementDecl *, nextcode::SILBasicBlock *>, 16>
convertCases(SILType enumTy, const void * _Nullable enumCases, NeXTCodeInt numEnumCases) {
  using BridgedCase = const std::pair<NeXTCodeInt, BridgedBasicBlock>;
  llvm::ArrayRef<BridgedCase> cases(static_cast<BridgedCase *>(enumCases),
                                    (unsigned)numEnumCases);
  llvm::SmallDenseMap<NeXTCodeInt, nextcode::EnumElementDecl *> mappedElements;
  nextcode::EnumDecl *enumDecl = enumTy.getEnumOrBoundGenericEnum();
  for (auto elemWithIndex : llvm::enumerate(enumDecl->getAllElements())) {
    mappedElements[elemWithIndex.index()] = elemWithIndex.value();
  }
  llvm::SmallVector<std::pair<nextcode::EnumElementDecl *, nextcode::SILBasicBlock *>, 16> convertedCases;
  for (auto c : cases) {
    assert(mappedElements.count(c.first) && "wrong enum element index");
    convertedCases.push_back({mappedElements[c.first], c.second.unbridged()});
  }
  return convertedCases;
}

BridgedInstruction BridgedBuilder::createSwitchEnumInst(BridgedValue enumVal, OptionalBridgedBasicBlock defaultBlock,
                                        const void * _Nullable enumCases, NeXTCodeInt numEnumCases) const {
  return {unbridged().createSwitchEnum(regularLoc(),
                                       enumVal.getSILValue(),
                                       defaultBlock.unbridged(),
                                       convertCases(enumVal.getSILValue()->getType(), enumCases, numEnumCases))};
}

BridgedInstruction BridgedBuilder::createSwitchEnumAddrInst(BridgedValue enumAddr,
                                                            OptionalBridgedBasicBlock defaultBlock,
                                                            const void * _Nullable enumCases,
                                                            NeXTCodeInt numEnumCases) const {
  return {unbridged().createSwitchEnumAddr(regularLoc(),
                                           enumAddr.getSILValue(),
                                           defaultBlock.unbridged(),
                                           convertCases(enumAddr.getSILValue()->getType(), enumCases, numEnumCases))};
}
