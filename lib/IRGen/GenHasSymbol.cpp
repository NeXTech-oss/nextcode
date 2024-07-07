//===--- GenHasSymbol.cpp - IR Generation for #_hasSymbol queries ---------===//
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
//  This file implements IR generation for `if #_hasSymbol` condition queries.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ASTMangler.h"
#include "nextcode/AST/PrettyStackTrace.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/IRGen/IRSymbolVisitor.h"
#include "nextcode/IRGen/Linking.h"
#include "nextcode/SIL/SILFunctionBuilder.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SIL/SILSymbolVisitor.h"
#include "clang/AST/DeclObjC.h"
#include "clang/AST/GlobalDecl.h"

#include "GenDecl.h"
#include "IRGenFunction.h"
#include "IRGenModule.h"

using namespace nextcode;
using namespace irgen;

/// Wrapper for IRGenModule::getAddrOfLLVMVariable() that also handles a few
/// additional types of entities that the main utility cannot.
static llvm::Constant *getAddrOfLLVMVariable(IRGenModule &IGM,
                                             LinkEntity entity) {
  if (entity.isTypeMetadataAccessFunction())
    return IGM.getAddrOfTypeMetadataAccessFunction(entity.getType(),
                                                   NotForDefinition);
  if (entity.isDispatchThunk())
    return IGM.getAddrOfDispatchThunk(entity.getSILDeclRef(), NotForDefinition);

  if (entity.isOpaqueTypeDescriptorAccessor()) {
    OpaqueTypeDecl *decl =
        const_cast<OpaqueTypeDecl *>(cast<OpaqueTypeDecl>(entity.getDecl()));
    bool isImplementation = entity.isOpaqueTypeDescriptorAccessorImpl();
    return IGM
        .getAddrOfOpaqueTypeDescriptorAccessFunction(decl, NotForDefinition,
                                                     isImplementation)
        .getDirectPointer();
  }

  // FIXME: Look up addr of the replaceable function (has "TI" mangling suffix)
  if (entity.isDynamicallyReplaceableFunctionImpl())
    return nullptr;

  return IGM.getAddrOfLLVMVariable(entity, ConstantInit(), DebugTypeInfo());
}

class HasSymbolIRGenVisitor : public IRSymbolVisitor {
  IRGenModule &IGM;
  llvm::SmallVector<llvm::Constant *, 4> &Addrs;

  void addFunction(SILFunction *fn) {
    Addrs.emplace_back(IGM.getAddrOfSILFunction(fn, NotForDefinition));
  }

  void addFunction(StringRef name) {
    SILFunction *silFn = IGM.getSILModule().lookUpFunction(name);
    // Definitions for each SIL function should have been emitted by SILGen.
    assert(silFn && "missing SIL function?");
    if (silFn)
      addFunction(silFn);
  }

public:
  HasSymbolIRGenVisitor(IRGenModule &IGM,
                        llvm::SmallVector<llvm::Constant *, 4> &Addrs)
      : IGM{IGM}, Addrs{Addrs} {};

  void addFunction(SILDeclRef declRef) override {
    addFunction(declRef.mangle());
  }

  void addFunction(StringRef name, SILDeclRef declRef) override {
    addFunction(name);
  }

  void addGlobalVar(VarDecl *VD) override {
    // FIXME: Handle global vars
    llvm::report_fatal_error("unhandled global var");
  }

  void addLinkEntity(LinkEntity entity) override {
    if (entity.hasSILFunction()) {
      addFunction(entity.getSILFunction());
      return;
    }

    auto constant = getAddrOfLLVMVariable(IGM, entity);
    if (constant) {
      auto global = cast<llvm::GlobalValue>(constant);
      Addrs.emplace_back(global);
    }
  }

  void addProtocolWitnessThunk(RootProtocolConformance *C,
                               ValueDecl *requirementDecl) override {
    // FIXME: Handle protocol witness thunks
    llvm::report_fatal_error("unhandled protocol witness thunk");
  }
};

static llvm::Constant *
getAddrOfLLVMVariableForClangDecl(IRGenModule &IGM, ValueDecl *decl,
                                  const clang::Decl *clangDecl) {
  if (isa<clang::FunctionDecl>(clangDecl)) {
    SILFunction *silFn =
        IGM.getSILModule().lookUpFunction(SILDeclRef(decl).asForeign());
    assert(silFn && "missing SIL function?");
    return silFn ? IGM.getAddrOfSILFunction(silFn, NotForDefinition) : nullptr;
  }

  if (auto var = dyn_cast<clang::ObjCInterfaceDecl>(clangDecl))
    return IGM.getAddrOfObjCClass(cast<ClassDecl>(decl), NotForDefinition);

  llvm::report_fatal_error("Unexpected clang decl kind");
}

static void
getSymbolAddrsForDecl(IRGenModule &IGM, ValueDecl *decl,
                      llvm::SmallVector<llvm::Constant *, 4> &addrs) {
  if (auto *clangDecl = decl->getClangDecl()) {
    if (auto *addr = getAddrOfLLVMVariableForClangDecl(IGM, decl, clangDecl))
      addrs.push_back(addr);
    return;
  }

  SILSymbolVisitorOptions opts;
  opts.VisitMembers = false;
  auto silCtx = SILSymbolVisitorContext(IGM.getNeXTCodeModule(), opts);
  auto linkInfo = UniversalLinkageInfo(IGM);
  auto symbolVisitorCtx = IRSymbolVisitorContext(linkInfo, silCtx);
  HasSymbolIRGenVisitor(IGM, addrs).visit(decl, symbolVisitorCtx);
}

llvm::Function *IRGenModule::emitHasSymbolFunction(ValueDecl *decl) {

  PrettyStackTraceDecl trace("emitting #_hasSymbol query for", decl);
  Mangle::ASTMangler mangler;

  auto func = cast<llvm::Function>(getOrCreateHelperFunction(
      mangler.mangleHasSymbolQuery(decl), Int1Ty, {},
      [decl, this](IRGenFunction &IGF) {
        auto &Builder = IGF.Builder;
        llvm::SmallVector<llvm::Constant *, 4> addrs;
        getSymbolAddrsForDecl(*this, decl, addrs);

        llvm::Value *ret = nullptr;
        for (llvm::Constant *addr : addrs) {
          assert(cast<llvm::GlobalValue>(addr)->hasExternalWeakLinkage());

          auto isNonNull = IGF.Builder.CreateIsNotNull(addr);
          ret = (ret) ? IGF.Builder.CreateAnd(ret, isNonNull) : isNonNull;
        }

        if (ret) {
          Builder.CreateRet(ret);
        } else {
          // There were no addresses produced by the visitor, return true.
          Builder.CreateRet(llvm::ConstantInt::get(Int1Ty, 1));
        }
      },
      /*IsNoInline*/ false));

  func->setDoesNotThrow();
  func->setCallingConv(DefaultCC);
  func->setOnlyReadsMemory();

  return func;
}
