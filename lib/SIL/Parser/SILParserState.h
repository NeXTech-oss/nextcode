//===--- SILParserState.h - SILParserState declaration -------------------===//
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

#ifndef NEXTCODE_SIL_SILPARSERSTATE_H
#define NEXTCODE_SIL_SILPARSERSTATE_H

#include "nextcode/AST/Identifier.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/Parse/ParseSILSupport.h"
#include "nextcode/SIL/SILFunction.h"

#include "llvm/ADT/DenseMap.h"

//===----------------------------------------------------------------------===//
// SILParserState
//===----------------------------------------------------------------------===//

namespace nextcode {

class Parser;
class SILModule;

/// The global state of the SIL parser that the ordinary parser needs to
/// maintain while parsing a SIL file.  Local state, like the value map of
/// a SIL function, does not need to be kept here.
class SILParserState : public SILParserStateBase {
public:
  explicit SILParserState(SILModule &M) : M(M) {}
  ~SILParserState();

  SILModule &M;

  /// This is all of the forward referenced functions with
  /// the location for where the reference is.
  llvm::DenseMap<Identifier, Located<SILFunction *>> ForwardRefFns;
  /// A list of all functions forward-declared by a sil_scope.
  llvm::DenseSet<SILFunction *> PotentialZombieFns;

  /// A map from textual .sil scope number to SILDebugScopes.
  llvm::DenseMap<unsigned, SILDebugScope *> ScopeSlots;

  /// Did we parse a sil_stage for this module?
  bool DidParseSILStage = false;

  bool parseDeclSIL(Parser &P) override;
  bool parseDeclSILStage(Parser &P) override;
  bool parseSILVTable(Parser &P) override;
  bool parseSILMoveOnlyDeinit(Parser &P) override;
  bool parseSILGlobal(Parser &P) override;
  bool parseSILWitnessTable(Parser &P) override;
  bool parseSILDefaultWitnessTable(Parser &P) override;
  bool parseSILDifferentiabilityWitness(Parser &P) override;
  bool parseSILCoverageMap(Parser &P) override;
  bool parseSILProperty(Parser &P) override;
  bool parseSILScope(Parser &P) override;
};

} // end namespace nextcode

#endif // NEXTCODE_SIL_SILPARSERSTATE_H
