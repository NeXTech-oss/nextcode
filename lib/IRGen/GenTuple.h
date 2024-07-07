//===--- GenTuple.h - NeXTCode IR generation for tuples ------------*- C++ -*-===//
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
//  This file provides the private interface to the tuple-emission code.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_GENTUPLE_H
#define NEXTCODE_IRGEN_GENTUPLE_H

#include "nextcode/Basic/LLVM.h"

namespace llvm {
  class Value;
}

namespace nextcode {
  class CanType;

namespace irgen {
  class Address;
  class Explosion;
  class IRGenFunction;
  class Size;

  /// Project the address of a tuple element.
  Address projectTupleElementAddress(IRGenFunction &IGF,
                                     Address base,
                                     SILType tupleType,
                                     unsigned fieldNo);

  /// Project the address of a tuple element, given a dynamic index.
  Address projectTupleElementAddressByDynamicIndex(IRGenFunction &IGF,
                                                   Address base,
                                                   SILType tupleType,
                                                   llvm::Value *index,
                                                   SILType elementType);

  /// Project a tuple element rvalue from an already-exploded tuple rvalue.
  void projectTupleElementFromExplosion(IRGenFunction &IGF,
                                        SILType tupleType,
                                        Explosion &tuple,
                                        unsigned fieldNo,
                                        Explosion &out);

  /// Return the offset to the given tuple element, if it's fixed.
  ///
  /// This API is used by RemoteAST.
  std::optional<Size> getFixedTupleElementOffset(IRGenModule &IGM,
                                                 SILType tupleType,
                                                 unsigned fieldNo);

  /// Returns the index of the element in the llvm struct type which represents
  /// \p fieldNo in \p tupleType.
  ///
  /// Returns None if the tuple element is an empty type and therefore has no
  /// corresponding element in the llvm type.
  std::optional<unsigned> getPhysicalTupleElementStructIndex(IRGenModule &IGM,
                                                             SILType tupleType,
                                                             unsigned fieldNo);

  /// Emit a string encoding the labels in the given tuple type.
  llvm::Constant *getTupleLabelsString(IRGenModule &IGM,
                                       CanTupleType type);

  /// Load the NumElements of a tuple type metadata.
  llvm::Value *emitTupleTypeMetadataLength(IRGenFunction &IGF,
                                           llvm::Value *metadata);

  llvm::Value *emitTupleTypeMetadataElementType(IRGenFunction &IGF,
                                                llvm::Value *metadata,
                                                llvm::Value *index);
} // end namespace irgen
} // end namespace nextcode

#endif
