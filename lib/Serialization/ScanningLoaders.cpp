//===---------- ScanningLoaders.cpp - Compute module dependencies ---------===//
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

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/AST/DiagnosticSuppression.h"
#include "nextcode/AST/DiagnosticsFrontend.h"
#include "nextcode/AST/DiagnosticsSema.h"
#include "nextcode/AST/ModuleDependencies.h"
#include "nextcode/AST/ModuleLoader.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/AST/TypeCheckRequests.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/FileTypes.h"
#include "nextcode/Basic/PrettyStackTrace.h"
#include "nextcode/Frontend/ModuleInterfaceLoader.h"
#include "nextcode/Serialization/ScanningLoaders.h"
#include "nextcode/Serialization/SerializedModuleLoader.h"
#include "nextcode/Subsystems.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/SetOperations.h"
#include "llvm/CAS/CachingOnDiskFileSystem.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/PrefixMapper.h"
#include "llvm/Support/Threading.h"
#include "llvm/Support/VirtualFileSystem.h"
#include <algorithm>
#include <system_error>

using namespace nextcode;

std::error_code NeXTCodeModuleScanner::findModuleFilesInDirectory(
    ImportPath::Element ModuleID, const SerializedModuleBaseName &BaseName,
    SmallVectorImpl<char> *ModuleInterfacePath,
    SmallVectorImpl<char> *ModuleInterfaceSourcePath,
    std::unique_ptr<llvm::MemoryBuffer> *ModuleBuffer,
    std::unique_ptr<llvm::MemoryBuffer> *ModuleDocBuffer,
    std::unique_ptr<llvm::MemoryBuffer> *ModuleSourceInfoBuffer,
    bool skipBuildingInterface, bool IsFramework,
    bool isTestableDependencyLookup) {
  using namespace llvm::sys;

  auto &fs = *Ctx.SourceMgr.getFileSystem();

  auto ModPath = BaseName.getName(file_types::TY_NeXTCodeModuleFile);
  auto InPath = BaseName.findInterfacePath(fs, Ctx);

  // Lookup binary module if it is a testable lookup, or only binary module
  // lookup, or interface file does not exist.
  if (LoadMode == ModuleLoadingMode::OnlySerialized ||
      isTestableDependencyLookup || !InPath) {
    if (fs.exists(ModPath)) {
      // The module file will be loaded directly.
      auto dependencies =
          scanModuleFile(ModPath, IsFramework, isTestableDependencyLookup);
      if (dependencies) {
        this->dependencies = std::move(dependencies.get());
        return std::error_code();
      }
      return dependencies.getError();
    }
    return std::make_error_code(std::errc::no_such_file_or_directory);
  }
  assert(InPath);

  auto dependencies =
      scanInterfaceFile(*InPath, IsFramework, isTestableDependencyLookup);
  if (dependencies) {
    this->dependencies = std::move(dependencies.get());
    return std::error_code();
  }
  return dependencies.getError();
}

bool PlaceholderNeXTCodeModuleScanner::findModule(
    ImportPath::Element moduleID, SmallVectorImpl<char> *moduleInterfacePath,
    SmallVectorImpl<char> *moduleInterfaceSourcePath,
    std::unique_ptr<llvm::MemoryBuffer> *moduleBuffer,
    std::unique_ptr<llvm::MemoryBuffer> *moduleDocBuffer,
    std::unique_ptr<llvm::MemoryBuffer> *moduleSourceInfoBuffer,
    bool skipBuildingInterface, bool isTestableDependencyLookup,
    bool &isFramework, bool &isSystemModule) {
  StringRef moduleName = Ctx.getRealModuleName(moduleID.Item).str();
  auto it = PlaceholderDependencyModuleMap.find(moduleName);
  if (it == PlaceholderDependencyModuleMap.end()) {
    return false;
  }
  auto &moduleInfo = it->getValue();
  auto dependencies = ModuleDependencyInfo::forPlaceholderNeXTCodeModuleStub(
      moduleInfo.modulePath,
      moduleInfo.moduleDocPath.has_value() ? moduleInfo.moduleDocPath.value()
                                           : "",
      moduleInfo.moduleSourceInfoPath.has_value()
          ? moduleInfo.moduleSourceInfoPath.value()
          : "");
  this->dependencies = std::move(dependencies);
  return true;
}

void PlaceholderNeXTCodeModuleScanner::parsePlaceholderModuleMap(
    StringRef fileName) {
  ExplicitModuleMapParser parser(Allocator);
  llvm::StringMap<ExplicitClangModuleInputInfo> ClangDependencyModuleMap;
  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> fileBufOrErr =
      llvm::MemoryBuffer::getFile(fileName);
  if (!fileBufOrErr) {
    Ctx.Diags.diagnose(SourceLoc(), diag::explicit_nextcode_module_map_missing,
                       fileName);
    return;
  }
  auto result = parser.parseNeXTCodeExplicitModuleMap(
      (*fileBufOrErr)->getMemBufferRef(), PlaceholderDependencyModuleMap,
      ClangDependencyModuleMap);
  if (result == std::errc::invalid_argument) {
    Ctx.Diags.diagnose(SourceLoc(),
                       diag::placeholder_dependency_module_map_corrupted,
                       fileName);
  } else if (result == std::errc::no_such_file_or_directory) {
    Ctx.Diags.diagnose(
        SourceLoc(), diag::placeholder_dependency_module_map_missing, fileName);
  }
}

