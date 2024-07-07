//===--- DiagnosticHelper.h - Diagnostic Helper -----------------*- C++ -*-===//
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
// This file exposes helper class to emit diagnostics from nextcode-frontend.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_FRONTEND_DIAGNOSTIC_HELPER_H
#define NEXTCODE_FRONTEND_DIAGNOSTIC_HELPER_H

#include "nextcode/Basic/LLVM.h"
#include "llvm/Support/raw_ostream.h"

namespace nextcode {
class CompilerInstance;
class CompilerInvocation;

class DiagnosticHelper {
private:
  class Implementation;
  Implementation &Impl;

public:
  /// Create a DiagnosticHelper class to emit diagnostics from frontend actions.
  /// OS is the stream to print diagnostics. useQuasiPID determines if using
  /// real PID when priting parseable output.
  static DiagnosticHelper create(CompilerInstance &instance,
                                 llvm::raw_pwrite_stream &OS = llvm::errs(),
                                 bool useQuasiPID = false);

  /// Initialized all DiagConsumers and add to the CompilerInstance.
  void initDiagConsumers(CompilerInvocation &invocation);

  /// Begin emitting the message, specifically the parseable output message.
  void beginMessage(CompilerInvocation &invocation,
                    ArrayRef<const char *> args);

  /// End emitting all diagnostics. This has to be called if beginMessage() is
  /// called.
  void endMessage(int retCode);

  /// Set if printing output should be suppressed.
  void setSuppressOutput(bool suppressOutput);

  /// Helper function to emit fatal error.
  void diagnoseFatalError(const char *reason, bool shouldCrash);

  DiagnosticHelper(const DiagnosticHelper &) = delete;
  DiagnosticHelper(DiagnosticHelper &&) = delete;
  DiagnosticHelper &operator=(const DiagnosticHelper &) = delete;
  DiagnosticHelper &operator=(DiagnosticHelper &&) = delete;
  ~DiagnosticHelper();

private:
  DiagnosticHelper(CompilerInstance &instance, llvm::raw_pwrite_stream &OS,
                   bool useQuasiPID);
};

} // namespace nextcode

#endif
