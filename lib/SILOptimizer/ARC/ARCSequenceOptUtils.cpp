//===--- ARCSequenceOptUtils.cpp - ARCSequenceOpt utilities ------------===//
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

#include "ARCSequenceOptUtils.h"

using namespace nextcode;
namespace nextcode {
bool isARCSignificantTerminator(TermInst *TI) {
  switch (TI->getTermKind()) {
  case TermKind::UnreachableInst:
  // br is a forwarding use for its arguments. It cannot in of itself extend
  // the lifetime of an object (just like a phi-node) cannot.
  case TermKind::BranchInst:
  // A cond_br is a forwarding use for its non-operand arguments in a similar
  // way to br. Its operand must be an i1 that has a different lifetime from any
  // ref counted object.
  case TermKind::CondBranchInst:
    return false;
  // Be conservative for now. These actually perform some sort of operation
  // against the operand or can use the value in some way.
  case TermKind::ThrowInst:
  case TermKind::ThrowAddrInst:
  case TermKind::ReturnInst:
  case TermKind::UnwindInst:
  case TermKind::YieldInst:
  case TermKind::AwaitAsyncContinuationInst:
  case TermKind::TryApplyInst:
  case TermKind::SwitchValueInst:
  case TermKind::SwitchEnumInst:
  case TermKind::SwitchEnumAddrInst:
  case TermKind::DynamicMethodBranchInst:
  case TermKind::CheckedCastBranchInst:
  case TermKind::CheckedCastAddrBranchInst:
    return true;
  }

  llvm_unreachable("Unhandled TermKind in switch.");
}

} // end namespace 'nextcode'
