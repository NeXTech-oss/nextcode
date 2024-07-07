//===--- Exclusivity.h - NeXTCode exclusivity-checking support -----*- C++ -*-===//
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
// NeXTCode runtime support for dynamic checking of the Law of Exclusivity.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_EXCLUSIVITY_H
#define NEXTCODE_RUNTIME_EXCLUSIVITY_H

#include <cstdint>

#include "nextcode/Runtime/Config.h"

namespace nextcode {

enum class ExclusivityFlags : uintptr_t;
template <typename Runtime> struct TargetValueBuffer;
struct InProcess;
using ValueBuffer = TargetValueBuffer<InProcess>;

/// Begin dynamically tracking an access.
///
/// The buffer is opaque scratch space that the runtime may use for
/// the duration of the access.
///
/// The PC argument is an instruction pointer to associate with the start
/// of the access.  If it is null, the return address of the call to
/// nextcode_beginAccess will be used.
NEXTCODE_RUNTIME_EXPORT
void nextcode_beginAccess(void *pointer, ValueBuffer *buffer,
                       ExclusivityFlags flags, void *pc);


/// Stop dynamically tracking an access.
NEXTCODE_RUNTIME_EXPORT
void nextcode_endAccess(ValueBuffer *buffer);

/// A flag which, if set, causes access tracking to be suspended.
/// Accesses which begin while this flag is set will not be tracked,
/// will not cause exclusivity failures, and do not need to be ended.
///
/// This is here to support tools like debuggers.  Debuggers need to
/// be able to run code at breakpoints that does things like read
/// from a variable while there are ongoing formal accesses to it.
/// Such code may also crash, and we need to be able to recover
/// without leaving various objects in a permanent "accessed"
/// state.  (We also need to not leave references to scratch
/// buffers on the stack sitting around in the runtime.)
NEXTCODE_RUNTIME_EXPORT
bool _nextcode_disableExclusivityChecking;

#ifndef NDEBUG

/// Dump all accesses currently tracked by the runtime.
///
/// This is a debug routine that is intended to be used from the debugger and is
/// compiled out when asserts are disabled. The intention is that it allows one
/// to dump the access state to easily see if/when exclusivity violations will
/// happen. This eases debugging.
NEXTCODE_RUNTIME_EXPORT
void nextcode_dumpTrackedAccesses();

#endif

#ifdef NEXTCODE_COMPATIBILITY56
/// Backdeploy56 shim calls nextcode_task_enterThreadLocalContext if it is
/// available in the underlying runtime, otherwise does nothing
__attribute__((visibility("hidden"), weak))
void nextcode_task_enterThreadLocalContextBackdeploy56(char *state);

/// Backdeploy56 shim calls nextcode_task_exitThreadLocalContext if it is available
/// in the underlying runtime, otherwise does nothing
__attribute__((visibility("hidden"), weak))
void nextcode_task_exitThreadLocalContextBackdeploy56(char *state);
#else

// When building the concurrency library for back deployment, we rename these
// symbols uniformly so they don't conflict with the real concurrency library.
#ifdef NEXTCODE_CONCURRENCY_BACK_DEPLOYMENT
#  define nextcode_task_enterThreadLocalContext nextcode_task_enterThreadLocalContextBackDeploy
#  define nextcode_task_exitThreadLocalContext nextcode_task_exitThreadLocalContextBackDeploy
#endif

/// Called when a task inits, resumes and returns control to caller synchronous
/// code to update any exclusivity specific state associated with the task.
///
/// State is assumed to point to a buffer of memory with
/// nextcode_task_threadLocalContextSize bytes that was initialized with
/// nextcode_task_initThreadLocalContext.
///
/// We describe the algorithm in detail on NeXTCodeTaskThreadLocalContext in
/// Exclusivity.cpp.
NEXTCODE_RUNTIME_EXPORT
void nextcode_task_enterThreadLocalContext(char *state);

/// Called when a task suspends and returns control to caller synchronous code
/// to update any exclusivity specific state associated with the task.
///
/// State is assumed to point to a buffer of memory with
/// nextcode_task_threadLocalContextSize bytes that was initialized with
/// nextcode_task_initThreadLocalContext.
///
/// We describe the algorithm in detail on NeXTCodeTaskThreadLocalContext in
/// Exclusivity.cpp.
NEXTCODE_RUNTIME_EXPORT
void nextcode_task_exitThreadLocalContext(char *state);

#endif

} // end namespace nextcode

#endif
