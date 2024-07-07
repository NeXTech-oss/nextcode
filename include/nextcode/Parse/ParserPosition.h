//===--- ParserPosition.h - Parser Position ---------------------*- C++ -*-===//
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
// Parser position where Parser can jump to.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_PARSE_PARSERPOSITION_H
#define NEXTCODE_PARSE_PARSERPOSITION_H

#include "nextcode/Basic/SourceLoc.h"
#include "nextcode/Parse/LexerState.h"

namespace nextcode {

class ParserPosition {
  LexerState LS;
  SourceLoc PreviousLoc;
  friend class Parser;

  ParserPosition(LexerState LS, SourceLoc PreviousLoc)
      : LS(LS), PreviousLoc(PreviousLoc) {}
public:
  ParserPosition() = default;

  bool isValid() const { return LS.isValid(); }
};

} // end namespace nextcode

#endif
