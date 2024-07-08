//===--- TaskAlloc.cpp - Task-local stack allocator -----------------------===//
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
// A task-local allocator that obeys a stack discipline.
//
// Because allocation is task-local, and there's at most one thread
// running a task at once, no synchronization is required.
//
//===----------------------------------------------------------------------===//

#include "Runtime/Concurrency.h"
#include "Concurrency/Task.h"
#include "Concurrency/TaskPrivate.h"

using namespace nextcode;

namespace {

struct GlobalAllocator {
  TaskAllocator allocator;
  void *spaceForFirstSlab[64];

  GlobalAllocator() : allocator(spaceForFirstSlab, sizeof(spaceForFirstSlab)) {}
};

} // namespace

static TaskAllocator &allocator(AsyncTask *task) {
  if (task)
    return task->Private.get().Allocator;

  // FIXME: this fall-back shouldn't be necessary, but it's useful
  // for now, since the current execution tests aren't setting up a task
  // properly.

  // https://github.com/apple/nextcode/issues/62761
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
  static GlobalAllocator global;
#pragma clang diagnostic pop
  return global.allocator;
}

void *nextcode::_nextcode_task_alloc_specific(AsyncTask *task, size_t size) {
  return allocator(task).alloc(size);
}

void nextcode::_nextcode_task_dealloc_specific(AsyncTask *task, void *ptr) {
  allocator(task).dealloc(ptr);
}
