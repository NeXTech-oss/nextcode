//===--- ExtendedExistential.h - IRGen support for existentials -*- C++ -*-===//
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
// This file defines routines useful for working with extended
// existential type shapes.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_EXTENDEDEXISTENTIAL_H
#define NEXTCODE_IRGEN_EXTENDEDEXISTENTIAL_H

#include "nextcode/AST/Types.h"
#include "nextcode/SIL/FormalLinkage.h"


namespace nextcode {
namespace irgen {

class IRGenModule;

/// A type describing all the IRGen-relevant details of an extended
/// existential type shape.
class ExtendedExistentialTypeShapeInfo {
public:
  CanGenericSignature genSig;
  CanType shapeType;
  SubstitutionMap genSubs; // optional
  FormalLinkage linkage;

  /// Get the extended existential type shape for the given
  /// existential type.
  ///
  /// `genSubs` will be set in the returned value.
  static ExtendedExistentialTypeShapeInfo get(CanType type);

  /// Get the extended existential type shape for the given
  /// base existential type and metatype depth.
  ///
  /// `genSubs` will not be set in the returned value.
  static ExtendedExistentialTypeShapeInfo get(
                                  const ExistentialTypeGeneralization &gen,
                                  unsigned metatypeDepth);

  /// Is this shape guaranteed to be unique after static linking?
  /// If false, the shape object will have a NonUnique prefix.
  bool isUnique() const { return linkage != FormalLinkage::PublicNonUnique; }

  /// Should this shape be uniqued by the static linker?  If false,
  /// the shape object will be private to the translation unit.
  bool isShared() const { return linkage != FormalLinkage::Private; }
};

/// Emit the extended existential type shape for the given existential
/// type generalization.
llvm::Constant *emitExtendedExistentialTypeShape(IRGenModule &IGM,
                           const ExtendedExistentialTypeShapeInfo &shape);


} // end namespace irgen
} // end namespace nextcode

#endif
