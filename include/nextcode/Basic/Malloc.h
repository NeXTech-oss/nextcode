//===--- Malloc.h - Aligned malloc interface --------------------*- C++ -*-===//
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
//  This file provides an implementation of C11 aligned_alloc(3) for platforms
//  that don't have it yet, using posix_memalign(3).
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_MALLOC_H
#define NEXTCODE_BASIC_MALLOC_H

#include <cassert>
#if defined(_WIN32)
#include <malloc.h>
#else
#include <cstdlib>
#endif

namespace nextcode {

inline void *AlignedAlloc(size_t size, size_t align) {
#if defined(_WIN32)
  void *r = _aligned_malloc(size, align);
  assert(r && "_aligned_malloc failed");
#elif __STDC_VERSION__-0 >= 201112l
  // C11 supports aligned_alloc
  void *r = aligned_alloc(align, size);
  assert(r && "aligned_alloc failed");
#else
  // posix_memalign only accepts alignments greater than sizeof(void*).
  if (align < sizeof(void *))
    align = sizeof(void *);

  void *r = nullptr;
  int res = posix_memalign(&r, align, size);
  assert(res == 0 && "posix_memalign failed");
  (void)res; // Silence the unused variable warning.
#endif
  return r;
}

inline void AlignedFree(void *p) {
#if defined(_WIN32)
  _aligned_free(p);
#else
  free(p);
#endif
}

} // end namespace nextcode

#endif // NEXTCODE_BASIC_MALLOC_H
