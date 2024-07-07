//===----- IDETypeCheckingRequests.h - IDE type-check Requests --*- C++ -*-===//
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
//  This file defines IDE type checking request using the evaluator model.
//  The file needs to exist in sema because it needs internal implementation
//  of the type checker to fulfill some requests
//
//===----------------------------------------------------------------------===//
#ifndef NEXTCODE_IDE_TYPE_CHECKING_REQUESTS_H
#define NEXTCODE_IDE_TYPE_CHECKING_REQUESTS_H

#include "nextcode/AST/ASTTypeIDs.h"
#include "nextcode/AST/Evaluator.h"
#include "nextcode/AST/SimpleRequest.h"
#include "nextcode/AST/TypeCheckRequests.h"

namespace nextcode {
//----------------------------------------------------------------------------//
// Decl applicability checking
//----------------------------------------------------------------------------//
struct DeclApplicabilityOwner {
  const DeclContext *DC;
  const Type Ty;
  const Decl *ExtensionOrMember;

  DeclApplicabilityOwner(const DeclContext *DC, Type Ty, const ExtensionDecl *ED):
    DC(DC), Ty(Ty), ExtensionOrMember(ED) {}
  DeclApplicabilityOwner(const DeclContext *DC, Type Ty, const ValueDecl *VD):
    DC(DC), Ty(Ty), ExtensionOrMember(VD) {}

  friend llvm::hash_code hash_value(const DeclApplicabilityOwner &CI) {
    return llvm::hash_combine(CI.Ty.getPointer(), CI.ExtensionOrMember);
  }

  friend bool operator==(const DeclApplicabilityOwner &lhs,
                         const DeclApplicabilityOwner &rhs) {
    return lhs.Ty.getPointer() == rhs.Ty.getPointer() &&
      lhs.ExtensionOrMember == rhs.ExtensionOrMember;
  }

  friend bool operator!=(const DeclApplicabilityOwner &lhs,
                         const DeclApplicabilityOwner &rhs) {
    return !(lhs == rhs);
  }

  friend void simple_display(llvm::raw_ostream &out,
                             const DeclApplicabilityOwner &owner) {
    out << "Checking if ";
    simple_display(out, owner.ExtensionOrMember);
    out << " is applicable for ";
    simple_display(out, owner.Ty);
  }
};

class IsDeclApplicableRequest:
    public SimpleRequest<IsDeclApplicableRequest,
                         bool(DeclApplicabilityOwner),
                         RequestFlags::Cached> {
public:
  using SimpleRequest::SimpleRequest;

private:
  friend SimpleRequest;

  // Evaluation.
  bool evaluate(Evaluator &evaluator, DeclApplicabilityOwner Owner) const;

public:
  // Caching
  bool isCached() const { return true; }
  // Source location
  SourceLoc getNearestLoc() const { return SourceLoc(); };
};

//----------------------------------------------------------------------------//
// Type relation checking
//----------------------------------------------------------------------------//
enum class TypeRelation: uint8_t {
  ConvertTo,
  SubtypeOf
};

struct TypePair {
  Type FirstTy;
  Type SecondTy;
  TypePair(Type FirstTy, Type SecondTy): FirstTy(FirstTy), SecondTy(SecondTy) {}
  TypePair(): TypePair(Type(), Type()) {}
  friend llvm::hash_code hash_value(const TypePair &TI) {
    return llvm::hash_combine(TI.FirstTy.getPointer(),
                              TI.SecondTy.getPointer());
  }

  friend bool operator==(const TypePair &lhs,
                         const TypePair &rhs) {
    return lhs.FirstTy.getPointer() == rhs.FirstTy.getPointer() &&
      lhs.SecondTy.getPointer() == rhs.SecondTy.getPointer();
  }

  friend bool operator!=(const TypePair &lhs,
                         const TypePair &rhs) {
    return !(lhs == rhs);
  }

  friend void simple_display(llvm::raw_ostream &out,
                             const TypePair &owner) {
    out << "<";
    simple_display(out, owner.FirstTy);
    out << ", ";
    simple_display(out, owner.SecondTy);
    out << ">";
  }
};

struct TypeRelationCheckInput {
  DeclContext *DC;
  TypePair Pair;
  TypeRelation Relation;
  bool OpenArchetypes;

