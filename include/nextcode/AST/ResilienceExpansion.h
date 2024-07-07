//===--- ResilienceExpansion.h ----------------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_AST_RESILIENCE_EXPANSION_H
#define NEXTCODE_AST_RESILIENCE_EXPANSION_H

#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/ErrorHandling.h"

namespace nextcode {

/// A specification for how much to expand resilient types.
///
/// Right now, this is just a placeholder; a proper expansion
/// specification will probably need to be able to express things like
/// 'expand any type that was fragile in at least such-and-such
/// version'.
enum class ResilienceExpansion : unsigned {
  /// A minimal expansion does not expand types that do not have a
  /// universally fragile representation.  This provides a baseline
  /// for what all components can possibly support.
  ///   - All exported functions must be compiled to at least provide
  ///     a minimally-expanded entrypoint, or else it will be
  ///     impossible for components that do not have that type
  ///     to call the function.
  ///   - Similarly, any sort of abstracted function call must go through
  ///     a minimally-expanded entrypoint.
  ///
  /// Minimal expansion will generally pass all resilient types indirectly.
  Minimal,

  /// A maximal expansion expands all types with fragile
  /// representation, even when they're not universally fragile.  This
  /// is better when internally manipulating values or when working
  /// with specialized entry points for a function.
  Maximal,

  Last_ResilienceExpansion = Maximal
};

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &os,
                                     ResilienceExpansion expansion) {
  switch (expansion) {
  case ResilienceExpansion::Minimal:
    return os << "Minimal";
  case ResilienceExpansion::Maximal:
    return os << "Maximal";
  }
  llvm_unreachable("Unhandled ResilienceExpansion in switch");
}

} // namespace nextcode

#endif // LLVM_NEXTCODE_AST_CAPTURE_INFO_H

