//===--- ApplySite.cpp - Wrapper around apply instructions ----------------===//
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

#include "nextcode/SIL/ApplySite.h"
#include "nextcode/SIL/SILBuilder.h"


using namespace nextcode;

void ApplySite::insertAfterInvocation(function_ref<void(SILBuilder &)> func) const {
  SILBuilderWithScope::insertAfter(getInstruction(), func);
}

void ApplySite::insertAfterApplication(
    function_ref<void(SILBuilder &)> func) const {
  switch (getKind()) {
  case ApplySiteKind::ApplyInst:
  case ApplySiteKind::TryApplyInst:
  case ApplySiteKind::PartialApplyInst:
    return insertAfterInvocation(func);
  case ApplySiteKind::BeginApplyInst:
    SmallVector<EndApplyInst *, 2> endApplies;
    SmallVector<AbortApplyInst *, 2> abortApplies;
    auto *bai = cast<BeginApplyInst>(getInstruction());
    bai->getCoroutineEndPoints(endApplies, abortApplies);
    for (auto *eai : endApplies) {
      SILBuilderWithScope builder(std::next(eai->getIterator()));
      func(builder);
    }
    for (auto *aai : abortApplies) {
      SILBuilderWithScope builder(std::next(aai->getIterator()));
      func(builder);
    }
    return;
  }
  llvm_unreachable("covered switch isn't covered");
}

