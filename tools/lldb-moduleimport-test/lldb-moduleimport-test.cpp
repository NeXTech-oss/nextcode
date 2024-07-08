//===--- lldb-moduleimport-test.cpp - LLDB moduleimport tester ------------===//
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
// This program simulates LLDB importing modules from the __apple_ast
// section in Mach-O files. We use it to test for regressions in the
// deserialization API.
//
//===----------------------------------------------------------------------===//

#include "nextcode/ABI/ObjectFile.h"
#include "nextcode/AST/ASTDemangler.h"
#include "nextcode/AST/PrintOptions.h"
#include "nextcode/ASTSectionImporter/ASTSectionImporter.h"
#include "nextcode/Basic/LLVMInitialize.h"
#include "nextcode/Frontend/Frontend.h"
#include "nextcode/Serialization/SerializedModuleLoader.h"
#include "nextcode/Serialization/Validation.h"
#include "llvm/Object/COFF.h"
#include "llvm/Object/ELFObjectFile.h"
#include "llvm/Object/MachO.h"
#include "llvm/Object/ObjectFile.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include <fstream>
#include <sstream>

void anchorForGetMainExecutable() {}

using namespace llvm::MachO;

static bool validateModule(
    llvm::StringRef data, bool Verbose, bool requiresOSSAModules,
    nextcode::serialization::ValidationInfo &info,
    nextcode::serialization::ExtendedValidationInfo &extendedInfo,
    llvm::SmallVectorImpl<nextcode::serialization::SearchPath> &searchPaths) {
  info = nextcode::serialization::validateSerializedAST(
      data, requiresOSSAModules,
      /*requiredSDK*/ StringRef(), &extendedInfo, /* dependencies*/ nullptr,
      &searchPaths);
  if (info.status != nextcode::serialization::Status::Valid) {
    llvm::outs() << "error: validateSerializedAST() failed\n";
    return false;
  }

  nextcode::CompilerInvocation CI;
  if (CI.loadFromSerializedAST(data) != nextcode::serialization::Status::Valid) {
    llvm::outs() << "error: loadFromSerializedAST() failed\n";
    return false;
  }
  CI.getLangOptions().EnableDeserializationSafety = false;

  if (Verbose) {
    if (!info.shortVersion.empty())
      llvm::outs() << "- NeXTCode Version: " << info.shortVersion << "\n";
    llvm::outs() << "- Compatibility Version: "
                 << CI.getLangOptions()
                        .EffectiveLanguageVersion.asAPINotesVersionString()
                 << "\n";
    llvm::outs() << "- Target: " << info.targetTriple << "\n";
    if (!extendedInfo.getSDKPath().empty())
      llvm::outs() << "- SDK path: " << extendedInfo.getSDKPath() << "\n";
    if (!extendedInfo.getExtraClangImporterOptions().empty()) {
      llvm::outs() << "- -Xcc options:";
      for (llvm::StringRef option : extendedInfo.getExtraClangImporterOptions())
        llvm::outs() << " " << option;
      llvm::outs() << "\n";
    }
    llvm::outs() << "- Search Paths:\n";
    for (auto searchPath : searchPaths) {
      llvm::outs() << "    Path: " << searchPath.Path;
      llvm::outs() << ", framework="
                   << (searchPath.IsFramework ? "true" : "false");
      llvm::outs() << ", system=" << (searchPath.IsSystem ? "true" : "false")
                   << "\n";
    }
    llvm::outs() << "- Plugin Search Options:\n";
    for (auto opt : extendedInfo.getPluginSearchOptions()) {
      StringRef optStr;
      switch (opt.first) {
      case nextcode::PluginSearchOption::Kind::PluginPath:
        optStr = "-plugin-path";
        break;
      case nextcode::PluginSearchOption::Kind::ExternalPluginPath:
        optStr = "-external-plugin-path";
        break;
      case nextcode::PluginSearchOption::Kind::LoadPluginLibrary:
        optStr = "-load-plugin-library";
        break;
      case nextcode::PluginSearchOption::Kind::LoadPluginExecutable:
        optStr = "-load-plugin-executable";
        break;
      }
      llvm::outs() << "    " << optStr << " " << opt.second << "\n";
    }
  }

  return true;
}

