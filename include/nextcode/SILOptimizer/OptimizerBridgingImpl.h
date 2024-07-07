//===--- OptimizerBridgingImpl.h ------------------------------------------===//
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

#ifndef NEXTCODE_SILOPTIMIZER_OPTIMIZERBRIDGING_IMPL_H
#define NEXTCODE_SILOPTIMIZER_OPTIMIZERBRIDGING_IMPL_H

#include "nextcode/Demangling/Demangle.h"
#include "nextcode/SILOptimizer/Analysis/AliasAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/BasicCalleeAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/DeadEndBlocksAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/DominanceAnalysis.h"
#include "nextcode/SILOptimizer/OptimizerBridging.h"
#include "nextcode/SILOptimizer/PassManager/PassManager.h"
#include "nextcode/SILOptimizer/Utils/InstOptUtils.h"

NEXTCODE_BEGIN_NULLABILITY_ANNOTATIONS

//===----------------------------------------------------------------------===//
//                                BridgedAliasAnalysis
//===----------------------------------------------------------------------===//

BridgedMemoryBehavior BridgedAliasAnalysis::getMemBehavior(BridgedInstruction inst, BridgedValue addr) const {
  return (BridgedMemoryBehavior)aa->computeMemoryBehavior(inst.unbridged(),
                                                          addr.getSILValue());
}

//===----------------------------------------------------------------------===//
//                                BridgedCalleeAnalysis
//===----------------------------------------------------------------------===//

static_assert(sizeof(BridgedCalleeAnalysis::CalleeList) >= sizeof(nextcode::CalleeList),
              "BridgedCalleeAnalysis::CalleeList has wrong size");

bool BridgedCalleeAnalysis::CalleeList::isIncomplete() const {
  return unbridged().isIncomplete();
}

NeXTCodeInt BridgedCalleeAnalysis::CalleeList::getCount() const {
  return unbridged().getCount();
}

BridgedFunction BridgedCalleeAnalysis::CalleeList::getCallee(NeXTCodeInt index) const {
  return {unbridged().get((unsigned)index)};
}

//===----------------------------------------------------------------------===//
//                          BridgedDeadEndBlocksAnalysis
//===----------------------------------------------------------------------===//

bool BridgedDeadEndBlocksAnalysis::isDeadEnd(BridgedBasicBlock block) const {
  return deb->isDeadEnd(block.unbridged());
}

//===----------------------------------------------------------------------===//
//                      BridgedDomTree, BridgedPostDomTree
//===----------------------------------------------------------------------===//

bool BridgedDomTree::dominates(BridgedBasicBlock dominating, BridgedBasicBlock dominated) const {
  return di->dominates(dominating.unbridged(), dominated.unbridged());
}

bool BridgedPostDomTree::postDominates(BridgedBasicBlock dominating, BridgedBasicBlock dominated) const {
  return pdi->dominates(dominating.unbridged(), dominated.unbridged());
}

//===----------------------------------------------------------------------===//
//                            BridgedBasicBlockSet
//===----------------------------------------------------------------------===//

bool BridgedBasicBlockSet::contains(BridgedBasicBlock block) const {
  return set->contains(block.unbridged());
}

bool BridgedBasicBlockSet::insert(BridgedBasicBlock block) const {
  return set->insert(block.unbridged());
}

void BridgedBasicBlockSet::erase(BridgedBasicBlock block) const {
  set->erase(block.unbridged());
}

BridgedFunction BridgedBasicBlockSet::getFunction() const {
  return {set->getFunction()};
}

//===----------------------------------------------------------------------===//
//                            BridgedNodeSet
//===----------------------------------------------------------------------===//

bool BridgedNodeSet::containsValue(BridgedValue value) const {
  return set->contains(value.getSILValue());
}

bool BridgedNodeSet::insertValue(BridgedValue value) const {
  return set->insert(value.getSILValue());
}

void BridgedNodeSet::eraseValue(BridgedValue value) const {
  set->erase(value.getSILValue());
}

bool BridgedNodeSet::containsInstruction(BridgedInstruction inst) const {
  return set->contains(inst.unbridged()->asSILNode());
}

