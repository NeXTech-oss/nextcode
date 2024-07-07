//===--- PersistentParserState.cpp - Parser State Implementation ----------===//
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
//
//  This file implements parser state persistent across multiple parses.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/Expr.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/SourceManager.h"
#include "nextcode/Parse/PersistentParserState.h"

using namespace nextcode;

PersistentParserState::PersistentParserState() { }

PersistentParserState::~PersistentParserState() { }

void PersistentParserState::setIDEInspectionDelayedDeclState(
    SourceManager &SM, unsigned BufferID, IDEInspectionDelayedDeclKind Kind,
    DeclContext *ParentContext, SourceRange BodyRange, SourceLoc PreviousLoc) {
  assert(!IDEInspectionDelayedDeclStat.get() &&
         "only one decl can be delayed for code completion");
  unsigned startOffset = SM.getLocOffsetInBuffer(BodyRange.Start, BufferID);
  unsigned endOffset = SM.getLocOffsetInBuffer(BodyRange.End, BufferID);
  unsigned prevOffset = ~0U;
  if (PreviousLoc.isValid())
    prevOffset = SM.getLocOffsetInBuffer(PreviousLoc, BufferID);

  IDEInspectionDelayedDeclStat.reset(new IDEInspectionDelayedDeclState(
      Kind, ParentContext, startOffset, endOffset, prevOffset));
}

void PersistentParserState::restoreIDEInspectionDelayedDeclState(
    const IDEInspectionDelayedDeclState &other) {
  IDEInspectionDelayedDeclStat.reset(new IDEInspectionDelayedDeclState(
      other.Kind, other.ParentContext, other.StartOffset, other.EndOffset,
      other.PrevOffset));
}
