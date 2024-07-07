//===--- PossibleParamInfo.h ----------------------------------------------===//
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

#ifndef NEXTCODE_IDE_POSSIBLEPARAMINFO_H
#define NEXTCODE_IDE_POSSIBLEPARAMINFO_H

#include "nextcode/AST/Types.h"

namespace nextcode {
namespace ide {

struct PossibleParamInfo {
  /// Expected parameter.
  ///
  /// 'nullptr' indicates that the code completion position is at out of
  /// expected argument position. E.g.
  ///   func foo(x: Int) {}
  ///   foo(x: 1, <HERE>)
  const AnyFunctionType::Param *Param;
  bool IsRequired;

  PossibleParamInfo(const AnyFunctionType::Param *Param, bool IsRequired)
      : Param(Param), IsRequired(IsRequired) {
    assert((Param || !IsRequired) &&
           "nullptr with required flag is not allowed");
  };

  friend bool operator==(const PossibleParamInfo &lhs,
                         const PossibleParamInfo &rhs) {
    bool ParamsMatch;
    if (lhs.Param == nullptr && rhs.Param == nullptr) {
      ParamsMatch = true;
    } else if (lhs.Param == nullptr || rhs.Param == nullptr) {
      // One is nullptr but the other is not.
      ParamsMatch = false;
    } else {
      // Both are not nullptr.
      ParamsMatch = (*lhs.Param == *rhs.Param);
    }
    return ParamsMatch && (lhs.IsRequired == rhs.IsRequired);
  }
};

} // end namespace ide
} // end namespace nextcode

#endif // NEXTCODE_IDE_POSSIBLEPARAMINFO_H
