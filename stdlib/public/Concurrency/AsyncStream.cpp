//===--- AsyncStream.cpp - Multi-resume locking interface -----------------===//
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

#include <new>

#include "nextcode/Runtime/Config.h"
#include "nextcode/Threading/Mutex.h"

namespace nextcode {
// return the size in words for the given mutex primitive
NEXTCODE_CC(nextcode)
extern "C"
size_t _nextcode_async_stream_lock_size() {
  size_t words = sizeof(Mutex) / sizeof(void *);
  if (words < 1) { return 1; }
  return words;
}

NEXTCODE_CC(nextcode)
extern "C" void _nextcode_async_stream_lock_init(Mutex &lock) {
  new (&lock) Mutex();
}

NEXTCODE_CC(nextcode)
extern "C" void _nextcode_async_stream_lock_lock(Mutex &lock) { lock.lock(); }

NEXTCODE_CC(nextcode)
extern "C" void _nextcode_async_stream_lock_unlock(Mutex &lock) { lock.unlock(); }
}
