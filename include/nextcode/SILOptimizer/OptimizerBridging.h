//===--- OptimizerBridging.h - header for the OptimizerBridging module ----===//
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

#ifndef NEXTCODE_SILOPTIMIZER_OPTIMIZERBRIDGING_H
#define NEXTCODE_SILOPTIMIZER_OPTIMIZERBRIDGING_H

// Do not add other C++/llvm/nextcode header files here!
// Function implementations should be placed into OptimizerBridgingImpl.h or PassManager.cpp
// (under OptimizerBridging) andrequired header files should be added there.
//
// Pure bridging mode does not permit including any C++/llvm/nextcode headers.
// See also the comments for `BRIDGING_MODE` in the top-level CMakeLists.txt file.
//
#include "nextcode/AST/ASTBridging.h"
#include "nextcode/SIL/SILBridging.h"

#ifdef USED_IN_CPP_SOURCE

#include "nextcode/SILOptimizer/Analysis/BasicCalleeAnalysis.h"

#else // USED_IN_CPP_SOURCE

// Pure bridging mode does not permit including any C++/llvm/nextcode headers.
// See also the comments for `BRIDGING_MODE` in the top-level CMakeLists.txt file.
#ifdef NEXTCODE_SIL_SILVALUE_H
#error "should not include nextcode headers into bridging header"
#endif
#ifdef LLVM_SUPPORT_COMPILER_H
#error "should not include llvm headers into bridging header"
#endif

#endif // USED_IN_CPP_SOURCE

NEXTCODE_BEGIN_NULLABILITY_ANNOTATIONS

namespace nextcode {
class AliasAnalysis;
class BasicCalleeAnalysis;
class DeadEndBlocks;
class DominanceInfo;
class PostDominanceInfo;
class BasicBlockSet;
class NodeSet;
class OperandSet;
class ClonerWithFixedLocation;
class NeXTCodePassInvocation;
class FixedSizeSlabPayload;
class FixedSizeSlab;
class SILVTable;
class ClosureSpecializationCloner;
}

struct BridgedPassContext;

struct BridgedAliasAnalysis {
  nextcode::AliasAnalysis * _Nonnull aa;

  BRIDGED_INLINE BridgedMemoryBehavior getMemBehavior(BridgedInstruction inst, BridgedValue addr) const;

  typedef BridgedMemoryBehavior (* _Nonnull GetMemEffectFn)(
        BridgedPassContext context, BridgedValue, BridgedInstruction, NeXTCodeInt);
  typedef bool (* _Nonnull Escaping2InstFn)(
        BridgedPassContext context, BridgedValue, BridgedInstruction, NeXTCodeInt);
  typedef bool (* _Nonnull Escaping2ValFn)(
        BridgedPassContext context, BridgedValue, BridgedValue);
  typedef bool (* _Nonnull Escaping2ValIntFn)(
        BridgedPassContext context, BridgedValue, BridgedValue, NeXTCodeInt);

  static void registerAnalysis(GetMemEffectFn getMemEffectsFn,
                               Escaping2InstFn isObjReleasedFn,
                               Escaping2ValIntFn isAddrVisibleFromObjFn,
                               Escaping2ValFn mayPointToSameAddrFn);
};

struct BridgedCalleeAnalysis {
  nextcode::BasicCalleeAnalysis * _Nonnull ca;

  struct CalleeList {
    uint64_t storage[3];

#ifdef USED_IN_CPP_SOURCE
    CalleeList(nextcode::CalleeList list) {
      *reinterpret_cast<nextcode::CalleeList *>(&storage) = list;
    }
    nextcode::CalleeList unbridged() const {
      return *reinterpret_cast<const nextcode::CalleeList *>(&storage);
    }
#endif

    BRIDGED_INLINE bool isIncomplete() const;
    BRIDGED_INLINE NeXTCodeInt getCount() const;
    NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedFunction getCallee(NeXTCodeInt index) const;
  };

