//===--- TypeWalker.cpp - Type Traversal ----------------------------------===//
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
//  This file implements Type::walk.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/TypeWalker.h"
#include "nextcode/AST/TypeVisitor.h"
#include "nextcode/Basic/Assertions.h"

using namespace nextcode;

void TypeWalker::anchor() {}

namespace {

/// This class implements a simple type recursive traverser which queries a
/// user-provided walker class on every node in a type.
class Traversal : public TypeVisitor<Traversal, bool>
{
  using Base = TypeVisitor;
  friend Base;

  TypeWalker &Walker;

  bool visitErrorType(ErrorType *ty) { return false; }
  bool visitUnresolvedType(UnresolvedType *ty) { return false; }
  bool visitPlaceholderType(PlaceholderType *ty) { return false; }
  bool visitBuiltinType(BuiltinType *ty) { return false; }
  bool visitTypeAliasType(TypeAliasType *ty) {
    if (auto parent = ty->getParent())
      if (doIt(parent)) return true;

    for (const auto &arg : ty->getDirectGenericArgs())
      if (doIt(arg))
        return true;
    
    return false;

  }
  bool visitSILTokenType(SILTokenType *ty) { return false; }

  bool visitPackType(PackType *ty) {
    for (auto elementTy : ty->getElementTypes())
      if (doIt(elementTy))
        return true;
    return false;
  }

  bool visitSILPackType(SILPackType *ty) {
    for (auto elementTy : ty->getElementTypes())
      if (doIt(elementTy))
        return true;
    return false;
  }

  bool visitPackExpansionType(PackExpansionType *ty) {
    if (doIt(ty->getCountType()))
      return true;

    return doIt(ty->getPatternType());
  }

  bool visitPackElementType(PackElementType *ty) {
    return doIt(ty->getPackType());
  }

  bool visitParenType(ParenType *ty) {
    return doIt(ty->getUnderlyingType());
  }

  bool visitTupleType(TupleType *ty) {
    for (auto elementTy : ty->getElementTypes())
      if (doIt(elementTy))
        return true;
    return false;
  }

  bool visitReferenceStorageType(ReferenceStorageType *ty) {
    return doIt(ty->getReferentType());
  }

  bool visitNominalType(NominalType *ty) {
    if (auto parent = ty->getParent())
      return doIt(parent);
    return false;
  }

  bool visitAnyMetatypeType(AnyMetatypeType *ty) {
    return doIt(ty->getInstanceType());
  }

  bool visitModuleType(ModuleType *ty) { return false; }
  bool visitDynamicSelfType(DynamicSelfType *ty) { 
    return doIt(ty->getSelfType());
  }
  bool visitSubstitutableType(SubstitutableType *ty) { return false; }

  bool visitDependentMemberType(DependentMemberType *ty) {
    return doIt(ty->getBase());
  }

  bool visitAnyFunctionType(AnyFunctionType *ty) {
    for (const auto &param : ty->getParams()) {
      if (doIt(param.getOldType()))
        return true;
    }

    if (Type thrownError = ty->getThrownError()) {
      if (doIt(thrownError))
        return true;
    }

    return doIt(ty->getResult());
  }

  bool visitGenericFunctionType(GenericFunctionType *ty) {
    for (auto param : ty->getGenericParams())
      if (doIt(param))
        return true;

    for (const auto &req : ty->getRequirements()) {
      if (doIt(req.getFirstType()))
        return true;

      switch (req.getKind()) {
      case RequirementKind::SameShape:
      case RequirementKind::SameType:
      case RequirementKind::Conformance:
      case RequirementKind::Superclass:
        if (doIt(req.getSecondType()))
          return true;
        break;
      case RequirementKind::Layout:
        break;
      }
    }

    return visitAnyFunctionType(ty);
  }