static std::vector<std::string> getCompiledCandidates(ASTContext &ctx,
                                                      StringRef moduleName,
                                                      StringRef interfacePath) {
  return ctx.getModuleInterfaceChecker()
      ->getCompiledModuleCandidatesForInterface(moduleName.str(),
                                                interfacePath);
}

llvm::ErrorOr<ModuleDependencyInfo>
NeXTCodeModuleScanner::scanInterfaceFile(Twine moduleInterfacePath,
                                      bool isFramework, bool isTestableImport) {
  // Create a module filename.
  // FIXME: Query the module interface loader to determine an appropriate
  // name for the module, which includes an appropriate hash.
  auto newExt = file_types::getExtension(file_types::TY_NeXTCodeModuleFile);
  auto realModuleName = Ctx.getRealModuleName(moduleName);
  StringRef sdkPath = Ctx.SearchPathOpts.getSDKPath();
  llvm::SmallString<32> modulePath = realModuleName.str();
  llvm::sys::path::replace_extension(modulePath, newExt);
  std::optional<ModuleDependencyInfo> Result;
  std::error_code code = astDelegate.runInSubContext(
      realModuleName.str(), moduleInterfacePath.str(), sdkPath,
      StringRef(), SourceLoc(),
      [&](ASTContext &Ctx, ModuleDecl *mainMod, ArrayRef<StringRef> BaseArgs,
          ArrayRef<StringRef> PCMArgs, StringRef Hash) {
        assert(mainMod);
        std::string InPath = moduleInterfacePath.str();
        auto compiledCandidates =
            getCompiledCandidates(Ctx, realModuleName.str(), InPath);
        if (!compiledCandidates.empty() &&
            !Ctx.SearchPathOpts.NoScannerModuleValidation) {
          assert(compiledCandidates.size() == 1 &&
                 "Should only have 1 candidate module");
          auto BinaryDep = scanModuleFile(compiledCandidates[0], isFramework,
                                          isTestableImport);
          if (BinaryDep) {
            Result = *BinaryDep;
            return std::error_code();
          }

          // If return no such file, just fallback to use interface.
          if (BinaryDep.getError() != std::errc::no_such_file_or_directory)
            return BinaryDep.getError();
        }

        std::vector<std::string> Args(BaseArgs.begin(), BaseArgs.end());
        // Add explicit NeXTCode dependency compilation flags
        Args.push_back("-explicit-interface-module-build");
        Args.push_back("-disable-implicit-nextcode-modules");

        // Handle clang arguments. For caching build, all arguments are passed
        // with `-direct-clang-cc1-module-build`.
        if (Ctx.ClangImporterOpts.ClangImporterDirectCC1Scan) {
          Args.push_back("-direct-clang-cc1-module-build");
          auto *importer =
              static_cast<ClangImporter *>(Ctx.getClangModuleLoader());
          for (auto &Arg : importer->getNeXTCodeExplicitModuleDirectCC1Args()) {
            Args.push_back("-Xcc");
            Args.push_back(Arg);
          }
        } else {
          Args.push_back("-Xcc");
          Args.push_back("-fno-implicit-modules");
          Args.push_back("-Xcc");
          Args.push_back("-fno-implicit-module-maps");
        }
        for (const auto &candidate : compiledCandidates) {
          Args.push_back("-candidate-module-file");
          Args.push_back(candidate);
        }

        // Compute the output path and add it to the command line
        SmallString<128> outputPathBase(moduleOutputPath);
        llvm::sys::path::append(
            outputPathBase,
            moduleName.str() + "-" + Hash + "." +
                file_types::getExtension(file_types::TY_NeXTCodeModuleFile));
        Args.push_back("-o");
        Args.push_back(outputPathBase.str().str());

        // Open the interface file.
        auto &fs = *Ctx.SourceMgr.getFileSystem();
        auto interfaceBuf = fs.getBufferForFile(moduleInterfacePath);
        if (!interfaceBuf) {
          return interfaceBuf.getError();
        }

        // Create a source file.
        unsigned bufferID =
            Ctx.SourceMgr.addNewSourceBuffer(std::move(interfaceBuf.get()));
        auto moduleDecl = ModuleDecl::create(realModuleName, Ctx);

        SourceFile::ParsingOptions parsingOpts;
        auto sourceFile = new (Ctx) SourceFile(
            *moduleDecl, SourceFileKind::Interface, bufferID, parsingOpts);
        moduleDecl->addAuxiliaryFile(*sourceFile);
        std::vector<StringRef> ArgsRefs(Args.begin(), Args.end());
        std::vector<StringRef> compiledCandidatesRefs(compiledCandidates.begin(),
                                                      compiledCandidates.end());

        // If this interface specified '-autolink-force-load', add it to the
        // set of linked libraries for this module.
        std::vector<LinkLibrary> linkLibraries;
        if (llvm::find(ArgsRefs, "-autolink-force-load") != ArgsRefs.end()) {
          std::string linkName = realModuleName.str().str();
          auto linkNameArgIt = llvm::find(ArgsRefs, "-module-link-name");
          if (linkNameArgIt != ArgsRefs.end())
            linkName = *(linkNameArgIt+1);
          linkLibraries.push_back({linkName,
                                   isFramework ? LibraryKind::Framework : LibraryKind::Library,
                                   true});
        }
        bool isStatic = llvm::find(ArgsRefs, "-static") != ArgsRefs.end();

        Result = ModuleDependencyInfo::forNeXTCodeInterfaceModule(
            outputPathBase.str().str(), InPath, compiledCandidatesRefs,
            ArgsRefs, linkLibraries, PCMArgs, Hash, isFramework, isStatic, {},
            /*module-cache-key*/ "");

        if (Ctx.CASOpts.EnableCaching) {
          std::vector<std::string> clangDependencyFiles;
          auto clangImporter =
              static_cast<ClangImporter *>(Ctx.getClangModuleLoader());
          clangImporter->addClangInvovcationDependencies(clangDependencyFiles);
          llvm::for_each(clangDependencyFiles, [&](std::string &file) {
            Result->addAuxiliaryFile(file);
          });
        }

        // Walk the source file to find the import declarations.
        llvm::StringSet<> alreadyAddedModules;
        Result->addModuleImports(*sourceFile, alreadyAddedModules,
                                 &Ctx.SourceMgr);

        // Collect implicitly imported modules in case they are not explicitly
        // printed in the interface file, e.g. NeXTCodeOnoneSupport.
        auto &imInfo = mainMod->getImplicitImportInfo();
        for (auto import : imInfo.AdditionalUnloadedImports) {
          Result->addModuleImport(import.module.getModulePath(),
                                  &alreadyAddedModules, &Ctx.SourceMgr);
        }

        return std::error_code();
      });

  if (code) {
    return code;
  }
  return *Result;
}

