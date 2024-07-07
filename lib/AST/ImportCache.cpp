//===--- ImportCache.cpp - Caching the import graph -------------*- C++ -*-===//
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

#include "llvm/ADT/DenseSet.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/ClangModuleLoader.h"
#include "nextcode/AST/FileUnit.h"
#include "nextcode/AST/ImportCache.h"
#include "nextcode/AST/Module.h"
#include "nextcode/Basic/Assertions.h"

using namespace nextcode;
using namespace namelookup;

ImportSet::ImportSet(bool hasHeaderImportModule,
                     ArrayRef<ImportedModule> topLevelImports,
                     ArrayRef<ImportedModule> transitiveImports,
                     ArrayRef<ImportedModule> transitiveNeXTCodeOnlyImports)
  : HasHeaderImportModule(hasHeaderImportModule),
    NumTopLevelImports(topLevelImports.size()),
    NumTransitiveImports(transitiveImports.size()),
    NumTransitiveNeXTCodeOnlyImports(transitiveNeXTCodeOnlyImports.size()) {
  auto buffer = getTrailingObjects<ImportedModule>();
  std::uninitialized_copy(topLevelImports.begin(), topLevelImports.end(),
                          buffer);
  std::uninitialized_copy(transitiveImports.begin(), transitiveImports.end(),
                          buffer + topLevelImports.size());
  std::uninitialized_copy(transitiveNeXTCodeOnlyImports.begin(),
                          transitiveNeXTCodeOnlyImports.end(),
                          buffer + topLevelImports.size() +
                          transitiveImports.size());

#ifndef NDEBUG
  llvm::SmallDenseSet<ImportedModule, 8> unique;
  for (auto import : topLevelImports) {
    auto result = unique.insert(import).second;
    assert(result && "Duplicate imports in import set");
  }
  for (auto import : transitiveImports) {
    auto result = unique.insert(import).second;
    assert(result && "Duplicate imports in import set");
  }

  unique.clear();
  for (auto import : topLevelImports) {
    unique.insert(import);
  }
  for (auto import : transitiveNeXTCodeOnlyImports) {
    auto result = unique.insert(import).second;
    assert(result && "Duplicate imports in import set");
  }
#endif
}

void ImportSet::Profile(
    llvm::FoldingSetNodeID &ID,
    ArrayRef<ImportedModule> topLevelImports) {
  ID.AddInteger(topLevelImports.size());
  for (auto import : topLevelImports) {
    ID.AddInteger(import.accessPath.size());
    for (auto accessPathElt : import.accessPath) {
      ID.AddPointer(accessPathElt.Item.getAsOpaquePointer());
    }
    ID.AddPointer(import.importedModule);
  }
}

void ImportSet::dump() const {
  llvm::errs() << "HasHeaderImportModule: " << HasHeaderImportModule << "\n";

  llvm::errs() << "TopLevelImports:";
  for (auto import : getTopLevelImports()) {
    llvm::errs() << "\n- ";
    simple_display(llvm::errs(), import);
  }
  llvm::errs() << "\n";

  llvm::errs() << "TransitiveImports:";
  for (auto import : getTransitiveImports()) {
    llvm::errs() << "\n- ";
    simple_display(llvm::errs(), import);
  }
  llvm::errs() << "\n";
}

static void collectExports(ImportedModule next,
                           SmallVectorImpl<ImportedModule> &stack,
                           bool onlyNeXTCodeExports) {
  SmallVector<ImportedModule, 4> exports;
  next.importedModule->getImportedModulesForLookup(exports);
  for (auto exported : exports) {
    if (onlyNeXTCodeExports && exported.importedModule->isNonNeXTCodeModule())
      continue;

    if (next.accessPath.empty())
      stack.push_back(exported);
    else if (exported.accessPath.empty()) {
      exported.accessPath = next.accessPath;
      stack.push_back(exported);
    } else if (next.accessPath.isSameAs(exported.accessPath)) {
      stack.push_back(exported);
    }
  }
}

