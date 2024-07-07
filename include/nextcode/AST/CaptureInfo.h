//===--- CaptureInfo.h - Data Structure for Capture Lists -------*- C++ -*-===//
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

#ifndef NEXTCODE_AST_CAPTURE_INFO_H
#define NEXTCODE_AST_CAPTURE_INFO_H

#include "nextcode/Basic/Debug.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/Basic/OptionSet.h"
#include "nextcode/Basic/SourceLoc.h"
#include "nextcode/AST/TypeAlignments.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/PointerIntPair.h"
#include "llvm/ADT/PointerUnion.h"
#include "llvm/Support/TrailingObjects.h"

namespace nextcode {
class CapturedValue;
} // namespace nextcode

namespace nextcode {
namespace Lowering {
class TypeConverter;
} // namespace Lowering
} // namespace nextcode

namespace llvm {
class raw_ostream;
template <> struct DenseMapInfo<nextcode::CapturedValue>;
} // namespace llvm

namespace nextcode {
class ValueDecl;
class FuncDecl;
class Expr;
class OpaqueValueExpr;
class PackElementExpr;
class VarDecl;
class GenericEnvironment;
class Type;

/// CapturedValue includes both the declaration being captured, along with flags
/// that indicate how it is captured.
class CapturedValue {
  friend class Lowering::TypeConverter;

public:
  using Storage =
      llvm::PointerIntPair<llvm::PointerUnion<ValueDecl *, Expr *>, 2,
                           unsigned>;

private:
  Storage Value;
  SourceLoc Loc;

  explicit CapturedValue(Storage V, SourceLoc Loc) : Value(V), Loc(Loc) {}

public:
  friend struct llvm::DenseMapInfo<CapturedValue>;

  enum {
    /// IsDirect is set when a VarDecl with storage *and* accessors is captured
    /// by its storage address.  This happens in the accessors for the VarDecl.
    IsDirect = 1 << 0,

    /// IsNoEscape is set when a vardecl is captured by a noescape closure, and
    /// thus has its lifetime guaranteed.  It can be closed over by a fixed
    /// address if it has storage.
    IsNoEscape = 1 << 1
  };

  CapturedValue(ValueDecl *Val, unsigned Flags, SourceLoc Loc)
      : Value(Val, Flags), Loc(Loc) {}

  CapturedValue(Expr *Val, unsigned Flags);

public:
  static CapturedValue getDynamicSelfMetadata() {
    return CapturedValue((ValueDecl *)nullptr, 0, SourceLoc());
  }

  bool isDirect() const { return Value.getInt() & IsDirect; }
  bool isNoEscape() const { return Value.getInt() & IsNoEscape; }

  bool isDynamicSelfMetadata() const { return !Value.getPointer(); }

  bool isExpr() const {
    return Value.getPointer().dyn_cast<Expr *>();
  }

  bool isPackElement() const;
  bool isOpaqueValue() const;

  /// Returns true if this captured value is a local capture.
  ///
  /// NOTE: This implies that the value is not dynamic self metadata, since
  /// values with decls are the only values that are able to be local captures.
  bool isLocalCapture() const;

  CapturedValue mergeFlags(unsigned flags) const {
    return CapturedValue(Storage(Value.getPointer(), getFlags() & flags), Loc);
  }

  ValueDecl *getDecl() const {
    return Value.getPointer().dyn_cast<ValueDecl *>();
  }

  Expr *getExpr() const {
    return Value.getPointer().dyn_cast<Expr *>();
  }

  OpaqueValueExpr *getOpaqueValue() const;

  PackElementExpr *getPackElement() const;

  Type getPackElementType() const;

  SourceLoc getLoc() const { return Loc; }

  unsigned getFlags() const { return Value.getInt(); }
};

} // end nextcode namespace

