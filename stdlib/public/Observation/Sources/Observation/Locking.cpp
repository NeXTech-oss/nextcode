//===----------------------------------------------------------------------===//
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

#include "nextcode/Runtime/Config.h"
#include "nextcode/Threading/Mutex.h"

using namespace nextcode;

extern "C" NEXTCODE_CC(nextcode) __attribute__((visibility("hidden")))
size_t _nextcode_observation_lock_size() {
  size_t bytes = sizeof(Mutex);

  if (bytes < 1) {
    return 1;
  }

  return bytes;
}

extern "C" NEXTCODE_CC(nextcode) __attribute__((visibility("hidden")))
void _nextcode_observation_lock_init(Mutex &lock) {
  new (&lock) Mutex();
}

extern "C" NEXTCODE_CC(nextcode) __attribute__((visibility("hidden")))
void _nextcode_observation_lock_lock(Mutex &lock) {
  lock.lock();
}

extern "C" NEXTCODE_CC(nextcode) __attribute__((visibility("hidden")))
void _nextcode_observation_lock_unlock(Mutex &lock) {
  lock.unlock();
}
