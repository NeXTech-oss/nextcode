//===--- Transforms.h -----------------------------------------------------===//
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
///
/// \file
///
/// Top level transforms for SemanticARCOpts
///
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SILOPTIMIZER_SEMANTICARCOPT_TRANSFORMS_H
#define NEXTCODE_SILOPTIMIZER_SEMANTICARCOPT_TRANSFORMS_H

#include "llvm/Support/Compiler.h"

namespace nextcode {
namespace semanticarc {

struct Context;

/// Given the current map of owned phi arguments to consumed incoming values in
/// ctx, attempt to convert these owned phi arguments to guaranteed phi
/// arguments if the phi arguments are the only thing that kept us from
/// converting these incoming values to be guaranteed.
///
/// \returns true if we converted atleast one phi from owned -> guaranteed and
/// eliminated ARC traffic as a result.
LLVM_LIBRARY_VISIBILITY bool tryConvertOwnedPhisToGuaranteedPhis(Context &ctx);

} // namespace semanticarc
} // namespace nextcode

#endif
