//===--- GenDecl.h - NeXTCode IR generation for some decl ----------*- C++ -*-===//
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
//  This file provides the private interface to some decl emission code.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_GENDECL_H
#define NEXTCODE_IRGEN_GENDECL_H

#include "DebugTypeInfo.h"
#include "IRGen.h"
#include "nextcode/Basic/OptimizationMode.h"
#include "nextcode/SIL/SILLocation.h"
#include "clang/AST/DeclCXX.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/Support/CommandLine.h"

namespace llvm {
  class AttributeList;
  class Function;
  class FunctionType;
  class CallBase;
}
namespace nextcode {
namespace irgen {
  class IRGenModule;
  class LinkEntity;
  class LinkInfo;
  class Signature;

  void updateLinkageForDefinition(IRGenModule &IGM,
                                  llvm::GlobalValue *global,
                                  const LinkEntity &entity);

  llvm::Function *createFunction(
      IRGenModule &IGM, LinkInfo &linkInfo, const Signature &signature,
      llvm::Function *insertBefore = nullptr,
      OptimizationMode FuncOptMode = OptimizationMode::NotSet,
      StackProtectorMode stackProtect = StackProtectorMode::NoStackProtector);

  llvm::GlobalVariable *
  createVariable(IRGenModule &IGM, LinkInfo &linkInfo, llvm::Type *objectType,
                 Alignment alignment, DebugTypeInfo DebugType = DebugTypeInfo(),
                 std::optional<SILLocation> DebugLoc = std::nullopt,
                 StringRef DebugName = StringRef());

  llvm::GlobalVariable *
  createLinkerDirectiveVariable(IRGenModule &IGM, StringRef Name);

  void disableAddressSanitizer(IRGenModule &IGM, llvm::GlobalVariable *var);

  /// If the calling convention for `ctor` doesn't match the calling convention
  /// that we assumed for it when we imported it as `initializer`, emit and
  /// return a thunk that conforms to the assumed calling convention. The thunk
  /// is marked `alwaysinline`, so it doesn't generate any runtime overhead.
  /// If the assumed calling convention was correct, just return `ctor`.
  ///
  /// See also comments in CXXMethodConventions in SIL/IR/SILFunctionType.cpp.
  llvm::Constant *
  emitCXXConstructorThunkIfNeeded(IRGenModule &IGM, Signature signature,
                                  const clang::CXXConstructorDecl *ctor,
                                  StringRef name, llvm::Constant *ctorAddress);

  llvm::CallBase *emitCXXConstructorCall(IRGenFunction &IGF,
                                         const clang::CXXConstructorDecl *ctor,
                                         llvm::FunctionType *ctorFnType,
                                         llvm::Constant *ctorAddress,
                                         llvm::ArrayRef<llvm::Value *> args);
}
}

extern llvm::cl::opt<bool> UseBasicDynamicReplacement;

#endif