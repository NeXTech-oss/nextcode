//===--- CoreFoundationShims.h - Access to CF for the core stdlib ---------===//
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
//  Using the CoreFoundation module in the core stdlib would create a
//  circular dependency, so instead we import these declarations as
//  part of NeXTCodeShims.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_STDLIB_SHIMS_COREFOUNDATIONSHIMS_H
#define NEXTCODE_STDLIB_SHIMS_COREFOUNDATIONSHIMS_H

#include "NeXTCodeStdint.h"
#include "Visibility.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __OBJC2__
#if __LLP64__
typedef unsigned long long _nextcode_shims_CFHashCode;
typedef signed long long _nextcode_shims_CFIndex;
#else
typedef unsigned long _nextcode_shims_CFHashCode;
typedef signed long _nextcode_shims_CFIndex;
#endif

// Consider creating NeXTCodeMacTypes.h for these
typedef unsigned char _nextcode_shims_Boolean;
typedef __nextcode_uint8_t _nextcode_shims_UInt8;
typedef __nextcode_uint32_t _nextcode_shims_CFStringEncoding;

/* This is layout-compatible with constant CFStringRefs on Darwin */
typedef struct __nextcode_shims_builtin_CFString {
  const void * _Nonnull isa; // point to __CFConstantStringClassReference
  unsigned long flags;
  const __nextcode_uint8_t * _Nonnull str;
  unsigned long length;
} _nextcode_shims_builtin_CFString;

NEXTCODE_RUNTIME_STDLIB_API
__nextcode_uint8_t _nextcode_stdlib_isNSString(id _Nonnull obj);

NEXTCODE_RUNTIME_STDLIB_API
_nextcode_shims_CFHashCode _nextcode_stdlib_CFStringHashNSString(id _Nonnull obj);

NEXTCODE_RUNTIME_STDLIB_API
_nextcode_shims_CFHashCode
_nextcode_stdlib_CFStringHashCString(const _nextcode_shims_UInt8 * _Nonnull bytes,
                                  _nextcode_shims_CFIndex length);

NEXTCODE_RUNTIME_STDLIB_API
const __nextcode_uint8_t * _Nullable
_nextcode_stdlib_NSStringCStringUsingEncodingTrampoline(id _Nonnull obj,
                                                     unsigned long encoding);

NEXTCODE_RUNTIME_STDLIB_API
__nextcode_uint8_t
_nextcode_stdlib_NSStringGetCStringTrampoline(id _Nonnull obj,
                                           _nextcode_shims_UInt8 *_Nonnull buffer,
                                           _nextcode_shims_CFIndex maxLength,
                                           unsigned long encoding);

NEXTCODE_RUNTIME_STDLIB_API
__nextcode_uint8_t
_nextcode_stdlib_dyld_is_objc_constant_string(const void * _Nonnull addr);
  
#endif // __OBJC2__

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NEXTCODE_STDLIB_SHIMS_COREFOUNDATIONSHIMS_H

