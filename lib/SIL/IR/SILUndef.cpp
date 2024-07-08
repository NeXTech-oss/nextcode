//===--- SILUndef.cpp -----------------------------------------------------===//
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

#include "nextcode/SIL/SILUndef.h"
#include "nextcode/SIL/SILModule.h"

using namespace nextcode;

SILUndef::SILUndef(SILFunction *parent, SILType type)
    : ValueBase(ValueKind::SILUndef, type), parent(parent) {}

SILUndef *SILUndef::get(SILFunction *fn, SILType ty) {
  SILUndef *&entry = fn->undefValues[ty];
  if (entry == nullptr)
    entry = new (fn->getModule()) SILUndef(fn, ty);
  return entry;
}
