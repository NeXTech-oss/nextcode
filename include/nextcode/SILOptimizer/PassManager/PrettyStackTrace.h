//===--- PrettyStackTrace.h - PrettyStackTrace for Transforms ---*- C++ -*-===//
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

#ifndef NEXTCODE_SILOPTIMIZER_PASSMANAGER_PRETTYSTACKTRACE_H
#define NEXTCODE_SILOPTIMIZER_PASSMANAGER_PRETTYSTACKTRACE_H

#include "nextcode/SIL/PrettyStackTrace.h"
#include "llvm/Support/PrettyStackTrace.h"

namespace nextcode {

class SILFunctionTransform;
class SILModuleTransform;

class PrettyStackTraceSILFunctionTransform
    : public PrettyStackTraceSILFunction {
  SILFunctionTransform *SFT;
  unsigned PassNumber;

public:
  PrettyStackTraceSILFunctionTransform(SILFunctionTransform *SFT,
                                       unsigned PassNumber);

  virtual void print(llvm::raw_ostream &OS) const override;
};

class PrettyStackTraceSILModuleTransform : public llvm::PrettyStackTraceEntry {
  SILModuleTransform *SMT;
  unsigned PassNumber;

public:
  PrettyStackTraceSILModuleTransform(SILModuleTransform *SMT,
                                     unsigned PassNumber)
      : SMT(SMT), PassNumber(PassNumber) {}
  virtual void print(llvm::raw_ostream &OS) const override;
};

} // end namespace nextcode

#endif
