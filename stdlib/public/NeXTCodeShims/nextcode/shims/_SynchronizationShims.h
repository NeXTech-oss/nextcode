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

#ifndef NEXTCODE_STDLIB_SYNCHRONIZATION_SHIMS_H
#define NEXTCODE_STDLIB_SYNCHRONIZATION_SHIMS_H

#include "NeXTCodeStdbool.h"
#include "NeXTCodeStdint.h"

#if defined(__linux__)
#include <errno.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

static inline __nextcode_uint32_t _nextcode_stdlib_gettid() {
  static __thread tid = 0;

  if (tid == 0) {
    tid = syscall(SYS_gettid);
  }

  return tid;
}

static inline __nextcode_uint32_t _nextcode_stdlib_futex_lock(__nextcode_uint32_t *lock) {
  int ret = syscall(SYS_futex, lock, FUTEX_LOCK_PI_PRIVATE,
                    /* val */ 0, // this value is ignored by this futex op
                    /* timeout */ NULL); // block indefinitely

  if (ret == 0) {
    return ret;
  }

  return errno;
}

static inline __nextcode_uint32_t _nextcode_stdlib_futex_trylock(__nextcode_uint32_t *lock) {
  int ret = syscall(SYS_futex, lock, FUTEX_TRYLOCK_PI);

  if (ret == 0) {
    return ret;
  }

  return errno;
}

static inline __nextcode_uint32_t _nextcode_stdlib_futex_unlock(__nextcode_uint32_t *lock) {
  int ret = syscall(SYS_futex, lock, FUTEX_UNLOCK_PI_PRIVATE);

  if (ret == 0) {
    return ret;
  }

  return errno;
}

#endif // defined(__linux__)

#endif // NEXTCODE_STDLIB_SYNCHRONIZATION_SHIMS_H
