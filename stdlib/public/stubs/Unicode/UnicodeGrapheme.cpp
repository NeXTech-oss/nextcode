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
#include "Common/GraphemeData.h"
#else
#include "nextcode/Runtime/Debug.h"
#endif
#include "nextcode/shims/UnicodeData.h"
#include <stdint.h>


NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_uint8_t _nextcode_stdlib_getGraphemeBreakProperty(__nextcode_uint32_t scalar) {
#if !NEXTCODE_STDLIB_ENABLE_UNICODE_DATA
  nextcode::nextcode_abortDisabledUnicodeSupport();
#else
  auto index = 1; //0th element is a dummy element
  while (index < GRAPHEME_BREAK_DATA_COUNT) {
    auto entry = _nextcode_stdlib_graphemeBreakProperties[index];
    
    // Shift the enum and range count out of the value.
    auto lower = (entry << 11) >> 11;
    
    // Shift the enum out first, then shift out the scalar value.
    auto upper = lower + ((entry << 3) >> 24);
    
    // Shift everything out.
    auto enumValue = (__nextcode_uint8_t)(entry >> 29);
    
    // Special case: extendedPictographic who used an extra bit for the range.
    if (enumValue == 5) {
      upper = lower + ((entry << 2) >> 23);
    }
    
    //If we want the left child of the current node in our virtual tree,
    //that's at index * 2, if we want the right child it's at (index * 2) + 1
    if (scalar < lower) {
      index = 2 * index;
    } else if (scalar <= upper) {
      return enumValue;
    } else {
      index = 2 * index + 1;
    }
  }
  // If we made it out here, then our scalar was not found in the grapheme
  // array (this occurs when a scalar doesn't map to any grapheme break
  // property). Return the max value here to indicate .any.
  return 0xFF;
#endif
}

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_bool _nextcode_stdlib_isLinkingConsonant(__nextcode_uint32_t scalar) {
#if !NEXTCODE_STDLIB_ENABLE_UNICODE_DATA
  nextcode::nextcode_abortDisabledUnicodeSupport();
#else
  auto idx = _nextcode_stdlib_getScalarBitArrayIdx(scalar,
                                          _nextcode_stdlib_linkingConsonant,
                                          _nextcode_stdlib_linkingConsonant_ranks);

  if (idx == INTPTR_MAX) {
    return false;
  }

  return true;
#endif
}
