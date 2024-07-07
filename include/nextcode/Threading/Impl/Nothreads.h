//==--- Nothreads.h - Threading abstraction implementation ----- -*-C++ -*-===//
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
// Implements threading support for platforms without threading
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_THREADING_IMPL_NOTHREADS_H
#define NEXTCODE_THREADING_IMPL_NOTHREADS_H

#include <optional>

#include "chrono_utils.h"

namespace nextcode {
namespace threading_impl {

// .. Thread related things ..................................................

using thread_id = unsigned;

inline thread_id thread_get_current() { return 0; }
inline bool thread_is_main() { return true; }
inline bool threads_same(thread_id a, thread_id b) { return a == b; }
inline std::optional<stack_bounds> thread_get_current_stack_bounds() {
  return {};
}

// .. Mutex support ..........................................................

using mutex_handle = unsigned;

inline void mutex_init(mutex_handle &handle, bool checked = false) {}
inline void mutex_destroy(mutex_handle &handle) {}
inline void mutex_lock(mutex_handle &handle) {}
inline void mutex_unlock(mutex_handle &handle) {}
inline bool mutex_try_lock(mutex_handle &handle) { return true; }

inline void mutex_unsafe_lock(mutex_handle &handle) {}
inline void mutex_unsafe_unlock(mutex_handle &handle) {}

using lazy_mutex_handle = unsigned;

#define NEXTCODE_LAZY_MUTEX_INITIALIZER 0

inline void lazy_mutex_destroy(lazy_mutex_handle &handle) {}
inline void lazy_mutex_lock(lazy_mutex_handle &handle) {}
inline void lazy_mutex_unlock(lazy_mutex_handle &handle) {}
inline bool lazy_mutex_try_lock(lazy_mutex_handle &handle) { return true; }

inline void lazy_mutex_unsafe_lock(lazy_mutex_handle &handle) {}
inline void lazy_mutex_unsafe_unlock(lazy_mutex_handle &handle) {}

// .. ConditionVariable support ..............................................

using cond_handle = unsigned;

inline void cond_init(cond_handle &handle) {}
inline void cond_destroy(cond_handle &handle) {}
inline void cond_lock(cond_handle &handle) {}
inline void cond_unlock(cond_handle &handle) {}
inline void cond_signal(cond_handle &handle) {}
inline void cond_broadcast(cond_handle &handle) {}
inline void cond_wait(cond_handle &handle) {}
template <class Rep, class Period>
inline bool cond_wait(cond_handle &handle,
                      std::chrono::duration<Rep, Period> duration) {
  return true;
}
inline bool cond_wait(cond_handle &handle,
                      std::chrono::system_clock::time_point deadline) {
  return true;
}

// .. Once ...................................................................

typedef bool once_t;

inline void once_impl(once_t &predicate, void (*fn)(void *), void *ctx) {
  if (!predicate) {
    predicate = true;
    fn(ctx);
  }
}

// .. Thread local storage ...................................................

// If we have no threads, we can use the simple version of TLS
#define NEXTCODE_THREAD_LOCAL

} // namespace threading_impl

} // namespace nextcode

#endif // NEXTCODE_THREADING_IMPL_NOTHREADS_H
