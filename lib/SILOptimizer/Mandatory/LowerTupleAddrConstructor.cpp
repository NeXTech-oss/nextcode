//===--- LowerTupleAddrConstructor.cpp ------------------------------------===//
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

#include "nextcode/SIL/SILBuilder.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"

using namespace nextcode;

//===----------------------------------------------------------------------===//
//                         MARK: Top Level Entrypoint
//===----------------------------------------------------------------------===//

namespace {

class LowerTupleAddrConstructorTransform : public SILFunctionTransform {
  void run() override {
    SILFunction *function = getFunction();

    // Once we have finished, lower all tuple_addr_constructor that we see.
    bool deletedInst = false;
    for (auto &block : *function) {
      for (auto ii = block.begin(), ie = block.end(); ii != ie;) {
        auto *inst = dyn_cast<TupleAddrConstructorInst>(&*ii);
        ++ii;

        if (!inst)
          continue;

        SILBuilderWithScope builder(inst);

        unsigned count = 0;
        visitExplodedTupleValue(
            inst->getDest(),
            [&](SILValue value, std::optional<unsigned> index) -> SILValue {
              if (!index) {
                SILValue elt = inst->getElement(count);
                if (elt->getType().isAddress()) {
                  builder.createCopyAddr(inst->getLoc(), elt, value, IsTake,
                                         inst->isInitializationOfDest());
                } else {
                  builder.emitStoreValueOperation(
                      inst->getLoc(), elt, value,
                      bool(inst->isInitializationOfDest())
                          ? StoreOwnershipQualifier::Init
                          : StoreOwnershipQualifier::Assign);
                }
                ++count;
                return value;
              }
              auto *teai =
                  builder.createTupleElementAddr(inst->getLoc(), value, *index);
              return teai;
            });
        inst->eraseFromParent();
        deletedInst = true;
      }
    }

    if (deletedInst)
      invalidateAnalysis(SILAnalysis::InvalidationKind::Instructions);
  }
};

} // end anonymous namespace

SILTransform *nextcode::createLowerTupleAddrConstructor() {
  return new LowerTupleAddrConstructorTransform();
}
