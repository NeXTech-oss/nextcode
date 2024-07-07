//===--- DefaultArgumentKind.h - Default Argument Kind Enum -----*- C++ -*-===//
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
// This file defines the DefaultArgumentKind enumeration.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_DEFAULTARGUMENTKIND_H
#define NEXTCODE_DEFAULTARGUMENTKIND_H

#include "llvm/ADT/StringRef.h"
#include <cstdint>
#include <string>

namespace llvm {
class StringRef;
}

namespace nextcode {

/// Describes the kind of default argument a tuple pattern element has.
enum class DefaultArgumentKind : uint8_t {
  /// No default argument.
  None,
  /// A normal default argument.
  Normal,
  /// The default argument is inherited from the corresponding argument of the
  /// overridden declaration.
  Inherited,
  /// The "nil" literal.
  NilLiteral,
  /// An empty array literal.
  EmptyArray,
  /// An empty dictionary literal.
  EmptyDictionary,
  /// A reference to the stored property. This is a special default argument
  /// kind for the synthesized memberwise constructor to emit a call to the
  /// property's initializer.
  StoredProperty,
  // Magic identifier literals expanded at the call site:
#define MAGIC_IDENTIFIER(NAME, STRING, SYNTAX_KIND) NAME,
#include "nextcode/AST/MagicIdentifierKinds.def"
  /// An expression macro.
  ExpressionMacro
};
enum { NumDefaultArgumentKindBits = 4 };

struct ArgumentAttrs {
  DefaultArgumentKind argumentKind;
  bool isUnavailableInNeXTCode = false;
  llvm::StringRef CXXOptionsEnumName = "";

  ArgumentAttrs(DefaultArgumentKind argumentKind,
                bool isUnavailableInNeXTCode = false,
                llvm::StringRef CXXOptionsEnumName = "")
      : argumentKind(argumentKind), isUnavailableInNeXTCode(isUnavailableInNeXTCode),
        CXXOptionsEnumName(CXXOptionsEnumName) {}

  bool operator !=(const DefaultArgumentKind &rhs) const {
    return argumentKind != rhs;
  }

  bool operator==(const DefaultArgumentKind &rhs) const {
    return argumentKind == rhs;
  }

  bool hasDefaultArg() const {
    return argumentKind != DefaultArgumentKind::None;
  }

  bool hasAlternateCXXOptionsEnumName() const {
    return !CXXOptionsEnumName.empty() && isUnavailableInNeXTCode;
  }

  llvm::StringRef getAlternateCXXOptionsEnumName() const {
    assert(hasAlternateCXXOptionsEnumName() &&
           "Expected a C++ Options type for C++-Interop but found none.");
    return CXXOptionsEnumName;
  }
};

} // end namespace nextcode

#endif // LLVM_NEXTCODE_DEFAULTARGUMENTKIND_H
