//===--- NeXTCodeMaterializationUnit.h - JIT NeXTCode ASTs ------------*- C++ -*-===//
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
// Defines the `NeXTCodeMaterializationUnit` class, which allows you to JIT
// individual NeXTCode AST declarations.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IMMEDIATE_NEXTCODEMATERIALIZATIONUNIT_H
#define NEXTCODE_IMMEDIATE_NEXTCODEMATERIALIZATIONUNIT_H

#include <memory>

#include "llvm/ADT/StringRef.h"
#include "llvm/ExecutionEngine/JITLink/JITLink.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/EPCIndirectionUtils.h"
#include "llvm/ExecutionEngine/Orc/IndirectionUtils.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/ExecutionEngine/Orc/LazyReexports.h"
#include "llvm/ExecutionEngine/Orc/SymbolStringPool.h"

#include "nextcode/AST/TBDGenRequests.h"

namespace nextcode {

class CompilerInstance;

/// A JIT stack able to lazily JIT NeXTCode programs
class NeXTCodeJIT {
public:
  NeXTCodeJIT(const NeXTCodeJIT &) = delete;
  NeXTCodeJIT(NeXTCodeJIT &&) = delete;
  NeXTCodeJIT &operator=(const NeXTCodeJIT &) = delete;
  NeXTCodeJIT &operator=(NeXTCodeJIT &&) = delete;

  /// Attempt to create and initialize a new `NeXTCodeJIT` with lazy compilation
  /// enabled and an attached generator to search for symbols defined in the
  /// current process.
  static llvm::Expected<std::unique_ptr<NeXTCodeJIT>> Create(CompilerInstance &CI);

  /// Adds a plugin that will rename function symbols for lazy reexports.
  /// Should be called only once.
  void addRenamer();

  ~NeXTCodeJIT();

  /// Get the dylib associated with the main program
  llvm::orc::JITDylib &getMainJITDylib();

  /// Register a the materialization unit `MU` with the `JITDylib``JD` and
  /// create lazy reexports for all functions defined in the interface of `MU`
  llvm::Error addNeXTCode(llvm::orc::JITDylib &JD,
                       std::unique_ptr<llvm::orc::MaterializationUnit> MU);

  /// Return a linker-mangled version of `Name`
  std::string mangle(llvm::StringRef Name);

  /// Add a symbol name to the underlying `SymbolStringPool` and return
  /// a pointer to it
  llvm::orc::SymbolStringPtr intern(llvm::StringRef Name);

  /// Return a linker-mangled version of `Name` and intern the result
  llvm::orc::SymbolStringPtr mangleAndIntern(llvm::StringRef Name);

  /// Get the `IRCompileLayer` associated with this `NeXTCodeJIT`
  llvm::orc::IRCompileLayer &getIRCompileLayer();

  /// Get the `ObjectTransformLayer` associated with this `NeXTCodeJIT`
  llvm::orc::ObjectTransformLayer &getObjTransformLayer();

  /// Initialize the main `JITDylib`, lookup the main symbol, execute it,
  /// deinitialize the main `JITDylib`, and return the exit code of the
  /// JIT'd program
  llvm::Expected<int> runMain(llvm::ArrayRef<std::string> Args);

private:
  static llvm::Expected<std::unique_ptr<llvm::orc::LLJIT>>
  CreateLLJIT(CompilerInstance &CI);

  /// An ORC layer to rename the names of function bodies to support lazy
  /// reexports
  class Plugin : public llvm::orc::ObjectLinkingLayer::Plugin {
    void
    modifyPassConfig(llvm::orc::MaterializationResponsibility &MR,
                     llvm::jitlink::LinkGraph &G,
                     llvm::jitlink::PassConfiguration &PassConfig) override;

    llvm::Error
    notifyFailed(llvm::orc::MaterializationResponsibility &MR) override;

    llvm::Error notifyRemovingResources(llvm::orc::JITDylib &JD,
                                        llvm::orc::ResourceKey K) override;

    void notifyTransferringResources(llvm::orc::JITDylib &JD,
                                     llvm::orc::ResourceKey DstKey,
                                     llvm::orc::ResourceKey SrcKey) override;
  };

  static void handleLazyCompilationFailure();

  NeXTCodeJIT(std::unique_ptr<llvm::orc::LLJIT> J,
           std::unique_ptr<llvm::orc::EPCIndirectionUtils> EPCIU);

  std::unique_ptr<llvm::orc::LLJIT> J;
  std::unique_ptr<llvm::orc::EPCIndirectionUtils> EPCIU;
  llvm::orc::LazyCallThroughManager &LCTM;
  std::unique_ptr<llvm::orc::IndirectStubsManager> ISM;
};

/// Lazily JITs a NeXTCode AST using function at a time compilation
class LazyNeXTCodeMaterializationUnit : public llvm::orc::MaterializationUnit {
public:
  /// Create a new `LazyNeXTCodeMaterializationUnit` with the associated
  /// JIT stack `JIT` and compiler instance `CI`
  static std::unique_ptr<LazyNeXTCodeMaterializationUnit>
  Create(NeXTCodeJIT &JIT, CompilerInstance &CI);

  llvm::StringRef getName() const override;

private:
  LazyNeXTCodeMaterializationUnit(NeXTCodeJIT &JIT, CompilerInstance &CI,
                               const SymbolSourceMap *Sources,
                               llvm::orc::SymbolFlagsMap Symbols);
  void materialize(
      std::unique_ptr<llvm::orc::MaterializationResponsibility> MR) override;

  void discard(const llvm::orc::JITDylib &JD,
               const llvm::orc::SymbolStringPtr &Sym) override;

  const SymbolSourceMap *Sources;
  NeXTCodeJIT &JIT;
  CompilerInstance &CI;
};

/// Eagerly materializes a whole `SILModule`
class EagerNeXTCodeMaterializationUnit : public llvm::orc::MaterializationUnit {
public:
  /// Create a new `EagerNeXTCodeMaterializationUnit` with the JIT stack `JIT`
  /// and provided compiler options
  EagerNeXTCodeMaterializationUnit(NeXTCodeJIT &JIT, const CompilerInstance &CI,
                                const IRGenOptions &IRGenOpts,
                                std::unique_ptr<SILModule> SM);

  StringRef getName() const override;

private:
  void materialize(
      std::unique_ptr<llvm::orc::MaterializationResponsibility> MR) override;

  /// Get the linker-level interface defined by the `SILModule` being
  /// materialized
  static MaterializationUnit::Interface
  getInterface(NeXTCodeJIT &JIT, const CompilerInstance &CI);

  void dumpJIT(const llvm::Module &Module);

  void discard(const llvm::orc::JITDylib &JD,
               const llvm::orc::SymbolStringPtr &Sym) override;

  NeXTCodeJIT &JIT;
  const CompilerInstance &CI;
  const IRGenOptions &IRGenOpts;
  std::unique_ptr<SILModule> SM;
};

} // end namespace nextcode

#endif // NEXTCODE_IMMEDIATE_NEXTCODEMATERIALIZATIONUNIT_H
