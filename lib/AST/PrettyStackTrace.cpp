//===--- PrettyStackTrace.cpp - NeXTCode-specific PrettyStackTraceEntries ----===//
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
//
//  This file implements several NeXTCode-specific implementations of
//  PrettyStackTraceEntry.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/Expr.h"
#include "nextcode/AST/GenericSignature.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/Pattern.h"
#include "nextcode/AST/ProtocolConformance.h"
#include "nextcode/AST/Stmt.h"
#include "nextcode/AST/PrettyStackTrace.h"
#include "nextcode/AST/TypeRepr.h"
#include "nextcode/AST/TypeVisitor.h"
#include "nextcode/Basic/SourceManager.h"
#include "clang/AST/Type.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/MemoryBuffer.h"

using namespace nextcode;

void PrettyStackTraceDecl::print(llvm::raw_ostream &out) const {
  out << "While " << Action << ' ';
  printDeclDescription(out, TheDecl);
}

void PrettyStackTraceDeclAndSubst::print(llvm::raw_ostream &out) const {
  out << "While " << action << ' ';
  printDeclDescription(out, decl);

  out << "with substitution map: ";
  subst.dump(out);
}


void nextcode::printDeclDescription(llvm::raw_ostream &out, const Decl *D,
                                 bool addNewline) {
  if (!D) {
    out << "NULL declaration!";
    if (addNewline) out << '\n';
    return;
  }
  SourceLoc loc = D->getStartLoc();
  bool hasPrintedName = false;
  if (auto *named = dyn_cast<ValueDecl>(D)) {
    if (named->hasName()) {
      out << '\'' << named->getName() << '\'';
      hasPrintedName = true;
    } else if (auto *accessor = dyn_cast<AccessorDecl>(named)) {
      auto ASD = accessor->getStorage();
      if (ASD->hasName()) {
        switch (accessor->getAccessorKind()) {
        case AccessorKind::Get:
          out << "getter";
          break;
        case AccessorKind::DistributedGet:
          out << "_distributed_getter";
          break;
        case AccessorKind::Set:
          out << "setter";
          break;
        case AccessorKind::WillSet:
          out << "willset";
          break;
        case AccessorKind::DidSet:
          out << "didset";
          break;
        case AccessorKind::Address:
          out << "addressor";
          break;
        case AccessorKind::MutableAddress:
          out << "mutableAddressor";
          break;
        case AccessorKind::Read:
          out << "read";
          break;
        case AccessorKind::Modify:
          out << "modify";
          break;
        case AccessorKind::Init:
          out << "constructor";
          break;
        }

        out << " for " << ASD->getName();
        hasPrintedName = true;
        loc = ASD->getStartLoc();
      }
    }
  } else if (auto *extension = dyn_cast<ExtensionDecl>(D)) {
    Type extendedTy = extension->getExtendedType();
    if (extendedTy) {
      out << "extension of " << extendedTy;
      hasPrintedName = true;
    }
  }

  if (!hasPrintedName)
    out << "declaration " << (const void *)D;

  if (loc.isValid()) {
    out << " (at ";
    loc.print(out, D->getASTContext().SourceMgr);
    out << ')';
  } else {
    out << " (in module '" << D->getModuleContext()->getName() << "')";
  }
  if (addNewline) out << '\n';
}

void PrettyStackTraceAnyFunctionRef::print(llvm::raw_ostream &out) const {
  out << "While " << Action << ' ';
  if (auto *AFD = TheRef.getAbstractFunctionDecl()) {
    printDeclDescription(out, AFD);
  } else {
    auto *ACE = TheRef.getAbstractClosureExpr();
    printExprDescription(out, ACE, ACE->getASTContext());
  }
}

void PrettyStackTraceFreestandingMacroExpansion::print(
    llvm::raw_ostream &out) const {
  out << "While " << Action << ' ';
  switch (Expansion->getFreestandingMacroKind()) {
  case FreestandingMacroKind::Expr: {
    auto &Context = Expansion->getDeclContext()->getASTContext();
    printExprDescription(out, cast<MacroExpansionExpr>(Expansion), Context);
    break;
  }
  case FreestandingMacroKind::Decl:
    printDeclDescription(out, cast<MacroExpansionDecl>(Expansion));
    break;
  }
}

void PrettyStackTraceExpr::print(llvm::raw_ostream &out) const {
  out << "While " << Action << ' ';
  if (!TheExpr) {
    out << "NULL expression!\n";
    return;
  }
  printExprDescription(out, TheExpr, Context);
}

void nextcode::printExprDescription(llvm::raw_ostream &out, const Expr *E,
                                 const ASTContext &Context, bool addNewline) {
  out << "expression at ";
  E->getSourceRange().print(out, Context.SourceMgr);
  if (addNewline) out << '\n';
}

void PrettyStackTraceStmt::print(llvm::raw_ostream &out) const {
  out << "While " << Action << ' ';
  if (!TheStmt) {
    out << "NULL statement!\n";
    return;
  }
  printStmtDescription(out, TheStmt, Context);
}

