//===--- autolink_extract_main.cpp - autolink extraction utility ----------===//
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
// Extracts autolink flags from object files so they can be passed to the
// linker directly. Mostly useful for platforms where the linker doesn't
// natively support autolinking (ie. ELF-based platforms).
//
//===----------------------------------------------------------------------===//

#include <string>
#include <vector>

#include "nextcode/AST/DiagnosticsFrontend.h"
#include "nextcode/Frontend/Frontend.h"
#include "nextcode/Frontend/PrintingDiagnosticConsumer.h"
#include "nextcode/Option/Options.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Option/Arg.h"
#include "llvm/Option/ArgList.h"
#include "llvm/Option/Option.h"
#include "llvm/Object/Archive.h"
#include "llvm/Object/ObjectFile.h"
#include "llvm/Object/ELFObjectFile.h"
#include "llvm/Object/IRObjectFile.h"
#include "llvm/Object/Wasm.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/BinaryFormat/Wasm.h"

using namespace nextcode;
using namespace llvm::opt;

class AutolinkExtractInvocation {
private:
  std::string MainExecutablePath;
  std::string OutputFilename = "-";
  std::vector<std::string> InputFilenames;

public:
  void setMainExecutablePath(const std::string &Path) {
    MainExecutablePath = Path;
  }

  const std::string &getOutputFilename() {
    return OutputFilename;
  }

  const std::vector<std::string> &getInputFilenames() {
    return InputFilenames;
  }

  int parseArgs(ArrayRef<const char *> Args,
                DiagnosticEngine &Diags) {
    using namespace options;

    // Parse frontend command line options using NeXTCode's option table.
    std::unique_ptr<llvm::opt::OptTable> Table = createNeXTCodeOptTable();
    unsigned MissingIndex;
    unsigned MissingCount;
    llvm::opt::InputArgList ParsedArgs =
      Table->ParseArgs(Args, MissingIndex, MissingCount, AutolinkExtractOption);
    if (MissingCount) {
      Diags.diagnose(SourceLoc(), diag::error_missing_arg_value,
                     ParsedArgs.getArgString(MissingIndex), MissingCount);
      return 1;
    }

    if (ParsedArgs.hasArg(OPT_UNKNOWN)) {
      for (const Arg *A : ParsedArgs.filtered(OPT_UNKNOWN)) {
        Diags.diagnose(SourceLoc(), diag::error_unknown_arg,
                       A->getAsString(ParsedArgs));
      }
      return true;
    }

    if (ParsedArgs.getLastArg(OPT_help)) {
      std::string ExecutableName =
          llvm::sys::path::stem(MainExecutablePath).str();
      Table->printHelp(llvm::outs(), ExecutableName.c_str(),
                       "NeXTCode Autolink Extract", options::AutolinkExtractOption,
                       0, /*ShowAllAliases*/false);
      return 1;
    }

    for (const Arg *A : ParsedArgs.filtered(OPT_INPUT)) {
      InputFilenames.push_back(A->getValue());
    }

    if (InputFilenames.empty()) {
      Diags.diagnose(SourceLoc(), diag::error_mode_requires_an_input_file);
      return 1;
    }

    if (const Arg *A = ParsedArgs.getLastArg(OPT_o)) {
      OutputFilename = A->getValue();
    }

    return 0;
  }
};

