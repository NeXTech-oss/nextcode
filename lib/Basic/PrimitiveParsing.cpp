//===--- PrimitiveParsing.cpp - Primitive parsing routines ----------------===//
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
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Primitive parsing routines useful in various places in the compiler.
///
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/PrimitiveParsing.h"
#include "llvm/ADT/SmallVector.h"

using namespace llvm;

unsigned nextcode::measureNewline(const char *BufferPtr, const char *BufferEnd) {
  if (BufferPtr == BufferEnd)
    return 0;

  if (*BufferPtr == '\n')
    return 1;

  assert(*BufferPtr == '\r');
  unsigned Bytes = 1;
  if (BufferPtr != BufferEnd && *BufferPtr == '\n')
    ++Bytes;
  return Bytes;
}

void
nextcode::trimLeadingWhitespaceFromLines(StringRef RawText,
                                      unsigned WhitespaceToTrim,
                                      SmallVectorImpl<StringRef> &OutLines) {
  SmallVector<StringRef, 8> Lines;

  bool IsFirstLine = true;

  while (!RawText.empty()) {
    size_t Pos = RawText.find_first_of("\n\r");
    if (Pos == StringRef::npos)
      Pos = RawText.size();

    StringRef Line = RawText.substr(0, Pos);
    Lines.push_back(Line);
    if (!IsFirstLine) {
      size_t NonWhitespacePos = RawText.find_first_not_of(' ');
      if (NonWhitespacePos != StringRef::npos)
        WhitespaceToTrim =
            std::min(WhitespaceToTrim,
                     static_cast<unsigned>(NonWhitespacePos));
    }
    IsFirstLine = false;

    RawText = RawText.drop_front(Pos);
    unsigned NewlineBytes = measureNewline(RawText);
    RawText = RawText.drop_front(NewlineBytes);
  }

  IsFirstLine = true;
  for (auto &Line : Lines) {
    if (!IsFirstLine) {
      Line = Line.drop_front(WhitespaceToTrim);
    }
    IsFirstLine = false;
  }

  OutLines.append(Lines.begin(), Lines.end());
}
