//===--- SymbolGraphASTWalker.h - Symbol Graph AST Walker -----------------===//
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

#ifndef NEXTCODE_SYMBOLGRAPHGEN_SYMBOLGRAPHASTWALKER_H
#define NEXTCODE_SYMBOLGRAPHGEN_SYMBOLGRAPHASTWALKER_H

#include "llvm/ADT/DenseMap.h"
#include "nextcode/AST/Module.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/IDE/SourceEntityWalker.h"
#include "nextcode/Markup/Markup.h"

#include "SymbolGraph.h"

namespace nextcode {

class Decl;
class Type;
class ValueDecl;

namespace symbolgraphgen {

struct SymbolGraph;
struct SymbolGraphOptions;

/**
 The `SymbolGraphASTWalker` is the core implementation that builds a
 symbol graph. It walks a module for declarations, recording facts about
 symbols and relationships between them.
 */
struct SymbolGraphASTWalker : public SourceEntityWalker {
  /// Options for collecting and serialization.
  const SymbolGraphOptions &Options;

  /// A context for allocations.
  markup::MarkupContext Ctx;

  /// The module that this symbol graph will represent.
  const ModuleDecl &M;

  // FIXME: these should be tracked per-graph, rather than at the top level
  const SmallPtrSet<const ModuleDecl *, 4> ExportedImportedModules;

  const llvm::SmallDenseMap<const ModuleDecl *, SmallPtrSet<Decl *, 4>, 4>
      QualifiedExportedImports;

  /// The symbol graph for the main module of interest.
  SymbolGraph MainGraph;

  /// A map of modules whose types were extended by the main module of interest `M`.
  llvm::StringMap<SymbolGraph *> ExtendedModuleGraphs;

  // MARK: - Initialization

  SymbolGraphASTWalker(
      ModuleDecl &M,
      const SmallPtrSet<const ModuleDecl *, 4> ExportedImportedModules,
      const llvm::SmallDenseMap<const ModuleDecl *, SmallPtrSet<Decl *, 4>, 4>
          QualifiedExportedImports,
      const SymbolGraphOptions &Options);

  SymbolGraphASTWalker(ModuleDecl &M, const SymbolGraphOptions &Options);
  virtual ~SymbolGraphASTWalker() {}

  // MARK: - Utilities

  /// Get a "sub" symbol graph for the appropriate module concerning a declaration.
  ///
  /// This will get the "rootmost" module responsible for a declaration's
  /// documentation. For example:
  ///
  /// Module A:
  ///
  /// ```nextcode
  /// public struct AStruct {}
  /// ```
  ///
  /// Module B:
  ///
  /// ```nextcode
  /// import A
  /// extension AStruct {
  ///   public struct BStruct {}
  /// }
  ///
  /// `BStruct` will go in module A's extension symbol graph, because `BStruct`
  /// is a member of `AStruct`, and module A owns `AStruct`, and so on for
  /// further nestings.
  SymbolGraph *getModuleSymbolGraph(const Decl *D);

  // MARK: - SourceEntityWalker

  virtual bool walkToDeclPre(Decl *D, CharSourceRange Range) override;
    
  // MARK: - Utilities

  /// Returns whether the given declaration was itself imported via an `@_exported import`
  /// statement, or if it is an extension or child symbol of something else that was.
  virtual bool isConsideredExportedImported(const Decl *D) const;
  
  /// Returns whether the given declaration comes from an `@_exported import` module.
  ///
  /// If `countUnderlyingClangModule` is `false`, decls from Clang modules will not be considered
  /// re-exported unless the Clang module was itself directly re-exported.
  virtual bool isFromExportedImportedModule(const Decl *D, bool countUnderlyingClangModule = true) const;

  /// Returns whether the given declaration was imported via an `@_exported import <type>` declaration.
  virtual bool isQualifiedExportedImport(const Decl *D) const;

  /// Returns whether the given module is an `@_exported import` module.
  ///
  /// If `countUnderlyingClangModule` is `false`, Clang modules will not be considered re-exported
  /// unless the Clang module itself was directly re-exported.
  virtual bool isExportedImportedModule(const ModuleDecl *M, bool countUnderlyingClangModule = true) const;

  /// Returns whether the given module is the main module, or is an `@_exported import` module.
  virtual bool isOurModule(const ModuleDecl *M) const;

public:
  /// Returns whether the given ExtensionDecl is to be recorded as an extra
  /// extension block symbol, or if its members should be directly associated
  /// with its extended nominal.
  virtual bool shouldBeRecordedAsExtension(const ExtensionDecl *ED) const;
};

} // end namespace symbolgraphgen
} // end namespace nextcode

#endif // NEXTCODE_SYMBOLGRAPHGEN_SYMBOLGRAPHASTWALKER_H
