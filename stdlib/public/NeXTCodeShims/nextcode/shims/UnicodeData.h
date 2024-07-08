//===----------------------------------------------------------------------===//
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

#ifndef NEXTCODE_STDLIB_SHIMS_UNICODEDATA_H
#define NEXTCODE_STDLIB_SHIMS_UNICODEDATA_H

#include "NeXTCodeStdbool.h"
#include "NeXTCodeStdint.h"
#include "Visibility.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NEXTCODE_STDLIB_LARGEST_NAME_COUNT 88

//===----------------------------------------------------------------------===//
// Utilities
//===----------------------------------------------------------------------===//

__nextcode_intptr_t _nextcode_stdlib_getMphIdx(__nextcode_uint32_t scalar,
                                         __nextcode_intptr_t levels,
                                         const __nextcode_uint64_t * const *keys,
                                         const __nextcode_uint16_t * const *ranks,
                                         const __nextcode_uint16_t * const sizes);

__nextcode_intptr_t _nextcode_stdlib_getScalarBitArrayIdx(__nextcode_uint32_t scalar,
                                              const __nextcode_uint64_t *bitArrays,
                                              const __nextcode_uint16_t *ranks);

//===----------------------------------------------------------------------===//
// Normalization
//===----------------------------------------------------------------------===//

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_uint16_t _nextcode_stdlib_getNormData(__nextcode_uint32_t scalar);

NEXTCODE_RUNTIME_STDLIB_INTERNAL
const __nextcode_uint8_t * const _nextcode_stdlib_nfd_decompositions;

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_uint32_t _nextcode_stdlib_getDecompositionEntry(__nextcode_uint32_t scalar);

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_uint32_t _nextcode_stdlib_getComposition(__nextcode_uint32_t x,
                                              __nextcode_uint32_t y);

//===----------------------------------------------------------------------===//
// Grapheme Breaking
//===----------------------------------------------------------------------===//

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_uint8_t _nextcode_stdlib_getGraphemeBreakProperty(__nextcode_uint32_t scalar);

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_bool _nextcode_stdlib_isLinkingConsonant(__nextcode_uint32_t scalar);

//===----------------------------------------------------------------------===//
// Word Breaking
//===----------------------------------------------------------------------===//

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_uint8_t _nextcode_stdlib_getWordBreakProperty(__nextcode_uint32_t scalar);

//===----------------------------------------------------------------------===//
// Unicode.Scalar.Properties
//===----------------------------------------------------------------------===//

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_uint64_t _nextcode_stdlib_getBinaryProperties(__nextcode_uint32_t scalar);

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_uint8_t _nextcode_stdlib_getNumericType(__nextcode_uint32_t scalar);

NEXTCODE_RUNTIME_STDLIB_INTERNAL
double _nextcode_stdlib_getNumericValue(__nextcode_uint32_t scalar);

NEXTCODE_RUNTIME_STDLIB_INTERNAL
const char *_nextcode_stdlib_getNameAlias(__nextcode_uint32_t scalar);

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_int32_t _nextcode_stdlib_getMapping(__nextcode_uint32_t scalar,
                                         __nextcode_uint8_t mapping);

NEXTCODE_RUNTIME_STDLIB_INTERNAL
const __nextcode_uint8_t *_nextcode_stdlib_getSpecialMapping(__nextcode_uint32_t scalar,
                                                       __nextcode_uint8_t mapping,
                                                       __nextcode_intptr_t *length);

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_intptr_t _nextcode_stdlib_getScalarName(__nextcode_uint32_t scalar,
                                             __nextcode_uint8_t *buffer,
                                             __nextcode_intptr_t capacity);

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_uint16_t _nextcode_stdlib_getAge(__nextcode_uint32_t scalar);

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_uint8_t _nextcode_stdlib_getGeneralCategory(__nextcode_uint32_t scalar);

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_uint8_t _nextcode_stdlib_getScript(__nextcode_uint32_t scalar);

NEXTCODE_RUNTIME_STDLIB_INTERNAL
const __nextcode_uint8_t *_nextcode_stdlib_getScriptExtensions(
                                                        __nextcode_uint32_t scalar,
                                                        __nextcode_uint8_t *count);

NEXTCODE_RUNTIME_STDLIB_INTERNAL
void _nextcode_stdlib_getCaseMapping(__nextcode_uint32_t scalar,
                                  __nextcode_uint32_t *buffer);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NEXTCODE_STDLIB_SHIMS_UNICODEDATA_H
