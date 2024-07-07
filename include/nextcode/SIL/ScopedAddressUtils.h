//===--- ScopedAddressUtils.h ---------------------------------------------===//
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

#ifndef NEXTCODE_SIL_SCOPEDADDRESSUTILS_H
#define NEXTCODE_SIL_SCOPEDADDRESSUTILS_H

#include "nextcode/Basic/Debug.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/SIL/OwnershipUtils.h"
#include "nextcode/SIL/PrunedLiveness.h"
#include "nextcode/SIL/SILBasicBlock.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILValue.h"
#include "nextcode/SILOptimizer/Utils/InstModCallbacks.h"

namespace nextcode {

class ScopedAddressValueKind {
public:
  enum Kind : uint8_t {
    Invalid = 0,
    StoreBorrow,
    BeginAccess,
  };

private:
  Kind value;

public:
  static ScopedAddressValueKind get(SILValue value) {
    switch (value->getKind()) {
    default:
      return Kind::Invalid;
    case ValueKind::StoreBorrowInst:
      return Kind::StoreBorrow;
    case ValueKind::BeginAccessInst:
      return Kind::BeginAccess;
    }
  }

  ScopedAddressValueKind(Kind newValue) : value(newValue) {}

  operator Kind() const { return value; }

  void print(llvm::raw_ostream &os) const;
  NEXTCODE_DEBUG_DUMP { print(llvm::dbgs()); }
};

llvm::raw_ostream &operator<<(llvm::raw_ostream &os,
                              ScopedAddressValueKind kind);

struct ScopedAddressValue {
  SILValue value;
  ScopedAddressValueKind kind = ScopedAddressValueKind::Invalid;

  ScopedAddressValue() = default;

  explicit ScopedAddressValue(SILValue value) {
    kind = ScopedAddressValueKind::get(value);
    if (kind)
      this->value = value;
  }

  operator bool() const {
    return kind != ScopedAddressValueKind::Invalid && value;
  }

  // Both the store_borrow source and address operands are effectively used for
  // the duration of the address scope.
  static ScopedAddressValue forUse(Operand *use) {
    if (auto svi = dyn_cast<SingleValueInstruction>(use->getUser()))
      return ScopedAddressValue(svi);

    return ScopedAddressValue();
  }

  void print(llvm::raw_ostream &os) const;
  NEXTCODE_DEBUG_DUMP { print(llvm::dbgs()); }

  // Helpers to allow a ScopedAddressValue to easily be used as a SILValue
  // programatically.
  SILValue operator->() { return value; }
  SILValue operator->() const { return value; }
  SILValue operator*() { return value; }
  SILValue operator*() const { return value; }

  /// Returns true if \p op is a scope ending use of the scoped address value.
  bool isScopeEndingUse(Operand *op) const;
  /// Pass all scope ending instructions to the visitor.
  bool visitScopeEndingUses(function_ref<bool(Operand *)> visitor) const;

  /// Optimistically computes liveness for all transitive uses, and adds this
  /// scope's live blocks into the SSA PrunedLiveness result. Returns
  /// AddressUseKind indicated whether a PointerEscape or Unknown use was
  /// encountered.
  ///
  /// This transitively finds uses within nested borrow scopes to handle
  /// incomplete nested lifetimes. Here, liveness will consider the apply to be
  /// a live use of the store_borrow:
  ///   %a = store_borrow
  ///   %v = load_borrow
  ///   apply (%v)
  ///   unreachable
  ///
  /// FIXME: with complete OSSA lifetimes, store borrow liveness is simply
  /// computed by visiting the end_borrow users.
  AddressUseKind computeTransitiveLiveness(SSAPrunedLiveness &liveness) const;

  /// Update \p liveness for all the transitive address uses.
  ///
  /// Valid for any type of liveness, SSA or MultiDef, that may be used by a
  /// scoped address.
  AddressUseKind updateTransitiveLiveness(SSAPrunedLiveness &liveness) const;

  /// Create appropriate scope ending instruction at \p insertPt.
  void createScopeEnd(SILBasicBlock::iterator insertPt, SILLocation loc) const;

  /// Create scope ending instructions at \p liveness boundary.
  void endScopeAtLivenessBoundary(SSAPrunedLiveness *liveness) const;
};

llvm::raw_ostream &operator<<(llvm::raw_ostream &os,
                              const ScopedAddressValue &value);

/// Returns true if there are other store_borrows enclosed within a store_borrow
/// \p sbi's scope
bool hasOtherStoreBorrowsInLifetime(StoreBorrowInst *sbi,
                                    SSAPrunedLiveness *liveness,
                                    DeadEndBlocks *deadEndBlocks);

} // namespace nextcode

#endif