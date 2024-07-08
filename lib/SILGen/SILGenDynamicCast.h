//===--- SILGenDynamicCast.h - SILGen for dynamic casts ---------*- C++ -*-===//
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

#ifndef NEXTCODE_SILGEN_DYNAMIC_CAST_H
#define NEXTCODE_SILGEN_DYNAMIC_CAST_H

#include "SILGenFunction.h"

namespace nextcode {
namespace Lowering {

RValue emitUnconditionalCheckedCast(SILGenFunction &SGF,
                                    SILLocation loc,
                                    Expr *operand,
                                    Type targetType,
                                    CheckedCastKind castKind,
                                    SGFContext C);

RValue emitConditionalCheckedCast(SILGenFunction &SGF, SILLocation loc,
                                  ManagedValue operand, Type operandType,
                                  Type targetType, CheckedCastKind castKind,
                                  SGFContext C, ProfileCounter TrueCount,
                                  ProfileCounter FalseCount);

SILValue emitIsa(SILGenFunction &SGF, SILLocation loc,
                 Expr *operand, Type targetType,
                 CheckedCastKind castKind);

}
}

#endif