  TypeRelationCheckInput(DeclContext *DC, Type FirstType, Type SecondType,
                         TypeRelation Relation, bool OpenArchetypes = true):
    DC(DC), Pair(FirstType, SecondType), Relation(Relation),
    OpenArchetypes(OpenArchetypes) {}

  friend llvm::hash_code hash_value(const TypeRelationCheckInput &TI) {
    return llvm::hash_combine(TI.Pair, TI.Relation, TI.OpenArchetypes);
  }

  friend bool operator==(const TypeRelationCheckInput &lhs,
                         const TypeRelationCheckInput &rhs) {
    return lhs.Pair == rhs.Pair && lhs.Relation == rhs.Relation &&
      lhs.OpenArchetypes == rhs.OpenArchetypes;
  }

  friend bool operator!=(const TypeRelationCheckInput &lhs,
                         const TypeRelationCheckInput &rhs) {
    return !(lhs == rhs);
  }

  friend void simple_display(llvm::raw_ostream &out,
                               const TypeRelationCheckInput &owner) {
    out << "Check if ";
    simple_display(out, owner.Pair);
    out << " is ";
    switch(owner.Relation) {
#define CASE(NAME) case TypeRelation::NAME: out << #NAME << " "; break;
    CASE(ConvertTo)
    CASE(SubtypeOf)
#undef CASE
    }
  }
};

class TypeRelationCheckRequest:
    public SimpleRequest<TypeRelationCheckRequest,
                         bool(TypeRelationCheckInput),
                         RequestFlags::Cached> {
public:
  using SimpleRequest::SimpleRequest;

private:
  friend SimpleRequest;

  // Evaluation.
  bool evaluate(Evaluator &evaluator, TypeRelationCheckInput Owner) const;

public:
  // Caching
  bool isCached() const { return true; }
  // Source location
  SourceLoc getNearestLoc() const { return SourceLoc(); };
};

//----------------------------------------------------------------------------//
// RootAndResultTypeOfKeypathDynamicMemberRequest
//----------------------------------------------------------------------------//
class RootAndResultTypeOfKeypathDynamicMemberRequest:
    public SimpleRequest<RootAndResultTypeOfKeypathDynamicMemberRequest,
                         TypePair(SubscriptDecl*),
                         RequestFlags::Cached> {
public:
  using SimpleRequest::SimpleRequest;

private:
  friend SimpleRequest;

  // Evaluation.
  TypePair evaluate(Evaluator &evaluator, SubscriptDecl* SD) const;

public:
  // Caching
  bool isCached() const { return true; }
  // Source location
  SourceLoc getNearestLoc() const { return SourceLoc(); };
};

class RootTypeOfKeypathDynamicMemberRequest:
    public SimpleRequest<RootTypeOfKeypathDynamicMemberRequest,
                         Type(SubscriptDecl*),
                         /*Cached in the request above*/RequestFlags::Uncached> {
public:
  using SimpleRequest::SimpleRequest;

private:
  friend SimpleRequest;

  // Evaluation.
  Type evaluate(Evaluator &evaluator, SubscriptDecl* SD) const {
    return evaluateOrDefault(SD->getASTContext().evaluator,
      RootAndResultTypeOfKeypathDynamicMemberRequest{SD}, TypePair()).
        FirstTy;
  }

public:
  // Caching
  bool isCached() const { return true; }
  // Source location
  SourceLoc getNearestLoc() const { return SourceLoc(); };
};

/// The zone number for the IDE.
#define NEXTCODE_TYPEID_ZONE IDETypeChecking
#define NEXTCODE_TYPEID_HEADER "nextcode/Sema/IDETypeCheckingRequestIDZone.def"
#include "nextcode/Basic/DefineTypeIDZone.h"
#undef NEXTCODE_TYPEID_ZONE
#undef NEXTCODE_TYPEID_HEADER

// Set up reporting of evaluated requests.
#define NEXTCODE_REQUEST(Zone, RequestType, Sig, Caching, LocOptions)             \
template<>                                                                     \
inline void reportEvaluatedRequest(UnifiedStatsReporter &stats,                \
                            const RequestType &request) {                      \
  ++stats.getFrontendCounters().RequestType;                                   \
}
#include "nextcode/Sema/IDETypeCheckingRequestIDZone.def"
#undef NEXTCODE_REQUEST

} // end namespace nextcode

#endif // NEXTCODE_IDE_REQUESTS_H