//===--- Errors.h - Demangling library error handling -----------*- C++ -*-===//
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
// This file exists because not every client links to libnextcodeCore (the
// runtime), so calling nextcode::fatalError() or nextcode::warning() from within
// the demangler is not an option.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_DEMANGLING_ERRORS_H
#define NEXTCODE_DEMANGLING_ERRORS_H

#include "nextcode/Demangling/NamespaceMacros.h"
#include <inttypes.h>
#include <stdarg.h>

#ifndef NEXTCODE_FORMAT
// NEXTCODE_FORMAT(fmt,first) marks a function as taking a format string argument
// at argument `fmt`, with the first argument for the format string as `first`.
#if __has_attribute(format)
#define NEXTCODE_FORMAT(fmt, first) __attribute__((format(printf, fmt, first)))
#else
#define NEXTCODE_FORMAT(fmt, first)
#endif
#endif

#ifndef NEXTCODE_VFORMAT
// NEXTCODE_VFORMAT(fmt) marks a function as taking a format string argument at
// argument `fmt`, with the arguments in a `va_list`.
#if __has_attribute(format)
#define NEXTCODE_VFORMAT(fmt) __attribute__((format(printf, fmt, 0)))
#else
#define NEXTCODE_VFORMAT(fmt)
#endif
#endif

#ifndef NEXTCODE_NORETURN
#if __has_attribute(noreturn)
#define NEXTCODE_NORETURN __attribute__((__noreturn__))
#else
#define NEXTCODE_NORETURN
#endif
#endif

namespace nextcode {
namespace Demangle {
NEXTCODE_BEGIN_INLINE_NAMESPACE

NEXTCODE_NORETURN NEXTCODE_FORMAT(2, 3) void fatal(uint32_t flags, const char *format,
                                             ...);
NEXTCODE_FORMAT(2, 3) void warn(uint32_t flags, const char *format, ...);

NEXTCODE_NORETURN NEXTCODE_VFORMAT(2) void fatalv(uint32_t flags, const char *format,
                                            va_list val);
NEXTCODE_VFORMAT(2) void warnv(uint32_t flags, const char *format, va_list val);

NEXTCODE_END_INLINE_NAMESPACE
} // end namespace Demangle
} // end namespace nextcode

#endif // NEXTCODE_DEMANGLING_DEMANGLE_H
