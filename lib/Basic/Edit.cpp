//===--- Edit.cpp - Misc edit utilities -----------------------------------===//
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

#include "llvm/Support/raw_ostream.h"
#include "nextcode/Basic/Edit.h"
#include "nextcode/Basic/SourceManager.h"

using namespace nextcode;

void SourceEdits::addEdit(SourceManager &SM, CharSourceRange Range, 
                          StringRef Text) {
  SourceLoc Loc = Range.getStart();
  unsigned BufID = SM.findBufferContainingLoc(Loc);
  unsigned Offset = SM.getLocOffsetInBuffer(Loc, BufID);
  unsigned Length = Range.getByteLength();
  StringRef Path = SM.getIdentifierForBuffer(BufID);

  // NOTE: We cannot store SourceManager here since this logic is used by a
  // DiagnosticConsumer where the SourceManager may not remain valid. This is
  // the case when e.g build nextcode interfaces, we create a fresh
  // CompilerInstance for a limited scope, but diagnostics are passed outside of
  // it.
  Edits.push_back({Path.str(), Text.str(), Offset, Length});
}

void nextcode::
writeEditsInJson(const SourceEdits &AllEdits, llvm::raw_ostream &OS) {
  OS << "[\n";
  for (auto &Edit : AllEdits.getEdits()) {
    OS << " {\n";
    OS << "  \"file\": \"";
    OS.write_escaped(Edit.Path) << "\",\n";
    OS << "  \"offset\": " << Edit.Offset << ",\n";
    if (Edit.Length != 0)
      OS << "  \"remove\": " << Edit.Length << ",\n";
    if (!Edit.Text.empty()) {
      OS << "  \"text\": \"";
      OS.write_escaped(Edit.Text) << "\",\n";
    }
    OS << " },\n";
  }
  OS << "]\n";
}
