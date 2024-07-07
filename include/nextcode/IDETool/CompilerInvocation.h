//===--- CompilerInvocation.h ---------------------------------------------===//
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

#ifndef NEXTCODE_IDE_COMPILERINVOCATION_H
#define NEXTCODE_IDE_COMPILERINVOCATION_H

#include "nextcode/Frontend/Frontend.h"

namespace nextcode {

class CompilerInvocation;

namespace ide {

bool initCompilerInvocation(
    CompilerInvocation &Invocation, ArrayRef<const char *> OrigArgs,
    FrontendOptions::ActionType Action, DiagnosticEngine &Diags,
    StringRef UnresolvedPrimaryFile,
    llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem> FileSystem,
    const std::string &nextcodeExecutablePath,
    const std::string &runtimeResourcePath,
    const std::string &diagnosticDocumentationPath, time_t sessionTimestamp,
    std::string &Error);

bool initInvocationByClangArguments(ArrayRef<const char *> ArgList,
                                    CompilerInvocation &Invok,
                                    std::string &Error);

} // namespace ide
} // namespace nextcode

#endif // NEXTCODE_IDE_COMPILERINVOCATION_H
