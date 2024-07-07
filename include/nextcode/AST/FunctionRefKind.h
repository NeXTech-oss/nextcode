//===--- FunctionRefKind.h - Function reference kind ------------*- C++ -*-===//
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
// This file defines the FunctionRefKind enum, which is used to describe how
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_FUNCTION_REF_KIND_H
#define NEXTCODE_AST_FUNCTION_REF_KIND_H

#include "llvm/ADT/StringRef.h"

namespace nextcode {

/// Describes how a function is referenced within an expression node,
/// which dictates whether argument labels are part of the resulting
/// function type or not.
///
/// How a function is referenced comes down to how it was spelled in
/// the source code, e.g., was it called in the source code and was it
/// spelled as a compound name.
enum class FunctionRefKind : unsigned {
  /// The function was referenced using a bare function name (e.g.,
  /// 'f') and not directly called.
  Unapplied,
  /// The function was referenced using a bare function name and was
  /// directly applied once, e.g., "f(a: 1, b: 2)".
  SingleApply,
  /// The function was referenced using a bare function name and was
  /// directly applied two or more times, e.g., "g(x)(y)".
  DoubleApply,
  /// The function was referenced using a compound function name,
  /// e.g., "f(a:b:)".
  Compound,
};

/// Produce a string describing a function reference kind, for
/// debugging purposes.
llvm::StringRef getFunctionRefKindStr(FunctionRefKind refKind);

}

#endif // NEXTCODE_AST_FUNCTION_REF_KIND_H
