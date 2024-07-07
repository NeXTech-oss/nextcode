//===--- IRGenSILPasses.cpp - The IRGen Prepare SIL Passes ----------------===//
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
//  This file implements the registration of SILOptimizer passes necessary for
//  IRGen.
//
//===----------------------------------------------------------------------===//

#include "nextcode/IRGen/IRGenSILPasses.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/Subsystems.h"

namespace nextcode {
using SILTransformCtor = SILTransform *(void);
static SILTransformCtor *irgenSILPassFns[] = {
#define PASS(Name, Tag, Desc)
#define IRGEN_PASS(Name, Tag, Desc) &irgen::create##Name,
#include "nextcode/SILOptimizer/PassManager/Passes.def"
};
} // end namespace nextcode

void nextcode::registerIRGenSILTransforms(ASTContext &ctx) {
  ctx.registerIRGenSILTransforms(irgenSILPassFns);
}
