//===--- PhiStorageOptimizer.h - Phi storage optimizer --------------------===//
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
/// This file defines PhiStorageOptimizer, a utility for use with the
/// mandatory AddressLowering pass.
///
//===----------------------------------------------------------------------===//

#include "AddressLowering.h"
#include "nextcode/SIL/SILArgument.h"
#include "nextcode/SIL/SILBasicBlock.h"
#include "nextcode/SIL/SILValue.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"

namespace nextcode {

class DominanceInfo;

class CoalescedPhi {
  friend class PhiStorageOptimizer;

  SmallVector<Operand *, 4> coalescedOperands;

  CoalescedPhi(const CoalescedPhi &) = delete;
  CoalescedPhi &operator=(const CoalescedPhi &) = delete;

public:
  CoalescedPhi() = default;
  CoalescedPhi(CoalescedPhi &&) = default;
  CoalescedPhi &operator=(CoalescedPhi &&) = default;

  void coalesce(PhiValue phi, const ValueStorageMap &valueStorageMap,
                DominanceInfo *domInfo);

  bool empty() const { return coalescedOperands.empty(); }

  ArrayRef<Operand *> getCoalescedOperands() const { return coalescedOperands; }

  SILInstruction::OperandValueRange getCoalescedValues() const {
    return SILInstruction::getOperandValues(getCoalescedOperands());
  }
};

} // namespace nextcode
