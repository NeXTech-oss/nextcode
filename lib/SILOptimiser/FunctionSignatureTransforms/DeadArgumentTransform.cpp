//===--- DeadArgumentTransform.cpp ----------------------------------------===//
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

#define DEBUG_TYPE "fso-dead-argument-transform"
#include "FunctionSignatureOpts.h"
#include "nextcode/SIL/DebugUtils.h"
#include "llvm/Support/CommandLine.h"

using namespace nextcode;

static llvm::cl::opt<bool> FSODisableDeadArgument(
    "sil-fso-disable-dead-argument",
    llvm::cl::desc("Do not perform dead argument elimination during FSO. "
                   "Intended only for testing purposes"));

bool FunctionSignatureTransform::DeadArgumentAnalyzeParameters() {
  if (FSODisableDeadArgument)
    return false;

  // Did we decide we should optimize any parameter?
  SILFunction *F = TransformDescriptor.OriginalFunction;
  bool SignatureOptimize = false;
  auto Args = F->begin()->getSILFunctionArguments();
  auto OrigShouldModifySelfArgument =
      TransformDescriptor.shouldModifySelfArgument;
  // Analyze the argument information.
  for (unsigned i : indices(Args)) {
    ArgumentDescriptor &A = TransformDescriptor.ArgumentDescList[i];
    if (!A.PInfo.has_value()) {
      // It is not an argument. It could be an indirect result.
      continue;
    }

    if (!A.canOptimizeLiveArg()) {
      continue;
    }

    // Check whether argument is dead.
    if (!hasNonTrivialNonDebugTransitiveUsers(Args[i])) {
      A.IsEntirelyDead = true;
      SignatureOptimize = true;
      if (Args[i]->isSelf())
        TransformDescriptor.shouldModifySelfArgument = true;
    }
  }

  if (F->getLoweredFunctionType()->isPolymorphic()) {
    // If the set of dead arguments contains only type arguments,
    // don't remove them, because it would produce a slower code
    // for generic functions.
    bool HasNonTypeDeadArguments = false;
    for (auto &AD : TransformDescriptor.ArgumentDescList) {
      if (AD.IsEntirelyDead &&
          !isa<AnyMetatypeType>(AD.Arg->getType().getASTType())) {
        HasNonTypeDeadArguments = true;
        break;
      }
    }

    if (!HasNonTypeDeadArguments) {
      for (auto &AD : TransformDescriptor.ArgumentDescList) {
        if (AD.IsEntirelyDead) {
          AD.IsEntirelyDead = false;
        }
      }
      TransformDescriptor.shouldModifySelfArgument =
          OrigShouldModifySelfArgument;
      SignatureOptimize = false;
    }
  }

  return SignatureOptimize;
}

void FunctionSignatureTransform::DeadArgumentTransformFunction() {
  SILFunction *F = TransformDescriptor.OriginalFunction;
  SILBasicBlock *BB = &*F->begin();
  for (const ArgumentDescriptor &AD : TransformDescriptor.ArgumentDescList) {
    if (!AD.IsEntirelyDead)
      continue;
    eraseUsesOfValue(BB->getArgument(AD.Index));
  }
}

void FunctionSignatureTransform::DeadArgumentFinalizeOptimizedFunction() {
  auto *BB = &*TransformDescriptor.OptimizedFunction.get()->begin();
  // Remove any dead argument starting from the last argument to the first.
  for (ArgumentDescriptor &AD : reverse(TransformDescriptor.ArgumentDescList)) {
    if (!AD.IsEntirelyDead)
      continue;
    AD.WasErased = true;
    BB->eraseArgument(AD.Arg->getIndex());
  }
}
