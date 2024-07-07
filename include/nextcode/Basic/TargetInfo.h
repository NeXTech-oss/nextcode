//===--- TargetInfo.h - Target Info Output ---------------------*- C++ -*-===//
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
// This file provides a high-level API for emitting target info
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_TARGETINFO_H
#define NEXTCODE_TARGETINFO_H

#include "nextcode/Basic/LLVM.h"

#include <optional>

namespace llvm {
class Triple;
class VersionTuple;
}

namespace nextcode {
class CompilerInvocation;

namespace targetinfo {
void printTargetInfo(const CompilerInvocation &invocation,
                     llvm::raw_ostream &out);

void printTripleInfo(const llvm::Triple &triple,
                     std::optional<llvm::VersionTuple> runtimeVersion,
                     llvm::raw_ostream &out);
} // namespace targetinfo
} // namespace nextcode

#endif
