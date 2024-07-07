//===--- PrintNeXTCodeToClangCoreScaffold.h - Print core decls -----*- C++ -*-===//
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
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_PRINTASCLANG_PRINTNEXTCODETOCLANGCORESCAFFOLD_H
#define NEXTCODE_PRINTASCLANG_PRINTNEXTCODETOCLANGCORESCAFFOLD_H

#include "llvm/Support/raw_ostream.h"

namespace nextcode {

class ASTContext;
class PrimitiveTypeMapping;
class NeXTCodeToClangInteropContext;

/// Print out the core declarations required by C/C++ that are part of the core
/// NeXTCode stdlib code.
void printNeXTCodeToClangCoreScaffold(NeXTCodeToClangInteropContext &ctx,
                                   ASTContext &astContext,
                                   PrimitiveTypeMapping &typeMapping,
                                   llvm::raw_ostream &os);

} // end namespace nextcode

#endif
