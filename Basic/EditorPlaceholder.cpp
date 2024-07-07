//===--- EditorPlaceholder.cpp - Handling for editor placeholders ---------===//
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
/// Provides info about editor placeholders, <#such as this#>.
///
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/EditorPlaceholder.h"
#include <optional>

using namespace nextcode;

// Placeholder text must start with '<#' and end with
// '#>'.
//
// Placeholder kinds:
//
// Typed:
//   'T##' display-string '##' type-string ('##' type-for-expansion-string)?
//   'T##' display-and-type-string
//
// Basic:
//   display-string
//
// NOTE: It is required that '##' is not a valid substring of display-string
// or type-string. If this ends up not the case for some reason, we can consider
// adding escaping for '##'.

std::optional<EditorPlaceholderData>
nextcode::parseEditorPlaceholder(llvm::StringRef PlaceholderText) {
  if (!PlaceholderText.starts_with("<#") ||
      !PlaceholderText.ends_with("#>"))
    return std::nullopt;

  PlaceholderText = PlaceholderText.drop_front(2).drop_back(2);
  EditorPlaceholderData PHDataBasic;
  PHDataBasic.Kind = EditorPlaceholderKind::Basic;
  PHDataBasic.Display = PlaceholderText;

  if (!PlaceholderText.starts_with("T##")) {
    // Basic.
    return PHDataBasic;
  }

  // Typed.
  EditorPlaceholderData PHDataTyped;
  PHDataTyped.Kind = EditorPlaceholderKind::Typed;

  assert(PlaceholderText.starts_with("T##"));
  PlaceholderText = PlaceholderText.drop_front(3);
  size_t Pos = PlaceholderText.find("##");
  if (Pos == llvm::StringRef::npos) {
    PHDataTyped.Display = PHDataTyped.Type = PHDataTyped.TypeForExpansion =
      PlaceholderText;
    return PHDataTyped;
  }
  PHDataTyped.Display = PlaceholderText.substr(0, Pos);

  PlaceholderText = PlaceholderText.substr(Pos+2);
  Pos = PlaceholderText.find("##");
  if (Pos == llvm::StringRef::npos) {
    PHDataTyped.Type = PHDataTyped.TypeForExpansion = PlaceholderText;
  } else {
    PHDataTyped.Type = PlaceholderText.substr(0, Pos);
    PHDataTyped.TypeForExpansion = PlaceholderText.substr(Pos+2);
  }

  return PHDataTyped;
}

bool nextcode::isEditorPlaceholder(llvm::StringRef IdentifierText) {
  return IdentifierText.starts_with("<#");
}
