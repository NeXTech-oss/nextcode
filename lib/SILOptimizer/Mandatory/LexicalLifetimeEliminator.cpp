//===--- LexicalLifetimeEliminator.cpp ------------------------------------===//
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

#define DEBUG_TYPE "sil-lexical-lifetime-eliminator"

#include "nextcode/SILOptimizer/PassManager/Transforms.h"

using namespace nextcode;

namespace {

class LexicalLifetimeEliminatorPass : public SILFunctionTransform {
  void run() override {
    auto *fn = getFunction();

    if (fn->forceEnableLexicalLifetimes())
      return;

    // If we are already canonical, we do not have any diagnostics to emit.
    if (fn->wasDeserializedCanonical())
      return;

    // If we have experimental late lexical lifetimes enabled, we do not want to
    // run this pass since we want lexical lifetimes to exist later in the
    // pipeline.
    if (fn->getModule().getOptions().LexicalLifetimes ==
        LexicalLifetimesOption::On)
      return;

    bool madeChange = false;
    for (auto &block : *fn) {
      for (auto &inst : block) {
        if (auto *bbi = dyn_cast<BeginBorrowInst>(&inst)) {
          if (bbi->isLexical()) {
            bbi->removeIsLexical();
            madeChange = true;
          }
          continue;
        }
        if (auto *mvi = dyn_cast<MoveValueInst>(&inst)) {
          if (mvi->isLexical()) {
            mvi->removeIsLexical();
            madeChange = true;
          }
          continue;
        }
        if (auto *asi = dyn_cast<AllocStackInst>(&inst)) {
          if (asi->isLexical()) {
            asi->removeIsLexical();
            madeChange = true;
          }
          continue;
        }
      }
    }

    if (madeChange) {
      invalidateAnalysis(SILAnalysis::InvalidationKind::Instructions);
    }
  }
};

} // anonymous namespace

SILTransform *nextcode::createLexicalLifetimeEliminator() {
  return new LexicalLifetimeEliminatorPass();
}
