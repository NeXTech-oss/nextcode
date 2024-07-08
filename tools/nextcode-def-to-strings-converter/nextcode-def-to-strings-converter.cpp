//===--- nextcode-def-to-strings-converter.cpp -------------------------------===//
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
// Create a .strings file from the diagnostic messages text in `.def` files.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/LLVMInitialize.h"
#include "nextcode/Basic/Compiler.h"
#include "nextcode/Localization/LocalizationFormat.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/raw_ostream.h"
#include <cstdlib>
#include <string>
#include <system_error>

static constexpr const char *const diagnosticID[] = {
#define DIAG(KIND, ID, Options, Text, Signature) #ID,
#include "nextcode/AST/DiagnosticsAll.def"
};

static constexpr const char *const diagnosticMessages[] = {
#define DIAG(KIND, ID, Options, Text, Signature) Text,
#include "nextcode/AST/DiagnosticsAll.def"
};

enum LocalDiagID : uint32_t {
#define DIAG(KIND, ID, Options, Text, Signature) ID,
#include "nextcode/AST/DiagnosticsAll.def"
  NumDiags
};

namespace options {

static llvm::cl::OptionCategory
    Category("nextcode-def-to-strings-converter Options");

static llvm::cl::opt<std::string>
    OutputDirectory("output-directory",
                    llvm::cl::desc("Directory for the output file"),
                    llvm::cl::cat(Category));

static llvm::cl::opt<std::string>
    OutputFilename("output-filename",
                   llvm::cl::desc("Filename for the output file"),
                   llvm::cl::cat(Category));

} // namespace options

int main(int argc, char *argv[]) {
  PROGRAM_START(argc, argv);

  llvm::cl::HideUnrelatedOptions(options::Category);
  llvm::cl::ParseCommandLineOptions(argc, argv,
                                    "NeXTCode `.def` to `.strings` Converter\n");

  llvm::SmallString<128> LocalizedFilePath;
  if (options::OutputFilename.empty()) {
    // The default language for localization is English
    std::string defaultLocaleCode = "en";
    LocalizedFilePath = options::OutputDirectory;
    llvm::sys::path::append(LocalizedFilePath, defaultLocaleCode);
    llvm::sys::path::replace_extension(LocalizedFilePath, ".strings");
  } else {
    LocalizedFilePath = options::OutputFilename;
  }

  std::error_code error;
  llvm::raw_fd_ostream OS(LocalizedFilePath.str(), error,
                          llvm::sys::fs::OF_None);

  if (OS.has_error() || error) {
    llvm::errs() << "Error has occurred while trying to write to "
                 << LocalizedFilePath.str()
                 << " with error code: " << error.message() << "\n";
    return EXIT_FAILURE;
  }

  llvm::ArrayRef<const char *> ids(diagnosticID, LocalDiagID::NumDiags);
  llvm::ArrayRef<const char *> messages(diagnosticMessages,
                                        LocalDiagID::NumDiags);

  nextcode::diag::DefToStringsConverter converter(ids, messages);
  converter.convert(OS);

  return EXIT_SUCCESS;
}
