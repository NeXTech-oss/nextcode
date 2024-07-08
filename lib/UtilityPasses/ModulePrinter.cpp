//===--- ModulePrinter.cpp - Module printer pass --------------------------===//
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
//
// A utility module pass to print the module as textual SIL.
//
//===----------------------------------------------------------------------===//

#include "nextcode/SIL/SILPrintContext.h"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"

using namespace nextcode;

namespace {

class SILModulePrinter : public SILModuleTransform {

  /// The entry point.
  void run() override {
    auto *module = getModule();
    SILPrintContext context(llvm::outs(), /*Verbose*/ true, /*SortedSIL*/ true,
                            /*PrintFullConvention*/ true);
    module->print(context);
  }
};

} // end anonymous namespace

SILTransform *nextcode::createModulePrinter() { return new SILModulePrinter(); }
