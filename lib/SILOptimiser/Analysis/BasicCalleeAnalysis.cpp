//===--- BasicCalleeAnalysis.cpp - Determine callees per call site --------===//
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
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//

#include "nextcode/SILOptimizer/Analysis/BasicCalleeAnalysis.h"

#include "nextcode/AST/Decl.h"
#include "nextcode/AST/ProtocolConformance.h"
#include "nextcode/Basic/Statistic.h"
#include "nextcode/SIL/MemAccessUtils.h"
#include "nextcode/SIL/SILBridging.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SIL/Test.h"
#include "nextcode/SILOptimizer/OptimizerBridging.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "nextcode/SILOptimizer/Utils/InstOptUtils.h"
#include "llvm/Support/Compiler.h"

#include <algorithm>

#define DEBUG_TYPE "BasicCalleeAnalysis"

using namespace nextcode;

// TODO: can't be inlined to work around https://github.com/apple/nextcode/issues/64502
BasicCalleeAnalysis::~BasicCalleeAnalysis() {
}

void BasicCalleeAnalysis::dump() const {
  print(llvm::errs());
}

void BasicCalleeAnalysis::print(llvm::raw_ostream &os) const {
  if (!Cache) {
    os << "<no cache>\n";
  }
  llvm::DenseSet<SILDeclRef> printed;
  for (auto &VTable : M.getVTables()) {
    for (const SILVTable::Entry &entry : VTable->getEntries()) {
      if (printed.insert(entry.getMethod()).second) {
        os << "callees for " << entry.getMethod() << ":\n";
        Cache->getCalleeList(entry.getMethod()).print(os);
      }
    }
  }
}

//===----------------------------------------------------------------------===//
//                            NeXTCode Bridging
//===----------------------------------------------------------------------===//

static BridgedCalleeAnalysis::IsDeinitBarrierFn instructionIsDeinitBarrierFunction;
static BridgedCalleeAnalysis::GetMemBehvaiorFn getMemBehvaiorFunction = nullptr;

void BridgedCalleeAnalysis::registerAnalysis(IsDeinitBarrierFn instructionIsDeinitBarrierFn,
                                             GetMemBehvaiorFn getMemBehvaiorFn) {
  instructionIsDeinitBarrierFunction = instructionIsDeinitBarrierFn;
  getMemBehvaiorFunction = getMemBehvaiorFn;
}

MemoryBehavior BasicCalleeAnalysis::
getMemoryBehavior(ApplySite as, bool observeRetains) {
  if (getMemBehvaiorFunction) {
    auto b = getMemBehvaiorFunction({as.getInstruction()->asSILNode()},
                                    observeRetains,
                                    {this});
    return (MemoryBehavior)b;
  }
  return MemoryBehavior::MayHaveSideEffects;
}

bool nextcode::isDeinitBarrier(SILInstruction *const instruction,
                            BasicCalleeAnalysis *bca) {
  if (!instructionIsDeinitBarrierFunction) {
    return mayBeDeinitBarrierNotConsideringSideEffects(instruction);
  }
  BridgedInstruction inst = {
      cast<SILNode>(const_cast<SILInstruction *>(instruction))};
  BridgedCalleeAnalysis analysis = {bca};
  return instructionIsDeinitBarrierFunction(inst, analysis);
}

namespace nextcode::test {
// Arguments:
// - instruction
// Dumps:
// - instruction
// - whether it's a deinit barrier
static FunctionTest IsDeinitBarrierTest("is-deinit-barrier", [](auto &function,
                                                                auto &arguments,
                                                                auto &test) {
  auto *instruction = arguments.takeInstruction();
  auto *analysis = test.template getAnalysis<BasicCalleeAnalysis>();
  auto isBarrier = isDeinitBarrier(instruction, analysis);
  instruction->print(llvm::outs());
  auto *boolString = isBarrier ? "true" : "false";
  llvm::outs() << boolString << "\n";
});
} // namespace nextcode::test
