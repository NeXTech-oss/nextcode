//===--- SILMoveOnlyDeinit.cpp ---------------------------------------===//
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

#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILMoveOnlyDeinit.h"
#include "nextcode/SIL/SILModule.h"
#include "llvm/Analysis/ValueTracking.h"

using namespace nextcode;

SILMoveOnlyDeinit *SILMoveOnlyDeinit::create(SILModule &mod,
                                             NominalTypeDecl *nominalDecl,
                                             SerializedKind_t serialized,
                                             SILFunction *funcImpl) {
  auto buf =
      mod.allocate(sizeof(SILMoveOnlyDeinit), alignof(SILMoveOnlyDeinit));
  auto *table =
      ::new (buf) SILMoveOnlyDeinit(nominalDecl, funcImpl, unsigned(serialized));
  mod.moveOnlyDeinits.push_back(table);
  mod.MoveOnlyDeinitMap[nominalDecl] = table;
  return table;
}

SILMoveOnlyDeinit::SILMoveOnlyDeinit(NominalTypeDecl *nominalDecl,
                                     SILFunction *implementation,
                                     unsigned serialized)
    : nominalDecl(nominalDecl), funcImpl(implementation),
      serialized(serialized) {
  assert(funcImpl);
  funcImpl->incrementRefCount();
}

SILMoveOnlyDeinit::~SILMoveOnlyDeinit() {
  if (funcImpl)
    funcImpl->decrementRefCount();
}
