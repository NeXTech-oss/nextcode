//===--- Portability.h ------------------------------------------*- C++ -*-===//
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
// Includes stub APIs that make the portable runtime easier to write.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_PORTABILITY_H
#define NEXTCODE_RUNTIME_PORTABILITY_H

#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

size_t _nextcode_strlcpy(char *dst, const char *src, size_t maxlen);

// Skip the attribute when included by the compiler.
#ifdef NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE
NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE
#endif
#ifdef __clang__
__attribute((__format__(__printf__, 2, 0)))
#endif
inline static int
nextcode_vasprintf(char **strp, const char *fmt, va_list args) {
  va_list args_for_len;
  va_copy(args_for_len, args);
  int len = vsnprintf(nullptr, 0, fmt, args_for_len);
  va_end(args_for_len);

  // If we fail for any reason, strp needs to be set to NULL.
  *strp = nullptr;

  if (len < 0)
    return -1;
  char *buffer = reinterpret_cast<char *>(malloc(len + 1));
  if (!buffer)
    return -1;
  int result = vsnprintf(buffer, len + 1, fmt, args);
  if (result < 0) {
    free(buffer);
    return -1;
  }
  *strp = buffer;
  return result;
}

// Skip the attribute when included by the compiler.
#ifdef NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE
NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE
#endif
#ifdef __clang__
__attribute((__format__(__printf__, 2, 3)))
#endif
inline static int
nextcode_asprintf(char **strp, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int result = nextcode_vasprintf(strp, fmt, args);
  va_end(args);
  return result;
}

#endif