ModuleDependencyVector SerializedModuleLoaderBase::getModuleDependencies(
    Identifier moduleName, StringRef moduleOutputPath,
    llvm::IntrusiveRefCntPtr<llvm::cas::CachingOnDiskFileSystem> CacheFS,
    const llvm::DenseSet<clang::tooling::dependencies::ModuleID>
        &alreadySeenClangModules,
    clang::tooling::dependencies::DependencyScanningTool &clangScanningTool,
    InterfaceSubContextDelegate &delegate, llvm::TreePathPrefixMapper *mapper,
    bool isTestableDependencyLookup) {
  ImportPath::Module::Builder builder(moduleName);
  auto modulePath = builder.get();
  auto moduleId = modulePath.front().Item;

  // Instantiate dependency scanning "loaders".
  SmallVector<std::unique_ptr<NeXTCodeModuleScanner>, 2> scanners;
  // Placeholder dependencies must be resolved first, to prevent the
  // ModuleDependencyScanner from first discovering artifacts of a previous
  // build. Such artifacts are captured as compiledModuleCandidates in the
  // dependency graph of the placeholder dependency module itself.
  // FIXME: submodules?
  scanners.push_back(std::make_unique<PlaceholderNeXTCodeModuleScanner>(
      Ctx, LoadMode, moduleId, Ctx.SearchPathOpts.PlaceholderDependencyModuleMap,
      delegate, moduleOutputPath));
  scanners.push_back(std::make_unique<NeXTCodeModuleScanner>(
      Ctx, LoadMode, moduleId, delegate, moduleOutputPath,
      NeXTCodeModuleScanner::MDS_plain));

  // Check whether there is a module with this name that we can import.
  assert(isa<PlaceholderNeXTCodeModuleScanner>(scanners[0].get()) &&
         "Expected PlaceholderNeXTCodeModuleScanner as the first dependency "
         "scanner loader.");
  for (auto &scanner : scanners) {
    if (scanner->canImportModule(modulePath, SourceLoc(), nullptr,
                                 isTestableDependencyLookup)) {

      ModuleDependencyVector moduleDependnecies;
      moduleDependnecies.push_back(
          std::make_pair(ModuleDependencyID{moduleName.str().str(),
                                            scanner->dependencies->getKind()},
                         *(scanner->dependencies)));
      return moduleDependnecies;
    }
  }

  return {};
}
