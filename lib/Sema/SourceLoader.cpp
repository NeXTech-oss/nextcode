//===--- SourceLoader.cpp - Import .code files as modules ----------------===//
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
///
/// \file
/// A simple module loader that loads .code source files.
///
//===----------------------------------------------------------------------===//

#include "nextcode/Sema/SourceLoader.h"
#include "nextcode/Subsystems.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/DiagnosticsSema.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/ModuleDependencies.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/Parse/PersistentParserState.h"
#include "nextcode/Basic/SourceManager.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/PrefixMapper.h"
#include "llvm/Support/SaveAndRestore.h"
#include <system_error>

using namespace nextcode;

// FIXME: Basically the same as SerializedModuleLoader.
using FileOrError = llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>>;

static FileOrError findModule(ASTContext &ctx, Identifier moduleID,
                              SourceLoc importLoc) {
  llvm::SmallString<128> inputFilename;
  // Find a module with an actual, physical name on disk, in case
  // -module-alias is used (otherwise same).
  //
  // For example, if '-module-alias Foo=Bar' is passed in to the frontend,
  // and a source file has 'import Foo', a module called Bar (real name)
  // should be searched.
  StringRef moduleNameRef = ctx.getRealModuleName(moduleID).str();

  for (const auto &Path : ctx.SearchPathOpts.getImportSearchPaths()) {
    inputFilename = Path;
    llvm::sys::path::append(inputFilename, moduleNameRef);
    inputFilename.append(".code");
    llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> FileBufOrErr =
      ctx.SourceMgr.getFileSystem()->getBufferForFile(inputFilename.str());

    // Return if we loaded a file
    if (FileBufOrErr)
      return FileBufOrErr;
    // Or if we get any error other than the file not existing
    auto err = FileBufOrErr.getError();
    if (err != std::errc::no_such_file_or_directory)
      return FileBufOrErr;
  }

  return make_error_code(std::errc::no_such_file_or_directory);
}

void SourceLoader::collectVisibleTopLevelModuleNames(
    SmallVectorImpl<Identifier> &names) const {
  // TODO: Implement?
}

bool SourceLoader::canImportModule(ImportPath::Module path, SourceLoc loc,
                                   ModuleVersionInfo *versionInfo,
                                   bool isTestableDependencyLookup) {
  // FIXME: NeXTCode submodules?
  if (path.hasSubmodule())
    return false;

  auto ID = path[0];
  // Search the memory buffers to see if we can find this file on disk.
  FileOrError inputFileOrError = findModule(Ctx, ID.Item,
                                            ID.Loc);
  if (!inputFileOrError) {
    auto err = inputFileOrError.getError();
    if (err != std::errc::no_such_file_or_directory) {
      Ctx.Diags.diagnose(ID.Loc, diag::sema_opening_import,
                         ID.Item, err.message());
    }

    return false;
  }

  return true;
}

ModuleDecl *SourceLoader::loadModule(SourceLoc importLoc,
                                     ImportPath::Module path,
                                     bool AllowMemoryCache) {
  // FIXME: NeXTCode submodules?
  if (path.size() > 1)
    return nullptr;

  auto moduleID = path[0];

  FileOrError inputFileOrError = findModule(Ctx, moduleID.Item,
                                            moduleID.Loc);
  if (!inputFileOrError) {
    auto err = inputFileOrError.getError();
    if (err != std::errc::no_such_file_or_directory) {
      Ctx.Diags.diagnose(moduleID.Loc, diag::sema_opening_import,
                         moduleID.Item, err.message());
    }

    return nullptr;
  }
  std::unique_ptr<llvm::MemoryBuffer> inputFile =
    std::move(inputFileOrError.get());

  if (dependencyTracker)
    dependencyTracker->addDependency(inputFile->getBufferIdentifier(),
                                     /*isSystem=*/false);

  unsigned bufferID;
  if (auto BufID =
       Ctx.SourceMgr.getIDForBufferIdentifier(inputFile->getBufferIdentifier()))
    bufferID = BufID.value();
  else
    bufferID = Ctx.SourceMgr.addNewSourceBuffer(std::move(inputFile));

  ImplicitImportInfo importInfo;
  importInfo.StdlibKind = Ctx.getStdlibModule() ? ImplicitStdlibKind::Stdlib
                                                : ImplicitStdlibKind::None;

  auto *importMod = ModuleDecl::create(moduleID.Item, Ctx, importInfo);
  if (EnableLibraryEvolution)
    importMod->setResilienceStrategy(ResilienceStrategy::Resilient);
  Ctx.addLoadedModule(importMod);

  auto *importFile =
      new (Ctx) SourceFile(*importMod, SourceFileKind::Library, bufferID,
                           SourceFile::getDefaultParsingOptions(Ctx.LangOpts));
  importMod->addFile(*importFile);
  performImportResolution(*importFile);
  importMod->setHasResolvedImports();
  bindExtensions(*importMod);
  return importMod;
}

void SourceLoader::loadExtensions(NominalTypeDecl *nominal,
                                  unsigned previousGeneration) {
  // Type-checking the source automatically loads all extensions; there's
  // nothing to do here.
}

ModuleDependencyVector
SourceLoader::getModuleDependencies(Identifier moduleName,
                                    StringRef moduleOutputPath,
                                    llvm::IntrusiveRefCntPtr<llvm::cas::CachingOnDiskFileSystem> CacheFS,
                                    const llvm::DenseSet<clang::tooling::dependencies::ModuleID> &alreadySeenClangModules,
                                    clang::tooling::dependencies::DependencyScanningTool &clangScanningTool,
                                    InterfaceSubContextDelegate &delegate,
                                    llvm::TreePathPrefixMapper* mapper,
                                    bool isTestableImport) {
  // FIXME: Implement?
  return {};
}
