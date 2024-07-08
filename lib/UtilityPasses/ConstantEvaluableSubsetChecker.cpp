//===--ConstantEvaluableSubsetChecker.cpp - Test Constant Evaluable NeXTCode--===//
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

// This file implements a pass for checking the constant evaluability of NeXTCode
// code snippets. This pass is only used in tests and is not part of the
// compilation pipeline.

#define DEBUG_TYPE "sil-constant-evaluable-subset-checker"
#include "nextcode/AST/DiagnosticsSIL.h"
#include "nextcode/AST/Module.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Demangling/Demangle.h"
#include "nextcode/SIL/CFG.h"
#include "nextcode/SIL/SILConstants.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "nextcode/SILOptimizer/Utils/ConstExpr.h"

using namespace nextcode;

namespace {

static const StringRef testDriverSemanticsAttr = "test_driver";

template <typename... T, typename... U>
static InFlightDiagnostic diagnose(ASTContext &Context, SourceLoc loc,
                                   Diag<T...> diag, U &&... args) {
  return Context.Diags.diagnose(loc, diag, std::forward<U>(args)...);
}

static std::string demangleSymbolName(StringRef name) {
  Demangle::DemangleOptions options;
  options.QualifyEntities = false;
  return Demangle::demangleSymbolAsString(name, options);
}

/// A SILModule pass that invokes the constant evaluator on all functions in a
/// SILModule with the semantics attribute "test_driver". Each "test_driver"
/// must invoke one or more functions in the module annotated as
/// "constant_evaluable" with constant arguments.
class ConstantEvaluableSubsetChecker : public SILModuleTransform {

  llvm::SmallPtrSet<SILFunction *, 4> constantEvaluableFunctions;
  llvm::SmallPtrSet<SILFunction *, 4> evaluatedFunctions;

  /// Evaluate the body of \c fun with the constant evaluator. \c fun must be
  /// annotated as "test_driver" and must invoke one or more functions annotated
  /// as "constant_evaluable" with constant arguments. Emit diagnostics if the
  /// evaluation of any  "constant_evaluable" function called in the body of
  /// \c fun fails.
  void constantEvaluateDriver(SILFunction *fun) {
    ASTContext &astContext = fun->getASTContext();

    // Create a step evaluator and run it on the function.
    SymbolicValueBumpAllocator allocator;
    ConstExprStepEvaluator stepEvaluator(allocator, fun,
                                         getOptions().AssertConfig,
                                         /*trackCallees*/ true);
    bool previousEvaluationHadFatalError = false;

    for (auto currI = fun->getEntryBlock()->begin();;) {
      auto *inst = &(*currI);

      if (isa<ReturnInst>(inst))
        break;

      auto *applyInst = dyn_cast<ApplyInst>(inst);
      SILFunction *callee = nullptr;
      if (applyInst) {
        callee = applyInst->getReferencedFunctionOrNull();
      }

      std::optional<SILBasicBlock::iterator> nextInstOpt;
      std::optional<SymbolicValue> errorVal;

      if (!applyInst || !callee || !isConstantEvaluable(callee)) {

        // Ignore these instructions if we had a fatal error already.
        if (previousEvaluationHadFatalError) {
          if (isa<TermInst>(inst)) {
            assert(false && "non-constant control flow in the test driver");
          }
          ++currI;
          continue;
        }

        std::tie(nextInstOpt, errorVal) =
            stepEvaluator.tryEvaluateOrElseMakeEffectsNonConstant(currI);
        if (!nextInstOpt) {
          // This indicates an error in the test driver.
          errorVal->emitUnknownDiagnosticNotes(inst->getLoc());
          assert(false && "non-constant control flow in the test driver");
        }
        currI = nextInstOpt.value();
        continue;
      }

      assert(!previousEvaluationHadFatalError &&
             "cannot continue evaluation of test driver as previous call "
             "resulted in non-skippable evaluation error.");

      // Here, a function annotated as "constant_evaluable" is called.
      llvm::errs() << "@" << demangleSymbolName(callee->getName()) << "\n";
      std::tie(nextInstOpt, errorVal) =
          stepEvaluator.tryEvaluateOrElseMakeEffectsNonConstant(currI);

      if (errorVal) {
        SourceLoc instLoc = inst->getLoc().getSourceLoc();
        diagnose(astContext, instLoc, diag::not_constant_evaluable);
        errorVal->emitUnknownDiagnosticNotes(inst->getLoc());
      }

      if (nextInstOpt) {
        currI = nextInstOpt.value();
        continue;
      }

      // Here, a non-skippable error like "instruction-limit exceeded" has been
      // encountered during evaluation. Proceed to the next instruction but
      // ensure that an assertion failure occurs if there is any instruction
      // to evaluate after this instruction.
      ++currI;
      previousEvaluationHadFatalError = true;
    }

    // For every function seen during the evaluation of this constant evaluable
    // function:
    // 1. Record it so as to detect whether the test drivers in the SILModule
    // cover all function annotated as "constant_evaluable".
    //
    // 2. If the callee is annotated as constant_evaluable and is imported from
    // a different NeXTCode module (other than stdlib), check that the function is
    // marked as Onone. Otherwise, it could have been optimized, which will
    // break constant evaluability.
    for (SILFunction *callee : stepEvaluator.getFuncsCalledDuringEvaluation()) {
      evaluatedFunctions.insert(callee);

      SILModule &calleeModule = callee->getModule();
      if (callee->isAvailableExternally() &&
          hasConstantEvaluableAnnotation(callee) &&
          callee->getOptimizationMode() != OptimizationMode::NoOptimization) {
        diagnose(calleeModule.getASTContext(),
                 callee->getLocation().getSourceLoc(),
                 diag::constexpr_imported_func_not_onone,
                 demangleSymbolName(callee->getName()));
      }
    }
  }

  void run() override {
    SILModule *module = getModule();
    assert(module);

    for (SILFunction &fun : *module) {
      // Record functions annotated as constant evaluable.
      if (hasConstantEvaluableAnnotation(&fun)) {
        constantEvaluableFunctions.insert(&fun);
        continue;
      }

      // Evaluate test drivers.
      if (!fun.hasSemanticsAttr(testDriverSemanticsAttr))
        continue;
      constantEvaluateDriver(&fun);
    }

    // Assert that every function annotated as "constant_evaluable" was covered
    // by a test driver.
    bool error = false;
    for (SILFunction *constEvalFun : constantEvaluableFunctions) {
      if (!evaluatedFunctions.count(constEvalFun)) {
        llvm::errs() << "Error: function "
                     << demangleSymbolName(constEvalFun->getName());
        llvm::errs() << " annotated as constant evaluable";
        llvm::errs() << " does not have a test driver"
                     << "\n";
        error = true;
      }
    }
    assert(!error);
  }
};

} // end anonymous namespace

SILTransform *nextcode::createConstantEvaluableSubsetChecker() {
  return new ConstantEvaluableSubsetChecker();
}
