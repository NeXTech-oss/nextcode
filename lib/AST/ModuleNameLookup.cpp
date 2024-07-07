//===--- ModuleNameLookup.cpp - Name lookup within a module ---------------===//
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

#include "nextcode/AST/ModuleNameLookup.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/ClangModuleLoader.h"
#include "nextcode/AST/ImportCache.h"
#include "nextcode/AST/NameLookup.h"
#include "nextcode/AST/NameLookupRequests.h"
#include "nextcode/Basic/Assertions.h"
#include "llvm/Support/raw_ostream.h"

using namespace nextcode;
using namespace namelookup;

namespace {

/// Encapsulates the work done for a recursive qualified lookup into a module.
///
/// The \p LookupStrategy handles the non-recursive part of the lookup. It
/// must be a subclass of ModuleNameLookup.
template <typename LookupStrategy>
class ModuleNameLookup {
  ASTContext &ctx;
  const ResolutionKind resolutionKind;
  const bool respectAccessControl;

  LookupStrategy *getDerived() {
    static_assert(std::is_base_of<ModuleNameLookup<LookupStrategy>,
                                  LookupStrategy>::value,
                  "ModuleNameLookup is a CRTP class");
    return static_cast<LookupStrategy *>(this);
  }

public:
  ModuleNameLookup(ASTContext &ctx, ResolutionKind resolutionKind)
      : ctx(ctx),
        resolutionKind(resolutionKind),
        respectAccessControl(!ctx.isAccessControlDisabled()) {}

  /// Performs a qualified lookup into the given module and, if necessary, its
  /// reexports.
  ///
  /// The results are appended to \p decls.
  void lookupInModule(SmallVectorImpl<ValueDecl *> &decls,
                      const DeclContext *moduleOrFile,
                      ImportPath::Access accessPath,
                      const DeclContext *moduleScopeContext,
                      NLOptions options);
};

// Exclude names introduced by macro expansions.
enum class LocalLookupFlags {
  ExcludeMacroExpansions,
};

/// Encapsulates the work done for a recursive qualified lookup into a module
/// by full name.
class LookupByName : public ModuleNameLookup<LookupByName> {
  using Super = ModuleNameLookup<LookupByName>;
  friend Super;

  const DeclName name;
  const NLKind lookupKind;

public:
  LookupByName(ASTContext &ctx, ResolutionKind resolutionKind,
               DeclName name, NLKind lookupKind)
    : Super(ctx, resolutionKind), name(name),
      lookupKind(lookupKind) {}

private:
  /// Returns whether it's okay to stop recursively searching imports, given
  /// that we found something non-overloadable.
  static bool canReturnEarly() {
    return true;
  }

  void doLocalLookup(ModuleDecl *module, ImportPath::Access path,
                     OptionSet<ModuleLookupFlags> flags,
                     SmallVectorImpl<ValueDecl *> &localDecls) {
    // If this import is specific to some named decl ("import NeXTCode.Int")
    // then filter out any lookups that don't match.
    if (!path.matches(name))
      return;
    module->lookupValue(name, lookupKind, flags, localDecls);
  }
};

/// Encapsulates the work done for a recursive qualified lookup into a module
/// to find all visible decls.
class LookupVisibleDecls : public ModuleNameLookup<LookupVisibleDecls> {
  using Super = ModuleNameLookup<LookupVisibleDecls>;
  friend Super;

  const NLKind lookupKind;

public:
  LookupVisibleDecls(ASTContext &ctx, ResolutionKind resolutionKind,
                     NLKind lookupKind)
    : ModuleNameLookup(ctx, resolutionKind),
      lookupKind(lookupKind) {}

private:
  /// Returns whether it's okay to stop recursively searching imports, given
  /// that we found something non-overloadable.
  static bool canReturnEarly() {
    return false;
  }

