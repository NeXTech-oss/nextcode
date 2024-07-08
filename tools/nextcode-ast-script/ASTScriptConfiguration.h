//===--- ASTScriptConfiguration.h - AST script configuration ----*- C++ -*-===//
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
// Types for configuring an AST script invocation.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SCRIPTING_ASTSCRIPTCONFIGURATION_H
#define NEXTCODE_SCRIPTING_ASTSCRIPTCONFIGURATION_H

#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/StringRef.h"

namespace nextcode {
class CompilerInstance;

namespace scripting {

/// A configuration for working with an ASTScript.
class ASTScriptConfiguration {
  ASTScriptConfiguration(CompilerInstance &compiler) : Compiler(compiler) {}
public:
  CompilerInstance &Compiler;
  StringRef ScriptFile;

  /// Attempt to parse this configuration.
  ///
  /// Returns null if there's a problem.
  static std::unique_ptr<ASTScriptConfiguration>
  parse(CompilerInstance &compiler, ArrayRef<const char *> args);
};

}
}

#endif