void nextcode::printStmtDescription(llvm::raw_ostream &out, Stmt *S,
                                 const ASTContext &Context, bool addNewline) {
  out << "statement at ";
  S->getSourceRange().print(out, Context.SourceMgr);
  if (addNewline) out << '\n';
}

void PrettyStackTracePattern::print(llvm::raw_ostream &out) const {
  out << "While " << Action << ' ';
  if (!ThePattern) {
    out << "NULL pattern!\n";
    return;
  }
  printPatternDescription(out, ThePattern, Context);
}

void nextcode::printPatternDescription(llvm::raw_ostream &out, Pattern *P,
                                    const ASTContext &Context,
                                    bool addNewline) {
  out << "pattern at ";
  P->getSourceRange().print(out, Context.SourceMgr);
  if (addNewline) out << '\n';
}

namespace {
  /// Map a Type to an interesting declaration whose source range we
  /// should print.
  struct InterestingDeclForType
      : TypeVisitor<InterestingDeclForType, Decl*> {
    Decl *visitType(TypeBase *type) {
      return nullptr;
    }
    Decl *visitUnboundGenericType(UnboundGenericType *type) {
      return type->getDecl();
    }
    Decl *visitBoundGenericType(BoundGenericType *type) {
      return type->getDecl();
    }
    Decl *visitNominalType(NominalType *type) {
      return type->getDecl();
    }
    Decl *visitTypeAliasType(TypeAliasType *type) {
      return type->getDecl();
    }
  };
} // end anonymous namespace

void PrettyStackTraceType::print(llvm::raw_ostream &out) const {
  out << "While " << Action << ' ';
  if (TheType.isNull()) {
    out << "NULL type!\n";
    return;
  }
  printTypeDescription(out, TheType, Context);
}

void nextcode::printTypeDescription(llvm::raw_ostream &out, Type type,
                                 const ASTContext &Context, bool addNewline) {
  out << "type '" << type << '\'';
  if (Decl *decl = InterestingDeclForType().visit(type)) {
    if (decl->getSourceRange().isValid()) {
      out << " (declared at ";
      decl->getSourceRange().print(out, Context.SourceMgr);
      out << ')';
    }
  }
  if (addNewline) out << '\n';
}

void PrettyStackTraceClangType::print(llvm::raw_ostream &out) const {
  out << "While " << Action << ' ';
  if (TheType == nullptr) {
    out << "NULL clang type!\n";
    return;
  }
  TheType->dump(out, Context);
}

void PrettyStackTraceTypeRepr::print(llvm::raw_ostream &out) const {
  out << "While " << Action << " type ";
  TheType->print(out);
  if (TheType && TheType->getSourceRange().isValid()) {
    out << " at ";
    TheType->getSourceRange().print(out, Context.SourceMgr);
  }
  out << '\n';
}

void PrettyStackTraceConformance::print(llvm::raw_ostream &out) const {
  out << "While " << Action << ' ';
  auto &Context = Conformance->getDeclContext()->getASTContext();
  printConformanceDescription(out, Conformance, Context);
}

void nextcode::printConformanceDescription(llvm::raw_ostream &out,
                                        const ProtocolConformance *conformance,
                                        const ASTContext &ctxt,
                                        bool addNewline) {
  if (!conformance) {
    out << "NULL protocol conformance!";
    if (addNewline) out << '\n';
    return;
  }

  out << "protocol conformance "
      << conformance->getType() << ": "
      << conformance->getProtocol()->getName() << " at ";
  auto *decl = conformance->getDeclContext()->getInnermostDeclarationDeclContext();
  printDeclDescription(out, decl, addNewline);
}

void nextcode::printSourceLocDescription(llvm::raw_ostream &out,
                                      SourceLoc loc, const ASTContext &ctx,
                                      bool addNewline) {
  loc.print(out, ctx.SourceMgr);
  if (addNewline) out << '\n';
}

void PrettyStackTraceLocation::print(llvm::raw_ostream &out) const {
  out << "While " << Action << " starting at ";
  printSourceLocDescription(out, Loc, Context);
}

void PrettyStackTraceGenericSignature::print(llvm::raw_ostream &out) const {
  out << "While " << Action << " generic signature ";
  GenericSig->print(out);
  if (Requirement) {
    out << " in requirement #" << *Requirement;
  }
  out << '\n';
}

void PrettyStackTraceSelector::print(llvm::raw_ostream &out) const {
  out << "While " << Action << " '" << Selector << "'";
}

void PrettyStackTraceDifferentiabilityWitness::print(
    llvm::raw_ostream &out) const {
  out << "While " << Action << ' ';
  printDifferentiabilityWitnessDescription(out, Key);
}

void nextcode::printDifferentiabilityWitnessDescription(
    llvm::raw_ostream &out, const SILDifferentiabilityWitnessKey key,
    bool addNewline) {
  key.print(out);
  if (addNewline)
    out << '\n';
}

void PrettyStackTraceDeclContext::print(llvm::raw_ostream &out) const {
  out << "While " << Action << " in decl context:\n";
  out << "    ---\n";
  DC->printContext(out, /*indent=*/4);
  out << "    ---\n";
}
