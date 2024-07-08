//===--- IVAnalysis.cpp - SIL IV Analysis ---------------------------------===//
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
//             Tural Ghuliev (turalquliyev@it-gss.com)
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//

#include "nextcode/SILOptimizer/Analysis/IVAnalysis.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/PatternMatch.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILValue.h"

using namespace nextcode;
using namespace nextcode::PatternMatch;

#if !defined(NDEBUG)
static bool inSCC(ValueBase *value, IVInfo::SCCType &SCC) {
  for (SILNode *node : SCC) {
    if (node == value)
      return true;
  }
  return false;
}
#endif

// For now, we'll consider only the simplest induction variables:
// - Exactly one element in the cycle must be a SILArgument.
// - Only a single increment by a literal.
//
// In other words many valid things that could be considered induction
// variables are disallowed at this point.
SILArgument *IVInfo::isInductionSequence(SCCType &SCC) {
  // Ignore SCCs of size 1 for now. Some of these are derived IVs
  // like i+1 or i*4, which we will eventually want to handle.
  if (SCC.size() == 1)
    return nullptr;

  BuiltinInst *FoundBuiltin = nullptr;
  SILArgument *FoundArgument = nullptr;
  IntegerLiteralInst *IncValue = nullptr;
  for (unsigned long i = 0, e = SCC.size(); i != e; ++i) {
    if (auto IV = dyn_cast<SILArgument>(SCC[i])) {
      if (FoundArgument)
        return nullptr;

      FoundArgument = IV;
      continue;
    }

    // TODO: MultiValueInstruction

    auto *I = cast<SILInstruction>(SCC[i]);
    switch (I->getKind()) {
    case SILInstructionKind::BuiltinInst: {
      if (FoundBuiltin)
        return nullptr;

      FoundBuiltin = cast<BuiltinInst>(I);

      SILValue L, R;
      if (!match(FoundBuiltin, m_ApplyInst(BuiltinValueKind::SAddOver,
                                           m_SILValue(L), m_SILValue(R))))
        return nullptr;

      if (match(L, m_IntegerLiteralInst(IncValue)))
        std::swap(L, R);

      if (!match(R, m_IntegerLiteralInst(IncValue)))
        return nullptr;
      break;
    }

    case SILInstructionKind::TupleExtractInst: {
      assert(inSCC(cast<TupleExtractInst>(I)->getOperand(), SCC) &&
             "TupleExtract operand not an induction var");
      break;
    }

    default:
      return nullptr;
    }
  }
  if (!FoundBuiltin || !FoundArgument || !IncValue)
    return nullptr;

  InductionInfoMap[FoundArgument] = IVDesc(FoundBuiltin, IncValue);
  return FoundArgument;
}

void IVInfo::visit(SCCType &SCC) {
  assert(SCC.size() && "SCCs should have an element!!");

  SILArgument *IV;
  if (!(IV = isInductionSequence(SCC)))
    return;

  for (auto node : SCC) {
    if (auto value = dyn_cast<ValueBase>(node))
      InductionVariableMap[value] = IV;
  }
}