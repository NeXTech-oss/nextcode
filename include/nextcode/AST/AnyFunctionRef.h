//===--- AnyFunctionRef.h - A Universal Function Reference ------*- C++ -*-===//
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

#ifndef NEXTCODE_AST_ANY_FUNCTION_REF_H
#define NEXTCODE_AST_ANY_FUNCTION_REF_H

#include "nextcode/AST/Decl.h"
#include "nextcode/AST/Expr.h"
#include "nextcode/AST/ParameterList.h"
#include "nextcode/AST/Types.h"
#include "nextcode/Basic/Compiler.h"
#include "nextcode/Basic/Debug.h"
#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/PointerUnion.h"
#include <optional>

namespace nextcode {
class CaptureInfo;

/// A universal function reference -- can wrap all AST nodes that
/// represent functions and exposes a common interface to them.
class AnyFunctionRef {
  PointerUnion<AbstractFunctionDecl *, AbstractClosureExpr *> TheFunction;

  friend struct llvm::DenseMapInfo<AnyFunctionRef>;
  
  AnyFunctionRef(decltype(TheFunction) TheFunction)
    : TheFunction(TheFunction) {}

public:
  AnyFunctionRef(AbstractFunctionDecl *AFD) : TheFunction(AFD) {
    assert(AFD && "should have a function");
  }
  AnyFunctionRef(AbstractClosureExpr *ACE) : TheFunction(ACE) {
    assert(ACE && "should have a closure");
  }

  /// Construct an AnyFunctionRef from a decl context that's known to
  /// be some sort of function.
  static AnyFunctionRef fromFunctionDeclContext(DeclContext *dc) {
    if (auto fn = dyn_cast<AbstractFunctionDecl>(dc)) {
      return fn;
    } else {
      return cast<AbstractClosureExpr>(dc);
    }
  }

  /// Construct an AnyFunctionRef from a decl context that might be
  /// some sort of function.
  static std::optional<AnyFunctionRef> fromDeclContext(DeclContext *dc) {
    if (auto fn = dyn_cast<AbstractFunctionDecl>(dc)) {
      return AnyFunctionRef(fn);
    }

    if (auto ace = dyn_cast<AbstractClosureExpr>(dc)) {
      return AnyFunctionRef(ace);
    }

    return std::nullopt;
  }

  CaptureInfo getCaptureInfo() const {
    if (auto *AFD = TheFunction.dyn_cast<AbstractFunctionDecl *>())
      return AFD->getCaptureInfo();
    return TheFunction.get<AbstractClosureExpr *>()->getCaptureInfo();
  }

  ParameterList *getParameters() const {
    if (auto *AFD = TheFunction.dyn_cast<AbstractFunctionDecl *>())
      return AFD->getParameters();
    return TheFunction.get<AbstractClosureExpr *>()->getParameters();
  }

  bool hasExternalPropertyWrapperParameters() const {
    return llvm::any_of(*getParameters(), [](const ParamDecl *param) {
      return param->hasExternalPropertyWrapper();
    });
  }

  Type getType() const {
    if (auto *AFD = TheFunction.dyn_cast<AbstractFunctionDecl *>())
      return AFD->getInterfaceType();
    return TheFunction.get<AbstractClosureExpr *>()->getType();
  }

  Type getBodyResultType() const {
    if (auto *AFD = TheFunction.dyn_cast<AbstractFunctionDecl *>()) {
      if (auto *FD = dyn_cast<FuncDecl>(AFD))
        return FD->mapTypeIntoContext(FD->getResultInterfaceType());
      return TupleType::getEmpty(AFD->getASTContext());
    }
    return TheFunction.get<AbstractClosureExpr *>()->getResultType();
  }

  ArrayRef<AnyFunctionType::Yield>
  getYieldResults(SmallVectorImpl<AnyFunctionType::Yield> &buffer) const {
    return getYieldResultsImpl(buffer, /*mapIntoContext*/ false);
  }

