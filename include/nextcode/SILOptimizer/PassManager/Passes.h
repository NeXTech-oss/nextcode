//===--- Passes.h - NeXTCode Compiler SIL Pass Entrypoints ---------*- C++ -*-===//
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
//  This file declares the main entrypoints to SIL passes.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SILOPTIMIZER_PASSMANAGER_PASSES_H
#define NEXTCODE_SILOPTIMIZER_PASSMANAGER_PASSES_H

#include "nextcode/SIL/SILModule.h"

namespace nextcode {
  class SILOptions;
  class SILTransform;
  class SILModuleTransform;

  namespace irgen {
    class IRGenModule;
  }

  /// Run all the SIL diagnostic passes on \p M.
  ///
  /// \returns true if the diagnostic passes produced an error
  bool runSILDiagnosticPasses(SILModule &M);

  /// Run all the SIL performance optimization passes on \p M.
  void runSILOptimizationPasses(SILModule &M);

  /// Run all SIL passes for -Onone on module \p M.
  void runSILPassesForOnone(SILModule &M);

  /// Run the SIL lower hop-to-actor pass on \p M.
  bool runSILLowerHopToActorPass(SILModule &M);

/// Run the SIL ownership eliminator pass on \p M.
  bool runSILOwnershipEliminatorPass(SILModule &M);

  void runSILOptimizationPassesWithFileSpecification(SILModule &Module,
                                                     StringRef FileName);

  /// Detect and remove unreachable code. Diagnose provably unreachable
  /// user code.
  void performSILDiagnoseUnreachable(SILModule *M);

  /// Remove dead functions from \p M.
  void performSILDeadFunctionElimination(SILModule *M);

  /// Convert SIL to a lowered form suitable for IRGen.
  void runSILLoweringPasses(SILModule &M);

  /// Perform SIL Inst Count on M if needed.
  void performSILInstCountIfNeeded(SILModule *M);

  /// Identifiers for all passes. Used to procedurally create passes from
  /// lists of passes.
  enum class PassKind {
#define PASS(ID, TAG, NAME) ID,
#define PASS_RANGE(ID, START, END) ID##_First = START, ID##_Last = END,
#include "Passes.def"
    invalidPassKind
  };

  PassKind PassKindFromString(StringRef ID);
  StringRef PassKindID(PassKind Kind);
  StringRef PassKindTag(PassKind Kind);

#define PASS(ID, TAG, NAME) \
  SILTransform *create##ID();
#define IRGEN_PASS(ID, TAG, NAME)
#include "Passes.def"

} // end namespace nextcode

#endif
