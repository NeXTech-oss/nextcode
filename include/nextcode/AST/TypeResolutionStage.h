//===--- TypeResolutionStage.h - Type Resolution Stage ----------*- C++ -*-===//
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
#ifndef NEXTCODE_AST_TYPE_RESOLUTION_STAGE_H
#define NEXTCODE_AST_TYPE_RESOLUTION_STAGE_H

namespace llvm {
class raw_ostream;
}

namespace nextcode {

/// Describes the stage at which a particular type should be computed.
///
/// Later stages compute more information about the type, requiring more
/// complete analysis.
enum class TypeResolutionStage : uint8_t {
  /// Produces an interface type describing its structure, but without
  /// performing semantic analysis to resolve (e.g.) references to members of
  /// type parameters.
  Structural,

  /// Produces a complete interface type where all member references have been
  /// resolved.
  Interface,
};

/// Display a type resolution stage.
void simple_display(llvm::raw_ostream &out, const TypeResolutionStage &value);

} // end namespace nextcode

#endif // NEXTCODE_AST_TYPE_RESOLUTION_STAGE_H
