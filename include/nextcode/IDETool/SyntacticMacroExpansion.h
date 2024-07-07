//===--- SyntacticMacroExpansion.h ----------------------------------------===//
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

#ifndef NEXTCODE_IDE_SYNTACTICMACROEXPANSION_H
#define NEXTCODE_IDE_SYNTACTICMACROEXPANSION_H

#include "nextcode/AST/Decl.h"
#include "nextcode/AST/MacroDefinition.h"
#include "nextcode/AST/PluginRegistry.h"
#include "nextcode/Basic/Fingerprint.h"
#include "nextcode/Frontend/Frontend.h"
#include "llvm/Support/MemoryBuffer.h"

namespace nextcode {

class ASTContext;
class SourceFile;

namespace ide {
class SourceEditConsumer;

/// Simple object to specify a syntactic macro expansion.
struct MacroExpansionSpecifier {
  unsigned offset;
  nextcode::MacroRoles macroRoles;
  nextcode::MacroDefinition macroDefinition;
};

/// Instance of a syntactic macro expansion context. This is created for each
/// list of compiler arguments (i.e. 'argHash'), and reused as long as the
/// compiler arguments are not changed.
class SyntacticMacroExpansionInstance {
  CompilerInvocation invocation;

  SourceManager SourceMgr;
  DiagnosticEngine Diags{SourceMgr};
  std::unique_ptr<ASTContext> Ctx;
  ModuleDecl *TheModule = nullptr;
  llvm::StringMap<MacroDecl *> MacroDecls;

  /// Create 'SourceFile' using the buffer.
  nextcode::SourceFile *getSourceFile(llvm::MemoryBuffer *inputBuf);

  /// Synthesize 'MacroDecl' AST object to use the expansion.
  nextcode::MacroDecl *
  getSynthesizedMacroDecl(nextcode::Identifier name,
                          const MacroExpansionSpecifier &expansion);

  /// Expand single 'expansion' in SF.
  void expand(nextcode::SourceFile *SF,
                    const MacroExpansionSpecifier &expansion,
                    SourceEditConsumer &consumer);

public:
  SyntacticMacroExpansionInstance() {}

  /// Setup the instance with \p args .
  bool setup(StringRef NeXTCodeExecutablePath, ArrayRef<const char *> args,
             std::shared_ptr<PluginRegistry> plugins, std::string &error);

  ASTContext &getASTContext() { return *Ctx; }

  /// Expand all macros in \p inputBuf and send the edit results to \p consumer.
  /// Expansions are specified by \p expansions .
  void expandAll(llvm::MemoryBuffer *inputBuf,
                     ArrayRef<MacroExpansionSpecifier> expansions,
                     SourceEditConsumer &consumer);
};

/// Manager object to vend 'SyntacticMacroExpansionInstance'.
class SyntacticMacroExpansion {
  StringRef NeXTCodeExecutablePath;
  std::shared_ptr<PluginRegistry> Plugins;

public:
  SyntacticMacroExpansion(StringRef NeXTCodeExecutablePath,
                          std::shared_ptr<PluginRegistry> Plugins)
      : NeXTCodeExecutablePath(NeXTCodeExecutablePath), Plugins(Plugins) {}

  /// Get instance configured with the specified compiler arguments.
  std::shared_ptr<SyntacticMacroExpansionInstance>
  getInstance(ArrayRef<const char *> args, std::string &error);
};

} // namespace ide
} // namespace nextcode

#endif // NEXTCODE_IDE_SYNTACTICMACROEXPANSION_H
