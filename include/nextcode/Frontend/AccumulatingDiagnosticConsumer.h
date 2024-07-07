//===- AccumulatingDiagnosticConsumer.h - Collect Text Diagnostics  C++ -*-===//
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
//  This file defines the AccumulatingDiagnosticConsumer class, which collects
//  all emitted diagnostics into an externally-owned collection.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_ACCUMULATINGDIAGNOSTICCONSUMER_H
#define NEXTCODE_ACCUMULATINGDIAGNOSTICCONSUMER_H

#include "nextcode/AST/DiagnosticConsumer.h"
#include "nextcode/Basic/DiagnosticOptions.h"
#include "nextcode/Basic/LLVM.h"

#include <string>
#include <sstream>

namespace nextcode {
/// Diagnostic consumer that simply collects all emitted diagnostics into the provided
/// collection.
class AccumulatingFileDiagnosticConsumer : public DiagnosticConsumer {
  std::vector<std::string> &Diagnostics;

public:
  AccumulatingFileDiagnosticConsumer(std::vector<std::string> &DiagBuffer)
    : Diagnostics(DiagBuffer) {}

private:
  void handleDiagnostic(SourceManager &SM,
                        const DiagnosticInfo &Info) override {
    addDiagnostic(SM, Info);

    for (auto ChildInfo : Info.ChildDiagnosticInfo) {
      addDiagnostic(SM, *ChildInfo);
    }
  }

  // TODO: Support NeXTCode-style diagnostic formatting
  void addDiagnostic(SourceManager &SM, const DiagnosticInfo &Info) {
    // Determine what kind of diagnostic we're emitting.
    llvm::SourceMgr::DiagKind SMKind;
    switch (Info.Kind) {
    case DiagnosticKind::Error:
      SMKind = llvm::SourceMgr::DK_Error;
      break;
    case DiagnosticKind::Warning:
      SMKind = llvm::SourceMgr::DK_Warning;
      break;

    case DiagnosticKind::Note:
      SMKind = llvm::SourceMgr::DK_Note;
      break;

    case DiagnosticKind::Remark:
      SMKind = llvm::SourceMgr::DK_Remark;
      break;
    }

    // Translate ranges.
    SmallVector<llvm::SMRange, 2> Ranges;
    for (auto R : Info.Ranges)
      Ranges.push_back(getRawRange(SM, R));

    // Translate fix-its.
    SmallVector<llvm::SMFixIt, 2> FixIts;
    for (DiagnosticInfo::FixIt F : Info.FixIts)
      FixIts.push_back(getRawFixIt(SM, F));

    // Actually substitute the diagnostic arguments into the diagnostic text.
    llvm::SmallString<256> Text;
    {
      llvm::raw_svector_ostream Out(Text);
      DiagnosticEngine::formatDiagnosticText(Out, Info.FormatString,
                                             Info.FormatArgs);
    }

    const llvm::SourceMgr &rawSM = SM.getLLVMSourceMgr();
    auto Msg = SM.GetMessage(Info.Loc, SMKind, Text, Ranges, FixIts);
    std::string result;
    llvm::raw_string_ostream os(result);
    rawSM.PrintMessage(os, Msg, false);
    os.flush();
    Diagnostics.push_back(result);
  }
};
}
#endif
