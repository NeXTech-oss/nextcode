//===--- Callee.h -----------------------------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_SILGEN_CALLEE_H
#define NEXTCODE_SILGEN_CALLEE_H

#include "nextcode/AST/ForeignAsyncConvention.h"
#include "nextcode/AST/ForeignErrorConvention.h"
#include "nextcode/AST/ForeignInfo.h"
#include "nextcode/AST/Types.h"
#include "nextcode/SIL/AbstractionPattern.h"

namespace nextcode {
namespace Lowering {

class CalleeTypeInfo {
public:
  std::optional<AbstractionPattern> origFormalType;
  CanSILFunctionType substFnType;
  std::optional<AbstractionPattern> origResultType;
  CanType substResultType;
  ForeignInfo foreign;

private:
  std::optional<SILFunctionTypeRepresentation> overrideRep;

public:
  CalleeTypeInfo() = default;

  CalleeTypeInfo(
      CanSILFunctionType substFnType, AbstractionPattern origResultType,
      CanType substResultType,
      const std::optional<ForeignErrorConvention> &foreignError,
      const std::optional<ForeignAsyncConvention> &foreignAsync,
      ImportAsMemberStatus foreignSelf,
      std::optional<SILFunctionTypeRepresentation> overrideRep = std::nullopt)
      : origFormalType(std::nullopt), substFnType(substFnType),
        origResultType(origResultType), substResultType(substResultType),
        foreign{foreignSelf, foreignError, foreignAsync},
        overrideRep(overrideRep) {}

  CalleeTypeInfo(
      CanSILFunctionType substFnType, AbstractionPattern origResultType,
      CanType substResultType,
      std::optional<SILFunctionTypeRepresentation> overrideRep = std::nullopt)
      : origFormalType(std::nullopt), substFnType(substFnType),
        origResultType(origResultType), substResultType(substResultType),
        foreign(), overrideRep(overrideRep) {}

  SILFunctionTypeRepresentation getOverrideRep() const {
    return overrideRep.value_or(substFnType->getRepresentation());
  }
};

} // namespace Lowering
} // namespace nextcode

#endif
