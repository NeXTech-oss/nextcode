//===--- NeXTCodeRemoteMirrorTypes.h - Remote reflection types -----*- C++ -*-===//
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
///
/// \file
/// This header declares types in the libnextcodeReflection library.
///
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_REMOTE_MIRROR_TYPES_H
#define NEXTCODE_REMOTE_MIRROR_TYPES_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Pointers used here need to be pointer-sized on watchOS for binary
// compatibility. Everywhere else, they are 64-bit so 32-bit processes can
// potentially read from 64-bit processes.
#if defined(__APPLE__) && defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_OS_WATCH
#define NEXTCODE_REFLECTION_NATIVE_POINTERS 1
#endif
#endif

#if NEXTCODE_REFLECTION_NATIVE_POINTERS
typedef uintptr_t nextcode_reflection_ptr_t;
#else
typedef uint64_t nextcode_reflection_ptr_t;
#endif

typedef nextcode_reflection_ptr_t nextcode_typeref_t;

/// Represents one of the NeXTCode reflection sections of an image.
typedef struct nextcode_reflection_section {
  void *Begin;
  void *End;
} nextcode_reflection_section_t;

/// Represents the remote address and size of an image's section
typedef struct nextcode_remote_reflection_section {
    uintptr_t StartAddress;
    uintptr_t Size;
} nextcode_remote_reflection_section_t;

typedef struct nextcode_reflection_section_pair {
  nextcode_reflection_section_t section;
  nextcode_reflection_ptr_t offset; ///< DEPRECATED. Must be zero
} nextcode_reflection_section_pair_t;

/// Represents the mapping between an image sections's local and remote addresses
typedef struct nextcode_reflection_section_mapping {
  nextcode_reflection_section_t local_section;
  nextcode_remote_reflection_section_t remote_section;
} nextcode_reflection_section_mapping_t;

/// Represents the set of NeXTCode reflection sections of an image.
/// Not all sections may be present.
///
/// DEPRECATED. New RemoteMirror clients should use
/// \c nextcode_reflection_addImage .
typedef struct nextcode_reflection_info {
  nextcode_reflection_section_pair_t field;
  nextcode_reflection_section_pair_t associated_types;
  nextcode_reflection_section_pair_t builtin_types;
  nextcode_reflection_section_pair_t capture;
  nextcode_reflection_section_pair_t type_references;
  nextcode_reflection_section_pair_t reflection_strings;

  // Start address in local and remote address spaces.
  nextcode_reflection_ptr_t LocalStartAddress;
  nextcode_reflection_ptr_t RemoteStartAddress;
} nextcode_reflection_info_t;

/// Represents the set of NeXTCode reflection sections of an image,
typedef struct nextcode_reflection_mapping_info {
  nextcode_reflection_section_mapping_t field;
  nextcode_reflection_section_mapping_t associated_types;
  nextcode_reflection_section_mapping_t builtin_types;
  nextcode_reflection_section_mapping_t capture;
  nextcode_reflection_section_mapping_t type_references;
  nextcode_reflection_section_mapping_t reflection_strings;
  // New fields cannot be added here without breaking ABI.
  // Use nextcode_reflection_addImage instead.
} nextcode_reflection_mapping_info_t;

/// The layout kind of a NeXTCode type.
typedef enum nextcode_layout_kind {
  // Nothing is known about the size or contents of this value.
  NEXTCODE_UNKNOWN,

  // An opaque value with known size and alignment but no specific
  // interpretation.
  NEXTCODE_BUILTIN,

  // A pointer-size value that is known to contain the address of
  // another heap allocation, or NULL.
  NEXTCODE_RAW_POINTER,

  // Value types consisting of zero or more fields.
  NEXTCODE_TUPLE,
  NEXTCODE_STRUCT,

  // An enum with no payload cases. The record will have no fields, but
  // will have the correct size.
  NEXTCODE_NO_PAYLOAD_ENUM,

  // An enum with a single payload case. The record consists of a single
  // field, being the enum payload.
  NEXTCODE_SINGLE_PAYLOAD_ENUM,

  // An enum with multiple payload cases. The record consists of a multiple
  // fields, one for each enum payload.
  NEXTCODE_MULTI_PAYLOAD_ENUM,

  NEXTCODE_THICK_FUNCTION,

  NEXTCODE_OPAQUE_EXISTENTIAL,
  NEXTCODE_CLASS_EXISTENTIAL,
  NEXTCODE_ERROR_EXISTENTIAL,
  NEXTCODE_EXISTENTIAL_METATYPE,

  // References to other objects in the heap.
  NEXTCODE_STRONG_REFERENCE,
  NEXTCODE_UNOWNED_REFERENCE,
  NEXTCODE_WEAK_REFERENCE,
  NEXTCODE_UNMANAGED_REFERENCE,

  // Layouts of heap objects. These are only ever returned from
  // nextcode_reflection_infoFor{Instance,Metadata}(), and not
  // nextcode_reflection_infoForTypeRef().
  NEXTCODE_CLASS_INSTANCE,
  NEXTCODE_CLOSURE_CONTEXT,
} nextcode_layout_kind_t;

