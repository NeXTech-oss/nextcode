//===--- EditorAdapter.cpp ------------------------------------------------===//
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
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/SourceLoc.h"
#include "nextcode/Basic/SourceManager.h"
#include "nextcode/Migrator/EditorAdapter.h"
#include "nextcode/Migrator/Replacement.h"
#include "nextcode/Parse/Lexer.h"
#include "clang/Basic/SourceManager.h"

using namespace nextcode;
using namespace nextcode::migrator;

std::pair<unsigned, unsigned>
EditorAdapter::getLocInfo(nextcode::SourceLoc Loc) const {
  auto NeXTCodeBufferID = NeXTCodeSrcMgr.findBufferContainingLoc(Loc);
  auto Offset = NeXTCodeSrcMgr.getLocOffsetInBuffer(Loc, NeXTCodeBufferID);
  return { NeXTCodeBufferID, Offset };
}

bool
EditorAdapter::cacheReplacement(CharSourceRange Range, StringRef Text) const {
  if (!CacheEnabled)
    return false;
  unsigned NeXTCodeBufferID, Offset;
  std::tie(NeXTCodeBufferID, Offset) = getLocInfo(Range.getStart());
  Replacement R{Offset, Range.getByteLength(), Text.str()};
  if (Replacements.count(R)) {
    return true;
  } else {
    Replacements.insert(R);
  }
  return false;
}

clang::FileID
EditorAdapter::getClangFileIDForNeXTCodeBufferID(unsigned BufferID) const {
  /// Check if we already have a mapping for this BufferID.
  auto Found = NeXTCodeToClangBufferMap.find(BufferID);
  if (Found != NeXTCodeToClangBufferMap.end()) {
    return Found->getSecond();
  }

  // If we don't copy the corresponding buffer's text into a new buffer
  // that the ClangSrcMgr can understand.
  auto Text = NeXTCodeSrcMgr.getEntireTextForBuffer(BufferID);
  auto NewBuffer = llvm::MemoryBuffer::getMemBufferCopy(Text);
  auto NewFileID = ClangSrcMgr.createFileID(std::move(NewBuffer));

  NeXTCodeToClangBufferMap.insert({BufferID, NewFileID});

  return NewFileID;
}

clang::SourceLocation
EditorAdapter::translateSourceLoc(SourceLoc NeXTCodeLoc) const {
  unsigned NeXTCodeBufferID, Offset;
  std::tie(NeXTCodeBufferID, Offset) = getLocInfo(NeXTCodeLoc);

  auto ClangFileID = getClangFileIDForNeXTCodeBufferID(NeXTCodeBufferID);
  return ClangSrcMgr.getLocForStartOfFile(ClangFileID).getLocWithOffset(Offset);
}

clang::SourceRange
EditorAdapter::translateSourceRange(SourceRange NeXTCodeSourceRange) const {
  auto Start = translateSourceLoc(NeXTCodeSourceRange.Start);
  auto End = translateSourceLoc(NeXTCodeSourceRange.End);
  return clang::SourceRange { Start, End };
}

clang::CharSourceRange EditorAdapter::
translateCharSourceRange(CharSourceRange NeXTCodeSourceSourceRange) const {
  auto ClangStartLoc = translateSourceLoc(NeXTCodeSourceSourceRange.getStart());
  auto ClangEndLoc = translateSourceLoc(NeXTCodeSourceSourceRange.getEnd());
  return clang::CharSourceRange::getCharRange(ClangStartLoc, ClangEndLoc);
}

bool EditorAdapter::insert(SourceLoc Loc, StringRef Text, bool AfterToken,
                           bool BeforePreviousInsertions) {
  // We don't have tokens on the clang side, so handle AfterToken in NeXTCode
  if (AfterToken)
    Loc = Lexer::getLocForEndOfToken(NeXTCodeSrcMgr, Loc);

  if (cacheReplacement(CharSourceRange { Loc, 0 }, Text)) {
    return true;
  }

  auto ClangLoc = translateSourceLoc(Loc);
  return Edits.insert(ClangLoc, Text, /*AfterToken=*/false, BeforePreviousInsertions);
}

