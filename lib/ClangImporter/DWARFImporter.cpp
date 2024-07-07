//===--- DWARFImporter.cpp - Import Clang modules from DWARF --------------===//
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

#include "ImporterImpl.h"
#include "nextcode/AST/ImportCache.h"
#include "nextcode/AST/Module.h"
#include "nextcode/Basic/Assertions.h"

using namespace nextcode;

void DWARFImporterDelegate::anchor() {}

/// Represents a Clang module that was "imported" from debug info. Since all the
/// loading of types is done on demand, this class is effectively empty.
class DWARFModuleUnit final : public LoadedFile {
  ClangImporter::Implementation &Owner;

public:
  DWARFModuleUnit(ModuleDecl &M, ClangImporter::Implementation &owner)
      : LoadedFile(FileUnitKind::DWARFModule, M), Owner(owner) {}

  virtual bool isSystemModule() const override { return false; }

  /// Forwards the request to the ClangImporter, which forwards it to the
  /// DWARFimporterDelegate.
  virtual void
  lookupValue(DeclName name, NLKind lookupKind,
              OptionSet<ModuleLookupFlags> Flags,
              SmallVectorImpl<ValueDecl *> &results) const override {
    Owner.lookupValueDWARF(name, lookupKind,
                           getParentModule()->getName(), results);
  }

  virtual TypeDecl *
  lookupNestedType(Identifier name,
                   const NominalTypeDecl *baseType) const override {
    return nullptr;
  }

  virtual void lookupVisibleDecls(ImportPath::Access accessPath,
                                  VisibleDeclConsumer &consumer,
                                  NLKind lookupKind) const override {}

  virtual void
  lookupClassMembers(ImportPath::Access accessPath,
                     VisibleDeclConsumer &consumer) const override {}

  virtual void
  lookupClassMember(ImportPath::Access accessPath, DeclName name,
                    SmallVectorImpl<ValueDecl *> &decls) const override {}

  void lookupObjCMethods(
      ObjCSelector selector,
      SmallVectorImpl<AbstractFunctionDecl *> &results) const override {}

  virtual void
  getTopLevelDecls(SmallVectorImpl<Decl *> &results) const override {}

  virtual void
  getDisplayDecls(SmallVectorImpl<Decl *> &results, bool recursive = false) const override {}

  virtual void
  getImportedModules(SmallVectorImpl<ImportedModule> &imports,
                     ModuleDecl::ImportFilter filter) const override {}

  virtual void getImportedModulesForLookup(
      SmallVectorImpl<ImportedModule> &imports) const override {};

  virtual void collectLinkLibraries(
      ModuleDecl::LinkLibraryCallback callback) const override {};

  Identifier
  getDiscriminatorForPrivateDecl(const Decl *D) const override {
    llvm_unreachable("no private decls in Clang modules");
  }

  virtual version::Version getLanguageVersionBuiltWith() const override {
    return version::Version();
  }

  virtual StringRef getFilename() const override { return ""; }

  virtual const clang::Module *getUnderlyingClangModule() const override {
    return nullptr;
  }

  static bool classof(const FileUnit *file) {
    return file->getKind() == FileUnitKind::DWARFModule;
  }
  static bool classof(const DeclContext *DC) {
    return isa<FileUnit>(DC) && classof(cast<FileUnit>(DC));
  }
};

static_assert(IsTriviallyDestructible<DWARFModuleUnit>::value,
              "DWARFModuleUnits are BumpPtrAllocated; the d'tor is not called");

