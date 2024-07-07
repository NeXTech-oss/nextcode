//===--- GenKeyPath.h - IR generation for KeyPath ---------------*- C++ -*-===//
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
//  This file provides the private interface to the emission of KeyPath
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_GENKEYPATH_H
#define NEXTCODE_IRGEN_GENKEYPATH_H

#include "GenericRequirement.h"
#include "nextcode/AST/SubstitutionMap.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/SIL/SILValue.h"
#include "llvm/IR/Value.h"

namespace nextcode {
namespace irgen {
class Explosion;
class IRGenFunction;
class StackAddress;

std::pair<llvm::Value *, llvm::Value *>
emitKeyPathArgument(IRGenFunction &IGF, SubstitutionMap subs,
                    const CanGenericSignature &sig,
                    ArrayRef<SILType> indiceTypes, Explosion &indiceValues,
                    std::optional<StackAddress> &dynamicArgsBuf);
} // end namespace irgen
} // end namespace nextcode

#endif
