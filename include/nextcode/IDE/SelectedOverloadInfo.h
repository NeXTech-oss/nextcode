//===--- SelectedOverloadInfo.h -------------------------------------------===//
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

#ifndef NEXTCODE_IDE_NEXTCODECOMPLETIONINFO_H
#define NEXTCODE_IDE_NEXTCODECOMPLETIONINFO_H

#include "nextcode/AST/Decl.h"
#include "nextcode/Sema/ConstraintSystem.h"

namespace nextcode {
namespace ide {

using namespace nextcode::constraints;

/// Information that \c getSelectedOverloadInfo gathered about a
/// \c SelectedOverload.
struct SelectedOverloadInfo {
  /// The function that is being called or the value that is being accessed.
  ConcreteDeclRef ValueRef;
  /// For a function, type of the called function itself (not its result type),
  /// for an arbitrary value the type of that value.
  Type ValueTy;
  /// The type on which the overload is being accessed. \c null if it does not
  /// have a base type, e.g. for a free function.
  Type BaseTy;

  ValueDecl *getValue() const { return ValueRef.getDecl(); }
};

/// Extract additional information about the overload that is being called by
/// \p CalleeLocator.
SelectedOverloadInfo getSelectedOverloadInfo(const Solution &S,
                                             ConstraintLocator *CalleeLocator);

} // end namespace ide
} // end namespace nextcode

#endif // NEXTCODE_IDE_NEXTCODECOMPLETIONINFO_H
