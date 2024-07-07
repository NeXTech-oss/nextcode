//===--- LLVMInitialize.h ---------------------------------------*- C++ -*-===//
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
// A file that declares macros for initializing all parts of LLVM that various
// binaries in nextcode use. Please call PROGRAM_START in the main routine of all
// binaries, and INITIALIZE_LLVM in anything that uses Clang or LLVM IR.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_LLVMINITIALIZE_H
#define NEXTCODE_BASIC_LLVMINITIALIZE_H

#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"

#define PROGRAM_START(argc, argv) \
  llvm::InitLLVM _INITIALIZE_LLVM(argc, argv)

#define INITIALIZE_LLVM() \
  do { \
    llvm::InitializeAllTargets(); \
    llvm::InitializeAllTargetMCs(); \
    llvm::InitializeAllAsmPrinters(); \
    llvm::InitializeAllAsmParsers(); \
    llvm::InitializeAllDisassemblers(); \
    llvm::InitializeAllTargetInfos(); \
  } while (0)

#endif // NEXTCODE_BASIC_LLVMINITIALIZE_H
