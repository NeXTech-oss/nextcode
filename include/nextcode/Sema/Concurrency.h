//===--- Concurrency.h ----------------------------------------------------===//
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
///
/// \file
///
/// This file defines concurrency utility routines from Sema that are used by
/// later parts of the compiler like the pass pipeline.
///
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SEMA_CONCURRENCY_H
#define NEXTCODE_SEMA_CONCURRENCY_H

#include <optional>

namespace nextcode {

class DeclContext;
class SourceFile;
class NominalTypeDecl;
class VarDecl;

enum class DiagnosticBehavior: uint8_t;

/// If any of the imports in this source file was @preconcurrency but there were
/// no diagnostics downgraded or suppressed due to that @preconcurrency, suggest
/// that the attribute be removed.
void diagnoseUnnecessaryPreconcurrencyImports(SourceFile &sf);

/// Determine whether the given nominal type has an explicit Sendable
/// conformance (regardless of its availability).
bool hasExplicitSendableConformance(NominalTypeDecl *nominal,
                                    bool applyModuleDefault = true);

/// Diagnose the use of an instance property of non-sendable type from an
/// nonisolated deinitializer within an actor-isolated type.
///
/// \returns true iff a diagnostic was emitted for this reference.
bool diagnoseNonSendableFromDeinit(
    SourceLoc refLoc, VarDecl *var, DeclContext *dc);

/// Determinate the appropriate diagnostic behavior when referencing
/// the given nominal type from the given declaration context.
std::optional<DiagnosticBehavior>
getConcurrencyDiagnosticBehaviorLimit(NominalTypeDecl *nominal,
                                      const DeclContext *fromDC,
                                      bool ignoreExplicitConformance = false);

} // namespace nextcode

#endif
