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
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ASTPrinter.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/NameLookup.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/SourceManager.h"
#include "nextcode/Frontend/Frontend.h"
#include "nextcode/Sema/ConstraintSystem.h"
#include "nextcode/Sema/IDETypeCheckingRequests.h"
#include "nextcode/Subsystems.h"
#include "TypeChecker.h"

using namespace nextcode;

namespace nextcode {
// Implement the IDE type zone.
#define NEXTCODE_TYPEID_ZONE IDETypeChecking
#define NEXTCODE_TYPEID_HEADER "nextcode/Sema/IDETypeCheckingRequestIDZone.def"
#include "nextcode/Basic/ImplementTypeIDZone.h"
#undef NEXTCODE_TYPEID_ZONE
#undef NEXTCODE_TYPEID_HEADER
}

// Define request evaluation functions for each of the IDE type check requests.
static AbstractRequestFunction *ideTypeCheckRequestFunctions[] = {
#define NEXTCODE_REQUEST(Zone, Name, Sig, Caching, LocOptions)                    \
reinterpret_cast<AbstractRequestFunction *>(&Name::evaluateRequest),
#include "nextcode/Sema/IDETypeCheckingRequestIDZone.def"
#undef NEXTCODE_REQUEST
};

void nextcode::registerIDETypeCheckRequestFunctions(Evaluator &evaluator) {
  evaluator.registerRequestFunctions(Zone::IDETypeChecking,
                                     ideTypeCheckRequestFunctions);
}

/// Consider the following example
///
/// \code
/// protocol FontStyle {}
/// struct FontStyleOne: FontStyle {}
/// extension FontStyle where Self == FontStyleOne {
///     static var one: FontStyleOne { FontStyleOne() }
/// }
/// func foo<T: FontStyle>(x: T) {}
///
/// func case1() {
///     foo(x: .#^COMPLETE^#) // extension should be considered applied here
/// }
/// func case2<T: FontStyle>(x: T) {
///     x.#^COMPLETE_2^# // extension should not be considered applied here
/// }
/// \endcode
/// We want to consider the extension applied in the first case but not the
/// second case. In the first case the constraint `T: FontStyle` from the
/// definition of `foo` should be considered an 'at-least' constraint and any
/// additional constraints on `T` (like `T == FontStyleOne`) can be
/// fulfilled by picking a more specialized version of `T`.
/// However, in the second case, `T: FontStyle` should be considered an
/// 'at-most' constraint and we can't make the assumption that `x` has a more
/// specialized type.
///
/// After type-checking we cannot easily differentiate the two cases. In both
/// we have a unresolved dot completion on a primary archetype that
/// conforms to `FontStyle`.
///
/// To tell them apart, we apply the following heuristic: If the primary
/// archetype refers to a generic parameter that is not visible in the current
/// decl context (i.e. the current decl context is not a child context of the
/// parameter's decl context), it is not the type of a variable visible
/// in the current decl context. Hence, we must be in the first case and
/// consider all extensions applied, otherwise we should only consider those
/// extensions applied whose requirements are fulfilled.
class ContainsSpecializableArchetype : public TypeWalker {
  const DeclContext *DC;
  bool Result = false;
  ContainsSpecializableArchetype(const DeclContext *DC) : DC(DC) {}

  Action walkToTypePre(Type T) override {
    if (auto *Archetype = T->getAs<ArchetypeType>()) {
      if (auto *GenericTypeParam =
              Archetype->mapTypeOutOfContext()->getAs<GenericTypeParamType>()) {
        if (auto GenericTypeParamDecl = GenericTypeParam->getDecl()) {
          bool ParamMaybeVisibleInCurrentContext =
              (DC == GenericTypeParamDecl->getDeclContext() ||
               DC->isChildContextOf(GenericTypeParamDecl->getDeclContext()));
          if (!ParamMaybeVisibleInCurrentContext) {
            Result = true;
            return Action::Stop;
          }
        }
      }
    }
    return Action::Continue;
  }

public:
  static bool check(const DeclContext *DC, Type T) {
    if (!T->hasArchetype()) {
      // Fast path, we don't have an archetype to check.
      return false;
    }
    ContainsSpecializableArchetype Checker(DC);
    T.walk(Checker);
    return Checker.Result;
  }
};

/// Returns `true` if `ED` is an extension that binds `Self` to a
/// concrete type, like `extension MyProto where Self == MyStruct {}`. The
/// protocol being extended must either be `PD`, or `Self` must be a type
/// that conforms to `PD`.
///
/// In these cases, it is possible to access static members defined in the
/// extension when perfoming unresolved member lookup in a type context of
/// `PD`.
static bool isExtensionWithSelfBound(const ExtensionDecl *ED,
                                     ProtocolDecl *PD) {
  if (!ED || !PD)
    return false;

  GenericSignature genericSig = ED->getGenericSignature();
  Type selfType = genericSig->getConcreteType(ED->getSelfInterfaceType());
  if (!selfType)
    return false;

  if (selfType->is<ExistentialType>())
    return false;

  auto *M = ED->getParentModule();
  return ED->getExtendedNominal() == PD || M->checkConformance(selfType, PD);
}

