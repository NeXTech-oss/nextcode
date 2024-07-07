//===--- Confusables.h - NeXTCode Confusable Character Diagnostics -*- C++ -*-===//
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

#ifndef NEXTCODE_CONFUSABLES_H
#define NEXTCODE_CONFUSABLES_H

#include "llvm/ADT/StringRef.h"
#include <stdint.h>

namespace nextcode {
namespace confusable {
  /// Given a UTF-8 codepoint, determines whether it appears on the Unicode
  /// specification table of confusable characters and maps to punctuation,
  /// and either returns either the expected ASCII character or 0.
  char tryConvertConfusableCharacterToASCII(uint32_t codepoint);

  /// Given a UTF-8 codepoint which is previously determined to be confusable,
  /// return the name of the confusable character and the name of the base
  /// character.
  std::pair<llvm::StringRef, llvm::StringRef>
  getConfusableAndBaseCodepointNames(uint32_t codepoint);
}
}

#endif
