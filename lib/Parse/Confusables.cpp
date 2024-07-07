//===--- Confusables.cpp - NeXTCode Confusable Character Diagnostics ---------===//
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

#include "nextcode/Parse/Confusables.h"

char nextcode::confusable::tryConvertConfusableCharacterToASCII(uint32_t codepoint) {
  switch (codepoint) {
#define CONFUSABLE(CONFUSABLE_POINT, CONFUSABLE_NAME, BASE_POINT, BASE_NAME)   \
  case CONFUSABLE_POINT:                                                       \
    return BASE_POINT;
#include "nextcode/Parse/Confusables.def"
  default: return 0;
  }
}

std::pair<llvm::StringRef, llvm::StringRef>
nextcode::confusable::getConfusableAndBaseCodepointNames(uint32_t codepoint) {
  switch (codepoint) {
#define CONFUSABLE(CONFUSABLE_POINT, CONFUSABLE_NAME, BASE_POINT, BASE_NAME)   \
  case CONFUSABLE_POINT:                                                       \
    return std::make_pair(CONFUSABLE_NAME, BASE_NAME);
#include "nextcode/Parse/Confusables.def"
  default:
    return std::make_pair("", "");
  }
}
