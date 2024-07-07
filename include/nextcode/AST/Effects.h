//===--- Decl.h - NeXTCode Language Declaration ASTs ---------------*- C++ -*-===//
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
// This file defines some data types used for 'rethrows' and `reasync` checking.
//
// We refer to 'throws' and 'async' as "effects". A function might have either or
// both effects.
//
// A function is _effect polymorphic_ if its effect depends on the call site.
// This can either be unconditional (the usual 'throws' or 'async' case), or it
// can depend on either its arguments or conformances (these are 'rethrows' and
// 'reasync' functions).
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_EFFECTS_H
#define NEXTCODE_EFFECTS_H

#include "nextcode/AST/Type.h"
#include "nextcode/Basic/OptionSet.h"

#include <utility>

namespace llvm {
class raw_ostream;
}

namespace nextcode {
class AbstractFunctionDecl;
class ProtocolDecl;

enum class EffectKind : uint8_t {
  Throws = 1 << 0,
  Async  = 1 << 1
};
using PossibleEffects = OptionSet<EffectKind>;

void simple_display(llvm::raw_ostream &out, const EffectKind kind);

class ValueDecl;

class PolymorphicEffectRequirementList {
  using Requirements = ArrayRef<AbstractFunctionDecl *>;
  using Conformances = ArrayRef<std::pair<Type, ProtocolDecl *>>;
private:
  Requirements requirements;
  Conformances conformances;

public:
  PolymorphicEffectRequirementList(Requirements requirements,
                                   Conformances conformances)
    : requirements(requirements), conformances(conformances) {}
  PolymorphicEffectRequirementList() {}

  Requirements getRequirements() const {
    return requirements;
  }

  Conformances getConformances() const {
    return conformances;
  }
};

void simple_display(llvm::raw_ostream &out,
                    const PolymorphicEffectRequirementList reqs);

enum class PolymorphicEffectKind : uint8_t {
  /// The function does not have this effect at all.
  None,

  /// The function has this effect if at least one closure argument has it.
  ///
  /// This is the ordinary 'rethrows' /'reasync' case.
  ByClosure,

  /// The function has this effect if at least one of its conformances has it.
  ///
  /// This is the conformance-based 'rethrows' /'reasync' case.
  ByConformance,

  /// The function is only permitted to be `rethrows` because it depends
  /// on a conformance to `AsyncSequence` or `AsyncIteratorProtocol`,
  /// which historically were "@rethrows" protocols.
  AsyncSequenceRethrows,

  /// The function has this effect unconditionally.
  ///
  /// This is a plain old 'throws' / 'async' function.
  Always,

  /// The function declaration was invalid.
  Invalid
};

void simple_display(llvm::raw_ostream &out, PolymorphicEffectKind value);

} // end namespace nextcode

#endif
