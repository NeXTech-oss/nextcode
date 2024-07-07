//===--- DebugTypeInfo.h - Type Info for Debugging --------------*- C++ -*-===//
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
// This file defines the data structure that holds all the debug info
// we want to emit for types.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_DEBUGTYPEINFO_H
#define NEXTCODE_IRGEN_DEBUGTYPEINFO_H

#include "IRGen.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/Types.h"

namespace llvm {
class Type;
}

namespace nextcode {
class SILDebugScope;
class SILGlobalVariable;

namespace irgen {
class TypeInfo;
class IRGenModule;

/// This data structure holds everything needed to emit debug info
/// for a type.
class DebugTypeInfo {
protected:
  /// The type we need to emit may be different from the type
  /// mentioned in the Decl, for example, stripped of qualifiers.
  TypeBase *Type = nullptr;
  /// Needed to determine the size of basic types and to determine
  /// the storage type for undefined variables.
  llvm::Type *FragmentStorageType = nullptr;
  std::optional<uint32_t> NumExtraInhabitants;
  Alignment Align;
  bool DefaultAlignment = true;
  bool IsMetadataType = false;
  bool IsFixedBuffer = false;

public:
  DebugTypeInfo() = default;
  DebugTypeInfo(nextcode::Type Ty, llvm::Type *StorageTy = nullptr,
                Alignment AlignInBytes = Alignment(1),
                bool HasDefaultAlignment = true, bool IsMetadataType = false,
                bool IsFixedBuffer = false,
                std::optional<uint32_t> NumExtraInhabitants = {});

  /// Create type for a local variable.
  static DebugTypeInfo getLocalVariable(VarDecl *Decl, nextcode::Type Ty,
                                        const TypeInfo &Info, IRGenModule &IGM);
  /// Create type for global type metadata.
  static DebugTypeInfo getGlobalMetadata(nextcode::Type Ty, llvm::Type *StorageTy,
                                         Size size, Alignment align);
  /// Create type for an artificial metadata variable.
  static DebugTypeInfo getTypeMetadata(nextcode::Type Ty, llvm::Type *StorageTy,
                                       Size size, Alignment align);

  /// Create a forward declaration for a type whose size is unknown.
  static DebugTypeInfo getForwardDecl(nextcode::Type Ty);

  /// Create a standalone type from a TypeInfo object.
  static DebugTypeInfo getFromTypeInfo(nextcode::Type Ty, const TypeInfo &Info,
                                       IRGenModule &IGM);
  /// Global variables.
  static DebugTypeInfo getGlobal(SILGlobalVariable *GV,
                                 llvm::Type *StorageType, IRGenModule &IGM);
  static DebugTypeInfo getGlobalFixedBuffer(SILGlobalVariable *GV,
                                            llvm::Type *StorageType,
                                            Size SizeInBytes, Alignment align);
  /// ObjC classes.
  static DebugTypeInfo getObjCClass(ClassDecl *theClass,
                                    llvm::Type *StorageType, Size size,
                                    Alignment align);
  /// Error type.
  static DebugTypeInfo getErrorResult(nextcode::Type Ty, IRGenModule &IGM);

  TypeBase *getType() const { return Type; }

  TypeDecl *getDecl() const;

  // Determine whether this type is an Archetype dependent on a generic context.
  bool isContextArchetype() const {
    if (auto archetype =
            Type->getWithoutSpecifierType()->getAs<ArchetypeType>()) {
      return !isa<OpaqueTypeArchetypeType>(archetype);
    }
    return false;
  }

  llvm::Type *getFragmentStorageType() const { return FragmentStorageType; }
  Alignment getAlignment() const { return Align; }
  bool isNull() const { return Type == nullptr; }
  bool isForwardDecl() const { return FragmentStorageType == nullptr; }
  bool isMetadataType() const { return IsMetadataType; }
  bool hasDefaultAlignment() const { return DefaultAlignment; }
  bool isFixedBuffer() const { return IsFixedBuffer; }
  std::optional<uint32_t> getNumExtraInhabitants() const {
    return NumExtraInhabitants;
  }

  bool operator==(DebugTypeInfo T) const;
  bool operator!=(DebugTypeInfo T) const;
#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
  LLVM_DUMP_METHOD void dump() const;
#endif
};

/// A DebugTypeInfo with a defined size (that may be 0).
class CompletedDebugTypeInfo : public DebugTypeInfo {
  Size::int_type SizeInBits;

  CompletedDebugTypeInfo(DebugTypeInfo DbgTy, Size::int_type SizeInBits)
    : DebugTypeInfo(DbgTy), SizeInBits(SizeInBits) {}

public:
  static std::optional<CompletedDebugTypeInfo>
  get(DebugTypeInfo DbgTy, std::optional<Size::int_type> SizeInBits) {
    if (!SizeInBits)
      return {};
    return CompletedDebugTypeInfo(DbgTy, *SizeInBits);
  }

  static std::optional<CompletedDebugTypeInfo>
  getFromTypeInfo(nextcode::Type Ty, const TypeInfo &Info, IRGenModule &IGM);

  Size::int_type getSizeInBits() const { return SizeInBits; }
};

}
}

namespace llvm {

// Dense map specialization.
template <> struct DenseMapInfo<nextcode::irgen::DebugTypeInfo> {
  static nextcode::irgen::DebugTypeInfo getEmptyKey() {
    return {};
  }
  static nextcode::irgen::DebugTypeInfo getTombstoneKey() {
    return nextcode::irgen::DebugTypeInfo(
        llvm::DenseMapInfo<nextcode::TypeBase *>::getTombstoneKey(), nullptr,
        nextcode::irgen::Alignment(), /* HasDefaultAlignment = */ false);
  }
  static unsigned getHashValue(nextcode::irgen::DebugTypeInfo Val) {
    return DenseMapInfo<nextcode::CanType>::getHashValue(Val.getType());
  }
  static bool isEqual(nextcode::irgen::DebugTypeInfo LHS,
                      nextcode::irgen::DebugTypeInfo RHS) {
    return LHS == RHS;
  }
};
}

#endif
