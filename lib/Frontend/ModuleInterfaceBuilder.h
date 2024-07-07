//===----- ModuleInterfaceBuilder.h - Compiles .codeinterface files ------===//
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

#ifndef NEXTCODE_FRONTEND_MODULEINTERFACEBUILDER_H
#define NEXTCODE_FRONTEND_MODULEINTERFACEBUILDER_H

#include "nextcode/AST/ModuleLoader.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/Basic/SourceLoc.h"
#include "nextcode/Frontend/Frontend.h"
#include "nextcode/Serialization/SerializationOptions.h"
#include "llvm/Support/StringSaver.h"

namespace llvm {
namespace vfs {
class FileSystem;
}
} // namespace llvm

namespace nextcode {

class DiagnosticEngine;
class LangOptions;
class SearchPathOptions;
class DependencyTracker;

/// A utility class to build a NeXTCode interface file into a module
/// using a `CompilerInstance` constructed from the flags specified in the
/// interface file.
class ImplicitModuleInterfaceBuilder {
  SourceManager &sourceMgr;
  DiagnosticEngine *diags;
  InterfaceSubContextDelegate &subASTDelegate;
  const StringRef interfacePath;
  const StringRef sdkPath;
  const StringRef moduleName;
  const StringRef moduleCachePath;
  const StringRef prebuiltCachePath;
  const StringRef backupInterfaceDir;
  const StringRef ABIDescriptorPath;
  const bool disableInterfaceFileLock;
  const bool silenceInterfaceDiagnostics;
  const SourceLoc diagnosticLoc;
  DependencyTracker *const dependencyTracker;
  SmallVector<StringRef, 3> extraDependencies;

public:
  /// Emit a diagnostic tied to this declaration.
  template <typename... ArgTypes>
  static InFlightDiagnostic
  diagnose(DiagnosticEngine *Diags, SourceManager &SM, StringRef InterfacePath,
           SourceLoc Loc, Diag<ArgTypes...> ID,
           typename detail::PassArgument<ArgTypes>::type... Args) {
    if (Loc.isInvalid()) {
      // Diagnose this inside the interface file, if possible.
      Loc = SM.getLocFromExternalSource(InterfacePath, 1, 1);
    }
    return Diags->diagnose(Loc, ID, std::move(Args)...);
  }

private:
  /// Emit a diagnostic tied to this declaration.
  template <typename... ArgTypes>
  InFlightDiagnostic
  diagnose(Diag<ArgTypes...> ID,
           typename detail::PassArgument<ArgTypes>::type... Args) const {
    return diagnose(diags, sourceMgr, interfacePath, diagnosticLoc, ID,
                    std::move(Args)...);
  }

  bool
  buildNeXTCodeModuleInternal(StringRef OutPath, bool ShouldSerializeDeps,
                           std::unique_ptr<llvm::MemoryBuffer> *ModuleBuffer,
                           ArrayRef<std::string> CandidateModules);

public:
  ImplicitModuleInterfaceBuilder(
      SourceManager &sourceMgr, DiagnosticEngine *diags,
      InterfaceSubContextDelegate &subASTDelegate,
      StringRef interfacePath, StringRef sdkPath,
      StringRef moduleName, StringRef moduleCachePath,
      StringRef backupInterfaceDir, StringRef prebuiltCachePath,
      StringRef ABIDescriptorPath, bool disableInterfaceFileLock = false,
      bool silenceInterfaceDiagnostics = false,
      SourceLoc diagnosticLoc = SourceLoc(),
      DependencyTracker *tracker = nullptr)
      : sourceMgr(sourceMgr), diags(diags), subASTDelegate(subASTDelegate),
        interfacePath(interfacePath), sdkPath(sdkPath), moduleName(moduleName),
        moduleCachePath(moduleCachePath), prebuiltCachePath(prebuiltCachePath),
        backupInterfaceDir(backupInterfaceDir),
        ABIDescriptorPath(ABIDescriptorPath),
        disableInterfaceFileLock(disableInterfaceFileLock),
        silenceInterfaceDiagnostics(silenceInterfaceDiagnostics),
        diagnosticLoc(diagnosticLoc), dependencyTracker(tracker) {}

