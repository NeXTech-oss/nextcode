//===--- GenDistributed.h - IRGen for distributed features ------*- C++ -*-===//
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
//
//===----------------------------------------------------------------------===//
//
// This file defines interfaces for emitting code for various distributed
// features.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_GENDISTRIBUTED_H
#define NEXTCODE_IRGEN_GENDISTRIBUTED_H

#include "nextcode/AST/Types.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/SIL/ApplySite.h"
#include "llvm/IR/CallingConv.h"

#include "Callee.h"
#include "GenHeap.h"
#include "IRGenModule.h"

namespace llvm {
class Value;
}

namespace nextcode {
class CanType;
class ProtocolConformanceRef;
class SILType;

namespace irgen {
class Explosion;
class IRGenFunction;

/// Emit the '_distributedActorRemoteInitialize' call.
llvm::Value *emitDistributedActorInitializeRemote(
    IRGenFunction &IGF,
    SILType selfType,
    llvm::Value *actorMetatype,
    Explosion &out);

} // end namespace irgen
} // end namespace nextcode

#endif
