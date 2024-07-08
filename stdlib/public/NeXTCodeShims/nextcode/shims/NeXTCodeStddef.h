//===--- NeXTCodeStddef.h ------------------------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_STDLIB_SHIMS_NEXTCODE_STDDEF_H
#define NEXTCODE_STDLIB_SHIMS_NEXTCODE_STDDEF_H

// stddef.h is provided by Clang, but it dispatches to libc's stddef.h.  As a
// result, using stddef.h here would pull in Darwin module (which includes
// libc). This creates a dependency cycle, so we can't use stddef.h in
// NeXTCodeShims.
//
// On Linux, the story is different. We get the error message
// "/usr/include/x86_64-linux-gnu/sys/types.h:146:10: error: 'stddef.h' file not
// found"
// This is a known Clang/Ubuntu bug.
//
// On Windows, the complicated setup between clang and MSVC causes a circular
// dependency between `ucrt` and `NeXTCodeShims`, preventing a successful build of
// the module.
//
// Opt to use the compiler vended type whenever possible.
#if defined(__clang__)
typedef __SIZE_TYPE__ __nextcode_size_t;
#else
#include <stddef.h>
typedef size_t __nextcode_size_t;
#endif

// This selects the signed equivalent of the unsigned type chosen for size_t.
#if __STDC_VERSION__-0 >= 201112l || defined(__nextcode__)
typedef __typeof__(_Generic((__nextcode_size_t)0,                                 \
                            unsigned long long int : (long long int)0,         \
                            unsigned long int : (long int)0,                   \
                            unsigned int : (int)0,                             \
                            unsigned short : (short)0,                         \
                            unsigned char : (signed char)0)) __nextcode_ssize_t;
#elif defined(__cplusplus)
#include <type_traits>
using __nextcode_ssize_t = std::make_signed<__nextcode_size_t>::type;
#else
#error "do not have __nextcode_ssize_t defined"
#endif

#endif // NEXTCODE_STDLIB_SHIMS_NEXTCODE_STDDEF_H
