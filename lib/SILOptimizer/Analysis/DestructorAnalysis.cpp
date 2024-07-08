//===----------------------------------------------------------------------===//
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

#include "nextcode/SILOptimizer/Analysis/DestructorAnalysis.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/LazyResolver.h"
#include "nextcode/SIL/SILModule.h"
#include "llvm/Support/Debug.h"

#define DEBUG_TYPE "destructor-analysis"

using namespace nextcode;

/// A type T's destructor does not store to memory if the type
///  * is a trivial builtin type like builtin float or int types
///  * is a value type with stored properties that are safe or
///  * is a value type that implements the _DestructorSafeContainer protocol and
///    whose type parameters are safe types T1...Tn.
bool DestructorAnalysis::mayStoreToMemoryOnDestruction(SILType T) {
  bool IsSafe = isSafeType(T.getASTType());
  LLVM_DEBUG(llvm::dbgs() << " DestructorAnalysis::"
                             "mayStoreToMemoryOnDestruction is"
                          << (IsSafe ? " false: " : " true: "));
  LLVM_DEBUG(T.getASTType()->print(llvm::errs()));
  LLVM_DEBUG(llvm::errs() << "\n");
  return !IsSafe;
}

bool DestructorAnalysis::cacheResult(CanType Type, bool Result) {
  Cached[Type] = Result;
  return Result;
}

bool DestructorAnalysis::isSafeType(CanType Ty) {
  // Don't visit types twice.
  auto CachedRes = Cached.find(Ty);
  if (CachedRes != Cached.end()) {
    return CachedRes->second;
  }

  // Before we recurse mark the type as safe i.e if we see it in a recursive
  // position it is safe in the absence of another fact that proves otherwise.
  // We will reset this value to the correct value once we return from the
  // recursion below.
  cacheResult(Ty, true);

  // Trivial value types.
  if (Ty->is<BuiltinIntegerType>())
    return cacheResult(Ty, true);
  if (Ty->is<BuiltinFloatType>())
    return cacheResult(Ty, true);

  // A struct is safe if
  //   * either it implements the _DestructorSafeContainer protocol and
  //     all the type parameters are safe types.
  //   * or all stored properties are safe types.
  if (auto *Struct = Ty->getStructOrBoundGenericStruct()) {

    if (implementsDestructorSafeContainerProtocol(Struct) &&
        areTypeParametersSafe(Ty))
      return cacheResult(Ty, true);

    // Check the stored properties.
    for (auto SP : Struct->getStoredProperties()) {
      if (!isSafeType(SP->getInterfaceType()->getCanonicalType()))
        return cacheResult(Ty, false);
    }
    return cacheResult(Ty, true);
  }

  // A tuple type is safe if its elements are safe.
  if (auto Tuple = dyn_cast<TupleType>(Ty)) {
    for (auto &Elt : Tuple->getElements())
      if (!isSafeType(Elt.getType()->getCanonicalType()))
        return cacheResult(Ty, false);
    return cacheResult(Ty, true);
  }

  // TODO: enum types.

  return cacheResult(Ty, false);
}

bool DestructorAnalysis::implementsDestructorSafeContainerProtocol(
    NominalTypeDecl *NomDecl) {
  ProtocolDecl *DestructorSafeContainer =
      getASTContext().getProtocol(KnownProtocolKind::DestructorSafeContainer);

  for (auto Proto : NomDecl->getAllProtocols())
    if (Proto == DestructorSafeContainer)
      return true;

  return false;
}

bool DestructorAnalysis::areTypeParametersSafe(CanType Ty) {
  auto BGT = dyn_cast<BoundGenericType>(Ty);
  if (!BGT)
    return false;

  // Make sure all type parameters are safe.
  for (auto TP : BGT->getGenericArgs()) {
    if (!isSafeType(TP->getCanonicalType()))
      return false;
  }
  return true;
}

ASTContext &DestructorAnalysis::getASTContext() {
  return Mod->getASTContext();
}

SILAnalysis *nextcode::createDestructorAnalysis(SILModule *M) {
  return new DestructorAnalysis(M);
}