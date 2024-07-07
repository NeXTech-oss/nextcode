//===--- EditorPlaceholder.h - Handling for editor placeholders -*- C++ -*-===//
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
/// Provides info about editor placeholders, <#such as this#>.
///
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_EDITORPLACEHOLDER_H
#define NEXTCODE_BASIC_EDITORPLACEHOLDER_H

#include "llvm/ADT/StringRef.h"
#include <optional>

namespace nextcode {

enum class EditorPlaceholderKind {
  Basic,
  Typed,
};

struct EditorPlaceholderData {
  /// Placeholder kind.
  EditorPlaceholderKind Kind;
  /// The part that is displayed in the editor.
  llvm::StringRef Display;
  /// If kind is \c Typed, this is the type string for the placeholder.
  llvm::StringRef Type;
  /// If kind is \c Typed, this is the type string to be considered for
  /// placeholder expansion.
  /// It can be same as \c Type or different if \c Type is a typealias.
  llvm::StringRef TypeForExpansion;
};

/// Deconstructs a placeholder string and returns info about it.
/// \returns None if the \c PlaceholderText is not a valid placeholder string.
std::optional<EditorPlaceholderData>
parseEditorPlaceholder(llvm::StringRef PlaceholderText);

/// Checks if an identifier with the given text is an editor placeholder
bool isEditorPlaceholder(llvm::StringRef IdentifierText);
} // end namespace nextcode

#endif // NEXTCODE_BASIC_EDITORPLACEHOLDER_H
