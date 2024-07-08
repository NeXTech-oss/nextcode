//===------------------------------------------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_STDLIB_SHIMS_ASSERTIONREPORTING_H_
#define NEXTCODE_STDLIB_SHIMS_ASSERTIONREPORTING_H_

#include "NeXTCodeStdint.h"
#include "Visibility.h"

#if __has_feature(nullability)
#pragma clang assume_nonnull begin
#endif

#ifdef __cplusplus
extern "C" {
#endif

/// Report a fatal error to system console, stderr, and crash logs.
///
///     <prefix>: <message>: file <file>, line <line>\n
///
/// The message may be omitted by passing messageLength=0.
NEXTCODE_RUNTIME_STDLIB_API
void _nextcode_stdlib_reportFatalErrorInFile(
    const unsigned char *prefix, int prefixLength,
    const unsigned char *message, int messageLength,
    const unsigned char *file, int fileLength,
    __nextcode_uint32_t line,
    __nextcode_uint32_t flags);

/// Report a fatal error to system console, stderr, and crash logs.
///
///     <prefix>: <message>\n
NEXTCODE_RUNTIME_STDLIB_API
void _nextcode_stdlib_reportFatalError(
    const unsigned char *prefix, int prefixLength,
    const unsigned char *message, int messageLength,
    __nextcode_uint32_t flags);

/// Report a call to an unimplemented initializer.
///
///     <file>: <line>: <column>: fatal error: use of unimplemented
///     initializer '<initName>' for class '<className>'
NEXTCODE_RUNTIME_STDLIB_API
void _nextcode_stdlib_reportUnimplementedInitializerInFile(
    const unsigned char *className, int classNameLength,
    const unsigned char *initName, int initNameLength,
    const unsigned char *file, int fileLength,
    __nextcode_uint32_t line, __nextcode_uint32_t column,
    __nextcode_uint32_t flags);

/// Report a call to an unimplemented initializer.
///
///     fatal error: use of unimplemented initializer '<initName>'
///     for class 'className'
NEXTCODE_RUNTIME_STDLIB_API
void _nextcode_stdlib_reportUnimplementedInitializer(
    const unsigned char *className, int classNameLength,
    const unsigned char *initName, int initNameLength,
    __nextcode_uint32_t flags);

#ifdef __cplusplus
} // extern "C"
#endif

#if __has_feature(nullability)
#pragma clang assume_nonnull end
#endif

#endif // NEXTCODE_STDLIB_SHIMS_ASSERTIONREPORTING_H_

