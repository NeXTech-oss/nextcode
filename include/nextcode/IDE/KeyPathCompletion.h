//===--- KeyPathCompletion.h ----------------------------------------------===//
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

#ifndef NEXTCODE_IDE_KEYPATHCOMPLETION_H
#define NEXTCODE_IDE_KEYPATHCOMPLETION_H

#include "nextcode/IDE/CodeCompletionConsumer.h"
#include "nextcode/IDE/CodeCompletionContext.h"
#include "nextcode/IDE/TypeCheckCompletionCallback.h"

namespace nextcode {
namespace ide {

class KeyPathTypeCheckCompletionCallback : public TypeCheckCompletionCallback {
  struct Result {
    /// The type on which completion should occur, i.e. a result type of the
    /// previous component.
    Type BaseType;
    /// Whether code completion happens on the key path's root.
    bool OnRoot;
  };

  KeyPathExpr *KeyPath;
  SmallVector<Result, 4> Results;

  void sawSolutionImpl(const constraints::Solution &solution) override;

public:
  KeyPathTypeCheckCompletionCallback(KeyPathExpr *KeyPath) : KeyPath(KeyPath) {}

  void collectResults(DeclContext *DC, SourceLoc DotLoc,
                      ide::CodeCompletionContext &CompletionCtx);
};

} // end namespace ide
} // end namespace nextcode

#endif // NEXTCODE_IDE_KEYPATHCOMPLETION_H
