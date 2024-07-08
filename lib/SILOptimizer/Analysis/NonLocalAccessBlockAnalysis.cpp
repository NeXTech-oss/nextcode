//===--- NonLocalAccessBlockAnalysis.cpp  - Nonlocal end_access -----------===//
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

#include "nextcode/SILOptimizer/Analysis/NonLocalAccessBlockAnalysis.h"
#include "nextcode/SIL/SILFunction.h"

using namespace nextcode;

// Populate this->accessBlocks with all blocks containing a non-local
// end_access.
void NonLocalAccessBlocks::compute() {
  for (SILBasicBlock &block : *this->function) {
    for (SILInstruction &inst : block) {
      if (auto *endAccess = dyn_cast<EndAccessInst>(&inst)) {
        if (endAccess->getBeginAccess()->getParent() != endAccess->getParent())
          this->accessBlocks.insert(&block);
      } else if (isa<EndUnpairedAccessInst>(inst)) {
        this->accessBlocks.insert(&block);
      }
    }
  }
}
