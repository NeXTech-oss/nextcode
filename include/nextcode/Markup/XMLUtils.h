//===--- XMLUtils.h - Various XML utility routines --------------*- C++ -*-===//
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

#ifndef NEXTCODE_MARKUP_XML_UTILS_H
#define NEXTCODE_MARKUP_XML_UTILS_H

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"

namespace nextcode {
namespace markup {

// FIXME: copied from Clang's
// CommentASTToXMLConverter::appendToResultWithXMLEscaping
static inline void appendWithXMLEscaping(raw_ostream &OS, StringRef S) {
  auto Start = S.begin(), Cursor = Start, End = S.end();
  for (; Cursor != End; ++Cursor) {
    switch (*Cursor) {
    case '&':
      OS.write(Start, Cursor - Start);
      OS << "&amp;";
      break;
    case '<':
      OS.write(Start, Cursor - Start);
      OS << "&lt;";
      break;
    case '>':
      OS.write(Start, Cursor - Start);
      OS << "&gt;";
      break;
    case '"':
      OS.write(Start, Cursor - Start);
      OS << "&quot;";
      break;
    case '\'':
      OS.write(Start, Cursor - Start);
      OS << "&apos;";
      break;
    default:
      continue;
    }
    Start = Cursor + 1;
  }
  OS.write(Start, Cursor - Start);
}

// FIXME: copied from Clang's
// CommentASTToXMLConverter::appendToResultWithCDATAEscaping
static inline void appendWithCDATAEscaping(raw_ostream &OS, StringRef S) {
  if (S.empty())
    return;

  OS << "<![CDATA[";
  while (!S.empty()) {
    size_t Pos = S.find("]]>");
    if (Pos == 0) {
      OS << "]]]]><![CDATA[>";
      S = S.drop_front(3);
      continue;
    }
    if (Pos == StringRef::npos)
      Pos = S.size();

    OS << S.substr(0, Pos);

    S = S.drop_front(Pos);
  }
  OS << "]]>";
}

} // namespace markup
} // namespace nextcode

#endif // NEXTCODE_MARKUP_XML_UTILS_H

