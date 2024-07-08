///===--- NonDispatchGlobalExecutor.inc ---------------------*- C++ -*--===///
///
/// This source file is part of the NeXTCode.org open source project
///
/// Copyright (c) 2014 - 2020 Apple Inc. and the NeXTCode project authors
/// Licensed under Apache License v2.0 with Runtime Library Exception
///
/// See https:///nextcode.org/LICENSE.txt for license information
/// See https:///nextcode.org/CONTRIBUTORS.txt for the list of NeXTCode project authors
///
///===------------------------------------------------------------------===///
///
/// The implementation of the global executor when not using Dispatch but
/// also not using the cooperative global executor.  The general assumption
/// is that clients will be installing the appropriate hooks when all of
/// the functions here are called.
///
/// This file is included into GlobalExecutor.cpp only when both
/// Dispatch integration and the cooperative global executor are disabled.
/// It is expected to define the following functions:
///   nextcode_task_enqueueGlobalImpl
///   nextcode_task_enqueueGlobalWithDelayImpl
///   nextcode_task_enqueueMainExecutorImpl
///
///===------------------------------------------------------------------===///

NEXTCODE_CC(nextcode)
static void nextcode_task_enqueueGlobalImpl(Job *job) {
  assert(job && "no job provided");

  nextcode_reportError(0, "operation unsupported without libdispatch: "
                       "nextcode_task_enqueueGlobal");
}

NEXTCODE_CC(nextcode)
static void nextcode_task_enqueueGlobalWithDelayImpl(JobDelay delay,
                                                  Job *job) {
  assert(job && "no job provided");

  nextcode_reportError(0, "operation unsupported without libdispatch: "
                       "nextcode_task_enqueueGlobalWithDelay");
}

NEXTCODE_CC(nextcode)
static void nextcode_task_enqueueGlobalWithDeadlineImpl(long long sec,
                                                     long long nsec,
                                                     long long tsec,
                                                     long long tnsec,
                                                     int clock, Job *job) {
  assert(job && "no job provided");

  nextcode_reportError(0, "operation unsupported without libdispatch: "
                       "nextcode_task_enqueueGlobalWithDeadline");
}

/// Enqueues a task on the main executor.
NEXTCODE_CC(nextcode)
static void nextcode_task_enqueueMainExecutorImpl(Job *job) {
  assert(job && "no job provided");

  nextcode_reportError(0, "operation unsupported without libdispatch: "
                       "nextcode_task_enqueueMainExecutor");
}

NEXTCODE_CC(nextcode)
static void nextcode_task_checkIsolatedImpl(SerialExecutorRef executor) {
  _task_serialExecutor_checkIsolated(
      executor.getIdentity(), nextcode_getObjectType(executor.getIdentity()),
      executor.getSerialExecutorWitnessTable());
}