  void doLocalLookup(ModuleDecl *module, ImportPath::Access path,
                     OptionSet<ModuleLookupFlags> flags,
                     SmallVectorImpl<ValueDecl *> &localDecls) {
    VectorDeclConsumer consumer(localDecls);
    module->lookupVisibleDecls(path, consumer, lookupKind);
  }
};

} // end anonymous namespace

bool nextcode::declIsVisibleToNameLookup(
    const ValueDecl *decl, const DeclContext *moduleScopeContext,
    NLOptions options) {
  // NL_IgnoreAccessControl only applies to the current module. If
  // it applies here, the declaration is visible.
  if ((options & NL_IgnoreAccessControl) &&
      moduleScopeContext &&
      moduleScopeContext->getParentModule() ==
          decl->getDeclContext()->getParentModule())
    return true;

  bool includeUsableFromInline = options & NL_IncludeUsableFromInline;
  return decl->isAccessibleFrom(moduleScopeContext, false,
                                includeUsableFromInline);
}

template <typename LookupStrategy>
void ModuleNameLookup<LookupStrategy>::lookupInModule(
    SmallVectorImpl<ValueDecl *> &decls,
    const DeclContext *moduleOrFile,
    ImportPath::Access accessPath,
    const DeclContext *moduleScopeContext,
    NLOptions options) {
  assert(moduleOrFile->isModuleScopeContext());

  // Does the module scope have any separately-imported overlays shadowing
  // the module we're looking into?
  SmallVector<ModuleDecl *, 4> overlays;
  moduleScopeContext->getSeparatelyImportedOverlays(
      moduleOrFile->getParentModule(), overlays);
  if (!overlays.empty()) {
    // If so, look in each of those overlays.
    for (auto overlay : overlays)
      lookupInModule(decls, overlay, accessPath, moduleScopeContext, options);
    // FIXME: This may not work gracefully if more than one of these lookups
    // finds something.
    return;
  }

  const size_t initialCount = decls.size();
  size_t currentCount = decls.size();

  auto updateNewDecls = [&](const DeclContext *moduleScopeContext) {
    if (decls.size() == currentCount)
      return;

    auto newEnd = std::remove_if(
      decls.begin() + currentCount, decls.end(),
      [&](ValueDecl *VD) {
        if (resolutionKind == ResolutionKind::TypesOnly && !isa<TypeDecl>(VD))
          return true;
        if (resolutionKind == ResolutionKind::MacrosOnly && !isa<MacroDecl>(VD))
          return true;
        if (respectAccessControl &&
            !declIsVisibleToNameLookup(VD, moduleScopeContext, options))
          return true;
        return false;
      });
    decls.erase(newEnd, decls.end());

    currentCount = decls.size();
  };

  OptionSet<ModuleLookupFlags> currentModuleLookupFlags = {};
  if (options & NL_ExcludeMacroExpansions)
    currentModuleLookupFlags |= ModuleLookupFlags::ExcludeMacroExpansions;

  // Do the lookup into the current module.
  auto *module = moduleOrFile->getParentModule();
  getDerived()->doLocalLookup(
      module, accessPath, currentModuleLookupFlags, decls);
  updateNewDecls(moduleScopeContext);

  bool canReturnEarly = (initialCount != decls.size() &&
                         getDerived()->canReturnEarly());
  if (canReturnEarly &&
      resolutionKind == ResolutionKind::Overloadable) {
    // If we only found top-level functions or macros, keep looking, since
    // we may find additional overloads.
    if (std::all_of(decls.begin() + initialCount, decls.end(),
                    [](ValueDecl *VD) {
      return isa<FuncDecl>(VD) || isa<MacroDecl>(VD);
    }))
      canReturnEarly = false;
  }

  // If needed, search for decls in re-exported modules as well.
  if (!canReturnEarly) {
    auto &imports = ctx.getImportCache().getImportSet(moduleOrFile);

    auto visitImport = [&](ImportedModule import,
                           const DeclContext *moduleScopeContext) {
      if (import.accessPath.empty())
        import.accessPath = accessPath;
      else if (!accessPath.empty() &&
               !import.accessPath.isSameAs(accessPath))
        return;

      getDerived()->doLocalLookup(import.importedModule, import.accessPath,
                                  { }, decls);
      updateNewDecls(moduleScopeContext);
    };

    // If the ClangImporter's special header import module appears in the
    // import set, we must visit it first.
    ModuleDecl *headerImportModule = nullptr;
    if (imports.hasHeaderImportModule()) {
      if (auto *loader = ctx.getClangModuleLoader()) {
        headerImportModule = loader->getImportedHeaderModule();
        if (headerImportModule) {
          visitImport(ImportedModule(headerImportModule), nullptr);
        }
      }
    }

    for (auto import : imports.getTopLevelImports()) {
      // A module appears in its own top-level import list; since we checked
      // it already, skip it.
      if (import.importedModule == module)
        continue;

      // Skip the special import set module; we've already visited it.
      if (import.importedModule == headerImportModule)
        continue;

      visitImport(import, moduleScopeContext);
    }

    for (auto import : imports.getTransitiveImports()) {
      // Skip the special import set module; we've already visited it.
      if (import.importedModule == headerImportModule)
        continue;

      visitImport(import, moduleScopeContext);
    }
  }

  // Nothing more to do if we don't have ambiguous results.
  if (decls.size() - initialCount <= 1)
    return;

  // Remove duplicated declarations, which can happen when the same module is
  // imported with multiple access paths.
  llvm::SmallPtrSet<ValueDecl *, 4> knownDecls;
  decls.erase(std::remove_if(decls.begin() + initialCount, decls.end(),
                             [&](ValueDecl *d) -> bool {
                               return !knownDecls.insert(d).second;
                             }),
              decls.end());
}

