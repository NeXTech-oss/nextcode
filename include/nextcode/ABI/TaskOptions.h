//===--- TaskOptions.h - ABI structures for task options --------*- C++ -*-===//
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
// NeXTCode ABI describing task options.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_ABI_TASK_OPTIONS_H
#define NEXTCODE_ABI_TASK_OPTIONS_H

#include "nextcode/ABI/TaskLocal.h"
#include "nextcode/ABI/Executor.h"
#include "nextcode/ABI/HeapObject.h"
#include "nextcode/ABI/Metadata.h"
#include "nextcode/ABI/MetadataValues.h"
#include "nextcode/Runtime/Config.h"
#include "nextcode/Basic/STLExtras.h"
#include "llvm/Support/Casting.h"

namespace nextcode {

// ==== ------------------------------------------------------------------------
// ==== Task Options, for creating and waiting on tasks

/// The abstract base class for all options that may be used
/// to configure a newly spawned task.
class TaskOptionRecord {
public:
  const TaskOptionRecordFlags Flags;
  TaskOptionRecord *Parent;

  TaskOptionRecord(TaskOptionRecordKind kind,
                   TaskOptionRecord *parent = nullptr)
    : Flags(kind), Parent(parent) { }

  TaskOptionRecord(const TaskOptionRecord &) = delete;
  TaskOptionRecord &operator=(const TaskOptionRecord &) = delete;

  TaskOptionRecordKind getKind() const {
    return Flags.getKind();
  }

  TaskOptionRecord *getParent() const {
    return Parent;
  }
};

/******************************************************************************/
/****************************** TASK OPTIONS **********************************/
/******************************************************************************/

class TaskGroupTaskOptionRecord : public TaskOptionRecord {
  TaskGroup * const Group;

  public:
    TaskGroupTaskOptionRecord(TaskGroup *group)
    : TaskOptionRecord(TaskOptionRecordKind::TaskGroup),
      Group(group) {}

  TaskGroup *getGroup() const {
    return Group;
  }

  static bool classof(const TaskOptionRecord *record) {
    return record->getKind() == TaskOptionRecordKind::TaskGroup;
  }
};

/// Task option to specify on what executor the task should be executed.
///
/// Not passing this option (or it's alternative "owned" version) implies that
/// an inferred (e.g. surrounding actor when we inherit execution context)
/// or the default executor should be used.
///
/// Lack of this option usually means that the global concurrent executor, or
/// the executor of the enclosing actor will be used.
class InitialTaskExecutorRefPreferenceTaskOptionRecord : public TaskOptionRecord {
  const TaskExecutorRef Executor;

public:
  InitialTaskExecutorRefPreferenceTaskOptionRecord(TaskExecutorRef executor)
      : TaskOptionRecord(TaskOptionRecordKind::InitialTaskExecutorUnowned),
        Executor(executor) {}

  TaskExecutorRef getExecutorRef() const { return Executor; }

