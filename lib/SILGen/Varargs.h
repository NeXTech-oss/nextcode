//===--- Varargs.h - SIL generation for (native) NeXTCode varargs --*- C++ -*-===//
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
// A storage structure for holding a destructured rvalue with an optional
// cleanup(s).
// Ownership of the rvalue can be "forwarded" to disable the associated
// cleanup(s).
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_LOWERING_VARARGS_H
#define NEXTCODE_LOWERING_VARARGS_H

#include "ManagedValue.h"
#include "nextcode/SIL/AbstractionPattern.h"

namespace nextcode {
namespace Lowering {
class SILGenFunction;
class TypeLowering;

/// Information about a varargs emission.
class VarargsInfo {
  ManagedValue Array;
  CleanupHandle AbortCleanup;
  SILValue BaseAddress;
  AbstractionPattern BasePattern;
  const TypeLowering &BaseTL;
public:
  VarargsInfo(ManagedValue array, CleanupHandle abortCleanup,
              SILValue baseAddress, const TypeLowering &baseTL,
              AbstractionPattern basePattern)
    : Array(array), AbortCleanup(abortCleanup),
      BaseAddress(baseAddress), BasePattern(basePattern), BaseTL(baseTL) {}

  /// Return the array value.  emitEndVarargs() is really the only
  /// function that should be accessing this directly.
  ManagedValue getArray() const {
    return Array;
  }
  CleanupHandle getAbortCleanup() const { return AbortCleanup; }

  /// An address of the lowered type.
  SILValue getBaseAddress() const { return BaseAddress; }

  AbstractionPattern getBaseAbstractionPattern() const {
    return BasePattern;
  }

  const TypeLowering &getBaseTypeLowering() const {
    return BaseTL;
  }
};

/// Begin a varargs emission sequence.
VarargsInfo emitBeginVarargs(SILGenFunction &SGF, SILLocation loc,
                             CanType baseTy, CanType arrayTy,
                             unsigned numElements);

/// Successfully end a varargs emission sequence.
ManagedValue emitEndVarargs(SILGenFunction &SGF, SILLocation loc,
                            VarargsInfo &&varargs,
                             unsigned numElements); 

} // end namespace Lowering
} // end namespace nextcode

#endif