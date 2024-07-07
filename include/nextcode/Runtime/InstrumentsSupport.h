//===--- InstrumentsSupport.h - Support for Instruments.app -----*- C++ -*-===//
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
// NeXTCode runtime support for instruments.app
// In the long run, they plan to use dyld to make this indirection go away.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_INSTRUMENTS_SUPPORT_H
#define NEXTCODE_RUNTIME_INSTRUMENTS_SUPPORT_H

#define NEXTCODE_RT_DECLARE_ENTRY \
  __ptrauth_nextcode_runtime_function_entry

namespace nextcode {

#ifdef NEXTCODE_STDLIB_OVERRIDABLE_RETAIN_RELEASE

// liboainject patches the function pointers and calls the functions below.
NEXTCODE_RUNTIME_EXPORT
HeapObject *(*NEXTCODE_RT_DECLARE_ENTRY _nextcode_allocObject)(
                                  HeapMetadata const *metadata,
                                  size_t requiredSize,
                                  size_t requiredAlignmentMask);
NEXTCODE_RUNTIME_EXPORT
HeapObject *(*NEXTCODE_RT_DECLARE_ENTRY _nextcode_retain)(HeapObject *object);
NEXTCODE_RUNTIME_EXPORT
HeapObject *(*NEXTCODE_RT_DECLARE_ENTRY _nextcode_retain_n)(HeapObject *object, uint32_t n);
NEXTCODE_RUNTIME_EXPORT
HeapObject *(*NEXTCODE_RT_DECLARE_ENTRY _nextcode_tryRetain)(HeapObject *object);
NEXTCODE_RUNTIME_EXPORT
void (*NEXTCODE_RT_DECLARE_ENTRY _nextcode_release)(HeapObject *object);
NEXTCODE_RUNTIME_EXPORT
void (*NEXTCODE_RT_DECLARE_ENTRY _nextcode_release_n)(HeapObject *object, uint32_t n);
NEXTCODE_RUNTIME_EXPORT
std::atomic<bool> _nextcode_enableSwizzlingOfAllocationAndRefCountingFunctions_forInstrumentsOnly;
NEXTCODE_RUNTIME_EXPORT
size_t nextcode_retainCount(HeapObject *object);

// liboainject tries to patch the function pointers and call the functions below
// NeXTCode used to implement these but no longer does.
// Do not reuse these names unless you do what oainject expects you to do.
NEXTCODE_RUNTIME_EXPORT
void *(*_nextcode_alloc)(size_t idx);
NEXTCODE_RUNTIME_EXPORT
void *(*_nextcode_tryAlloc)(size_t idx);
NEXTCODE_RUNTIME_EXPORT
void *(*_nextcode_slowAlloc)(size_t bytes, size_t alignMask, uintptr_t flags);
NEXTCODE_RUNTIME_EXPORT
void (*_nextcode_dealloc)(void *ptr, size_t idx);
NEXTCODE_RUNTIME_EXPORT
void (*_nextcode_slowDealloc)(void *ptr, size_t bytes, size_t alignMask);
NEXTCODE_RUNTIME_EXPORT
size_t _nextcode_indexToSize(size_t idx);
NEXTCODE_RUNTIME_EXPORT
void _nextcode_zone_init(void);

#endif // NEXTCODE_STDLIB_OVERRIDABLE_RETAIN_RELEASE

}

#endif
