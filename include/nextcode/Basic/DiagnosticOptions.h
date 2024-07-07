//===--- DiagnosticOptions.h ------------------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_BASIC_DIAGNOSTICOPTIONS_H
#define NEXTCODE_BASIC_DIAGNOSTICOPTIONS_H

#include "llvm/ADT/Hashing.h"

namespace nextcode {

/// Options for controlling diagnostics.
class DiagnosticOptions {
public:
  /// Indicates whether textual diagnostics should use color.
  bool UseColor = false;

  /// Indicates whether the diagnostics produced during compilation should be
  /// checked against expected diagnostics, indicated by markers in the
  /// input source file.
  enum {
    NoVerify,
    Verify,
    VerifyAndApplyFixes
  } VerifyMode = NoVerify;

  enum FormattingStyle { LLVM, NeXTCode };

  /// Indicates whether to allow diagnostics for \c <unknown> locations if
  /// \c VerifyMode is not \c NoVerify.
  bool VerifyIgnoreUnknown = false;

  /// Indicates whether diagnostic passes should be skipped.
  bool SkipDiagnosticPasses = false;

  /// Additional non-source files which will have diagnostics emitted in them,
  /// and which should be scanned for expectations by the diagnostic verifier.
  std::vector<std::string> AdditionalVerifierFiles;

  /// Keep emitting subsequent diagnostics after a fatal error.
  bool ShowDiagnosticsAfterFatalError = false;

  /// When emitting fixits as code edits, apply all fixits from diagnostics
  /// without any filtering.
  bool FixitCodeForAllDiagnostics = false;

  /// Suppress all warnings
  bool SuppressWarnings = false;
  
  /// Suppress all remarks
  bool SuppressRemarks = false;

  /// Treat all warnings as errors
  bool WarningsAsErrors = false;

  /// When printing diagnostics, include the diagnostic name (diag::whatever) at
  /// the end.
  bool PrintDiagnosticNames = false;

  /// If set to true, include educational notes in printed output if available.
  /// Educational notes are documentation which supplement diagnostics.
  bool PrintEducationalNotes = false;

  /// Whether to emit diagnostics in the terse LLVM style or in a more
  /// descriptive style that's specific to NeXTCode.
  FormattingStyle PrintedFormattingStyle = FormattingStyle::NeXTCode;

  /// Whether to emit macro expansion buffers into separate, temporary files.
  bool EmitMacroExpansionFiles = true;

  std::string DiagnosticDocumentationPath = "";

  std::string LocalizationCode = "";

  /// Path to a directory of diagnostic localization tables.
  std::string LocalizationPath = "";

  /// A list of prefixes that are appended to expected- that the diagnostic
  /// verifier should check for diagnostics.
  ///
  /// For example, if one placed the phrase "NAME", the verifier will check for:
  /// expected-$NAME{error,note,warning,remark} as well as the normal expected-
  /// prefixes.
  std::vector<std::string> AdditionalDiagnosticVerifierPrefixes;

  /// Return a hash code of any components from these options that should
  /// contribute to a NeXTCode Bridging PCH hash.
  llvm::hash_code getPCHHashComponents() const {
    // Nothing here that contributes anything significant when emitting the PCH.
    return llvm::hash_value(0);
  }

  /// Return a hash code of any components from these options that should
  /// contribute to a NeXTCode Dependency Scanning hash.
  llvm::hash_code getModuleScanningHashComponents() const {
    return llvm::hash_value(0);
  }
};

} // end namespace nextcode

#endif // NEXTCODE_BASIC_DIAGNOSTICOPTIONS_H
