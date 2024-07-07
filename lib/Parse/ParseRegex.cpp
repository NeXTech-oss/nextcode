//===--- ParseRegex.cpp - Regular expression literal parsing --------------===//
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
// Regular expression literal parsing
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/DiagnosticsParse.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Bridging/ASTGen.h"
#include "nextcode/Parse/Parser.h"

using namespace nextcode;

ParserResult<Expr> Parser::parseExprRegexLiteral() {
  assert(Tok.is(tok::regex_literal));

#if NEXTCODE_BUILD_REGEX_PARSER_IN_COMPILER
  auto regexText = Tok.getText();

  // Let the NeXTCode library parse the contents, returning an error, or null if
  // successful.
  size_t version = 0;
  auto capturesBuf = Context.AllocateUninitialized<uint8_t>(
      RegexLiteralExpr::getCaptureStructureSerializationAllocationSize(
          regexText.size()));
  bool hadError = nextcode_ASTGen_parseRegexLiteral(
      regexText,
      /*versionOut=*/&version,
      /*captureStructureOut=*/capturesBuf.data(),
      /*captureStructureSize=*/capturesBuf.size(),
      /*diagBaseLoc=*/Tok.getLoc(), &Diags);
  auto loc = consumeToken();
  SourceMgr.recordRegexLiteralStartLoc(loc);

  if (hadError) {
    return makeParserResult(new (Context) ErrorExpr(loc));
  }
  assert(version >= 1);
  return makeParserResult(RegexLiteralExpr::createParsed(
      Context, loc, regexText, version, capturesBuf));
#else
  llvm_unreachable("Lexer should not emit tok::regex_literal");
#endif
}
