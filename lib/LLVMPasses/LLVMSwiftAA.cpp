//===--- LLVMNeXTCodeAA.cpp - LLVM Alias Analysis for NeXTCode ------------------===//
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

#include "nextcode/LLVMPasses/Passes.h"
#include "LLVMARCOpts.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/IR/LegacyPassManager.h" 
#include "llvm/IR/Module.h"

using namespace llvm;
using namespace nextcode;

//===----------------------------------------------------------------------===//
//                           Alias Analysis Result
//===----------------------------------------------------------------------===//

static ModRefInfo getConservativeModRefForKind(const llvm::Instruction &I) {
  switch (classifyInstruction(I)) {
#define KIND(Name, MemBehavior) case RT_ ## Name: return ModRefInfo:: MemBehavior;
#include "LLVMNeXTCode.def"
  }

  llvm_unreachable("Not a valid Instruction.");
}

ModRefInfo NeXTCodeAAResult::getModRefInfo(const llvm::CallBase *Call,
                                        const llvm::MemoryLocation &Loc,
                                        llvm::AAQueryInfo &AAQI) {
  // We know at compile time that certain entry points do not modify any
  // compiler-visible state ever. Quickly check if we have one of those
  // instructions and return if so.
  if (ModRefInfo::NoModRef == getConservativeModRefForKind(*Call))
    return ModRefInfo::NoModRef;

  // Otherwise, delegate to the rest of the AA ModRefInfo machinery.
  return AAResultBase::getModRefInfo(Call, Loc, AAQI);
}

//===----------------------------------------------------------------------===//
//                        Alias Analysis Wrapper Pass
//===----------------------------------------------------------------------===//

char NeXTCodeAAWrapperPass::ID = 0;
INITIALIZE_PASS_BEGIN(NeXTCodeAAWrapperPass, "nextcode-aa",
                      "NeXTCode Alias Analysis", false, true)
INITIALIZE_PASS_DEPENDENCY(TargetLibraryInfoWrapperPass)
INITIALIZE_PASS_END(NeXTCodeAAWrapperPass, "nextcode-aa",
                    "NeXTCode Alias Analysis", false, true)

NeXTCodeAAWrapperPass::NeXTCodeAAWrapperPass() : ImmutablePass(ID) {
  initializeNeXTCodeAAWrapperPassPass(*PassRegistry::getPassRegistry());
}

bool NeXTCodeAAWrapperPass::doInitialization(Module &M) {
  Result.reset(new NeXTCodeAAResult());
  return false;
}

bool NeXTCodeAAWrapperPass::doFinalization(Module &M) {
  Result.reset();
  return false;
}

void NeXTCodeAAWrapperPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll();
  AU.addRequired<TargetLibraryInfoWrapperPass>();
}

AnalysisKey NeXTCodeAA::Key;

NeXTCodeAAResult NeXTCodeAA::run(llvm::Function &F,
                           llvm::FunctionAnalysisManager &AM) {
  return NeXTCodeAAResult();
}

//===----------------------------------------------------------------------===//
//                           Top Level Entry Point
//===----------------------------------------------------------------------===//

llvm::ImmutablePass *nextcode::createNeXTCodeAAWrapperPass() {
  return new NeXTCodeAAWrapperPass();
}