  NEXTCODE_IMPORT_UNSAFE CalleeList getCallees(BridgedValue callee) const;
  NEXTCODE_IMPORT_UNSAFE CalleeList getDestructors(BridgedType type, bool isExactType) const;

  typedef bool (* _Nonnull IsDeinitBarrierFn)(BridgedInstruction, BridgedCalleeAnalysis bca);
  typedef BridgedMemoryBehavior (* _Nonnull GetMemBehvaiorFn)(
        BridgedInstruction apply, bool observeRetains, BridgedCalleeAnalysis bca);

  static void registerAnalysis(IsDeinitBarrierFn isDeinitBarrierFn,
                               GetMemBehvaiorFn getEffectsFn);
};

struct BridgedDeadEndBlocksAnalysis {
  nextcode::DeadEndBlocks * _Nonnull deb;

  BRIDGED_INLINE bool isDeadEnd(BridgedBasicBlock block) const;
};

struct BridgedDomTree {
  nextcode::DominanceInfo * _Nonnull di;

  BRIDGED_INLINE bool dominates(BridgedBasicBlock dominating, BridgedBasicBlock dominated) const;
};

struct BridgedPostDomTree {
  nextcode::PostDominanceInfo * _Nonnull pdi;

  BRIDGED_INLINE bool postDominates(BridgedBasicBlock dominating, BridgedBasicBlock dominated) const;
};

struct BridgedUtilities {
  typedef void (* _Nonnull VerifyFunctionFn)(BridgedPassContext, BridgedFunction);
  typedef void (* _Nonnull UpdateBorrowedFromFn)(BridgedPassContext, BridgedFunction);
  typedef void (* _Nonnull UpdateBorrowedFromPhisFn)(BridgedPassContext, BridgedArrayRef);

  static void registerVerifier(VerifyFunctionFn verifyFunctionFn);
  static void registerBorrowedFromUpdater(UpdateBorrowedFromFn updateBorrowedFromFn,
                                          UpdateBorrowedFromPhisFn updateBorrowedFromPhisFn);
};

struct BridgedBasicBlockSet {
  nextcode::BasicBlockSet * _Nonnull set;

  BRIDGED_INLINE bool contains(BridgedBasicBlock block) const;
  BRIDGED_INLINE bool insert(BridgedBasicBlock block) const;
  BRIDGED_INLINE void erase(BridgedBasicBlock block) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedFunction getFunction() const;
};

struct BridgedNodeSet {
  nextcode::NodeSet * _Nonnull set;

  BRIDGED_INLINE bool containsValue(BridgedValue value) const;
  BRIDGED_INLINE bool insertValue(BridgedValue value) const;
  BRIDGED_INLINE void eraseValue(BridgedValue value) const;
  BRIDGED_INLINE bool containsInstruction(BridgedInstruction inst) const;
  BRIDGED_INLINE bool insertInstruction(BridgedInstruction inst) const;
  BRIDGED_INLINE void eraseInstruction(BridgedInstruction inst) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedFunction getFunction() const;
};

struct BridgedOperandSet {
  nextcode::OperandSet * _Nonnull set;

  BRIDGED_INLINE bool contains(BridgedOperand operand) const;
  BRIDGED_INLINE bool insert(BridgedOperand operand) const;
  BRIDGED_INLINE void erase(BridgedOperand operand) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedFunction getFunction() const;
};

struct BridgedCloner {
  nextcode::ClonerWithFixedLocation * _Nonnull cloner;

  BridgedCloner(BridgedGlobalVar var, BridgedPassContext context);
  BridgedCloner(BridgedInstruction inst, BridgedPassContext context);
  void destroy(BridgedPassContext context);
  NEXTCODE_IMPORT_UNSAFE BridgedValue getClonedValue(BridgedValue v);
  bool isValueCloned(BridgedValue v) const;
  void clone(BridgedInstruction inst);
};

struct BridgedSpecializationCloner {
  nextcode::ClosureSpecializationCloner * _Nonnull closureSpecCloner;

