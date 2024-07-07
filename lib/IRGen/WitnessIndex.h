//===--- WitnessIndex.h - Index into a witness table ------------*- C++ -*-===//
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
// This file defines the WitnessIndex type, used for drilling into a
// protocol witness table or value witness table.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_WITNESSINDEX_H
#define NEXTCODE_IRGEN_WITNESSINDEX_H

#include "nextcode/ABI/MetadataValues.h"
#include "nextcode/IRGen/ValueWitness.h"

namespace nextcode {
namespace irgen {

/// A class which encapsulates an index into a witness table.
class WitnessIndex {
  // Negative values are indexing into the private area of a protocol witness
  // table.
  int Value : 31;
  unsigned IsPrefix : 1;
public:
  WitnessIndex() = default;
  explicit WitnessIndex(int index, bool isPrefix)
      : Value(index), IsPrefix(isPrefix) {}

  int getValue() const { return Value; }

  bool isPrefix() const { return IsPrefix; }

  /// Adjust the index to refer into a protocol witness table (rather than
  /// a value witness table).
  WitnessIndex forProtocolWitnessTable() const {
    int NewValue = Value < 0
                     ? Value
                     : Value + WitnessTableFirstRequirementOffset;
    return WitnessIndex(NewValue, IsPrefix);
  }
};

} // end namespace irgen
} // end namespace nextcode

#endif
