//===------------------- FunctionBodySkipping.h -----------------*- C++ -*-===//
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

#ifndef NEXTCODE_BASIC_FUNCTIONBODYSKIPPING_H
#define NEXTCODE_BASIC_FUNCTIONBODYSKIPPING_H

#include "llvm/Support/DataTypes.h"

namespace nextcode {

/// Describes the function bodies that can be skipped in type-checking.
enum class FunctionBodySkipping : uint8_t {
  /// Do not skip type-checking for any function bodies.
  None,
  /// Only non-inlinable function bodies should be skipped.
  NonInlinable,
  /// Only non-inlinable functions bodies without type definitions should
  /// be skipped.
  NonInlinableWithoutTypes,
  /// All function bodies should be skipped, where not otherwise required
  /// for type inference.
  All
};

} // end namespace nextcode

#endif // NEXTCODE_BASIC_FUNCTIONBODYSKIPPING_H
