//===--- ModuleDependencies.h - Module Dependencies -------------*- C++ -*-===//
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
// This file defines data structures for capturing all of the source files
// and modules on which a given module depends, forming a graph of all of the
// modules that need to be present for a given module to be built.
//
//===----------------------------------------------------------------------===//
#ifndef NEXTCODE_AST_MODULE_DEPENDENCIES_H
#define NEXTCODE_AST_MODULE_DEPENDENCIES_H

#include "nextcode/AST/Import.h"
#include "nextcode/AST/LinkLibrary.h"
#include "nextcode/AST/SearchPathOptions.h"
#include "nextcode/Basic/LLVM.h"
#include "clang/CAS/CASOptions.h"
#include "clang/Tooling/DependencyScanning/DependencyScanningService.h"
#include "clang/Tooling/DependencyScanning/DependencyScanningTool.h"
#include "clang/Tooling/DependencyScanning/ModuleDepCollector.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/CAS/CASProvidingFileSystem.h"
#include "llvm/CAS/CASReference.h"
#include "llvm/CAS/CachingOnDiskFileSystem.h"
#include "llvm/CAS/ObjectStore.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/Mutex.h"
#include "llvm/Support/PrefixMapper.h"
#include "llvm/Support/VirtualFileSystem.h"
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace nextcode {

class ClangModuleDependenciesCacheImpl;
class SourceFile;
class ASTContext;
class Identifier;
class CompilerInstance;
class IRGenOptions;

/// Which kind of module dependencies we are looking for.
enum class ModuleDependencyKind : int8_t {
  FirstKind,
  // Textual NeXTCode dependency
  NeXTCodeInterface = FirstKind,
  // Binary module NeXTCode dependency
  NeXTCodeBinary,
  // Clang module dependency
  Clang,
  // Used to model the translation unit's source module
  NeXTCodeSource,
  // Placeholder dependencies are a kind of dependencies used only by the
  // dependency scanner. They are nextcode modules that the scanner will not be
  // able to locate in its search paths and which are the responsibility of the
  // scanner's client to ensure are provided.
  //
  // Placeholder dependencies will be specified in the scanner's output
  // dependency graph where it is the responsibility of the scanner's client to
  // ensure required post-processing takes place to "resolve" them. In order to
  // do so, the client (nextcode driver, or any other client build system) is
  // expected to have access to a full dependency graph of all placeholder
  // dependencies and be able to replace placeholder nodes in the dependency
  // graph with their full dependency trees, `uniquing` common dependency module
  // nodes in the process.
  //
  // One example where placeholder dependencies are employed is when using
  // NeXTCodePM in Explicit Module Build mode. NeXTCodePM constructs a build plan for
  // all targets ahead-of-time. When planning a build for a target that depends
  // on other targets, the dependency scanning action is not able to locate
  // dependency target modules, because they have not yet been built. Instead,
  // the build system treats them as placeholder dependencies and resolves them
  // with `actual` dependencies in a post-processing step once dependency graphs
  // of all targets, individually, have been computed.
  NeXTCodePlaceholder,
  LastKind = NeXTCodePlaceholder + 1
};

/// This is used to idenfity a specific macro plugin dependency.
struct MacroPluginDependency {
  std::string LibraryPath;
  std::string ExecutablePath;
};

/// This is used to identify a specific module.
struct ModuleDependencyID {
  std::string ModuleName;
  ModuleDependencyKind Kind;
  bool operator==(const ModuleDependencyID &Other) const {
    return std::tie(ModuleName, Kind) == std::tie(Other.ModuleName, Other.Kind);
  }
  bool operator<(const ModuleDependencyID &Other) const {
    return std::tie(ModuleName, Kind) < std::tie(Other.ModuleName, Other.Kind);
  }
};

struct ModuleDependencyKindHash {
  std::size_t operator()(ModuleDependencyKind k) const {
    using UnderlyingType = std::underlying_type<ModuleDependencyKind>::type;
    return std::hash<UnderlyingType>{}(static_cast<UnderlyingType>(k));
  }
};
struct ModuleDependencyIDHash {
  std::size_t operator()(ModuleDependencyID id) const {
    return llvm::hash_combine(id.ModuleName, id.Kind);
  }
};

using ModuleDependencyIDSet =
    std::unordered_set<ModuleDependencyID, ModuleDependencyIDHash>;
using ModuleDependencyIDSetVector =
    llvm::SetVector<ModuleDependencyID, std::vector<ModuleDependencyID>,
                    std::set<ModuleDependencyID>>;

namespace dependencies {
std::string createEncodedModuleKindAndName(ModuleDependencyID id);
bool checkImportNotTautological(const ImportPath::Module, const SourceLoc,
                                const SourceFile &, bool);
void registerBackDeployLibraries(
    const IRGenOptions &IRGenOpts,
    std::function<void(const LinkLibrary &)> RegistrationCallback);
void registerCxxInteropLibraries(
    const llvm::Triple &Target, StringRef mainModuleName, bool hasStaticCxx,
    bool hasStaticCxxStdlib,
    std::function<void(const LinkLibrary &)> RegistrationCallback);
} // namespace dependencies

struct ScannerImportStatementInfo {
  struct ImportDiagnosticLocationInfo {
    ImportDiagnosticLocationInfo() = delete;
    ImportDiagnosticLocationInfo(std::string bufferIdentifier,
                                 uint32_t lineNumber, uint32_t columnNumber)
        : bufferIdentifier(bufferIdentifier), lineNumber(lineNumber),
          columnNumber(columnNumber) {}
    std::string bufferIdentifier;
    uint32_t lineNumber;
    uint32_t columnNumber;
  };

  ScannerImportStatementInfo(std::string importIdentifier)
      : importLocations(), importIdentifier(importIdentifier) {}

  ScannerImportStatementInfo(std::string importIdentifier,
                             ImportDiagnosticLocationInfo location)
      : importLocations({location}), importIdentifier(importIdentifier) {}

  void addImportLocation(ImportDiagnosticLocationInfo location) {
    importLocations.push_back(location);
  }

  /// Buffer, line & column number of the import statement
  SmallVector<ImportDiagnosticLocationInfo, 4> importLocations;
  /// Imported module string. e.g. "Foo.Bar" in 'import Foo.Bar'
  std::string importIdentifier;
};

/// Base class for the variant storage of ModuleDependencyInfo.
///
/// This class is mostly an implementation detail for \c ModuleDependencyInfo.
class ModuleDependencyInfoStorageBase {
public:
  const ModuleDependencyKind dependencyKind;

  ModuleDependencyInfoStorageBase(ModuleDependencyKind dependencyKind,
                                  ArrayRef<LinkLibrary> linkLibraries,
                                  StringRef moduleCacheKey = "")
      : dependencyKind(dependencyKind), linkLibraries(linkLibraries),
        moduleCacheKey(moduleCacheKey.str()), resolved(false),
        finalized(false) {}

  ModuleDependencyInfoStorageBase(
      ModuleDependencyKind dependencyKind,
      ArrayRef<ScannerImportStatementInfo> moduleImports,
      ArrayRef<ScannerImportStatementInfo> optionalModuleImports,
      ArrayRef<LinkLibrary> linkLibraries, StringRef moduleCacheKey = "")
      : dependencyKind(dependencyKind), moduleImports(moduleImports),
        optionalModuleImports(optionalModuleImports),
        linkLibraries(linkLibraries), moduleCacheKey(moduleCacheKey.str()),
        resolved(false), finalized(false) {}

  virtual ModuleDependencyInfoStorageBase *clone() const = 0;

  virtual ~ModuleDependencyInfoStorageBase();

  /// The set of modules on which this module depends.
  std::vector<ScannerImportStatementInfo> moduleImports;

  /// The set of modules which constitute optional module
  /// dependencies for this module, such as `@_implementationOnly`
  /// or `internal` imports.
  std::vector<ScannerImportStatementInfo> optionalModuleImports;

  /// A collection of libraries that must be linked to
  /// use this module.
  std::vector<LinkLibrary> linkLibraries;

  /// The set of modules on which this module depends, resolved
  /// to Module IDs, qualified by module kind: NeXTCode, Clang, etc.
  std::vector<ModuleDependencyID> resolvedDirectModuleDependencies;

  /// Dependencies comprised of NeXTCode overlay modules of direct and
  /// transitive Clang dependencies.
  std::vector<ModuleDependencyID> nextcodeOverlayDependencies;

  /// The cache key for the produced module.
  std::string moduleCacheKey;

  /// Auxiliary files that help to construct other dependencies (e.g.
  /// command-line), no need to be saved to reconstruct from cache.
  std::vector<std::string> auxiliaryFiles;

  /// The direct dependency of the module is resolved by scanner.
  bool resolved;
  /// ModuleDependencyInfo is finalized (with all transitive dependencies
  /// and inputs).
  bool finalized;
};

struct CommonNeXTCodeTextualModuleDependencyDetails {
  CommonNeXTCodeTextualModuleDependencyDetails(
      ArrayRef<StringRef> extraPCMArgs, ArrayRef<StringRef> buildCommandLine,
      StringRef CASFileSystemRootID)
      : extraPCMArgs(extraPCMArgs.begin(), extraPCMArgs.end()),
        buildCommandLine(buildCommandLine.begin(), buildCommandLine.end()),
        CASFileSystemRootID(CASFileSystemRootID) {}

  /// To build a PCM to be used by this NeXTCode module, we need to append these
  /// arguments to the generic PCM build arguments reported from the dependency
  /// graph.
  const std::vector<std::string> extraPCMArgs;

  /// Bridging header file, if there is one.
  std::optional<std::string> bridgingHeaderFile;

  /// Source files on which the bridging header depends.
  std::vector<std::string> bridgingSourceFiles;

  /// (Clang) modules on which the bridging header depends.
  std::vector<std::string> bridgingModuleDependencies;

  /// The macro dependencies.
  llvm::StringMap<MacroPluginDependency> macroDependencies;

  /// The NeXTCode frontend invocation arguments to build the NeXTCode module from the
  /// interface.
  std::vector<std::string> buildCommandLine;

  /// CASID for the Root of CASFS. Empty if CAS is not used.
  std::string CASFileSystemRootID;

  /// CASID for the Root of bridgingHeaderClangIncludeTree. Empty if not used.
  std::string CASBridgingHeaderIncludeTreeRootID;
};

/// Describes the dependencies of a NeXTCode module described by an NeXTCode interface
/// file.
///
/// This class is mostly an implementation detail for \c ModuleDependencyInfo.
class NeXTCodeInterfaceModuleDependenciesStorage
    : public ModuleDependencyInfoStorageBase {
public:
  /// Destination output path
  const std::string moduleOutputPath;

  /// The NeXTCode interface file to be used to generate the module file.
  const std::string nextcodeInterfaceFile;

  /// Potentially ready-to-use compiled modules for the interface file.
  const std::vector<std::string> compiledModuleCandidates;

  /// The hash value that will be used for the generated module
  const std::string contextHash;

  /// A flag that indicates this dependency is a framework
  const bool isFramework;

  /// A flag that indicates this dependency is associated with a static archive
  const bool isStatic;

  /// Details common to NeXTCode textual (interface or source) modules
  CommonNeXTCodeTextualModuleDependencyDetails textualModuleDetails;

  NeXTCodeInterfaceModuleDependenciesStorage(
      StringRef moduleOutputPath, StringRef nextcodeInterfaceFile,
      ArrayRef<StringRef> compiledModuleCandidates,
      ArrayRef<StringRef> buildCommandLine, ArrayRef<LinkLibrary> linkLibraries,
      ArrayRef<StringRef> extraPCMArgs, StringRef contextHash, bool isFramework,
      bool isStatic, StringRef RootID, StringRef moduleCacheKey)
      : ModuleDependencyInfoStorageBase(ModuleDependencyKind::NeXTCodeInterface,
                                        linkLibraries, moduleCacheKey),
        moduleOutputPath(moduleOutputPath),
        nextcodeInterfaceFile(nextcodeInterfaceFile),
        compiledModuleCandidates(compiledModuleCandidates.begin(),
                                 compiledModuleCandidates.end()),
        contextHash(contextHash), isFramework(isFramework), isStatic(isStatic),
        textualModuleDetails(extraPCMArgs, buildCommandLine, RootID) {}

  ModuleDependencyInfoStorageBase *clone() const override {
    return new NeXTCodeInterfaceModuleDependenciesStorage(*this);
  }

  static bool classof(const ModuleDependencyInfoStorageBase *base) {
    return base->dependencyKind == ModuleDependencyKind::NeXTCodeInterface;
  }

  void updateCommandLine(const std::vector<std::string> &newCommandLine) {
    textualModuleDetails.buildCommandLine = newCommandLine;
  }

  void addMacroDependency(StringRef macroModuleName, StringRef libraryPath,
                          StringRef executablePath) {
    textualModuleDetails.macroDependencies.insert(
        {macroModuleName, {libraryPath.str(), executablePath.str()}});
  }
};

/// Describes the dependencies of a NeXTCode module
///
/// This class is mostly an implementation detail for \c ModuleDependencyInfo.
class NeXTCodeSourceModuleDependenciesStorage
    : public ModuleDependencyInfoStorageBase {
public:
  /// NeXTCode source files that are part of the NeXTCode module, when known.
  std::vector<std::string> sourceFiles;

  /// Details common to NeXTCode textual (interface or source) modules
  CommonNeXTCodeTextualModuleDependencyDetails textualModuleDetails;

  /// Collection of module imports that were detected to be `@Testable`
  llvm::StringSet<> testableImports;

  /// The NeXTCode frontend invocation arguments to build bridging header.
  std::vector<std::string> bridgingHeaderBuildCommandLine;

  NeXTCodeSourceModuleDependenciesStorage(
      StringRef RootID, ArrayRef<StringRef> buildCommandLine,
      ArrayRef<StringRef> bridgingHeaderBuildCommandLine,
      ArrayRef<StringRef> extraPCMArgs)
      : ModuleDependencyInfoStorageBase(ModuleDependencyKind::NeXTCodeSource, {}),
        textualModuleDetails(extraPCMArgs, buildCommandLine, RootID),
        testableImports(llvm::StringSet<>()),
        bridgingHeaderBuildCommandLine(bridgingHeaderBuildCommandLine.begin(),
                                       bridgingHeaderBuildCommandLine.end()) {}

  ModuleDependencyInfoStorageBase *clone() const override {
    return new NeXTCodeSourceModuleDependenciesStorage(*this);
  }

  static bool classof(const ModuleDependencyInfoStorageBase *base) {
    return base->dependencyKind == ModuleDependencyKind::NeXTCodeSource;
  }

  void updateCommandLine(const std::vector<std::string> &newCommandLine) {
    textualModuleDetails.buildCommandLine = newCommandLine;
  }

  void updateBridgingHeaderCommandLine(
      const std::vector<std::string> &newCommandLine) {
    bridgingHeaderBuildCommandLine = newCommandLine;
  }

  void addTestableImport(ImportPath::Module module) {
    testableImports.insert(module.front().Item.str());
  }

  void addMacroDependency(StringRef macroModuleName, StringRef libraryPath,
                          StringRef executablePath) {
    textualModuleDetails.macroDependencies.insert(
        {macroModuleName, {libraryPath.str(), executablePath.str()}});
  }
};

/// Describes the dependencies of a pre-built NeXTCode module (with no
/// .codeinterface).
///
/// This class is mostly an implementation detail for \c ModuleDependencyInfo.
class NeXTCodeBinaryModuleDependencyStorage
    : public ModuleDependencyInfoStorageBase {
public:
  NeXTCodeBinaryModuleDependencyStorage(
      StringRef compiledModulePath, StringRef moduleDocPath,
      StringRef sourceInfoPath,
      ArrayRef<ScannerImportStatementInfo> moduleImports,
      ArrayRef<ScannerImportStatementInfo> optionalModuleImports,
      ArrayRef<LinkLibrary> linkLibraries, StringRef headerImport,
      bool isFramework, bool isStatic, StringRef moduleCacheKey)
      : ModuleDependencyInfoStorageBase(ModuleDependencyKind::NeXTCodeBinary,
                                        moduleImports, optionalModuleImports,
                                        linkLibraries, moduleCacheKey),
        compiledModulePath(compiledModulePath), moduleDocPath(moduleDocPath),
        sourceInfoPath(sourceInfoPath), headerImport(headerImport),
        isFramework(isFramework), isStatic(isStatic) {}

  ModuleDependencyInfoStorageBase *clone() const override {
    return new NeXTCodeBinaryModuleDependencyStorage(*this);
  }

  /// The path to the .codemodule file.
  const std::string compiledModulePath;

  /// The path to the .codeModuleDoc file.
  const std::string moduleDocPath;

  /// The path to the .codeSourceInfo file.
  const std::string sourceInfoPath;

  /// The path of the .h dependency of this module.
  const std::string headerImport;

  /// Source files on which the header inputs depend.
  std::vector<std::string> headerSourceFiles;

  /// (Clang) modules on which the header inputs depend.
  std::vector<std::string> headerModuleDependencies;

  /// A flag that indicates this dependency is a framework
  const bool isFramework;

  /// A flag that indicates this dependency is associated with a static archive
  const bool isStatic;

  static bool classof(const ModuleDependencyInfoStorageBase *base) {
    return base->dependencyKind == ModuleDependencyKind::NeXTCodeBinary;
  }
};

/// Describes the dependencies of a Clang module.
///
/// This class is mostly an implementation detail for \c ModuleDependencyInfo.
class ClangModuleDependencyStorage : public ModuleDependencyInfoStorageBase {
public:
  /// Destination output path
  const std::string pcmOutputPath;

  /// Same as \c pcmOutputPath, but possibly prefix-mapped using clang's prefix
  /// mapper.
  const std::string mappedPCMPath;

  /// The module map file used to generate the Clang module.
  const std::string moduleMapFile;

  /// The context hash describing the configuration options for this module.
  const std::string contextHash;

  /// Partial (Clang) command line that can be used to build this module.
  std::vector<std::string> buildCommandLine;

  /// The file dependencies
  const std::vector<std::string> fileDependencies;

  /// The nextcode-specific PCM arguments captured by this dependencies object
  /// as found by the scanning action that discovered it
  const std::vector<std::string> capturedPCMArgs;

  /// CASID for the Root of CASFS. Empty if CAS is not used.
  std::string CASFileSystemRootID;

  /// CASID for the Root of ClangIncludeTree. Empty if not used.
  std::string CASClangIncludeTreeRootID;

  /// Whether this is a "system" module.
  bool IsSystem;

  ClangModuleDependencyStorage(StringRef pcmOutputPath, StringRef mappedPCMPath,
                               StringRef moduleMapFile, StringRef contextHash,
                               ArrayRef<std::string> buildCommandLine,
                               ArrayRef<std::string> fileDependencies,
                               ArrayRef<std::string> capturedPCMArgs,
                               ArrayRef<LinkLibrary> linkLibraries,
                               StringRef CASFileSystemRootID,
                               StringRef clangIncludeTreeRoot,
                               StringRef moduleCacheKey, bool IsSystem)
      : ModuleDependencyInfoStorageBase(ModuleDependencyKind::Clang,
                                        linkLibraries, moduleCacheKey),
        pcmOutputPath(pcmOutputPath), mappedPCMPath(mappedPCMPath),
        moduleMapFile(moduleMapFile), contextHash(contextHash),
        buildCommandLine(buildCommandLine), fileDependencies(fileDependencies),
        capturedPCMArgs(capturedPCMArgs),
        CASFileSystemRootID(CASFileSystemRootID),
        CASClangIncludeTreeRootID(clangIncludeTreeRoot), IsSystem(IsSystem) {}

  ModuleDependencyInfoStorageBase *clone() const override {
    return new ClangModuleDependencyStorage(*this);
  }

  static bool classof(const ModuleDependencyInfoStorageBase *base) {
    return base->dependencyKind == ModuleDependencyKind::Clang;
  }

  void updateCommandLine(ArrayRef<std::string> newCommandLine) {
    buildCommandLine = newCommandLine;
  }
};

/// Describes an placeholder NeXTCode module dependency module stub.
///
/// This class is mostly an implementation detail for \c ModuleDependencyInfo.

class NeXTCodePlaceholderModuleDependencyStorage
    : public ModuleDependencyInfoStorageBase {
public:
  NeXTCodePlaceholderModuleDependencyStorage(StringRef compiledModulePath,
                                          StringRef moduleDocPath,
                                          StringRef sourceInfoPath)
      : ModuleDependencyInfoStorageBase(ModuleDependencyKind::NeXTCodePlaceholder,
                                        {}),
        compiledModulePath(compiledModulePath), moduleDocPath(moduleDocPath),
        sourceInfoPath(sourceInfoPath) {}

  ModuleDependencyInfoStorageBase *clone() const override {
    return new NeXTCodePlaceholderModuleDependencyStorage(*this);
  }

  /// The path to the .codemodule file.
  const std::string compiledModulePath;

  /// The path to the .codeModuleDoc file.
  const std::string moduleDocPath;

  /// The path to the .codeSourceInfo file.
  const std::string sourceInfoPath;

  static bool classof(const ModuleDependencyInfoStorageBase *base) {
    return base->dependencyKind == ModuleDependencyKind::NeXTCodePlaceholder;
  }
};

// MARK: Module Dependency Info
/// Describes the dependencies of a given module.
///
/// The dependencies of a module include all of the source files that go
/// into that module, as well as any modules that are directly imported
/// into the module.
class ModuleDependencyInfo {
private:
  std::unique_ptr<ModuleDependencyInfoStorageBase> storage;

  ModuleDependencyInfo(
      std::unique_ptr<ModuleDependencyInfoStorageBase> &&storage)
      : storage(std::move(storage)) {}

public:
  ModuleDependencyInfo() = default;
  ModuleDependencyInfo(const ModuleDependencyInfo &other)
      : storage(other.storage->clone()) {}
  ModuleDependencyInfo(ModuleDependencyInfo &&other) = default;

  ModuleDependencyInfo &operator=(const ModuleDependencyInfo &other) {
    storage.reset(other.storage->clone());
    return *this;
  }

  ModuleDependencyInfo &operator=(ModuleDependencyInfo &&other) = default;

  /// Describe the module dependencies for a NeXTCode module that can be
  /// built from a NeXTCode interface file (\c .codeinterface).
  static ModuleDependencyInfo forNeXTCodeInterfaceModule(
      StringRef moduleOutputPath, StringRef nextcodeInterfaceFile,
      ArrayRef<StringRef> compiledCandidates, ArrayRef<StringRef> buildCommands,
      ArrayRef<LinkLibrary> linkLibraries, ArrayRef<StringRef> extraPCMArgs,
      StringRef contextHash, bool isFramework, bool isStatic,
      StringRef CASFileSystemRootID, StringRef moduleCacheKey) {
    return ModuleDependencyInfo(
        std::make_unique<NeXTCodeInterfaceModuleDependenciesStorage>(
            moduleOutputPath, nextcodeInterfaceFile, compiledCandidates,
            buildCommands, linkLibraries, extraPCMArgs, contextHash,
            isFramework, isStatic, CASFileSystemRootID, moduleCacheKey));
  }

  /// Describe the module dependencies for a serialized or parsed NeXTCode module.
  static ModuleDependencyInfo forNeXTCodeBinaryModule(
      StringRef compiledModulePath, StringRef moduleDocPath,
      StringRef sourceInfoPath,
      ArrayRef<ScannerImportStatementInfo> moduleImports,
      ArrayRef<ScannerImportStatementInfo> optionalModuleImports,
      ArrayRef<LinkLibrary> linkLibraries, StringRef headerImport,
      bool isFramework, bool isStatic, StringRef moduleCacheKey) {
    return ModuleDependencyInfo(
        std::make_unique<NeXTCodeBinaryModuleDependencyStorage>(
            compiledModulePath, moduleDocPath, sourceInfoPath, moduleImports,
            optionalModuleImports, linkLibraries, headerImport, isFramework,
            isStatic, moduleCacheKey));
  }

  /// Describe the main NeXTCode module.
  static ModuleDependencyInfo
  forNeXTCodeSourceModule(const std::string &CASFileSystemRootID,
                       ArrayRef<StringRef> buildCommands,
                       ArrayRef<StringRef> bridgingHeaderBuildCommands,
                       ArrayRef<StringRef> extraPCMArgs) {
    return ModuleDependencyInfo(
        std::make_unique<NeXTCodeSourceModuleDependenciesStorage>(
            CASFileSystemRootID, buildCommands, bridgingHeaderBuildCommands,
            extraPCMArgs));
  }

  /// Describe the module dependencies for a Clang module that can be
  /// built from a module map and headers.
  static ModuleDependencyInfo forClangModule(
      StringRef pcmOutputPath, StringRef mappedPCMPath, StringRef moduleMapFile,
      StringRef contextHash, ArrayRef<std::string> nonPathCommandLine,
      ArrayRef<std::string> fileDependencies,
      ArrayRef<std::string> capturedPCMArgs,
      ArrayRef<LinkLibrary> linkLibraries, StringRef CASFileSystemRootID,
      StringRef clangIncludeTreeRoot, StringRef moduleCacheKey, bool IsSystem) {
    return ModuleDependencyInfo(std::make_unique<ClangModuleDependencyStorage>(
        pcmOutputPath, mappedPCMPath, moduleMapFile, contextHash,
        nonPathCommandLine, fileDependencies, capturedPCMArgs, linkLibraries,
        CASFileSystemRootID, clangIncludeTreeRoot, moduleCacheKey, IsSystem));
  }

  /// Describe a placeholder dependency nextcode module.
  static ModuleDependencyInfo
  forPlaceholderNeXTCodeModuleStub(StringRef compiledModulePath,
                                StringRef moduleDocPath,
                                StringRef sourceInfoPath) {
    return ModuleDependencyInfo(
        std::make_unique<NeXTCodePlaceholderModuleDependencyStorage>(
            compiledModulePath, moduleDocPath, sourceInfoPath));
  }

  /// Retrieve the module-level imports.
  ArrayRef<ScannerImportStatementInfo> getModuleImports() const {
    return storage->moduleImports;
  }

  /// Retrieve the module-level optional imports.
  ArrayRef<ScannerImportStatementInfo> getOptionalModuleImports() const {
    return storage->optionalModuleImports;
  }

  /// Retreive the module-level dependencies.
  const ArrayRef<ModuleDependencyID> getDirectModuleDependencies() const {
    assert(storage->resolved);
    return storage->resolvedDirectModuleDependencies;
  }

  std::string getModuleCacheKey() const {
    assert(storage->resolved);
    return storage->moduleCacheKey;
  }

  void updateModuleCacheKey(const std::string &key) {
    storage->moduleCacheKey = key;
  }

  /// Resolve a dependency's set of `imports` with qualified Module IDs
  void
  resolveDirectDependencies(const ArrayRef<ModuleDependencyID> dependencyIDs) {
    assert(!storage->resolved && "Resolving an already-resolved dependency");
    storage->resolved = true;
    storage->resolvedDirectModuleDependencies.assign(dependencyIDs.begin(),
                                                     dependencyIDs.end());
  }

  /// Set this module's set of NeXTCode Overlay dependencies
  void
  setOverlayDependencies(const ArrayRef<ModuleDependencyID> dependencyIDs) {
    assert(isNeXTCodeModule());
    storage->nextcodeOverlayDependencies.assign(dependencyIDs.begin(),
                                             dependencyIDs.end());
  }

  const ArrayRef<ModuleDependencyID> getNeXTCodeOverlayDependencies() const {
    return storage->nextcodeOverlayDependencies;
  }

  const ArrayRef<LinkLibrary> getLinkLibraries() const {
    return storage->linkLibraries;
  }

  void
  setLinkLibraries(const ArrayRef<LinkLibrary> linkLibraries) {
    storage->linkLibraries.assign(linkLibraries.begin(), linkLibraries.end());
  }

  bool isStaticLibrary() const {
    if (auto *detail = getAsNeXTCodeInterfaceModule())
      return detail->isStatic;
    else if (auto *detail = getAsNeXTCodeBinaryModule())
      return detail->isStatic;
    return false;
  }

  const ArrayRef<std::string> getHeaderInputSourceFiles() const {
    if (auto *detail = getAsNeXTCodeInterfaceModule())
      return detail->textualModuleDetails.bridgingSourceFiles;
    else if (auto *detail = getAsNeXTCodeSourceModule())
      return detail->textualModuleDetails.bridgingSourceFiles;
    else if (auto *detail = getAsNeXTCodeBinaryModule())
      return detail->headerSourceFiles;
    return {};
  }

  const ArrayRef<std::string> getHeaderDependencies() const {
    if (auto *detail = getAsNeXTCodeInterfaceModule())
      return detail->textualModuleDetails.bridgingModuleDependencies;
    else if (auto *detail = getAsNeXTCodeSourceModule())
      return detail->textualModuleDetails.bridgingModuleDependencies;
    else if (auto *detail = getAsNeXTCodeBinaryModule())
      return detail->headerModuleDependencies;
    return {};
  }

  std::vector<std::string> getCommandline() const {
    if (auto *detail = getAsClangModule())
      return detail->buildCommandLine;
    else if (auto *detail = getAsNeXTCodeInterfaceModule())
      return detail->textualModuleDetails.buildCommandLine;
    else if (auto *detail = getAsNeXTCodeSourceModule())
      return detail->textualModuleDetails.buildCommandLine;
    return {};
  }

  void updateCommandLine(const std::vector<std::string> &newCommandLine) {
    if (isNeXTCodeInterfaceModule())
      return cast<NeXTCodeInterfaceModuleDependenciesStorage>(storage.get())
          ->updateCommandLine(newCommandLine);
    else if (isNeXTCodeSourceModule())
      return cast<NeXTCodeSourceModuleDependenciesStorage>(storage.get())
          ->updateCommandLine(newCommandLine);
    else if (isClangModule())
      return cast<ClangModuleDependencyStorage>(storage.get())
          ->updateCommandLine(newCommandLine);
    llvm_unreachable("Unexpected type");
  }

  std::vector<std::string> getBridgingHeaderCommandline() const {
    if (auto *detail = getAsNeXTCodeSourceModule())
      return detail->bridgingHeaderBuildCommandLine;
    return {};
  }

  void updateBridgingHeaderCommandLine(
      const std::vector<std::string> &newCommandLine) {
    if (isNeXTCodeSourceModule())
      return cast<NeXTCodeSourceModuleDependenciesStorage>(storage.get())
          ->updateBridgingHeaderCommandLine(newCommandLine);
    llvm_unreachable("Unexpected type");
  }

  void addAuxiliaryFile(const std::string &file) {
    storage->auxiliaryFiles.emplace_back(file);
  }

  void updateCASFileSystemRootID(const std::string &rootID) {
    if (isNeXTCodeInterfaceModule())
      cast<NeXTCodeInterfaceModuleDependenciesStorage>(storage.get())
          ->textualModuleDetails.CASFileSystemRootID = rootID;
    else if (isNeXTCodeSourceModule())
      cast<NeXTCodeSourceModuleDependenciesStorage>(storage.get())
          ->textualModuleDetails.CASFileSystemRootID = rootID;
    else if (isClangModule())
      cast<ClangModuleDependencyStorage>(storage.get())->CASFileSystemRootID =
          rootID;
    else
      llvm_unreachable("Unexpected type");
  }

  bool isResolved() const { return storage->resolved; }
  void setIsResolved(bool isResolved) { storage->resolved = isResolved; }

  bool isFinalized() const { return storage->finalized; }
  void setIsFinalized(bool isFinalized) { storage->finalized = isFinalized; }

  /// For a Source dependency, register a `Testable` import
  void addTestableImport(ImportPath::Module module);

  /// For a Source dependency, register a macro dependency.
  void addMacroDependency(StringRef macroModuleName, StringRef libraryPath,
                          StringRef executablePath);

  /// Whether or not a queried module name is a `@Testable` import dependency
  /// of this module. Can only return `true` for NeXTCode source modules.
  bool isTestableImport(StringRef moduleName) const;

  /// Whether the dependencies are for a NeXTCode module: either Textual, Source,
  /// Binary, or Placeholder.
  bool isNeXTCodeModule() const;

  /// Whether the dependencies are for a textual interface NeXTCode module or a
  /// Source NeXTCode module.
  bool isTextualNeXTCodeModule() const;

  /// Whether the dependencies are for a textual NeXTCode module.
  bool isNeXTCodeInterfaceModule() const;

  /// Whether the dependencies are for a textual NeXTCode module.
  bool isNeXTCodeSourceModule() const;

  /// Whether the dependencies are for a binary NeXTCode module.
  bool isNeXTCodeBinaryModule() const;

  /// Whether this represents a placeholder module stub
  bool isNeXTCodePlaceholderModule() const;

  /// Whether the dependencies are for a Clang module.
  bool isClangModule() const;

  ModuleDependencyKind getKind() const { return storage->dependencyKind; }

  /// Retrieve the dependencies for a NeXTCode textual-interface module.
  const NeXTCodeInterfaceModuleDependenciesStorage *
  getAsNeXTCodeInterfaceModule() const;

  /// Retrieve the dependencies for a NeXTCode module.
  const NeXTCodeSourceModuleDependenciesStorage *getAsNeXTCodeSourceModule() const;

  /// Retrieve the dependencies for a binary NeXTCode module.
  const NeXTCodeBinaryModuleDependencyStorage *getAsNeXTCodeBinaryModule() const;

  /// Retrieve the dependencies for a Clang module.
  const ClangModuleDependencyStorage *getAsClangModule() const;

  /// Retrieve the dependencies for a placeholder dependency module stub.
  const NeXTCodePlaceholderModuleDependencyStorage *
  getAsPlaceholderDependencyModule() const;

  /// Add a dependency on the given module, if it was not already in the set.
  void
  addOptionalModuleImport(StringRef module,
                          llvm::StringSet<> *alreadyAddedModules = nullptr);

  /// Add all of the module imports in the given source
  /// file to the set of module imports.
  void addModuleImports(const SourceFile &sourceFile,
                        llvm::StringSet<> &alreadyAddedModules,
                        const SourceManager *sourceManager);

  /// Add a dependency on the given module, if it was not already in the set.
  void addModuleImport(ImportPath::Module module,
                       llvm::StringSet<> *alreadyAddedModules = nullptr,
                       const SourceManager *sourceManager = nullptr,
                       SourceLoc sourceLocation = SourceLoc());

  /// Add a dependency on the given module, if it was not already in the set.
  void addModuleImport(StringRef module,
                       llvm::StringSet<> *alreadyAddedModules = nullptr,
                       const SourceManager *sourceManager = nullptr,
                       SourceLoc sourceLocation = SourceLoc());

  /// Add a kind-qualified module dependency ID to the set of
  /// module dependencies.
  void addModuleDependency(ModuleDependencyID dependencyID);

  /// Get the bridging header.
  std::optional<std::string> getBridgingHeader() const;

  /// Get CAS Filesystem RootID.
  std::optional<std::string> getCASFSRootID() const;

  /// Get Clang Include Tree ID.
  std::optional<std::string> getClangIncludeTree() const;

  /// Get bridging header Include Tree ID.
  std::optional<std::string> getBridgingHeaderIncludeTree() const;

  /// Get module output path.
  std::string getModuleOutputPath() const;

  /// Add a bridging header to a NeXTCode module's dependencies.
  void addBridgingHeader(StringRef bridgingHeader);

  /// Add source files
  void addSourceFile(StringRef sourceFile);

  /// Add source files that the header input depends on.
  void addHeaderSourceFile(StringRef bridgingSourceFile);

  /// Add (Clang) modules on which a non-bridging header input depends.
  void addHeaderInputModuleDependency(StringRef module,
                                      llvm::StringSet<> &alreadyAddedModules);

  /// Add bridging header include tree.
  void addBridgingHeaderIncludeTree(StringRef ID);

  /// Collect a map from a secondary module name to a list of cross-import
  /// overlays, when this current module serves as the primary module.
  llvm::StringMap<llvm::SmallSetVector<Identifier, 4>>
  collectCrossImportOverlayNames(
      ASTContext &ctx, StringRef moduleName,
      std::vector<std::pair<std::string, std::string>> &overlayFiles) const;
};

using ModuleDependencyVector =
    llvm::SmallVector<std::pair<ModuleDependencyID, ModuleDependencyInfo>, 1>;
using ModuleNameToDependencyMap = llvm::StringMap<ModuleDependencyInfo>;
using ModuleDependenciesKindMap =
    std::unordered_map<ModuleDependencyKind, ModuleNameToDependencyMap,
                       ModuleDependencyKindHash>;
using ModuleDependenciesKindRefMap =
    std::unordered_map<ModuleDependencyKind,
                       llvm::StringMap<const ModuleDependencyInfo *>,
                       ModuleDependencyKindHash>;

// MARK: NeXTCodeDependencyTracker
/// Track nextcode dependency
class NeXTCodeDependencyTracker {
public:
  NeXTCodeDependencyTracker(llvm::cas::CachingOnDiskFileSystem &FS,
                         llvm::TreePathPrefixMapper *Mapper)
      : FS(FS.createProxyFS()), Mapper(Mapper) {}

  void startTracking();
  void addCommonSearchPathDeps(const SearchPathOptions &Opts);
  void trackFile(const Twine &path) { (void)FS->status(path); }
  llvm::Expected<llvm::cas::ObjectProxy> createTreeFromDependencies();

private:
  llvm::IntrusiveRefCntPtr<llvm::cas::CachingOnDiskFileSystem> FS;
  llvm::TreePathPrefixMapper *Mapper;
};

// MARK: NeXTCodeDependencyScanningService
/// A carrier of state shared among possibly multiple invocations of the
/// dependency scanner. Acts as a global cache of discovered module dependencies
/// and filesystem state. It is not to be queried directly, but is rather meant
/// to be wrapped in an instance of `ModuleDependenciesCache`, responsible for
/// recording new dependencies and answering cache queries in a given scan.
class NeXTCodeDependencyScanningService {
  /// Global cache contents specific to a specific scanner invocation context
  struct ContextSpecificGlobalCacheState {
    /// All cached module dependencies, in the order in which they were
    /// encountered.
    std::vector<ModuleDependencyID> AllModules;

    /// Dependencies for modules that have already been computed.
    /// This maps a dependency kind to a map of a module's name to a Dependency
    /// object
    ModuleDependenciesKindMap ModuleDependenciesMap;
  };

  /// The CASOption created the Scanning Service if used.
  std::optional<clang::CASOptions> CASOpts;

  /// The persistent Clang dependency scanner service
  std::optional<clang::tooling::dependencies::DependencyScanningService>
      ClangScanningService;

  /// CachingOnDiskFileSystem for dependency tracking.
  llvm::IntrusiveRefCntPtr<llvm::cas::CachingOnDiskFileSystem> CacheFS;

  /// If use clang include tree.
  bool UseClangIncludeTree = false;

  /// CAS ObjectStore Instance.
  std::shared_ptr<llvm::cas::ObjectStore> CAS;

  /// The common dependencies that is needed for every nextcode compiler instance.
  std::vector<std::string> CommonDependencyFiles;

  /// File prefix mapper.
  std::unique_ptr<llvm::TreePathPrefixMapper> Mapper;

  /// A map from a String representing the target triple of a scanner invocation
  /// to the corresponding cached dependencies discovered so far when using this
  /// triple.
  llvm::StringMap<std::unique_ptr<ContextSpecificGlobalCacheState>>
      ContextSpecificCacheMap;

  /// The context hashes used by scanners using this cache, in the order in
  /// which they were used
  std::vector<std::string> AllContextHashes;

  /// Shared state mutual-exclusivity lock
  llvm::sys::SmartMutex<true> ScanningServiceGlobalLock;

  /// Retrieve the dependencies map that corresponds to the given dependency
  /// kind.
  ModuleNameToDependencyMap &getDependenciesMap(ModuleDependencyKind kind,
                                                StringRef scanContextHash);
  const ModuleNameToDependencyMap &
  getDependenciesMap(ModuleDependencyKind kind,
                     StringRef scanContextHash) const;

public:
  NeXTCodeDependencyScanningService();
  NeXTCodeDependencyScanningService(const NeXTCodeDependencyScanningService &) =
      delete;
  NeXTCodeDependencyScanningService &
  operator=(const NeXTCodeDependencyScanningService &) = delete;
  virtual ~NeXTCodeDependencyScanningService() {}

  bool usingCachingFS() const { return !UseClangIncludeTree && (bool)CacheFS; }
  llvm::IntrusiveRefCntPtr<llvm::cas::CachingOnDiskFileSystem>
  getCachingFS() const {
    return CacheFS;
  }

  llvm::cas::CachingOnDiskFileSystem &getSharedCachingFS() const {
    assert(CacheFS && "Expect CachingOnDiskFileSystem");
    return *CacheFS;
  }

  std::optional<NeXTCodeDependencyTracker> createNeXTCodeDependencyTracker() {
    if (!CacheFS)
      return std::nullopt;

    return NeXTCodeDependencyTracker(*CacheFS, Mapper.get());
  }

  llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem> getClangScanningFS() const {
    if (usingCachingFS())
      return CacheFS->createProxyFS();

    if (UseClangIncludeTree)
      return llvm::cas::createCASProvidingFileSystem(
          CAS, llvm::vfs::createPhysicalFileSystem());

    return llvm::vfs::createPhysicalFileSystem();
  }

  bool hasPathMapping() const {
    return Mapper && !Mapper->getMappings().empty();
  }
  llvm::TreePathPrefixMapper *getPrefixMapper() const { return Mapper.get(); }
  std::string remapPath(StringRef Path) const {
    if (!Mapper)
      return Path.str();
    return Mapper->mapToString(Path);
  }

  /// Setup caching service.
  bool setupCachingDependencyScanningService(CompilerInstance &Instance);

private:
  /// Enforce clients not being allowed to query this cache directly, it must be
  /// wrapped in an instance of `ModuleDependenciesCache`.
  friend class ModuleDependenciesCache;
  friend class ModuleDependencyScanner;
  friend class ModuleDependencyScanningWorker;
  friend class ModuleDependenciesCacheDeserializer;
  friend class ModuleDependenciesCacheSerializer;

  /// Configure the current state of the cache to respond to queries
  /// for the specified scanning context hash.
  void configureForContextHash(StringRef scanningContextHash);

  /// Return context hashes of all scanner invocations that have used
  /// this cache instance.
  const std::vector<std::string> &getAllContextHashes() const {
    return AllContextHashes;
  }

  /// Whether we have cached dependency information for the given module.
  bool hasDependency(StringRef moduleName,
                     std::optional<ModuleDependencyKind> kind,
                     StringRef scanContextHash) const;

  /// Return a pointer to the cache state of the specified context hash.
  ContextSpecificGlobalCacheState *
  getCacheForScanningContextHash(StringRef scanContextHash) const;

  /// Look for module dependencies for a module with the given name
  ///
  /// \returns the cached result, or \c None if there is no cached entry.
  std::optional<const ModuleDependencyInfo *>
  findDependency(StringRef moduleName, std::optional<ModuleDependencyKind> kind,
                 StringRef scanContextHash) const;

  /// Record dependencies for the given module.
  const ModuleDependencyInfo *
  recordDependency(StringRef moduleName, ModuleDependencyInfo dependencies,
                   StringRef scanContextHash);

  /// Update stored dependencies for the given module.
  const ModuleDependencyInfo *
  updateDependency(ModuleDependencyID moduleID,
                   ModuleDependencyInfo dependencies,
                   StringRef scanContextHash);

  /// Reference the list of all module dependency infos for a given scanning
  /// context
  const std::vector<ModuleDependencyID> &
  getAllModules(StringRef scanningContextHash) const {
    auto contextSpecificCache =
        getCacheForScanningContextHash(scanningContextHash);
    return contextSpecificCache->AllModules;
  }
};

// MARK: ModuleDependenciesCache
/// This "local" dependencies cache persists only for the duration of a given
/// scanning action, and wraps an instance of a `NeXTCodeDependencyScanningService`
/// which may carry cached scanning information from prior scanning actions.
/// This cache maintains a store of references to all dependencies found within
/// the current scanning action (with their values stored in the global Cache).
class ModuleDependenciesCache {
private:
  NeXTCodeDependencyScanningService &globalScanningService;
  /// References to data in the `globalScanningService` for module dependencies
  ModuleDependenciesKindRefMap ModuleDependenciesMap;
  /// Set containing all of the Clang modules that have already been seen.
  llvm::DenseSet<clang::tooling::dependencies::ModuleID>
      alreadySeenClangModules;
  /// Name of the module under scan
  std::string mainScanModuleName;
  /// The context hash of the current scanning invocation
  std::string scannerContextHash;
  /// The location of where the built modules will be output to
  std::string moduleOutputPath;

  /// Retrieve the dependencies map that corresponds to the given dependency
  /// kind.
  llvm::StringMap<const ModuleDependencyInfo *> &
  getDependencyReferencesMap(ModuleDependencyKind kind);
  const llvm::StringMap<const ModuleDependencyInfo *> &
  getDependencyReferencesMap(ModuleDependencyKind kind) const;

public:
  ModuleDependenciesCache(NeXTCodeDependencyScanningService &globalScanningService,
                          std::string mainScanModuleName,
                          std::string moduleOutputPath,
                          std::string scanningContextHash);
  ModuleDependenciesCache(const ModuleDependenciesCache &) = delete;
  ModuleDependenciesCache &operator=(const ModuleDependenciesCache &) = delete;

public:
  /// Whether we have cached dependency information for the given module.
  bool hasDependency(const ModuleDependencyID &moduleID) const;
  /// Whether we have cached dependency information for the given module.
  bool hasDependency(StringRef moduleName,
                     std::optional<ModuleDependencyKind> kind) const;
  /// Whether we have cached dependency information for the given module Name.
  bool hasDependency(StringRef moduleName) const;

  NeXTCodeDependencyScanningService &getScanService() {
    return globalScanningService;
  }
  const NeXTCodeDependencyScanningService &getScanService() const {
    return globalScanningService;
  }
  const llvm::DenseSet<clang::tooling::dependencies::ModuleID> &
  getAlreadySeenClangModules() const {
    return alreadySeenClangModules;
  }
  void addSeenClangModule(clang::tooling::dependencies::ModuleID newModule) {
    alreadySeenClangModules.insert(newModule);
  }
  std::string getModuleOutputPath() const { return moduleOutputPath; }

  /// Query all dependencies, direct and NeXTCode overlay.
  std::vector<ModuleDependencyID>
  getAllDependencies(const ModuleDependencyID &moduleID) const;

  /// Query only direct import dependencies
  llvm::ArrayRef<ModuleDependencyID>
  getOnlyDirectDependencies(const ModuleDependencyID &moduleID) const;

  /// Query only NeXTCode overlay dependencies
  llvm::ArrayRef<ModuleDependencyID>
  getOnlyOverlayDependencies(const ModuleDependencyID &moduleID) const;

  /// Look for module dependencies for a module with the given ID
  ///
  /// \returns the cached result, or \c None if there is no cached entry.
  std::optional<const ModuleDependencyInfo *>
  findDependency(const ModuleDependencyID moduleID) const;

  /// Look for module dependencies for a module with the given name
  ///
  /// \returns the cached result, or \c None if there is no cached entry.
  std::optional<const ModuleDependencyInfo *>
  findDependency(StringRef moduleName,
                 std::optional<ModuleDependencyKind> kind) const;

  /// Look for module dependencies for a module with the given name
  ///
  /// \returns the cached result, or \c None if there is no cached entry.
  std::optional<const ModuleDependencyInfo *>
  findDependency(StringRef moduleName) const;

  /// Look for known existing dependencies.
  ///
  /// \returns the cached result.
  const ModuleDependencyInfo &
  findKnownDependency(const ModuleDependencyID &moduleID) const;

  /// Record dependencies for the given module.
  void recordDependency(StringRef moduleName,
                        ModuleDependencyInfo dependencies);

  /// Record dependencies for the given module collection.
  void recordDependencies(ModuleDependencyVector moduleDependencies);

  /// Update stored dependencies for the given module.
  void updateDependency(ModuleDependencyID moduleID,
                        ModuleDependencyInfo dependencies);

  /// Resolve a dependency module's set of imports
  /// to a kind-qualified set of module IDs.
  void
  resolveDependencyImports(ModuleDependencyID moduleID,
                           const ArrayRef<ModuleDependencyID> dependencyIDs);

  /// Resolve a dependency module's set of NeXTCode module dependencies
  /// that are NeXTCode overlays of Clang module dependencies.
  void
  setNeXTCodeOverlayDependencies(ModuleDependencyID moduleID,
                              const ArrayRef<ModuleDependencyID> dependencyIDs);

  StringRef getMainModuleName() const { return mainScanModuleName; }
};
} // namespace nextcode

namespace std {
template <>
struct hash<nextcode::ModuleDependencyID> {
  std::size_t operator()(const nextcode::ModuleDependencyID &id) const {
    return llvm::hash_combine(id.ModuleName, id.Kind);
  }
};
} // namespace std

#endif /* NEXTCODE_AST_MODULE_DEPENDENCIES_H */