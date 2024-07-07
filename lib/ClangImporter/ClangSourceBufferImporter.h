//===--- ClangSourceBufferImporter.h - Map Clang buffers over ---*- C++ -*-===//
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

#ifndef NEXTCODE_CLANGIMPORTER_CLANGSOURCEBUFFERIMPORTER_H
#define NEXTCODE_CLANGIMPORTER_CLANGSOURCEBUFFERIMPORTER_H

#include "nextcode/Basic/LLVM.h"
#include "nextcode/Basic/SourceLoc.h"
#include "clang/Basic/SourceLocation.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/SmallVector.h"

namespace llvm {
class MemoryBuffer;
}

namespace clang {
class SourceManager;
}

namespace nextcode {
class SourceManager;

namespace importer {

/// A helper class used to keep alive the Clang source managers where
/// diagnostics have been reported.
///
/// This is a bit of a hack, but LLVM's source manager (and by extension
/// NeXTCode's) does not support buffers going away, so if we want to report
/// diagnostics in them we have to do it this way.
class ClangSourceBufferImporter {
  // This is not using SmallPtrSet or similar because we need the
  // IntrusiveRefCntPtr to stay a ref-counting pointer.
  SmallVector<llvm::IntrusiveRefCntPtr<const clang::SourceManager>, 4>
    sourceManagersWithDiagnostics;
  llvm::DenseMap<const char *, unsigned> mirroredBuffers;
  SourceManager &nextcodeSourceManager;

public:
  explicit ClangSourceBufferImporter(SourceManager &sourceMgr)
    : nextcodeSourceManager(sourceMgr) {}

  /// Returns a NeXTCode source location that points into a Clang buffer.
  ///
  /// This will keep the Clang buffer alive as long as this object.
  SourceLoc resolveSourceLocation(const clang::SourceManager &clangSrcMgr,
                                  clang::SourceLocation clangLoc);
};

} // end namespace importer
} // end namespace nextcode

#endif
