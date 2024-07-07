//===--- TypeExpansionAnalysis.h --------------------------------*- C++ -*-===//
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
#ifndef NEXTCODE_SILOPTIMIZER_ANALYSIS_TYPEEXPANSIONANALYSIS_H
#define NEXTCODE_SILOPTIMIZER_ANALYSIS_TYPEEXPANSIONANALYSIS_H

#include "nextcode/AST/TypeExpansionContext.h"
#include "nextcode/SIL/Projection.h"
#include "nextcode/SIL/SILType.h"
#include "nextcode/SIL/SILValue.h"
#include "nextcode/SILOptimizer/Analysis/Analysis.h"
#include "llvm/ADT/DenseMap.h"

namespace nextcode {

/// This analysis determines memory effects during destruction.
class TypeExpansionAnalysis : public SILAnalysis {
  llvm::DenseMap<std::pair<SILType, TypeExpansionContext>, ProjectionPathList>
      ExpansionCache;

public:
  TypeExpansionAnalysis(SILModule *M)
      : SILAnalysis(SILAnalysisKind::TypeExpansion) {}

  static bool classof(const SILAnalysis *S) {
    return S->getKind() == SILAnalysisKind::TypeExpansion;
  }

  /// Return ProjectionPath to every leaf or intermediate node of the given type.
  const ProjectionPathList &getTypeExpansion(SILType B, SILModule *Mod,
                                             TypeExpansionContext context);

  /// Invalidate all information in this analysis.
  virtual void invalidate() override {
    // Nothing can invalidate, because types are static and cannot be changed
    // during the SIL pass pipeline.
  }

  /// Invalidate all of the information for a specific function.
  virtual void invalidate(SILFunction *F, InvalidationKind K)  override { }

  /// Notify the analysis about a newly created function.
  virtual void notifyAddedOrModifiedFunction(SILFunction *F) override {}

  /// Notify the analysis about a function which will be deleted from the
  /// module.
  virtual void notifyWillDeleteFunction(SILFunction *F) override {}

  /// Notify the analysis about changed witness or vtables.
  virtual void invalidateFunctionTables() override { }
};

}
#endif
