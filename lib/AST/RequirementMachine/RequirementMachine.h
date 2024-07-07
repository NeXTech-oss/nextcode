//===--- RequirementMachine.h - Generics with term rewriting ----*- C++ -*-===//
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

#ifndef NEXTCODE_REQUIREMENTMACHINE_H
#define NEXTCODE_REQUIREMENTMACHINE_H

#include "nextcode/AST/GenericSignature.h"
#include "nextcode/AST/RequirementSignature.h"
#include "llvm/ADT/DenseMap.h"
#include <vector>

#include "Diagnostics.h"
#include "PropertyMap.h"
#include "RewriteContext.h"
#include "RewriteSystem.h"

namespace llvm {
class raw_ostream;
}

namespace nextcode {

class AbstractGenericSignatureRequest;
class ASTContext;
class AssociatedTypeDecl;
class CanType;
class GenericTypeParamType;
class InferredGenericSignatureRequest;
class LayoutConstraint;
class ProtocolDecl;
class Requirement;
class RequirementSignatureRequest;
class Type;
class UnifiedStatsReporter;

namespace rewriting {
class RewriteContext;

/// Wraps a rewrite system with higher-level operations in terms of
/// generic signatures and interface types.
class RequirementMachine final {
  friend class nextcode::ASTContext;
  friend class nextcode::rewriting::RewriteContext;
  friend class nextcode::RequirementSignatureRequest;
  friend class nextcode::AbstractGenericSignatureRequest;
  friend class nextcode::InferredGenericSignatureRequest;

  GenericSignature Sig;
  SmallVector<GenericTypeParamType *, 2> Params;

  RewriteContext &Context;
  RewriteSystem System;
  PropertyMap Map;

  bool Dump = false;
  bool Complete = false;

  /// Parameters to prevent runaway completion and property map construction.
  unsigned MaxRuleCount;
  unsigned MaxRuleLength;
  unsigned MaxConcreteNesting;

  UnifiedStatsReporter *Stats;

  /// All conformance paths computed so far.
  llvm::DenseMap<std::pair<Term, ProtocolDecl *>,
                 ConformancePath> ConformancePaths;

  /// Conformance access paths computed during the last round. All elements
  /// have the same length. If a conformance access path of greater length
  /// is requested, we refill CurrentConformancePaths with all paths of
  /// length N+1, and add them to the ConformancePaths map.
  std::vector<std::pair<Term, ConformancePath>> CurrentConformancePaths;

  explicit RequirementMachine(RewriteContext &rewriteCtx);

  RequirementMachine(const RequirementMachine &) = delete;
  RequirementMachine(RequirementMachine &&) = delete;
  RequirementMachine &operator=(const RequirementMachine &) = delete;
  RequirementMachine &operator=(RequirementMachine &&) = delete;

  void checkCompletionResult(CompletionResult result) const;

  std::pair<CompletionResult, unsigned>
  initWithProtocolSignatureRequirements(
      ArrayRef<const ProtocolDecl *> protos);

  std::pair<CompletionResult, unsigned>
  initWithGenericSignature(GenericSignature sig);

  std::pair<CompletionResult, unsigned>
  initWithProtocolWrittenRequirements(
      ArrayRef<const ProtocolDecl *> component,
      const llvm::DenseMap<const ProtocolDecl *,
                           SmallVector<StructuralRequirement, 4>> protos);

  std::pair<CompletionResult, unsigned>
  initWithWrittenRequirements(
      ArrayRef<GenericTypeParamType *> genericParams,
      ArrayRef<StructuralRequirement> requirements);

  bool isComplete() const;

  std::pair<CompletionResult, unsigned>
  computeCompletion(RewriteSystem::ValidityPolicy policy);

  void freeze();

  void computeRequirementDiagnostics(
                            SmallVectorImpl<RequirementError> &errors,
                            ArrayRef<InverseRequirement> inverses,
                            SourceLoc signatureLoc);

  MutableTerm getLongestValidPrefix(const MutableTerm &term) const;

  void buildRequirementsFromRules(
    ArrayRef<unsigned> requirementRules,
    ArrayRef<unsigned> typeAliasRules,
    ArrayRef<GenericTypeParamType *> genericParams,
    bool reconstituteSugar,
    std::vector<Requirement> &reqs,
    std::vector<ProtocolTypeAlias> &aliases) const;

  ArrayRef<GenericTypeParamType *> getGenericParams() const {
    return Params;
  }

public:
  ~RequirementMachine();

  // Generic signature queries. Generally you shouldn't have to construct a
  // RequirementMachine instance; instead, call the corresponding methods on
  // GenericSignature, which lazily create a RequirementMachine for you.
  GenericSignature::LocalRequirements getLocalRequirements(Type depType,
                      ArrayRef<GenericTypeParamType *> genericParams) const;
  bool requiresClass(Type depType) const;
  LayoutConstraint getLayoutConstraint(Type depType) const;
  bool requiresProtocol(Type depType, const ProtocolDecl *proto) const;
  GenericSignature::RequiredProtocols getRequiredProtocols(Type depType) const;
  Type getSuperclassBound(Type depType,
                          ArrayRef<GenericTypeParamType *> genericParams) const;
  bool isConcreteType(Type depType,
                      const ProtocolDecl *proto=nullptr) const;
  Type getConcreteType(Type depType,
                       ArrayRef<GenericTypeParamType *> genericParams,
                       const ProtocolDecl *proto=nullptr) const;
  bool areReducedTypeParametersEqual(Type depType1, Type depType2) const;
  bool isReducedType(Type type) const;
  Type getReducedType(Type type,
                      ArrayRef<GenericTypeParamType *> genericParams) const;
  bool isValidTypeParameter(Type type) const;
  ConformancePath getConformancePath(Type type, ProtocolDecl *protocol);
  TypeDecl *lookupNestedType(Type depType, Identifier name) const;

private:
  MutableTerm getReducedShapeTerm(Type type) const;

public:
  Type getReducedShape(Type type,
                       ArrayRef<GenericTypeParamType *> genericParams) const;

  bool haveSameShape(Type type1, Type type2) const;

  llvm::DenseMap<const ProtocolDecl *, RequirementSignature>
  computeMinimalProtocolRequirements();

  GenericSignature
  computeMinimalGenericSignature(bool reconstituteSugar);

  ArrayRef<Rule> getLocalRules() const;

  std::string getRuleAsStringForDiagnostics(unsigned ruleID) const;

  GenericSignatureErrors getErrors() const;

  void verify(const MutableTerm &term) const;
  void dump(llvm::raw_ostream &out) const;

  DebugOptions getDebugOptions() const { return Context.getDebugOptions(); }
};

} // end namespace rewriting

} // end namespace nextcode

#endif
