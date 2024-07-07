//===--- GenericMetadataBuilder.h - Math utilities. -------------*- C++ -*-===//
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
// Utility functions for math operations.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_MATH_UTILS_H
#define NEXTCODE_BASIC_MATH_UTILS_H

#include "Compiler.h"
#include <cstddef>
#include <cstdint>

#if NEXTCODE_COMPILER_IS_MSVC
#include <intrin.h>
#endif

namespace nextcode {

/// Round the given value up to the given alignment, as a power of two.
template <class T>
static inline constexpr T roundUpToAlignment(T offset, T alignment) {
  return (offset + alignment - 1) & ~(alignment - 1);
}

/// Round the given value up to the given alignment, expressed as a mask (a
/// power of two minus one).
static inline size_t roundUpToAlignMask(size_t size, size_t alignMask) {
  return (size + alignMask) & ~alignMask;
}

static inline unsigned popcount(unsigned value) {
#if NEXTCODE_COMPILER_IS_MSVC && (defined(_M_IX86) || defined(_M_X64))
  // The __popcnt intrinsic is only available when targetting x86{_64} with MSVC.
  return __popcnt(value);
#elif __has_builtin(__builtin_popcount)
  return __builtin_popcount(value);
#else
  // From llvm/ADT/bit.h which the runtime doesn't have access to (yet?)
  uint32_t v = value;
  v = v - ((v >> 1) & 0x55555555);
  v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
  return int(((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24);
#endif
}

} // namespace nextcode

#endif // #ifndef NEXTCODE_BASIC_MATH_UTILS_H