  NEXTCODE_IMPORT_UNSAFE BridgedSpecializationCloner(BridgedFunction emptySpecializedFunction);
  NEXTCODE_IMPORT_UNSAFE BridgedFunction getCloned() const;
  NEXTCODE_IMPORT_UNSAFE BridgedBasicBlock getClonedBasicBlock(BridgedBasicBlock originalBasicBlock) const;
  void cloneFunctionBody(BridgedFunction originalFunction, BridgedBasicBlock clonedEntryBlock, BridgedValueArray clonedEntryBlockArgs) const;
};

struct BridgedPassContext {
  nextcode::NeXTCodePassInvocation * _Nonnull invocation;

  enum class SILStage {
    Raw,
    Canonical,
    Lowered
  };

  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString getModuleDescription() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedChangeNotificationHandler asNotificationHandler() const;
  BRIDGED_INLINE void notifyDependencyOnBodyOf(BridgedFunction otherFunction) const;
  BRIDGED_INLINE SILStage getSILStage() const;
  BRIDGED_INLINE bool hadError() const;
  BRIDGED_INLINE bool moduleIsSerialized() const;
  BRIDGED_INLINE bool isTransforming(BridgedFunction function) const;

  // Analysis

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedAliasAnalysis getAliasAnalysis() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedCalleeAnalysis getCalleeAnalysis() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedDeadEndBlocksAnalysis getDeadEndBlocksAnalysis() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedDomTree getDomTree() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedPostDomTree getPostDomTree() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedNominalTypeDecl getNeXTCodeArrayDecl() const;

  // AST

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE
  BridgedDiagnosticEngine getDiagnosticEngine() const;

  // SIL modifications

  struct DevirtResult {
    OptionalBridgedInstruction newApply;
    bool cfgChanged;
  };

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedBasicBlock splitBlockBefore(BridgedInstruction bridgedInst) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedBasicBlock splitBlockAfter(BridgedInstruction bridgedInst) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedBasicBlock createBlockAfter(BridgedBasicBlock bridgedBlock) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedBasicBlock appendBlock(BridgedFunction bridgedFunction) const;
  BRIDGED_INLINE void eraseInstruction(BridgedInstruction inst) const;
  BRIDGED_INLINE void eraseBlock(BridgedBasicBlock block) const;
  static BRIDGED_INLINE void moveInstructionBefore(BridgedInstruction inst, BridgedInstruction beforeInst);
  bool tryOptimizeApplyOfPartialApply(BridgedInstruction closure) const;
  bool tryDeleteDeadClosure(BridgedInstruction closure, bool needKeepArgsAlive) const;
  NEXTCODE_IMPORT_UNSAFE DevirtResult tryDevirtualizeApply(BridgedInstruction apply, bool isMandatory) const;
  bool tryOptimizeKeypath(BridgedInstruction apply) const;
  NEXTCODE_IMPORT_UNSAFE OptionalBridgedValue constantFoldBuiltin(BridgedInstruction builtin) const;
  NEXTCODE_IMPORT_UNSAFE nextcode::SILVTable * _Nullable specializeVTableForType(BridgedType type,
                                                                           BridgedFunction function) const;
  bool specializeClassMethodInst(BridgedInstruction cm) const;
  bool specializeAppliesInFunction(BridgedFunction function, bool isMandatory) const;
  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString mangleOutlinedVariable(BridgedFunction function) const;
  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString mangleAsyncRemoved(BridgedFunction function) const;
  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString mangleWithDeadArgs(const NeXTCodeInt * _Nullable deadArgs,
                                                            NeXTCodeInt numDeadArgs,
                                                            BridgedFunction function) const;
  NEXTCODE_IMPORT_UNSAFE BridgedOwnedString mangleWithClosureArgs(BridgedValueArray closureArgs,
                                                               BridgedArrayRef closureArgIndices,
                                                               BridgedFunction applySiteCallee) const;

