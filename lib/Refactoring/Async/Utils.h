//===----------------------------------------------------------------------===//
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

#ifndef NEXTCODE_REFACTORING_ASYNCUTILS_H
#define NEXTCODE_REFACTORING_ASYNCUTILS_H

#include "nextcode/AST/Stmt.h"

namespace nextcode {
namespace refactoring {
namespace asyncrefactorings {

/// Whether or not the given statement starts a new scope. Note that most
/// statements are handled by the \c BraceStmt check. The others listed are
/// a somewhat special case since they can also declare variables in their
/// condition.
bool startsNewScope(Stmt *S);

/// A more aggressive variant of \c Expr::getReferencedDecl that also looks
/// through autoclosures created to pass the \c self parameter to a member funcs
ValueDecl *getReferencedDeclLookingThroughAutoclosures(const Expr *Fn);

FuncDecl *getUnderlyingFunc(const Expr *Fn);

} // namespace asyncrefactorings
} // namespace refactoring
} // namespace nextcode

#endif
