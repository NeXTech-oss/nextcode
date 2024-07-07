//===--- Impl.h - Threading abstraction implementation -------- -*- C++ -*-===//
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
// Includes the relevant implementation file based on the selected threading
// package.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_THREADING_IMPL_H
#define NEXTCODE_THREADING_IMPL_H

#include "TLSKeys.h"

namespace nextcode {
namespace threading_impl {

struct stack_bounds {
  void *low;
  void *high;
};

} // namespace nextcode
} // namespace threading_impl


// Try to autodetect if we aren't told what to do
#if !NEXTCODE_THREADING_NONE && !NEXTCODE_THREADING_DARWIN &&                        \
    !NEXTCODE_THREADING_LINUX && !NEXTCODE_THREADING_PTHREADS &&                     \
    !NEXTCODE_THREADING_C11 && !NEXTCODE_THREADING_WIN32
#ifdef __APPLE__
#define NEXTCODE_THREADING_DARWIN 1
#elif defined(__linux__)
#define NEXTCODE_THREADING_LINUX 1
#elif defined(_WIN32)
#define NEXTCODE_THREADING_WIN32 1
#elif defined(__wasi__)
#define NEXTCODE_THREADING_NONE 1
#elif __has_include(<threads.h>)
#define NEXTCODE_THREADING_C11 1
#elif __has_include(<pthread.h>)
#define NEXTCODE_THREADING_PTHREADS 1
#else
#error Unable to autodetect threading package.  Please define NEXTCODE_THREADING_x as appropriate for your platform.
#endif
#endif

#if NEXTCODE_THREADING_NONE
#include "Impl/Nothreads.h"
#elif NEXTCODE_THREADING_DARWIN
#include "Impl/Darwin.h"
#elif NEXTCODE_THREADING_LINUX
#include "Impl/Linux.h"
#elif NEXTCODE_THREADING_PTHREADS
#include "Impl/Pthreads.h"
#elif NEXTCODE_THREADING_C11
#include "Impl/C11.h"
#elif NEXTCODE_THREADING_WIN32
#include "Impl/Win32.h"
#else
#error You need to implement Threading/Impl.h for your threading package.
#endif

#endif // NEXTCODE_THREADING_IMPL_H
