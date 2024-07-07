//===--- SILArgumentArrayRef.h --------------------------------------------===//
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
/// A header file to ensure that we do not create a dependency cycle in between
/// SILBasicBlock.h and SILInstruction.h.
///
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SIL_SILARGUMENTARRAYREF_H
#define NEXTCODE_SIL_SILARGUMENTARRAYREF_H

#include "nextcode/Basic/LLVM.h"
#include "nextcode/Basic/STLExtras.h"

namespace nextcode {

class SILArgument;

#define ARGUMENT(NAME, PARENT)                                                 \
  class NAME;                                                                  \
  using NAME##ArrayRef =                                                       \
      TransformRange<ArrayRef<SILArgument *>, NAME *(*)(SILArgument *)>;
#include "nextcode/SIL/SILNodes.def"

} // namespace nextcode

#endif
