//===--- UUID.h - UUID generation -------------------------------*- C++ -*-===//
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
// This is an interface over the standard OSF uuid library that gives UUIDs
// sound value semantics and operators.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_UUID_H
#define NEXTCODE_BASIC_UUID_H

#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/Support/raw_ostream.h"
#include <array>
#include <optional>

namespace nextcode {
  
class UUID {
public:
  enum {
    /// The number of bytes in a UUID's binary representation.
    Size = 16,
    
    /// The number of characters in a UUID's string representation.
    StringSize = 36,
    
    /// The number of bytes necessary to store a null-terminated UUID's string
    /// representation.
    StringBufferSize = StringSize + 1,
  };
  
  unsigned char Value[Size];

private:
  enum FromRandom_t { FromRandom };
  enum FromTime_t { FromTime };

  UUID(FromRandom_t);
  
  UUID(FromTime_t);
  
public:
  /// Default constructor.
  UUID();
  
  UUID(std::array<unsigned char, Size> bytes) {
    memcpy(Value, &bytes, Size);
  }
  
  /// Create a new random UUID from entropy (/dev/random).
  static UUID fromRandom() { return UUID(FromRandom); }
  
  /// Create a new pseudorandom UUID using the time, MAC address, and pid.
  static UUID fromTime() { return UUID(FromTime); }
  
  /// Parse a UUID from a C string.
  static std::optional<UUID> fromString(const char *s);

  /// Convert a UUID to its string representation.
  void toString(llvm::SmallVectorImpl<char> &out) const;
  
  int compare(UUID y) const;
  
#define COMPARE_UUID(op) \
  bool operator op(UUID y) { return compare(y) op 0; }
  
  COMPARE_UUID(==)
  COMPARE_UUID(!=)
  COMPARE_UUID(<)
  COMPARE_UUID(<=)
  COMPARE_UUID(>)
  COMPARE_UUID(>=)
#undef COMPARE_UUID
};
  
llvm::raw_ostream &operator<<(llvm::raw_ostream &os, UUID uuid);
  
}

namespace llvm {
  template<>
  struct DenseMapInfo<nextcode::UUID> {
    static inline nextcode::UUID getEmptyKey() {
      return {{{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}}};
    }
    static inline nextcode::UUID getTombstoneKey() {
      return {{{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE}}};
    }
    
    static unsigned getHashValue(nextcode::UUID uuid) {
      union {
        nextcode::UUID uu;
        unsigned words[4];
      } reinterpret = {uuid};
      return reinterpret.words[0] ^ reinterpret.words[1]
           ^ reinterpret.words[2] ^ reinterpret.words[3];
    }
    
    static bool isEqual(nextcode::UUID a, nextcode::UUID b) {
      return a == b;
    }
  };
} // end namespace nextcode

#endif // NEXTCODE_BASIC_UUID_H
