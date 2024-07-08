//===--- PrettyStackTrace.cpp - Defines SIL crash prettifiers -------------===//
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
//             Tural Ghuliev (turalquliyev@it-gss.com)
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/QuotedString.h"
#include "nextcode/AST/PrettyStackTrace.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/Expr.h"
#include "nextcode/AST/Pattern.h"
#include "nextcode/AST/Stmt.h"
#include "nextcode/SIL/PrettyStackTrace.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILModule.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

using namespace nextcode;

llvm::cl::opt<bool>
SILPrintOnError("sil-print-on-error", llvm::cl::init(false),
                llvm::cl::desc("Printing SIL function bodies in crash diagnostics."));

llvm::cl::opt<bool> SILPrintModuleOnError(
    "sil-print-module-on-error", llvm::cl::init(false),
    llvm::cl::desc("Printing SIL module in crash diagnostics."));

static void printLocationDescription(llvm::raw_ostream &out,
                                         SILLocation::FilenameAndLocation loc,
                                         ASTContext &Context) {
  out << "<<debugloc at " << QuotedString(loc.filename)
      << ":" << loc.line << ":" << loc.column << ">>";
}

void nextcode::printSILLocationDescription(llvm::raw_ostream &out,
                                        SILLocation loc,
                                        ASTContext &Context) {
  if (loc.isASTNode()) {
    if (auto decl = loc.getAsASTNode<Decl>()) {
      printDeclDescription(out, decl);
    } else if (auto expr = loc.getAsASTNode<Expr>()) {
      printExprDescription(out, expr, Context);
    } else if (auto stmt = loc.getAsASTNode<Stmt>()) {
      printStmtDescription(out, stmt, Context);
    } else if (auto pattern = loc.castToASTNode<Pattern>()) {
      printPatternDescription(out, pattern, Context);
    } else {
      out << "<<unknown AST node>>";
    }
    return;
  }
  if (loc.isFilenameAndLocation()) {
    printLocationDescription(out, *loc.getFilenameAndLocation(), Context);
    return;
  }
  if (loc.isSILFile()) {
    printSourceLocDescription(out, loc.getSourceLoc(), Context);
    return;
  }
  out << "<<invalid location>>";
  return;
}

void PrettyStackTraceSILLocation::print(llvm::raw_ostream &out) const {
  out << "While " << Action << " at ";
  printSILLocationDescription(out, Loc, Context);
}

void PrettyStackTraceSILFunction::print(llvm::raw_ostream &out) const {
  out << "While " << action << " SIL function ";
  if (!func) {
    out << " <<null>>";
    return;
  }

  printFunctionInfo(out);
}

void PrettyStackTraceSILFunction::printFunctionInfo(llvm::raw_ostream &out) const {  
  out << "\"";
  func->printName(out);
  out << "\".\n";

  if (!func->getLocation().isNull()) {
    out << " for ";
    printSILLocationDescription(out, func->getLocation(),
                                func->getModule().getASTContext());
  }
  if (SILPrintOnError)
    func->print(out);
  if (SILPrintModuleOnError)
    func->getModule().print(out);
}

void PrettyStackTraceSILNode::print(llvm::raw_ostream &out) const {
  out << "While " << Action << " SIL node ";
  if (Node)
    out << *Node;
}
