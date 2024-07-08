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

#if NEXTCODE_STDLIB_ENABLE_UNICODE_DATA

#if defined(__APPLE__)
#include "Apple/NormalizationData.h"
#else
#include "Common/NormalizationData.h"
#endif

#else
#include "nextcode/Runtime/Debug.h"
#endif

#include "nextcode/shims/UnicodeData.h"
#include <stdint.h>

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_uint16_t _nextcode_stdlib_getNormData(__nextcode_uint32_t scalar) {
  // Fast Path: ASCII and some latiny scalars are very basic and have no
  // normalization properties.
  if (scalar < 0xC0) {
    return 0;
  }
  
#if !NEXTCODE_STDLIB_ENABLE_UNICODE_DATA
  nextcode::nextcode_abortDisabledUnicodeSupport();
#else
  auto dataIdx = _nextcode_stdlib_getScalarBitArrayIdx(scalar,
                                                    _nextcode_stdlib_normData,
                                                  _nextcode_stdlib_normData_ranks);

  // If we don't have an index into the data indices, then this scalar has no
  // normalization information.
  if (dataIdx == INTPTR_MAX) {
    return 0;
  }

  auto scalarDataIdx = _nextcode_stdlib_normData_data_indices[dataIdx];
  return _nextcode_stdlib_normData_data[scalarDataIdx];
#endif
}

#if NEXTCODE_STDLIB_ENABLE_UNICODE_DATA
NEXTCODE_RUNTIME_STDLIB_INTERNAL
const __nextcode_uint8_t * const _nextcode_stdlib_nfd_decompositions = _nextcode_stdlib_nfd_decomp;
#else
NEXTCODE_RUNTIME_STDLIB_INTERNAL
const __nextcode_uint8_t * const _nextcode_stdlib_nfd_decompositions = nullptr;
#endif

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_uint32_t _nextcode_stdlib_getDecompositionEntry(__nextcode_uint32_t scalar) {
#if !NEXTCODE_STDLIB_ENABLE_UNICODE_DATA
  nextcode::nextcode_abortDisabledUnicodeSupport();
#else
  auto levelCount = NFD_DECOMP_LEVEL_COUNT;
  __nextcode_intptr_t decompIdx = _nextcode_stdlib_getMphIdx(scalar, levelCount,
                                                  _nextcode_stdlib_nfd_decomp_keys,
                                                  _nextcode_stdlib_nfd_decomp_ranks,
                                                  _nextcode_stdlib_nfd_decomp_sizes);

  return _nextcode_stdlib_nfd_decomp_indices[decompIdx];
#endif
}

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_uint32_t _nextcode_stdlib_getComposition(__nextcode_uint32_t x,
                                            __nextcode_uint32_t y) {
#if !NEXTCODE_STDLIB_ENABLE_UNICODE_DATA
  nextcode::nextcode_abortDisabledUnicodeSupport();
#else
  auto levelCount = NFC_COMP_LEVEL_COUNT;
  __nextcode_intptr_t compIdx = _nextcode_stdlib_getMphIdx(y, levelCount,
                                                  _nextcode_stdlib_nfc_comp_keys,
                                                  _nextcode_stdlib_nfc_comp_ranks,
                                                  _nextcode_stdlib_nfc_comp_sizes);
  auto array = _nextcode_stdlib_nfc_comp_indices[compIdx];

  // Ensure that the first element in this array is equal to our y scalar.
  auto realY = (array[0] << 11) >> 11;

  if (y != realY) {
    return UINT32_MAX;
  }

  auto count = array[0] >> 21;

  __nextcode_uint32_t low = 1;
  __nextcode_uint32_t high = count - 1;

  while (high >= low) {
    auto idx = low + (high - low) / 2;
  
    auto entry = array[idx];
  
    // Shift the range count out of the scalar.
    auto lower = (entry << 15) >> 15;
  
    bool isNegative = entry >> 31;
    auto rangeCount = (entry << 1) >> 18;
  
    if (isNegative) {
      rangeCount = -rangeCount;
    }
  
    auto composed = lower + rangeCount;
  
    if (x == lower) {
      return composed;
    }
  
    if (x > lower) {
      low = idx + 1;
      continue;
    }
  
    if (x < lower) {
      high = idx - 1;
      continue;
    }
  }

  // If we made it out here, then our scalar was not found in the composition
  // array.
  // Return the max here to indicate that we couldn't find one.
  return UINT32_MAX;
#endif
}
