//===---- ExistentialSpecializerTransform.h - Existential Specializer -----===//
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
// This contains utilities for transforming existential args to generics.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SIL_EXISTENTIALTRANSFORM_H
#define NEXTCODE_SIL_EXISTENTIALTRANSFORM_H
#include "FunctionSignatureOpts.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SILOptimizer/Utils/Existential.h"
#include "nextcode/SILOptimizer/Utils/InstOptUtils.h"
#include "nextcode/SILOptimizer/Utils/SILOptFunctionBuilder.h"
#include "nextcode/SILOptimizer/Utils/SpecializationMangler.h"
#include "llvm/ADT/SmallBitVector.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"

namespace nextcode {

/// A descriptor to carry information from ExistentialTransform analysis
/// to transformation.
struct ExistentialTransformArgumentDescriptor {
  OpenedExistentialAccess AccessType;
  bool isConsumed;
};

/// ExistentialTransform creates a protocol constrained generic and a thunk.
class ExistentialTransform {
  /// Function Builder to create a new thunk.
  SILOptFunctionBuilder &FunctionBuilder;

  /// The original function to analyze and transform.
  SILFunction *F;

  /// The newly created inner function.
  SILFunction *NewF;

  /// The function signature mangler we are using.
  Mangle::FunctionSignatureSpecializationMangler &Mangler;

  /// List of arguments and their descriptors to specialize
  llvm::SmallDenseMap<int, ExistentialTransformArgumentDescriptor>
      &ExistentialArgDescriptor;

  /// Argument to Generic Type Map for NewF.
  llvm::SmallDenseMap<int, GenericTypeParamType *> ArgToGenericTypeMap;

  /// Allocate the argument descriptors.
  llvm::SmallVector<ArgumentDescriptor, 4> &ArgumentDescList;

  /// Create the Devirtualized Inner Function.
  void createExistentialSpecializedFunction();

  /// Create new generic arguments from existential arguments.
  void
  convertExistentialArgTypesToGenericArgTypes(
      SmallVectorImpl<GenericTypeParamType *> &genericParams,
      SmallVectorImpl<Requirement> &requirements);

  /// Create a name for the inner function.
  std::string createExistentialSpecializedFunctionName();

  /// Create the new devirtualized protocol function signature.
  CanSILFunctionType createExistentialSpecializedFunctionType();

  /// Create the thunk.
  void populateThunkBody();

public:
  /// Constructor.
  ExistentialTransform(
      SILOptFunctionBuilder &FunctionBuilder, SILFunction *F,
      Mangle::FunctionSignatureSpecializationMangler &Mangler,
      llvm::SmallVector<ArgumentDescriptor, 4> &ADL,
      llvm::SmallDenseMap<int, ExistentialTransformArgumentDescriptor>
          &ExistentialArgDescriptor)
      : FunctionBuilder(FunctionBuilder), F(F), NewF(nullptr), Mangler(Mangler),
        ExistentialArgDescriptor(ExistentialArgDescriptor),
        ArgumentDescList(ADL) {}

  /// Return the optimized iner function.
  SILFunction *getExistentialSpecializedFunction() { return NewF; }

  /// External function for the optimization.
  bool run() {
    createExistentialSpecializedFunction();
    return true;
  }
};

} // end namespace nextcode

#endif