ModuleDecl *ClangImporter::Implementation::loadModuleDWARF(
    SourceLoc importLoc, ImportPath::Module path) {
  // There's no importing from debug info if no importer is installed.
  if (!DWARFImporter)
    return nullptr;

  // FIXME: Implement submodule support!
  Identifier name = path[0].Item;
  auto it = DWARFModuleUnits.find(name);
  if (it != DWARFModuleUnits.end())
    return it->second->getParentModule();

  auto *decl = ModuleDecl::create(name, NeXTCodeContext);
  decl->setIsNonNeXTCodeModule();
  decl->setHasResolvedImports();
  auto *wrapperUnit = new (NeXTCodeContext) DWARFModuleUnit(*decl, *this);
  DWARFModuleUnits.insert({name, wrapperUnit});
  decl->addFile(*wrapperUnit);

  // Force load overlay modules for all imported modules.
  assert(namelookup::getAllImports(decl).size() == 1 &&
         namelookup::getAllImports(decl).front().importedModule == decl &&
         "DWARF module depends on additional modules?");

  // Register the module with the ASTContext so it is available for lookups.
  if (!NeXTCodeContext.getLoadedModule(name))
    NeXTCodeContext.addLoadedModule(decl);

  return decl;
}

// This function exists to defeat the lazy member importing mechanism. The
// DWARFImporter is not capable of loading individual members, so it cannot
// benefit from this optimization yet anyhow. Besides, if you're importing a
// type here, you more than likely want to dump it and its fields. Loading all
// members populates lookup tables in the Clang Importer and ensures the
// absence of cache-fill-related side effects.
static void forceLoadAllMembers(IterableDeclContext *IDC) {
  if (!IDC) return;
  IDC->loadAllMembers();
}

void ClangImporter::Implementation::lookupValueDWARF(
    DeclName name, NLKind lookupKind, Identifier inModule,
    SmallVectorImpl<ValueDecl *> &results) {
  if (lookupKind != NLKind::QualifiedLookup)
    return;

  if (!DWARFImporter)
    return;

  SmallVector<clang::Decl *, 4> decls;
  DWARFImporter->lookupValue(name.getBaseName().userFacingName(), std::nullopt,
                             inModule.str(), decls);
  for (auto *clangDecl : decls) {
    auto *namedDecl = dyn_cast<clang::NamedDecl>(clangDecl);
    if (!namedDecl)
      continue;
    auto *nextcodeDecl = cast_or_null<ValueDecl>(
        importDeclReal(namedDecl->getMostRecentDecl(), CurrentVersion));
    if (!nextcodeDecl)
      continue;

    if (nextcodeDecl->getName().matchesRef(name) &&
        nextcodeDecl->getDeclContext()->isModuleScopeContext()) {
      forceLoadAllMembers(dyn_cast<IterableDeclContext>(nextcodeDecl));
      results.push_back(nextcodeDecl);
    }
  }
}

void ClangImporter::Implementation::lookupTypeDeclDWARF(
    StringRef rawName, ClangTypeKind kind,
    llvm::function_ref<void(TypeDecl *)> receiver) {
  if (!DWARFImporter)
    return;

  /// This function is invoked by ASTDemangler, which doesn't filter by module.
  SmallVector<clang::Decl *, 1> decls;
  DWARFImporter->lookupValue(rawName, kind, {}, decls);
  for (auto *clangDecl : decls) {
    if (!isa<clang::TypeDecl>(clangDecl) &&
        !isa<clang::ObjCContainerDecl>(clangDecl) &&
        !isa<clang::ObjCCompatibleAliasDecl>(clangDecl)) {
      continue;
    }
    auto *namedDecl = cast<clang::NamedDecl>(clangDecl);
    Decl *importedDecl = cast_or_null<ValueDecl>(
        importDeclReal(namedDecl->getMostRecentDecl(), CurrentVersion));

    if (auto *importedType = dyn_cast_or_null<TypeDecl>(importedDecl)) {
      forceLoadAllMembers(dyn_cast<IterableDeclContext>(importedType));
      receiver(importedType);
    }
  }
}

void ClangImporter::setDWARFImporterDelegate(DWARFImporterDelegate &delegate) {
  Impl.setDWARFImporterDelegate(delegate);
}

void ClangImporter::Implementation::setDWARFImporterDelegate(
    DWARFImporterDelegate &delegate) {
  DWARFImporter = &delegate;
}