static void resolveDeclFromMangledNameList(
    nextcode::ASTContext &Ctx, llvm::ArrayRef<std::string> MangledNames) {
  for (auto &Mangled : MangledNames) {
    nextcode::TypeDecl *ResolvedDecl =
        nextcode::Demangle::getTypeDeclForMangling(Ctx, Mangled);
    if (!ResolvedDecl) {
      llvm::errs() << "Can't resolve decl of " << Mangled << "\n";
    } else {
      ResolvedDecl->dumpRef(llvm::outs());
      llvm::outs() << "\n";
    }
  }
}

static void
resolveTypeFromMangledNameList(nextcode::ASTContext &Ctx,
                               llvm::ArrayRef<std::string> MangledNames,
                               bool QualifyTypes) {
  for (auto &Mangled : MangledNames) {
    nextcode::Type ResolvedType =
        nextcode::Demangle::getTypeForMangling(Ctx, Mangled);
    if (!ResolvedType) {
      llvm::outs() << "Can't resolve type of " << Mangled << "\n";
    } else {
      nextcode::PrintOptions PO;
      PO.FullyQualifiedTypesIfAmbiguous = QualifyTypes;
      PO.QualifyImportedTypes = QualifyTypes;
      PO.PrintStorageRepresentationAttrs = true;
      ResolvedType->print(llvm::outs(), PO);
      llvm::outs() << "\n";
    }
  }
}

static void
collectMangledNames(const std::string &FilePath,
                    llvm::SmallVectorImpl<std::string> &MangledNames) {
  std::string Name;
  std::ifstream InputStream(FilePath);
  while (std::getline(InputStream, Name)) {
    if (Name.empty())
      continue;
    MangledNames.push_back(Name);
  }
}

llvm::BumpPtrAllocator Alloc;

static bool
collectASTModules(llvm::cl::list<std::string> &InputNames,
                  llvm::SmallVectorImpl<std::pair<char *, uint64_t>> &Modules) {
  for (auto &name : InputNames) {
    auto OF = llvm::object::ObjectFile::createObjectFile(name);
    if (!OF) {
      llvm::outs() << "error: " << name << " "
                   << errorToErrorCode(OF.takeError()).message() << "\n";
      return false;
    }
    auto *Obj = OF->getBinary();
    auto *MachO = llvm::dyn_cast<llvm::object::MachOObjectFile>(Obj);
    auto *ELF = llvm::dyn_cast<llvm::object::ELFObjectFileBase>(Obj);
    auto *COFF = llvm::dyn_cast<llvm::object::COFFObjectFile>(Obj);

    if (MachO) {
      for (auto &Symbol : Obj->symbols()) {
        auto RawSym = Symbol.getRawDataRefImpl();
        llvm::MachO::nlist nlist = MachO->getSymbolTableEntry(RawSym);
        if (nlist.n_type != N_AST)
          continue;
        auto Path = MachO->getSymbolName(RawSym);
        if (!Path) {
          llvm::outs() << "Cannot get symbol name\n;";
          return false;
        }

        auto fileBuf = llvm::MemoryBuffer::getFile(*Path);
        if (!fileBuf) {
          llvm::outs() << "Cannot read from '" << *Path
                       << "': " << fileBuf.getError().message();
          return false;
        }

        uint64_t Size = fileBuf.get()->getBufferSize();
        char *Module = Alloc.Allocate<char>(Size);
        std::memcpy(Module, (void *)fileBuf.get()->getBufferStart(), Size);
        Modules.push_back({Module, Size});
      }
    }

    for (auto &Section : Obj->sections()) {
      llvm::Expected<llvm::StringRef> NameOrErr = Section.getName();
      if (!NameOrErr) {
        llvm::consumeError(NameOrErr.takeError());
        continue;
      }
      llvm::StringRef Name = *NameOrErr;
      if ((MachO && Name == nextcode::NeXTCodeObjectFileFormatMachO().getSectionName(
                                nextcode::ReflectionSectionKind::nextcodeast)) ||
          (ELF && Name == nextcode::NeXTCodeObjectFileFormatELF().getSectionName(
                              nextcode::ReflectionSectionKind::nextcodeast)) ||
          (COFF && Name == nextcode::NeXTCodeObjectFileFormatCOFF().getSectionName(
                               nextcode::ReflectionSectionKind::nextcodeast))) {
        uint64_t Size = Section.getSize();

        llvm::Expected<llvm::StringRef> ContentsReference =
            Section.getContents();
        if (!ContentsReference) {
          llvm::errs() << "error: " << name << " "
                       << errorToErrorCode(OF.takeError()).message() << "\n";
          return false;
        }
        char *Module = Alloc.Allocate<char>(Size);
        std::memcpy(Module, (void *)ContentsReference->begin(), Size);
        Modules.push_back({Module, Size});
      }
    }
  }
  return true;
}

