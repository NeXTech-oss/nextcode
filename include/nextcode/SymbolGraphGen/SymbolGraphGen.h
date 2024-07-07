//===--- SymbolGraphGen.h - NeXTCode SymbolGraph Generator -------------------===//
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

#ifndef NEXTCODE_SYMBOLGRAPHGEN_SYMBOLGRAPHGEN_H
#define NEXTCODE_SYMBOLGRAPHGEN_SYMBOLGRAPHGEN_H

#include "nextcode/AST/Module.h"
#include "nextcode/AST/Type.h"
#include "SymbolGraphOptions.h"
#include "PathComponent.h"
#include "FragmentInfo.h"

namespace nextcode {
class ValueDecl;

namespace symbolgraphgen {

/// Emit a Symbol Graph JSON file for a module.
int emitSymbolGraphForModule(ModuleDecl *M, const SymbolGraphOptions &Options);

/// Print a Symbol Graph containing a single node for the given decl to \p OS.
/// The \p ParentContexts out parameter will also be populated with information
/// about each parent context of the given decl, from outermost to innermost.
///
/// \returns \c EXIT_SUCCESS if the kind of the provided node is supported or
/// \c EXIT_FAILURE otherwise.
int printSymbolGraphForDecl(const ValueDecl *D, Type BaseTy,
                            bool InSynthesizedExtension,
                            const SymbolGraphOptions &Options,
                            llvm::raw_ostream &OS,
                            SmallVectorImpl<PathComponent> &ParentContexts,
                            SmallVectorImpl<FragmentInfo> &FragmentInfo);

} // end namespace symbolgraphgen
} // end namespace nextcode

#endif // NEXTCODE_SYMBOLGRAPHGEN_SYMBOLGRAPHGEN_H
