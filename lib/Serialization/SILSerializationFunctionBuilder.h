//===--- SerializationFunctionBuilder.h -----------------------------------===//
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

#ifndef NEXTCODE_SERIALIZATION_SERIALIZATIONFUNCTIONBUILDER_H
#define NEXTCODE_SERIALIZATION_SERIALIZATIONFUNCTIONBUILDER_H

#include "nextcode/SIL/SILFunctionBuilder.h"

namespace nextcode {

class LLVM_LIBRARY_VISIBILITY SILSerializationFunctionBuilder {
  SILFunctionBuilder builder;

public:
  SILSerializationFunctionBuilder(SILModule &mod) : builder(mod) {}

  /// Create a SILFunction declaration for use either as a forward reference or
  /// for the eventual deserialization of a function body.
  SILFunction *createDeclaration(StringRef name, SILType type,
                                 SILLocation loc) {
    return builder.createFunction(
        SILLinkage::Private, name, type.getAs<SILFunctionType>(), nullptr,
        loc, IsNotBare, IsNotTransparent,
        IsNotSerialized, IsNotDynamic, IsNotDistributed, IsNotRuntimeAccessible,
        ProfileCounter(), IsNotThunk, SubclassScope::NotApplicable);
  }

  void setHasOwnership(SILFunction *f, bool newValue) {
    builder.setHasOwnership(f, newValue);
  }
};

} // namespace nextcode

#endif
