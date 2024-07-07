//===--- PrintAsClang.h - Emit a header file for a NeXTCode AST ----*- C++ -*-===//
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

#ifndef NEXTCODE_PRINTASCLANG_H
#define NEXTCODE_PRINTASCLANG_H

#include "nextcode/Basic/LLVM.h"
#include "nextcode/AST/AttrKind.h"
#include "nextcode/AST/Identifier.h"

namespace clang {
class HeaderSearch;
}

namespace nextcode {
class FrontendOptions;
class IRGenOptions;
class ModuleDecl;
class ValueDecl;

/// Print the exposed declarations in a module into a Clang header.
///
/// The Objective-C compatible declarations are printed into a block that
/// ensures that those declarations are only usable when the header is
/// compiled in Objective-C mode.
/// The C++ compatible declarations are printed into a block that ensures
/// that those declarations are only usable when the header is compiled in
/// C++ mode.
///
/// Returns true on error.
bool printAsClangHeader(raw_ostream &out, ModuleDecl *M,
                        StringRef bridgingHeader,
                        const FrontendOptions &frontendOpts,
                        const IRGenOptions &irGenOpts,
                        clang::HeaderSearch &headerSearchInfo);
}

#endif
