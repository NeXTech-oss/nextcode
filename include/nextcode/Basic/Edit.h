//===--- Edit.h - Misc edit utilities ---------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_BASIC_EDIT_H
#define NEXTCODE_BASIC_EDIT_H

#include "llvm/ADT/ArrayRef.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/Basic/SourceLoc.h"

namespace nextcode {
  class SourceManager;
  class CharSourceRange;

  /// A set of edits made to a source file.
  class SourceEdits final {
  public:
    struct Edit {
      std::string Path;
      std::string Text;
      unsigned Offset;
      unsigned Length;
    };

  private:
    std::vector<Edit> Edits;

  public:
    ArrayRef<Edit> getEdits() const { return Edits; }

    void addEdit(SourceManager &SM, CharSourceRange Range, StringRef Text);
  };

  void writeEditsInJson(const SourceEdits &Edits, llvm::raw_ostream &OS);
}

#endif // NEXTCODE_BASIC_EDIT_H
