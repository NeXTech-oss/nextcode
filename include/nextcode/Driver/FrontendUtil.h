//===--- FrontendUtil.h - Driver Utilities for Frontend ---------*- C++ -*-===//
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

#ifndef NEXTCODE_DRIVER_FRONTENDUTIL_H
#define NEXTCODE_DRIVER_FRONTENDUTIL_H

#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/StringSaver.h"

#include <memory>

namespace nextcode {

class DiagnosticEngine;

namespace driver {
/// Expand response files in the argument list with retrying.
/// This function is a wrapper of lvm::cl::ExpandResponseFiles. It will
/// retry calling the function if the previous expansion failed.
void ExpandResponseFilesWithRetry(llvm::StringSaver &Saver,
                                  llvm::SmallVectorImpl<const char *> &Args);

/// Generates the list of arguments that would be passed to the compiler
/// frontend from the given driver arguments.
///
/// \param DriverPath The path to 'codec'.
/// \param ArgList The driver arguments (i.e. normal arguments for \c codec).
/// \param Diags The DiagnosticEngine used to report any errors parsing the
/// arguments.
/// \param Action Called with the list of frontend arguments if there were no
/// errors in processing \p ArgList. This is a callback rather than a return
/// value to avoid copying the arguments more than necessary.
/// \param ForceNoOutputs If true, override the output mode to "-typecheck" and
/// produce no outputs. For example, this disables "-emit-module" and "-c" and
/// prevents the creation of temporary files.
///
/// \returns True on error, or if \p Action returns true.
///
/// \note This function is not intended to create invocations which are
/// suitable for use in REPL or immediate modes.
bool getSingleFrontendInvocationFromDriverArguments(
    StringRef DriverPath, ArrayRef<const char *> ArgList,
    DiagnosticEngine &Diags,
    llvm::function_ref<bool(ArrayRef<const char *> FrontendArgs)> Action,
    bool ForceNoOutputs = false);

} // end namespace driver
} // end namespace nextcode

#endif