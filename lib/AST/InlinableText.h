//===------ InlinableText.h - Extract inlinable source text -----*- C++ -*-===//
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

#ifndef NEXTCODE_AST_INLINABLETEXT_H
#define NEXTCODE_AST_INLINABLETEXT_H

#include "nextcode/AST/ASTNode.h"
#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/SmallVector.h"

namespace nextcode {
class SourceManager;

/// Extracts the text of this ASTNode from the source buffer, ignoring
/// all #if declarations and clauses except the elements that are active.
StringRef extractInlinableText(SourceManager &sourceMgr, ASTNode node,
                               SmallVectorImpl<char> &scratch);

} // end namespace nextcode

#endif // defined(NEXTCODE_AST_INLINABLETEXT_H)