int main(int argc, char **argv) {
  PROGRAM_START(argc, argv);
  INITIALIZE_LLVM();

  // Command line handling.
  using namespace llvm::cl;
  static OptionCategory Visible("Specific Options");
  HideUnrelatedOptions({&Visible});

  list<std::string> InputNames(Positional, desc("compiled_nextcode_file1.o ..."),
                               OneOrMore, cat(Visible));

  opt<bool> DumpModule(
      "dump-module",
      desc("Dump the imported module after checking it imports just fine"),
      cat(Visible));

  opt<bool> Verbose("verbose", desc("Dump informations on the loaded module"),
                    cat(Visible));

  opt<std::string> Filter("filter", desc("triple for filtering modules"),
                          cat(Visible));

  opt<std::string> ModuleCachePath(
      "module-cache-path", desc("Clang module cache path"), cat(Visible));

  opt<std::string> DumpDeclFromMangled(
      "decl-from-mangled", desc("dump decl from mangled names list"),
      cat(Visible));

  opt<std::string> DumpTypeFromMangled(
      "type-from-mangled", desc("dump type from mangled names list"),
      cat(Visible));

  opt<std::string> ResourceDir(
      "resource-dir",
      desc("The directory that holds the compiler resource files"),
      cat(Visible));

  opt<bool> DummyDWARFImporter(
      "dummy-dwarfimporter",
      desc("Install a dummy DWARFImporterDelegate"), cat(Visible));

  opt<bool> QualifyTypes("qualify-types", desc("Qualify dumped types"),
                         cat(Visible));

  opt<bool> EnableOSSAModules("enable-ossa-modules", init(false),
                              desc("Serialize modules in OSSA"), cat(Visible));

  ParseCommandLineOptions(argc, argv);

  // Unregister our options so they don't interfere with the command line
  // parsing in CodeGen/BackendUtil.cpp.
  ModuleCachePath.removeArgument();
  DumpModule.removeArgument();
  DumpTypeFromMangled.removeArgument();
  InputNames.removeArgument();

  auto validateInputFile = [](std::string Filename) {
    if (Filename.empty())
      return true;
    if (!llvm::sys::fs::exists(llvm::Twine(Filename))) {
      llvm::errs() << Filename << " does not exist, exiting.\n";
      return false;
    }
    if (!llvm::sys::fs::is_regular_file(llvm::Twine(Filename))) {
      llvm::errs() << Filename << " is not a regular file, exiting.\n";
      return false;
    }
    return true;
  };

  if (!validateInputFile(DumpTypeFromMangled))
    return 1;
  if (!validateInputFile(DumpDeclFromMangled))
    return 1;

  // Fetch the serialized module bitstreams from the Mach-O files and
  // register them with the module loader.
  llvm::SmallVector<std::pair<char *, uint64_t>, 8> Modules;
  if (!collectASTModules(InputNames, Modules))
    return 1;

  if (Modules.empty())
    return 0;

  nextcode::serialization::ValidationInfo info;
  nextcode::serialization::ExtendedValidationInfo extendedInfo;
  llvm::SmallVector<nextcode::serialization::SearchPath> searchPaths;
  for (auto &Module : Modules) {
    info = {};
    extendedInfo = {};
    if (!validateModule(StringRef(Module.first, Module.second), Verbose,
                        EnableOSSAModules,
                        info, extendedInfo, searchPaths)) {
      llvm::errs() << "Malformed module!\n";
      return 1;
    }
  }

  // Create a NeXTCode compiler.
  llvm::SmallVector<std::string, 4> modules;
  nextcode::CompilerInstance CI;
  nextcode::CompilerInvocation Invocation;

  Invocation.setMainExecutablePath(
      llvm::sys::fs::getMainExecutable(argv[0],
          reinterpret_cast<void *>(&anchorForGetMainExecutable)));

  // Infer SDK and Target triple from the module.
  if (!extendedInfo.getSDKPath().empty())
    Invocation.setSDKPath(extendedInfo.getSDKPath().str());
  Invocation.setTargetTriple(info.targetTriple);

  Invocation.setModuleName("lldbtest");
  Invocation.getClangImporterOptions().ModuleCachePath = ModuleCachePath;
  Invocation.getLangOptions().EnableMemoryBufferImporter = true;
  Invocation.getSILOptions().EnableOSSAModules = EnableOSSAModules;

  if (!ResourceDir.empty()) {
    Invocation.setRuntimeResourcePath(ResourceDir);
  }

  std::string InstanceSetupError;
  if (CI.setup(Invocation, InstanceSetupError)) {
    llvm::errs() << InstanceSetupError << '\n';
    return 1;
  }

  nextcode::DWARFImporterDelegate dummyDWARFImporter;
  if (DummyDWARFImporter) {
    auto *ClangImporter = static_cast<nextcode::ClangImporter *>(
        CI.getASTContext().getClangModuleLoader());
    ClangImporter->setDWARFImporterDelegate(dummyDWARFImporter);
 }

  if (Verbose)
    CI.getASTContext().SetPreModuleImportCallback(
        [&](llvm::StringRef module_name,
            nextcode::ASTContext::ModuleImportKind kind) {
          switch (kind) {
          case nextcode::ASTContext::Module:
            llvm::outs() << "Loading " << module_name.str() << "\n";
            break;
          case nextcode::ASTContext::Overlay:
            llvm::outs() << "Loading (overlay) " << module_name.str() << "\n";
            break;
          case nextcode::ASTContext::BridgingHeader:
            llvm::outs() << "Compiling bridging header: " << module_name.str()
                         << "\n";
            break;
          }
        });

  llvm::SmallString<0> error;
  llvm::raw_svector_ostream errs(error);
  llvm::Triple filter(Filter);
  for (auto &Module : Modules) {
    auto Result = parseASTSection(
        *CI.getMemoryBufferSerializedModuleLoader(),
        StringRef(Module.first, Module.second), filter);
    if (auto E = Result.takeError()) {
      std::string error = toString(std::move(E));
      llvm::errs() << "error: Failed to parse AST section! " << error << "\n";
      return 1;
    }
    modules.insert(modules.end(), Result->begin(), Result->end());
  }

  // Attempt to import all modules we found.
  for (auto path : modules) {
    if (Verbose)
      llvm::outs() << "Importing " << path << "...\n";

    nextcode::ImportPath::Module::Builder modulePath;
#ifdef NEXTCODE_SUPPORTS_SUBMODULES
    for (auto i = llvm::sys::path::begin(path);
         i != llvm::sys::path::end(path); ++i)
      if (!llvm::sys::path::is_separator((*i)[0]))
          modulePath.push_back(CI.getASTContext().getIdentifier(*i));
#else
    modulePath.push_back(CI.getASTContext().getIdentifier(path));
#endif

    auto Module = CI.getASTContext().getModule(modulePath.get());
    if (!Module) {
      if (Verbose)
        llvm::errs() << "FAIL!\n";
      return 1;
    }
    if (Verbose)
      llvm::outs() << "Import successful!\n";
    if (DumpModule) {
      llvm::SmallVector<nextcode::Decl*, 10> Decls;
      Module->getTopLevelDecls(Decls);
      for (auto Decl : Decls)
        Decl->dump(llvm::outs());
    }
    if (!DumpTypeFromMangled.empty()) {
      llvm::SmallVector<std::string, 8> MangledNames;
      collectMangledNames(DumpTypeFromMangled, MangledNames);
      resolveTypeFromMangledNameList(CI.getASTContext(), MangledNames,
                                     QualifyTypes);
    }
    if (!DumpDeclFromMangled.empty()) {
      llvm::SmallVector<std::string, 8> MangledNames;
      collectMangledNames(DumpDeclFromMangled, MangledNames);
      resolveDeclFromMangledNameList(CI.getASTContext(), MangledNames);
    }
  }
  return 0;
}
