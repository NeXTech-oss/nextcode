//===--- DebugInfoVerifier.cpp --------------------------------------------===//
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
/// Utility verifier code for validating debug info.
///
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILDebugScope.h"
#include "nextcode/SIL/SILInstruction.h"

using namespace nextcode;

//===----------------------------------------------------------------------===//
//                   MARK: Verify SILInstruction Debug Info
//===----------------------------------------------------------------------===//

void SILInstruction::verifyDebugInfo() const {
  auto require = [&](bool reqt, StringRef message) {
    if (!reqt) {
      llvm::errs() << message << "\n";
      assert(false && "invoking standard assertion failure");
    }
  };

  // Check the location kind.
  SILLocation loc = getLoc();
  SILLocation::LocationKind locKind = loc.getKind();
  SILInstructionKind instKind = getKind();

  // Regular locations are allowed on all instructions.
  if (locKind == SILLocation::RegularKind)
    return;

  if (locKind == SILLocation::ReturnKind ||
      locKind == SILLocation::ImplicitReturnKind)
    require(
        instKind == SILInstructionKind::BranchInst ||
            instKind == SILInstructionKind::ReturnInst ||
            instKind == SILInstructionKind::UnreachableInst,
        "return locations are only allowed on branch and return instructions");

  if (locKind == SILLocation::ArtificialUnreachableKind)
    require(
        instKind == SILInstructionKind::UnreachableInst,
        "artificial locations are only allowed on Unreachable instructions");
}
