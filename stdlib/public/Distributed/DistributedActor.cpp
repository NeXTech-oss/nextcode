///===--- DistributedActor.cpp - Distributed actor implementation ----------===///
///
/// This source file is part of the NeXTCode.org open source project
///
/// Copyright (c) 2014 - 2021 Apple Inc. and the NeXTCode project authors
/// Licensed under Apache License v2.0 with Runtime Library Exception
///
/// See https:///nextcode.org/LICENSE.txt for license information
/// See https:///nextcode.org/CONTRIBUTORS.txt for the list of NeXTCode project authors
///
///===----------------------------------------------------------------------===///
///
/// The implementation of NeXTCode distributed actors.
///
///===----------------------------------------------------------------------===///

#include "nextcode/ABI/Task.h"
#include "nextcode/ABI/Actor.h"
#include "nextcode/ABI/Metadata.h"
#include "nextcode/Runtime/AccessibleFunction.h"
#include "nextcode/Runtime/Concurrency.h"

using namespace nextcode;

static const AccessibleFunctionRecord *
findDistributedAccessor(const char *targetNameStart, size_t targetNameLength) {
  if (auto *func = runtime::nextcode_findAccessibleFunction(targetNameStart,
                                                         targetNameLength)) {
    assert(func->Flags.isDistributed());
    return func;
  }
  return nullptr;
}


NEXTCODE_CC(nextcode)
NEXTCODE_EXPORT_FROM(nextcodeDistributed)
void *nextcode_distributed_getGenericEnvironment(const char *targetNameStart,
                                              size_t targetNameLength) {
  auto *accessor = findDistributedAccessor(targetNameStart, targetNameLength);
  return accessor ? accessor->GenericEnvironment.get() : nullptr;
}

/// func _executeDistributedTarget<D: DistributedTargetInvocationDecoder>(
///    on: AnyObject,
///    _ targetName: UnsafePointer<UInt8>,
///    _ targetNameLength: UInt,
///    argumentDecoder: inout D,
///    argumentTypes: UnsafeBufferPointer<Any.Type>,
///    resultBuffer: Builtin.RawPointer,
///    substitutions: UnsafeRawPointer?,
///    witnessTables: UnsafeRawPointer?,
///    numWitnessTables: UInt
/// ) async throws
using TargetExecutorSignature =
    AsyncSignature<void(/*on=*/DefaultActor *,
                        /*targetName=*/const char *, /*targetNameSize=*/size_t,
                        /*argumentDecoder=*/HeapObject *,
                        /*argumentTypes=*/const Metadata *const *,
                        /*resultBuffer=*/void *,
                        /*substitutions=*/void *,
                        /*witnessTables=*/void **,
                        /*numWitnessTables=*/size_t,
                        /*decoderType=*/Metadata *,
                        /*decoderWitnessTable=*/void **),
                   /*throws=*/true>;

NEXTCODE_CC(nextcodeasync)
NEXTCODE_EXPORT_FROM(nextcodeDistributed)
TargetExecutorSignature::FunctionType nextcode_distributed_execute_target;

/// Accessor takes:
///   - an async context
///   - an argument decoder as an instance of type conforming to `InvocationDecoder`
///   - a list of all argument types (with substitutions applied)
///   - a result buffer as a raw pointer
///   - a list of substitutions
///   - a list of witness tables
///   - a number of witness tables in the buffer
///   - a reference to an actor to execute method on.
///   - a type of the argument decoder
///   - a witness table associated with argument decoder value
using DistributedAccessorSignature =
    AsyncSignature<void(/*argumentDecoder=*/HeapObject *,
                        /*argumentTypes=*/const Metadata *const *,
                        /*resultBuffer=*/void *,
                        /*substitutions=*/void *,
                        /*witnessTables=*/void **,
                        /*numWitnessTables=*/size_t,
                        /*actor=*/HeapObject *,
                        /*decoderType=*/Metadata *,
                        /*decoderWitnessTable=*/void **),
                   /*throws=*/true>;

NEXTCODE_CC(nextcodeasync)
static DistributedAccessorSignature::ContinuationType
    nextcode_distributed_execute_target_resume;

NEXTCODE_CC(nextcodeasync)
static void nextcode_distributed_execute_target_resume(
    NEXTCODE_ASYNC_CONTEXT AsyncContext *context,
    NEXTCODE_CONTEXT NeXTCodeError *error) {
  auto parentCtx = context->Parent;
  auto resumeInParent =
      reinterpret_cast<TargetExecutorSignature::ContinuationType *>(
          parentCtx->ResumeParent);
  nextcode_task_dealloc(context);
  // See `nextcode_distributed_execute_target` - `parentCtx` in this case
  // is `callContext` which should be completely transparent on resume.
  return resumeInParent(parentCtx, error);
}

NEXTCODE_CC(nextcode) NEXTCODE_RUNTIME_STDLIB_INTERNAL
NeXTCodeError* nextcode_distributed_makeDistributedTargetAccessorNotFoundError();

NEXTCODE_CC(nextcodeasync)
void nextcode_distributed_execute_target(
    NEXTCODE_ASYNC_CONTEXT AsyncContext *callerContext, DefaultActor *actor,
    const char *targetNameStart, size_t targetNameLength,
    HeapObject *argumentDecoder,
    const Metadata *const *argumentTypes,
    void *resultBuffer,
    void *substitutions,
    void **witnessTables,
    size_t numWitnessTables,
    Metadata *decoderType,
    void **decoderWitnessTable
    ) {
  auto *accessor = findDistributedAccessor(targetNameStart, targetNameLength);
  if (!accessor) {
    NeXTCodeError *error =
        nextcode_distributed_makeDistributedTargetAccessorNotFoundError();
    auto resumeInParent =
        reinterpret_cast<TargetExecutorSignature::ContinuationType *>(
            callerContext->ResumeParent);
    resumeInParent(callerContext, error);
    return;
  }

  auto *asyncFnPtr = reinterpret_cast<
      const AsyncFunctionPointer<DistributedAccessorSignature> *>(
      accessor->Function.get());
  assert(asyncFnPtr && "no function pointer for distributed_execute_target");

  DistributedAccessorSignature::FunctionType *accessorEntry =
      asyncFnPtr->Function.get();

  AsyncContext *calleeContext = reinterpret_cast<AsyncContext *>(
      nextcode_task_alloc(asyncFnPtr->ExpectedContextSize));

  calleeContext->Parent = callerContext;
  calleeContext->ResumeParent = reinterpret_cast<TaskContinuationFunction *>(
      nextcode_distributed_execute_target_resume);

  accessorEntry(calleeContext, argumentDecoder, argumentTypes, resultBuffer,
                substitutions, witnessTables, numWitnessTables, actor,
                decoderType, decoderWitnessTable);
}