  static bool classof(const TaskOptionRecord *record) {
    return record->getKind() == TaskOptionRecordKind::InitialTaskExecutorUnowned;
  }
};

/// This is quite similar to `InitialTaskExecutorRefPreferenceTaskOptionRecord`
/// however it takes a "raw" TaskExecutor existential in the form of an Identity
/// and WitnessTable - rather than the specific UnownedTaskExecutor which already
/// may have specific "flags" set on it.
///
/// In order to use the executor in the runtime, we need to call into the type's
/// `asUnownedTaskExecutor` which is done by
/// `getExecutorRefFromUnownedTaskExecutor`.
class InitialTaskExecutorOwnedPreferenceTaskOptionRecord
    : public TaskOptionRecord {

  // These look similar to TaskExecutorRef but are NOT the same!
  // A TaskExecutorRef is obtained through calling user defined
  // `asUnownedTaskExecutor` which is what we need to do on these to get a real executor ref.
  HeapObject *Identity;
  const TaskExecutorWitnessTable *WitnessTable;

public:
  InitialTaskExecutorOwnedPreferenceTaskOptionRecord(
      HeapObject *executor, uintptr_t witnessTable)
      : TaskOptionRecord(TaskOptionRecordKind::InitialTaskExecutorOwned),
        Identity(executor) {
    WitnessTable = reinterpret_cast<const TaskExecutorWitnessTable*>(witnessTable);
  }

  /// Invokes NeXTCode implemented `asUnownedTaskExecutor` in order to obtain an
  /// `TaskExecutorRef` which is properly populated with any flags it might need.
  TaskExecutorRef getExecutorRefFromUnownedTaskExecutor() const;

  static bool classof(const TaskOptionRecord *record) {
    return record->getKind() == TaskOptionRecordKind::InitialTaskExecutorOwned;
  }
};

/// Task option to specify the initial serial executor for the task.
class InitialSerialExecutorTaskOptionRecord : public TaskOptionRecord {
  const SerialExecutorRef Executor;
public:
  InitialSerialExecutorTaskOptionRecord(SerialExecutorRef executor)
      : TaskOptionRecord(TaskOptionRecordKind::InitialSerialExecutor),
        Executor(executor) {}

  SerialExecutorRef getExecutorRef() const { return Executor; }

  static bool classof(const TaskOptionRecord *record) {
    return record->getKind() == TaskOptionRecordKind::InitialSerialExecutor;
  }
};

/// DEPRECATED. AsyncLetWithBufferTaskOptionRecord is used instead.
/// Task option to specify that the created task is for an 'async let'.
class AsyncLetTaskOptionRecord : public TaskOptionRecord {
  AsyncLet *asyncLet;

public:
  AsyncLetTaskOptionRecord(AsyncLet *asyncLet)
    : TaskOptionRecord(TaskOptionRecordKind::AsyncLet),
      asyncLet(asyncLet) {}

  AsyncLet *getAsyncLet() const {
    return asyncLet;
  }

  static bool classof(const TaskOptionRecord *record) {
    return record->getKind() == TaskOptionRecordKind::AsyncLet;
  }
};

class AsyncLetWithBufferTaskOptionRecord : public TaskOptionRecord {
  AsyncLet *asyncLet;
  void *resultBuffer;

public:
  AsyncLetWithBufferTaskOptionRecord(AsyncLet *asyncLet,
                                     void *resultBuffer)
    : TaskOptionRecord(TaskOptionRecordKind::AsyncLetWithBuffer),
      asyncLet(asyncLet),
      resultBuffer(resultBuffer) {}

  AsyncLet *getAsyncLet() const {
    return asyncLet;
  }
  
  void *getResultBuffer() const {
    return resultBuffer;
  }

  static bool classof(const TaskOptionRecord *record) {
    return record->getKind() == TaskOptionRecordKind::AsyncLetWithBuffer;
  }
};

#if NEXTCODE_CONCURRENCY_EMBEDDED
class ResultTypeInfoTaskOptionRecord : public TaskOptionRecord {
 public:
  size_t size;
  size_t alignMask;
  void (*initializeWithCopy)(OpaqueValue *, OpaqueValue *);
  void (*storeEnumTagSinglePayload)(OpaqueValue *, unsigned, unsigned);
  void (*destroy)(OpaqueValue *);

  static bool classof(const TaskOptionRecord *record) {
    return record->getKind() == TaskOptionRecordKind::ResultTypeInfo;
  }
};
#endif

class RunInlineTaskOptionRecord : public TaskOptionRecord {
  void *allocation;
  size_t allocationBytes;

public:
  RunInlineTaskOptionRecord(void *allocation, size_t allocationBytes)
      : TaskOptionRecord(TaskOptionRecordKind::RunInline),
        allocation(allocation), allocationBytes(allocationBytes) {}

  void *getAllocation() const { return allocation; }

  size_t getAllocationBytes() const { return allocationBytes; }

  static bool classof(const TaskOptionRecord *record) {
    return record->getKind() == TaskOptionRecordKind::RunInline;
  }
};

} // end namespace nextcode

#endif
