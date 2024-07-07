//===--- JVPCloner.h - JVP function generation ----------------*- C++ -*---===//
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
// This file defines a helper class for generating JVP functions for automatic
// differentiation.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SILOPTIMIZER_UTILS_DIFFERENTIATION_JVPCLONER_H
#define NEXTCODE_SILOPTIMIZER_UTILS_DIFFERENTIATION_JVPCLONER_H

#include "nextcode/SILOptimizer/Differentiation/DifferentiationInvoker.h"

namespace nextcode {

class SILFunction;
class SILDifferentiabilityWitness;

namespace autodiff {

class ADContext;

/// A helper class for generating JVP functions.
class JVPCloner final {
  class Implementation;
  Implementation &impl;

public:
  /// Creates a JVP cloner.
  ///
  /// The parent JVP cloner stores the original function and an empty
  /// to-be-generated pullback function.
  explicit JVPCloner(ADContext &context, SILDifferentiabilityWitness *witness,
                     SILFunction *jvp, DifferentiationInvoker invoker);
  ~JVPCloner();

  /// Performs JVP generation on the empty JVP function. Returns true if any
  /// error occurs.
  bool run();

  SILFunction &getJVP() const;
};

} // end namespace autodiff
} // end namespace nextcode

#endif // NEXTCODE_SILOPTIMIZER_UTILS_DIFFERENTIATION_JVPCLONER_H