bool BridgedNodeSet::insertInstruction(BridgedInstruction inst) const {
  return set->insert(inst.unbridged()->asSILNode());
}

void BridgedNodeSet::eraseInstruction(BridgedInstruction inst) const {
  set->erase(inst.unbridged()->asSILNode());
}

BridgedFunction BridgedNodeSet::getFunction() const {
  return {set->getFunction()};
}

//===----------------------------------------------------------------------===//
//                            BridgedOperandSet
//===----------------------------------------------------------------------===//

bool BridgedOperandSet::contains(BridgedOperand operand) const {
  return set->contains(operand.op);
}

bool BridgedOperandSet::insert(BridgedOperand operand) const {
  return set->insert(operand.op);
}

void BridgedOperandSet::erase(BridgedOperand operand) const {
  set->erase(operand.op);
}

BridgedFunction BridgedOperandSet::getFunction() const {
  return {set->getFunction()};
}

//===----------------------------------------------------------------------===//
//                            BridgedPassContext
//===----------------------------------------------------------------------===//

BridgedChangeNotificationHandler BridgedPassContext::asNotificationHandler() const {
  return {invocation};
}

void BridgedPassContext::notifyDependencyOnBodyOf(BridgedFunction otherFunction) const {
  // Currently `otherFunction` is ignored. We could design a more accurate dependency system
  // in the pass manager, which considers the actual function. But it's probaboly not worth the effort.
  invocation->getPassManager()->setDependingOnCalleeBodies();
}

BridgedPassContext::SILStage BridgedPassContext::getSILStage() const {
  return (SILStage)invocation->getPassManager()->getModule()->getStage();
}

bool BridgedPassContext::hadError() const {
  return invocation->getPassManager()->getModule()->getASTContext().hadError();
}

bool BridgedPassContext::moduleIsSerialized() const {
  return invocation->getPassManager()->getModule()->isSerialized();
}

bool BridgedPassContext::isTransforming(BridgedFunction function) const {
  return invocation->getFunction() == function.getFunction();
}

BridgedAliasAnalysis BridgedPassContext::getAliasAnalysis() const {
  return {invocation->getPassManager()->getAnalysis<nextcode::AliasAnalysis>(invocation->getFunction())};
}

BridgedCalleeAnalysis BridgedPassContext::getCalleeAnalysis() const {
  return {invocation->getPassManager()->getAnalysis<nextcode::BasicCalleeAnalysis>()};
}

BridgedDeadEndBlocksAnalysis BridgedPassContext::getDeadEndBlocksAnalysis() const {
  auto *dba = invocation->getPassManager()->getAnalysis<nextcode::DeadEndBlocksAnalysis>();
  return {dba->get(invocation->getFunction())};
}

BridgedDomTree BridgedPassContext::getDomTree() const {
  auto *da = invocation->getPassManager()->getAnalysis<nextcode::DominanceAnalysis>();
  return {da->get(invocation->getFunction())};
}

BridgedPostDomTree BridgedPassContext::getPostDomTree() const {
  auto *pda = invocation->getPassManager()->getAnalysis<nextcode::PostDominanceAnalysis>();
  return {pda->get(invocation->getFunction())};
}

BridgedNominalTypeDecl BridgedPassContext::getNeXTCodeArrayDecl() const {
  nextcode::SILModule *mod = invocation->getPassManager()->getModule();
  return {mod->getASTContext().getArrayDecl()};
}

// AST

NEXTCODE_IMPORT_UNSAFE BRIDGED_INLINE
BridgedDiagnosticEngine BridgedPassContext::getDiagnosticEngine() const {
  nextcode::SILModule *mod = invocation->getPassManager()->getModule();
  return {&mod->getASTContext().Diags};
}

// SIL modifications

BridgedBasicBlock BridgedPassContext::splitBlockBefore(BridgedInstruction bridgedInst) const {
  auto *block = bridgedInst.unbridged()->getParent();
  return {block->split(bridgedInst.unbridged()->getIterator())};
}

