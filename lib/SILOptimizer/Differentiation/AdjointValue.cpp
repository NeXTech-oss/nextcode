//===--- AdjointValue.h - Helper class for differentiation ----*- C++ -*---===//
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
// AdjointValue - a symbolic representation for adjoint values enabling
// efficient differentiation by avoiding zero materialization.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "differentiation"

#include "nextcode/SILOptimizer/Differentiation/AdjointValue.h"

void nextcode::autodiff::AdjointValue::print(llvm::raw_ostream &s) const {
  switch (getKind()) {
  case AdjointValueKind::Zero:
    s << "Zero[" << getType() << ']';
    break;
  case AdjointValueKind::Aggregate:
    s << "Aggregate[" << getType() << "](";
    if (auto *decl = getType().getASTType()->getStructOrBoundGenericStruct()) {
      interleave(
          llvm::zip(decl->getStoredProperties(), getAggregateElements()),
          [&s](std::tuple<VarDecl *, const AdjointValue &> elt) {
            s << std::get<0>(elt)->getName() << ": ";
            std::get<1>(elt).print(s);
          },
          [&s] { s << ", "; });
    } else if (getType().is<TupleType>()) {
      interleave(
          getAggregateElements(),
          [&s](const AdjointValue &elt) { elt.print(s); }, [&s] { s << ", "; });
    } else {
      llvm_unreachable("Invalid aggregate");
    }
    s << ')';
    break;
  case AdjointValueKind::Concrete:
    s << "Concrete[" << getType() << "](" << base->value.concrete << ')';
    break;
  case AdjointValueKind::AddElement:
    auto *addElementValue = getAddElementValue();
    auto baseAdjoint = addElementValue->baseAdjoint;
    auto eltToAdd = addElementValue->eltToAdd;

    s << "AddElement[";
    baseAdjoint.print(s);

    s << ", Field(";
    if (addElementValue->isTupleAdjoint()) {
      s << addElementValue->getFieldIndex();
    } else {
      s << addElementValue->getFieldDecl()->getNameStr();
    }
    s << "), ";

    eltToAdd.print(s);

    s << "]";
    break;
  }
}
