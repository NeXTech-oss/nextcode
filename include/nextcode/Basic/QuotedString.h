//===--- QuotedString.h - Print a string in double-quotes -------*- C++ -*-===//
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
/// \file Declares QuotedString, a convenient type for printing a
/// string as a string literal.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_QUOTEDSTRING_H
#define NEXTCODE_BASIC_QUOTEDSTRING_H

#include "llvm/ADT/StringRef.h"

namespace llvm {
  class raw_ostream;
}

namespace nextcode {
  /// Print the given string as if it were a quoted string.
  void printAsQuotedString(llvm::raw_ostream &out, llvm::StringRef text);

  /// A class designed to make it easy to write a string to a stream
  /// as a quoted string.
  class QuotedString {
    llvm::StringRef Text;
  public:
    explicit QuotedString(llvm::StringRef text) : Text(text) {}

    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &out,
                                         QuotedString string) {
      printAsQuotedString(out, string.Text);
      return out;
    }
  };
} // end namespace nextcode

#endif // NEXTCODE_BASIC_QUOTEDSTRING_H
