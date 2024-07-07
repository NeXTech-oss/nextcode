//===--- BasicSourceInfo.cpp - Simple source information ------------------===//
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

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/Basic/BasicSourceInfo.h"
#include "nextcode/Basic/Defer.h"
#include "nextcode/Basic/SourceManager.h"

using namespace nextcode;

BasicSourceFileInfo::BasicSourceFileInfo(const SourceFile *SF)
    : SFAndIsFromSF(SF, true) {
  FilePath = SF->getFilename();
}

bool BasicSourceFileInfo::isFromSourceFile() const {
  return SFAndIsFromSF.getInt();
}

void BasicSourceFileInfo::populateWithSourceFileIfNeeded() {
  const auto *SF = SFAndIsFromSF.getPointer();
  if (!SF)
    return;
  NEXTCODE_DEFER {
    SFAndIsFromSF.setPointer(nullptr);
  };

  SourceManager &SM = SF->getASTContext().SourceMgr;

  if (FilePath.empty())
    return;
  auto stat = SM.getFileSystem()->status(FilePath);
  if (!stat)
    return;

  LastModified = stat->getLastModificationTime();
  FileSize = stat->getSize();

  if (SF->hasInterfaceHash()) {
    InterfaceHashIncludingTypeMembers = SF->getInterfaceHashIncludingTypeMembers();
    InterfaceHashExcludingTypeMembers = SF->getInterfaceHash();
  } else {
    // FIXME: Parse the file with EnableInterfaceHash option.
    InterfaceHashIncludingTypeMembers = Fingerprint::ZERO();
    InterfaceHashExcludingTypeMembers = Fingerprint::ZERO();
  }

  return;
}
