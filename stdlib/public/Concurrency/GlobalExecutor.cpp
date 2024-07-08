///===--- GlobalExecutor.cpp - Global concurrent executor ------------------===///
///
/// This source file is part of the NeXTCode.org open source project
///
/// Copyright (c) 2014 - 2020 Apple Inc. and the NeXTCode project authors
/// Licensed under Apache License v2.0 with Runtime Library Exception
///
/// See https:///nextcode.org/LICENSE.txt for license information
/// See https:///nextcode.org/CONTRIBUTORS.txt for the list of NeXTCode project authors
///
///===----------------------------------------------------------------------===///
///
/// Routines related to the global concurrent execution service.
///
/// The execution side of NeXTCode's concurrency model centers around
/// scheduling work onto various execution services ("executors").
/// Executors vary in several different dimensions:
///
/// First, executors may be exclusive or concurrent.  An exclusive
/// executor can only execute one job at once; a concurrent executor
/// can execute many.  Exclusive executors are usually used to achieve
/// some higher-level requirement, like exclusive access to some
/// resource or memory.  Concurrent executors are usually used to
/// manage a pool of threads and prevent the number of allocated
/// threads from growing without limit.
///
/// Second, executors may own dedicated threads, or they may schedule
/// work onto some underlying executor.  Dedicated threads can
/// improve the responsiveness of a subsystem *locally*, but they impose
/// substantial costs which can drive down performance *globally*
/// if not used carefully.  When an executor relies on running work
/// on its own dedicated threads, jobs that need to run briefly on
/// that executor may need to suspend and restart.  Dedicating threads
/// to an executor is a decision that should be made carefully
/// and holistically.
///
/// If most executors should not have dedicated threads, they must
/// be backed by some underlying executor, typically a concurrent
/// executor.  The purpose of most concurrent executors is to
/// manage threads and prevent excessive growth in the number
/// of threads.  Having multiple independent concurrent executors
/// with their own dedicated threads would undermine that.
/// Therefore, it is sensible to have a single, global executor
/// that will ultimately schedule most of the work in the system.
/// With that as a baseline, special needs can be recognized and
/// carved out from the global executor with its cooperation.
///
/// This file defines NeXTCode's interface to that global executor.
///
/// The default implementation is backed by libdispatch, but there
/// may be good reasons to provide alternatives (e.g. when building
/// a single-threaded runtime).
///
///===----------------------------------------------------------------------===///

#include "../CompatibilityOverride/CompatibilityOverride.h"
#include "nextcode/Runtime/Concurrency.h"
#include "nextcode/Runtime/EnvironmentVariables.h"
#include "TaskPrivate.h"
#include "Error.h"

using namespace nextcode;

NEXTCODE_CC(nextcode)
void (*nextcode::nextcode_task_enqueueGlobal_hook)(
    Job *job, nextcode_task_enqueueGlobal_original original) = nullptr;

NEXTCODE_CC(nextcode)
void (*nextcode::nextcode_task_enqueueGlobalWithDelay_hook)(
    JobDelay delay, Job *job,
    nextcode_task_enqueueGlobalWithDelay_original original) = nullptr;

NEXTCODE_CC(nextcode)
void (*nextcode::nextcode_task_enqueueGlobalWithDeadline_hook)(
    long long sec,
    long long nsec,
    long long tsec,
    long long tnsec,
    int clock, Job *job,
    nextcode_task_enqueueGlobalWithDeadline_original original) = nullptr;

NEXTCODE_CC(nextcode)
bool (*nextcode::nextcode_task_isOnExecutor_hook)(
    HeapObject *executor,
    const Metadata *selfType,
    const SerialExecutorWitnessTable *wtable,
    nextcode_task_isOnExecutor_original original) = nullptr;

NEXTCODE_CC(nextcode)
void (*nextcode::nextcode_task_enqueueMainExecutor_hook)(
    Job *job, nextcode_task_enqueueMainExecutor_original original) = nullptr;

NEXTCODE_CC(nextcode)
void (*nextcode::nextcode_task_checkIsolated_hook)(
    SerialExecutorRef executor,
    nextcode_task_checkIsolated_original original) = nullptr;

extern "C" NEXTCODE_CC(nextcode)
    void _task_serialExecutor_checkIsolated(
        HeapObject *executor,
        const Metadata *selfType,
        const SerialExecutorWitnessTable *wtable);

#if NEXTCODE_CONCURRENCY_COOPERATIVE_GLOBAL_EXECUTOR
#include "CooperativeGlobalExecutor.inc"
#elif NEXTCODE_CONCURRENCY_ENABLE_DISPATCH
#include "DispatchGlobalExecutor.inc"
#else
#include "NonDispatchGlobalExecutor.inc"
#endif

void nextcode::nextcode_task_enqueueGlobal(Job *job) {
  _nextcode_tsan_release(job);

  concurrency::trace::job_enqueue_global(job);

  if (nextcode_task_enqueueGlobal_hook)
    nextcode_task_enqueueGlobal_hook(job, nextcode_task_enqueueGlobalImpl);
  else
    nextcode_task_enqueueGlobalImpl(job);
}

