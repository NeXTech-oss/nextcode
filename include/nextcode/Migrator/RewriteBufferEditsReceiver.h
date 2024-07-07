//===--- RewriteBufferEditsReceiver.h ---------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_MIGRATOR_REWRITEBUFFEREDITSRECEIVER_H
#define NEXTCODE_MIGRATOR_REWRITEBUFFEREDITSRECEIVER_H

#include "clang/Basic/SourceManager.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Edit/EditsReceiver.h"
#include "clang/Rewrite/Core/RewriteBuffer.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"

using llvm::StringRef;

namespace nextcode {
namespace migrator {

/// An EditsReceiver that collects edits from an EditedSource and directly
/// applies it to a clang::RewriteBuffer.
class RewriteBufferEditsReceiver final : public clang::edit::EditsReceiver {
  const clang::SourceManager &ClangSourceManager;
  const clang::FileID InputFileID;
  const StringRef InputText;
  clang::RewriteBuffer RewriteBuf;
public:
  RewriteBufferEditsReceiver(const clang::SourceManager &ClangSourceManager,
                             const clang::FileID InputFileID,
                             const StringRef InputText)
    : ClangSourceManager(ClangSourceManager),
      InputFileID(InputFileID),
      InputText(InputText) {
    RewriteBuf.Initialize(InputText);
  }

  virtual void insert(clang::SourceLocation Loc, StringRef Text) override;
  virtual void replace(clang::CharSourceRange Range, StringRef Text) override;

  /// Print the result of all of the edits to the given output stream.
  void printResult(llvm::raw_ostream &OS) const;
};

} // end namespace migrator
} // end namespace nextcode

#endif // NEXTCODE_MIGRATOR_REWRITEBUFFEREDITSRECEIVER_H
