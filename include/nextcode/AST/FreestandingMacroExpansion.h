//===--- FreestandingMacroExpansion.h ------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_AST_FREESTANDING_MACRO_EXPANSION_H
#define NEXTCODE_AST_FREESTANDING_MACRO_EXPANSION_H

#include "nextcode/AST/ASTAllocated.h"
#include "nextcode/AST/ASTNode.h"
#include "nextcode/AST/ConcreteDeclRef.h"
#include "nextcode/AST/DeclNameLoc.h"
#include "nextcode/AST/Identifier.h"
#include "nextcode/Basic/SourceLoc.h"

namespace nextcode {
class MacroExpansionDecl;
class MacroExpansionExpr;
class Expr;
class Decl;
class ArgumentList;

/// Information about a macro expansion that is common between macro
/// expansion declarations and expressions.
///
/// Instances of these types will be shared among paired macro expansion
/// declaration/expression nodes.
struct MacroExpansionInfo : ASTAllocated<MacroExpansionInfo> {
  SourceLoc SigilLoc;
  DeclNameRef ModuleName;
  DeclNameLoc ModuleNameLoc;
  DeclNameRef MacroName;
  DeclNameLoc MacroNameLoc;
  SourceLoc LeftAngleLoc, RightAngleLoc;
  llvm::ArrayRef<TypeRepr *> GenericArgs;
  ArgumentList *ArgList;

  /// The referenced macro.
  ConcreteDeclRef macroRef;

  enum : unsigned { InvalidDiscriminator = 0xFFFF };

  unsigned Discriminator = InvalidDiscriminator;

  MacroExpansionInfo(SourceLoc sigilLoc, DeclNameRef moduleName,
                     DeclNameLoc moduleNameLoc, DeclNameRef macroName,
                     DeclNameLoc macroNameLoc, SourceLoc leftAngleLoc,
                     SourceLoc rightAngleLoc, ArrayRef<TypeRepr *> genericArgs,
                     ArgumentList *argList)
      : SigilLoc(sigilLoc), ModuleName(moduleName),
        ModuleNameLoc(moduleNameLoc), MacroName(macroName),
        MacroNameLoc(macroNameLoc), LeftAngleLoc(leftAngleLoc),
        RightAngleLoc(rightAngleLoc), GenericArgs(genericArgs),
        ArgList(argList) {}

  SourceLoc getLoc() const { return SigilLoc; }
  SourceRange getGenericArgsRange() const {
    return {LeftAngleLoc, RightAngleLoc};
  }
  SourceRange getSourceRange() const;
};

enum class FreestandingMacroKind {
  Expr, // MacroExpansionExpr.
  Decl, // MacroExpansionDecl.
};

/// A base class of either 'MacroExpansionExpr' or 'MacroExpansionDecl'.
class FreestandingMacroExpansion {
  llvm::PointerIntPair<MacroExpansionInfo *, 1, FreestandingMacroKind>
    infoAndKind;

protected:
  FreestandingMacroExpansion(FreestandingMacroKind kind,
                             MacroExpansionInfo *info)
  : infoAndKind(info, kind) {}

public:
  MacroExpansionInfo *getExpansionInfo() const {
    return infoAndKind.getPointer();
  }
  FreestandingMacroKind getFreestandingMacroKind() const {
    return infoAndKind.getInt();
  }

  ASTNode getASTNode();

  SourceLoc getPoundLoc() const { return getExpansionInfo()->SigilLoc; }

  DeclNameLoc getModuleNameLoc() const {
    return getExpansionInfo()->ModuleNameLoc;
  }
  DeclNameRef getModuleName() const { return getExpansionInfo()->ModuleName; }
  DeclNameLoc getMacroNameLoc() const {
    return getExpansionInfo()->MacroNameLoc;
  }
  DeclNameRef getMacroName() const { return getExpansionInfo()->MacroName; }

  ArrayRef<TypeRepr *> getGenericArgs() const {
    return getExpansionInfo()->GenericArgs;
  }
  SourceRange getGenericArgsRange() const {
    return getExpansionInfo()->getGenericArgsRange();
  }

  ArgumentList *getArgs() const { return getExpansionInfo()->ArgList; }
  void setArgs(ArgumentList *args) { getExpansionInfo()->ArgList = args; }

  ConcreteDeclRef getMacroRef() const { return getExpansionInfo()->macroRef; }
  void setMacroRef(ConcreteDeclRef ref) { getExpansionInfo()->macroRef = ref; }

  DeclContext *getDeclContext() const;
  SourceRange getSourceRange() const;

  /// Returns a discriminator which determines this macro expansion's index
  /// in the sequence of macro expansions within the current context.
  unsigned getDiscriminator() const;

  /// Returns the raw discriminator, for debugging purposes only.
  unsigned getRawDiscriminator() const;
};

} // namespace nextcode

#endif // NEXTCODE_AST_FREESTANDING_MACRO_EXPANSION_H