BridgedBasicBlock BridgedPassContext::splitBlockAfter(BridgedInstruction bridgedInst) const {
  auto *block = bridgedInst.unbridged()->getParent();
  return {block->split(std::next(bridgedInst.unbridged()->getIterator()))};
}

BridgedBasicBlock BridgedPassContext::createBlockAfter(BridgedBasicBlock bridgedBlock) const {
  nextcode::SILBasicBlock *block = bridgedBlock.unbridged();
  return {block->getParent()->createBasicBlockAfter(block)};
}

BridgedBasicBlock BridgedPassContext::appendBlock(BridgedFunction bridgedFunction) const {
  return {bridgedFunction.getFunction()->createBasicBlock()};
}

void BridgedPassContext::eraseInstruction(BridgedInstruction inst) const {
  invocation->eraseInstruction(inst.unbridged());
}

void BridgedPassContext::eraseBlock(BridgedBasicBlock block) const {
  block.unbridged()->eraseFromParent();
}

void BridgedPassContext::moveInstructionBefore(BridgedInstruction inst, BridgedInstruction beforeInst) {
  nextcode::SILBasicBlock::moveInstruction(inst.unbridged(), beforeInst.unbridged());
}

BridgedValue BridgedPassContext::getSILUndef(BridgedType type) const {
  return {nextcode::SILUndef::get(invocation->getFunction(), type.unbridged())};
}

bool BridgedPassContext::optimizeMemoryAccesses(BridgedFunction f) const {
  return nextcode::optimizeMemoryAccesses(f.getFunction(), this->getDomTree().di);
}
bool BridgedPassContext::eliminateDeadAllocations(BridgedFunction f) const {
  return nextcode::eliminateDeadAllocations(f.getFunction(),
                                         this->getDomTree().di);
}

BridgedBasicBlockSet BridgedPassContext::allocBasicBlockSet() const {
  return {invocation->allocBlockSet()};
}

void BridgedPassContext::freeBasicBlockSet(BridgedBasicBlockSet set) const {
  invocation->freeBlockSet(set.set);
}

BridgedNodeSet BridgedPassContext::allocNodeSet() const {
  return {invocation->allocNodeSet()};
}

void BridgedPassContext::freeNodeSet(BridgedNodeSet set) const {
  invocation->freeNodeSet(set.set);
}

BridgedOperandSet BridgedPassContext::allocOperandSet() const {
  return {invocation->allocOperandSet()};
}

void BridgedPassContext::freeOperandSet(BridgedOperandSet set) const {
  invocation->freeOperandSet(set.set);
}

void BridgedPassContext::notifyInvalidatedStackNesting() const {
  invocation->setNeedFixStackNesting(true);
}

bool BridgedPassContext::getNeedFixStackNesting() const {
  return invocation->getNeedFixStackNesting();
}

NeXTCodeInt BridgedPassContext::Slab::getCapacity() {
  return (NeXTCodeInt)nextcode::FixedSizeSlabPayload::capacity;
}

BridgedPassContext::Slab::Slab(nextcode::FixedSizeSlab * _Nullable slab) {
  if (slab) {
    data = slab;
    assert((void *)data == slab->dataFor<void>());
  }
}

nextcode::FixedSizeSlab * _Nullable BridgedPassContext::Slab::getSlab() const {
  if (data)
    return static_cast<nextcode::FixedSizeSlab *>(data);
  return nullptr;
}

BridgedPassContext::Slab BridgedPassContext::Slab::getNext() const {
  return &*std::next(getSlab()->getIterator());
}

BridgedPassContext::Slab BridgedPassContext::Slab::getPrevious() const {
  return &*std::prev(getSlab()->getIterator());
}

BridgedPassContext::Slab BridgedPassContext::allocSlab(Slab afterSlab) const {
  return invocation->allocSlab(afterSlab.getSlab());
}

BridgedPassContext::Slab BridgedPassContext::freeSlab(Slab slab) const {
  return invocation->freeSlab(slab.getSlab());
}

OptionalBridgedFunction BridgedPassContext::getFirstFunctionInModule() const {
  nextcode::SILModule *mod = invocation->getPassManager()->getModule();
  if (mod->getFunctions().empty())
    return {nullptr};
  return {&*mod->getFunctions().begin()};
}

