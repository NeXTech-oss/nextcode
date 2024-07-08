//===--- DifferentiationInvoker.cpp ---------------------------*- C++ -*---===//
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
// Class that represents an invoker of differentiation.
// Used to track diagnostic source locations.
//
//===----------------------------------------------------------------------===//

#include "nextcode/SILOptimizer/Differentiation/DifferentiationInvoker.h"

#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILDifferentiabilityWitness.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILInstruction.h"

namespace nextcode {
namespace autodiff {

SourceLoc DifferentiationInvoker::getLocation() const {
  switch (kind) {
  case Kind::DifferentiableFunctionInst:
    return getDifferentiableFunctionInst()->getLoc().getSourceLoc();
  case Kind::LinearFunctionInst:
    return getLinearFunctionInst()->getLoc().getSourceLoc();
  case Kind::IndirectDifferentiation:
    return getIndirectDifferentiation().first->getLoc().getSourceLoc();
  case Kind::SILDifferentiabilityWitnessInvoker:
    return getSILDifferentiabilityWitnessInvoker()
        ->getOriginalFunction()
        ->getLocation()
        .getSourceLoc();
  }
  llvm_unreachable("Invalid invoker kind"); // silences MSVC C4715
}

void DifferentiationInvoker::print(llvm::raw_ostream &os) const {
  os << "(differentiation_invoker ";
  switch (kind) {
  case Kind::DifferentiableFunctionInst:
    os << "differentiable_function_inst=(" << *getDifferentiableFunctionInst()
       << ')';
    break;
  case Kind::LinearFunctionInst:
    os << "linear_function_inst=(" << *getLinearFunctionInst() << ')';
    break;
  case Kind::IndirectDifferentiation: {
    auto indDiff = getIndirectDifferentiation();
    os << "indirect_differentiation=(" << *std::get<0>(indDiff) << ')';
    // TODO: Enable printing parent invokers.
    // May require storing a `DifferentiableInvoker *` in the
    // `IndirectDifferentiation` case.
    /*
    SILInstruction *inst;
    SILDifferentiableAttr *attr;
    std::tie(inst, attr) = getIndirectDifferentiation();
    auto invokerLookup = invokers.find(attr); // No access to ADContext?
    assert(invokerLookup != invokers.end() && "Expected parent invoker");
    */
    break;
  }
  case Kind::SILDifferentiabilityWitnessInvoker: {
    auto witness = getSILDifferentiabilityWitnessInvoker();
    os << "sil_differentiability_witness_invoker=(witness=(";
    witness->print(os);
    os << ") function=" << witness->getOriginalFunction()->getName();
    break;
  }
  }
  os << ')';
}

} // end namespace autodiff
} // end namespace nextcode