QualifiedLookupResult
LookupInModuleRequest::evaluate(
    Evaluator &evaluator, const DeclContext *moduleOrFile, DeclName name,
    NLKind lookupKind, ResolutionKind resolutionKind,
    const DeclContext *moduleScopeContext, NLOptions options) const {
  assert(moduleScopeContext->isModuleScopeContext());

  QualifiedLookupResult decls;
  LookupByName lookup(moduleOrFile->getASTContext(), resolutionKind,
                      name, lookupKind);
  lookup.lookupInModule(decls, moduleOrFile, {}, moduleScopeContext, options);
  return decls;
}

void namelookup::lookupInModule(const DeclContext *moduleOrFile,
                                DeclName name,
                                SmallVectorImpl<ValueDecl *> &decls,
                                NLKind lookupKind,
                                ResolutionKind resolutionKind,
                                const DeclContext *moduleScopeContext,
                                SourceLoc loc, NLOptions options) {
  auto &ctx = moduleOrFile->getASTContext();
  LookupInModuleRequest req(moduleOrFile, name, lookupKind, resolutionKind,
                            moduleScopeContext, loc, options);
  auto results = evaluateOrDefault(ctx.evaluator, req, {});
  decls.append(results.begin(), results.end());
}

void namelookup::lookupVisibleDeclsInModule(
    const DeclContext *moduleOrFile,
    ImportPath::Access accessPath,
    SmallVectorImpl<ValueDecl *> &decls,
    NLKind lookupKind,
    ResolutionKind resolutionKind,
    const DeclContext *moduleScopeContext) {
  assert(moduleScopeContext->isModuleScopeContext());
  auto &ctx = moduleOrFile->getASTContext();
  LookupVisibleDecls lookup(ctx, resolutionKind, lookupKind);
  lookup.lookupInModule(decls, moduleOrFile, accessPath, moduleScopeContext,
                        NL_QualifiedDefault);
}

void namelookup::simple_display(llvm::raw_ostream &out, ResolutionKind kind) {
  switch (kind) {
  case ResolutionKind::Overloadable:
    out << "Overloadable";
    return;
  case ResolutionKind::TypesOnly:
    out << "TypesOnly";
    return;
  case ResolutionKind::MacrosOnly:
    out << "MacrosOnly";
    return;
  }
  llvm_unreachable("Unhandled case in switch");
}
