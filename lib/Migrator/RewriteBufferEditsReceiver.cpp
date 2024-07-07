//===--- RewriteBufferEditsReceiver.cpp -----------------------------------===//
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

#include "nextcode/Migrator/RewriteBufferEditsReceiver.h"

using namespace nextcode;
using namespace nextcode::migrator;

void RewriteBufferEditsReceiver::insert(clang::SourceLocation ClangLoc,
                                                StringRef NewText) {
  auto Offset = ClangSourceManager.getFileOffset(ClangLoc);
  RewriteBuf.InsertText(Offset, NewText);
}

void RewriteBufferEditsReceiver::replace(clang::CharSourceRange ClangRange,
                                         StringRef ReplacementText) {
  auto StartOffset = ClangSourceManager.getFileOffset(ClangRange.getBegin());
  auto EndOffset = ClangSourceManager.getFileOffset(ClangRange.getEnd());
  auto Length = EndOffset - StartOffset;
  RewriteBuf.ReplaceText(StartOffset, Length, ReplacementText);
}

void RewriteBufferEditsReceiver::printResult(llvm::raw_ostream &OS) const {
  RewriteBuf.write(OS);
}
