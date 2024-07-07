//===--- Passes.h - LLVM optimizer passes for NeXTCode -------------*- C++ -*-===//
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

#ifndef NEXTCODE_LLVMPASSES_PASSES_H
#define NEXTCODE_LLVMPASSES_PASSES_H

#include "nextcode/LLVMPasses/PassesFwd.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"

namespace nextcode {

  struct NeXTCodeAAResult : llvm::AAResultBase {
    friend llvm::AAResultBase;

    explicit NeXTCodeAAResult() : AAResultBase() {}
    NeXTCodeAAResult(NeXTCodeAAResult &&Arg)
        : AAResultBase(std::move(Arg)) {}

    bool invalidate(llvm::Function &,
                    const llvm::PreservedAnalyses &) { return false; }

    bool invalidate(llvm::Function &, const llvm::PreservedAnalyses &,
                    llvm::FunctionAnalysisManager::Invalidator &) {
      return false;
    }

    using AAResultBase::getModRefInfo;
    llvm::ModRefInfo getModRefInfo(const llvm::CallBase *Call,
                                   const llvm::MemoryLocation &Loc) {
// FIXME: how to construct a SimpleAAQueryInfo without an AAResults?
//      llvm::SimpleAAQueryInfo AAQI;
//      return getModRefInfo(Call, Loc, AAQI);
      return llvm::ModRefInfo::ModRef;
   }
    llvm::ModRefInfo getModRefInfo(const llvm::CallBase *Call,
                                   const llvm::MemoryLocation &Loc,
                                   llvm::AAQueryInfo &AAQI);
  };

  class NeXTCodeAAWrapperPass : public llvm::ImmutablePass {
    std::unique_ptr<NeXTCodeAAResult> Result;

  public:
    static char ID; // Class identification, replacement for typeinfo
    NeXTCodeAAWrapperPass();

    NeXTCodeAAResult &getResult() { return *Result; }
    const NeXTCodeAAResult &getResult() const { return *Result; }

    bool doInitialization(llvm::Module &M) override;
    bool doFinalization(llvm::Module &M) override;
    void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
  };

  class NeXTCodeAA : public llvm::AnalysisInfoMixin<NeXTCodeAA> {
    friend llvm::AnalysisInfoMixin<NeXTCodeAA>;

    static llvm::AnalysisKey Key;

  public:
    using Result = NeXTCodeAAResult;

    NeXTCodeAAResult run(llvm::Function &F, llvm::FunctionAnalysisManager &AM);
  };

  class NeXTCodeRCIdentity {
  public:
    NeXTCodeRCIdentity() {}

    /// Returns the root of the RC-equivalent value for the given V.
    llvm::Value *getNeXTCodeRCIdentityRoot(llvm::Value *V);

  private:
    enum { MaxRecursionDepth = 16 };

    llvm::Value *stripPointerCasts(llvm::Value *Val);
    llvm::Value *stripReferenceForwarding(llvm::Value *Val);
  };

  class NeXTCodeARCOpt : public llvm::FunctionPass {
    /// NeXTCode RC Identity analysis.
    NeXTCodeRCIdentity RC;
    virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
    virtual bool runOnFunction(llvm::Function &F) override;
  public:
    static char ID;
    NeXTCodeARCOpt();
  };

  struct NeXTCodeARCOptPass : public llvm::PassInfoMixin<NeXTCodeARCOptPass> {
    NeXTCodeRCIdentity RC;

    llvm::PreservedAnalyses run(llvm::Function &F,
                                llvm::FunctionAnalysisManager &AM);
  };

  class NeXTCodeARCContract : public llvm::FunctionPass {
    /// NeXTCode RC Identity analysis.
    NeXTCodeRCIdentity RC;
    virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
    virtual bool runOnFunction(llvm::Function &F) override;
  public:
    static char ID;
    NeXTCodeARCContract() : llvm::FunctionPass(ID) {}
  };

  struct NeXTCodeARCContractPass
      : public llvm::PassInfoMixin<NeXTCodeARCContractPass> {
    NeXTCodeRCIdentity RC;

    llvm::PreservedAnalyses run(llvm::Function &F,
                                llvm::FunctionAnalysisManager &AM);
  };

  class InlineTreePrinter : public llvm::ModulePass {
    virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
    virtual bool runOnModule(llvm::Module &M) override;
  public:
    static char ID;
    InlineTreePrinter() : llvm::ModulePass(ID) {}
  };

  class NeXTCodeMergeFunctionsPass
      : public llvm::PassInfoMixin<NeXTCodeMergeFunctionsPass> {
    bool ptrAuthEnabled = false;
    unsigned ptrAuthKey = 0;

  public:
    NeXTCodeMergeFunctionsPass(bool ptrAuthEnabled, unsigned ptrAuthKey)
        : ptrAuthEnabled(ptrAuthEnabled), ptrAuthKey(ptrAuthKey) {}
    llvm::PreservedAnalyses run(llvm::Module &M,
                                llvm::ModuleAnalysisManager &AM);
  };

  struct InlineTreePrinterPass
      : public llvm::PassInfoMixin<InlineTreePrinterPass> {
    llvm::PreservedAnalyses run(llvm::Module &M,
                                llvm::ModuleAnalysisManager &AM);
  };
} // end namespace nextcode

#endif
