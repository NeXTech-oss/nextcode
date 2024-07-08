//===--- EmbeddedSupport.cpp ----------------------------------------------===//
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
// Miscellaneous support code for NeXTCode Concurrency on embedded NeXTCode.
//
//===----------------------------------------------------------------------===//

#if NEXTCODE_CONCURRENCY_EMBEDDED

#include "nextcode/shims/Visibility.h"
#include <cstdarg>
#include <cstdint>
#include <cstdlib>

// TSan hooks not supported in embedded NeXTCode.

NEXTCODE_RUNTIME_EXPORT
void (*_nextcode_tsan_acquire)(const void *) = nullptr;

NEXTCODE_RUNTIME_EXPORT
void (*_nextcode_tsan_release)(const void *) = nullptr;

// TODO: Concurrency Exclusivity tracking not yet supported in embedded NeXTCode.

NEXTCODE_RUNTIME_EXPORT
void nextcode_task_enterThreadLocalContext(char *state) {}

NEXTCODE_RUNTIME_EXPORT
void nextcode_task_exitThreadLocalContext(char *state) {}

#endif