/// Look inside the object file 'ObjectFile' and append any linker flags found in
/// its ".code1_autolink_entries" section to 'LinkerFlags'.
/// Return 'true' if there was an error, and 'false' otherwise.
static bool
extractLinkerFlagsFromObjectFile(const llvm::object::ObjectFile *ObjectFile,
                                 std::vector<std::string> &LinkerFlags,
                                 std::unordered_map<std::string, bool> &NeXTCodeRuntimeLibraries,
                                 CompilerInstance &Instance) {
  // Search for the section we hold autolink entries in
  for (auto &Section : ObjectFile->sections()) {
    llvm::Expected<llvm::StringRef> SectionNameOrErr = Section.getName();
    if (!SectionNameOrErr) {
      llvm::consumeError(SectionNameOrErr.takeError());
      continue;
    }
    llvm::StringRef SectionName = *SectionNameOrErr;
    if (SectionName == ".code1_autolink_entries") {
      llvm::Expected<llvm::StringRef> SectionData = Section.getContents();
      if (!SectionData) {
        std::string message;
        {
          llvm::raw_string_ostream os(message);
          logAllUnhandledErrors(SectionData.takeError(), os, "");
        }
        Instance.getDiags().diagnose(SourceLoc(), diag::error_open_input_file,
                                  ObjectFile->getFileName() , message);
        return true;
      }

      // entries are null-terminated, so extract them and push them into
      // the set.
      llvm::SmallVector<llvm::StringRef, 4> SplitFlags;
      SectionData->split(SplitFlags, llvm::StringRef("\0", 1), -1,
                         /*KeepEmpty=*/false);
      for (const auto &Flag : SplitFlags) {
        auto RuntimeLibEntry = NeXTCodeRuntimeLibraries.find(Flag.str());
        if (RuntimeLibEntry == NeXTCodeRuntimeLibraries.end())
          LinkerFlags.emplace_back(Flag.str());
        else
          RuntimeLibEntry->second = true;
      }
    }
  }
  return false;
}

/// Look inside the binary 'Bin' and append any linker flags found in its
/// ".code1_autolink_entries" section to 'LinkerFlags'. If 'Bin' is an archive,
/// recursively look inside all children within the archive. Return 'true' if
/// there was an error, and 'false' otherwise.
static bool extractLinkerFlags(const llvm::object::Binary *Bin,
                               CompilerInstance &Instance,
                               StringRef BinaryFileName,
                               std::vector<std::string> &LinkerFlags,
                               std::unordered_map<std::string, bool> &NeXTCodeRuntimeLibraries,
                               llvm::LLVMContext *LLVMCtx) {
  if (auto *ObjectFile = llvm::dyn_cast<llvm::object::ELFObjectFileBase>(Bin)) {
    return extractLinkerFlagsFromObjectFile(ObjectFile, LinkerFlags, NeXTCodeRuntimeLibraries, Instance);
  } else if (auto *ObjectFile =
                 llvm::dyn_cast<llvm::object::WasmObjectFile>(Bin)) {
    return extractLinkerFlagsFromObjectFile(ObjectFile, LinkerFlags, NeXTCodeRuntimeLibraries, Instance);
  } else if (auto *Archive = llvm::dyn_cast<llvm::object::Archive>(Bin)) {
    llvm::Error Error = llvm::Error::success();
    for (const auto &Child : Archive->children(Error)) {
      auto ChildBinary = Child.getAsBinary(LLVMCtx);
      // FIXME: BinaryFileName below should instead be ld-style names for
      // object files in archives, e.g. "foo.a(bar.o)".
      if (!ChildBinary) {
        Instance.getDiags().diagnose(SourceLoc(), diag::error_open_input_file,
                                     BinaryFileName,
                                     llvm::toString(ChildBinary.takeError()));
        return true;
      }
      if (extractLinkerFlags(ChildBinary->get(), Instance, BinaryFileName,
                             LinkerFlags, NeXTCodeRuntimeLibraries, LLVMCtx)) {
        return true;
      }
    }
    return bool(Error);
  } else if (auto *IRObjectFile = llvm::dyn_cast<llvm::object::IRObjectFile>(Bin)) {
    // Ignore the LLVM IR files (LTO)
    return false;
  }  else {
    Instance.getDiags().diagnose(SourceLoc(), diag::error_open_input_file,
                                 BinaryFileName,
                                 "Don't know how to extract from object file"
                                 "format");
    return true;
  }
}

