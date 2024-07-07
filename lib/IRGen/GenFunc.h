//===--- GenFunc.h - NeXTCode IR generation for functions ----------*- C++ -*-===//
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
//  This file provides the private interface to the function and
//  function-type emission code.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_GENFUNC_H
#define NEXTCODE_IRGEN_GENFUNC_H

#include "nextcode/AST/Types.h"

namespace llvm {
  class Function;
  class Value;
}

namespace nextcode {
namespace irgen {
  class Address;
  class Explosion;
  class ForeignFunctionInfo;
  class IRGenFunction;

  /// Project the capture address from on-stack block storage.
  Address projectBlockStorageCapture(IRGenFunction &IGF,
                                     Address storageAddr,
                                     CanSILBlockStorageType storageTy);

  /// Load the stored isolation of an @isolated(any) function type, which
  /// is assumed to be at a known offset within a closure object.
  void emitExtractFunctionIsolation(IRGenFunction &IGF,
                                    llvm::Value *fnContext,
                                    Explosion &result);
  
  /// Emit the block header into a block storage slot.
  void emitBlockHeader(IRGenFunction &IGF,
                       Address storage,
                       CanSILBlockStorageType blockTy,
                       llvm::Constant *invokeFunction,
                       CanSILFunctionType invokeTy,
                       ForeignFunctionInfo foreignInfo);

  /// Emit a partial application thunk for a function pointer applied to a
  /// partial set of argument values.
  std::optional<StackAddress> emitFunctionPartialApplication(
      IRGenFunction &IGF, SILFunction &SILFn, const FunctionPointer &fnPtr,
      llvm::Value *fnContext, Explosion &args,
      ArrayRef<SILParameterInfo> argTypes, SubstitutionMap subs,
      CanSILFunctionType origType, CanSILFunctionType substType,
      CanSILFunctionType outType, Explosion &out, bool isOutlined);
  CanType getArgumentLoweringType(CanType type, SILParameterInfo paramInfo,
                                  bool isNoEscape);
} // end namespace irgen
} // end namespace nextcode

#endif
