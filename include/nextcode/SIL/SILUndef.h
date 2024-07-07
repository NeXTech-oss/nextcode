//===--- SILUndef.h - SIL Undef Value Representation ------------*- C++ -*-===//
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

#ifndef NEXTCODE_SIL_UNDEF_H
#define NEXTCODE_SIL_UNDEF_H

#include "nextcode/Basic/Compiler.h"
#include "nextcode/SIL/SILValue.h"

namespace nextcode {

class SILArgument;
class SILInstruction;
class SILModule;

class SILUndef : public ValueBase {
  /// A back pointer to the function that this SILUndef is uniqued by.
  SILFunction *parent;

  SILUndef(SILFunction *parent, SILType type);

public:
  void operator=(const SILArgument &) = delete;
  void operator delete(void *, size_t) = delete;

  /// Return a SILUndef with the same type as the passed in value.
  static SILUndef *get(SILValue value) {
    return SILUndef::get(value->getFunction(), value->getType());
  }

  static SILUndef *get(SILFunction *f, SILType ty);
  static SILUndef *get(SILFunction &f, SILType ty) {
    return SILUndef::get(&f, ty);
  }

  /// This is an API only used by SILSSAUpdater... please do not use it anywhere
  /// else.
  template <class OwnerTy>
  static SILUndef *getSentinelValue(SILFunction *fn, OwnerTy owner,
                                    SILType type) {
    // Ownership kind isn't used here, the value just needs to have a unique
    // address.
    return new (*owner) SILUndef(fn, type);
  }

  SILFunction *getParent() const { return parent; }
  ValueOwnershipKind getOwnershipKind() const { return OwnershipKind::None; }

  static bool classof(const SILArgument *) = delete;
  static bool classof(const SILInstruction *) = delete;
  static bool classof(SILNodePointer node) {
    return node->getKind() == SILNodeKind::SILUndef;
  }
};

} // end nextcode namespace

#endif
