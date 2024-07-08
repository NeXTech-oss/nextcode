//===--- stdlib_shims.h - NeXTCode Concurrency Support -----------------------===//
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
//  Forward declarations of <stdlib.h> interfaces so that NeXTCode Concurrency
//  doesn't depend on the C library.
//
//===----------------------------------------------------------------------===//
#ifndef STDLIB_SHIMS_H
#define STDLIB_SHIMS_H

#ifdef __cplusplus
extern "C" [[noreturn]]
#endif
void exit(int);

#define EXIT_SUCCESS 0

#endif // STDLIB_SHIMS_H
