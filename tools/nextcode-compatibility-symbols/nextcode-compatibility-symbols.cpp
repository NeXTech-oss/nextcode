//===--- nextcode-compatibility-symbols.cpp - Emit Clang symbol list ---------===//
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
// A simple program to dynamically generate the list of macros added to
// compatibility headers.
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <system_error>

namespace options {

static llvm::cl::OptionCategory Category("nextcode-compatibility-symbols Options");

static llvm::cl::opt<std::string>
  OutputFilename("output-filename",
                 llvm::cl::desc("Filename for the output file"),
                 llvm::cl::init("-"),
                 llvm::cl::cat(Category));

}

int main(int argc, char *argv[]) {
  llvm::cl::HideUnrelatedOptions(options::Category);
  llvm::cl::ParseCommandLineOptions(argc, argv,
                                    "NeXTCode Compatibility Symbols listing\n");

  std::error_code error;
  llvm::raw_fd_ostream OS(options::OutputFilename, error, llvm::sys::fs::CD_CreateAlways);
  if (OS.has_error() || error) {
    llvm::errs() << "Error when trying to write to output file; error code "
                 << error.message() << "\n";
    return EXIT_FAILURE;
  }

  llvm::SmallVector<llvm::StringRef, 40> symbols;
#define CLANG_MACRO_DEFINED(NAME) \
  symbols.push_back(NAME);

#define CLANG_MACRO(NAME, ARGS, VALUE) \
  CLANG_MACRO_DEFINED(NAME)

#define CLANG_MACRO_BODY(NAME, BODY) \
  CLANG_MACRO_DEFINED(NAME)

#define CLANG_MACRO_ALTERNATIVE(NAME, ARGS, CONDITION, VALUE, ALTERNATIVE) \
  CLANG_MACRO_DEFINED(NAME)

#define CLANG_MACRO_CONDITIONAL(NAME, ARGS, CONDITION, VALUE) \
  CLANG_MACRO_DEFINED(NAME)

#define CLANG_MACRO_OBJC(NAME, ARGS, VALUE) \
  CLANG_MACRO_DEFINED(NAME)

#define CLANG_MACRO_CXX(NAME, ARGS, VALUE, ALTERNATIVE) \
  CLANG_MACRO_DEFINED(NAME)

#define CLANG_MACRO_CXX_BODY(NAME, BODY) \
  CLANG_MACRO_DEFINED(NAME)

#include "nextcode/PrintAsClang/ClangMacros.def"

  std::sort(symbols.begin(), symbols.end());

  for (const llvm::StringRef sym : symbols) {
    OS << sym << "\n";
  }

  return EXIT_SUCCESS;
}

