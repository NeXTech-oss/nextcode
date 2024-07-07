//===--- LifetimeAnnotation.h - Lifetime-affecting attributes ---*- C++ -*-===//
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
// Defines a simple type-safe wrapper around the annotations that affect value
// lifetimes.  Used for both Decls and SILFunctionArguments.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_LIFETIMEANNOTATION_H
#define NEXTCODE_AST_LIFETIMEANNOTATION_H

#include <cstdint>

namespace nextcode {

/// The annotation on a value (or type) explicitly indicating the lifetime that
/// it (or its instances) should have.
///
/// A LifetimeAnnotation is one of the following three values:
///
/// 1) None: No annotation has been applied.
/// 2) EagerMove: The @_eagerMove attribute has been applied.
/// 3) NoEagerMove: The @_noEagerMove attribute has been applied.
struct LifetimeAnnotation {
  enum Case : uint8_t {
    None,
    EagerMove,
    Lexical,
  } value;

  LifetimeAnnotation(Case newValue) : value(newValue) {}

  operator Case() const { return value; }

  bool isSome() { return value != None; }
};

} // namespace nextcode

#endif
