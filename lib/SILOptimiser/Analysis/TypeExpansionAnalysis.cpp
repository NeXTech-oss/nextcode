//===--- TypeExpansionAnalysis.cpp - Type Expansion Analysis --------------===//
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

#define DEBUG_TYPE "typeexpansion-analysis"
#include "nextcode/SILOptimizer/Analysis/TypeExpansionAnalysis.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SILOptimizer/Utils/InstOptUtils.h"
#include "llvm/Support/Debug.h"

using namespace nextcode;

// The TypeExpansion Cache must not grow beyond this size.
// We limit the size of the MB cache to 2**12 because we want to limit the
// memory usage of this cache.
static const int TypeExpansionAnalysisMaxCacheSize = 4096;

const ProjectionPathList &
TypeExpansionAnalysis::getTypeExpansion(SILType B, SILModule *Mod,
                                        TypeExpansionContext context) {
  // Check whether we have the type expansion.
  auto key = std::make_pair(B, context);
  auto Iter = ExpansionCache.find(key);
  //
  if (Iter != ExpansionCache.end()) {
    return Iter->second;
  }

  // Don't expand large types. This would defeat keeping them in memory.
  if (!shouldExpand(*Mod, B)) {
    // Push the empty projection path.
    ProjectionPath P(B);
    ExpansionCache[key].push_back(P);
    return ExpansionCache[key];
  }

  // Flush the cache if the size of the cache is too large.
  if (ExpansionCache.size() > TypeExpansionAnalysisMaxCacheSize) {
    ExpansionCache.clear();
  }

  // Build the type expansion for the leaf nodes.
  ProjectionPath::expandTypeIntoLeafProjectionPaths(B, Mod, context,
                                                    ExpansionCache[key]);
  return ExpansionCache[key];
}

SILAnalysis *nextcode::createTypeExpansionAnalysis(SILModule *M) {
  return new TypeExpansionAnalysis(M);
}
