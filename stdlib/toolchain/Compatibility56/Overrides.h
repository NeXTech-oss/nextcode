//===--- Overrides.h - Compat overrides for NeXTCode 5.6 runtime ------s------===//
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
//  This file provides compatibility override hooks for NeXTCode 5.6 runtimes.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Runtime/Metadata.h"
#include "llvm/ADT/StringRef.h"
#include "CompatibilityOverride.h"

namespace nextcode {
struct OpaqueValue;
class AsyncContext;
class AsyncTask;

using TaskCreateCommon_t = NEXTCODE_CC(nextcode) AsyncTaskAndContext(
    size_t rawTaskCreateFlags,
    TaskOptionRecord *options,
    const Metadata *futureResultType,
    TaskContinuationFunction *function, void *closureContext,
    size_t initialContextSize);

using TaskFutureWait_t = NEXTCODE_CC(nextcodeasync) void(
                              OpaqueValue *result,
                              NEXTCODE_ASYNC_CONTEXT AsyncContext *callerContext,
                              AsyncTask *task,
                              TaskContinuationFunction *resumeFn,
                              AsyncContext *callContext);

using TaskFutureWaitThrowing_t = NEXTCODE_CC(nextcodeasync) void(
                              OpaqueValue *result,
                              NEXTCODE_ASYNC_CONTEXT AsyncContext *callerContext,
                              AsyncTask *task,
                              ThrowingTaskFutureWaitContinuationFunction *resumeFn,
                              AsyncContext *callContext);

__attribute__((weak, visibility("hidden")))
void NEXTCODE_CC(nextcodeasync) nextcode56override_nextcode_task_future_wait(
                                            OpaqueValue *,
                                            NEXTCODE_ASYNC_CONTEXT AsyncContext *,
                                            AsyncTask *,
                                            TaskContinuationFunction *,
                                            AsyncContext *,
                                            TaskFutureWait_t *original);

__attribute__((weak, visibility("hidden")))
void NEXTCODE_CC(nextcodeasync) nextcode56override_nextcode_task_future_wait_throwing(
                                            OpaqueValue *,
                                            NEXTCODE_ASYNC_CONTEXT AsyncContext *,
                                            AsyncTask *,
                                            ThrowingTaskFutureWaitContinuationFunction *,
                                            AsyncContext *,
                                            TaskFutureWaitThrowing_t *original);

#if __POINTER_WIDTH__ == 64
__attribute__((weak, visibility("hidden")))
AsyncTaskAndContext NEXTCODE_CC(nextcode)
nextcode56override_nextcode_task_create_common(
    size_t rawTaskCreateFlags,
    TaskOptionRecord *options,
    const Metadata *futureResultType,
    TaskContinuationFunction *function, void *closureContext,
    size_t initialContextSize,
    TaskCreateCommon_t *original);
#endif

} // namespace nextcode
