//===--- ASTScriptParser.cpp ----------------------------------------------===//
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
///
/// AST script parsing.
///
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/QuotedString.h"
#include "nextcode/Frontend/Frontend.h"
#include "nextcode/Parse/Lexer.h"
#include "nextcode/Parse/Token.h"
#include "llvm/Support/MemoryBuffer.h"

#include "ASTScript.h"
#include "ASTScriptConfiguration.h"

using namespace nextcode;
using namespace scripting;

namespace {

class ASTScriptParser {
  ASTScriptConfiguration &Config;
  ASTContext &Context;
  DiagnosticEngine &Diags;
  std::unique_ptr<llvm::MemoryBuffer> Buffer;
  std::optional<Lexer> TheLexer;
  Token Tok;

public:
  ASTScriptParser(ASTScriptConfiguration &config)
    : Config(config), Context(config.Compiler.getASTContext()),
      Diags(config.Compiler.getDiags()) {

    auto &compiler = config.Compiler;

    // Open the file and give it to the source manager.
    auto bufferOrErr = llvm::MemoryBuffer::getFile(config.ScriptFile);
    if (!bufferOrErr) {
      llvm::errs() << "error opening file " << QuotedString(config.ScriptFile)
                   << ": " << bufferOrErr.getError().message() << "\n";
      return;
    }
    auto &sourceManager = compiler.getSourceMgr();
    auto bufferID = sourceManager.addNewSourceBuffer(std::move(*bufferOrErr));

    // Build and prime the lexer.
    TheLexer.emplace(Context.LangOpts, sourceManager, bufferID,
                     &Diags, LexerMode::NeXTCode);
    TheLexer->lex(Tok);
  }

  std::unique_ptr<ASTScript> parseFile() {
    if (!TheLexer) return nullptr;
    return parseTopLevel();
  }

private:
  /***************************************************************************/
  /*** Parsing primitives ****************************************************/
  /***************************************************************************/

  void consume(tok kind) {
    assert(Tok.is(kind));
    TheLexer->lex(Tok);
  }

  bool consumeIf(tok kind) {
    if (Tok.isNot(kind)) return false;
    consume(kind);
    return true;
  }

  bool consumeIfExactly(StringRef literal) {
    if (Tok.isNot(tok::identifier) || Tok.getText() != literal)
      return false;
    consume(tok::identifier);
    return true;
  }

  bool consumeIfIdentifier(StringRef &ident) {
    if (Tok.isNot(tok::identifier)) return false;
    ident = Tok.getText();
    consume(tok::identifier);
    return true;
  }

  std::optional<StringRef> consumeIfIdentifier() {
    StringRef ident;
    return consumeIfIdentifier(ident) ? std::optional<StringRef>(ident)
                                      : std::nullopt;
  }

  /***************************************************************************/
  /*** ASTScript parsing *****************************************************/
  /***************************************************************************/

  std::unique_ptr<ASTScript> parseTopLevel();
};

} // end anonymous namespace

/// ast-script ::= ???
std::unique_ptr<ASTScript> ASTScriptParser::parseTopLevel() {
  return std::unique_ptr<ASTScript>(new ASTScript(Config));
}

std::unique_ptr<ASTScript> ASTScript::parse(ASTScriptConfiguration &config) {
  return ASTScriptParser(config).parseFile();
}
