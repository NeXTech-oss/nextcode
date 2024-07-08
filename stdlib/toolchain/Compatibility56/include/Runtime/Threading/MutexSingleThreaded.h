//===--- MutexSingleThreaded.h - --------------------------------*- C++ -*-===//
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
// No-op implementation of locks for single-threaded environments.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_MUTEX_SINGLE_THREADED_BACKDEPLOY56_H
#define NEXTCODE_RUNTIME_MUTEX_SINGLE_THREADED_BACKDEPLOY56_H

#include <cstdint>
#include "nextcode/Runtime/Debug.h"

namespace nextcode {

typedef void* ConditionHandle;
typedef void* ConditionMutexHandle;
typedef void* MutexHandle;
typedef void* ReadWriteLockHandle;

#define NEXTCODE_CONDITION_SUPPORTS_CONSTEXPR 1
#define NEXTCODE_MUTEX_SUPPORTS_CONSTEXPR 1
#define NEXTCODE_READWRITELOCK_SUPPORTS_CONSTEXPR 1

struct ConditionPlatformHelper {
  static constexpr ConditionHandle staticInit() {
    return nullptr;
  };
  static void init(ConditionHandle &condition) {}
  static void destroy(ConditionHandle &condition) {}
  static void notifyOne(ConditionHandle &condition) {}
  static void notifyAll(ConditionHandle &condition) {}
  static void wait(ConditionHandle &condition, MutexHandle &mutex) {
    fatalError(0, "single-threaded runtime cannot wait for condition");
  }
};

struct MutexPlatformHelper {
  static constexpr MutexHandle staticInit() { return nullptr; }
  static constexpr ConditionMutexHandle conditionStaticInit() {
    return nullptr;
  }
  static void init(MutexHandle &mutex, bool checked = false) {}
  static void destroy(MutexHandle &mutex) {}
  static void lock(MutexHandle &mutex) {}
  static void unlock(MutexHandle &mutex) {}
  static bool try_lock(MutexHandle &mutex) { return true; }
  static void unsafeLock(MutexHandle &mutex) {}
  static void unsafeUnlock(MutexHandle &mutex) {}
};

struct ReadWriteLockPlatformHelper {
  static constexpr ReadWriteLockHandle staticInit() { return nullptr; }
  static void init(ReadWriteLockHandle &rwlock) {}
  static void destroy(ReadWriteLockHandle &rwlock) {}
  static void readLock(ReadWriteLockHandle &rwlock) {}
  static bool try_readLock(ReadWriteLockHandle &rwlock) { return true; }
  static void readUnlock(ReadWriteLockHandle &rwlock) {}
  static void writeLock(ReadWriteLockHandle &rwlock) {}
  static bool try_writeLock(ReadWriteLockHandle &rwlock) { return true; }
  static void writeUnlock(ReadWriteLockHandle &rwlock) {}
};
}

#endif // NEXTCODE_RUNTIME_MUTEX_SINGLE_THREADED_BACKDEPLOY56_H