OptionalBridgedFunction BridgedPassContext::getNextFunctionInModule(BridgedFunction function) {
  auto *f = function.getFunction();
  auto nextIter = std::next(f->getIterator());
  if (nextIter == f->getModule().getFunctions().end())
    return {nullptr};
  return {&*nextIter};
}

OptionalBridgedGlobalVar BridgedPassContext::getFirstGlobalInModule() const {
  nextcode::SILModule *mod = invocation->getPassManager()->getModule();
  if (mod->getSILGlobals().empty())
    return {nullptr};
  return {&*mod->getSILGlobals().begin()};
}

OptionalBridgedGlobalVar BridgedPassContext::getNextGlobalInModule(BridgedGlobalVar global) {
  auto *g = global.getGlobal();
  auto nextIter = std::next(g->getIterator());
  if (nextIter == g->getModule().getSILGlobals().end())
    return {nullptr};
  return {&*nextIter};
}

BridgedPassContext::VTableArray BridgedPassContext::getVTables() const {
  nextcode::SILModule *mod = invocation->getPassManager()->getModule();
  auto vTables = mod->getVTables();
  return {vTables.data(), (NeXTCodeInt)vTables.size()};
}

OptionalBridgedWitnessTable BridgedPassContext::getFirstWitnessTableInModule() const {
  nextcode::SILModule *mod = invocation->getPassManager()->getModule();
  if (mod->getWitnessTables().empty())
    return {nullptr};
  return {&*mod->getWitnessTables().begin()};
}

OptionalBridgedWitnessTable BridgedPassContext::getNextWitnessTableInModule(BridgedWitnessTable table) {
  auto *t = table.table;
  auto nextIter = std::next(t->getIterator());
  if (nextIter == t->getModule().getWitnessTables().end())
    return {nullptr};
  return {&*nextIter};
}

OptionalBridgedDefaultWitnessTable BridgedPassContext::getFirstDefaultWitnessTableInModule() const {
  nextcode::SILModule *mod = invocation->getPassManager()->getModule();
  if (mod->getDefaultWitnessTables().empty())
    return {nullptr};
  return {&*mod->getDefaultWitnessTables().begin()};
}

OptionalBridgedDefaultWitnessTable BridgedPassContext::
getNextDefaultWitnessTableInModule(BridgedDefaultWitnessTable table) {
  auto *t = table.table;
  auto nextIter = std::next(t->getIterator());
  if (nextIter == t->getModule().getDefaultWitnessTables().end())
    return {nullptr};
  return {&*nextIter};
}

OptionalBridgedFunction BridgedPassContext::lookupFunction(BridgedStringRef name) const {
  nextcode::SILModule *mod = invocation->getPassManager()->getModule();
  return {mod->lookUpFunction(name.unbridged())};
}

OptionalBridgedFunction BridgedPassContext::loadFunction(BridgedStringRef name, bool loadCalleesRecursively) const {
  nextcode::SILModule *mod = invocation->getPassManager()->getModule();
  return {mod->loadFunction(name.unbridged(),
                            loadCalleesRecursively
                                ? nextcode::SILModule::LinkingMode::LinkAll
                                : nextcode::SILModule::LinkingMode::LinkNormal)};
}

void BridgedPassContext::loadFunction(BridgedFunction function, bool loadCalleesRecursively) const {
  nextcode::SILModule *mod = invocation->getPassManager()->getModule();
  mod->loadFunction(function.getFunction(),
                    loadCalleesRecursively ? nextcode::SILModule::LinkingMode::LinkAll
                                           : nextcode::SILModule::LinkingMode::LinkNormal);
}

BridgedSubstitutionMap BridgedPassContext::getContextSubstitutionMap(BridgedType type) const {
  nextcode::SILType ty = type.unbridged();
  auto *ntd = ty.getASTType()->getAnyNominal();
  auto *mod = invocation->getPassManager()->getModule()->getNeXTCodeModule();
  return ty.getASTType()->getContextSubstitutionMap(mod, ntd);
}

