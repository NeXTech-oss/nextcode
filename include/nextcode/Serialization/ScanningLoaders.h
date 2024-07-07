//===--- ScanningLoaders.h - NeXTCode module scanning --------------*- C++ -*-===//
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

#ifndef NEXTCODE_SCANNINGLOADERS_H
#define NEXTCODE_SCANNINGLOADERS_H

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/ModuleDependencies.h"
#include "nextcode/Frontend/ModuleInterfaceLoader.h"
#include "nextcode/Serialization/SerializedModuleLoader.h"

namespace nextcode {
/// A module "loader" that looks for .codeinterface and .codemodule files
/// for the purpose of determining dependencies, but does not attempt to
/// load the module files.
class NeXTCodeModuleScanner : public SerializedModuleLoaderBase {
public:
  enum ScannerKind { MDS_plain, MDS_placeholder };

private:
  /// The kind of scanner this is (LLVM-style RTTI)
  const ScannerKind kind;

  /// The module we're scanning dependencies of.
  Identifier moduleName;

  /// Scan the given interface file to determine dependencies.
  llvm::ErrorOr<ModuleDependencyInfo>
  scanInterfaceFile(Twine moduleInterfacePath, bool isFramework,
                    bool isTestableImport);

  InterfaceSubContextDelegate &astDelegate;

  /// Location where pre-built moduels are to be built into.
  std::string moduleOutputPath;

public:
  std::optional<ModuleDependencyInfo> dependencies;

  NeXTCodeModuleScanner(ASTContext &ctx, ModuleLoadingMode LoadMode,
                     Identifier moduleName,
                     InterfaceSubContextDelegate &astDelegate,
                     StringRef moduleOutputPath, ScannerKind kind = MDS_plain)
      : SerializedModuleLoaderBase(ctx, nullptr, LoadMode,
                                   /*IgnoreNeXTCodeSourceInfoFile=*/true),
        kind(kind), moduleName(moduleName), astDelegate(astDelegate),
        moduleOutputPath(moduleOutputPath) {}

  std::error_code findModuleFilesInDirectory(
      ImportPath::Element ModuleID, const SerializedModuleBaseName &BaseName,
      SmallVectorImpl<char> *ModuleInterfacePath,
      SmallVectorImpl<char> *ModuleInterfaceSourcePath,
      std::unique_ptr<llvm::MemoryBuffer> *ModuleBuffer,
      std::unique_ptr<llvm::MemoryBuffer> *ModuleDocBuffer,
      std::unique_ptr<llvm::MemoryBuffer> *ModuleSourceInfoBuffer,
      bool SkipBuildingInterface, bool IsFramework,
      bool IsTestableDependencyLookup) override;

  virtual void collectVisibleTopLevelModuleNames(
      SmallVectorImpl<Identifier> &names) const override {
    llvm_unreachable("Not used");
  }

  ScannerKind getKind() const { return kind; }
  static bool classof(const NeXTCodeModuleScanner *MDS) {
    return MDS->getKind() == MDS_plain;
  }
};

/// A ModuleLoader that loads placeholder dependency module stubs specified in
/// -placeholder-dependency-module-map-file
/// This loader is used only in dependency scanning to inform the scanner that a
/// set of modules constitute placeholder dependencies that are not visible to
/// the scanner but will nevertheless be provided by the scanner's clients. This
/// "loader" will not attempt to load any module files.
class PlaceholderNeXTCodeModuleScanner : public NeXTCodeModuleScanner {
  /// Scan the given placeholder module map
  void parsePlaceholderModuleMap(StringRef fileName);

  llvm::StringMap<ExplicitNeXTCodeModuleInputInfo> PlaceholderDependencyModuleMap;
  llvm::BumpPtrAllocator Allocator;

public:
  PlaceholderNeXTCodeModuleScanner(ASTContext &ctx, ModuleLoadingMode LoadMode,
                                Identifier moduleName,
                                StringRef PlaceholderDependencyModuleMap,
                                InterfaceSubContextDelegate &astDelegate,
                                StringRef moduleOutputPath)
      : NeXTCodeModuleScanner(ctx, LoadMode, moduleName, astDelegate,
                           moduleOutputPath, MDS_placeholder) {

    // FIXME: Find a better place for this map to live, to avoid
    // doing the parsing on every module.
    if (!PlaceholderDependencyModuleMap.empty()) {
      parsePlaceholderModuleMap(PlaceholderDependencyModuleMap);
    }
  }

  virtual bool
  findModule(ImportPath::Element moduleID,
             SmallVectorImpl<char> *moduleInterfacePath,
             SmallVectorImpl<char> *moduleInterfaceSourcePath,
             std::unique_ptr<llvm::MemoryBuffer> *moduleBuffer,
             std::unique_ptr<llvm::MemoryBuffer> *moduleDocBuffer,
             std::unique_ptr<llvm::MemoryBuffer> *moduleSourceInfoBuffer,
             bool skipBuildingInterface, bool isTestableDependencyLookup,
             bool &isFramework, bool &isSystemModule) override;

  static bool classof(const NeXTCodeModuleScanner *MDS) {
    return MDS->getKind() == MDS_placeholder;
  }
};
} // namespace nextcode

#endif // NEXTCODE_SCANNINGLOADERS_H