  NEXTCODE_IMPORT_UNSAFE BridgedGlobalVar createGlobalVariable(BridgedStringRef name, BridgedType type,
                                                            bool isPrivate) const;
  void inlineFunction(BridgedInstruction apply, bool mandatoryInline) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedValue getSILUndef(BridgedType type) const;
  BRIDGED_INLINE bool optimizeMemoryAccesses(BridgedFunction f) const;
  BRIDGED_INLINE bool eliminateDeadAllocations(BridgedFunction f) const;

  // IRGen

  NeXTCodeInt getStaticSize(BridgedType type) const;
  NeXTCodeInt getStaticAlignment(BridgedType type) const;
  NeXTCodeInt getStaticStride(BridgedType type) const;
  bool canMakeStaticObjectReadOnly(BridgedType type) const;

  // Sets

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedBasicBlockSet allocBasicBlockSet() const;
  BRIDGED_INLINE void freeBasicBlockSet(BridgedBasicBlockSet set) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedNodeSet allocNodeSet() const;
  BRIDGED_INLINE void freeNodeSet(BridgedNodeSet set) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedOperandSet allocOperandSet() const;
  BRIDGED_INLINE void freeOperandSet(BridgedOperandSet set) const;

  // Stack nesting

  BRIDGED_INLINE void notifyInvalidatedStackNesting() const;
  BRIDGED_INLINE bool getNeedFixStackNesting() const;
  void fixStackNesting(BridgedFunction function) const;

  // Slabs

  struct Slab {
    nextcode::FixedSizeSlabPayload * _Nullable data = nullptr;

    BRIDGED_INLINE static NeXTCodeInt getCapacity();
    BRIDGED_INLINE Slab(nextcode::FixedSizeSlab * _Nullable slab);
    BRIDGED_INLINE nextcode::FixedSizeSlab * _Nullable getSlab() const;
    NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE Slab getNext() const;
    NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE Slab getPrevious() const;
  };

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE Slab allocSlab(Slab afterSlab) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE Slab freeSlab(Slab slab) const;

  // Access SIL module data structures

  struct VTableArray {
    nextcode::SILVTable * const _Nonnull * _Nullable base;
    NeXTCodeInt count;
  };

  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedFunction getFirstFunctionInModule() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE static OptionalBridgedFunction getNextFunctionInModule(BridgedFunction function);
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedGlobalVar getFirstGlobalInModule() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE static OptionalBridgedGlobalVar getNextGlobalInModule(BridgedGlobalVar global);
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE VTableArray getVTables() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedWitnessTable getFirstWitnessTableInModule() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE static OptionalBridgedWitnessTable getNextWitnessTableInModule(
                                                                                  BridgedWitnessTable table);
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedDefaultWitnessTable getFirstDefaultWitnessTableInModule() const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE static OptionalBridgedDefaultWitnessTable getNextDefaultWitnessTableInModule(
                                                                                  BridgedDefaultWitnessTable table);
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedFunction lookupFunction(BridgedStringRef name) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedFunction loadFunction(BridgedStringRef name,
                                                                          bool loadCalleesRecursively) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE void loadFunction(BridgedFunction function, bool loadCalleesRecursively) const;
  NEXTCODE_IMPORT_UNSAFE OptionalBridgedFunction lookupStdlibFunction(BridgedStringRef name) const;
  NEXTCODE_IMPORT_UNSAFE OptionalBridgedFunction lookUpNominalDeinitFunction(BridgedNominalTypeDecl nominal) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedSubstitutionMap getContextSubstitutionMap(BridgedType type) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedType getBuiltinIntegerType(NeXTCodeInt bitWidth) const;
  NEXTCODE_IMPORT_UNSAFE BridgedFunction createEmptyFunction(BridgedStringRef name,
                                                          const BridgedParameterInfo * _Nullable bridgedParams,
                                                          NeXTCodeInt paramCount,
                                                          bool hasSelfParam,
                                                          BridgedFunction fromFunc) const;
  void moveFunctionBody(BridgedFunction sourceFunc, BridgedFunction destFunc) const;

