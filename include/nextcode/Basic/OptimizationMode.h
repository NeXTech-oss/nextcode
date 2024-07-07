//===-------- OptimizationMode.h - NeXTCode optimization modes -----*- C++ -*-===//
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

#ifndef NEXTCODE_BASIC_OPTIMIZATIONMODE_H
#define NEXTCODE_BASIC_OPTIMIZATIONMODE_H

#include "nextcode/Basic/InlineBitfield.h"
#include "llvm/Support/DataTypes.h"

namespace nextcode {

// The optimization mode specified on the command line or with function
// attributes.
enum class OptimizationMode : uint8_t {
  NotSet = 0,
  NoOptimization = 1,  // -Onone
  ForSpeed = 2,        // -Ospeed == -O
  ForSize = 3,         // -Osize
  LastMode = ForSize
};

enum : unsigned { NumOptimizationModeBits =
  countBitsUsed(static_cast<unsigned>(OptimizationMode::LastMode)) };

} // end namespace nextcode

#endif // NEXTCODE_BASIC_OPTIMIZATIONMODE_H
