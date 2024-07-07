//===--- GenInit.cpp - IR Generation for Initialization -------------------===//
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
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//
//
//  This file implements IR generation for the initialization of
//  local and global variables.
//
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/Pattern.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILDeclRef.h"
#include "nextcode/SIL/SILGlobalVariable.h"
#include "nextcode/IRGen/Linking.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Module.h"

#include "DebugTypeInfo.h"
#include "GenTuple.h"
#include "IRGenDebugInfo.h"
#include "IRGenFunction.h"
#include "IRGenModule.h"
#include "FixedTypeInfo.h"
#include "Temporary.h"

using namespace nextcode;
using namespace irgen;

/// Emit a global variable.
void IRGenModule::emitSILGlobalVariable(SILGlobalVariable *var) {
  auto &ti = getTypeInfo(var->getLoweredType());
  auto expansion = getResilienceExpansionForLayout(var);

  // If the variable is empty in all resilience domains that can access this
  // variable directly, don't actually emit it; just return undef.
  if (ti.isKnownEmpty(expansion)) {
    if (DebugInfo && var->getDecl()) {
      auto DbgTy = DebugTypeInfo::getGlobal(var, Int8Ty, *this);
      DebugInfo->emitGlobalVariableDeclaration(nullptr, var->getDecl()->getName().str(),
                                               "", DbgTy,
                                               var->getLinkage() != SILLinkage::Public &&
                                               var->getLinkage() != SILLinkage::Package,
                                               SILLocation(var->getDecl()));
    }
    return;
  }

  /// Create the global variable.
  getAddrOfSILGlobalVariable(var, ti,
                     var->isDefinition() ? ForDefinition : NotForDefinition);
}

StackAddress FixedTypeInfo::allocateStack(IRGenFunction &IGF, SILType T,
                                          const Twine &name) const {
  // If the type is known to be empty, don't actually allocate anything.
  if (isKnownEmpty(ResilienceExpansion::Maximal)) {
    auto addr = getUndefAddress();
    return { addr };
  }

  Address alloca =
    IGF.createAlloca(getStorageType(), getFixedAlignment(), name);
  IGF.Builder.CreateLifetimeStart(alloca, getFixedSize());
  
  return { alloca };
}

StackAddress FixedTypeInfo::allocateVector(IRGenFunction &IGF, SILType T,
                                           llvm::Value *capacity,
                                           const Twine &name) const {
  // If the type is known to be empty, don't actually allocate anything.
  if (isKnownEmpty(ResilienceExpansion::Maximal)) {
    auto addr = getUndefAddress();
    return { addr };
  }

  StackAddress alloca =
    IGF.emitDynamicAlloca(getStorageType(), capacity, getFixedAlignment(),
                          /*allowTaskAlloc*/ true, name);
  IGF.Builder.CreateLifetimeStart(alloca.getAddress(), getFixedSize());

  return { alloca };
}

void FixedTypeInfo::destroyStack(IRGenFunction &IGF, StackAddress addr,
                                 SILType T, bool isOutlined) const {
  destroy(IGF, addr.getAddress(), T, isOutlined);
  FixedTypeInfo::deallocateStack(IGF, addr, T);
}

void FixedTypeInfo::deallocateStack(IRGenFunction &IGF, StackAddress addr,
                                    SILType T) const {
  if (isKnownEmpty(ResilienceExpansion::Maximal))
    return;
  IGF.Builder.CreateLifetimeEnd(addr.getAddress(), getFixedSize());
}

void TemporarySet::destroyAll(IRGenFunction &IGF) const {
  assert(!hasBeenCleared() && "destroying a set that's been cleared?");

  // Deallocate all the temporaries.
  for (auto &temporary : llvm::reverse(Stack)) {
    temporary.destroy(IGF);
  }
}

void Temporary::destroy(IRGenFunction &IGF) const {
  auto &ti = IGF.getTypeInfo(Type);
  if (Type.isSensitive()) {
    llvm::Value *size = ti.getSize(IGF, Type);
    IGF.emitClearSensitive(Addr.getAddress(), size);
  }
  ti.deallocateStack(IGF, Addr, Type);
}