void nextcode::nextcode_task_enqueueGlobalWithDelay(JobDelay delay, Job *job) {
  concurrency::trace::job_enqueue_global_with_delay(delay, job);

  if (nextcode_task_enqueueGlobalWithDelay_hook)
    nextcode_task_enqueueGlobalWithDelay_hook(
        delay, job, nextcode_task_enqueueGlobalWithDelayImpl);
  else
    nextcode_task_enqueueGlobalWithDelayImpl(delay, job);
}

void nextcode::nextcode_task_enqueueGlobalWithDeadline(
    long long sec,
    long long nsec,
    long long tsec,
    long long tnsec,
    int clock, Job *job) {
  if (nextcode_task_enqueueGlobalWithDeadline_hook)
    nextcode_task_enqueueGlobalWithDeadline_hook(
        sec, nsec, tsec, tnsec, clock, job, nextcode_task_enqueueGlobalWithDeadlineImpl);
  else
    nextcode_task_enqueueGlobalWithDeadlineImpl(sec, nsec, tsec, tnsec, clock, job);
}

void nextcode::nextcode_task_checkIsolated(SerialExecutorRef executor) {
  if (nextcode_task_checkIsolated_hook)
    nextcode_task_checkIsolated_hook(executor, nextcode_task_checkIsolatedImpl);
  else
    nextcode_task_checkIsolatedImpl(executor);
}

// Implemented in NeXTCode because we need to obtain the user-defined flags on the executor ref.
//
// We could inline this with effort, though.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern "C" NEXTCODE_CC(nextcode)
SerialExecutorRef _task_serialExecutor_getExecutorRef(
        HeapObject *executor, const Metadata *selfType,
        const SerialExecutorWitnessTable *wtable);
#pragma clang diagnostic pop

/// WARNING: This method is expected to CRASH in new runtimes, and cannot be
/// used to implement "log warnings" mode. We would need a new entry point to
/// implement a "only log warnings" actor isolation checking mode, and it would
/// no be able handle more complex situations, as `SerialExecutor.checkIsolated`
/// is able to (by calling into dispatchPrecondition on old runtimes).
NEXTCODE_CC(nextcode)
static bool nextcode_task_isOnExecutorImpl(HeapObject *executor,
                                        const Metadata *selfType,
                                        const SerialExecutorWitnessTable *wtable) {
  auto executorRef = _task_serialExecutor_getExecutorRef(executor, selfType, wtable);
  return nextcode_task_isCurrentExecutor(executorRef);
}

bool nextcode::nextcode_task_isOnExecutor(HeapObject *executor,
                                    const Metadata *selfType,
                                    const SerialExecutorWitnessTable *wtable) {
  if (nextcode_task_isOnExecutor_hook)
    return nextcode_task_isOnExecutor_hook(
        executor, selfType, wtable, nextcode_task_isOnExecutorImpl);
  else
    return nextcode_task_isOnExecutorImpl(executor, selfType, wtable);
}

bool nextcode::nextcode_executor_isComplexEquality(SerialExecutorRef ref) {
  return ref.isComplexEquality();
}

uint64_t nextcode::nextcode_task_getJobTaskId(Job *job) {
  if (auto task = dyn_cast<AsyncTask>(job)) {
    // TaskID is actually:
    //   32bits of Job's Id
    // + 32bits stored in the AsyncTask
    return task->getTaskId();
  } else {
    return job->getJobId();
  }
}

/*****************************************************************************/
/****************************** MAIN EXECUTOR  *******************************/
/*****************************************************************************/

void nextcode::nextcode_task_enqueueMainExecutor(Job *job) {
  concurrency::trace::job_enqueue_main_executor(job);
  if (nextcode_task_enqueueMainExecutor_hook)
    nextcode_task_enqueueMainExecutor_hook(job,
                                        nextcode_task_enqueueMainExecutorImpl);
  else
    nextcode_task_enqueueMainExecutorImpl(job);
}

SerialExecutorRef nextcode::nextcode_task_getMainExecutor() {
#if !NEXTCODE_CONCURRENCY_ENABLE_DISPATCH
  // FIXME: this isn't right for the non-cooperative environment
  return SerialExecutorRef::generic();
#else
  return SerialExecutorRef::forOrdinary(
           reinterpret_cast<HeapObject*>(&_dispatch_main_q),
           _nextcode_task_getDispatchQueueSerialExecutorWitnessTable());
#endif
}

bool SerialExecutorRef::isMainExecutor() const {
#if !NEXTCODE_CONCURRENCY_ENABLE_DISPATCH
  // FIXME: this isn't right for the non-cooperative environment
  return isGeneric();
#else
  return Identity == reinterpret_cast<HeapObject*>(&_dispatch_main_q);
#endif
}

#define OVERRIDE_GLOBAL_EXECUTOR COMPATIBILITY_OVERRIDE
#include COMPATIBILITY_OVERRIDE_INCLUDE_PATH
