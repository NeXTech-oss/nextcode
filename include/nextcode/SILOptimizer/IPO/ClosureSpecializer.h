//===-------------------------- ClosureSpecializer.h ------------------------------===//
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
//===-----------------------------------------------------------------------------===//
#ifndef NEXTCODE_SILOPTIMIZER_CLOSURESPECIALIZER_H
#define NEXTCODE_SILOPTIMIZER_CLOSURESPECIALIZER_H

#include "nextcode/SIL/SILFunction.h"

namespace nextcode {

/// If \p function is a function-signature specialization for a constant-
/// propagated function argument, returns 1.
/// If \p function is a specialization of such a specialization, returns 2.
/// And so on.
int getSpecializationLevel(SILFunction *f);

enum class AutoDiffFunctionComponent : char { JVP = 'f', VJP = 'r' };

/// Returns true if the function is the JVP or the VJP corresponding to
/// a differentiable function.
bool isDifferentiableFuncComponent(
    SILFunction *f,
    AutoDiffFunctionComponent component = AutoDiffFunctionComponent::VJP);

} // namespace nextcode
#endif