//===--- BCReadingExtras.h - Useful helpers for bitcode reading -*- C++ -*-===//
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

#ifndef NEXTCODE_SERIALIZATION_BCREADINGEXTRAS_H
#define NEXTCODE_SERIALIZATION_BCREADINGEXTRAS_H

#include "llvm/Bitstream/BitstreamReader.h"

namespace nextcode {
namespace serialization {

/// Saves and restores a BitstreamCursor's bit offset in its stream.
class BCOffsetRAII {
  llvm::BitstreamCursor *Cursor;
  decltype(Cursor->GetCurrentBitNo()) Offset;

public:
  explicit BCOffsetRAII(llvm::BitstreamCursor &cursor)
    : Cursor(&cursor), Offset(cursor.GetCurrentBitNo()) {}

  void reset() {
    if (Cursor)
      Offset = Cursor->GetCurrentBitNo();
  }

  void cancel() {
    Cursor = nullptr;
  }

  ~BCOffsetRAII() {
    if (Cursor)
      cantFail(Cursor->JumpToBit(Offset),
               "BCOffsetRAII must be able to go back");
  }
};

} // end namespace serialization
} // end namespace nextcode

static constexpr const auto AF_DontPopBlockAtEnd =
  llvm::BitstreamCursor::AF_DontPopBlockAtEnd;

#endif
