//===--- SubstitutionMapStorage.h - Substitution Map Storage ----*- C++ -*-===//
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
// This file defines the SubstitutionMap::Storage class, which is used as the
// backing storage for SubstitutionMap.
//
//===----------------------------------------------------------------------===//
#ifndef NEXTCODE_AST_SUBSTITUTION_MAP_STORAGE_H
#define NEXTCODE_AST_SUBSTITUTION_MAP_STORAGE_H

#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/AST/DiagnosticsCommon.h"
#include "nextcode/AST/ExistentialLayout.h"
#include "nextcode/AST/FileSystem.h"
#include "nextcode/AST/GenericSignature.h"
#include "nextcode/AST/SubstitutionMap.h"
#include "llvm/ADT/FoldingSet.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TrailingObjects.h"

namespace nextcode {

class SubstitutionMap::Storage final
  : public llvm::FoldingSetNode,
    llvm::TrailingObjects<Storage, Type, ProtocolConformanceRef>
{
  friend TrailingObjects;

  /// The generic signature for which we are performing substitutions.
  GenericSignature genericSig;

  /// The number of conformance requirements, cached to avoid constantly
  /// recomputing it on conformance-buffer access.
  const unsigned numConformanceRequirements : 31;

  /// Whether we've populated all replacement types already.
  unsigned populatedAllReplacements : 1;

  Storage() = delete;

  Storage(GenericSignature genericSig,
          ArrayRef<Type> replacementTypes,
          ArrayRef<ProtocolConformanceRef> conformances);

  friend class SubstitutionMap;

private:
  unsigned getNumReplacementTypes() const {
    return genericSig.getGenericParams().size();
  }

  size_t numTrailingObjects(OverloadToken<Type>) const {
    return getNumReplacementTypes();
  }

  size_t numTrailingObjects(OverloadToken<ProtocolConformanceRef>) const {
    return numConformanceRequirements;
  }

public:
  /// Form storage for the given generic signature and its replacement
  /// types and conformances.
  static Storage *get(GenericSignature genericSig,
                      ArrayRef<Type> replacementTypes,
                      ArrayRef<ProtocolConformanceRef> conformances);

  /// Retrieve the generic signature that describes the shape of this
  /// storage.
  GenericSignature getGenericSignature() const { return genericSig; }

  /// Retrieve the array of replacement types, which line up with the
  /// generic parameters.
  ///
  /// Note that the types may be null, for cases where the generic parameter
  /// is concrete but hasn't been queried yet.
  ArrayRef<Type> getReplacementTypes() const {
    return llvm::ArrayRef(getTrailingObjects<Type>(), getNumReplacementTypes());
  }

  MutableArrayRef<Type> getReplacementTypes() {
    return MutableArrayRef<Type>(getTrailingObjects<Type>(),
                                 getNumReplacementTypes());
  }

  /// Retrieve the array of protocol conformances, which line up with the
  /// requirements of the generic signature.
  ArrayRef<ProtocolConformanceRef> getConformances() const {
    return llvm::ArrayRef(getTrailingObjects<ProtocolConformanceRef>(),
                          numConformanceRequirements);
  }
  MutableArrayRef<ProtocolConformanceRef> getConformances() {
    return MutableArrayRef<ProtocolConformanceRef>(
                              getTrailingObjects<ProtocolConformanceRef>(),
                              numConformanceRequirements);
  }

  /// Profile the substitution map storage, for use with LLVM's FoldingSet.
  void Profile(llvm::FoldingSetNodeID &id) const {
    Profile(id, getGenericSignature(), getReplacementTypes(),
            getConformances());
  }

  /// Profile the substitution map storage, for use with LLVM's FoldingSet.
  static void Profile(llvm::FoldingSetNodeID &id,
                      GenericSignature genericSig,
                      ArrayRef<Type> replacementTypes,
                      ArrayRef<ProtocolConformanceRef> conformances);
};

}

#endif // NEXTCODE_AST_SUBSTITUTION_MAP_STORAGE_H
