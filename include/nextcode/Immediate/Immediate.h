//===--- Immediate.h - Entry point for nextcode immediate mode -----*- C++ -*-===//
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
// This is the entry point to the nextcode immediate mode, which takes a
// source file, and runs it immediately using the JIT.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IMMEDIATE_IMMEDIATE_H
#define NEXTCODE_IMMEDIATE_IMMEDIATE_H

#include <memory>
#include <string>
#include <vector>

namespace nextcode {
  class CompilerInstance;
  class IRGenOptions;
  class SILOptions;
  class SILModule;

  // Using LLVM containers to store command-line arguments turns out
  // to be a lose, because LLVM's execution engine demands this vector
  // type.  We can flip the typedef if/when the LLVM interface
  // supports LLVM containers.
  using ProcessCmdLine = std::vector<std::string>;
  

  /// Attempt to run the script identified by the given compiler instance.
  ///
  /// \return the result returned from main(), if execution succeeded
  int RunImmediately(CompilerInstance &CI, const ProcessCmdLine &CmdLine,
                     const IRGenOptions &IRGenOpts, const SILOptions &SILOpts,
                     std::unique_ptr<SILModule> &&SM);

  int RunImmediatelyFromAST(CompilerInstance &CI);

} // end namespace nextcode

#endif // NEXTCODE_IMMEDIATE_IMMEDIATE_H
