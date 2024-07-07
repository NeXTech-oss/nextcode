//===--- NecessaryBindings.h - Optimizing archetype bindings ----*- C++ -*-===//
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
// This file defines a utility class for saving and restoring the
// archetype metadata necessary in order to carry out value operations
// on a type.
//
// This is a supplemental API of GenProto.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_NECESSARYBINDINGS_H
#define NEXTCODE_IRGEN_NECESSARYBINDINGS_H

#include "GenericRequirement.h"
#include "llvm/ADT/SetVector.h"
#include "nextcode/AST/Types.h"

namespace nextcode {
  class CanType;
  enum class MetadataState : size_t;
  class ProtocolDecl;
  class ProtocolConformanceRef;
  class SpecializedProtocolConformance;

  namespace irgen {
  class Address;
  class Explosion;
  class IRGenFunction;
  class IRGenModule;
  class Size;

/// NecessaryBindings - The set of metadata that must be saved in
/// order to perform some set of operations on a type.
class NecessaryBindings {
  llvm::SetVector<GenericRequirement> RequirementsSet;
  SubstitutionMap SubMap;
  bool NoEscape;

public:
  NecessaryBindings() {}
  NecessaryBindings(SubstitutionMap subs, bool noEscape)
    : SubMap(subs), NoEscape(noEscape) {}
  
  SubstitutionMap getSubstitutionMap() const {
    return SubMap;
  }

  /// Collect the necessary bindings to invoke a function with the given
  /// signature.
  static NecessaryBindings forPartialApplyForwarder(IRGenModule &IGM,
                                                    CanSILFunctionType origType,
                                                    SubstitutionMap subs,
                                                    bool noEscape,
                                                    bool considerParameterSources);

  void addRequirement(GenericRequirement requirement) {
    auto type = requirement.getTypeParameter().subst(SubMap);
    if (!type->hasArchetype())
      return;

    RequirementsSet.insert(requirement);
  }

  /// Get the requirement from the bindings at index i.
  const GenericRequirement &operator[](size_t i) const {
    return RequirementsSet[i];
  }

  size_t size() const { return getRequirements().size(); }

  /// Is the work to do trivial?
  bool empty() const { return getRequirements().empty(); }

  /// Returns the required size of the bindings.
  /// Pointer alignment is sufficient.
  Size getBufferSize(IRGenModule &IGM) const;

  /// Save the necessary bindings to the given buffer.
  void save(IRGenFunction &IGF, Address buffer) const;

  /// Restore the necessary bindings from the given buffer.
  void restore(IRGenFunction &IGF, Address buffer, MetadataState state) const;

  const llvm::ArrayRef<GenericRequirement> getRequirements() const {
    return RequirementsSet.getArrayRef();
  }

private:
  void computeBindings(IRGenModule &IGM,
                       CanSILFunctionType origType,
                       bool considerParameterSources);
};

} // end namespace irgen
} // end namespace nextcode

#endif
