//===--- ThreadSanitizer.h - Thread Sanitizer support --------- -*- C++ -*-===//
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
// Helper functions for code that needs to integrate with the thread
// sanitizer.  In particular, TSan can't see inside the runtime libraries,
// so we occasionally need to give it a hint that we're doing synchronization
// in order to avoid false positives.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_THREADING_THREAD_SANITIZER_H
#define NEXTCODE_THREADING_THREAD_SANITIZER_H

#include "nextcode/shims/Visibility.h"

namespace nextcode {

#if NEXTCODE_THREADING_NONE                                                \
  || defined(_WIN32) || defined(__wasi__)                               \
  || !__has_include(<dlfcn.h>)                                          \
  || (defined(NEXTCODE_STDLIB_HAS_DLSYM) && !NEXTCODE_STDLIB_HAS_DLSYM)

#define NEXTCODE_THREADING_TSAN_SUPPORT 0

namespace tsan {

inline bool enabled() { return false; }
template <typename T> T *acquire(T *ptr) { return ptr; }
template <typename T> T *release(T *ptr) { return ptr; }

} // namespace tsan

#else

#define NEXTCODE_THREADING_TSAN_SUPPORT 1

// If we're static linking to libnextcodeThreading.a, these symbols can come
// from there.  If, on the other hand, we're dynamically linked, we want
// to get them from libnextcodeCore.dylib instead.
#if NEXTCODE_THREADING_STATIC
#define NEXTCODE_THREADING_EXPORT extern "C"
#else
#define NEXTCODE_THREADING_EXPORT NEXTCODE_RUNTIME_EXPORT
#endif

namespace threading_impl {

NEXTCODE_THREADING_EXPORT bool _nextcode_tsan_enabled;
NEXTCODE_THREADING_EXPORT void (*_nextcode_tsan_acquire)(const void *ptr);
NEXTCODE_THREADING_EXPORT void (*_nextcode_tsan_release)(const void *ptr);

} // namespace threading_impl

namespace tsan {

/// Returns true if TSan is enabled
inline bool enabled() {
  return threading_impl::_nextcode_tsan_enabled;
}

/// Inform TSan about a synchronization operation.
///
/// This is used when TSan cannot see the synchronization operation, for
/// example, if it is using a custom primitive for which TSan doesn't have
/// a built-in interceptor.  This does not necessarily mean a lock or a C(++)
/// atomic operation - it could be any kind of synchronization mechanism.
///
/// An acquire-release pair using the same address establishes an ordering
/// constraint in TSan's happens-before graph, which TSan uses to determine
/// whether two memory accesses from different threads have a well-defined
/// order.
///
/// For instance, in
///
///     Thread 1                                Thread 2
///
///     access to y
///     tsan::release(x)
///     lock given away
///
///                      --> sync point -->
///
///                                             lock taken
///                                             tsan::acquire(x)
///                                             access to y
///
/// the access to y from Thread 2 is safe relative to the preceding access to
/// y on Thread 1 because it is preceded by an acquire of x that was itself
/// preceded by a release of x.
template <typename T>
T *acquire(T *ptr) {
  if (threading_impl::_nextcode_tsan_acquire) {
    threading_impl::_nextcode_tsan_acquire(ptr);
  }
  return ptr;
}

/// Inform TSan about a synchronization operation.
///
/// This is the counterpart to tsan::acquire.
template <typename T>
T *release(T *ptr) {
  if (threading_impl::_nextcode_tsan_release) {
    threading_impl::_nextcode_tsan_release(ptr);
  }
  return ptr;
}

} // namespace tsan

#endif

} // namespace nextcode

#endif