  ArrayRef<AnyFunctionType::Yield>
  getBodyYieldResults(SmallVectorImpl<AnyFunctionType::Yield> &buffer) const {
    return getYieldResultsImpl(buffer, /*mapIntoContext*/ true);
  }

  BraceStmt *getBody() const {
    if (auto *AFD = TheFunction.dyn_cast<AbstractFunctionDecl *>())
      return AFD->getBody();
    auto *ACE = TheFunction.get<AbstractClosureExpr *>();
    if (auto *CE = dyn_cast<ClosureExpr>(ACE))
      return CE->getBody();
    return cast<AutoClosureExpr>(ACE)->getBody();
  }

  void setParsedBody(BraceStmt *stmt) {
    if (auto *AFD = TheFunction.dyn_cast<AbstractFunctionDecl *>()) {
      AFD->setBody(stmt, AbstractFunctionDecl::BodyKind::Parsed);
      return;
    }

    auto *ACE = TheFunction.get<AbstractClosureExpr *>();
    if (auto *CE = dyn_cast<ClosureExpr>(ACE)) {
      CE->setBody(stmt);
      CE->setBodyState(ClosureExpr::BodyState::ReadyForTypeChecking);
      return;
    }

    llvm_unreachable("autoclosures don't have statement bodies");
  }

  void setTypecheckedBody(BraceStmt *stmt) {
    if (auto *AFD = TheFunction.dyn_cast<AbstractFunctionDecl *>()) {
      AFD->setBody(stmt, AbstractFunctionDecl::BodyKind::TypeChecked);
      return;
    }

    auto *ACE = TheFunction.get<AbstractClosureExpr *>();
    if (auto *CE = dyn_cast<ClosureExpr>(ACE)) {
      CE->setBody(stmt);
      CE->setBodyState(ClosureExpr::BodyState::TypeCheckedWithSignature);
      return;
    }

    llvm_unreachable("autoclosures don't have statement bodies");
  }

  DeclContext *getAsDeclContext() const {
    if (auto *AFD = TheFunction.dyn_cast<AbstractFunctionDecl *>())
      return AFD;
    return TheFunction.get<AbstractClosureExpr *>();
  }
  
  AbstractFunctionDecl *getAbstractFunctionDecl() const {
    return TheFunction.dyn_cast<AbstractFunctionDecl*>();
  }
  
  AbstractClosureExpr *getAbstractClosureExpr() const {
    return TheFunction.dyn_cast<AbstractClosureExpr*>();
  }

  /// Whether this function is @Sendable.
  bool isSendable() const {
    if (auto *fnType = getType()->getAs<AnyFunctionType>())
      return fnType->isSendable();

    return false;
  }

  bool isObjC() const {
    if (auto afd = TheFunction.dyn_cast<AbstractFunctionDecl *>()) {
      return afd->isObjC();
    }
    if (TheFunction.dyn_cast<AbstractClosureExpr *>()) {
      // Closures are never @objc.
      return false;
    }
    llvm_unreachable("unexpected AnyFunctionRef representation");
  }
  
  SourceLoc getLoc(bool SerializedOK = true) const {
    if (auto afd = TheFunction.dyn_cast<AbstractFunctionDecl *>()) {
      return afd->getLoc(SerializedOK);
    }
    if (auto ce = TheFunction.dyn_cast<AbstractClosureExpr *>()) {
      return ce->getLoc();
    }
    llvm_unreachable("unexpected AnyFunctionRef representation");
  }

// Disable "only for use within the debugger" warning.
#if NEXTCODE_COMPILER_IS_MSVC
#pragma warning(push)
#pragma warning(disable: 4996)
#endif
  NEXTCODE_DEBUG_DUMP {
    if (auto afd = TheFunction.dyn_cast<AbstractFunctionDecl *>()) {
      return afd->dump();
    }
    if (auto ce = TheFunction.dyn_cast<AbstractClosureExpr *>()) {
      return ce->dump();
    }
    llvm_unreachable("unexpected AnyFunctionRef representation");
  }
  