  /// Ensures the requested file name is added as a dependency of the resulting
  /// module.
  void addExtraDependency(StringRef path) { extraDependencies.push_back(path); }

  bool buildNeXTCodeModule(StringRef OutPath, bool ShouldSerializeDeps,
                        std::unique_ptr<llvm::MemoryBuffer> *ModuleBuffer,
                        llvm::function_ref<void()> RemarkRebuild = nullptr,
                        ArrayRef<std::string> CandidateModules = {});
};

/// Use the provided `CompilerInstance` to build a nextcode interface into a module
class ExplicitModuleInterfaceBuilder {
public:
  ExplicitModuleInterfaceBuilder(CompilerInstance &Instance,
                                 DiagnosticEngine *diags,
                                 SourceManager &sourceMgr,
                                 const StringRef moduleCachePath,
                                 const StringRef backupInterfaceDir,
                                 const StringRef prebuiltCachePath,
                                 const StringRef ABIDescriptorPath,
                                 const SmallVector<StringRef, 3> &extraDependencies,
                                 SourceLoc diagnosticLoc = SourceLoc(),
                                 DependencyTracker *tracker = nullptr)
      : Instance(Instance),
        diags(diags),
        sourceMgr(sourceMgr),
        moduleCachePath(moduleCachePath),
        prebuiltCachePath(prebuiltCachePath),
        backupInterfaceDir(backupInterfaceDir),
        ABIDescriptorPath(ABIDescriptorPath),
        extraDependencies(extraDependencies),
        diagnosticLoc(diagnosticLoc),
        dependencyTracker(tracker) {}

  std::error_code buildNeXTCodeModuleFromInterface(
      StringRef interfacePath, StringRef outputPath, bool ShouldSerializeDeps,
      std::unique_ptr<llvm::MemoryBuffer> *ModuleBuffer,
      ArrayRef<std::string> CompiledCandidates,
      StringRef CompilerVersion);

  /// Populate the provided \p Deps with \c FileDependency entries for all
  /// dependencies \p SubInstance's DependencyTracker recorded while compiling
  /// the module, excepting .codemodules in \p moduleCachePath or
  /// \p prebuiltCachePath. Those have _their_ dependencies added instead, both
  /// to avoid having to do recursive scanning when rechecking this dependency
  /// in future and to make the module caches relocatable.
  bool collectDepsForSerialization(
      SmallVectorImpl<SerializationOptions::FileDependency> &Deps,
      StringRef interfacePath, bool IsHashBased);

  /// Emit a diagnostic tied to this declaration.
  template <typename... ArgTypes>
  static InFlightDiagnostic
  diagnose(DiagnosticEngine *Diags, SourceManager &SM, StringRef InterfacePath,
           SourceLoc Loc, Diag<ArgTypes...> ID,
           typename detail::PassArgument<ArgTypes>::type... Args) {
    if (Loc.isInvalid()) {
      // Diagnose this inside the interface file, if possible.
      Loc = SM.getLocFromExternalSource(InterfacePath, 1, 1);
    }
    return Diags->diagnose(Loc, ID, std::move(Args)...);
  }

private:
  /// Emit a diagnostic tied to this declaration.
  template<typename ...ArgTypes>
  InFlightDiagnostic diagnose(
      Diag<ArgTypes...> ID,
      StringRef InterfacePath,
      typename detail::PassArgument<ArgTypes>::type... Args) const {
    return diagnose(diags, sourceMgr, InterfacePath, diagnosticLoc,
                    ID, std::move(Args)...);
  }

private:
  CompilerInstance &Instance;
  DiagnosticEngine *diags;
  SourceManager &sourceMgr;
  StringRef moduleCachePath;
  StringRef prebuiltCachePath;
  StringRef backupInterfaceDir;
  StringRef ABIDescriptorPath;
  const SmallVector<StringRef, 3> extraDependencies;
  const SourceLoc diagnosticLoc;
  DependencyTracker *const dependencyTracker;
};

} // end namespace nextcode

#endif // defined(NEXTCODE_FRONTEND_MODULEINTERFACEBUILDER_H)
