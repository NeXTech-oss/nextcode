//===--- GenValueWitness.h - NeXTCode IRGen for value witnesses ----*- C++ -*-===//
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
//  This file provides the private interface to the value witness emission
//  code.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_GENVALUEWITNESS_H
#define NEXTCODE_IRGEN_GENVALUEWITNESS_H

#include "nextcode/Basic/LLVM.h"

namespace llvm {
  class Constant;
  class Type;
}

namespace nextcode {
  class CanType;

namespace irgen {
  class ConstantStructBuilder;
  class IRGenModule;
  class ConstantReference;

  /// True if a type has a generic-parameter-dependent value witness table.
  /// Currently, this is true if the size and/or alignment of the type is
  /// dependent on its generic parameters.
  bool hasDependentValueWitnessTable(IRGenModule &IGM, NominalTypeDecl *decl);

  /// Emit a value-witness table for the given type.
  ///
  /// \param isPattern - true if the table just serves as an instantiation
  ///   pattern and does not need to be modifiable in-place (if the type
  ///   does not have static layout for some reason)
  ConstantReference emitValueWitnessTable(IRGenModule &IGM, CanType type,
                                          bool isPattern,
                                          bool relativeReference);

  SILType getLoweredTypeInPrimaryContext(IRGenModule &IGM,
                                         NominalTypeDecl *type);

  bool layoutStringsEnabled(IRGenModule &IGM, bool diagnose = false);
}
}

#endif
