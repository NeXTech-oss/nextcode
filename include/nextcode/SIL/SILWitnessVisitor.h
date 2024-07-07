//===--- SILWitnessVisitor.h - Witness method table visitor -----*- C++ -*-===//
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
// This file defines the SILWitnessVisitor class, which is used to generate and
// perform lookups in witness method tables for protocols and protocol
// conformances.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SIL_SILWITNESSVISITOR_H
#define NEXTCODE_SIL_SILWITNESSVISITOR_H

#include "nextcode/AST/ASTVisitor.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/DistributedDecl.h"
#include "nextcode/AST/ProtocolAssociations.h"
#include "nextcode/AST/Types.h"
#include "nextcode/SIL/TypeLowering.h"
#include "llvm/Support/ErrorHandling.h"

namespace nextcode {

/// A CRTP class for visiting the witnesses of a protocol.
///
/// The design here is that each entry (or small group of entries)
/// gets turned into a call to the implementation class describing
/// the exact variant of witness.  For example, for member
/// variables, there should be separate callbacks for adding a
/// getter/setter pair, for just adding a getter, and for adding a
/// physical projection (if we decide to support that).
///
/// You must override the following methods:
/// - addProtocolConformanceDescriptor()
/// - addOutOfLineBaseProtocol()
/// - addAssociatedType()
/// - addAssociatedConformance()
/// - addMethod()
/// - addPlaceholder()

template <class T> class SILWitnessVisitor : public ASTVisitor<T> {
  T &asDerived() { return *static_cast<T*>(this); }

public:
  void visitProtocolDecl(ProtocolDecl *protocol) {
    // The protocol conformance descriptor gets added first.
    asDerived().addProtocolConformanceDescriptor();

    auto requirements = protocol->getRequirementSignature().getRequirements();
    for (const auto &reqt : requirements) {
      switch (reqt.getKind()) {
      case RequirementKind::SameShape:
        llvm_unreachable("Same-shape requirement not supported here");

      // These requirements don't show up in the witness table.
      case RequirementKind::Superclass:
      case RequirementKind::SameType:
      case RequirementKind::Layout:
        continue;

      case RequirementKind::Conformance: {
        auto type = reqt.getFirstType()->getCanonicalType();
        assert(type->isTypeParameter());
        auto requirement = reqt.getProtocolDecl();

        // ObjC protocols do not have witnesses.
        if (!Lowering::TypeConverter::protocolRequiresWitnessTable(requirement))
          continue;

        // If the type parameter is 'self', consider this to be protocol
        // inheritance.  In the canonical signature, these should all
        // come before any protocol requirements on associated types.
        if (auto parameter = dyn_cast<GenericTypeParamType>(type)) {
          assert(type->isEqual(protocol->getSelfInterfaceType()));
          assert(parameter->getDepth() == 0 && parameter->getIndex() == 0 &&
                 "non-self type parameter in protocol");
          asDerived().addOutOfLineBaseProtocol(requirement);
          continue;
        }

        // Otherwise, add an associated requirement.
        AssociatedConformance assocConf(protocol, type, requirement);
        asDerived().addAssociatedConformance(assocConf);
        continue;
      }
      }
      llvm_unreachable("bad requirement kind");
    }

    // Add the associated types.
    for (auto *associatedType : protocol->getAssociatedTypeMembers()) {
      // If this is a new associated type (which does not override an
      // existing associated type), add it.
      if (associatedType->getOverriddenDecls().empty())
        asDerived().addAssociatedType(AssociatedType(associatedType));
    }

    if (asDerived().shouldVisitRequirementSignatureOnly())
      return;

    // Visit the witnesses for the direct members of a protocol.
    for (Decl *member : protocol->getMembers()) {
      ASTVisitor<T>::visit(member);
    }
  }

  /// If true, only the base protocols and associated types will be visited.
  /// The base implementation returns false.
  bool shouldVisitRequirementSignatureOnly() const {
    return false;
  }

  /// Fallback for unexpected protocol requirements.
  void visitDecl(Decl *d) {
    llvm_unreachable("unhandled protocol requirement");
  }

  void visitAbstractStorageDecl(AbstractStorageDecl *sd) {
    sd->visitOpaqueAccessors([&](AccessorDecl *accessor) {
      if (accessor->requiresNewWitnessTableEntry()) {
        asDerived().addMethod(SILDeclRef(accessor, SILDeclRef::Kind::Func));
        addAutoDiffDerivativeMethodsIfRequired(accessor,
                                               SILDeclRef::Kind::Func);
        addDistributedWitnessMethodsIfRequired(accessor,
                                               SILDeclRef::Kind::Func);
      }
    });
  }

  void visitConstructorDecl(ConstructorDecl *cd) {
    if (cd->requiresNewWitnessTableEntry()) {
      asDerived().addMethod(SILDeclRef(cd, SILDeclRef::Kind::Allocator));
      addAutoDiffDerivativeMethodsIfRequired(cd, SILDeclRef::Kind::Allocator);
    }
  }

  void visitAccessorDecl(AccessorDecl *func) {
    // Accessors are emitted by visitAbstractStorageDecl, above.
  }

  void visitFuncDecl(FuncDecl *func) {
    assert(!isa<AccessorDecl>(func));
    if (!func->requiresNewWitnessTableEntry())
      return;

    asDerived().addMethod(SILDeclRef(func, SILDeclRef::Kind::Func));
    addAutoDiffDerivativeMethodsIfRequired(func, SILDeclRef::Kind::Func);
    addDistributedWitnessMethodsIfRequired(func, SILDeclRef::Kind::Func);
  }

  void visitMissingMemberDecl(MissingMemberDecl *placeholder) {
    asDerived().addPlaceholder(placeholder);
  }

  void visitAssociatedTypeDecl(AssociatedTypeDecl *td) {
    // We already visited these in the first pass.
  }

  void visitTypeAliasDecl(TypeAliasDecl *tad) {
    // We don't care about these by themselves for witnesses.
  }

  void visitPatternBindingDecl(PatternBindingDecl *pbd) {
    // We only care about the contained VarDecls.
  }

  void visitIfConfigDecl(IfConfigDecl *icd) {
    // We only care about the active members, which were already subsumed by the
    // enclosing type.
  }

  void visitPoundDiagnosticDecl(PoundDiagnosticDecl *pdd) {
    // We don't care about diagnostics at this stage.
  }

private:
  void addAutoDiffDerivativeMethodsIfRequired(AbstractFunctionDecl *AFD,
                                              SILDeclRef::Kind kind) {
    SILDeclRef declRef(AFD, kind);
    for (auto *diffAttr : AFD->getAttrs().getAttributes<DifferentiableAttr>()) {
      asDerived().addMethod(declRef.asAutoDiffDerivativeFunction(
          AutoDiffDerivativeFunctionIdentifier::get(
              AutoDiffDerivativeFunctionKind::JVP,
              diffAttr->getParameterIndices(),
              diffAttr->getDerivativeGenericSignature(),
              AFD->getASTContext())));
      asDerived().addMethod(declRef.asAutoDiffDerivativeFunction(
          AutoDiffDerivativeFunctionIdentifier::get(
              AutoDiffDerivativeFunctionKind::VJP,
              diffAttr->getParameterIndices(),
              diffAttr->getDerivativeGenericSignature(),
              AFD->getASTContext())));
    }
  }

  void addDistributedWitnessMethodsIfRequired(AbstractFunctionDecl *AFD,
                                              SILDeclRef::Kind kind) {
    if (!AFD)
      return;

    auto thunk = AFD->getDistributedThunk();
    if (!thunk)
      return;

    SILDeclRef declRef(thunk, kind);
    asDerived().addMethod(declRef.asDistributed());
  }
};

} // end namespace nextcode

#endif
