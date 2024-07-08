//===--- SymbolGraphGen.cpp - Symbol Graph Generator Entry Point ----------===//
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

#include "nextcode/SymbolGraphGen/SymbolGraphGen.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/FileSystem.h"
#include "nextcode/AST/Import.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/NameLookup.h"
#include "nextcode/Sema/IDETypeChecking.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/Path.h"

#include "SymbolGraphASTWalker.h"

using namespace nextcode;
using namespace symbolgraphgen;

namespace {
int serializeSymbolGraph(SymbolGraph &SG,
                         const SymbolGraphOptions &Options) {
  SmallString<256> FileName;
  FileName.append(SG.M.getNameStr());
  if (SG.ExtendedModule.has_value()) {
    FileName.push_back('@');
    FileName.append(SG.ExtendedModule.value()->getNameStr());
  } else if (SG.DeclaringModule.has_value()) {
    // Treat cross-import overlay modules as "extensions" of their declaring module
    FileName.push_back('@');
    FileName.append(SG.DeclaringModule.value()->getNameStr());
  }
  FileName.append(".symbols.json");

  SmallString<1024> OutputPath(Options.OutputDir);
  llvm::sys::path::append(OutputPath, FileName);

  return withOutputPath(
      SG.M.getASTContext().Diags, SG.M.getASTContext().getOutputBackend(),
      OutputPath, [&](raw_ostream &OS) {
        llvm::json::OStream J(OS, Options.PrettyPrint ? 2 : 0);
        SG.serialize(J);
        return false;
      });
}

} // end anonymous namespace

// MARK: - Main Entry Point

/// Emit a symbol graph JSON file for a `ModuleDecl`.
int symbolgraphgen::emitSymbolGraphForModule(
    ModuleDecl *M, const SymbolGraphOptions &Options) {
  ModuleDecl::ImportCollector importCollector(Options.MinimumAccessLevel);

  auto importFilter = [&Options](const ModuleDecl *module) {
      if (!module)
        return false;

      for (const auto &allowedModuleName : *Options.AllowedReexportedModules)
        if (allowedModuleName == module->getNameStr())
          return true;

       return false;
    };

  if (Options.AllowedReexportedModules.has_value())
    importCollector.importFilter = std::move(importFilter);

  SmallVector<Decl *, 64> ModuleDecls;
  nextcode::getTopLevelDeclsForDisplay(
      M, ModuleDecls, [&importCollector](ModuleDecl *M, SmallVectorImpl<Decl *> &results) {
        M->getDisplayDeclsRecursivelyAndImports(results, importCollector);
      });

  if (Options.PrintMessages)
    llvm::errs() << ModuleDecls.size()
        << " top-level declarations in this module.\n";

  SymbolGraphASTWalker Walker(*M, importCollector.imports,
                              importCollector.qualifiedImports, Options);

  for (auto *Decl : ModuleDecls) {
    Walker.walk(Decl);
  }

  if (Options.PrintMessages)
    llvm::errs()
      << "Found " << Walker.MainGraph.Nodes.size() << " symbols and "
      << Walker.MainGraph.Edges.size() << " relationships.\n";

  int Success = EXIT_SUCCESS;

  Success |= serializeSymbolGraph(Walker.MainGraph, Options);

  for (const auto &Entry : Walker.ExtendedModuleGraphs) {
    if (Entry.getValue()->empty()) {
      continue;
    }
    Success |= serializeSymbolGraph(*Entry.getValue(), Options);
  }

  return Success;
}

int symbolgraphgen::
printSymbolGraphForDecl(const ValueDecl *D, Type BaseTy,
                        bool InSynthesizedExtension,
                        const SymbolGraphOptions &Options,
                        llvm::raw_ostream &OS,
                        SmallVectorImpl<PathComponent> &ParentContexts,
                        SmallVectorImpl<FragmentInfo> &FragmentInfo) {
  if (!Symbol::supportsKind(D->getKind()))
    return EXIT_FAILURE;

  llvm::json::OStream JOS(OS, Options.PrettyPrint ? 2 : 0);
  ModuleDecl *MD = D->getModuleContext();
  SymbolGraphASTWalker Walker(*MD, Options);
  markup::MarkupContext MarkupCtx;
  SymbolGraph Graph(Walker, *MD, std::nullopt, MarkupCtx, std::nullopt,
                    /*IsForSingleNode=*/true);
  NominalTypeDecl *NTD = InSynthesizedExtension
      ? BaseTy->getAnyNominal()
      : nullptr;

  Symbol MySym(&Graph, D, NTD, BaseTy);
  MySym.getPathComponents(ParentContexts);
  MySym.getFragmentInfo(FragmentInfo);
  Graph.recordNode(MySym);
  Graph.serialize(JOS);
  return EXIT_SUCCESS;
}
