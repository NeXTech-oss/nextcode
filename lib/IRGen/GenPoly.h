//===--- GenPoly.h - NeXTCode IR generation for polymorphism -------*- C++ -*-===//
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
//  This file provides the private interface to the code for translating
//  between polymorphic and monomorphic values.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_GENPOLY_H
#define NEXTCODE_IRGEN_GENPOLY_H

namespace llvm {
  class Type;
  template <class T> class ArrayRef;
}

namespace nextcode {
  class CanType;

namespace irgen {
  class Explosion;
  class IRGenFunction;
  class IRGenModule;

  /// Do the given types differ by abstraction when laid out as memory?
  bool differsByAbstractionInMemory(IRGenModule &IGM,
                                    CanType origTy, CanType substTy);

  /// Do the given types differ by abstraction when passed in an explosion?
  bool differsByAbstractionInExplosion(IRGenModule &IGM,
                                       CanType origTy, CanType substTy);

  /// Given a substituted explosion, re-emit it as an unsubstituted one.
  ///
  /// For example, given an explosion which begins with the
  /// representation of an (Int, Float), consume that and produce the
  /// representation of an (Int, T).
  ///
  /// The substitutions must carry origTy to substTy.
  void reemitAsUnsubstituted(IRGenFunction &IGF,
                             SILType origTy, SILType substTy,
                             Explosion &src, Explosion &dest);

  /// True if a function's signature in LLVM carries polymorphic parameters.
  bool hasPolymorphicParameters(CanSILFunctionType ty);
} // end namespace irgen
} // end namespace nextcode

#endif
