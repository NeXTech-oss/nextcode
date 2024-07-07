//===--- GenBuiltin.h - IR generation for builtin functions -----*- C++ -*-===//
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
//  This file provides the private interface to the emission of builtin
//  functions in NeXTCode.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_GENBUILTIN_H
#define NEXTCODE_IRGEN_GENBUILTIN_H

#include "nextcode/AST/SubstitutionMap.h"
#include "nextcode/Basic/LLVM.h"

namespace nextcode {
  class BuiltinInfo;
  class BuiltinInst;
  class Identifier;
  class SILType;

namespace irgen {
  class Explosion;
  class IRGenFunction;

  /// Emit a call to a builtin function.
  void emitBuiltinCall(IRGenFunction &IGF, const BuiltinInfo &builtin,
                       BuiltinInst *Inst, ArrayRef<SILType> argTypes,
                       Explosion &args, Explosion &result);

} // end namespace irgen
} // end namespace nextcode

#endif
