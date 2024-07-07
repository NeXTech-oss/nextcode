//===--- PackExpansionMatcher.cpp - Matching pack expansions --------------===//
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
// Utilities for structural matching of sequences of types containing pack
// expansions.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/PackExpansionMatcher.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Type.h"
#include "nextcode/AST/Types.h"
#include "nextcode/Basic/Assertions.h"
#include "llvm/ADT/SmallVector.h"
#include <algorithm>

using namespace nextcode;

template <>
Identifier TypeListPackMatcher<TupleTypeElt>::getElementLabel(
    const TupleTypeElt &elt) const {
  return elt.getName();
}

template <>
Type TypeListPackMatcher<TupleTypeElt>::getElementType(
    const TupleTypeElt &elt) const {
  return elt.getType();
}

template <>
ParameterTypeFlags TypeListPackMatcher<TupleTypeElt>::getElementFlags(
    const TupleTypeElt &elt) const {
  return ParameterTypeFlags();
}

template <>
Identifier TypeListPackMatcher<AnyFunctionType::Param>::getElementLabel(
    const AnyFunctionType::Param &elt) const {
  return elt.getLabel();
}

template <>
Type TypeListPackMatcher<AnyFunctionType::Param>::getElementType(
    const AnyFunctionType::Param &elt) const {
  return elt.getPlainType();
}

template <>
ParameterTypeFlags TypeListPackMatcher<AnyFunctionType::Param>::getElementFlags(
    const AnyFunctionType::Param &elt) const {
  return elt.getParameterFlags();
}

template <>
Identifier TypeListPackMatcher<Type>::getElementLabel(const Type &elt) const {
  return Identifier();
}

template <>
Type TypeListPackMatcher<Type>::getElementType(const Type &elt) const {
  return elt;
}

template <>
ParameterTypeFlags
TypeListPackMatcher<Type>::getElementFlags(const Type &elt) const {
  return ParameterTypeFlags();
}