bool EditorAdapter::insertFromRange(SourceLoc Loc, CharSourceRange Range,
                                    bool AfterToken,
                                    bool BeforePreviousInsertions) {
  // We don't have tokens on the clang side, so handle AfterToken in NeXTCode
  if (AfterToken)
    Loc = Lexer::getLocForEndOfToken(NeXTCodeSrcMgr, Loc);

  if (cacheReplacement(CharSourceRange { Loc, 0 },
                       NeXTCodeSrcMgr.extractText(Range))) {
    return true;
  }

  auto ClangLoc = translateSourceLoc(Loc);
  auto ClangCharRange = translateCharSourceRange(Range);

  return Edits.insertFromRange(ClangLoc, ClangCharRange, /*AfterToken=*/false,
                               BeforePreviousInsertions);
}

bool EditorAdapter::insertWrap(StringRef Before, CharSourceRange Range,
                               StringRef After) {
  auto ClangRange = translateCharSourceRange(Range);
  return Edits.insertWrap(Before, ClangRange, After);
}

bool EditorAdapter::remove(CharSourceRange Range) {
  if (cacheReplacement(Range, "")) {
    return true;
  }
  auto ClangRange = translateCharSourceRange(Range);
  return Edits.remove(ClangRange);
}

bool EditorAdapter::replace(CharSourceRange Range, StringRef Text) {
  if (cacheReplacement(Range, Text)) {
    return true;
  }

  auto ClangRange = translateCharSourceRange(Range);
  return Edits.replace(ClangRange, Text);
}

bool EditorAdapter::replaceWithInner(CharSourceRange Range,
                                     CharSourceRange InnerRange) {

  if (cacheReplacement(Range, NeXTCodeSrcMgr.extractText(InnerRange))) {
    return true;
  }
  auto ClangRange = translateCharSourceRange(Range);
  auto ClangInnerRange = translateCharSourceRange(InnerRange);
  return Edits.replaceWithInner(ClangRange, ClangInnerRange);
}

bool EditorAdapter::replaceText(SourceLoc Loc, StringRef Text,
                                StringRef ReplacementText) {
  auto Range = Lexer::getCharSourceRangeFromSourceRange(NeXTCodeSrcMgr,
    { Loc, Loc.getAdvancedLoc(Text.size())});
  if (cacheReplacement(Range, Text)) {
    return true;
  }

  auto ClangLoc = translateSourceLoc(Loc);
  return Edits.replaceText(ClangLoc, Text, ReplacementText);
}

bool EditorAdapter::insertFromRange(SourceLoc Loc, SourceRange TokenRange,
                     bool AfterToken,
                     bool BeforePreviousInsertions) {
  auto CharRange = Lexer::getCharSourceRangeFromSourceRange(NeXTCodeSrcMgr, TokenRange);
  return insertFromRange(Loc, CharRange,
                         AfterToken, BeforePreviousInsertions);
}

bool EditorAdapter::insertWrap(StringRef Before, SourceRange TokenRange,
                               StringRef After) {
  auto CharRange = Lexer::getCharSourceRangeFromSourceRange(NeXTCodeSrcMgr, TokenRange);
  return insertWrap(Before, CharRange, After);
}

bool EditorAdapter::remove(SourceLoc TokenLoc) {
  return remove(Lexer::getCharSourceRangeFromSourceRange(NeXTCodeSrcMgr,
                                                         TokenLoc));
}

bool EditorAdapter::remove(SourceRange TokenRange) {
  auto CharRange = Lexer::getCharSourceRangeFromSourceRange(NeXTCodeSrcMgr, TokenRange);
  return remove(CharRange);
}

bool EditorAdapter::replace(SourceRange TokenRange, StringRef Text) {
  auto CharRange = Lexer::getCharSourceRangeFromSourceRange(NeXTCodeSrcMgr,TokenRange);
  return replace(CharRange, Text);
}

bool EditorAdapter::replaceWithInner(SourceRange TokenRange,
                                     SourceRange TokenInnerRange) {
  auto CharRange = Lexer::getCharSourceRangeFromSourceRange(NeXTCodeSrcMgr, TokenRange);
  auto CharInnerRange = Lexer::getCharSourceRangeFromSourceRange(NeXTCodeSrcMgr, TokenInnerRange);
  return replaceWithInner(CharRange, CharInnerRange);
}

bool EditorAdapter::replaceToken(SourceLoc TokenLoc, StringRef Text) {
  return replace(Lexer::getTokenAtLocation(NeXTCodeSrcMgr, TokenLoc).getRange(),
    Text);
}
