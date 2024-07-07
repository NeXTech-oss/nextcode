//===--- VJPCloner.h - VJP function generation ----------------*- C++ -*---===//
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
// This file defines a helper class for generating VJP functions for automatic
// differentiation.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SILOPTIMIZER_UTILS_DIFFERENTIATION_VJPCLONER_H
#define NEXTCODE_SILOPTIMIZER_UTILS_DIFFERENTIATION_VJPCLONER_H

#include "nextcode/SILOptimizer/Analysis/DifferentiableActivityAnalysis.h"
#include "nextcode/SILOptimizer/Differentiation/DifferentiationInvoker.h"
#include "nextcode/SILOptimizer/Differentiation/LinearMapInfo.h"
#include "nextcode/SIL/LoopInfo.h"

namespace nextcode {
namespace autodiff {

class ADContext;
class PullbackCloner;

/// A helper class for generating VJP functions.
class VJPCloner final {
  class Implementation;
  Implementation &impl;

public:
  /// Creates a VJP cloner.
  ///
  /// The parent VJP cloner stores the original function and an empty
  /// to-be-generated pullback function.
  explicit VJPCloner(ADContext &context, SILDifferentiabilityWitness *witness,
                     SILFunction *vjp, DifferentiationInvoker invoker);
  ~VJPCloner();

  ADContext &getContext() const;
  SILModule &getModule() const;
  SILFunction &getOriginal() const;
  SILFunction &getVJP() const;
  SILFunction &getPullback() const;
  SILDifferentiabilityWitness *getWitness() const;
  const AutoDiffConfig &getConfig() const;
  DifferentiationInvoker getInvoker() const;
  LinearMapInfo &getPullbackInfo() const;
  SILLoopInfo *getLoopInfo() const;
  const DifferentiableActivityInfo &getActivityInfo() const;

  /// Performs VJP generation on the empty VJP function. Returns true if any
  /// error occurs.
  bool run();
};

} // end namespace autodiff
} // end namespace nextcode

#endif // NEXTCODE_SILOPTIMIZER_UTILS_DIFFERENTIATION_VJPCLONER_H
