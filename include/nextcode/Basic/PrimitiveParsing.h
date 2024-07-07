//===--- PrimitiveParsing.h - Primitive parsing routines --------*- C++ -*-===//
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
///
/// \file
/// Primitive parsing routines useful in various places in the compiler.
///
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_PRIMITIVEPARSING_H
#define NEXTCODE_BASIC_PRIMITIVEPARSING_H

#include "llvm/ADT/StringRef.h"
#include "nextcode/Basic/LLVM.h"

namespace nextcode {

unsigned measureNewline(const char *BufferPtr, const char *BufferEnd);

static inline unsigned measureNewline(StringRef S) {
  return measureNewline(S.data(), S.data() + S.size());
}

static inline bool startsWithNewline(StringRef S) {
  return S.starts_with("\n") || S.starts_with("\r\n");
}

/// Breaks a given string to lines and trims leading whitespace from them.
void trimLeadingWhitespaceFromLines(StringRef Text, unsigned WhitespaceToTrim,
                                    SmallVectorImpl<StringRef> &Lines);

static inline void splitIntoLines(StringRef Text,
                                  SmallVectorImpl<StringRef> &Lines) {
  trimLeadingWhitespaceFromLines(Text, 0, Lines);
}

} // end namespace nextcode

#endif // NEXTCODE_BASIC_PRIMITIVEPARSING_H

