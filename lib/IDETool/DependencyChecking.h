//===--- DependencyChecking.h ---------------------------------------------===//
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

#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Support/Chrono.h"
#include "llvm/Support/VirtualFileSystem.h"
#include <optional>

namespace nextcode {
class CompilerInstance;

namespace ide {
/// Cache hash code of the dependencies into \p Map . If \p excludeBufferID is
/// specified, other source files are considered "dependencies", otherwise all
/// source files are considered "current"
void cacheDependencyHashIfNeeded(CompilerInstance &CI,
                                 std::optional<unsigned> excludeBufferID,
                                 llvm::StringMap<llvm::hash_code> &Map);

/// Check if any dependent files are modified since \p timestamp. If
/// \p excludeBufferID is specified, other source files are considered
/// "dependencies", otherwise all source files are considered "current".
/// \p Map should be the map populated by \c cacheDependencyHashIfNeeded at the
/// previous dependency checking.
bool areAnyDependentFilesInvalidated(
    CompilerInstance &CI, llvm::vfs::FileSystem &FS,
    std::optional<unsigned> excludeBufferID, llvm::sys::TimePoint<> timestamp,
    const llvm::StringMap<llvm::hash_code> &Map);

} // namespace ide
} // namespace nextcode
