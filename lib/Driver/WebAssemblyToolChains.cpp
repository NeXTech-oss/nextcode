//===---- WebAssemblyToolChains.cpp - Job invocations (WebAssembly-specific) ------===//
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

#include "ToolChains.h"

#include "nextcode/ABI/System.h"
#include "nextcode/AST/DiagnosticsDriver.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/Basic/Platform.h"
#include "nextcode/Basic/Range.h"
#include "nextcode/Config.h"
#include "nextcode/Driver/Compilation.h"
#include "nextcode/Driver/Driver.h"
#include "nextcode/Driver/Job.h"
#include "nextcode/Option/Options.h"
#include "nextcode/Option/SanitizerOptions.h"
#include "clang/Basic/Version.h"
#include "clang/Driver/Util.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/Option/Arg.h"
#include "llvm/Option/ArgList.h"
#include "llvm/ProfileData/InstrProf.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Process.h"
#include "llvm/Support/Program.h"

using namespace nextcode;
using namespace nextcode::driver;
using namespace llvm::opt;

std::string
toolchains::WebAssembly::sanitizerRuntimeLibName(StringRef Sanitizer,
                                                 bool shared) const {
  return (Twine("clang_rt.") + Sanitizer + "-" +
          this->getTriple().getArchName() + ".lib")
      .str();
}

ToolChain::InvocationInfo toolchains::WebAssembly::constructInvocation(
    const AutolinkExtractJobAction &job, const JobContext &context) const {
  assert(context.Output.getPrimaryOutputType() == file_types::TY_AutolinkFile);

  InvocationInfo II{"nextcode-autolink-extract"};
  ArgStringList &Arguments = II.Arguments;
  II.allowsResponseFiles = true;

  addPrimaryInputsOfType(Arguments, context.Inputs, context.Args,
                         file_types::TY_Object);
  addInputsOfType(Arguments, context.InputActions, file_types::TY_Object);

  Arguments.push_back("-o");
  Arguments.push_back(
      context.Args.MakeArgString(context.Output.getPrimaryOutputFilename()));

  return II;
}