BridgedType BridgedPassContext::getBuiltinIntegerType(NeXTCodeInt bitWidth) const {
  auto &ctxt = invocation->getPassManager()->getModule()->getASTContext();
  return nextcode::SILType::getBuiltinIntegerType(bitWidth, ctxt);
}

void BridgedPassContext::beginTransformFunction(BridgedFunction function) const {
  invocation->beginTransformFunction(function.getFunction());
}

void BridgedPassContext::endTransformFunction() const {
  invocation->endTransformFunction();
}

bool BridgedPassContext::continueWithNextSubpassRun(OptionalBridgedInstruction inst) const {
  nextcode::SILPassManager *pm = invocation->getPassManager();
  return pm->continueWithNextSubpassRun(
      inst.unbridged(), invocation->getFunction(), invocation->getTransform());
}

BridgedPassContext BridgedPassContext::initializeNestedPassContext(BridgedFunction newFunction) const {
  return { invocation->initializeNestedNeXTCodePassInvocation(newFunction.getFunction()) }; 
}

void BridgedPassContext::deinitializedNestedPassContext() const {
  invocation->deinitializeNestedNeXTCodePassInvocation();
}

void BridgedPassContext::SSAUpdater_initialize(
    BridgedFunction function, BridgedType type,
    BridgedValue::Ownership ownership) const {
  invocation->initializeSSAUpdater(function.getFunction(), type.unbridged(),
                                   BridgedValue::castToOwnership(ownership));
}

void BridgedPassContext::addFunctionToPassManagerWorklist(
    BridgedFunction newFunction, BridgedFunction oldFunction) const {
  nextcode::SILPassManager *pm = invocation->getPassManager();
  pm->addFunctionToWorklist(newFunction.getFunction(),
                            oldFunction.getFunction());
}

void BridgedPassContext::SSAUpdater_addAvailableValue(BridgedBasicBlock block, BridgedValue value) const {
  invocation->getSSAUpdater()->addAvailableValue(block.unbridged(),
                                                 value.getSILValue());
}

BridgedValue BridgedPassContext::SSAUpdater_getValueAtEndOfBlock(BridgedBasicBlock block) const {
  return {invocation->getSSAUpdater()->getValueAtEndOfBlock(block.unbridged())};
}

BridgedValue BridgedPassContext::SSAUpdater_getValueInMiddleOfBlock(BridgedBasicBlock block) const {
  return {
      invocation->getSSAUpdater()->getValueInMiddleOfBlock(block.unbridged())};
}

bool BridgedPassContext::enableStackProtection() const {
  nextcode::SILModule *mod = invocation->getPassManager()->getModule();
  return mod->getOptions().EnableStackProtection;
}

bool BridgedPassContext::hasFeature(BridgedFeature feature) const {
  nextcode::SILModule *mod = invocation->getPassManager()->getModule();
  return mod->getASTContext().LangOpts.hasFeature((nextcode::Feature)feature);
}

bool BridgedPassContext::enableMoveInoutStackProtection() const {
  nextcode::SILModule *mod = invocation->getPassManager()->getModule();
  return mod->getOptions().EnableMoveInoutStackProtection;
}

BridgedPassContext::AssertConfiguration BridgedPassContext::getAssertConfiguration() const {
  nextcode::SILModule *mod = invocation->getPassManager()->getModule();
  return (AssertConfiguration)mod->getOptions().AssertConfig;
}

static_assert((int)BridgedPassContext::SILStage::Raw == (int)nextcode::SILStage::Raw);
static_assert((int)BridgedPassContext::SILStage::Canonical == (int)nextcode::SILStage::Canonical);
static_assert((int)BridgedPassContext::SILStage::Lowered == (int)nextcode::SILStage::Lowered);

static_assert((int)BridgedPassContext::AssertConfiguration::Debug == (int)nextcode::SILOptions::Debug);
static_assert((int)BridgedPassContext::AssertConfiguration::Release == (int)nextcode::SILOptions::Release);
static_assert((int)BridgedPassContext::AssertConfiguration::Unchecked == (int)nextcode::SILOptions::Unchecked);

NEXTCODE_END_NULLABILITY_ANNOTATIONS

#endif
