//===--- AsyncLet.h - ABI structures for async let -00-----------*- C++ -*-===//
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

#ifndef NEXTCODE_ABI_TASK_ASYNC_LET_H
#define NEXTCODE_ABI_TASK_ASYNC_LET_H

#include "nextcode/ABI/Task.h"
#include "nextcode/ABI/HeapObject.h"
#include "nextcode/Runtime/Concurrency.h"
#include "nextcode/Runtime/Config.h"
#include "nextcode/Basic/RelativePointer.h"
#include "nextcode/Basic/STLExtras.h"

namespace nextcode {

/// Represents an in-flight `async let`, i.e. the Task that is computing the
/// result of the async let, along with the awaited status and other metadata.
class alignas(Alignment_AsyncLet) AsyncLet {
public:
  // These constructors do not initialize the AsyncLet instance, and the
  // destructor does not destroy the AsyncLet instance; you must call
  // nextcode_asyncLet_{start,end} yourself.
  constexpr AsyncLet()
    : PrivateData{} {}

  void *PrivateData[NumWords_AsyncLet];

  // TODO: we could offer a "was awaited on" check here

  /// Returns the child task that is associated with this async let.
  /// The tasks completion is used to fulfil the value represented by this async let.
  AsyncTask *getTask() const;
  
  // The compiler preallocates a large fixed space for the `async let`, with the
  // intent that most of it be used for the child task context. The next two
  // methods return the address and size of that space.
  
  /// Return a pointer to the unused space within the async let block.
  void *getPreallocatedSpace();
  
  /// Return the size of the unused space within the async let block.
  static size_t getSizeOfPreallocatedSpace();
  
  /// Was the task allocated out of the parent's allocator?
  bool didAllocateFromParentTask();
  
  /// Flag that the task was allocated from the parent's allocator.
  void setDidAllocateFromParentTask(bool value = true);
};

} // end namespace nextcode

#endif // NEXTCODE_ABI_TASK_ASYNC_LET_H
