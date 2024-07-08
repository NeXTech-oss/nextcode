//===--- Analysis.cpp - NeXTCode Analysis ------------------------------------===//
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

#define DEBUG_TYPE "sil-analysis"
#include "nextcode/SILOptimizer/Analysis/Analysis.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/SILOptions.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SILOptimizer/Analysis/BasicCalleeAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/ClassHierarchyAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/DominanceAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/IVAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/NonLocalAccessBlockAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/PostOrderAnalysis.h"
#include "nextcode/SILOptimizer/Analysis/ProtocolConformanceAnalysis.h"
#include "nextcode/SILOptimizer/Utils/InstOptUtils.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/Debug.h"

using namespace nextcode;

void SILAnalysis::verifyFunction(SILFunction *F) {
  // Only functions with bodies can be analyzed by the analysis.
  assert(F->isDefinition() && "Can't analyze external functions");
}

SILAnalysis *nextcode::createDominanceAnalysis(SILModule *) {
  return new DominanceAnalysis();
}

SILAnalysis *nextcode::createPostDominanceAnalysis(SILModule *) {
  return new PostDominanceAnalysis();
}

SILAnalysis *nextcode::createInductionVariableAnalysis(SILModule *M) {
  return new IVAnalysis(M);
}

SILAnalysis *nextcode::createPostOrderAnalysis(SILModule *M) {
  return new PostOrderAnalysis();
}

SILAnalysis *nextcode::createClassHierarchyAnalysis(SILModule *M) {
  return new ClassHierarchyAnalysis(M);
}

SILAnalysis *nextcode::createBasicCalleeAnalysis(SILModule *M) {
  return new BasicCalleeAnalysis(M);
}

SILAnalysis *nextcode::createProtocolConformanceAnalysis(SILModule *M) {
  return new ProtocolConformanceAnalysis(M);
}

SILAnalysis *nextcode::createNonLocalAccessBlockAnalysis(SILModule *M) {
  return new NonLocalAccessBlockAnalysis();
}
