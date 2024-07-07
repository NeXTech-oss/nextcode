//===--- TBD.cpp -- generates and validates TBD files ---------------------===//
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
//             Tural Ghuliev (turalquliyev@it-gss.com)
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//

#include "TBD.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/AST/DiagnosticsFrontend.h"
#include "nextcode/AST/FileSystem.h"
#include "nextcode/AST/FileUnit.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/TBDGenRequests.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/Demangling/Demangle.h"
#include "nextcode/Frontend/FrontendOptions.h"
#include "nextcode/IRGen/TBDGen.h"

#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Mangler.h"
#include "llvm/IR/ValueSymbolTable.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/VirtualOutputBackend.h"
#include <vector>

using namespace nextcode;

static std::vector<StringRef> sortSymbols(llvm::StringSet<> &symbols) {
  std::vector<StringRef> sorted;
  for (auto &symbol : symbols)
    sorted.push_back(symbol.getKey());
  std::sort(sorted.begin(), sorted.end());
  return sorted;
}

bool nextcode::writeTBD(ModuleDecl *M, StringRef OutputFilename,
                     llvm::vfs::OutputBackend &Backend,
                     const TBDGenOptions &Opts) {
  return withOutputPath(M->getDiags(), Backend, OutputFilename,
                        [&](raw_ostream &OS) -> bool {
                          writeTBDFile(M, OS, Opts);
                          return false;
                        });
}

static bool validateSymbols(DiagnosticEngine &diags,
                            const std::vector<std::string> &symbols,
                            const llvm::Module &IRModule,
                            bool diagnoseExtraSymbolsInTBD) {
  llvm::StringSet<> symbolSet;
  symbolSet.insert(symbols.begin(), symbols.end());

  auto error = false;

  // Diff the two sets of symbols, flagging anything outside their intersection.

  // Delay the emission of errors for things in the IR but not TBD, so we can
  // sort them to get a stable order.
  std::vector<StringRef> irNotTBD;

  for (auto &nameValue : IRModule.getValueSymbolTable()) {
    // TBDGen inserts mangled names (usually with a leading '_') into its
    // symbol table, so make sure to mangle IRGen names before comparing them
    // with what TBDGen created.
    auto unmangledName = nameValue.getKey();

    SmallString<128> name;
    llvm::Mangler::getNameWithPrefix(name, unmangledName,
                                     IRModule.getDataLayout());

    auto value = nameValue.getValue();
    if (auto GV = dyn_cast<llvm::GlobalValue>(value)) {
      // Is this a symbol that should be listed?
      auto externallyVisible =
          (GV->hasExternalLinkage() || GV->hasCommonLinkage())
        && !GV->hasHiddenVisibility();
      if (!GV->isDeclaration() && externallyVisible) {
        // Is it listed?
        if (!symbolSet.erase(name))
          // Note: Add the unmangled name to the irNotTBD list, which is owned
          //       by the IRModule, instead of the mangled name.
          irNotTBD.push_back(unmangledName);
      }
    } else {
      assert(!symbolSet.contains(name) &&
             "non-global value in value symbol table");
    }
  }

  std::sort(irNotTBD.begin(), irNotTBD.end());
  for (auto &name : irNotTBD) {
    diags.diagnose(SourceLoc(), diag::symbol_in_ir_not_in_tbd, name,
                   Demangle::demangleSymbolAsString(name));
    error = true;
  }

  if (diagnoseExtraSymbolsInTBD) {
    // Look for any extra symbols.
    for (auto &name : sortSymbols(symbolSet)) {
      diags.diagnose(SourceLoc(), diag::symbol_in_tbd_not_in_ir, name,
                     Demangle::demangleSymbolAsString(name));
      error = true;
    }
  }

  if (error) {
    diags.diagnose(SourceLoc(), diag::tbd_validation_failure);
  }

  return error;
}

bool nextcode::validateTBD(ModuleDecl *M,
                        const llvm::Module &IRModule,
                        const TBDGenOptions &opts,
                        bool diagnoseExtraSymbolsInTBD) {
  auto symbols = getPublicSymbols(TBDGenDescriptor::forModule(M, opts));
  return validateSymbols(M->getASTContext().Diags, symbols, IRModule,
                         diagnoseExtraSymbolsInTBD);
}

bool nextcode::validateTBD(FileUnit *file,
                        const llvm::Module &IRModule,
                        const TBDGenOptions &opts,
                        bool diagnoseExtraSymbolsInTBD) {
  auto symbols = getPublicSymbols(TBDGenDescriptor::forFile(file, opts));
  return validateSymbols(file->getParentModule()->getASTContext().Diags,
                         symbols, IRModule, diagnoseExtraSymbolsInTBD);
}
