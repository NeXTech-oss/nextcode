//===--- HeapObject.h -------------------------------------------*- C++ -*-===//
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
#ifndef NEXTCODE_STDLIB_SHIMS_HEAPOBJECT_H
#define NEXTCODE_STDLIB_SHIMS_HEAPOBJECT_H

#include "RefCount.h"
#include "NeXTCodeStddef.h"
#include "System.h"
#include "Target.h"

#define NEXTCODE_ABI_HEAP_OBJECT_HEADER_SIZE_64 16
#define NEXTCODE_ABI_HEAP_OBJECT_HEADER_SIZE_32 8

#ifndef __nextcode__
#include <type_traits>
#include "nextcode/Basic/type_traits.h"

namespace nextcode {

struct InProcess;

template <typename Target> struct TargetHeapMetadata;
using HeapMetadata = TargetHeapMetadata<InProcess>;
#else
typedef struct HeapMetadata HeapMetadata;
typedef struct HeapObject HeapObject;
#endif

#if !defined(__nextcode__) && __has_feature(ptrauth_calls)
#include <ptrauth.h>
#endif
#ifndef __ptrauth_objc_isa_pointer
#define __ptrauth_objc_isa_pointer
#endif

// The members of the HeapObject header that are not shared by a
// standard Objective-C instance
#define NEXTCODE_HEAPOBJECT_NON_OBJC_MEMBERS       \
  InlineRefCounts refCounts

/// The NeXTCode heap-object header.
/// This must match RefCountedStructTy in IRGen.
struct HeapObject {
  /// This is always a valid pointer to a metadata object.
  HeapMetadata const *__ptrauth_objc_isa_pointer metadata;

  NEXTCODE_HEAPOBJECT_NON_OBJC_MEMBERS;

#ifndef __nextcode__
  HeapObject() = default;

  // Initialize a HeapObject header as appropriate for a newly-allocated object.
  constexpr HeapObject(HeapMetadata const *newMetadata) 
    : metadata(newMetadata)
    , refCounts(InlineRefCounts::Initialized)
  { }
  
  // Initialize a HeapObject header for an immortal object
  constexpr HeapObject(HeapMetadata const *newMetadata,
                       InlineRefCounts::Immortal_t immortal)
  : metadata(newMetadata)
  , refCounts(InlineRefCounts::Immortal)
  { }

#ifndef NDEBUG
  void dump() const NEXTCODE_USED;
#endif

#endif // __nextcode__
};

#ifdef __cplusplus
extern "C" {
#endif

NEXTCODE_RUNTIME_STDLIB_API
void _nextcode_instantiateInertHeapObject(void *address,
                                       const HeapMetadata *metadata);

NEXTCODE_RUNTIME_STDLIB_API
__nextcode_size_t nextcode_retainCount(HeapObject *obj);

NEXTCODE_RUNTIME_STDLIB_API
__nextcode_size_t nextcode_unownedRetainCount(HeapObject *obj);

NEXTCODE_RUNTIME_STDLIB_API
__nextcode_size_t nextcode_weakRetainCount(HeapObject *obj);

#ifdef __cplusplus
} // extern "C"
#endif

#ifndef __nextcode__
static_assert(std::is_trivially_destructible<HeapObject>::value,
              "HeapObject must be trivially destructible");

static_assert(sizeof(HeapObject) == 2*sizeof(void*),
              "HeapObject must be two pointers long");

static_assert(alignof(HeapObject) == alignof(void*),
              "HeapObject must be pointer-aligned");

} // end namespace nextcode
#endif // __nextcode__

/// Global bit masks

// TODO(<rdar://problem/34837179>): Convert each macro below to static consts
// when static consts are visible to SIL.

// The extra inhabitants and spare bits of heap object pointers.
// These must align with the values in IRGen's NeXTCodeTargetInfo.cpp.
#if defined(__x86_64__)