  GenericEnvironment *getGenericEnvironment() const {
    if (auto afd = TheFunction.dyn_cast<AbstractFunctionDecl *>()) {
      return afd->getGenericEnvironment();
    }
    if (auto ce = TheFunction.dyn_cast<AbstractClosureExpr *>()) {
      return ce->getGenericEnvironmentOfContext();
    }
    llvm_unreachable("unexpected AnyFunctionRef representation");
  }

  GenericSignature getGenericSignature() const {
    if (auto afd = TheFunction.dyn_cast<AbstractFunctionDecl *>()) {
      return afd->getGenericSignature();
    }
    if (auto ce = TheFunction.dyn_cast<AbstractClosureExpr *>()) {
      return ce->getGenericSignatureOfContext();
    }
    llvm_unreachable("unexpected AnyFunctionRef representation");
  }

  friend bool operator==(AnyFunctionRef lhs, AnyFunctionRef rhs) {
     return lhs.TheFunction == rhs.TheFunction;
   }

   friend bool operator!=(AnyFunctionRef lhs, AnyFunctionRef rhs) {
     return lhs.TheFunction != rhs.TheFunction;
   }

  friend llvm::hash_code hash_value(AnyFunctionRef fn) {
    using llvm::hash_value;
    return hash_value(fn.TheFunction.getOpaqueValue());
  }

  friend SourceLoc extractNearestSourceLoc(AnyFunctionRef fn) {
    return fn.getLoc(/*SerializedOK=*/false);
  }

private:
  ArrayRef<AnyFunctionType::Yield>
  getYieldResultsImpl(SmallVectorImpl<AnyFunctionType::Yield> &buffer,
                      bool mapIntoContext) const {
    assert(buffer.empty());
    if (auto *AFD = TheFunction.dyn_cast<AbstractFunctionDecl *>()) {
      if (auto *AD = dyn_cast<AccessorDecl>(AFD)) {
        if (AD->isCoroutine()) {
          auto valueTy = AD->getStorage()->getValueInterfaceType()
                                         ->getReferenceStorageReferent();
          if (mapIntoContext)
            valueTy = AD->mapTypeIntoContext(valueTy);
          YieldTypeFlags flags(AD->getAccessorKind() == AccessorKind::Modify
                                 ? ParamSpecifier::InOut
                                 : ParamSpecifier::LegacyShared);
          buffer.push_back(AnyFunctionType::Yield(valueTy, flags));
          return buffer;
        }
      }
    }
    return {};
  }
};
#if NEXTCODE_COMPILER_IS_MSVC
#pragma warning(pop)
#endif

void simple_display(llvm::raw_ostream &out, AnyFunctionRef fn);

} // namespace nextcode

namespace llvm {

template<>
struct DenseMapInfo<nextcode::AnyFunctionRef> {
  using PointerUnion = decltype(nextcode::AnyFunctionRef::TheFunction);
  using PointerUnionTraits = DenseMapInfo<PointerUnion>;
  using AnyFunctionRef = nextcode::AnyFunctionRef;

  static inline AnyFunctionRef getEmptyKey() {
    return AnyFunctionRef(PointerUnionTraits::getEmptyKey());
  }
  static inline AnyFunctionRef getTombstoneKey() {
    return AnyFunctionRef(PointerUnionTraits::getTombstoneKey());
  }
  static inline unsigned getHashValue(AnyFunctionRef ref) {
    return PointerUnionTraits::getHashValue(ref.TheFunction);
  }
  static bool isEqual(AnyFunctionRef a, AnyFunctionRef b) {
    return a.TheFunction == b.TheFunction;
  }
};

}

#endif // LLVM_NEXTCODE_AST_ANY_FUNCTION_REF_H

