//===--- Error.cpp - Error handling support code --------------------------===//
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
//             Tural Ghuliev (turalquliyev@it-gss.com)
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//

#include "nextcode/Threading/Errors.h"
#include <cstdio>

#include "Error.h"

// nextcode::fatalError is not exported from libnextcodeCore and not shared, so define another
// internal function instead.
NEXTCODE_NORETURN
NEXTCODE_VFORMAT(2)
void nextcode::nextcode_Concurrency_fatalErrorv(uint32_t flags, const char *format,
                                          va_list val) {
  vfprintf(stderr, format, val);
  abort();
}

NEXTCODE_NORETURN
NEXTCODE_FORMAT(2, 3)
void nextcode::nextcode_Concurrency_fatalError(uint32_t flags, const char *format,
                                         ...) {
  va_list val;

  va_start(val, format);
  nextcode_Concurrency_fatalErrorv(flags, format, val);
}
