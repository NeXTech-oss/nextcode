//===--- ProtocolConformanceAnalysis.h - Protocol Conformance ---*- C++ -*-===//
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
// This analysis collects a set of nominal types (classes, structs, and enums)
// that conform to a protocol during whole module compilation. We only track
// protocols that are non-public.

#ifndef NEXTCODE_SILOPTIMIZER_ANALYSIS_PROTOCOLCONFORMANCE_H
#define NEXTCODE_SILOPTIMIZER_ANALYSIS_PROTOCOLCONFORMANCE_H

#include "nextcode/SIL/SILArgument.h"
#include "nextcode/SIL/SILValue.h"
#include "nextcode/SILOptimizer/Analysis/Analysis.h"
#include "nextcode/SILOptimizer/Analysis/ClassHierarchyAnalysis.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/Debug.h"

namespace nextcode {

class SILModule;
class NominalTypeDecl;
class ProtocolDecl;

class ProtocolConformanceAnalysis : public SILAnalysis {
public:
  typedef SmallVector<NominalTypeDecl *, 8> NominalTypeList;
  typedef llvm::DenseMap<ProtocolDecl *, NominalTypeList>
      ProtocolConformanceMap;
  typedef llvm::DenseMap<ProtocolDecl *, NominalTypeDecl *>
      SoleConformingTypeMap;

  ProtocolConformanceAnalysis(SILModule *Mod)
      : SILAnalysis(SILAnalysisKind::ProtocolConformance), M(Mod) {}

  ~ProtocolConformanceAnalysis();

  static bool classof(const SILAnalysis *S) {
    return S->getKind() == SILAnalysisKind::ProtocolConformance;
  }

  /// Invalidate all information in this analysis.
  virtual void invalidate() override {}

  /// Invalidate all of the information for a specific function.
  virtual void invalidate(SILFunction *F, InvalidationKind K) override {}

  /// Notify the analysis about a newly created function.
  virtual void notifyAddedOrModifiedFunction(SILFunction *F) override {}

  /// Notify the analysis about a function which will be deleted from the
  /// module.
  virtual void notifyWillDeleteFunction(SILFunction *F) override {}

  /// Notify the analysis about changed witness or vtables.
  virtual void invalidateFunctionTables() override {}

  /// Get the nominal types that implement a protocol.
  ArrayRef<NominalTypeDecl *> getConformances(const ProtocolDecl *P) {
    populateConformanceCacheIfNecessary();
    auto ConformsListIt = ProtocolConformanceCache->find(P);
    return ConformsListIt != ProtocolConformanceCache->end()
               ? ArrayRef<NominalTypeDecl *>(ConformsListIt->second.begin(),
                                             ConformsListIt->second.end())
               : ArrayRef<NominalTypeDecl *>();
  }

  /// Traverse ProtocolConformanceMapCache recursively to determine sole
  /// conforming concrete type. 
  NominalTypeDecl *findSoleConformingType(ProtocolDecl *Protocol);

  // Wrapper function to findSoleConformingType that checks for additional
  // constraints for classes using ClassHierarchyAnalysis.
  bool getSoleConformingType(ProtocolDecl *Protocol, ClassHierarchyAnalysis *CHA, CanType &ConcreteType);

private:
  /// The module.
  SILModule *M;

  /// A cache that maps a protocol to its conformances.
  std::optional<ProtocolConformanceMap> ProtocolConformanceCache;

  /// A cache that holds SoleConformingType for protocols.
  SoleConformingTypeMap SoleConformingTypeCache;

  /// Populates `ProtocolConformanceCache` if necessary.
  void populateConformanceCacheIfNecessary();
};

} // namespace nextcode
#endif
