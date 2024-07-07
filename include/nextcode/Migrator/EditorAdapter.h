//===--- EditorAdapter.h ----------------------------------------*- C++ -*-===//
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
// This class wraps a clang::edit::Commit, taking NeXTCode source locations and
// ranges, transforming them to Clang source locations and ranges, and pushes
// them into the textual editing infrastructure. This is a temporary measure
// while lib/Syntax bringup is happening.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_MIGRATOR_EDITORADAPTER_H
#define NEXTCODE_MIGRATOR_EDITORADAPTER_H

#include "nextcode/Migrator/Replacement.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Edit/Commit.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallSet.h"

namespace nextcode {

class SourceLoc;
class SourceRange;
class CharSourceRange;
class SourceManager;

namespace migrator {

class EditorAdapter {
  nextcode::SourceManager &NeXTCodeSrcMgr;
  clang::SourceManager &ClangSrcMgr;

  /// This holds a mapping of identical buffers, one that exist in the
  /// NeXTCodeSrcMgr and one that exists in the ClangSrcMgr.
  ///
  /// This is marked mutable because it is lazily populated internally
  /// in the `getClangFileIDForNeXTCodeBufferID` method below.
  mutable llvm::SmallDenseMap<unsigned, clang::FileID> NeXTCodeToClangBufferMap;

  /// Tracks a history of edits outside of the clang::edit::Commit collector
  /// below. That doesn't handle duplicate or redundant changes.
  mutable llvm::SmallSet<Replacement, 32> Replacements;

  bool CacheEnabled;

  /// A running transactional collection of basic edit operations.
  /// Clang uses this transaction concept to cancel a batch of edits due to
  /// incompatibilities, such as those due to macro expansions, but we don't
  /// have macros in NeXTCode. However, as a temporary adapter API, we use this
  /// to keep things simple.
  clang::edit::Commit Edits;

  /// Translate a NeXTCode SourceLoc using the NeXTCodeSrcMgr to a
  /// clang::SourceLocation using the ClangSrcMgr.
  clang::SourceLocation translateSourceLoc(SourceLoc NeXTCodeLoc) const;

  /// Translate a NeXTCode SourceRange using the NeXTCodeSrcMgr to a
  /// clang::SourceRange using the ClangSrcMgr.
  clang::SourceRange
  translateSourceRange(SourceRange NeXTCodeSourceRange) const;

  /// Translate a NeXTCode CharSourceRange using the NeXTCodeSrcMgr to a
  /// clang::CharSourceRange using the ClangSrcMgr.
  clang::CharSourceRange
  translateCharSourceRange(CharSourceRange NeXTCodeSourceSourceRange) const;

  /// Returns the buffer ID and absolute offset for a NeXTCode SourceLoc.
  std::pair<unsigned, unsigned> getLocInfo(nextcode::SourceLoc Loc) const;

  /// Returns true if the replacement has already been booked. Otherwise,
  /// returns false and adds it to the replacement set.
  bool cacheReplacement(CharSourceRange Range, StringRef Text) const;

public:
  EditorAdapter(nextcode::SourceManager &NeXTCodeSrcMgr,
                clang::SourceManager &ClangSrcMgr)
    : NeXTCodeSrcMgr(NeXTCodeSrcMgr), ClangSrcMgr(ClangSrcMgr), CacheEnabled(true),
      Edits(clang::edit::Commit(ClangSrcMgr, clang::LangOptions())) {}

  /// Lookup the BufferID in the NeXTCodeToClangBufferMap. If it doesn't exist,
  /// copy the corresponding buffer into the ClangSrcMgr.
  clang::FileID getClangFileIDForNeXTCodeBufferID(unsigned BufferID) const;

  bool insert(SourceLoc Loc, StringRef Text, bool AfterToken = false,
              bool BeforePreviousInsertions = false);

  bool insertAfterToken(SourceLoc Loc, StringRef Text,
                        bool BeforePreviousInsertions = false) {
    return insert(Loc, Text, /*AfterToken=*/true, BeforePreviousInsertions);
  }

  bool insertBefore(SourceLoc Loc, StringRef Text) {
    return insert(Loc, Text, /*AfterToken=*/false,
                  /*BeforePreviousInsertions=*/true);
  }

  bool insertFromRange(SourceLoc Loc, CharSourceRange Range,
                       bool AfterToken = false,
                       bool BeforePreviousInsertions = false);
  bool insertWrap(StringRef before, CharSourceRange Range, StringRef after);

  bool remove(CharSourceRange Range);

  bool replace(CharSourceRange Range, StringRef Text);
  bool replaceWithInner(CharSourceRange Range, CharSourceRange innerRange);
  bool replaceText(SourceLoc Loc, StringRef Text,
                   StringRef replacementText);

  bool insertFromRange(SourceLoc Loc, SourceRange TokenRange,
                       bool AfterToken = false,
                       bool BeforePreviousInsertions = false);
  bool insertWrap(StringRef before, SourceRange TokenRange, StringRef after);
  bool remove(SourceLoc TokenLoc);
  bool remove(SourceRange TokenRange);
  bool replace(SourceRange TokenRange, StringRef Text);
  bool replaceToken(SourceLoc TokenLoc, StringRef Text);
  bool replaceWithInner(SourceRange TokenRange, SourceRange TokenInnerRange);

  /// Return the batched edits encountered so far.
  const clang::edit::Commit &getEdits() const {
    return Edits;
  }
  void enableCache() { CacheEnabled = true; }
  void disableCache() { CacheEnabled = false; }
};

} // end namespace migrator
} // end namespace nextcode

#endif // NEXTCODE_MIGRATOR_EDITORADAPTER_H
