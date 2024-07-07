//===--- CompileJobCacheKey.h - compile cache key methods -------*- C++ -*-===//
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
//
// This file contains declarations of utility methods for creating cache keys
// for compilation jobs.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_COMPILEJOBCACHEKEY_H
#define NEXTCODE_COMPILEJOBCACHEKEY_H

#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/Basic/FileTypes.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/CAS/ObjectStore.h"
#include "llvm/Support/Error.h"

namespace nextcode {

/// Compute CompileJobBaseKey from nextcode-frontend command-line arguments.
/// CompileJobBaseKey represents the core inputs and arguments, and is used as a
/// base to compute keys for each compiler outputs.
// TODO: switch to create key from CompilerInvocation after we can canonicalize
// arguments.
llvm::Expected<llvm::cas::ObjectRef>
createCompileJobBaseCacheKey(llvm::cas::ObjectStore &CAS,
                             ArrayRef<const char *> Args);

/// Compute CompileJobKey for the compiler outputs. The key for the output
/// is computed from the base key for the compilation and the input file index
/// which is the index for the input among all the input files (not just the
/// output producing inputs).
llvm::Expected<llvm::cas::ObjectRef>
createCompileJobCacheKeyForOutput(llvm::cas::ObjectStore &CAS,
                                  llvm::cas::ObjectRef BaseKey,
                                  unsigned InputIndex);
} // namespace nextcode

#endif
