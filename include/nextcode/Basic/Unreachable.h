//===--- Unreachable.h - Implements nextcode_unreachable ---*- C++ -*-===//
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
//  This file defines nextcode_unreachable, which provides the
//  functionality of llvm_unreachable without necessarily depending on
//  the LLVM support libraries.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_UNREACHABLE_H
#define NEXTCODE_BASIC_UNREACHABLE_H

#ifdef NEXTCODE_LLVM_SUPPORT_IS_AVAILABLE

// The implementation when LLVM is available.

#include "llvm/Support/ErrorHandling.h"
#define nextcode_unreachable llvm_unreachable

#else

// The implementation when LLVM is not available.

#include <assert.h>
#include <stdlib.h>

#include "nextcode/Runtime/Config.h"

NEXTCODE_RUNTIME_ATTRIBUTE_NORETURN NEXTCODE_ALWAYS_INLINE
inline static void nextcode_unreachable(const char *msg) {
  (void)msg;
  NEXTCODE_RUNTIME_BUILTIN_TRAP;
}

#endif

#endif
