//===------- DifferentiationMangler.h --------- differentiation -*- C++ -*-===//
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

#ifndef NEXTCODE_SIL_UTILS_DIFFERENTIATIONMANGLER_H
#define NEXTCODE_SIL_UTILS_DIFFERENTIATIONMANGLER_H

#include "nextcode/AST/ASTMangler.h"
#include "nextcode/AST/AutoDiff.h"
#include "nextcode/Basic/NullablePtr.h"
#include "nextcode/Demangling/Demangler.h"
#include "nextcode/SIL/SILFunction.h"

namespace nextcode {
namespace Mangle {

/// A mangler for generated differentiation functions.
class DifferentiationMangler : public ASTMangler {
public:
  DifferentiationMangler() {}
  /// Returns the mangled name for a differentiation function of the given kind.
  std::string mangleAutoDiffFunction(StringRef originalName,
                                     Demangle::AutoDiffFunctionKind kind,
                                     const AutoDiffConfig &config);
  /// Returns the mangled name for a derivative function of the given kind.
  std::string mangleDerivativeFunction(StringRef originalName,
                                       AutoDiffDerivativeFunctionKind kind,
                                       const AutoDiffConfig &config);
  /// Returns the mangled name for a linear map of the given kind.
  std::string mangleLinearMap(StringRef originalName,
                              AutoDiffLinearMapKind kind,
                              const AutoDiffConfig &config);
  /// Returns the mangled name for a derivative function subset parameters
  /// thunk.
  std::string mangleDerivativeFunctionSubsetParametersThunk(
      StringRef originalName, CanType toType,
      AutoDiffDerivativeFunctionKind linearMapKind,
      IndexSubset *fromParamIndices, IndexSubset *fromResultIndices,
      IndexSubset *toParamIndices);
  /// Returns the mangled name for a linear map subset parameters thunk.
  std::string mangleLinearMapSubsetParametersThunk(
      CanType fromType, AutoDiffLinearMapKind linearMapKind,
      IndexSubset *fromParamIndices, IndexSubset *fromResultIndices,
      IndexSubset *toParamIndices);
};

} // end namespace Mangle
} // end namespace nextcode

#endif /* NEXTCODE_SIL_UTILS_DIFFERENTIATIONMANGLER_H */
