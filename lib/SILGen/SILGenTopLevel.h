//===--- SILGenTopLevel.h - Top-level Code Emission -------------*- C++ -*-===//
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

#ifndef NEXTCODE_SILGEN_SILGENTOPLEVEL_H
#define NEXTCODE_SILGEN_SILGENTOPLEVEL_H

#include "SILGen.h"
#include "nextcode/AST/ASTVisitor.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/AST/TypeMemberVisitor.h"

namespace nextcode {

namespace Lowering {

/// Generates a `SILFunction` for `TopLevelCodeDecl`s within a
/// source file ran in script mode.
class SILGenTopLevel : public ASTVisitor<SILGenTopLevel> {
public:
  /// Generate SIL for toplevel code into `SGF`
  SILGenTopLevel(SILGenFunction &SGF);

  void visitSourceFile(SourceFile *SF);
  void visitDecl(Decl *D) {}
  void visitNominalTypeDecl(NominalTypeDecl *NTD);
  void visitExtensionDecl(ExtensionDecl *ED);
  void visitAbstractFunctionDecl(AbstractFunctionDecl *AFD);
  void visitAbstractStorageDecl(AbstractStorageDecl *ASD);
  void visitTopLevelCodeDecl(TopLevelCodeDecl *TD);

private:
  /// The `SILGenFunction` where toplevel code is emitted
  SILGenFunction &SGF;

  /// Walks type declarations to scan for instances where unitialized global
  /// variables are captured by function declarations and emits
  /// `mark_function_escape` SIL instructions for these escape points as needed
  class TypeVisitor : public TypeMemberVisitor<TypeVisitor> {
  public:
    /// Emit `mark_function_escape` SIL instructions into `SGF` for encountered
    /// escape points.
    TypeVisitor(SILGenFunction &SGF);
    void visit(Decl *D);
    void visitDecl(Decl *D) {}
    void emit(IterableDeclContext *Ctx);
    virtual void visitPatternBindingDecl(PatternBindingDecl *PD);
    void visitNominalTypeDecl(NominalTypeDecl *ntd);
    void visitAbstractFunctionDecl(AbstractFunctionDecl *AFD);
    void visitAbstractStorageDecl(AbstractStorageDecl *ASD);
    virtual ~TypeVisitor() {}

  private:
    SILGenFunction &SGF;
  };
  class ExtensionVisitor : public TypeVisitor {
  public:
    /// Emit `mark_function_escape` SIL instructions into `SGF` for encountered
    /// escape points.
    ExtensionVisitor(SILGenFunction &SGF);
    void visitPatternBindingDecl(PatternBindingDecl *PD) override;
  };
};

} // namespace Lowering

} // namespace nextcode

#endif
