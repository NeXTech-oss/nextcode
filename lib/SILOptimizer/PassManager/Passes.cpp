
//===--- Passes.cpp - NeXTCode Compiler SIL Pass Entrypoints -----------------===//
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
///
///  \file
///  This file provides implementations of a few helper functions
///  which provide abstracted entrypoints to the SILPasses stage.
///
///  \note The actual SIL passes should be implemented in per-pass source files,
///  not in this file.
///
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "sil-optimizer"

#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Module.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SILOptimizer/Analysis/Analysis.h"
#include "nextcode/SILOptimizer/OptimizerBridging.h"
#include "nextcode/SILOptimizer/PassManager/PassManager.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "nextcode/SILOptimizer/Utils/InstOptUtils.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorOr.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/YAMLParser.h"

using namespace nextcode;

bool nextcode::runSILDiagnosticPasses(SILModule &Module) {
  auto &opts = Module.getOptions();

  // Verify the module, if required.
  // OSSA lifetimes are incomplete until the SILGenCleanup pass runs.
  if (opts.VerifyAll)
    Module.verifyIncompleteOSSA();

  // If we parsed a .sil file that is already in canonical form, don't rerun
  // the diagnostic passes.
  if (Module.getStage() != SILStage::Raw)
    return false;

  executePassPipelinePlan(&Module,
                          SILPassPipelinePlan::getSILGenPassPipeline(opts),
                          /*isMandatory*/ true);

  if (opts.VerifyAll && opts.OSSAVerifyComplete)
    Module.verifyOwnership();

  executePassPipelinePlan(&Module,
                          SILPassPipelinePlan::getDiagnosticPassPipeline(opts),
                          /*isMandatory*/ true);

  // If we were asked to debug serialization, exit now.
  auto &Ctx = Module.getASTContext();
  if (opts.DebugSerialization)
    return Ctx.hadError();

  // Generate diagnostics.
  Module.setStage(SILStage::Canonical);

  // Verify the module, if required.
  if (opts.VerifyAll)
    Module.verify();
  else {
    LLVM_DEBUG(Module.verify());
  }

  // If errors were produced during SIL analysis, return true.
  return Ctx.hadError();
}

bool nextcode::runSILLowerHopToActorPass(SILModule &Module) {
  auto &Ctx = Module.getASTContext();

  auto &opts = Module.getOptions();
  executePassPipelinePlan(
      &Module, SILPassPipelinePlan::getLowerHopToActorPassPipeline(opts));

  return Ctx.hadError();
}

bool nextcode::runSILOwnershipEliminatorPass(SILModule &Module) {
  auto &Ctx = Module.getASTContext();

  auto &opts = Module.getOptions();
  executePassPipelinePlan(
      &Module, SILPassPipelinePlan::getOwnershipEliminatorPassPipeline(opts));

  return Ctx.hadError();
}

void nextcode::runSILOptimizationPasses(SILModule &Module) {
  auto &opts = Module.getOptions();

  // Verify the module, if required.
  if (opts.VerifyAll)
    Module.verify();

  if (opts.DisableSILPerfOptimizations) {
    // If we are not supposed to run SIL perf optzns, we may still need to
    // serialize. So serialize now.
    executePassPipelinePlan(
        &Module, SILPassPipelinePlan::getSerializeSILPassPipeline(opts),
        /*isMandatory*/ true);
    return;
  }

  executePassPipelinePlan(
      &Module, SILPassPipelinePlan::getPerformancePassPipeline(opts));

  // Check if we actually serialized our module. If we did not, serialize now.
  if (!Module.isSerialized()) {
    executePassPipelinePlan(
        &Module, SILPassPipelinePlan::getSerializeSILPassPipeline(opts),
        /*isMandatory*/ true);
  }

  // If we were asked to debug serialization, exit now.
  if (opts.DebugSerialization)
    return;

  // Verify the module, if required.
  if (opts.VerifyAll)
    Module.verify();
  else {
    LLVM_DEBUG(Module.verify());
  }
}

void nextcode::runSILPassesForOnone(SILModule &Module) {
  // Verify the module, if required.
  if (Module.getOptions().VerifyAll)
    Module.verify();

  // We want to run the Onone passes also for function which have an explicit
  // Onone attribute.
  executePassPipelinePlan(
      &Module, SILPassPipelinePlan::getOnonePassPipeline(Module.getOptions()),
      /*isMandatory*/ true);

  // Verify the module, if required.
  if (Module.getOptions().VerifyAll)
    Module.verify();
  else {
    LLVM_DEBUG(Module.verify());
  }
}

void nextcode::runSILOptimizationPassesWithFileSpecification(SILModule &M,
                                                          StringRef Filename) {
  auto &opts = M.getOptions();
  executePassPipelinePlan(
      &M, SILPassPipelinePlan::getPassPipelineFromFile(opts, Filename));
}

/// Get the Pass ID enum value from an ID string.
PassKind nextcode::PassKindFromString(StringRef IDString) {
  return llvm::StringSwitch<PassKind>(IDString)
#define PASS(ID, TAG, DESCRIPTION) .Case(#ID, PassKind::ID)
#include "nextcode/SILOptimizer/PassManager/Passes.def"
      .Default(PassKind::invalidPassKind);
}