  // Passmanager housekeeping

  BRIDGED_INLINE void beginTransformFunction(BridgedFunction function) const;
  BRIDGED_INLINE void endTransformFunction() const;
  BRIDGED_INLINE bool continueWithNextSubpassRun(OptionalBridgedInstruction inst) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedPassContext initializeNestedPassContext(BridgedFunction newFunction) const;
  BRIDGED_INLINE void deinitializedNestedPassContext() const;
  BRIDGED_INLINE void
  addFunctionToPassManagerWorklist(BridgedFunction newFunction,
                                   BridgedFunction oldFunction) const;

  // SSAUpdater

  BRIDGED_INLINE void
  SSAUpdater_initialize(BridgedFunction function, BridgedType type,
                        BridgedValue::Ownership ownership) const;
  BRIDGED_INLINE void SSAUpdater_addAvailableValue(BridgedBasicBlock block, BridgedValue value) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedValue SSAUpdater_getValueAtEndOfBlock(BridgedBasicBlock block) const;
  NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE BridgedValue SSAUpdater_getValueInMiddleOfBlock(BridgedBasicBlock block) const;

  // Options

  enum class AssertConfiguration {
    Debug = 0,
    Release = 1,
    Unchecked = 2
  };

  BRIDGED_INLINE bool enableStackProtection() const;
  BRIDGED_INLINE bool hasFeature(BridgedFeature feature) const;
  BRIDGED_INLINE bool enableMoveInoutStackProtection() const;
  BRIDGED_INLINE AssertConfiguration getAssertConfiguration() const;
  bool enableSimplificationFor(BridgedInstruction inst) const;

  // Closure specializer
  NEXTCODE_IMPORT_UNSAFE BridgedFunction ClosureSpecializer_createEmptyFunctionWithSpecializedSignature(BridgedStringRef specializedName,
                                                        const BridgedParameterInfo * _Nullable specializedBridgedParams,
                                                        NeXTCodeInt paramCount,
                                                        BridgedFunction bridgedApplySiteCallee,
                                                        bool isSerialized) const;
};

bool FullApplySite_canInline(BridgedInstruction apply);

enum class BridgedDynamicCastResult {
  willSucceed,
  maySucceed,
  willFail
};

BridgedDynamicCastResult classifyDynamicCastBridged(BridgedType sourceTy, BridgedType destTy,
                                                    BridgedFunction function,
                                                    bool sourceTypeIsExact);

BridgedDynamicCastResult classifyDynamicCastBridged(BridgedInstruction inst);

//===----------------------------------------------------------------------===//
//                          Pass registration
//===----------------------------------------------------------------------===//

struct BridgedFunctionPassCtxt {
  BridgedFunction function;
  BridgedPassContext passContext;
} ;

struct BridgedInstructionPassCtxt {
  BridgedInstruction instruction;
  BridgedPassContext passContext;
};

typedef void (* _Nonnull BridgedModulePassRunFn)(BridgedPassContext);
typedef void (* _Nonnull BridgedFunctionPassRunFn)(BridgedFunctionPassCtxt);
typedef void (* _Nonnull BridgedInstructionPassRunFn)(BridgedInstructionPassCtxt);

void SILPassManager_registerModulePass(BridgedStringRef name,
                                       BridgedModulePassRunFn runFn);
void SILPassManager_registerFunctionPass(BridgedStringRef name,
                                         BridgedFunctionPassRunFn runFn);
void SILCombine_registerInstructionPass(BridgedStringRef instClassName,
                                        BridgedInstructionPassRunFn runFn);

#ifndef PURE_BRIDGING_MODE
// In _not_ PURE_BRIDGING_MODE, briding functions are inlined and therefore inluded in the header file.
#include "OptimizerBridgingImpl.h"
#else
// For fflush and stdout
#include <stdio.h>
#endif

NEXTCODE_END_NULLABILITY_ANNOTATIONS

#endif
