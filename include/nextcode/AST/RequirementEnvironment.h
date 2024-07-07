//===--- RequirementEnvironment.h - Requirement Environments ----*- C++ -*-===//
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
// This file defines the RequirementEnvironment class, which is used to
// capture how a witness to a protocol requirement maps type parameters.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_REQUIREMENT_ENVIRONMENT_H
#define NEXTCODE_AST_REQUIREMENT_ENVIRONMENT_H

#include "nextcode/AST/SubstitutionMap.h"

namespace nextcode {

class RootProtocolConformance;

/// Describes the environment of a requirement that will be used when
/// matching witnesses against the requirement and to form the resulting
/// \c Witness value.
///
/// The produced generic environment will have a fresh set of archetypes that
/// describe the combined constraints of the requirement (because those
/// are available to all potential witnesses) as well as the constraints from
/// the context to which the protocol conformance is ascribed, which may
/// include additional constraints beyond those of the extended type if the
/// conformance is conditional. The type parameters for the generic
/// environment are the type parameters of the conformance context
/// (\c conformanceDC) with another (deeper) level of type parameters for
/// generic requirements. See the \c Witness class for more information about
/// the witness thunk signature.
class RequirementEnvironment {
  /// A generic signature that combines the generic parameters of the
  /// concrete conforming type with the generic parameters of the
  /// requirement.
  ///
  ///
  /// For example, if you have:
  ///
  /// protocol P { func f<T>(_: T) }
  /// struct S<A, B> : P { func f<T>(_: T) }
  ///
  /// The requirement and witness signatures are, respectively:
  ///
  /// <Self : P, T>
  /// <A, B, T>
  ///
  /// The witness thunk signature in this case is just the witness signature.
  ///
  /// It may be that the witness is more generic than the requirement,
  /// for example:
  ///
  /// protocol P { func f(_: Int) }
  /// struct S<A, B> : P { func f<T>(_: T) { } }
  ///
  /// Here, the requirement signature and witness signatures are:
  ///
  /// <Self : P>
  /// <A, B, T>
  ///
  /// The witness thunk signature is just:
  ///
  /// <A, B>
  ///
  /// The witness thunk emitted by SILGen uses the witness thunk signature.
  /// Therefore one invariant we preserve is that the witness thunk is
  /// ABI compatible with the requirement's function type.
  GenericSignature witnessThunkSig = GenericSignature();

  /// The generic signature of the protocol requirement member.
  GenericSignature reqSig = GenericSignature();

  /// A substitution map mapping the requirement signature to the
  /// generic parameters of the witness thunk signature.
  SubstitutionMap reqToWitnessThunkSigMap;

public:
  /// Create a new environment for matching the given requirement within a
  /// particular conformance.
  ///
  /// \param conformanceDC The \c DeclContext to which the protocol
  /// conformance is ascribed, which provides additional constraints.
  ///
  /// \param reqSig The generic signature of the requirement for which we
  /// are creating a generic environment.
  ///
  /// \param proto The protocol containing the requirement.
  ///
  /// \param conformance The protocol conformance, or null if there is no
  /// conformance (because we're finding default implementations).
  RequirementEnvironment(DeclContext *conformanceDC,
                         GenericSignature reqSig,
                         ProtocolDecl *proto,
                         ClassDecl *covariantSelf,
                         RootProtocolConformance *conformance);

  /// Retrieve the generic signature of the requirement.
  GenericSignature getRequirementSignature() const {
    return reqSig;
  }

  /// Retrieve the generic signature of the witness thunk.
  GenericSignature getWitnessThunkSignature() const {
    return witnessThunkSig;
  }

  /// Retrieve the substitution map that maps the interface types of the
  /// requirement to the interface types of the witness thunk signature.
  SubstitutionMap getRequirementToWitnessThunkSubs() const {
    return reqToWitnessThunkSigMap;
  }
};

}

#endif // NEXTCODE_AST_REQUIREMENT_ENVIRONMENT_H
