//===--- SymbolGraphPathComponent.h - NeXTCode SymbolGraph Path Component ----===//
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

#ifndef NEXTCODE_SYMBOLGRAPHGEN_PATHCOMPONENT_H
#define NEXTCODE_SYMBOLGRAPHGEN_PATHCOMPONENT_H

#include "llvm/ADT/SmallString.h"

namespace nextcode {
class ValueDecl;

namespace symbolgraphgen {

/// Summary information for a node along a path through a symbol graph.
struct PathComponent {
  /// The title of the corresponding symbol graph node.
  SmallString<32> Title;
  /// The kind of the corresponding symbol graph node.
  StringRef Kind;
  /// The nextcode decl associated with the corresponding symbol graph node.
  const ValueDecl *VD;
};

} // end namespace symbolgraphgen
} // end namespace nextcode

#endif // NEXTCODE_SYMBOLGRAPHGEN_PATHCOMPONENT_H
