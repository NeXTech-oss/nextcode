//===------------ ProfileCounter.h - PGO Propfile counter -------*- C++ -*-===//
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
/// \file Declares ProfileCounter, a convenient type for PGO
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_PROFILECOUNTER_H
#define NEXTCODE_BASIC_PROFILECOUNTER_H

#include <cassert>
#include <cstdint>

namespace nextcode {
/// A class designed to be smaller than using Optional<uint64_t> for PGO
class ProfileCounter {
private:
  uint64_t count;

public:
  explicit ProfileCounter() : count(UINT64_MAX) {}
  ProfileCounter(uint64_t Count) : count(Count) {
    if (Count == UINT64_MAX) {
      count = UINT64_MAX - 1;
    }
  }

  bool hasValue() const { return count != UINT64_MAX; }
  uint64_t getValue() const {
    assert(hasValue());
    return count;
  }
  explicit operator bool() const { return hasValue(); }
};
} // end namespace nextcode

#endif // NEXTCODE_BASIC_PROFILECOUNTER_H
