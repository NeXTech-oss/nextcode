//===--- RuntimeInternals.h - Runtime Internal Structures -------*- C++ -*-===//
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
// Runtime data structures that Reflection inspects externally.
//
// FIXME: Ideally the original definitions would be templatized on a Runtime
// parameter and we could use the original definitions in both the runtime and
// in Reflection.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_REFLECTION_RUNTIME_INTERNALS_H
#define NEXTCODE_REFLECTION_RUNTIME_INTERNALS_H

#include <stdint.h>

namespace nextcode {

namespace reflection {

template <typename Runtime>
struct ConformanceNode {
  typename Runtime::StoredPointer Left, Right;
  typename Runtime::StoredPointer Type;
  typename Runtime::StoredPointer Proto;
  typename Runtime::StoredPointer Description;
  typename Runtime::StoredSize FailureGeneration;
};

template <typename Runtime>
struct MetadataAllocation {
  uint16_t Tag;
  typename Runtime::StoredPointer Ptr;
  unsigned Size;
};

template <typename Runtime> struct MetadataCacheNode {
  typename Runtime::StoredPointer Left;
  typename Runtime::StoredPointer Right;
};

template <typename Runtime> struct ConcurrentHashMap {
  uint32_t ReaderCount;
  uint32_t ElementCount;
  typename Runtime::StoredPointer Elements;
  typename Runtime::StoredPointer Indices;
  // We'll ignore the remaining fields for now....
};

template <typename Runtime> struct ConformanceCacheEntry {
  typename Runtime::StoredPointer Type;
  typename Runtime::StoredPointer Proto;
  typename Runtime::StoredPointer Witness;
};

template <typename Runtime>
struct HeapObject {
  typename Runtime::StoredPointer Metadata;
  typename Runtime::StoredSize RefCounts;
};

template <typename Runtime>
struct Job {
  HeapObject<Runtime> HeapObject;
  typename Runtime::StoredPointer SchedulerPrivate[2];
  uint32_t Flags;
  uint32_t Id;
  typename Runtime::StoredPointer Reserved[2];
  typename Runtime::StoredSignedPointer RunJob;
};

template <typename Runtime>
struct StackAllocator {
  typename Runtime::StoredPointer LastAllocation;
  typename Runtime::StoredPointer FirstSlab;
  int32_t NumAllocatedSlabs;
  bool FirstSlabIsPreallocated;

  struct Slab {
    typename Runtime::StoredPointer Metadata;
    typename Runtime::StoredPointer Next;
    uint32_t Capacity;
    uint32_t CurrentOffset;
  };
};

template <typename Runtime>
struct ActiveTaskStatusWithEscalation {
  uint32_t Flags[1];
  uint32_t ExecutionLock[(sizeof(typename Runtime::StoredPointer) == 8) ? 1 : 2];
  typename Runtime::StoredPointer Record;
};

template <typename Runtime>
struct ActiveTaskStatusWithoutEscalation {
  uint32_t Flags[sizeof(typename Runtime::StoredPointer) == 8 ? 2 : 1];
  typename Runtime::StoredPointer Record;
};

struct ActiveTaskStatusFlags {
  static const uint32_t PriorityMask = 0xFF;
  static const uint32_t IsCancelled = 0x100;
  static const uint32_t IsStatusRecordLocked = 0x200;
  static const uint32_t IsEscalated = 0x400;
  static const uint32_t IsRunning = 0x800;
  static const uint32_t IsEnqueued = 0x1000;
  static const uint32_t IsComplete = 0x2000;
};

template <typename Runtime, typename ActiveTaskStatus>
struct AsyncTaskPrivateStorage {
  typename Runtime::StoredPointer ExclusivityAccessSet[2];
  ActiveTaskStatus Status;
  StackAllocator<Runtime> Allocator;
  typename Runtime::StoredPointer Local;
  uint32_t Id;
  uint32_t BasePriority;
};

template <typename Runtime, typename ActiveTaskStatus>
struct AsyncTask: Job<Runtime> {
  // On 64-bit, there's a Reserved64 after ResumeContext.
  typename Runtime::StoredPointer ResumeContextAndReserved[
    sizeof(typename Runtime::StoredPointer) == 8 ? 2 : 1];
  union {
    AsyncTaskPrivateStorage<Runtime, ActiveTaskStatus> PrivateStorage;
    typename Runtime::StoredPointer PrivateStorageRaw[14];
  };
};

template <typename Runtime>
struct AsyncContext {
  typename Runtime::StoredSignedPointer Parent;
  typename Runtime::StoredSignedPointer ResumeParent;
};

template <typename Runtime>
struct AsyncContextPrefix {
  typename Runtime::StoredSignedPointer AsyncEntryPoint;
  typename Runtime::StoredPointer ClosureContext;
  typename Runtime::StoredPointer ErrorResult;
};

template <typename Runtime>
struct FutureAsyncContextPrefix {
  typename Runtime::StoredPointer IndirectResult;
  typename Runtime::StoredSignedPointer AsyncEntryPoint;
  typename Runtime::StoredPointer ClosureContext;
  typename Runtime::StoredPointer ErrorResult;
};

template <typename Runtime>
struct ActiveActorStatusWithEscalation {
  uint32_t Flags[1];
  uint32_t DrainLock[(sizeof(typename Runtime::StoredPointer) == 8) ? 1 : 2];
  typename Runtime::StoredPointer FirstJob;
};

template <typename Runtime>
struct ActiveActorStatusWithoutEscalation {
  uint32_t Flags[sizeof(typename Runtime::StoredPointer) == 8 ? 2 : 1];
  typename Runtime::StoredPointer FirstJob;
};

template <typename Runtime, typename ActiveActorStatus>
struct DefaultActorImpl {
  HeapObject<Runtime> HeapObject;
  Job<Runtime> JobStorage;
  ActiveActorStatus Status;
  bool IsDistributedRemote;
};

template <typename Runtime>
struct TaskStatusRecord {
  typename Runtime::StoredSize Flags;
  typename Runtime::StoredPointer Parent;
};

template <typename Runtime>
struct ChildTaskStatusRecord : TaskStatusRecord<Runtime> {
  typename Runtime::StoredPointer FirstChild;
};

template <typename Runtime>
struct TaskGroupTaskStatusRecord : TaskStatusRecord<Runtime> {
  typename Runtime::StoredPointer FirstChild;
};

template <typename Runtime>
struct ChildFragment {
  typename Runtime::StoredPointer Parent;
  typename Runtime::StoredPointer NextChild;
};

} // end namespace reflection
} // end namespace nextcode

#endif // NEXTCODE_REFLECTION_RUNTIME_INTERNALS_H