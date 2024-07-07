//===--- LLVMExtras.h -----------------------------------------------------===//
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
// This file provides additional functionality on top of LLVM types
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_LLVMEXTRAS_H
#define NEXTCODE_BASIC_LLVMEXTRAS_H

#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/SmallVector.h"

namespace nextcode {

/// A SetVector that does no allocations under the specified size
///
/// nextcode::SmallSetVector provides the old SmallSetVector semantics that allow
/// storing types that don't have `operator==`.
template <typename T, unsigned N>
using SmallSetVector = llvm::SetVector<T, llvm::SmallVector<T, N>,
      llvm::SmallDenseSet<T, N, llvm::DenseMapInfo<T>>>;

} // namespace nextcode

#endif // NEXTCODE_BASIC_LLVMEXTRAS_H