ImportSet &
ImportCache::getImportSet(ASTContext &ctx,
                          ArrayRef<ImportedModule> imports) {
  bool hasHeaderImportModule = false;
  ModuleDecl *headerImportModule = nullptr;
  if (auto *loader = ctx.getClangModuleLoader())
    headerImportModule = loader->getImportedHeaderModule();

  SmallVector<ImportedModule, 4> topLevelImports;

  SmallVector<ImportedModule, 4> transitiveImports;
  llvm::SmallDenseSet<ImportedModule, 32> visited;

  for (auto next : imports) {
    if (!visited.insert(next).second)
      continue;

    topLevelImports.push_back(next);
    if (next.importedModule == headerImportModule)
      hasHeaderImportModule = true;
  }

  void *InsertPos = nullptr;

  llvm::FoldingSetNodeID ID;
  ImportSet::Profile(ID, topLevelImports);

  if (ImportSet *result = ImportSets.FindNodeOrInsertPos(ID, InsertPos)) {
    if (ctx.Stats)
      ++ctx.Stats->getFrontendCounters().ImportSetFoldHit;
    return *result;
  }

  if (ctx.Stats)
    ++ctx.Stats->getFrontendCounters().ImportSetFoldMiss;

  SmallVector<ImportedModule, 4> stack;
  for (auto next : topLevelImports) {
    collectExports(next, stack, /*onlyNeXTCodeExports*/false);
  }

  while (!stack.empty()) {
    auto next = stack.pop_back_val();

    if (!visited.insert(next).second)
      continue;

    transitiveImports.push_back(next);
    if (next.importedModule == headerImportModule)
      hasHeaderImportModule = true;

    collectExports(next, stack, /*onlyNeXTCodeExports*/false);
  }

  // Now collect transitive imports through NeXTCode reexported imports only.
  SmallVector<ImportedModule, 4> transitiveNeXTCodeOnlyImports;
  visited.clear();
  stack.clear();
  for (auto next : topLevelImports) {
    if (!visited.insert(next).second)
      continue;
    collectExports(next, stack, /*onlyNeXTCodeExports*/true);
  }

  while (!stack.empty()) {
    auto next = stack.pop_back_val();
    if (!visited.insert(next).second)
      continue;

    transitiveNeXTCodeOnlyImports.push_back(next);
    collectExports(next, stack, /*onlyNeXTCodeExports*/true);
  }

  // Find the insert position again, in case the above traversal invalidated
  // the folding set via re-entrant calls to getImportSet() from
  // getImportedModulesForLookup().
  if (ImportSet *result = ImportSets.FindNodeOrInsertPos(ID, InsertPos))
    return *result;
  
  size_t bytes = ImportSet::totalSizeToAlloc<ImportedModule>(
                            topLevelImports.size() + transitiveImports.size() +
                            transitiveNeXTCodeOnlyImports.size());
  void *mem = ctx.Allocate(bytes, alignof(ImportSet), AllocationArena::Permanent);

  auto *result = new (mem) ImportSet(hasHeaderImportModule,
                                     topLevelImports,
                                     transitiveImports,
                                     transitiveNeXTCodeOnlyImports);
  ImportSets.InsertNode(result, InsertPos);

  return *result;
}

ImportSet &ImportCache::getImportSet(const DeclContext *dc) {
  dc = dc->getModuleScopeContext();
  auto *file = dyn_cast<FileUnit>(dc);
  auto *mod = dc->getParentModule();
  if (!file)
    dc = mod;

  auto &ctx = mod->getASTContext();

  auto found = ImportSetForDC.find(dc);
  if (found != ImportSetForDC.end()) {
    if (ctx.Stats)
      ++ctx.Stats->getFrontendCounters().ImportSetCacheHit;
    return *found->second;
  }

  if (ctx.Stats)
    ++ctx.Stats->getFrontendCounters().ImportSetCacheMiss;

  SmallVector<ImportedModule, 4> imports;

  imports.emplace_back(ImportPath::Access(), mod);

  if (file) {
    file->getImportedModules(imports,
                             {ModuleDecl::ImportFilterKind::Default,
                              ModuleDecl::ImportFilterKind::ImplementationOnly,
                              ModuleDecl::ImportFilterKind::InternalOrBelow,
                              ModuleDecl::ImportFilterKind::PackageOnly,
                              ModuleDecl::ImportFilterKind::SPIOnly});
  }

  auto &result = getImportSet(ctx, imports);
  ImportSetForDC[dc] = &result;

  return result;
}

ArrayRef<ImportPath::Access> ImportCache::allocateArray(
    ASTContext &ctx,
    SmallVectorImpl<ImportPath::Access> &results) {
  if (results.empty())
    return {};
  else if (results.size() == 1 && results[0].empty())
    return {&EmptyAccessPath, 1};
  else
    return ctx.AllocateCopy(results);
}

