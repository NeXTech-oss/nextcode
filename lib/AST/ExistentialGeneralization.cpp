//===--- ExistentialGeneralization.cpp - Shape generalization algorithm ---===//
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
//  This file defines the existential type generalization algorithm,
//  which is used in the ABI for existential types.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/CanTypeVisitor.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/GenericSignature.h"
#include "nextcode/AST/Requirement.h"
#include "nextcode/AST/Types.h"
#include "nextcode/Basic/Assertions.h"
#include "llvm/ADT/DenseMap.h"

using namespace nextcode;

namespace {

/// A helper type for performing existential type generalization.
class Generalizer : public CanTypeVisitor<Generalizer, Type> {
  friend CanTypeVisitor<Generalizer, Type>;

  ASTContext &ctx;

  llvm::DenseMap<CanType, Type> substTypes;
  llvm::DenseMap<std::pair<CanType, ProtocolDecl*>,
                 ProtocolConformanceRef> substConformances;

  SmallVector<GenericTypeParamType *, 2> addedParameters;
  SmallVector<Requirement, 2> addedRequirements;

public:
  Generalizer(ASTContext &ctx) : ctx(ctx) {}

  /// Given that the given type is not itself substitutable in whatever
  /// position it appears in, generalize it.
  Type generalizeStructure(CanType type) {
    return visit(type);
  }

  SubstitutionMap getGeneralizationSubstitutions() {
    // If we never introduced a generalization parameter, we're done.
    if (addedParameters.empty() && addedRequirements.empty())
      return SubstitutionMap();

    // Finish the signature.
    auto sig = buildGenericSignature(ctx, GenericSignature(),
                                     addedParameters,
                                     addedRequirements,
                                     /*allowInverses=*/false);

    // TODO: minimize the signature by removing redundant generic
    // parameters.

    auto lookupParameter = [&](SubstitutableType *type) {
      auto it = substTypes.find(CanType(type));
      assert(it != substTypes.end());
      return it->second;
    };
    auto lookupConformance = [&](CanType dependentType,
                                 Type conformingReplacementType,
                                 ProtocolDecl *conformedProtocol) {
      auto it = substConformances.find({dependentType, conformedProtocol});
      assert(it != substConformances.end());
      return it->second;
    };
    return SubstitutionMap::get(sig, lookupParameter, lookupConformance);
  }

private:
  Type visitProtocolType(CanProtocolType type) {
    // Simple protocol types have no sub-structure.
    assert(!type.getParent());
    return type;
  }

  Type visitParameterizedProtocolType(CanParameterizedProtocolType origType) {
    // Generalize the argument types of parameterized protocols,
    // but don't generalize the base type.
    auto origArgs = origType.getArgs();
    SmallVector<Type, 4> newArgs;
    newArgs.reserve(origArgs.size());
    for (auto origArg: origArgs) {
      newArgs.push_back(generalizeComponentType(origArg));
    }
    return ParameterizedProtocolType::get(ctx, origType->getBaseType(),
                                          newArgs);
  }

  Type visitProtocolCompositionType(CanProtocolCompositionType origType) {
    // The member types of protocol compositions are not substitutable,
    // including class constraints.  Generalize them individually,
    // preserving structure.
    auto origMembers = origType.getMembers();
    SmallVector<Type, 4> newMembers;
    newMembers.reserve(origMembers.size());
    for (auto origMember: origMembers) {
      newMembers.push_back(generalizeStructure(origMember));
    }
    return ProtocolCompositionType::get(ctx, newMembers,
                                        origType->getInverses(),
                                        origType->hasExplicitAnyObject());
  }

  // Generalize the type arguments of nominal types.
  Type visitBoundGenericType(CanBoundGenericType origType) {
    return generalizeGenericArguments(origType->getDecl(), origType);
  }
  Type visitNominalType(CanNominalType origType) {
    auto decl = origType->getDecl();
    if (decl->isGenericContext())
      return generalizeGenericArguments(decl, origType);
    return origType;
  }

  // Preserve existential structure.
  Type visitExistentialType(CanExistentialType origType) {
    return ExistentialType::get(
              generalizeStructure(origType.getConstraintType()));
  }
  Type visitExistentialMetatypeType(CanExistentialMetatypeType origType) {
    assert(!origType->hasRepresentation());
    return ExistentialMetatypeType::get(
              generalizeStructure(origType.getInstanceType()));
  }

  // These types can be generalized by a recursive transform of
  // their component types; we don't need to exclude anything or
  // handle conformances.
#define GENERALIZE_COMPONENTS(ID)                              \
  Type visit##ID##Type(Can##ID##Type origType) {               \
    return generalizeComponentTypes(origType);                 \
  }
  GENERALIZE_COMPONENTS(Function)
  GENERALIZE_COMPONENTS(Metatype)
  GENERALIZE_COMPONENTS(Tuple)
#undef GENERALIZE_COMPONENTS