static bool isExtensionAppliedInternal(const DeclContext *DC, Type BaseTy,
                                       const ExtensionDecl *ED) {
  // We can't do anything if the base type has unbound generic parameters.
  // We can't leak type variables into another constraint system.
  // For check on specializable archetype see comment on
  // ContainsSpecializableArchetype.
  if (BaseTy->hasTypeVariable() || BaseTy->hasUnboundGenericType() ||
      BaseTy->hasUnresolvedType() || BaseTy->hasError() ||
      ContainsSpecializableArchetype::check(DC, BaseTy))
    return true;

  if (!ED->isConstrainedExtension())
    return true;

  ProtocolDecl *BaseTypeProtocolDecl = nullptr;
  if (auto opaqueType = dyn_cast<OpaqueTypeArchetypeType>(BaseTy)) {
    if (opaqueType->getConformsTo().size() == 1) {
      BaseTypeProtocolDecl = opaqueType->getConformsTo().front();
    }
  } else {
    BaseTypeProtocolDecl = dyn_cast_or_null<ProtocolDecl>(BaseTy->getAnyNominal());
  }

  if (isExtensionWithSelfBound(ED, BaseTypeProtocolDecl)) {
    return true;
  }
  auto *module = DC->getParentModule();
  GenericSignature genericSig = ED->getGenericSignature();
  SubstitutionMap substMap = BaseTy->getContextSubstitutionMap(
      module, ED->getExtendedNominal());
  return checkRequirements(module,
                           genericSig.getRequirements(),
                           QuerySubstitutionMap{substMap}) ==
         CheckRequirementsResult::Success;
}

static bool isMemberDeclAppliedInternal(const DeclContext *DC, Type BaseTy,
                                        const ValueDecl *VD) {
  if (BaseTy->isExistentialType() && VD->isStatic() &&
      !isExtensionWithSelfBound(
          dyn_cast<ExtensionDecl>(VD->getDeclContext()),
          dyn_cast_or_null<ProtocolDecl>(BaseTy->getAnyNominal())))
    return false;

  // We can't leak type variables into another constraint system.
  // We can't do anything if the base type has unbound generic parameters.
  if (BaseTy->hasTypeVariable() || BaseTy->hasUnboundGenericType()||
      BaseTy->hasUnresolvedType() || BaseTy->hasError())
    return true;

  if (isa<TypeAliasDecl>(VD) && BaseTy->is<ProtocolType>()) {
    // The protocol doesn't satisfy its own generic signature (static members
    // of the protocol are not visible on the protocol itself) but we can still
    // access typealias declarations on it.
    return true;
  }

  const GenericContext *genericDecl = VD->getAsGenericContext();
  if (!genericDecl)
    return true;

  // The declaration may introduce inner generic parameters and requirements,
  // or it may be nested in an outer generic context.
  GenericSignature genericSig = genericDecl->getGenericSignature();
  if (!genericSig)
    return true;

  // The context substitution map for the base type fixes the declaration's
  // outer generic parameters.
  auto *module = DC->getParentModule();
  auto substMap = BaseTy->getContextSubstitutionMap(
      module, VD->getDeclContext(), genericDecl->getGenericEnvironment());

  // The innermost generic parameters are mapped to error types.
  unsigned innerDepth = genericSig->getMaxDepth();
  if (!genericDecl->isGeneric())
    ++innerDepth;

  // We treat substitution failure as success, to ignore requirements
  // that involve innermost generic parameters.
  return checkRequirements(module,
                           genericSig.getRequirements(),
                           [&](SubstitutableType *type) -> Type {
                             auto *paramTy = cast<GenericTypeParamType>(type);
                             if (paramTy->getDepth() == innerDepth)
                               return ErrorType::get(DC->getASTContext());
                             return Type(paramTy).subst(substMap);
                           }) != CheckRequirementsResult::RequirementFailure;
}

bool
IsDeclApplicableRequest::evaluate(Evaluator &evaluator,
                                  DeclApplicabilityOwner Owner) const {
  if (auto *VD = dyn_cast<ValueDecl>(Owner.ExtensionOrMember)) {
    return isMemberDeclAppliedInternal(Owner.DC, Owner.Ty, VD);
  } else if (auto *ED = dyn_cast<ExtensionDecl>(Owner.ExtensionOrMember)) {
    return isExtensionAppliedInternal(Owner.DC, Owner.Ty, ED);
  } else {
    llvm_unreachable("unhandled decl kind");
  }
}

bool
TypeRelationCheckRequest::evaluate(Evaluator &evaluator,
                                   TypeRelationCheckInput Owner) const {
  using namespace constraints;
  std::optional<ConstraintKind> CKind;
  switch (Owner.Relation) {
  case TypeRelation::ConvertTo:
    CKind = ConstraintKind::Conversion;
    break;
  case TypeRelation::SubtypeOf:
    CKind = ConstraintKind::Subtype;
    break;
  }
  assert(CKind.has_value());
  return TypeChecker::typesSatisfyConstraint(Owner.Pair.FirstTy,
                                             Owner.Pair.SecondTy,
                                             Owner.OpenArchetypes,
                                             *CKind, Owner.DC);
}

TypePair
RootAndResultTypeOfKeypathDynamicMemberRequest::evaluate(Evaluator &evaluator,
                                              SubscriptDecl *subscript) const {
  if (!isValidKeyPathDynamicMemberLookup(subscript))
    return TypePair();

  const auto *param = subscript->getIndices()->get(0);
  auto keyPathType = param->getTypeInContext()->getAs<BoundGenericType>();
  if (!keyPathType)
    return TypePair();
  auto genericArgs = keyPathType->getGenericArgs();
  assert(!genericArgs.empty() && genericArgs.size() == 2 &&
         "invalid keypath dynamic member");
  return TypePair(genericArgs[0], genericArgs[1]);
}
