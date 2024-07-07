//===--- PrintingDiagnosticConsumer.h - Print Text Diagnostics --*- C++ -*-===//
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
//  This file defines the PrintingDiagnosticConsumer class, which displays
//  diagnostics as text to a terminal.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_PRINTINGDIAGNOSTICCONSUMER_H
#define NEXTCODE_PRINTINGDIAGNOSTICCONSUMER_H

#include "nextcode/AST/DiagnosticConsumer.h"
#include "nextcode/Basic/DiagnosticOptions.h"
#include "nextcode/Basic/LLVM.h"

#include "llvm/ADT/DenseMap.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Process.h"

namespace nextcode {

/// Diagnostic consumer that displays diagnostics to standard error.
class PrintingDiagnosticConsumer : public DiagnosticConsumer {
  llvm::raw_ostream &Stream;
  bool ForceColors = false;
  bool PrintEducationalNotes = false;
  bool EmitMacroExpansionFiles = false;
  bool DidErrorOccur = false;
  DiagnosticOptions::FormattingStyle FormattingStyle =
      DiagnosticOptions::FormattingStyle::LLVM;
  // Educational notes which are buffered until the consumer is finished
  // constructing a snippet.
  SmallVector<std::string, 1> BufferedEducationalNotes;
  bool SuppressOutput = false;

  /// nextcode-syntax rendering

  /// A queued up source file known to the queued diagnostics.
  using QueuedBuffer = void *;

  /// The queued diagnostics structure.
  void *queuedDiagnostics = nullptr;
  llvm::DenseMap<unsigned, QueuedBuffer> queuedBuffers;

  /// Source file syntax nodes cached by { source manager, buffer ID }.
  llvm::DenseMap<std::pair<SourceManager *, unsigned>, void *> sourceFileSyntax;

public:
  PrintingDiagnosticConsumer(llvm::raw_ostream &stream = llvm::errs());
  ~PrintingDiagnosticConsumer();

  virtual void handleDiagnostic(SourceManager &SM,
                                const DiagnosticInfo &Info) override;

  virtual bool finishProcessing() override;

  void flush(bool includeTrailingBreak);

  virtual void flush() override { flush(false); }

  void forceColors() {
    ForceColors = true;
    llvm::sys::Process::UseANSIEscapeCodes(true);
  }

  void setPrintEducationalNotes(bool ShouldPrint) {
    PrintEducationalNotes = ShouldPrint;
  }

  void setFormattingStyle(DiagnosticOptions::FormattingStyle style) {
    FormattingStyle = style;
  }

  void setEmitMacroExpansionFiles(bool ShouldEmit) {
    EmitMacroExpansionFiles = ShouldEmit;
  }

  bool didErrorOccur() {
    return DidErrorOccur;
  }

  void setSuppressOutput(bool suppressOutput) {
    SuppressOutput = suppressOutput;
  }

private:
  /// Retrieve the SourceFileSyntax for the given buffer.
  void *getSourceFileSyntax(SourceManager &SM, unsigned bufferID,
                            StringRef displayName);

  void queueBuffer(SourceManager &sourceMgr, unsigned bufferID);
  void printDiagnostic(SourceManager &SM, const DiagnosticInfo &Info);
};
  
}

#endif
