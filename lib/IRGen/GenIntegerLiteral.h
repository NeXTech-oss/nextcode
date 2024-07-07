//===--- GenIntegerLiteral.h - IRGen for Builtin.IntegerLiteral -*- C++ -*-===//
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
//
// This file defines interfaces for emitting code for Builtin.IntegerLiteral
// values.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_GENINTEGERLITERAL_H
#define NEXTCODE_IRGEN_GENINTEGERLITERAL_H

#include "nextcode/Basic/APIntMap.h"

namespace llvm {
class Constant;
class IntegerType;
class Type;
class Value;
}

namespace nextcode {
class IntegerLiteralInst;

namespace irgen {
class Explosion;
class IRGenFunction;
class IRGenModule;

/// A constant integer literal value.
struct ConstantIntegerLiteral {
  llvm::Constant *Data;
  llvm::Constant *Flags;
};

/// A map for caching globally-emitted constant integers.
class ConstantIntegerLiteralMap {
  APIntMap<ConstantIntegerLiteral> map;

public:
  ConstantIntegerLiteralMap() {}

  ConstantIntegerLiteral get(IRGenModule &IGM, APInt &&value);
};

/// Construct a constant IntegerLiteral from an IntegerLiteralInst.
ConstantIntegerLiteral
emitConstantIntegerLiteral(IRGenModule &IGM, IntegerLiteralInst *ILI);

/// Emit a checked truncation of an IntegerLiteral value.
void emitIntegerLiteralCheckedTrunc(IRGenFunction &IGF, Explosion &in,
                                    llvm::Type *FromTy,
                                    llvm::IntegerType *resultTy,
                                    bool resultIsSigned, Explosion &out);

/// Emit a sitofp operation on an IntegerLiteral value.
llvm::Value *emitIntegerLiteralToFP(IRGenFunction &IGF,
                                    Explosion &in,
                                    llvm::Type *toType);

llvm::Value *emitIntLiteralBitWidth(IRGenFunction &IGF, Explosion &in);
llvm::Value *emitIntLiteralIsNegative(IRGenFunction &IGF, Explosion &in);
llvm::Value *emitIntLiteralWordAtIndex(IRGenFunction &IGF, Explosion &in);

}
}

#endif