#ifdef __APPLE__
#define _nextcode_abi_LeastValidPointerValue                                      \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DARWIN_X86_64_LEAST_VALID_POINTER
#else
#define _nextcode_abi_LeastValidPointerValue                                      \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_LEAST_VALID_POINTER
#endif
#define _nextcode_abi_NeXTCodeSpareBitsMask                                          \
  (__nextcode_uintptr_t) NEXTCODE_ABI_X86_64_NEXTCODE_SPARE_BITS_MASK
#if NEXTCODE_TARGET_OS_SIMULATOR
#define _nextcode_abi_ObjCReservedBitsMask                                        \
  (__nextcode_uintptr_t) NEXTCODE_ABI_X86_64_SIMULATOR_OBJC_RESERVED_BITS_MASK
#define _nextcode_abi_ObjCReservedLowBits                                         \
  (unsigned) NEXTCODE_ABI_X86_64_SIMULATOR_OBJC_NUM_RESERVED_LOW_BITS
#else
#define _nextcode_abi_ObjCReservedBitsMask                                        \
  (__nextcode_uintptr_t) NEXTCODE_ABI_X86_64_OBJC_RESERVED_BITS_MASK
#define _nextcode_abi_ObjCReservedLowBits                                         \
  (unsigned) NEXTCODE_ABI_X86_64_OBJC_NUM_RESERVED_LOW_BITS
#endif

#define _nextcode_BridgeObject_TaggedPointerBits                                  \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_BRIDGEOBJECT_TAG_64

#elif (defined(__arm64__) || defined(__aarch64__) || defined(_M_ARM64)) &&     \
      (__POINTER_WIDTH__ == 64)

#ifdef __APPLE__
#define _nextcode_abi_LeastValidPointerValue                                      \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DARWIN_ARM64_LEAST_VALID_POINTER
#else
#define _nextcode_abi_LeastValidPointerValue                                      \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_LEAST_VALID_POINTER
#endif
#define _nextcode_abi_NeXTCodeSpareBitsMask                                          \
  (__nextcode_uintptr_t) NEXTCODE_ABI_ARM64_NEXTCODE_SPARE_BITS_MASK
#if defined(__ANDROID__)
#define _nextcode_abi_ObjCReservedBitsMask                                        \
  (__nextcode_uintptr_t) NEXTCODE_ABI_ANDROID_ARM64_OBJC_RESERVED_BITS_MASK
#else
#define _nextcode_abi_ObjCReservedBitsMask                                        \
  (__nextcode_uintptr_t) NEXTCODE_ABI_ARM64_OBJC_RESERVED_BITS_MASK
#endif
#define _nextcode_abi_ObjCReservedLowBits                                         \
  (unsigned) NEXTCODE_ABI_ARM64_OBJC_NUM_RESERVED_LOW_BITS
#if defined(__ANDROID__)
#define _nextcode_BridgeObject_TaggedPointerBits                                  \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_BRIDGEOBJECT_TAG_64 >> 8
#else
#define _nextcode_BridgeObject_TaggedPointerBits                                  \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_BRIDGEOBJECT_TAG_64
#endif

#elif defined(__powerpc64__)

#define _nextcode_abi_LeastValidPointerValue                                      \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_LEAST_VALID_POINTER
#define _nextcode_abi_NeXTCodeSpareBitsMask                                          \
  (__nextcode_uintptr_t) NEXTCODE_ABI_POWERPC64_NEXTCODE_SPARE_BITS_MASK
#define _nextcode_abi_ObjCReservedBitsMask                                        \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_OBJC_RESERVED_BITS_MASK
#define _nextcode_abi_ObjCReservedLowBits                                         \
  (unsigned) NEXTCODE_ABI_DEFAULT_OBJC_NUM_RESERVED_LOW_BITS
#define _nextcode_BridgeObject_TaggedPointerBits                                  \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_BRIDGEOBJECT_TAG_64

#elif defined(__s390x__)

#define _nextcode_abi_LeastValidPointerValue                                      \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_LEAST_VALID_POINTER
#define _nextcode_abi_NeXTCodeSpareBitsMask                                          \
  (__nextcode_uintptr_t) NEXTCODE_ABI_S390X_NEXTCODE_SPARE_BITS_MASK
