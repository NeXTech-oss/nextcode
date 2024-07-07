//===--- TaskGroup.h - ABI structures for task groups -00--------*- C++ -*-===//
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
// NeXTCode ABI describing task groups.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_ABI_TASK_GROUP_H
#define NEXTCODE_ABI_TASK_GROUP_H

#include "nextcode/ABI/Task.h"
#include "nextcode/ABI/TaskStatus.h"
#include "nextcode/ABI/HeapObject.h"
#include "nextcode/Runtime/Concurrency.h"
#include "nextcode/Runtime/Config.h"
#include "nextcode/Basic/RelativePointer.h"
#include "nextcode/Basic/STLExtras.h"

namespace nextcode {

/// The task group is responsible for maintaining dynamically created child tasks.
class alignas(Alignment_TaskGroup) TaskGroup {
public:
  // These constructors do not initialize the group instance, and the
  // destructor does not destroy the group instance; you must call
  // nextcode_taskGroup_{initialize,destroy} yourself.
  constexpr TaskGroup()
    : PrivateData{} {}

  void *PrivateData[NumWords_TaskGroup];

  /// Upon a future task's completion, offer it to the task group it belongs to.
  void offer(AsyncTask *completed, AsyncContext *context);

  /// Checks the cancellation status of the group.
  bool isCancelled();

  // Add a child task to the task group. Always called while holding the
  // status record lock of the task group's owning task.
  void addChildTask(AsyncTask *task);

  // Remove a child task from the task group. Always called while holding
  // the status record lock of the task group's owning task.
  void removeChildTask(AsyncTask *task);

  // Provide accessor for task group's status record
  TaskGroupTaskStatusRecord *getTaskRecord();

  /// The group is a `TaskGroup` that accumulates results.
  bool isAccumulatingResults() {
    return !isDiscardingResults();
  }

  /// The group is a `DiscardingTaskGroup` that discards results.
  bool isDiscardingResults();
};

} // end namespace nextcode

#endif // NEXTCODE_ABI_TASK_GROUP_H
