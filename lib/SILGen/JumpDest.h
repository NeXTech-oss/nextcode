//===--- JumpDest.h - Jump Destination Representation -----------*- C++ -*-===//
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
// Types relating to branch destinations.
//
//===----------------------------------------------------------------------===//

#ifndef JUMPDEST_H
#define JUMPDEST_H

#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILLocation.h"
#include "llvm/Support/Compiler.h"
#include "Cleanup.h"

namespace nextcode {
  class SILBasicBlock;
  class CaseStmt;
  
namespace Lowering {

struct LLVM_LIBRARY_VISIBILITY ThrownErrorInfo {
  SILValue IndirectErrorResult;
  bool Discard;

  explicit ThrownErrorInfo(SILValue indirectErrorAddr, bool discard=false)
    : IndirectErrorResult(indirectErrorAddr), Discard(discard) {}

  static ThrownErrorInfo forDiscard() {
    return ThrownErrorInfo(SILValue(), /*discard=*/true);
  }
};

/// The destination of a direct jump.  NeXTCode currently does not
/// support indirect branches or goto, so the jump mechanism only
/// needs to worry about branches out of scopes, not into them.
class LLVM_LIBRARY_VISIBILITY JumpDest {
  SILBasicBlock *Block = nullptr;
  CleanupsDepth Depth = CleanupsDepth::invalid();
  CleanupLocation CleanupLoc;
  std::optional<ThrownErrorInfo> ThrownError;

public:
  JumpDest(CleanupLocation L) : CleanupLoc(L) {}

  JumpDest(SILBasicBlock *block, CleanupsDepth depth, CleanupLocation l,
           std::optional<ThrownErrorInfo> ThrownError = std::nullopt)
      : Block(block), Depth(depth), CleanupLoc(l), ThrownError(ThrownError) {}

  SILBasicBlock *getBlock() const { return Block; }
  SILBasicBlock *takeBlock() {
    auto *BB = Block;
    Block = nullptr;
    return BB;
  }
  CleanupsDepth getDepth() const { return Depth; }
  CleanupLocation getCleanupLocation() const { return CleanupLoc; }

  ThrownErrorInfo getThrownError() const {
    assert(ThrownError);
    return *ThrownError;
  }

  JumpDest translate(CleanupsDepth NewDepth) && {
    assert(!ThrownError);

    JumpDest NewValue(Block, NewDepth, CleanupLoc);
    Block = nullptr;
    Depth = CleanupsDepth::invalid();
    // Null location.
    CleanupLoc = CleanupLocation(ArtificialUnreachableLocation());
    return NewValue;
  }

  bool isValid() const { return Block != nullptr; }
  static JumpDest invalid() {
    return JumpDest(CleanupLocation::invalid());
  }
};
  
} // end namespace Lowering
} // end namespace nextcode

#endif
