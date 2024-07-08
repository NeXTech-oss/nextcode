//===--- SemanticARCOpts.h ------------------------------------------------===//
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

#ifndef NEXTCODE_SILOPTIMIZER_SEMANTICARC_SEMANTICARCOPTS_H
#define NEXTCODE_SILOPTIMIZER_SEMANTICARC_SEMANTICARCOPTS_H

#include <cstdint>
#include <type_traits>

namespace nextcode {
namespace semanticarc {

/// An enum used so that at the command line, we can override which transforms
/// we perform.
enum class ARCTransformKind : uint64_t {
  Invalid = 0,
  OwnedToGuaranteedPhi = 0x1,
  LoadCopyToLoadBorrowPeephole = 0x2,
  RedundantBorrowScopeElimPeephole = 0x4,
  // TODO: Split RedundantCopyValueElimPeephole into more granular categories
  // such as dead live range, guaranteed copy_value opt, etc.
  RedundantCopyValueElimPeephole = 0x8,
  LifetimeJoiningPeephole = 0x10,
  OwnershipConversionElimPeephole = 0x20,
  RedundantMoveValueElim = 0x40,

  AllPeepholes = LoadCopyToLoadBorrowPeephole |
                 RedundantBorrowScopeElimPeephole |
                 RedundantCopyValueElimPeephole | LifetimeJoiningPeephole |
                 OwnershipConversionElimPeephole,
  All = AllPeepholes | OwnedToGuaranteedPhi | RedundantMoveValueElim,
};

inline ARCTransformKind operator&(ARCTransformKind lhs, ARCTransformKind rhs) {
  using UnderlyingTy = std::underlying_type<ARCTransformKind>::type;
  return ARCTransformKind(UnderlyingTy(lhs) & UnderlyingTy(rhs));
}

} // namespace semanticarc
} // namespace nextcode

#endif
