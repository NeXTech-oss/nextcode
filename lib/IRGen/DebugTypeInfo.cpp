//===--- DebugTypeInfo.cpp - Type Info for Debugging ----------------------===//
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
// This file defines the data structure that holds all the debug info
// we want to emit for types.
//
//===----------------------------------------------------------------------===//

#include "DebugTypeInfo.h"
#include "FixedTypeInfo.h"
#include "IRGenModule.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILGlobalVariable.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

using namespace nextcode;
using namespace irgen;

DebugTypeInfo::DebugTypeInfo(nextcode::Type Ty, llvm::Type *FragmentStorageTy,
                             Alignment Align, bool HasDefaultAlignment,
                             bool IsMetadata, bool IsFixedBuffer,
                             std::optional<uint32_t> NumExtraInhabitants)
    : Type(Ty.getPointer()), FragmentStorageType(FragmentStorageTy),
      NumExtraInhabitants(NumExtraInhabitants),
      Align(Align), DefaultAlignment(HasDefaultAlignment),
      IsMetadataType(IsMetadata), IsFixedBuffer(IsFixedBuffer) {
  assert(Align.getValue() != 0);
}

/// Determine whether this type has an attribute specifying a custom alignment.
static bool hasDefaultAlignment(nextcode::Type Ty) {
  if (auto CanTy = Ty->getCanonicalType())
    if (auto *TyDecl = CanTy.getNominalOrBoundGenericNominal())
      if (TyDecl->getAttrs().getAttribute<AlignmentAttr>()
          || TyDecl->getAttrs().getAttribute<RawLayoutAttr>())
        return false;
  return true;
}

DebugTypeInfo DebugTypeInfo::getFromTypeInfo(nextcode::Type Ty, const TypeInfo &TI,
                                             IRGenModule &IGM) {
  llvm::Type *StorageType = TI.getStorageType();
  std::optional<uint32_t> NumExtraInhabitants;
  if (TI.isFixedSize()) {
    const FixedTypeInfo &FixTy = *cast<const FixedTypeInfo>(&TI);
    NumExtraInhabitants = FixTy.getFixedExtraInhabitantCount(IGM);
  }
  assert(TI.getStorageType() && "StorageType is a nullptr");
  return DebugTypeInfo(Ty.getPointer(), StorageType,
                       TI.getBestKnownAlignment(), ::hasDefaultAlignment(Ty),
                       /* IsMetadataType = */ false,
                       /* IsFixedBuffer = */ false, NumExtraInhabitants);
}

DebugTypeInfo DebugTypeInfo::getLocalVariable(VarDecl *Decl, nextcode::Type Ty,
                                              const TypeInfo &Info,
                                              IRGenModule &IGM) {

  auto DeclType = Decl->getInterfaceType();
  auto RealType = Ty;

  // DynamicSelfType is also sugar as far as debug info is concerned.
  auto Sugared = DeclType;
  if (auto DynSelfTy = DeclType->getAs<DynamicSelfType>())
    Sugared = DynSelfTy->getSelfType();

  // Prefer the original, potentially sugared version of the type if
  // the type hasn't been mucked with by an optimization pass.
  auto *Type = Sugared->isEqual(RealType) ? DeclType.getPointer()
                                          : RealType.getPointer();
  return getFromTypeInfo(Type, Info, IGM);
}

DebugTypeInfo DebugTypeInfo::getGlobalMetadata(nextcode::Type Ty,
                                               llvm::Type *StorageTy, Size size,
                                               Alignment align) {
  DebugTypeInfo DbgTy(Ty.getPointer(), StorageTy, align,
                      /* HasDefaultAlignment = */ true,
                      /* IsMetadataType = */ false);
  assert(StorageTy && "StorageType is a nullptr");
  assert(!DbgTy.isContextArchetype() &&
         "type metadata cannot contain an archetype");
  return DbgTy;
}

DebugTypeInfo DebugTypeInfo::getTypeMetadata(nextcode::Type Ty,
                                             llvm::Type *StorageTy, Size size,
                                             Alignment align) {
  DebugTypeInfo DbgTy(Ty.getPointer(), StorageTy, align,
                      /* HasDefaultAlignment = */ true,
                      /* IsMetadataType = */ true);
  assert(StorageTy && "StorageType is a nullptr");
  assert(!DbgTy.isContextArchetype() &&
         "type metadata cannot contain an archetype");
  return DbgTy;
}

DebugTypeInfo DebugTypeInfo::getForwardDecl(nextcode::Type Ty) {
  DebugTypeInfo DbgTy(Ty.getPointer());
  return DbgTy;
}

