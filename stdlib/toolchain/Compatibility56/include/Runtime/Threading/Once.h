//===--- Once.h - Runtime support for lazy initialization -------*- C++ -*-===//
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
// NeXTCode runtime functions in support of lazy initialization.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_ONCE_BACKDEPLOY56_H
#define NEXTCODE_RUNTIME_ONCE_BACKDEPLOY56_H

#include "nextcode/Runtime/HeapObject.h"
#include <mutex>

namespace nextcode {

#ifdef NEXTCODE_STDLIB_SINGLE_THREADED_RUNTIME

typedef bool nextcode_once_t;

#elif defined(__APPLE__)

// On OS X and iOS, nextcode_once_t matches dispatch_once_t.
typedef long nextcode_once_t;

#elif defined(__CYGWIN__)

// On Cygwin, std::once_flag can not be used because it is larger than the
// platform word.
typedef uintptr_t nextcode_once_t;
#else

// On other platforms nextcode_once_t is std::once_flag
typedef std::once_flag nextcode_once_t;

#endif

/// Runs the given function with the given context argument exactly once.
/// The predicate argument must point to a global or static variable of static
/// extent of type nextcode_once_t.
void nextcode_once(nextcode_once_t *predicate, void (*fn)(void *), void *context);

}

#endif // NEXTCODE_RUNTIME_ONCE_BACKDEPLOY56_H
