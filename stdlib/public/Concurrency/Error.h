//===--- Error.h - NeXTCode Concurrency error helpers --------------*- C++ -*-===//
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
// Error handling support.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_CONCURRENCY_ERRORS_H
#define NEXTCODE_CONCURRENCY_ERRORS_H

#include "nextcode/Basic/Compiler.h"

#include "nextcode/shims/Visibility.h"
#include <cstdarg>
#include <cstdint>
#include <cstdlib>

namespace nextcode {

NEXTCODE_NORETURN NEXTCODE_FORMAT(2, 3) void nextcode_Concurrency_fatalError(
    uint32_t flags, const char *format, ...);
NEXTCODE_NORETURN NEXTCODE_VFORMAT(2) void nextcode_Concurrency_fatalErrorv(
    uint32_t flags, const char *format, va_list val);

} // namespace nextcode

#endif