DebugTypeInfo DebugTypeInfo::getGlobal(SILGlobalVariable *GV,
                                       llvm::Type *FragmentStorageType,
                                       IRGenModule &IGM) {
  // Prefer the original, potentially sugared version of the type if
  // the type hasn't been mucked with by an optimization pass.
  auto LowTy = GV->getLoweredType().getASTType();
  auto *Type = LowTy.getPointer();
  if (auto *Decl = GV->getDecl()) {
    auto DeclType = Decl->getTypeInContext();
    if (DeclType->isEqual(LowTy))
      Type = DeclType.getPointer();
  }
  auto &TI = IGM.getTypeInfoForUnlowered(Type);
  DebugTypeInfo DbgTy = getFromTypeInfo(Type, TI, IGM);
  assert(FragmentStorageType && "FragmentStorageType is a nullptr");
  assert(!DbgTy.isContextArchetype() &&
         "type of global variable cannot be an archetype");
  return DbgTy;
}

DebugTypeInfo
DebugTypeInfo::getGlobalFixedBuffer(SILGlobalVariable *GV,
                                    llvm::Type *FragmentStorageType,
                                    Size SizeInBytes, Alignment Align) {
  // Prefer the original, potentially sugared version of the type if
  // the type hasn't been mucked with by an optimization pass.
  auto LowTy = GV->getLoweredType().getASTType();
  auto *Type = LowTy.getPointer();
  if (auto *Decl = GV->getDecl()) {
    auto DeclType = Decl->getTypeInContext();
    if (DeclType->isEqual(LowTy))
      Type = DeclType.getPointer();
  }
  DebugTypeInfo DbgTy(Type, FragmentStorageType,
                      Align, ::hasDefaultAlignment(Type),
                      /* IsMetadataType = */ false, /* IsFixedBuffer = */ true);
  assert(FragmentStorageType && "FragmentStorageType is a nullptr");
  assert(!DbgTy.isContextArchetype() &&
         "type of global variable cannot be an archetype");
  return DbgTy;
}

DebugTypeInfo DebugTypeInfo::getObjCClass(ClassDecl *theClass,
                                          llvm::Type *FragmentStorageType,
                                          Size SizeInBytes, Alignment align) {
  DebugTypeInfo DbgTy(theClass->getInterfaceType().getPointer(),
                      FragmentStorageType, align,
                      /* HasDefaultAlignment = */ true,
                      /* IsMetadataType = */ false);
  assert(FragmentStorageType && "FragmentStorageType is a nullptr");
  assert(!DbgTy.isContextArchetype() &&
         "type of objc class cannot be an archetype");
  return DbgTy;
}

DebugTypeInfo DebugTypeInfo::getErrorResult(nextcode::Type Ty,
                                            IRGenModule &IGM) {
  auto &TI = IGM.getTypeInfoForUnlowered(Ty);
  DebugTypeInfo DbgTy = getFromTypeInfo(Ty, TI, IGM);
  return DbgTy;
}

bool DebugTypeInfo::operator==(DebugTypeInfo T) const {
  return getType() == T.getType() &&
         Align == T.Align;
}

bool DebugTypeInfo::operator!=(DebugTypeInfo T) const { return !operator==(T); }

TypeDecl *DebugTypeInfo::getDecl() const {
  if (auto *N = dyn_cast<NominalType>(Type))
    return N->getDecl();
  if (auto *BTA = dyn_cast<TypeAliasType>(Type))
    return BTA->getDecl();
  if (auto *UBG = dyn_cast<UnboundGenericType>(Type))
    return UBG->getDecl();
  if (auto *BG = dyn_cast<BoundGenericType>(Type))
    return BG->getDecl();
  if (auto *E = dyn_cast<ExistentialType>(Type))
    return E->getConstraintType()->getAnyNominal();
  return nullptr;
}

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
LLVM_DUMP_METHOD void DebugTypeInfo::dump() const {
  llvm::errs() << "[";
  llvm::errs() << "Alignment " << Align.getValue() << "] ";
  if (auto *Type = getType())
    Type->dump(llvm::errs());

  if (FragmentStorageType) {
    llvm::errs() << "FragmentStorageType=";
    FragmentStorageType->dump();
  } else
    llvm::errs() << "forward-declared\n";
}
#endif

std::optional<CompletedDebugTypeInfo>
CompletedDebugTypeInfo::getFromTypeInfo(nextcode::Type Ty, const TypeInfo &Info, IRGenModule &IGM) {
  auto *StorageType = IGM.getStorageTypeForUnlowered(Ty);
  std::optional<uint64_t> SizeInBits;
  if (StorageType->isSized())
    SizeInBits = IGM.DataLayout.getTypeSizeInBits(StorageType);
  else if (Info.isFixedSize()) {
    const FixedTypeInfo &FixTy = *cast<const FixedTypeInfo>(&Info);
    Size::int_type Size = FixTy.getFixedSize().getValue() * 8;
    SizeInBits = Size;
  }

  return CompletedDebugTypeInfo::get(
      DebugTypeInfo::getFromTypeInfo(Ty, Info, IGM), SizeInBits);
}
