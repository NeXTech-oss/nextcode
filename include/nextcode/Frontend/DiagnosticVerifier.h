//===--- DiagnosticVerifier.h - Diagnostic Verifier (-verify) ---*- C++ -*-===//
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
// This file exposes support for the diagnostic verifier, which is used to
// implement -verify mode in the compiler.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_FRONTEND_DIAGNOSTIC_VERIFIER_H
#define NEXTCODE_FRONTEND_DIAGNOSTIC_VERIFIER_H

#include "llvm/ADT/SmallString.h"
#include "nextcode/AST/DiagnosticConsumer.h"
#include "nextcode/Basic/LLVM.h"

namespace nextcode {
class DependencyTracker;
class FileUnit;
class SourceManager;
class SourceFile;

// MARK: - DependencyVerifier

bool verifyDependencies(SourceManager &SM, ArrayRef<FileUnit *> SFs);
bool verifyDependencies(SourceManager &SM, ArrayRef<SourceFile *> SFs);

// MARK: - DiagnosticVerifier
struct ExpectedFixIt;

/// A range expressed in terms of line-and-column pairs.
struct LineColumnRange {
  unsigned StartLine, StartCol;
  unsigned EndLine, EndCol;

  LineColumnRange() : StartLine(0), StartCol(0), EndLine(0), EndCol(0) {}
};

class CapturedFixItInfo final {
  DiagnosticInfo::FixIt FixIt;
  mutable LineColumnRange LineColRange;

public:
  CapturedFixItInfo(DiagnosticInfo::FixIt FixIt) : FixIt(FixIt) {}

  CharSourceRange &getSourceRange() { return FixIt.getRange(); }
  const CharSourceRange &getSourceRange() const { return FixIt.getRange(); }

  StringRef getText() const { return FixIt.getText(); }

  /// Obtain the line-column range corresponding to the fix-it's
  /// replacement range.
  const LineColumnRange &getLineColumnRange(const SourceManager &SM,
                                            unsigned BufferID) const;
};

struct CapturedDiagnosticInfo {
  llvm::SmallString<128> Message;
  llvm::SmallString<32> FileName;
  DiagnosticKind Classification;
  SourceLoc Loc;
  unsigned Line;
  unsigned Column;
  SmallVector<CapturedFixItInfo, 2> FixIts;
  SmallVector<std::string, 1> EducationalNotes;

  CapturedDiagnosticInfo(llvm::SmallString<128> Message,
                         llvm::SmallString<32> FileName,
                         DiagnosticKind Classification, SourceLoc Loc,
                         unsigned Line, unsigned Column,
                         SmallVector<CapturedFixItInfo, 2> FixIts,
                         SmallVector<std::string, 1> EducationalNotes)
      : Message(Message), FileName(FileName), Classification(Classification),
        Loc(Loc), Line(Line), Column(Column), FixIts(FixIts),
        EducationalNotes(EducationalNotes) {
    std::sort(EducationalNotes.begin(), EducationalNotes.end());
  }
};
/// This class implements support for -verify mode in the compiler.  It
/// buffers up diagnostics produced during compilation, then checks them
/// against expected-error markers in the source file.
class DiagnosticVerifier : public DiagnosticConsumer {
  SourceManager &SM;
  std::vector<CapturedDiagnosticInfo> CapturedDiagnostics;
  ArrayRef<unsigned> BufferIDs;
  SmallVector<unsigned, 4> AdditionalBufferIDs;
  bool AutoApplyFixes;
  bool IgnoreUnknown;
  bool UseColor;
  ArrayRef<std::string> AdditionalExpectedPrefixes;

public:
  explicit DiagnosticVerifier(SourceManager &SM, ArrayRef<unsigned> BufferIDs,
                              bool AutoApplyFixes, bool IgnoreUnknown,
                              bool UseColor,
                              ArrayRef<std::string> AdditionalExpectedPrefixes)
      : SM(SM), BufferIDs(BufferIDs), AutoApplyFixes(AutoApplyFixes),
        IgnoreUnknown(IgnoreUnknown), UseColor(UseColor),
        AdditionalExpectedPrefixes(AdditionalExpectedPrefixes) {}

  void appendAdditionalBufferID(unsigned bufferID) {
    AdditionalBufferIDs.push_back(bufferID);
  }

  virtual void handleDiagnostic(SourceManager &SM,
                                const DiagnosticInfo &Info) override;

  virtual bool finishProcessing() override;

private:
  /// Result of verifying a file.
  struct Result {
    /// Were there any errors? All of the following are considered errors:
    /// - Expected diagnostics that were not present
    /// - Unexpected diagnostics that were present
    /// - Errors in the definition of expected diagnostics
    bool HadError;
    bool HadUnexpectedDiag;
  };

  void printDiagnostic(const llvm::SMDiagnostic &Diag) const;

  bool
  verifyUnknown(std::vector<CapturedDiagnosticInfo> &CapturedDiagnostics) const;

  /// verifyFile - After the file has been processed, check to see if we
  /// got all of the expected diagnostics and check to see if there were any
  /// unexpected ones.
  Result verifyFile(unsigned BufferID);

  bool checkForFixIt(const std::vector<ExpectedFixIt> &ExpectedAlts,
                     const CapturedDiagnosticInfo &D, unsigned BufferID) const;

  // Render the verifier syntax for a given set of fix-its.
  std::string renderFixits(ArrayRef<CapturedFixItInfo> ActualFixIts,
                           unsigned BufferID, unsigned DiagnosticLineNo) const;

  void printRemainingDiagnostics() const;
};

} // end namespace nextcode

#endif
