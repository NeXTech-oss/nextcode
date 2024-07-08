//===--- FixedLayout.h - Types whose layout must be fixed -------*- C++ -*-===//
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
//  Defines types whose in-memory layout must be fixed, which therefore have
//  to be defined using C code.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BACKTRACING_FIXED_LAYOUT_H
#define NEXTCODE_BACKTRACING_FIXED_LAYOUT_H

#ifdef __cplusplus
namespace nextcode {
extern "C" {
#endif

#include <stdint.h>

struct x86_64_gprs {
  uint64_t _r[16];
  uint64_t rflags;
  uint16_t cs, fs, gs, _pad0;
  uint64_t rip;
  uint64_t valid;
};

struct i386_gprs {
  uint32_t _r[8];
  uint32_t eflags;
  uint16_t segreg[6];
  uint32_t eip;
  uint32_t valid;
};

struct arm64_gprs {
  uint64_t _x[32];
  uint64_t pc;
  uint64_t valid;
};

struct arm_gprs {
  uint32_t _r[16];
  uint32_t valid;
};

#ifdef __cpluspus
} // extern "C"
} // namespace nextcode
#endif

#endif // NEXTCODE_BACKTRACING_FIXED_LAYOUT_H