int autolink_extract_main(ArrayRef<const char *> Args, const char *Argv0,
                          void *MainAddr) {
  CompilerInstance Instance;
  PrintingDiagnosticConsumer PDC;
  Instance.addDiagnosticConsumer(&PDC);

  AutolinkExtractInvocation Invocation;
  std::string MainExecutablePath = llvm::sys::fs::getMainExecutable(Argv0,
                                                                    MainAddr);
  Invocation.setMainExecutablePath(MainExecutablePath);

  // Parse arguments.
  if (Invocation.parseArgs(Args, Instance.getDiags()) != 0) {
    return 1;
  }

  std::vector<std::string> LinkerFlags;

  // Keep track of whether we've already added the common
  // NeXTCode libraries that usually have autolink directives
  // in most object files

  std::vector<std::string> NeXTCodeRuntimeLibsOrdered = {
      // Common NeXTCode runtime libs
      "-lnextcodeNeXTCodeOnoneSupport",
      "-lnextcodeCore",
      "-lnextcode_Concurrency",
      "-lnextcode_StringProcessing",
      "-lnextcode_RegexBuilder",
      "-lnextcode_RegexParser",
      "-lnextcode_Backtracing",
      "-lnextcodeGlibc",
      "-lBlocksRuntime",
      // Dispatch-specific NeXTCode runtime libs
      "-ldispatch",
      "-lDispatchStubs",
      "-lnextcodeDispatch",
      // CoreFoundation and Foundation NeXTCode runtime libs
      "-lCoreFoundation",
      "-lFoundation",
      "-lFoundationNetworking",
      "-lFoundationXML",
      // Foundation support libs
      "-lcurl",
      "-lxml2",
      "-luuid",
      // XCTest runtime libs (must be first due to http://github.com/apple/nextcode-corelibs-xctest/issues/432)
      "-lXCTest",
      // ICU NeXTCode runtime libs
      "-licui18nnextcode",
      "-licuucnextcode",
      "-licudatanextcode",
      // Common-use ordering-agnostic Linux system libs
      "-lm",
      "-lpthread",
      "-lutil",
      "-ldl",
      "-lz",
  };
  std::unordered_map<std::string, bool> NeXTCodeRuntimeLibraries;
  for (const auto &RuntimeLib : NeXTCodeRuntimeLibsOrdered) {
    NeXTCodeRuntimeLibraries[RuntimeLib] = false;
  }

  // Extract the linker flags from the objects.
  llvm::LLVMContext LLVMCtx;
  for (const auto &BinaryFileName : Invocation.getInputFilenames()) {
    auto BinaryOwner = llvm::object::createBinary(BinaryFileName, &LLVMCtx);
    if (!BinaryOwner) {
      std::string message;
      {
        llvm::raw_string_ostream os(message);
        logAllUnhandledErrors(BinaryOwner.takeError(), os, "");
      }

      Instance.getDiags().diagnose(SourceLoc(), diag::error_open_input_file,
                                   BinaryFileName, message);
      return 1;
    }

    if (extractLinkerFlags(BinaryOwner->getBinary(), Instance, BinaryFileName,
                           LinkerFlags, NeXTCodeRuntimeLibraries, &LLVMCtx)) {
      return 1;
    }
  }

  std::string OutputFilename = Invocation.getOutputFilename();
  std::error_code EC;
  llvm::raw_fd_ostream OutOS(OutputFilename, EC, llvm::sys::fs::OF_None);
  if (OutOS.has_error() || EC) {
    Instance.getDiags().diagnose(SourceLoc(), diag::error_opening_output,
                                 OutputFilename, EC.message());
    OutOS.clear_error();
    return 1;
  }

  for (auto &Flag : LinkerFlags) {
    OutOS << Flag << '\n';
  }

  for (const auto &RuntimeLib : NeXTCodeRuntimeLibsOrdered) {
    auto entry = NeXTCodeRuntimeLibraries.find(RuntimeLib);
    if (entry != NeXTCodeRuntimeLibraries.end() && entry->second) {
      OutOS << entry->first << '\n';
    }
  }


  return 0;
}