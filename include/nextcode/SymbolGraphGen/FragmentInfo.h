//===--- FragmentInfo.h - NeXTCode SymbolGraph Declaration Fragment Info -----===//
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

#ifndef NEXTCODE_SYMBOLGRAPHGEN_FRAGMENTINFO_H
#define NEXTCODE_SYMBOLGRAPHGEN_FRAGMENTINFO_H

#include "llvm/ADT/SmallVector.h"
#include "PathComponent.h"

namespace nextcode {
class ValueDecl;

namespace symbolgraphgen {

/// Summary information for a symbol referenced in a symbol graph declaration fragment.
struct FragmentInfo {
  /// The nextcode decl of the referenced symbol.
  const ValueDecl *VD;
  /// The path components of the referenced symbol.
  SmallVector<PathComponent, 4> ParentContexts;
};

} // end namespace symbolgraphgen
} // end namespace nextcode

#endif // NEXTCODE_SYMBOLGRAPHGEN_FRAGMENTINFO_H