  // These types can never contain component types with abstract
  // constraints, so generalizeComponentType should always substitute
  // them out.
#define NO_PRESERVABLE_STRUCTURE(ID)                           \
  Type visit##ID##Type(Can##ID##Type origType) {               \
    llvm_unreachable(#ID "Type has no structure to preserve"); \
  }
  NO_PRESERVABLE_STRUCTURE(Archetype)
  NO_PRESERVABLE_STRUCTURE(Builtin)
  NO_PRESERVABLE_STRUCTURE(DependentMember)
  NO_PRESERVABLE_STRUCTURE(GenericTypeParam)
  NO_PRESERVABLE_STRUCTURE(Module)
  NO_PRESERVABLE_STRUCTURE(Pack)
  NO_PRESERVABLE_STRUCTURE(PackExpansion)
  NO_PRESERVABLE_STRUCTURE(PackElement)
#undef NO_PRESERVABLE_STRUCTURE

  // These types simply shouldn't appear in types that we generalize at all.
#define INVALID_TO_GENERALIZE(ID)                                       \
  Type visit##ID##Type(Can##ID##Type origType) {                        \
    llvm_unreachable(#ID "type should not be found by generalization"); \
  }
  INVALID_TO_GENERALIZE(DynamicSelf)
  INVALID_TO_GENERALIZE(Error)
  INVALID_TO_GENERALIZE(GenericFunction)
  INVALID_TO_GENERALIZE(InOut)
  INVALID_TO_GENERALIZE(LValue)
  INVALID_TO_GENERALIZE(ReferenceStorage)
  INVALID_TO_GENERALIZE(SILBlockStorage)
  INVALID_TO_GENERALIZE(SILBox)
  INVALID_TO_GENERALIZE(SILFunction)
  INVALID_TO_GENERALIZE(SILPack)
  INVALID_TO_GENERALIZE(SILToken)
  INVALID_TO_GENERALIZE(SILMoveOnlyWrapped)
#undef INVALID_TO_GENERALIZE

  /// Generalize the generic arguments of the given generic type.s
  Type generalizeGenericArguments(NominalTypeDecl *decl, CanType type) {
    assert(decl->isGenericContext());
    auto origSubs = type->getContextSubstitutionMap(decl->getModuleContext(),
                                                    decl);

    // Generalize all of the arguments.
    auto origArgs = origSubs.getReplacementTypes();
    SmallVector<Type, 4> newArgs;
    for (auto origArg: origArgs) {
      newArgs.push_back(generalizeComponentType(CanType(origArg)));
    }

    // Generalize all of the conformances.
    // TODO: for abstract requirements, we might not generalize all
    // arguments, and we may need to leave corresponding conformances
    // concrete.
    SmallVector<ProtocolConformanceRef, 4> newConformances;
    auto origConformances = origSubs.getConformances();
    for (auto origConformance: origConformances) {
      newConformances.push_back(
        ProtocolConformanceRef(origConformance.getRequirement()));
    }

    auto origSig = origSubs.getGenericSignature();
    auto newSubs = SubstitutionMap::get(origSig, newArgs, newConformances);

    // Add any conformance requirements to the generic signature and
    // remember the conformances we generalized.
    if (!origConformances.empty()) {
      size_t i = 0;
      for (auto &origReq: origSig.getRequirements()) {
        if (origReq.getKind() != RequirementKind::Conformance) continue;
        auto origConformance = origConformances[i++];

        auto newReq = origReq.subst(newSubs);
        addedRequirements.push_back(newReq);

        substConformances.insert({{newReq.getFirstType()->getCanonicalType(),
                                   newReq.getProtocolDecl()},
                                  origConformance});
      }
    }

    // Build the new type.
    return decl->getDeclaredInterfaceType().subst(newSubs);
  }

  /// Generalize the given type by preserving its top-level structure
  /// but generalizing its component types.
  Type generalizeComponentTypes(CanType type) {
    return type.transformRec(
        [&](TypeBase *componentType) -> std::optional<Type> {
          // Ignore the top level.
          if (componentType == type.getPointer())
            return std::nullopt;

          return generalizeComponentType(CanType(componentType));
        });
  }

  Type generalizeComponentType(CanType origArg) {
    // TODO: Abstract constraints (some P) introduce *existential*
    // component types, which are not substitutable.  Therefore, types
    // containing them must be generalized preserving that structure
    // rather than wholly substituted.  They can appear in arbitrary
    // positions, including within tuple, function, and metatype types,
    // so we'll need to add cases for those to generalizeStructure
    // above.

    // Create a new generalization type parameter and record the
    // substitution.
    auto newParam = GenericTypeParamType::get(/*sequence*/ false,
                                              /*depth*/ 0,
                                              /*index*/ substTypes.size(),
                                              ctx);
    addedParameters.push_back(newParam);

    substTypes.insert({CanType(newParam), origArg});
    return newParam;
  }
};

} // end anonymous namespace

ExistentialTypeGeneralization
ExistentialTypeGeneralization::get(Type rawType) {
  assert(rawType->isAnyExistentialType());

  // Canonicalize.  We need to generalize the canonical shape of the
  // type or else generalization parameters won't match up.
  //
  // TODO: in full generality, do we need to do *contextual*
  // canonicalization in order to avoid introducing non-canonical
  // parameters?  (That is, do we need a contextual generic
  // signature if given an interface type?)
  CanType type = rawType->getCanonicalType();

  Generalizer generalizer(type->getASTContext());
  Type shape = generalizer.generalizeStructure(type);
  auto subs = generalizer.getGeneralizationSubstitutions();

  return {shape, subs};
}
