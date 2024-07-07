//===--- IVAnalysis.h - SIL IV Analysis -------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_SILOPTIMIZER_ANALYSIS_IVANALYSIS_H
#define NEXTCODE_SILOPTIMIZER_ANALYSIS_IVANALYSIS_H

#include "nextcode/SILOptimizer/Analysis/Analysis.h"
#include "nextcode/SIL/SILArgument.h"
#include "nextcode/SIL/SILValue.h"
#include "nextcode/SILOptimizer/Utils/SCCVisitor.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/Debug.h"

namespace nextcode {

class IVInfo : public SCCVisitor<IVInfo> {
public:
  typedef llvm::SmallVectorImpl<SILNode *> SCCType;
  friend SCCVisitor;

public:

  /// A descriptor for an induction variable comprised of a header argument
  /// (phi node) and an increment by an integer literal.
  class IVDesc {
  public:
    BuiltinInst *Inc;
    IntegerLiteralInst *IncVal;

    IVDesc() : Inc(nullptr), IncVal(nullptr) {}
    IVDesc(BuiltinInst *AI, IntegerLiteralInst *I) : Inc(AI), IncVal(I) {}

    operator bool() { return Inc != nullptr && IncVal != nullptr; }
    static IVDesc invalidIV() { return IVDesc(); }
  };

  IVInfo(SILFunction &F) : SCCVisitor(F) {
    run();
  }

  bool isInductionVariable(ValueBase *IV) {
    auto End = InductionVariableMap.end();
    auto Found = InductionVariableMap.find(IV);
    return Found != End;
  }

  SILArgument *getInductionVariableHeader(ValueBase *IV) {
    assert(isInductionVariable(IV) && "Expected induction variable!");

    return InductionVariableMap.find(IV)->second;
  }

  IVDesc getInductionDesc(SILArgument *Arg) {
    llvm::DenseMap<const ValueBase *, IVDesc>::iterator CI =
        InductionInfoMap.find(Arg);
    if (CI == InductionInfoMap.end())
      return IVDesc::invalidIV();
    return CI->second;
  }

private:
  // Map from an element of an induction sequence to the header.
  llvm::DenseMap<const ValueBase *, SILArgument *> InductionVariableMap;

  // Map from an induction variable header to the induction descriptor.
  llvm::DenseMap<const ValueBase *, IVDesc> InductionInfoMap;

  SILArgument *isInductionSequence(SCCType &SCC);
  void visit(SCCType &SCC);
};

class IVAnalysis final : public FunctionAnalysisBase<IVInfo> {
public:
  IVAnalysis(SILModule *)
      : FunctionAnalysisBase<IVInfo>(SILAnalysisKind::InductionVariable) {}
  IVAnalysis(const IVAnalysis &) = delete;
  IVAnalysis &operator=(const IVAnalysis &) = delete;

  static bool classof(const SILAnalysis *S) {
    return S->getKind() == SILAnalysisKind::InductionVariable;
  }

  std::unique_ptr<IVInfo> newFunctionAnalysis(SILFunction *F) override {
    return std::make_unique<IVInfo>(*F);
  }

  /// For now we always invalidate.
  virtual bool shouldInvalidate(SILAnalysis::InvalidationKind K) override {
    return true;
  }
};

}

#endif