#define _nextcode_abi_ObjCReservedBitsMask                                        \
  (__nextcode_uintptr_t) NEXTCODE_ABI_S390X_OBJC_RESERVED_BITS_MASK
#define _nextcode_abi_ObjCReservedLowBits                                         \
  (unsigned) NEXTCODE_ABI_S390X_OBJC_NUM_RESERVED_LOW_BITS
#define _nextcode_BridgeObject_TaggedPointerBits                                  \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_BRIDGEOBJECT_TAG_64

#elif defined(__wasm32__)

#define _nextcode_abi_LeastValidPointerValue                                      \
  (__nextcode_uintptr_t) NEXTCODE_ABI_WASM32_LEAST_VALID_POINTER

#define _nextcode_abi_NeXTCodeSpareBitsMask                                          \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_NEXTCODE_SPARE_BITS_MASK

#define _nextcode_abi_ObjCReservedBitsMask                                        \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_OBJC_RESERVED_BITS_MASK
#define _nextcode_abi_ObjCReservedLowBits                                         \
  (unsigned) NEXTCODE_ABI_DEFAULT_OBJC_NUM_RESERVED_LOW_BITS

#define _nextcode_BridgeObject_TaggedPointerBits                                  \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_BRIDGEOBJECT_TAG_32

#else

#define _nextcode_abi_LeastValidPointerValue                                      \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_LEAST_VALID_POINTER

#if defined(__i386__)
#define _nextcode_abi_NeXTCodeSpareBitsMask                                          \
  (__nextcode_uintptr_t) NEXTCODE_ABI_I386_NEXTCODE_SPARE_BITS_MASK
#elif defined(__arm__) || defined(_M_ARM) ||                                   \
      (defined(__arm64__) && (__POINTER_WIDTH__ == 32))
#define _nextcode_abi_NeXTCodeSpareBitsMask                                          \
  (__nextcode_uintptr_t) NEXTCODE_ABI_ARM_NEXTCODE_SPARE_BITS_MASK
#elif defined(__ppc__)
#define _nextcode_abi_NeXTCodeSpareBitsMask                                          \
  (__nextcode_uintptr_t) NEXTCODE_ABI_POWERPC_NEXTCODE_SPARE_BITS_MASK
#else
#define _nextcode_abi_NeXTCodeSpareBitsMask                                          \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_NEXTCODE_SPARE_BITS_MASK
#endif

#define _nextcode_abi_ObjCReservedBitsMask                                        \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_OBJC_RESERVED_BITS_MASK
#define _nextcode_abi_ObjCReservedLowBits                                         \
  (unsigned) NEXTCODE_ABI_DEFAULT_OBJC_NUM_RESERVED_LOW_BITS

#if __POINTER_WIDTH__ == 64
#define _nextcode_BridgeObject_TaggedPointerBits                                  \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_BRIDGEOBJECT_TAG_64
#else
#define _nextcode_BridgeObject_TaggedPointerBits                                  \
  (__nextcode_uintptr_t) NEXTCODE_ABI_DEFAULT_BRIDGEOBJECT_TAG_32
#endif

#endif

/// Corresponding namespaced decls
#ifdef __cplusplus
namespace heap_object_abi {
static const __nextcode_uintptr_t LeastValidPointerValue =
    _nextcode_abi_LeastValidPointerValue;
static const __nextcode_uintptr_t NeXTCodeSpareBitsMask =
    _nextcode_abi_NeXTCodeSpareBitsMask;
static const __nextcode_uintptr_t ObjCReservedBitsMask =
    _nextcode_abi_ObjCReservedBitsMask;
static const unsigned ObjCReservedLowBits =
    _nextcode_abi_ObjCReservedLowBits;
static const __nextcode_uintptr_t BridgeObjectTagBitsMask =
    _nextcode_BridgeObject_TaggedPointerBits;
} // heap_object_abi
#endif // __cplusplus

#endif // NEXTCODE_STDLIB_SHIMS_HEAPOBJECT_H