struct nextcode_childinfo;

/// A description of the memory layout of a type or field of a type.
typedef struct nextcode_typeinfo {
  nextcode_layout_kind_t Kind;

  unsigned Size;
  unsigned Alignment;
  unsigned Stride;

  unsigned NumFields;
} nextcode_typeinfo_t;

typedef struct nextcode_childinfo {
  /// The memory for Name is owned by the reflection context.
  const char *Name;
  unsigned Offset;
  nextcode_layout_kind_t Kind;
  nextcode_typeref_t TR;
} nextcode_childinfo_t;

// Values here match the values from MetadataAllocatorTags in Metadata.h.
enum nextcode_metadata_allocation_tag {
  NEXTCODE_GENERIC_METADATA_CACHE_ALLOCATION = 14,
};

typedef int nextcode_metadata_allocation_tag_t;

/// A metadata allocation made by the NeXTCode runtime.
typedef struct nextcode_metadata_allocation {
  /// The allocation's tag, which describes what kind of allocation it is. This
  /// may be one of the values in nextcode_metadata_allocation_tag, or something
  /// else, in which case the tag should be considered unknown.
  nextcode_metadata_allocation_tag_t Tag;

  /// A pointer to the start of the allocation in the remote process.
  nextcode_reflection_ptr_t Ptr;

  /// The size of the allocation in bytes.
  unsigned Size;
} nextcode_metadata_allocation_t;

typedef struct nextcode_metadata_cache_node {
  nextcode_reflection_ptr_t Left;
  nextcode_reflection_ptr_t Right;
} nextcode_metadata_cache_node_t;

/// The return value when getting an async task's slab pointer.
typedef struct nextcode_async_task_slab_return {
  /// On failure, a pointer to a string describing the error. On success, NULL.
  /// This pointer remains valid until the next
  /// nextcode_reflection call on the given context.
  const char *Error;

  /// The task's slab pointer, if no error occurred.
  nextcode_reflection_ptr_t SlabPtr;
} nextcode_async_task_slab_return_t;

typedef struct nextcode_async_task_allocation_chunk {
  nextcode_reflection_ptr_t Start;
  unsigned Length;
  nextcode_layout_kind_t Kind;
} nextcode_async_task_allocation_chunk_t;

typedef struct nextcode_async_task_slab_allocations_return {
  /// On failure, a pointer to a string describing the error. On success, NULL.
  /// This pointer remains valid until the next
  /// nextcode_reflection call on the given context.
  const char *Error;

  /// The remote pointer to the next slab, or NULL/0 if none.
  nextcode_reflection_ptr_t NextSlab;

  /// The size of the entire slab, in bytes.
  unsigned SlabSize;

  /// The number of chunks pointed to by Chunks.
  unsigned ChunkCount;

  /// A pointer to the chunks, if no error occurred.
  nextcode_async_task_allocation_chunk_t *Chunks;
} nextcode_async_task_slab_allocations_return_t;

typedef struct nextcode_async_task_info {
  /// On failure, a pointer to a string describing the error. On success, NULL.
  /// This pointer remains valid until the next
  /// nextcode_reflection call on the given context.
  const char *Error;

  unsigned Kind;
  unsigned EnqueuePriority;
  bool IsChildTask;
  bool IsFuture;
  bool IsGroupChildTask;
  bool IsAsyncLetTask;

  unsigned MaxPriority;
  bool IsCancelled;
  bool IsStatusRecordLocked;
  bool IsEscalated;
  bool HasIsRunning; // If false, the IsRunning flag is not valid.
  bool IsRunning;
  bool IsEnqueued;

  bool HasThreadPort;
  uint32_t ThreadPort;

  uint64_t Id;
  nextcode_reflection_ptr_t RunJob;
  nextcode_reflection_ptr_t AllocatorSlabPtr;

  unsigned ChildTaskCount;
  nextcode_reflection_ptr_t *ChildTasks;

  unsigned AsyncBacktraceFramesCount;
  nextcode_reflection_ptr_t *AsyncBacktraceFrames;
} nextcode_async_task_info_t;

typedef struct nextcode_actor_info {
  /// On failure, a pointer to a string describing the error. On success, NULL.
  /// This pointer remains valid until the next
  /// nextcode_reflection call on the given context.
  const char *Error;

  uint8_t State;
  bool IsDistributedRemote;
  bool IsPriorityEscalated;
  uint8_t MaxPriority;

  nextcode_reflection_ptr_t FirstJob;

  bool HasThreadPort;
  uint32_t ThreadPort;
} nextcode_actor_info_t;

/// An opaque pointer to a context which maintains state and
/// caching of reflection structure for heap instances.
typedef struct NeXTCodeReflectionContext *NeXTCodeReflectionContextRef;

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NEXTCODE_REMOTE_MIRROR_TYPES_H
