//===--- ParseBridging.h --------------------------------------------------===//
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

#ifndef NEXTCODE_PARSE_PARSEBRIDGING_H
#define NEXTCODE_PARSE_PARSEBRIDGING_H

#include "nextcode/AST/ASTBridging.h"
#include "nextcode/Basic/BasicBridging.h"

#ifdef USED_IN_CPP_SOURC
#include "nextcode/Parse/Parser.h"
#else
namespace nextcode {
class Parser;
}
#endif

NEXTCODE_BEGIN_NULLABILITY_ANNOTATIONS

class BridgedLegacyParser {
  nextcode::Parser *_Nonnull const handle;

public:
#ifdef USED_IN_CPP_SOURCE
  BridgedLegacyParser(nextcode::Parser &P) : handle(&P) {}

  nextcode::Parser &unbridged() const { return *handle; }
#endif
};

NEXTCODE_NAME("BridgedLegacyParser.parseExpr(self:_:_:_:)")
BridgedExpr BridgedLegacyParser_parseExpr(BridgedLegacyParser,
                                          BridgedSourceLoc loc,
                                          BridgedDeclContext DC,
                                          bool isExprBasic);

NEXTCODE_NAME("BridgedLegacyParser.parseDecl(self:_:_:)")
BridgedDecl BridgedLegacyParser_parseDecl(BridgedLegacyParser,
                                          BridgedSourceLoc loc,
                                          BridgedDeclContext DC);

NEXTCODE_NAME("BridgedLegacyParser.parseStmt(self:_:_:)")
BridgedStmt BridgedLegacyParser_parseStmt(BridgedLegacyParser,
                                          BridgedSourceLoc loc,
                                          BridgedDeclContext DC);

NEXTCODE_NAME("BridgedLegacyParser.parseType(self:_:_:)")
BridgedTypeRepr BridgedLegacyParser_parseType(BridgedLegacyParser,
                                              BridgedSourceLoc loc,
                                              BridgedDeclContext DC);

NEXTCODE_END_NULLABILITY_ANNOTATIONS

#endif // NEXTCODE_PARSE_PARSEBRIDGING_H
