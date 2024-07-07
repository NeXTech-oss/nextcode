//===--- LexerState.h - Lexer State -----------------------------*- C++ -*-===//
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
//  This file defines the LexerState object.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_LEXERSTATE_H
#define NEXTCODE_LEXERSTATE_H

#include "nextcode/Basic/SourceLoc.h"
#include <optional>

namespace nextcode {
class Lexer;

/// Lexer state can be saved/restored to/from objects of this class.
class LexerState {
public:
  LexerState() {}

  bool isValid() const { return Loc.isValid(); }

  LexerState advance(unsigned Offset) const {
    assert(isValid());
    return LexerState(Loc.getAdvancedLoc(Offset));
  }

private:
  explicit LexerState(SourceLoc Loc) : Loc(Loc) {}
  SourceLoc Loc;
  friend class Lexer;
};

} // end namespace nextcode

#endif