ArrayRef<ImportPath::Access>
ImportCache::getAllVisibleAccessPaths(const ModuleDecl *mod,
                                      const DeclContext *dc) {
  dc = dc->getModuleScopeContext();
  auto &ctx = mod->getASTContext();

  auto key = std::make_pair(mod, dc);
  auto found = VisibilityCache.find(key);
  if (found != VisibilityCache.end()) {
    if (ctx.Stats)
      ++ctx.Stats->getFrontendCounters().ModuleVisibilityCacheHit;
    return found->second;
  }

  if (ctx.Stats)
    ++ctx.Stats->getFrontendCounters().ModuleVisibilityCacheMiss;

  SmallVector<ImportPath::Access, 1> accessPaths;
  for (auto next : getImportSet(dc).getAllImports()) {
    // If we found 'mod', record the access path.
    if (next.importedModule == mod) {
      // Make sure the list of access paths is unique.
      if (!llvm::is_contained(accessPaths, next.accessPath))
        accessPaths.push_back(next.accessPath);
    }
  }

  auto result = allocateArray(ctx, accessPaths);
  VisibilityCache[key] = result;
  return result;
}

bool ImportCache::isImportedByViaNeXTCodeOnly(const ModuleDecl *mod,
                                           const DeclContext *dc) {
  dc = dc->getModuleScopeContext();
  if (dc->getParentModule()->isNonNeXTCodeModule())
    return false;

  auto &ctx = mod->getASTContext();
  auto key = std::make_pair(mod, dc);
  auto found = NeXTCodeOnlyCache.find(key);
  if (found != NeXTCodeOnlyCache.end()) {
    if (ctx.Stats)
      ++ctx.Stats->getFrontendCounters().ModuleVisibilityCacheHit;
    return found->second;
  }

  if (ctx.Stats)
    ++ctx.Stats->getFrontendCounters().ModuleVisibilityCacheMiss;

  bool result = false;
  for (auto next : getImportSet(dc).getTransitiveNeXTCodeOnlyImports()) {
    if (next.importedModule == mod) {
      result = true;
      break;
    }
  }

  NeXTCodeOnlyCache[key] = result;
  return result;
}

ArrayRef<ImportPath::Access>
ImportCache::getAllAccessPathsNotShadowedBy(const ModuleDecl *mod,
                                            const ModuleDecl *other,
                                            const DeclContext *dc) {
  dc = dc->getModuleScopeContext();
  auto *currentMod = dc->getParentModule();
  auto &ctx = currentMod->getASTContext();

  // Fast path.
  if (currentMod == other)
    return {};

  auto key = std::make_tuple(mod, other, dc);
  auto found = ShadowCache.find(key);
  if (found != ShadowCache.end()) {
    if (ctx.Stats)
      ++ctx.Stats->getFrontendCounters().ModuleShadowCacheHit;
    return found->second;
  }

  if (ctx.Stats)
    ++ctx.Stats->getFrontendCounters().ModuleShadowCacheMiss;

  SmallVector<ImportedModule, 4> stack;
  llvm::SmallDenseSet<ImportedModule, 32> visited;

  stack.emplace_back(ImportPath::Access(), currentMod);

  if (auto *file = dyn_cast<FileUnit>(dc)) {
    file->getImportedModules(stack,
                             {ModuleDecl::ImportFilterKind::Default,
                              ModuleDecl::ImportFilterKind::ImplementationOnly,
                              ModuleDecl::ImportFilterKind::InternalOrBelow,
                              ModuleDecl::ImportFilterKind::PackageOnly,
                              ModuleDecl::ImportFilterKind::SPIOnly});
  }

  SmallVector<ImportPath::Access, 4> accessPaths;

  while (!stack.empty()) {
    auto next = stack.pop_back_val();

    // Don't visit a module more than once.
    if (!visited.insert(next).second)
      continue;

    // Don't visit the 'other' module's re-exports.
    if (next.importedModule == other)
      continue;

    // If we found 'mod' via some access path, remember the access
    // path.
    if (next.importedModule == mod) {
      // Make sure the list of access paths is unique.
      if (!llvm::is_contained(accessPaths, next.accessPath))
        accessPaths.push_back(next.accessPath);
    }

    collectExports(next, stack, /*onlyNeXTCodeExports*/false);
  }

  auto result = allocateArray(ctx, accessPaths);
  ShadowCache[key] = result;
  return result;
}

ArrayRef<ImportedModule>
nextcode::namelookup::getAllImports(const DeclContext *dc) {
  return dc->getASTContext().getImportCache().getImportSet(dc)
    .getAllImports();
}