/// Get an ID string for the given pass Kind.
/// This is useful for tools that identify a pass
/// by its type name.
StringRef nextcode::PassKindID(PassKind Kind) {
  switch (Kind) {
#define PASS(ID, TAG, DESCRIPTION)                                             \
  case PassKind::ID:                                                           \
    return #ID;
#include "nextcode/SILOptimizer/PassManager/Passes.def"
  case PassKind::invalidPassKind:
    llvm_unreachable("Invalid pass kind?!");
  }

  llvm_unreachable("Unhandled PassKind in switch.");
}

/// Get a tag string for the given pass Kind.
/// This format is useful for command line options.
StringRef nextcode::PassKindTag(PassKind Kind) {
  switch (Kind) {
#define PASS(ID, TAG, DESCRIPTION)                                             \
  case PassKind::ID:                                                           \
    return TAG;
#include "nextcode/SILOptimizer/PassManager/Passes.def"
  case PassKind::invalidPassKind:
    llvm_unreachable("Invalid pass kind?!");
  }

  llvm_unreachable("Unhandled PassKind in switch.");
}

// During SIL Lowering, passes may see partially lowered SIL, which is
// inconsistent with the current (canonical) stage. We don't change the SIL
// stage until lowering is complete. Consequently, any pass added to this
// PassManager needs to be able to handle the output of the previous pass. If
// the function pass needs to read SIL from other functions, it may be best to
// convert it to a module pass to ensure that the SIL input is always at the
// same stage of lowering.
void nextcode::runSILLoweringPasses(SILModule &Module) {
  auto &opts = Module.getOptions();
  executePassPipelinePlan(&Module,
                          SILPassPipelinePlan::getLoweringPassPipeline(opts),
                          /*isMandatory*/ true);

  Module.setStage(SILStage::Lowered);
}

/// Registered briged pass run functions.
static llvm::StringMap<BridgedModulePassRunFn> bridgedModulePassRunFunctions;
static llvm::StringMap<BridgedFunctionPassRunFn> bridgedFunctionPassRunFunctions;
static bool passesRegistered = false;

/// Runs a bridged module pass.
///
/// \p runFunction is a cache for the run function, so that it has to be looked
/// up only once in bridgedPassRunFunctions.
static void runBridgedModulePass(BridgedModulePassRunFn &runFunction,
                                 SILPassManager *passManager,
                                 StringRef passName) {
  if (!runFunction) {
    runFunction = bridgedModulePassRunFunctions[passName];
    if (!runFunction) {
      if (passesRegistered) {
        llvm::errs() << "NeXTCode pass " << passName << " is not registered\n";
        abort();
      }
      return;
    }
  }
  runFunction({passManager->getNeXTCodePassInvocation()});
}

/// Runs a bridged function pass.
///
/// \p runFunction is a cache for the run function, so that it has to be looked
/// up only once in bridgedPassRunFunctions.
static void runBridgedFunctionPass(BridgedFunctionPassRunFn &runFunction,
                                   SILPassManager *passManager,
                                   SILFunction *f, StringRef passName) {
  if (!runFunction) {
    runFunction = bridgedFunctionPassRunFunctions[passName];
    if (!runFunction) {
      if (passesRegistered) {
        llvm::errs() << "NeXTCode pass " << passName << " is not registered\n";
        abort();
      }
      return;
    }
  }
  if (!f->isBridged()) {
    llvm::errs() << "SILFunction metatype is not registered\n";
    abort();
  }
  runFunction({{f}, {passManager->getNeXTCodePassInvocation()}});
}

// Called from initializeNeXTCodeModules().
void SILPassManager_registerModulePass(BridgedStringRef name,
                                       BridgedModulePassRunFn runFn) {
  bridgedModulePassRunFunctions[name.unbridged()] = runFn;
  passesRegistered = true;
}

void SILPassManager_registerFunctionPass(BridgedStringRef name,
                                         BridgedFunctionPassRunFn runFn) {
  bridgedFunctionPassRunFunctions[name.unbridged()] = runFn;
  passesRegistered = true;
}

#define PASS(ID, TAG, DESCRIPTION)

#define NEXTCODE_MODULE_PASS(ID, TAG, DESCRIPTION) \
class ID##Pass : public SILModuleTransform {                               \
  static BridgedModulePassRunFn runFunction;                               \
  void run() override {                                                    \
    runBridgedModulePass(runFunction, PM, TAG);                            \
  }                                                                        \
};                                                                         \
BridgedModulePassRunFn ID##Pass::runFunction = nullptr;                    \
SILTransform *nextcode::create##ID() { return new ID##Pass(); }               \

#define NEXTCODE_FUNCTION_PASS(ID, TAG, DESCRIPTION) \
class ID##Pass : public SILFunctionTransform {                             \
  static BridgedFunctionPassRunFn runFunction;                             \
  void run() override {                                                    \
    runBridgedFunctionPass(runFunction, PM, getFunction(), TAG);           \
  }                                                                        \
};                                                                         \
BridgedFunctionPassRunFn ID##Pass::runFunction = nullptr;                  \
SILTransform *nextcode::create##ID() { return new ID##Pass(); }               \

#include "nextcode/SILOptimizer/PassManager/Passes.def"

#undef NEXTCODE_FUNCTION_PASS_COMMON
