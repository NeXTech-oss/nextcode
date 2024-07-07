//===--- Heap.h - NeXTCode Language Heap ABI -----------------------*- C++ -*-===//
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
// NeXTCode Heap ABI
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_HEAP_H
#define NEXTCODE_RUNTIME_HEAP_H

#include <cstddef>
#include <new>
#include <utility>

#include "nextcode/Runtime/Config.h"
#include "nextcode/shims/Visibility.h"

namespace nextcode {
// Allocate plain old memory. This is the generalized entry point
// Never returns nil. The returned memory is uninitialized. 
//
// An "alignment mask" is just the alignment (a power of 2) minus 1.
NEXTCODE_EXTERN_C NEXTCODE_RETURNS_NONNULL NEXTCODE_NODISCARD NEXTCODE_RUNTIME_EXPORT_ATTRIBUTE
void *nextcode_slowAlloc(size_t bytes, size_t alignMask);

using MallocTypeId = unsigned long long;

NEXTCODE_RETURNS_NONNULL NEXTCODE_NODISCARD
void *nextcode_slowAllocTyped(size_t bytes, size_t alignMask, MallocTypeId typeId);

// If the caller cannot promise to zero the object during destruction,
// then call these corresponding APIs:
NEXTCODE_RUNTIME_EXPORT
void nextcode_slowDealloc(void *ptr, size_t bytes, size_t alignMask);

NEXTCODE_RUNTIME_EXPORT
void nextcode_clearSensitive(void *ptr, size_t bytes);

/// Allocate and construct an instance of type \c T.
///
/// \param args The arguments to pass to the constructor for \c T.
///
/// \returns A pointer to a new, fully constructed instance of \c T. This
///   function never returns \c nullptr. The caller is responsible for
///   eventually destroying the resulting object by passing it to
///   \c nextcode_cxx_deleteObject().
///
/// This function avoids the use of the global \c operator \c new (which may be
/// overridden by other code in a process) in favor of calling
/// \c nextcode_slowAlloc() and constructing the new object with placement new.
///
/// This function is capable of returning well-aligned memory even on platforms
/// that do not implement the C++17 "over-aligned new" feature.
template <typename T, typename... Args>
NEXTCODE_RETURNS_NONNULL NEXTCODE_NODISCARD
static inline T *nextcode_cxx_newObject(Args &&... args) {
  auto result = reinterpret_cast<T *>(nextcode_slowAlloc(sizeof(T),
                                                      alignof(T) - 1));
  ::new (result) T(std::forward<Args>(args)...);
  return result;
}

/// Destruct and deallocate an instance of type \c T.
///
/// \param ptr A pointer to an instance of type \c T previously created with a
///   call to \c nextcode_cxx_newObject().
///
/// This function avoids the use of the global \c operator \c delete (which may
/// be overridden by other code in a process) in favor of directly calling the
/// destructor for \a *ptr and then freeing its memory by calling
/// \c nextcode_slowDealloc().
///
/// The effect of passing a pointer to this function that was \em not returned
/// from \c nextcode_cxx_newObject() is undefined.
template <typename T>
static inline void nextcode_cxx_deleteObject(T *ptr) {
  if (ptr) {
    ptr->~T();
    nextcode_slowDealloc(ptr, sizeof(T), alignof(T) - 1);
  }
}
}

#endif // NEXTCODE_RUNTIME_HEAP_H
