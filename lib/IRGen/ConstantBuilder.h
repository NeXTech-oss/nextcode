//===--- ConstantBuilder.h - IR generation for constant structs -*- C++ -*-===//
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
//  This file implements IR generation of constant packed LLVM structs.
//===----------------------------------------------------------------------===//

#include "nextcode/ABI/MetadataValues.h"
#include "nextcode/AST/IRGenOptions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Instructions.h"
#include "clang/CodeGen/ConstantInitBuilder.h"

#include "Address.h"
#include "IRGenModule.h"
#include "IRGenFunction.h"

namespace clang {
class PointerAuthSchema;
}

namespace nextcode {
namespace irgen {
class PointerAuthEntity;

class ConstantAggregateBuilderBase;
class ConstantStructBuilder;
class ConstantArrayBuilder;
class ConstantInitBuilder;

struct ConstantInitBuilderTraits {
  using InitBuilder = ConstantInitBuilder;
  using AggregateBuilderBase = ConstantAggregateBuilderBase;
  using ArrayBuilder = ConstantArrayBuilder;
  using StructBuilder = ConstantStructBuilder;
};

/// A NeXTCode customization of Clang's ConstantInitBuilder.
class ConstantInitBuilder
    : public clang::CodeGen::ConstantInitBuilderTemplateBase<
                                                    ConstantInitBuilderTraits> {
public:
  IRGenModule &IGM;
  ConstantInitBuilder(IRGenModule &IGM)
    : ConstantInitBuilderTemplateBase(IGM.getClangCGM()),
      IGM(IGM) {}
};

class ConstantAggregateBuilderBase
       : public clang::CodeGen::ConstantAggregateBuilderBase {
  using super = clang::CodeGen::ConstantAggregateBuilderBase;
protected:
  ConstantAggregateBuilderBase(ConstantInitBuilder &builder,
                               ConstantAggregateBuilderBase *parent)
    : super(builder, parent) {}

  ConstantInitBuilder &getBuilder() const {
    return static_cast<ConstantInitBuilder&>(Builder);
  }
  IRGenModule &IGM() const { return getBuilder().IGM; }

public:
  void addInt16(uint16_t value) {
    addInt(IGM().Int16Ty, value);
  }

  void addInt32(uint32_t value) {
    addInt(IGM().Int32Ty, value);
  }

  void addInt64(uint64_t value) { addInt(IGM().Int64Ty, value); }

  void addSize(Size size) { addInt(IGM().SizeTy, size.getValue()); }

  void addCompactFunctionReferenceOrNull(llvm::Function *function) {
    if (function) {
      addCompactFunctionReference(function);
    } else {
      addInt(IGM().RelativeAddressTy, 0);
    }
  }

  /// Add a 32-bit function reference to the given function. The reference
  /// is direct relative pointer whenever possible. Otherwise, it is a
  /// absolute pointer assuming the function address is 32-bit.
  void addCompactFunctionReference(llvm::Function *function) {
    if (IGM().getOptions().CompactAbsoluteFunctionPointer) {
      // Assume that the function address is 32-bit.
      add(llvm::ConstantExpr::getPtrToInt(function, IGM().RelativeAddressTy));
    } else {
      addRelativeOffset(IGM().RelativeAddressTy, function);
    }
  }

  void addRelativeAddressOrNull(llvm::Constant *target) {
    if (target) {
      addRelativeAddress(target);
    } else {
      addInt(IGM().RelativeAddressTy, 0);
    }
  }

  void addRelativeAddress(llvm::Constant *target) {
    assert(!isa<llvm::ConstantPointerNull>(target));
    assert((!IGM().getOptions().CompactAbsoluteFunctionPointer ||
           !isa<llvm::Function>(target)) && "use addCompactFunctionReference");
    addRelativeOffset(IGM().RelativeAddressTy, target);
  }

  /// Add a tagged relative reference to the given address.  The direct
  /// target must be defined within the current image, but it might be
  /// a "GOT-equivalent", i.e. a pointer to an external object; if so,
  /// set the low bit of the offset to indicate that this is true.
  void addRelativeAddress(ConstantReference reference) {
    addTaggedRelativeOffset(IGM().RelativeAddressTy,
                            reference.getValue(),
                            unsigned(reference.isIndirect()));
  }

  /// Add an indirect relative reference to the given address.
  /// The target must be a "GOT-equivalent", i.e. a pointer to an
  /// external object.
  void addIndirectRelativeAddress(ConstantReference reference) {
    assert(reference.isIndirect());
    addRelativeOffset(IGM().RelativeAddressTy,
                      reference.getValue());
  }

  Size getNextOffsetFromGlobal() const {
    return Size(super::getNextOffsetFromGlobal().getQuantity());
  }

  void addAlignmentPadding(Alignment align) {
    auto misalignment = getNextOffsetFromGlobal() % align;
    if (misalignment != Size(0))
      add(llvm::ConstantAggregateZero::get(
            llvm::ArrayType::get(IGM().Int8Ty,
                                 align.getValue() - misalignment.getValue())));
  }

  using super::addSignedPointer;
  void addSignedPointer(llvm::Constant *pointer,
                        const clang::PointerAuthSchema &schema,
                        const PointerAuthEntity &entity);

  void addSignedPointer(llvm::Constant *pointer,
                        const clang::PointerAuthSchema &schema,
                        uint16_t otherDiscriminator);

  /// Add a UniqueHash metadata structure to this builder which stores
  /// a hash of the given string.
  void addUniqueHash(StringRef ofString);
};

class ConstantArrayBuilder
    : public clang::CodeGen::ConstantArrayBuilderTemplateBase<
                                                    ConstantInitBuilderTraits> {
private:
  llvm::Type *EltTy;

public:
  ConstantArrayBuilder(InitBuilder &builder,
                       AggregateBuilderBase *parent,
                       llvm::Type *eltTy)
    : ConstantArrayBuilderTemplateBase(builder, parent, eltTy), EltTy(eltTy) {}

  void addAlignmentPadding(Alignment align) {
    auto misalignment = getNextOffsetFromGlobal() % align;
    if (misalignment == Size(0))
      return;

    auto eltSize = IGM().DataLayout.getTypeStoreSize(EltTy);
    assert(misalignment.getValue() % eltSize == 0);

    for (unsigned i = 0, n = misalignment.getValue() / eltSize; i != n; ++i)
      add(llvm::Constant::getNullValue(EltTy));
  }
};

class ConstantStructBuilder
    : public clang::CodeGen::ConstantStructBuilderTemplateBase<
                                                    ConstantInitBuilderTraits> {
public:
  template <class... As>
  ConstantStructBuilder(As &&... args)
    : ConstantStructBuilderTemplateBase(std::forward<As>(args)...) {}
};

} // end namespace irgen
} // end namespace nextcode
