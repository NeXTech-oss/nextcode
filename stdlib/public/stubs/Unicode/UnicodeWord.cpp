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
//             Tural Ghuliev (turalquliyev@it-gss.com)
//
//===----------------------------------------------------------------------===//

#if NEXTCODE_STDLIB_ENABLE_UNICODE_DATA
#include "Common/WordData.h"
#else
#include "nextcode/Runtime/Debug.h"
#endif
#include "nextcode/shims/UnicodeData.h"
#include <stdint.h>

NEXTCODE_RUNTIME_STDLIB_INTERNAL
__nextcode_uint8_t _nextcode_stdlib_getWordBreakProperty(__nextcode_uint32_t scalar) {
#if !NEXTCODE_STDLIB_ENABLE_UNICODE_DATA
  nextcode::nextcode_abortDisabledUnicodeSupport();
#else
  auto index = 1; //0th element is a dummy element
  while (index < WORD_BREAK_DATA_COUNT) {
    auto entry = _nextcode_stdlib_words[index];

    // Shift the range count out of the value.
    auto lower = (entry << 11) >> 11;
    
    // Shift the enum out first, then shift out the scalar value.
    auto upper = lower + (entry >> 21) - 1;

    //If we want the left child of the current node in our virtual tree,
    //that's at index * 2, if we want the right child it's at (index * 2) + 1
    if (scalar < lower) {
      index = 2 * index;
    } else if (scalar <= upper) {
      return _nextcode_stdlib_words_data[index];
    } else {
      index = 2 * index + 1;
    }
  }
  // If we made it out here, then our scalar was not found in the word
  // array (this occurs when a scalar doesn't map to any word break
  // property). Return the max value here to indicate .any.
  return UINT8_MAX;
#endif
}