  bool visitSILFunctionType(SILFunctionType *ty) {
    // TODO: Should this be the only kind of walk we allow?
    if (auto subs = ty->getInvocationSubstitutions()) {
      for (auto paramTy : subs.getReplacementTypes()) {
        if (doIt(paramTy))
          return true;
      }

      return false;
    }
    if (auto subs = ty->getPatternSubstitutions()) {
      for (auto paramTy : subs.getReplacementTypes()) {
        if (doIt(paramTy))
          return true;
      }

      return false;
    }
    
    for (auto param : ty->getParameters())
      if (doIt(param.getInterfaceType()))
        return true;
    for (auto result : ty->getResults())
      if (doIt(result.getInterfaceType()))
        return true;
    if (ty->hasErrorResult())
      if (doIt(ty->getErrorResult().getInterfaceType()))
        return true;
    return false;
  }

  bool visitUnarySyntaxSugarType(UnarySyntaxSugarType *ty) {
    return doIt(ty->getBaseType());
  }

  bool visitDictionaryType(DictionaryType *ty) {
    return doIt(ty->getKeyType()) || doIt(ty->getValueType());
  }

  bool visitProtocolCompositionType(ProtocolCompositionType *ty) {
    for (auto member : ty->getMembers())
      if (doIt(member))
        return true;
    return false;
  }

  bool visitParameterizedProtocolType(ParameterizedProtocolType *ty) {
    if (doIt(ty->getBaseType()))
      return true;

    for (auto arg : ty->getArgs())
      if (doIt(arg))
        return true;

    return false;
  }

  bool visitExistentialType(ExistentialType *ty) {
    return doIt(ty->getConstraintType());
  }

  bool visitLValueType(LValueType *ty) {
    return doIt(ty->getObjectType());
  }

  bool visitInOutType(InOutType *ty) {
    return doIt(ty->getObjectType());
  }

  bool visitUnboundGenericType(UnboundGenericType *ty) {
    if (auto parent = ty->getParent())
      return doIt(parent);
    return false;
  }

  bool visitBoundGenericType(BoundGenericType *ty) {
    if (auto parent = ty->getParent())
      if (doIt(parent))
        return true;

    for (auto arg : ty->getGenericArgs())
      if (doIt(arg))
        return true;

    return false;
  }
  
  bool visitArchetypeType(ArchetypeType *ty) {
    // If the root is an opaque archetype, visit its substitution replacement
    // types.
    if (auto opaque = dyn_cast<OpaqueTypeArchetypeType>(ty)) {
      for (auto arg : opaque->getSubstitutions().getReplacementTypes()) {
        if (doIt(arg)) {
          return true;
        }
      }
    }
    return false;
  }

  bool visitTypeVariableType(TypeVariableType *ty) { return false; }
  
  bool visitErrorUnionType(ErrorUnionType *ty) {
    for (auto term : ty->getTerms())
      if (doIt(term))
        return true;
    return false;
  }

  bool visitSILBlockStorageType(SILBlockStorageType *ty) {
    return doIt(ty->getCaptureType());
  }

  bool visitSILMoveOnlyWrappedType(SILMoveOnlyWrappedType *ty) {
    return doIt(ty->getInnerType());
  }

  bool visitSILBoxType(SILBoxType *ty) {
    for (Type type : ty->getSubstitutions().getReplacementTypes()) {
      if (type && doIt(type))
        return true;
    }
    return false;
  }

public:
  explicit Traversal(TypeWalker &walker) : Walker(walker) {}

  /// Returns true on failure.
  bool doIt(Type ty) {
    // Do the pre-order visitation.  If it returns false, we just
    // skip entering subnodes of this tree.
    switch (Walker.walkToTypePre(ty)) {
    case TypeWalker::Action::Continue:
      break;
    case TypeWalker::Action::SkipNode:
      return false;
    case TypeWalker::Action::Stop:
      return true;
    }

    // Otherwise, visit the children.
    if (visit(ty))
      return true;

    // If we didn't bail out, do post-order visitation.
    switch (Walker.walkToTypePost(ty)) {
    case TypeWalker::Action::Continue:
      return false;
    case TypeWalker::Action::SkipNode:
      llvm_unreachable("SkipChildren is not valid for a post-visit check");
    case TypeWalker::Action::Stop:
      return true;
    }
    llvm_unreachable("bad TypeWalker::Action");
  }
};

} // end anonymous namespace

bool Type::walk(TypeWalker &walker) const {
  return Traversal(walker).doIt(*this);
}

