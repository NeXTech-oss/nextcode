//===--- SILParserFunctionBuilder.h ---------------------------------------===//
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

#ifndef NEXTCODE_PARSESIL_SILPARSERFUNCTIONBUILDER_H
#define NEXTCODE_PARSESIL_SILPARSERFUNCTIONBUILDER_H

#include "nextcode/SIL/SILFunctionBuilder.h"

namespace nextcode {

class LLVM_LIBRARY_VISIBILITY SILParserFunctionBuilder {
  SILFunctionBuilder builder;

public:
  SILParserFunctionBuilder(SILModule &mod) : builder(mod) {}

  SILFunction *createFunctionForForwardReference(StringRef name,
                                                 CanSILFunctionType ty,
                                                 SILLocation loc) {
    auto *result = builder.createFunction(
        SILLinkage::Private, name, ty, nullptr, loc, IsNotBare,
        IsNotTransparent, IsNotSerialized, IsNotDynamic, IsNotDistributed,
        IsNotRuntimeAccessible);
    result->setDebugScope(new (builder.mod) SILDebugScope(loc, result));

    // If we did not have a declcontext set, as a fallback set the parent module
    // of our SILFunction to the parent module of our SILModule.
    //
    // DISCUSSION: This ensures that we can perform protocol conformance checks.
    if (!result->getDeclContext()) {
      result->setParentModule(result->getModule().getNeXTCodeModule());
    }

    return result;
  }
};

} // namespace nextcode

#endif
