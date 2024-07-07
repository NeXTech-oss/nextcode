//==--- Pthreads.h - Threading abstraction implementation ------ -*-C++ -*-===//
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
// Implements threading support for plain pthreads
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_THREADING_IMPL_PTHREADS_H
#define NEXTCODE_THREADING_IMPL_PTHREADS_H

#include <errno.h>
#include <pthread.h>

#include <atomic>
#include <cstdint>

#include "chrono_utils.h"

#include <optional>

#include "nextcode/Threading/Errors.h"

namespace nextcode {
namespace threading_impl {

#define NEXTCODE_PTHREADS_CHECK(expr)                                             \
  do {                                                                         \
    int res_ = (expr);                                                         \
    if (res_ != 0)                                                             \
      nextcode::threading::fatal(#expr " failed with error %d\n", res_);          \
  } while (0)

#define NEXTCODE_PTHREADS_RETURN_TRUE_OR_FALSE(falseerr, expr)                    \
  do {                                                                         \
    int res_ = (expr);                                                         \
    switch (res_) {                                                            \
    case 0:                                                                    \
      return true;                                                             \
    case falseerr:                                                             \
      return false;                                                            \
    default:                                                                   \
      nextcode::threading::fatal(#expr " failed with error (%d)\n", res_);        \
    }                                                                          \
  } while (0)

// .. Thread related things ..................................................

using thread_id = ::pthread_t;

inline thread_id thread_get_current() { return ::pthread_self(); }

bool thread_is_main();

inline bool threads_same(thread_id a, thread_id b) {
  return ::pthread_equal(a, b);
}

std::optional<stack_bounds> thread_get_current_stack_bounds();

// .. Mutex support ..........................................................

using mutex_handle = ::pthread_mutex_t;

inline void mutex_init(mutex_handle &handle, bool checked = false) {
  if (!checked) {
    handle = PTHREAD_MUTEX_INITIALIZER;
  } else {
    ::pthread_mutexattr_t attr;
    NEXTCODE_PTHREADS_CHECK(::pthread_mutexattr_init(&attr));
    NEXTCODE_PTHREADS_CHECK(
        ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK));
    NEXTCODE_PTHREADS_CHECK(::pthread_mutex_init(&handle, &attr));
    NEXTCODE_PTHREADS_CHECK(::pthread_mutexattr_destroy(&attr));
  }
}
inline void mutex_destroy(mutex_handle &handle) {
  NEXTCODE_PTHREADS_CHECK(::pthread_mutex_destroy(&handle));
}

inline void mutex_lock(mutex_handle &handle) {
  NEXTCODE_PTHREADS_CHECK(::pthread_mutex_lock(&handle));
}
inline void mutex_unlock(mutex_handle &handle) {
  NEXTCODE_PTHREADS_CHECK(::pthread_mutex_unlock(&handle));
}
inline bool mutex_try_lock(mutex_handle &handle) {
  NEXTCODE_PTHREADS_RETURN_TRUE_OR_FALSE(EBUSY, ::pthread_mutex_trylock(&handle));
}

inline void mutex_unsafe_lock(mutex_handle &handle) {
  (void)::pthread_mutex_lock(&handle);
}
inline void mutex_unsafe_unlock(mutex_handle &handle) {
  (void)::pthread_mutex_unlock(&handle);
}

using lazy_mutex_handle = ::pthread_mutex_t;

// We don't actually need to be lazy here because pthreads has
// PTHREAD_MUTEX_INITIALIZER.
#define NEXTCODE_LAZY_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER

inline void lazy_mutex_destroy(lazy_mutex_handle &handle) {
  NEXTCODE_PTHREADS_CHECK(::pthread_mutex_destroy(&handle));
}

inline void lazy_mutex_lock(lazy_mutex_handle &handle) {
  NEXTCODE_PTHREADS_CHECK(::pthread_mutex_lock(&handle));
}
inline void lazy_mutex_unlock(lazy_mutex_handle &handle) {
  NEXTCODE_PTHREADS_CHECK(::pthread_mutex_unlock(&handle));
}
inline bool lazy_mutex_try_lock(lazy_mutex_handle &handle) {
  NEXTCODE_PTHREADS_RETURN_TRUE_OR_FALSE(EBUSY, ::pthread_mutex_trylock(&handle));
}

inline void lazy_mutex_unsafe_lock(lazy_mutex_handle &handle) {
  (void)::pthread_mutex_lock(&handle);
}
inline void lazy_mutex_unsafe_unlock(lazy_mutex_handle &handle) {
  (void)::pthread_mutex_unlock(&handle);
}

// .. ConditionVariable support ..............................................

struct cond_handle {
  ::pthread_cond_t  condition;
  ::pthread_mutex_t mutex;
};

inline void cond_init(cond_handle &handle) {
  handle.condition = PTHREAD_COND_INITIALIZER;
  handle.mutex = PTHREAD_MUTEX_INITIALIZER;
}
inline void cond_destroy(cond_handle &handle) {
  NEXTCODE_PTHREADS_CHECK(::pthread_cond_destroy(&handle.condition));
  NEXTCODE_PTHREADS_CHECK(::pthread_mutex_destroy(&handle.mutex));
}
inline void cond_lock(cond_handle &handle) {
  NEXTCODE_PTHREADS_CHECK(::pthread_mutex_lock(&handle.mutex));
}
inline void cond_unlock(cond_handle &handle) {
  NEXTCODE_PTHREADS_CHECK(::pthread_mutex_unlock(&handle.mutex));
}
inline void cond_signal(cond_handle &handle) {
  NEXTCODE_PTHREADS_CHECK(::pthread_cond_signal(&handle.condition));
}
inline void cond_broadcast(cond_handle &handle) {
  NEXTCODE_PTHREADS_CHECK(::pthread_cond_broadcast(&handle.condition));
}
inline void cond_wait(cond_handle &handle) {
  NEXTCODE_PTHREADS_CHECK(::pthread_cond_wait(&handle.condition, &handle.mutex));
}
template <class Rep, class Period>
inline bool cond_wait(cond_handle &handle,
                      std::chrono::duration<Rep, Period> duration) {
  auto to_wait = chrono_utils::ceil<
    std::chrono::system_clock::duration>(duration);
  auto deadline = std::chrono::system_clock::now() + to_wait;
  return cond_wait(handle, deadline);
}
inline bool cond_wait(cond_handle &handle,
                      std::chrono::system_clock::time_point deadline) {
  auto ns = chrono_utils::ceil<std::chrono::nanoseconds>(
    deadline.time_since_epoch()).count();
  struct ::timespec ts = { ::time_t(ns / 1000000000), long(ns % 1000000000) };
  NEXTCODE_PTHREADS_RETURN_TRUE_OR_FALSE(
    ETIMEDOUT,
    ::pthread_cond_timedwait(&handle.condition, &handle.mutex, &ts)
  );
}

// .. Once ...................................................................

using once_t = std::atomic<std::intptr_t>;

void once_slow(once_t &predicate, void (*fn)(void *), void *context);

inline void once_impl(once_t &predicate, void (*fn)(void *), void *context) {
  // Sadly we can't use ::pthread_once() for this (no context)
  if (predicate.load(std::memory_order_acquire) < 0)
    return;

  once_slow(predicate, fn, context);
}

// .. Thread local storage ...................................................

#if __cplusplus >= 201103L || __has_feature(cxx_thread_local)
#define NEXTCODE_THREAD_LOCAL thread_local
#endif

using tls_key_t = pthread_key_t;
using tls_dtor_t = void (*)(void *);

inline bool tls_alloc(tls_key_t &key, tls_dtor_t dtor) {
  return pthread_key_create(&key, dtor) == 0;
}

inline void *tls_get(tls_key_t key) { return pthread_getspecific(key); }

inline void tls_set(tls_key_t key, void *value) {
  pthread_setspecific(key, value);
}

} // namespace threading_impl

} // namespace nextcode

#endif // NEXTCODE_THREADING_IMPL_PTHREADS_H
