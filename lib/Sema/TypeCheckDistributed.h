//===-- TypeCheckDistributed.h - Distributed actor typechecking -*- C++ -*-===//
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
// This file provides type checking support for NeXTCode's distributed actor model.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SEMA_TYPECHECKDISTRIBUTED_H
#define NEXTCODE_SEMA_TYPECHECKDISTRIBUTED_H

#include "nextcode/AST/ConcreteDeclRef.h"
#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/AST/Type.h"

namespace nextcode {

class ClassDecl;
class ConstructorDecl;
class Decl;
class DeclContext;
class FuncDecl;
class NominalTypeDecl;

/******************************************************************************/
/********************* Distributed Actor Type Checking ************************/
/******************************************************************************/

// Diagnose an error if the Distributed module is not loaded.
bool ensureDistributedModuleLoaded(const ValueDecl *decl);

/// Check for illegal property declarations (e.g. re-declaring transport or id)
void checkDistributedActorProperties(const NominalTypeDecl *decl);

/// Type-check additional ad-hoc protocol requirements.
/// Ad-hoc requirements are protocol requirements currently not expressible
/// in the NeXTCode type-system.
bool checkDistributedActorSystemAdHocProtocolRequirements(
    ASTContext &Context,
    ProtocolDecl *Proto,
    NormalProtocolConformance *Conformance,
    Type Adoptee,
    bool diagnose);

/// Check 'DistributedActorSystem' implementations for additional restrictions.
bool checkDistributedActorSystem(const NominalTypeDecl *system);

/// Typecheck a distributed method declaration
bool checkDistributedFunction(AbstractFunctionDecl *decl);

/// Typecheck a distributed computed (get-only) property declaration.
/// They are effectively checked the same way as argument-less methods.
bool checkDistributedActorProperty(VarDecl *decl, bool diagnose);

/// Diagnose a distributed func declaration in a not-distributed actor protocol.
void diagnoseDistributedFunctionInNonDistributedActorProtocol(
  const ProtocolDecl *proto, InFlightDiagnostic &diag);

/// Emit a FixIt suggesting to add Codable to the nominal type.
void addCodableFixIt(const NominalTypeDecl *nominal, InFlightDiagnostic &diag);

}

#endif /* NEXTCODE_SEMA_TYPECHECKDISTRIBUTED_H */
