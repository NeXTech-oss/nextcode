//===--- FixitApplyDiagnosticConsumer.cpp ---------------------------------===//
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

#include "nextcode/Basic/SourceManager.h"
#include "nextcode/Frontend/Frontend.h"
#include "nextcode/Migrator/FixitApplyDiagnosticConsumer.h"
#include "nextcode/Migrator/Migrator.h"
#include "nextcode/Migrator/MigratorOptions.h"
#include "nextcode/AST/DiagnosticsSema.h"

using namespace nextcode;
using namespace nextcode::migrator;

FixitApplyDiagnosticConsumer::
FixitApplyDiagnosticConsumer(const StringRef Text,
                             const StringRef BufferName)
  : Text(Text), BufferName(BufferName), NumFixitsApplied(0) {
  RewriteBuf.Initialize(Text);
}

void FixitApplyDiagnosticConsumer::printResult(llvm::raw_ostream &OS) const {
  RewriteBuf.write(OS);
}

void FixitApplyDiagnosticConsumer::handleDiagnostic(
    SourceManager &SM, const DiagnosticInfo &Info) {
  if (Info.Loc.isInvalid()) {
    return;
  }
  auto ThisBufferID = SM.findBufferContainingLoc(Info.Loc);
  auto ThisBufferName = SM.getIdentifierForBuffer(ThisBufferID);
  if (ThisBufferName != BufferName) {
    return;
  }

  if (!shouldTakeFixit(Info)) {
    return;
  }

  for (const auto &Fixit : Info.FixIts) {

    auto Offset = SM.getLocOffsetInBuffer(Fixit.getRange().getStart(),
                                          ThisBufferID);
    auto Length = Fixit.getRange().getByteLength();
    auto Text = Fixit.getText();

    // Ignore meaningless Fix-its.
    if (Length == 0 && Text.size() == 0)
      continue;

    // Ignore pre-applied equivalents.
    Replacement R{Offset, Length, Text.str()};
    if (Replacements.count(R)) {
      continue;
    } else {
      Replacements.insert(R);
    }

    if (Length == 0) {
      RewriteBuf.InsertText(Offset, Text);
    } else if (Text.size() == 0) {
      RewriteBuf.RemoveText(Offset, Length);
    } else {
      RewriteBuf.ReplaceText(Offset, Length, Text);
    }
    ++NumFixitsApplied;
  }
}
