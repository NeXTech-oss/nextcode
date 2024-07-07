//===--- Sandbox.h ----------------------------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_BASIC_SANDBOX_H
#define NEXTCODE_BASIC_SANDBOX_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Allocator.h"

namespace nextcode {
namespace Sandbox {

/// Applies a sandbox invocation to the given command line (if the platform
/// supports it), and returns the modified command line. On platforms that don't
/// support sandboxing, the command line is returned unmodified.
///
/// - Parameters:
///   - command: The command line to sandbox (including executable as first
///              argument)
///   - strictness: The basic strictness level of the standbox.
///   - writableDirectories: Paths under which writing should be allowed, even
///     if they would otherwise be read-only based on the strictness or paths in
///     `readOnlyDirectories`.
///   - readOnlyDirectories: Paths under which writing should be denied, even if
///     they would have otherwise been allowed by the rules implied by the
///     strictness level.
bool apply(llvm::SmallVectorImpl<llvm::StringRef> &command,
           llvm::BumpPtrAllocator &Alloc);

} // namespace Sandbox
} // namespace nextcode

#endif // NEXTCODE_BASIC_SANDBOX_H
