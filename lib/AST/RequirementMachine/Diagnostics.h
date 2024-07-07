//===--- Diagnostics.h - Requirement machine diagnostics --------*- C++ -*-===//
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

#ifndef NEXTCODE_REQUIREMENT_DIAGNOSTICS_H
#define NEXTCODE_REQUIREMENT_DIAGNOSTICS_H

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Requirement.h"
#include "nextcode/AST/Type.h"
#include "nextcode/Basic/Assertions.h"

namespace nextcode {

namespace rewriting {

/// Represents an invalid requirement, such as `T: Int`.
///
/// Invalid requirements are recorded while computing the
/// generic signature of a declaration, and diagnosed via
/// \c diagnoseRequirementErrors .
struct RequirementError {
  /// The kind of requirement error.
  enum class Kind {
    /// A constraint to a non-protocol, non-class type, e.g. T: Int.
    InvalidTypeRequirement,
    /// A type requirement on a trivially invalid subject type,
    /// e.g. Bool: Collection.
    InvalidRequirementSubject,
    /// An inverse constraint applied to an invalid subject type,
    /// e.g., each T : ~Copyable
    InvalidInverseSubject,
    /// The inverse constraint requirement cannot applied to the subject because
    /// it's an outer generic parameter, e.g.,
    ///   protocol P { func f() where Self: ~Copyable }
    InvalidInverseOuterSubject,
    /// An invalid shape requirement, e.g. T.shape == Int.shape
    InvalidShapeRequirement,
    /// A pair of conflicting requirements, T == Int, T == String
    ConflictingRequirement,
    /// An inverse requirement that conflicts with the computed requirements of
    /// a generic parameter, e.g., T : Copyable, T : ~Copyable
    ConflictingInverseRequirement,
    /// A recursive requirement, e.g. T == G<T.A>.
    RecursiveRequirement,
    /// A not-yet-supported same-element requirement, e.g. each T == Int.
    UnsupportedSameElement,
  } kind;

private:
  /// The invalid requirement.
  union {
    Requirement requirement;
    InverseRequirement inverse;
  };
public:

  /// A requirement that conflicts with \c requirement. Both
  /// requirements will have the same subject type.
  std::optional<Requirement> conflictingRequirement;

  SourceLoc loc;

private:
  RequirementError(Kind kind, Requirement requirement, SourceLoc loc)
      : kind(kind), requirement(requirement),
        conflictingRequirement(std::nullopt), loc(loc) {}

  RequirementError(Kind kind, InverseRequirement inverse, SourceLoc loc)
      : kind(kind), inverse(inverse), conflictingRequirement(std::nullopt),
        loc(loc) {}

  RequirementError(Kind kind, Requirement requirement,
                   Requirement conflict,
                   SourceLoc loc)
    : kind(kind), requirement(requirement), conflictingRequirement(conflict), loc(loc) {}

public:
  Requirement getRequirement() const {
    ASSERT(kind != Kind::InvalidInverseOuterSubject &&
           kind != Kind::InvalidInverseSubject &&
           kind != Kind::ConflictingInverseRequirement);
    return requirement;
  }

  InverseRequirement getInverse() const {
    ASSERT(kind == Kind::InvalidInverseOuterSubject ||
           kind == Kind::InvalidInverseSubject ||
           kind == Kind::ConflictingInverseRequirement);
    return inverse;
  }

  static RequirementError forInvalidTypeRequirement(Type subjectType,
                                                    Type constraint,
                                                    SourceLoc loc) {
    Requirement requirement(RequirementKind::Conformance, subjectType, constraint);
    return {Kind::InvalidTypeRequirement, requirement, loc};
  }

  static RequirementError forInvalidRequirementSubject(Requirement req,
                                                       SourceLoc loc) {
    return {Kind::InvalidRequirementSubject, req, loc};
  }

  static RequirementError forInvalidInverseSubject(InverseRequirement inv) {
    return {Kind::InvalidInverseSubject, inv, inv.loc};
  }

  static
  RequirementError forInvalidInverseOuterSubject(InverseRequirement inv) {
    return {Kind::InvalidInverseOuterSubject, inv, inv.loc};
  }

  static RequirementError forConflictingInverseRequirement(
                                               InverseRequirement req,
                                               SourceLoc loc) {
    return {Kind::ConflictingInverseRequirement, req, loc};
  }

  static RequirementError forInvalidShapeRequirement(Requirement req,
                                                     SourceLoc loc) {
    return {Kind::InvalidShapeRequirement, req, loc};
  }

  static RequirementError forConflictingRequirement(Requirement req,
                                                    SourceLoc loc) {
    return {Kind::ConflictingRequirement, req, loc};
  }

  static RequirementError forConflictingRequirement(Requirement first,
                                                    Requirement second,
                                                    SourceLoc loc) {
    return {Kind::ConflictingRequirement, first, second, loc};
  }

  static RequirementError forRecursiveRequirement(Requirement req,
                                                  SourceLoc loc) {
    return {Kind::RecursiveRequirement, req, loc};
  }

  static RequirementError forSameElement(Requirement req, SourceLoc loc) {
    return {Kind::UnsupportedSameElement, req, loc};
  }
};

/// Policy for the fixit that transforms 'T : S' where 'S' is not a protocol
/// or a class into 'T == S'.
enum AllowConcreteTypePolicy {
  /// Any type parameter can be concrete.
  All,

  /// Only associated types can be concrete.
  AssocTypes,

  /// Only nested associated types can be concrete. This is for protocols,
  /// where we don't want to suggest making an associated type member of
  /// 'Self' concrete.
  NestedAssocTypes
};

bool diagnoseRequirementErrors(ASTContext &ctx,
                               ArrayRef<RequirementError> errors,
                               AllowConcreteTypePolicy concreteTypePolicy);

} // end namespace rewriting

} // end namespace nextcode

#endif
