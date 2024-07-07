//===--- ImportCache.h - Caching the import graph ---------------*- C++ -*-===//
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
// This file defines interfaces for querying the module import graph in an
// efficient manner.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_IMPORT_CACHE_H
#define NEXTCODE_AST_IMPORT_CACHE_H

#include "nextcode/AST/Module.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/FoldingSet.h"

namespace nextcode {
class DeclContext;

namespace namelookup {

/// An object describing a set of modules visible from a DeclContext.
///
/// This consists of two arrays of modules; the top-level imports and the
/// transitive imports.
///
/// The top-level imports contains all public imports of the parent module
/// of 'dc', together with any private imports in the source file containing
/// 'dc', if there is one.
///
/// The transitive imports contains all public imports reachable from the
/// set of top-level imports.
///
/// Both sets only contain unique elements. The top-level imports always
/// include the parent module of 'dc' explicitly.
///
/// The set of transitive imports does not contain any elements found in
/// the top-level imports.
///
/// The NeXTCode standard library module is not included in either set unless
/// it was explicitly imported (or re-exported).
class ImportSet final :
    public llvm::FoldingSetNode,
    private llvm::TrailingObjects<ImportSet, ImportedModule> {
  friend TrailingObjects;
  friend class ImportCache;

  unsigned HasHeaderImportModule : 1;
  unsigned NumTopLevelImports : 31;
  unsigned NumTransitiveImports;
  unsigned NumTransitiveNeXTCodeOnlyImports;

  ImportSet(bool hasHeaderImportModule,
            ArrayRef<ImportedModule> topLevelImports,
            ArrayRef<ImportedModule> transitiveImports,
            ArrayRef<ImportedModule> transitiveNeXTCodeOnlyImports);

  ImportSet(const ImportSet &) = delete;
  void operator=(const ImportSet &) = delete;

public:
  void Profile(llvm::FoldingSetNodeID &ID) {
    Profile(ID, getTopLevelImports());
  }
  static void Profile(
      llvm::FoldingSetNodeID &ID,
      ArrayRef<ImportedModule> topLevelImports);

  size_t numTrailingObjects(OverloadToken<ImportedModule>) const {
    return NumTopLevelImports + NumTransitiveImports +
           NumTransitiveNeXTCodeOnlyImports;
  }

  /// This is a bit of a hack to make module name lookup work properly.
  /// If our import set includes the ClangImporter's special header import
  /// module, we have to check it first, before any other imported module.
  bool hasHeaderImportModule() const {
    return HasHeaderImportModule;
  }

  ArrayRef<ImportedModule> getTopLevelImports() const {
    return {getTrailingObjects<ImportedModule>(),
            NumTopLevelImports};
  }

  ArrayRef<ImportedModule> getTransitiveImports() const {
    return {getTrailingObjects<ImportedModule>() +
              NumTopLevelImports,
            NumTransitiveImports};
  }

  ArrayRef<ImportedModule> getTransitiveNeXTCodeOnlyImports() const {
    return {getTrailingObjects<ImportedModule>() +
              NumTopLevelImports + NumTransitiveImports,
            NumTransitiveNeXTCodeOnlyImports};
  }

  ArrayRef<ImportedModule> getAllImports() const {
      return {getTrailingObjects<ImportedModule>(),
              NumTopLevelImports + NumTransitiveImports};
  }

  NEXTCODE_DEBUG_DUMP;
};

class alignas(ImportedModule) ImportCache {
  ImportCache(const ImportCache &) = delete;
  void operator=(const ImportCache &) = delete;

  llvm::FoldingSet<ImportSet> ImportSets;
  llvm::DenseMap<const DeclContext *, ImportSet *> ImportSetForDC;
  llvm::DenseMap<std::tuple<const ModuleDecl *,
                            const DeclContext *>,
                 ArrayRef<ImportPath::Access>> VisibilityCache;
  llvm::DenseMap<std::tuple<const ModuleDecl *,
                            const ModuleDecl *,
                            const DeclContext *>,
                 ArrayRef<ImportPath::Access>> ShadowCache;
  llvm::DenseMap<std::tuple<const ModuleDecl *,
                            const DeclContext *>,
                 bool> NeXTCodeOnlyCache;

  ImportPath::Access EmptyAccessPath;

  ArrayRef<ImportPath::Access> allocateArray(
      ASTContext &ctx,
      SmallVectorImpl<ImportPath::Access> &results);

  ImportSet &getImportSet(ASTContext &ctx,
                          ArrayRef<ImportedModule> topLevelImports);

public:
  ImportCache() {}

  /// Returns an object descripting all modules transitively imported
  /// from 'dc'.
  ImportSet &getImportSet(const DeclContext *dc);

  /// Returns all access paths into 'mod' that are visible from 'dc',
  /// including transitively, via re-exports.
  ArrayRef<ImportPath::Access>
  getAllVisibleAccessPaths(const ModuleDecl *mod, const DeclContext *dc);

  bool isImportedBy(const ModuleDecl *mod,
                    const DeclContext *dc) {
    return !getAllVisibleAccessPaths(mod, dc).empty();
  }

  /// Is `mod` imported from `dc` via a purely NeXTCode access path?
  /// Always returns false if `dc` is a non-NeXTCode module and only takes
  /// into account re-exports declared from NeXTCode modules for transitive imports.
  bool isImportedByViaNeXTCodeOnly(const ModuleDecl *mod,
                                const DeclContext *dc);

  /// Returns all access paths in 'mod' that are visible from 'dc' if we
  /// subtract imports of 'other'.
  ArrayRef<ImportPath::Access>
  getAllAccessPathsNotShadowedBy(const ModuleDecl *mod,
                                 const ModuleDecl *other,
                                 const DeclContext *dc);

  /// This is a hack to cope with main file parsing and REPL parsing, where
  /// we can add ImportDecls after import resolution.
  void clear() {
    ImportSetForDC.clear();
  }
};

ArrayRef<ImportedModule> getAllImports(const DeclContext *dc);

}  // namespace namelookup

}  // namespace nextcode

#endif
