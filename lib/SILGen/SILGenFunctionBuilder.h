//===--- SILGenFunctionBuilder.h ------------------------------------------===//
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

#ifndef NEXTCODE_SILGEN_SILGENFUNCTIONBUILDER_H
#define NEXTCODE_SILGEN_SILGENFUNCTIONBUILDER_H

#include "nextcode/SIL/SILFunctionBuilder.h"

namespace nextcode {
namespace Lowering {

class LLVM_LIBRARY_VISIBILITY SILGenFunctionBuilder {
  SILFunctionBuilder builder;

public:
  SILGenFunctionBuilder(SILGenModule &SGM) : builder(SGM.M) {}
  SILGenFunctionBuilder(SILGenFunction &SGF) : builder(SGF.SGM.M) {}

  template <class... ArgTys>
  SILFunction *getOrCreateSharedFunction(ArgTys &&... args) {
    return builder.getOrCreateSharedFunction(std::forward<ArgTys>(args)...);
  }

  template <class... ArgTys>
  SILFunction *getOrCreateFunction(ArgTys &&... args) {
    return builder.getOrCreateFunction(std::forward<ArgTys>(args)...);
  }

  template <class... ArgTys> SILFunction *createFunction(ArgTys &&... args) {
    return builder.createFunction(std::forward<ArgTys>(args)...);
  }
};

} // namespace Lowering
} // namespace nextcode

#endif
