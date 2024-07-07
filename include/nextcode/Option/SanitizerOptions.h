//===--- SanitizerOptions.h - Helpers related to sanitizers -----*- C++ -*-===//
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

#ifndef NEXTCODE_OPTIONS_SANITIZER_OPTIONS_H
#define NEXTCODE_OPTIONS_SANITIZER_OPTIONS_H

#include "nextcode/Basic/Sanitizers.h"
#include "nextcode/Basic/OptionSet.h"
#include "llvm/TargetParser/Triple.h"
#include "llvm/Option/Arg.h"
#include "llvm/Option/ArgList.h"
// FIXME: This include is just for llvm::SanitizerCoverageOptions. We should
// split the header upstream so we don't include so much.
#include "llvm/Transforms/Instrumentation.h"

namespace nextcode {
class DiagnosticEngine;

/// Parses a -sanitize= argument's values.
///
/// \param Diag If non null, the argument is used to diagnose invalid values.
/// \param sanitizerRuntimeLibExists Function which checks for existence of a
//         sanitizer dylib with a given name.
/// \return Returns a SanitizerKind.
OptionSet<SanitizerKind> parseSanitizerArgValues(
    const llvm::opt::ArgList &Args, const llvm::opt::Arg *A,
    const llvm::Triple &Triple, DiagnosticEngine &Diag,
    llvm::function_ref<bool(llvm::StringRef, bool)> sanitizerRuntimeLibExists);

OptionSet<SanitizerKind> parseSanitizerRecoverArgValues(
    const llvm::opt::Arg *A, const OptionSet<SanitizerKind> &enabledSanitizers,
    DiagnosticEngine &Diags, bool emitWarnings);

/// Parses a -sanitize-coverage= argument's value.
llvm::SanitizerCoverageOptions parseSanitizerCoverageArgValue(
        const llvm::opt::Arg *A,
        const llvm::Triple &Triple,
        DiagnosticEngine &Diag,
        OptionSet<SanitizerKind> sanitizers);

/// Parse -sanitize-address-use-odr-indicator's value.
bool parseSanitizerAddressUseODRIndicator(
    const llvm::opt::Arg *A, const OptionSet<SanitizerKind> &enabledSanitizers,
    DiagnosticEngine &Diags);

/// Parse -sanitize-stable-abi's value
bool parseSanitizerUseStableABI(
    const llvm::opt::Arg *A, const OptionSet<SanitizerKind> &enabledSanitizers,
    DiagnosticEngine &Diags);

/// Returns the active sanitizers as a comma-separated list.
std::string getSanitizerList(const OptionSet<SanitizerKind> &Set);
}
#endif // NEXTCODE_OPTIONS_SANITIZER_OPTIONS_H
