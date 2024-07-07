//===--- ClangSourceBufferImporter.cpp - Map Clang buffers to NeXTCode -------===//
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

#include "ClangSourceBufferImporter.h"
#include "nextcode/Basic/SourceManager.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/Support/MemoryBuffer.h"

using namespace nextcode;
using namespace nextcode::importer;

static SourceLoc findEndOfLine(SourceManager &SM, SourceLoc loc,
                               unsigned bufferID) {
  CharSourceRange entireBuffer = SM.getRangeForBuffer(bufferID);
  CharSourceRange rangeFromLoc{SM, loc, entireBuffer.getEnd()};
  StringRef textFromLoc = SM.extractText(rangeFromLoc);
  size_t newlineOffset = textFromLoc.find_first_of({"\r\n\0", 3});
  if (newlineOffset == StringRef::npos)
    return entireBuffer.getEnd();
  return loc.getAdvancedLoc(newlineOffset);
}

SourceLoc ClangSourceBufferImporter::resolveSourceLocation(
    const clang::SourceManager &clangSrcMgr,
    clang::SourceLocation clangLoc) {
  SourceLoc loc;

  clangLoc = clangSrcMgr.getFileLoc(clangLoc);
  auto decomposedLoc = clangSrcMgr.getDecomposedLoc(clangLoc);
  if (decomposedLoc.first.isInvalid())
    return loc;

  auto clangFileID = decomposedLoc.first;
  auto buffer = clangSrcMgr.getBufferOrFake(clangFileID);
  unsigned mirrorID;

  auto mirrorIter = mirroredBuffers.find(buffer.getBufferStart());
  if (mirrorIter != mirroredBuffers.end()) {
    mirrorID = mirrorIter->second;
  } else {
    std::unique_ptr<llvm::MemoryBuffer> mirrorBuffer{
      llvm::MemoryBuffer::getMemBuffer(buffer.getBuffer(),
                                       buffer.getBufferIdentifier(),
                                       /*RequiresNullTerminator=*/true)
    };
    mirrorID = nextcodeSourceManager.addNewSourceBuffer(std::move(mirrorBuffer));
    mirroredBuffers[buffer.getBufferStart()] = mirrorID;
  }
  loc = nextcodeSourceManager.getLocForOffset(mirrorID, decomposedLoc.second);

  auto presumedLoc = clangSrcMgr.getPresumedLoc(clangLoc);
  if (!presumedLoc.getFilename())
    return loc;
  if (presumedLoc.getLine() == 0)
    return SourceLoc();

  unsigned bufferLineNumber =
    clangSrcMgr.getLineNumber(decomposedLoc.first, decomposedLoc.second);

  StringRef presumedFile = presumedLoc.getFilename();
  SourceLoc startOfLine = loc.getAdvancedLoc(-presumedLoc.getColumn() + 1);

  // FIXME: Virtual files can't actually model the EOF position correctly, so
  // if this virtual file would start at EOF, just hope the physical location
  // will do.
  if (startOfLine != nextcodeSourceManager.getRangeForBuffer(mirrorID).getEnd()) {
    bool isNewVirtualFile = nextcodeSourceManager.openVirtualFile(
        startOfLine, presumedFile, presumedLoc.getLine() - bufferLineNumber);
    if (isNewVirtualFile) {
      SourceLoc endOfLine = findEndOfLine(nextcodeSourceManager, loc, mirrorID);
      nextcodeSourceManager.closeVirtualFile(endOfLine);
    }
  }

  using SourceManagerRef = llvm::IntrusiveRefCntPtr<const clang::SourceManager>;
  auto iter = std::lower_bound(sourceManagersWithDiagnostics.begin(),
                               sourceManagersWithDiagnostics.end(),
                               &clangSrcMgr,
                               [](const SourceManagerRef &inArray,
                                  const clang::SourceManager *toInsert) {
    return std::less<const clang::SourceManager *>()(inArray.get(), toInsert);
  });
  if (iter == sourceManagersWithDiagnostics.end() ||
      iter->get() != &clangSrcMgr) {
    sourceManagersWithDiagnostics.insert(iter, &clangSrcMgr);
  }

  return loc;
}
