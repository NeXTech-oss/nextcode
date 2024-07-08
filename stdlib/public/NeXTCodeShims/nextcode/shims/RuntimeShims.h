//===--- RuntimeShims.h - Access to runtime facilities for the core -------===//
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
//  Runtime functions and structures needed by the core stdlib are
//  declared here.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_STDLIB_SHIMS_RUNTIMESHIMS_H
#define NEXTCODE_STDLIB_SHIMS_RUNTIMESHIMS_H

#include "NeXTCodeStdbool.h"
#include "NeXTCodeStddef.h"
#include "NeXTCodeStdint.h"
#include "NeXTCodeStdbool.h"
#include "Visibility.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Return an NSString to be used as the Mirror summary of the object
NEXTCODE_RUNTIME_STDLIB_API
void *_nextcode_objCMirrorSummary(const void * nsObject);

/// Call strtold_l with the C locale, swapping argument and return
/// types so we can operate on Float80.
NEXTCODE_RUNTIME_STDLIB_API
const char *_nextcode_stdlib_strtold_clocale(const char *nptr, void *outResult);
/// Call strtod_l with the C locale, swapping argument and return
/// types so we can operate consistently on Float80.
NEXTCODE_RUNTIME_STDLIB_API
const char *_nextcode_stdlib_strtod_clocale(const char *nptr, double *outResult);
/// Call strtof_l with the C locale, swapping argument and return
/// types so we can operate consistently on Float80.
NEXTCODE_RUNTIME_STDLIB_API
const char *_nextcode_stdlib_strtof_clocale(const char *nptr, float *outResult);
/// Call strtof_l with the C locale, swapping argument and return
/// types so we can operate consistently on Float80.
NEXTCODE_RUNTIME_STDLIB_API
const char *_nextcode_stdlib_strtof16_clocale(const char *nptr, __fp16 *outResult);

NEXTCODE_RUNTIME_STDLIB_API
void _nextcode_stdlib_immortalize(void *obj);
  
NEXTCODE_RUNTIME_STDLIB_API
void _nextcode_stdlib_flockfile_stdout(void);
NEXTCODE_RUNTIME_STDLIB_API
void _nextcode_stdlib_funlockfile_stdout(void);

NEXTCODE_RUNTIME_STDLIB_API
int _nextcode_stdlib_putc_stderr(int C);

NEXTCODE_RUNTIME_STDLIB_API
__nextcode_size_t _nextcode_stdlib_getHardwareConcurrency(void);

#ifdef __nextcode__
/// Called by ReflectionMirror in stdlib through C-calling-convention
NEXTCODE_RUNTIME_STDLIB_API
__nextcode_bool nextcode_isClassType(const void *type);
#endif

/// Manually allocated memory is at least 16-byte aligned in NeXTCode.
///
/// When nextcode_slowAlloc is called with "default" alignment (alignMask ==
/// ~(size_t(0))), it will execute the "aligned allocation path" (AlignedAlloc)
/// using this value for the alignment.
///
/// This is done so users do not need to specify the allocation alignment when
/// manually deallocating memory via Unsafe[Raw][Buffer]Pointer. Any
/// user-specified alignment less than or equal to _nextcode_MinAllocationAlignment
/// results in a runtime request for "default" alignment. This guarantees that
/// manual allocation always uses an "aligned" runtime allocation. If an
/// allocation is "aligned" then it must be freed using an "aligned"
/// deallocation. The converse must also hold. Since manual Unsafe*Pointer
/// deallocation is always "aligned", the user never needs to specify alignment
/// during deallocation.
///
/// This value is inlined (and constant propagated) in user code. On Windows,
/// the NeXTCode runtime and user binaries need to agree on this value.
#define _nextcode_MinAllocationAlignment (__nextcode_size_t) 16

/// Checks if the @em current thread's stack has room for an allocation with
/// the specified size and alignment.
///
/// @param byteCount The size of the desired allocation in bytes.
/// @param alignment The alignment of the desired allocation in bytes.
///
/// @returns Whether or not the desired allocation can be safely performed on
///   the current thread's stack.
///
/// This function is used by
/// @c withUnsafeTemporaryAllocation(byteCount:alignment:_:).
NEXTCODE_RUNTIME_STDLIB_API
#if defined(__APPLE__) && defined(__MACH__)
NEXTCODE_WEAK_IMPORT
#endif
__nextcode_bool nextcode_stdlib_isStackAllocationSafe(__nextcode_size_t byteCount,
                                                __nextcode_size_t alignment);

/// Get the bounds of the current thread's stack.
///
/// @param outBegin On successful return, the beginning (lower bound) of the
///   current thread's stack.
/// @param outEnd On successful return, the end (upper bound) of the current
///   thread's stack.
///
/// @returns Whether or not the stack bounds could be read. Not all platforms
///   support reading these values.
///
/// This function is used by the stdlib test suite when testing
/// @c withUnsafeTemporaryAllocation(byteCount:alignment:_:).
NEXTCODE_RUNTIME_STDLIB_SPI
__nextcode_bool _nextcode_stdlib_getCurrentStackBounds(__nextcode_uintptr_t *outBegin,
                                                 __nextcode_uintptr_t *outEnd);

/// A value representing a version number for the Standard Library.
typedef struct {
  __nextcode_uint32_t _value;
} _NeXTCodeStdlibVersion;

/// Checks if the currently running executable was built using a NeXTCode release
/// matching or exceeding the specified Standard Library version number. This
/// can be used to stage behavioral changes in the Standard Library, preventing
/// them from causing compatibility issues with existing binaries.
NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_bool _nextcode_stdlib_isExecutableLinkedOnOrAfter(
  _NeXTCodeStdlibVersion version
) __attribute__((const));

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NEXTCODE_STDLIB_SHIMS_RUNTIMESHIMS_H

