//===--- GenControl.cpp - IR Generation for Control Flow ------------------===//
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
//  This file implements general IR generation for control flow.
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/Function.h"
#include "IRGenFunction.h"
#include "IRGenModule.h"
#include "nextcode/Basic/Assertions.h"

using namespace nextcode;
using namespace irgen;

/// Insert the given basic block after the IP block and move the
/// insertion point to it.  Only valid if the IP is valid.
void IRBuilder::emitBlock(llvm::BasicBlock *BB) {
  assert(ClearedIP == nullptr);
  llvm::BasicBlock *CurBB = GetInsertBlock();
  assert(CurBB && "current insertion point is invalid");
  CurBB->getParent()->insert(std::next(CurBB->getIterator()), BB);
  IRBuilderBase::SetInsertPoint(BB);
}

/// Create a new basic block with the given name.  The block is not
/// automatically inserted into the function.
llvm::BasicBlock *
IRGenFunction::createBasicBlock(const llvm::Twine &Name) {
  return llvm::BasicBlock::Create(IGM.getLLVMContext(), Name);
}

