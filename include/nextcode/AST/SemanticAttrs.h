//===--- Semantics.h - Semantics Attribute Definitions -------------*- C++ -*-===//
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
/// Implementation of the matching definition file.
/// This file holds all semantics attributes as constant string literals.
///
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SEMANTICS_H
#define NEXTCODE_SEMANTICS_H

#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/StringRef.h"

namespace nextcode {
namespace semantics {
#define SEMANTICS_ATTR(NAME, C_STR) constexpr static const StringLiteral NAME = C_STR;
#include "SemanticAttrs.def"
}
}

#endif
