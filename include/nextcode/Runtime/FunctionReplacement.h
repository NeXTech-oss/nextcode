//===--- FunctionReplacement.h --------------------------------------------===//
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

#ifndef NEXTCODE_RUNTIME_FUNCTIONREPLACEMENT_H
#define NEXTCODE_RUNTIME_FUNCTIONREPLACEMENT_H

#include "nextcode/Runtime/Config.h"

namespace nextcode {

/// Loads the replacement function pointer from \p ReplFnPtr and returns the
/// replacement function if it should be called.
/// Returns null if the original function (which is passed in \p CurrFn) should
/// be called.
#ifdef __APPLE__
__attribute__((weak_import))
#endif
NEXTCODE_RUNTIME_EXPORT char *
nextcode_getFunctionReplacement(char **ReplFnPtr, char *CurrFn);

/// Returns the original function of a replaced function, which is loaded from
/// \p OrigFnPtr.
/// This function is called from a replacement function to call the original
/// function.
#ifdef __APPLE__
__attribute__((weak_import))
#endif
NEXTCODE_RUNTIME_EXPORT char *
nextcode_getOrigOfReplaceable(char **OrigFnPtr);

} // namespace nextcode

#endif
