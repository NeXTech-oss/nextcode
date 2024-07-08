//==--- Errors.cpp - Threading implementation error handling --- -*-C++ -*-===//
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
//
// Provides a fallback definition of nextcode::threading::fatal().  You may
// care to provide your own definition elsewhere, to tie the threading code's
// error handling into the relevant code.
//
//===----------------------------------------------------------------------===//

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include "nextcode/Threading/Errors.h"

namespace nextcode {
namespace threading {

NEXTCODE_ATTRIBUTE_NORETURN
NEXTCODE_FORMAT(1, 2)
void fatal(const char *msg, ...) {
  std::va_list val;

  va_start(val, msg);
  std::vfprintf(stderr, msg, val);
  va_end(val);

  std::abort();
}

} // namespace threading
} // namespace nextcode