namespace nextcode {

class DynamicSelfType;

/// Stores information about captured variables.
class CaptureInfo {
  class CaptureInfoStorage final
      : public llvm::TrailingObjects<CaptureInfoStorage,
                                     CapturedValue,
                                     GenericEnvironment *> {

    DynamicSelfType *DynamicSelf;
    OpaqueValueExpr *OpaqueValue;
    unsigned NumCapturedValues;
    unsigned NumGenericEnvironments;

  public:
    explicit CaptureInfoStorage(DynamicSelfType *dynamicSelf,
                                OpaqueValueExpr *opaqueValue,
                                unsigned numCapturedValues,
                                unsigned numGenericEnvironments)
      : DynamicSelf(dynamicSelf), OpaqueValue(opaqueValue),
        NumCapturedValues(numCapturedValues),
        NumGenericEnvironments(numGenericEnvironments) { }

    ArrayRef<CapturedValue> getCaptures() const;

    ArrayRef<GenericEnvironment *> getGenericEnvironments() const;

    DynamicSelfType *getDynamicSelfType() const {
      return DynamicSelf;
    }

    OpaqueValueExpr *getOpaqueValue() const {
      return OpaqueValue;
    }

    unsigned numTrailingObjects(OverloadToken<CapturedValue>) const {
      return NumCapturedValues;
    }
  };

  enum class Flags : unsigned {
    HasGenericParamCaptures = 1 << 0
  };

  llvm::PointerIntPair<const CaptureInfoStorage *, 2, OptionSet<Flags>>
      StorageAndFlags;

public:
  /// The default-constructed CaptureInfo is "not yet computed".
  CaptureInfo() = default;
  CaptureInfo(ASTContext &ctx,
              ArrayRef<CapturedValue> captures,
              DynamicSelfType *dynamicSelf, OpaqueValueExpr *opaqueValue,
              bool genericParamCaptures,
              ArrayRef<GenericEnvironment *> genericEnv=ArrayRef<GenericEnvironment*>());

  /// A CaptureInfo representing no captures at all.
  static CaptureInfo empty();

  bool hasBeenComputed() const {
    return StorageAndFlags.getPointer();
  }

  bool isTrivial() const {
    assert(hasBeenComputed());
    return getCaptures().empty() && !hasGenericParamCaptures() &&
           !hasDynamicSelfCapture() && !hasOpaqueValueCapture();
  }

  /// Returns all captured values and opaque expressions.
  ArrayRef<CapturedValue> getCaptures() const {
    assert(hasBeenComputed());
    return StorageAndFlags.getPointer()->getCaptures();
  }

  /// Returns all captured pack element environments.
  ArrayRef<GenericEnvironment *> getGenericEnvironments() const {
    assert(hasBeenComputed());
    return StorageAndFlags.getPointer()->getGenericEnvironments();
  }

  /// \returns true if the function captures the primary generic environment
  /// from its innermost declaration context.
  bool hasGenericParamCaptures() const {
    assert(hasBeenComputed());
    return StorageAndFlags.getInt().contains(Flags::HasGenericParamCaptures);
  }

  /// \returns true if the function captures the dynamic Self type.
  bool hasDynamicSelfCapture() const {
    return getDynamicSelfType() != nullptr;
  }

  /// \returns the captured dynamic Self type, if any.
  DynamicSelfType *getDynamicSelfType() const {
    assert(hasBeenComputed());
    return StorageAndFlags.getPointer()->getDynamicSelfType();
  }

  bool hasOpaqueValueCapture() const {
    assert(hasBeenComputed());
    return getOpaqueValue() != nullptr;
  }

  OpaqueValueExpr *getOpaqueValue() const {
    assert(hasBeenComputed());
    return StorageAndFlags.getPointer()->getOpaqueValue();
  }

  /// Retrieve the variable corresponding to an isolated parameter that has
  /// been captured, if there is one. This might be a capture variable
  /// that was initialized with an isolated parameter.
  VarDecl *getIsolatedParamCapture() const;

  NEXTCODE_DEBUG_DUMP;
  void print(raw_ostream &OS) const;
};

} // namespace nextcode

#endif // LLVM_NEXTCODE_AST_CAPTURE_INFO_H

