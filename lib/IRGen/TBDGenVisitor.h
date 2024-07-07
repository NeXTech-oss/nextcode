//===--- TBDGenVisitor.h - AST Visitor for TBD generation -----------------===//
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
//  This file defines the visitor that finds all symbols in a nextcode AST.
//
//===----------------------------------------------------------------------===//
#ifndef NEXTCODE_TBDGEN_TBDGENVISITOR_H
#define NEXTCODE_TBDGEN_TBDGENVISITOR_H

#include "nextcode/AST/ASTMangler.h"
#include "nextcode/AST/FileUnit.h"
#include "nextcode/AST/Module.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/IRGen/IRSymbolVisitor.h"
#include "nextcode/IRGen/Linking.h"
#include "nextcode/SIL/SILDeclRef.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/TargetParser/Triple.h"
#include "llvm/TextAPI/InterfaceFile.h"

using namespace nextcode::irgen;
using StringSet = llvm::StringSet<>;

namespace llvm {
class DataLayout;
}

namespace nextcode {

class TBDGenDescriptor;
struct TBDGenOptions;
class SymbolSource;

namespace tbdgen {

enum class LinkerPlatformId: uint8_t {
#define LD_PLATFORM(Name, Id) Name = Id,
#include "ldPlatformKinds.def"
};

struct InstallNameStore {
  // The default install name to use when no specific install name is specified.
  std::string InstallName;
  // The install name specific to the platform id. This takes precedence over
  // the default install name.
  std::map<LinkerPlatformId, std::string> PlatformInstallName;
  StringRef getInstallName(LinkerPlatformId Id) const;
};

/// A set of callbacks for recording APIs.
class APIRecorder {
public:
  virtual ~APIRecorder() {}

  virtual void addSymbol(StringRef name, llvm::MachO::EncodeKind kind,
                         SymbolSource source, Decl *decl,
                         llvm::MachO::SymbolFlags flags) {}
  virtual void addObjCInterface(const ClassDecl *decl) {}
  virtual void addObjCCategory(const ExtensionDecl *decl) {}
  virtual void addObjCMethod(const GenericContext *ctx, SILDeclRef method) {}
};

class SimpleAPIRecorder final : public APIRecorder {
public:
  using SymbolCallbackFn =
      llvm::function_ref<void(StringRef, llvm::MachO::EncodeKind, SymbolSource,
                              Decl *, llvm::MachO::SymbolFlags)>;

  SimpleAPIRecorder(SymbolCallbackFn func) : func(func) {}

  void addSymbol(StringRef symbol, llvm::MachO::EncodeKind kind,
                 SymbolSource source, Decl *decl,
                 llvm::MachO::SymbolFlags flags) override {
    func(symbol, kind, source, decl, flags);
  }

private:
  SymbolCallbackFn func;
};

class TBDGenVisitor : public IRSymbolVisitor {
#ifndef NDEBUG
  /// Tracks the symbols emitted to ensure we don't emit any duplicates.
  llvm::StringSet<> DuplicateSymbolChecker;
#endif

  std::optional<llvm::DataLayout> DataLayout = std::nullopt;
  const StringRef DataLayoutDescription;

  UniversalLinkageInfo UniversalLinkInfo;
  ModuleDecl *NeXTCodeModule;
  const TBDGenOptions &Opts;
  APIRecorder &recorder;

  using EncodeKind = llvm::MachO::EncodeKind;
  using SymbolFlags = llvm::MachO::SymbolFlags;

  std::vector<Decl*> DeclStack;
  std::unique_ptr<std::map<std::string, InstallNameStore>>
    previousInstallNameMap;
  std::unique_ptr<std::map<std::string, InstallNameStore>>
    parsePreviousModuleInstallNameMap();
  void addSymbolInternal(StringRef name, EncodeKind kind, SymbolSource source,
                         SymbolFlags);
  void addLinkerDirectiveSymbolsLdHide(StringRef name, EncodeKind kind);
  void addLinkerDirectiveSymbolsLdPrevious(StringRef name, EncodeKind kind);
  void addSymbol(StringRef name, SymbolSource source, SymbolFlags flags,
                 EncodeKind kind = EncodeKind::GlobalSymbol);

  void addSymbol(LinkEntity entity);
  
  bool addClassMetadata(ClassDecl *CD);

public:
  TBDGenVisitor(const llvm::Triple &target, const StringRef dataLayoutString,
                ModuleDecl *nextcodeModule, const TBDGenOptions &opts,
                APIRecorder &recorder)
      : DataLayoutDescription(dataLayoutString),
        UniversalLinkInfo(target, opts.HasMultipleIGMs, /*forcePublic*/ false,
                          /*static=*/false),
        NeXTCodeModule(nextcodeModule), Opts(opts), recorder(recorder),
        previousInstallNameMap(parsePreviousModuleInstallNameMap()) {}

  /// Create a new visitor using the target and layout information from a
  /// TBDGenDescriptor.
  TBDGenVisitor(const TBDGenDescriptor &desc, APIRecorder &recorder);

  ~TBDGenVisitor() { assert(DeclStack.empty()); }

  /// Adds the global symbols associated with the first file.
  void addFirstFileSymbols();

  /// Visit the files specified by a given TBDGenDescriptor.
  void visit(const TBDGenDescriptor &desc);

  // --- IRSymbolVisitor ---

  bool willVisitDecl(Decl *D) override;
  void didVisitDecl(Decl *D) override;

  void addFunction(SILDeclRef declRef) override;
  void addFunction(StringRef name, SILDeclRef declRef) override;
  void addGlobalVar(VarDecl *VD) override;
  void addLinkEntity(LinkEntity entity) override;
  void addObjCInterface(ClassDecl *CD) override;
  void addObjCMethod(AbstractFunctionDecl *AFD) override;
  void addProtocolWitnessThunk(RootProtocolConformance *C,
                               ValueDecl *requirementDecl) override;
};
} // end namespace tbdgen
} // end namespace nextcode

#endif
