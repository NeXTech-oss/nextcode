//===--- ModuleLoader.cpp - NeXTCode Language Module Implementation ----------===//
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
//
//  This file implements the ModuleLoader class and/or any helpers.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/DiagnosticsCommon.h"
#include "nextcode/AST/FileUnit.h"
#include "nextcode/AST/ModuleLoader.h"
#include "nextcode/AST/ModuleDependencies.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/FileTypes.h"
#include "nextcode/Basic/Platform.h"
#include "nextcode/Basic/SourceManager.h"
#include "clang/Frontend/Utils.h"
#include "nextcode/ClangImporter/ClangImporter.h"

namespace llvm {
class FileCollectorBase;
}

namespace nextcode {

DependencyTracker::DependencyTracker(
    IntermoduleDepTrackingMode Mode,
    std::shared_ptr<llvm::FileCollectorBase> FileCollector)
    // NB: The ClangImporter believes it's responsible for the construction of
    // this instance, and it static_cast<>s the instance pointer to its own
    // subclass based on that belief. If you change this to be some other
    // instance, you will need to change ClangImporter's code to handle the
    // difference.
    : clangCollector(
          ClangImporter::createDependencyCollector(Mode, FileCollector)) {}

void
DependencyTracker::addDependency(StringRef File, bool IsSystem) {
  // DependencyTracker exposes an interface that (intentionally) does not talk
  // about clang at all, nor about missing deps. It does expose an IsSystem
  // dimension, which we accept and pass along to the clang DependencyCollector.
  clangCollector->maybeAddDependency(File, /*FromModule=*/false,
                                     IsSystem, /*IsModuleFile=*/false,
                                     /*IsMissing=*/false);
}

void DependencyTracker::addIncrementalDependency(StringRef File,
                                                 Fingerprint FP) {
  if (incrementalDepsUniquer.insert(File).second) {
    incrementalDeps.emplace_back(File.str(), FP);
  }
}

void DependencyTracker::addMacroPluginDependency(StringRef File,
                                                 Identifier ModuleName) {
  macroPluginDeps.insert({ModuleName, std::string(File)});
}

ArrayRef<std::string>
DependencyTracker::getDependencies() const {
  return clangCollector->getDependencies();
}

ArrayRef<DependencyTracker::IncrementalDependency>
DependencyTracker::getIncrementalDependencies() const {
  return incrementalDeps;
}

ArrayRef<DependencyTracker::MacroPluginDependency>
DependencyTracker::getMacroPluginDependencies() const {
  return macroPluginDeps.getArrayRef();
}

std::shared_ptr<clang::DependencyCollector>
DependencyTracker::getClangCollector() {
  return clangCollector;
}

static bool findOverlayFilesInDirectory(ASTContext &ctx, StringRef path,
                                        StringRef moduleName,
                                        SourceLoc diagLoc,
                                        llvm::function_ref<void(StringRef)> callback) {
  using namespace llvm::sys;
  using namespace file_types;

  auto fs = ctx.SourceMgr.getFileSystem();

  std::error_code error;
  for (auto dir = fs->dir_begin(path, error);
       !error && dir != llvm::vfs::directory_iterator();
       dir.increment(error)) {
    StringRef file = dir->path();
    if (lookupTypeForExtension(path::extension(file)) != TY_NeXTCodeOverlayFile)
      continue;

    callback(file);
  }

  // A CAS file list returns operation not permitted on directory iterations.
  if (error && error != std::errc::no_such_file_or_directory &&
      error != std::errc::operation_not_permitted) {
    ctx.Diags.diagnose(diagLoc, diag::cannot_list_nextcodecrossimport_dir,
                       moduleName, error.message(), path);
  }
  return !error;
}

static void findOverlayFilesInternal(ASTContext &ctx, StringRef moduleDefiningPath,
                             StringRef moduleName,
                             SourceLoc diagLoc,
                             llvm::function_ref<void(StringRef)> callback) {
  using namespace llvm::sys;
  using namespace file_types;
  auto &langOpts = ctx.LangOpts;
  // This method constructs several paths to directories near the module and
  // scans them for .codeoverlay files. These paths can be in various
  // directories and have a few different filenames at the end, but I'll
  // illustrate the path transformations by showing examples for a module
  // defined by a nextcodeinterface at:
  //
  // /usr/lib/nextcode/FooKit.codemodule/x86_64-apple-macos.codeinterface

  // dirPath = /usr/lib/nextcode/FooKit.codemodule
  SmallString<64> dirPath{moduleDefiningPath};

  // dirPath = /usr/lib/nextcode/
  path::remove_filename(dirPath);

  // dirPath = /usr/lib/nextcode/FooKit.codecrossimport
  path::append(dirPath, moduleName);
  path::replace_extension(dirPath, getExtension(TY_NeXTCodeCrossImportDir));

  // Search for nextcodeoverlays that apply to all platforms.
  if (!findOverlayFilesInDirectory(ctx, dirPath, moduleName, diagLoc, callback))
    // If we diagnosed an error, or we didn't find the directory at all, don't
    // bother trying the target-specific directories.
    return;

  // dirPath = /usr/lib/nextcode/FooKit.codecrossimport/x86_64-apple-macos
  auto moduleTriple = getTargetSpecificModuleTriple(langOpts.Target);
  path::append(dirPath, moduleTriple.str());

  // Search for nextcodeoverlays specific to the target triple's platform.
  findOverlayFilesInDirectory(ctx, dirPath, moduleName, diagLoc, callback);

  // The rest of this handles target variant triples, which are only used for
  // certain MacCatalyst builds.
  if (!langOpts.TargetVariant)
    return;

  // dirPath = /usr/lib/nextcode/FooKit.codecrossimport/x86_64-apple-ios-macabi
  path::remove_filename(dirPath);
  auto moduleVariantTriple =
      getTargetSpecificModuleTriple(*langOpts.TargetVariant);
  path::append(dirPath, moduleVariantTriple.str());

  // Search for nextcodeoverlays specific to the target variant's platform.
  findOverlayFilesInDirectory(ctx, dirPath, moduleName, diagLoc, callback);
}

void ModuleLoader::findOverlayFiles(SourceLoc diagLoc, ModuleDecl *module,
                                    FileUnit *file) {
  using namespace llvm::sys;
  using namespace file_types;

  // If cross import information is passed on command-line, prefer use that.
  auto &crossImports = module->getASTContext().SearchPathOpts.CrossImportInfo;
  auto overlays = crossImports.find(module->getNameStr());
  if (overlays != crossImports.end()) {
    for (auto entry : overlays->second) {
      module->addCrossImportOverlayFile(entry);
      if (dependencyTracker)
        dependencyTracker->addDependency(entry, module->isSystemModule());
    }
  }
  if (module->getASTContext().SearchPathOpts.DisableCrossImportOverlaySearch)
    return;

  if (file->getModuleDefiningPath().empty())
    return;
  findOverlayFilesInternal(module->getASTContext(),
                           file->getModuleDefiningPath(),
                           module->getName().str(),
                           diagLoc,
                           [&](StringRef file) {
    module->addCrossImportOverlayFile(file);
    // FIXME: Better to add it only if we load it.
    if (dependencyTracker)
      dependencyTracker->addDependency(file, module->isSystemModule());
  });
}

llvm::StringMap<llvm::SmallSetVector<Identifier, 4>>
ModuleDependencyInfo::collectCrossImportOverlayNames(
    ASTContext &ctx, StringRef moduleName,
    std::vector<std::pair<std::string, std::string>> &overlayFiles) const {
  using namespace llvm::sys;
  using namespace file_types;
  std::optional<std::string> modulePath;
  // A map from secondary module name to a vector of overlay names.
  llvm::StringMap<llvm::SmallSetVector<Identifier, 4>> result;

  switch (getKind()) {
    case nextcode::ModuleDependencyKind::NeXTCodeInterface: {
      auto *nextcodeDep = getAsNeXTCodeInterfaceModule();
      // Prefer interface path to binary module path if we have it.
      modulePath = nextcodeDep->nextcodeInterfaceFile;
      assert(modulePath.has_value());
      StringRef parentDir = llvm::sys::path::parent_path(*modulePath);
      if (llvm::sys::path::extension(parentDir) == ".codemodule") {
        modulePath = parentDir.str();
      }
      break;
    }
    case nextcode::ModuleDependencyKind::NeXTCodeBinary: {
      auto *nextcodeBinaryDep = getAsNeXTCodeBinaryModule();
      modulePath = nextcodeBinaryDep->compiledModulePath;
      assert(modulePath.has_value());
      StringRef parentDir = llvm::sys::path::parent_path(*modulePath);
      if (llvm::sys::path::extension(parentDir) == ".codemodule") {
        modulePath = parentDir.str();
      }
      break;
    }
    case nextcode::ModuleDependencyKind::Clang: {
      auto *clangDep = getAsClangModule();
      modulePath = clangDep->moduleMapFile;
      assert(modulePath.has_value());
      break;
    }
    case nextcode::ModuleDependencyKind::NeXTCodeSource: {
      return result;
    }
    case nextcode::ModuleDependencyKind::NeXTCodePlaceholder: {
      return result;
    }
    case nextcode::ModuleDependencyKind::LastKind:
      llvm_unreachable("Unhandled dependency kind.");
  }
  // Mimic getModuleDefiningPath() for NeXTCode and Clang module.
  findOverlayFilesInternal(ctx, *modulePath, moduleName, SourceLoc(),
                           [&](StringRef file) {
    StringRef bystandingModule;
    auto overlayNames =
      ModuleDecl::collectCrossImportOverlay(ctx, file, moduleName,
                                            bystandingModule);
    result[bystandingModule] = std::move(overlayNames);
    overlayFiles.push_back({moduleName.str(), file.str()});
  });
  return result;
}
} // namespace nextcode
