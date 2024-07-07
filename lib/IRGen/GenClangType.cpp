//===--- GenClangType.cpp - NeXTCode IR Generation For Types -----------------===//
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
// Wrapper functions for creating Clang types from NeXTCode types.
//
//===----------------------------------------------------------------------===//

#include "IRGenModule.h"

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Types.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/CanonicalType.h"
#include "clang/AST/Type.h"

using namespace nextcode;
using namespace irgen;

clang::CanQualType IRGenModule::getClangType(CanType type) {
  auto *ty = type->getASTContext().getClangTypeForIRGen(type);
  return ty ? ty->getCanonicalTypeUnqualified() : clang::CanQualType();
}

clang::CanQualType IRGenModule::getClangType(SILType type) {
  if (type.isForeignReferenceType())
    return getClangType(type.getASTType()
                            ->wrapInPointer(PTK_UnsafePointer)
                            ->getCanonicalType());
  return getClangType(type.getASTType());
}

clang::CanQualType IRGenModule::getClangType(SILParameterInfo params,
                                             CanSILFunctionType funcTy) {
  auto paramTy = params.getSILStorageType(getSILModule(), funcTy,
                                          getMaximalTypeExpansionContext());
  auto clangType = getClangType(paramTy);
  // @block_storage types must be @inout_aliasable and have
  // special lowering
  if (!paramTy.is<SILBlockStorageType>()) {
    if (params.isIndirectMutating()) {
      return getClangASTContext().getPointerType(clangType);
    }
    if (params.isFormalIndirect() &&
        // Sensitive return types are represented as indirect return value in SIL,
        // but are returned as values (if small) in LLVM IR.
        !paramTy.isSensitive()) {
      auto constTy =
        getClangASTContext().getCanonicalType(clangType.withConst());
      return getClangASTContext().getPointerType(constTy);
    }
  }
  return clangType;
}