ToolChain::InvocationInfo
toolchains::WebAssembly::constructInvocation(const DynamicLinkJobAction &job,
                                             const JobContext &context) const {
  assert(context.Output.getPrimaryOutputType() == file_types::TY_Image &&
         "Invalid linker output type.");

  ArgStringList Arguments;

  std::string Target = getTriple().str();
  if (!Target.empty()) {
    Arguments.push_back("-target");
    Arguments.push_back(context.Args.MakeArgString(Target));
  }

  switch (job.getKind()) {
  case LinkKind::None:
    llvm_unreachable("invalid link kind");
  case LinkKind::Executable:
    // Default case, nothing extra needed.
    break;
  case LinkKind::DynamicLibrary:
    llvm_unreachable("WebAssembly doesn't support dynamic library yet");
  case LinkKind::StaticLibrary:
    llvm_unreachable("invalid link kind");
  }

  // Select the linker to use.
  std::string Linker;
  if (const Arg *A = context.Args.getLastArg(options::OPT_use_ld)) {
    Linker = A->getValue();
  }
  if (!Linker.empty())
    Arguments.push_back(context.Args.MakeArgString("-fuse-ld=" + Linker));

  const char *Clang = "clang";
  if (const Arg *A = context.Args.getLastArg(options::OPT_tools_directory)) {
    StringRef toolchainPath(A->getValue());

    // If there is a clang in the toolchain folder, use that instead.
    if (auto tool = llvm::sys::findProgramByName("clang", {toolchainPath}))
      Clang = context.Args.MakeArgString(tool.get());
  }

  SmallVector<std::string, 4> RuntimeLibPaths;
  getRuntimeLibraryPaths(RuntimeLibPaths, context.Args, context.OI.SDKPath,
                         /*Shared=*/false);

  SmallString<128> SharedResourceDirPath;
  getResourceDirPath(SharedResourceDirPath, context.Args, /*Shared=*/false);

  SmallString<128> nextcodertPath = SharedResourceDirPath;
  llvm::sys::path::append(nextcodertPath,
                          nextcode::getMajorArchitectureName(getTriple()));
  llvm::sys::path::append(nextcodertPath, "nextcodert.o");
  Arguments.push_back(context.Args.MakeArgString(nextcodertPath));

  addPrimaryInputsOfType(Arguments, context.Inputs, context.Args,
                         file_types::TY_Object);
  addInputsOfType(Arguments, context.InputActions, file_types::TY_Object);
  addInputsOfType(Arguments, context.InputActions, file_types::TY_LLVM_BC);

  if (!context.OI.SDKPath.empty()) {
    Arguments.push_back("--sysroot");
    Arguments.push_back(context.Args.MakeArgString(context.OI.SDKPath));
  }

  // Add any autolinking scripts to the arguments
  for (const Job *Cmd : context.Inputs) {
    auto &OutputInfo = Cmd->getOutput();
    if (OutputInfo.getPrimaryOutputType() == file_types::TY_AutolinkFile)
      Arguments.push_back(context.Args.MakeArgString(
          Twine("@") + OutputInfo.getPrimaryOutputFilename()));
  }

  // Add the runtime library link paths.
  for (auto path : RuntimeLibPaths) {
    Arguments.push_back("-L");
    Arguments.push_back(context.Args.MakeArgString(path));
  }
  // Link the standard library. In two paths, we do this using a .lnk file;
  // if we're going that route, we'll set `linkFilePath` to the path to that
  // file.
  SmallString<128> linkFilePath;
  getResourceDirPath(linkFilePath, context.Args, /*Shared=*/false);
  llvm::sys::path::append(linkFilePath, "static-executable-args.lnk");

  auto linkFile = linkFilePath.str();
  if (llvm::sys::fs::is_regular_file(linkFile)) {
    Arguments.push_back(context.Args.MakeArgString(Twine("@") + linkFile));
  } else {
    llvm::report_fatal_error(linkFile + " not found");
  }

  // Delegate to Clang for sanitizers. It will figure out the correct linker
  // options.
  if (job.getKind() == LinkKind::Executable && context.OI.SelectedSanitizers) {
    Arguments.push_back(context.Args.MakeArgString(
        "-fsanitize=" + getSanitizerList(context.OI.SelectedSanitizers)));
  }

  if (context.Args.hasArg(options::OPT_profile_generate)) {
    SmallString<128> LibProfile(SharedResourceDirPath);
    llvm::sys::path::remove_filename(LibProfile); // remove platform name
    llvm::sys::path::append(LibProfile, "clang", "lib");

    llvm::sys::path::append(LibProfile, getTriple().getOSName(),
                            Twine("libclang_rt.profile-") +
                                getTriple().getArchName() + ".a");
    Arguments.push_back(context.Args.MakeArgString(LibProfile));
    Arguments.push_back(context.Args.MakeArgString(
        Twine("-u", llvm::getInstrProfRuntimeHookVarName())));
  }

  // Run clang++ in verbose mode if "-v" is set
  if (context.Args.hasArg(options::OPT_v)) {
    Arguments.push_back("-v");
  }

  // WebAssembly doesn't reserve low addresses But without "extra inhabitants"
  // of the pointer representation, runtime performance and memory footprint are
  // worse. So assume that compiler driver uses wasm-ld and --global-base=1024
  // to reserve low 1KB.
  Arguments.push_back("-Xlinker");
  Arguments.push_back(context.Args.MakeArgString(
      Twine("--global-base=") +
      std::to_string(NEXTCODE_ABI_WASM32_LEAST_VALID_POINTER)));

  // These custom arguments should be right before the object file at the end.
  context.Args.AddAllArgs(Arguments, options::OPT_linker_option_Group);
  context.Args.AddAllArgs(Arguments, options::OPT_Xlinker);
  context.Args.AddAllArgValues(Arguments, options::OPT_Xclang_linker);

  // This should be the last option, for convenience in checking output.
  Arguments.push_back("-o");
  Arguments.push_back(
      context.Args.MakeArgString(context.Output.getPrimaryOutputFilename()));

  InvocationInfo II{Clang, Arguments};
  II.allowsResponseFiles = true;

  return II;
}

void validateLinkerArguments(DiagnosticEngine &diags,
                             ArgStringList linkerArgs) {
  for (auto arg : linkerArgs) {
    if (StringRef(arg).starts_with("--global-base=")) {
      diags.diagnose(SourceLoc(), diag::error_wasm_doesnt_support_global_base);
    }
  }
}
void toolchains::WebAssembly::validateArguments(DiagnosticEngine &diags,
                                                const llvm::opt::ArgList &args,
                                                StringRef defaultTarget) const {
  ArgStringList linkerArgs;
  args.AddAllArgValues(linkerArgs, options::OPT_Xlinker);
  validateLinkerArguments(diags, linkerArgs);
}

ToolChain::InvocationInfo
toolchains::WebAssembly::constructInvocation(const StaticLinkJobAction &job,
                                             const JobContext &context) const {
  assert(context.Output.getPrimaryOutputType() == file_types::TY_Image &&
         "Invalid linker output type.");

  ArgStringList Arguments;

  const char *AR = "llvm-ar";
  Arguments.push_back("crs");

  Arguments.push_back(
      context.Args.MakeArgString(context.Output.getPrimaryOutputFilename()));

  addPrimaryInputsOfType(Arguments, context.Inputs, context.Args,
                         file_types::TY_Object);
  addInputsOfType(Arguments, context.InputActions, file_types::TY_Object);

  InvocationInfo II{AR, Arguments};

  return II;
}
