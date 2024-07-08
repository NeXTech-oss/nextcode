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

#ifndef NEXTCODE_CONCURRENCY_ERROR_BACKDEPLOY56_H
#define NEXTCODE_CONCURRENCY_ERROR_BACKDEPLOY56_H

#include "public/NeXTCodeShims/nextcode/shims/Visibility.h"
#include <cstdint>
#include <stdlib.h>

namespace nextcode {

__attribute__((visibility("hidden")))
NEXTCODE_NORETURN void nextcode_Concurrency_fatalError(uint32_t flags, const char *format, ...);

} // namespace nextcode

#endif // NEXTCODE_CONCURRENCY_ERROR_BACKDEPLOY56_H
