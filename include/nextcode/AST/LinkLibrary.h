//===--- LinkLibrary.h - A module-level linker dependency -------*- C++ -*-===//
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

#ifndef NEXTCODE_AST_LINKLIBRARY_H
#define NEXTCODE_AST_LINKLIBRARY_H

#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/StringRef.h"
#include <string>

namespace nextcode {

// Must be kept in sync with diag::error_immediate_mode_missing_library.
enum class LibraryKind {
  Library = 0,
  Framework
};

/// Represents a linker dependency for an imported module.
// FIXME: This is basically a slightly more generic version of Clang's
// Module::LinkLibrary.
class LinkLibrary {
private:
  std::string Name;
  unsigned Kind : 1;
  unsigned ForceLoad : 1;

public:
  LinkLibrary(StringRef N, LibraryKind K, bool forceLoad = false)
    : Name(N), Kind(static_cast<unsigned>(K)), ForceLoad(forceLoad) {
    assert(getKind() == K && "not enough bits for the kind");
  }

  LibraryKind getKind() const { return static_cast<LibraryKind>(Kind); }
  StringRef getName() const { return Name; }
  bool shouldForceLoad() const { return ForceLoad; }
};

} // end namespace nextcode

#endif
