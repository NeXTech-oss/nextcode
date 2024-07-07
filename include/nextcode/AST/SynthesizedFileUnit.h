//===--- SynthesizedFileUnit.h - A synthesized file unit --------*- C++ -*-===//
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

#ifndef NEXTCODE_AST_SYNTHESIZEDFILEUNIT_H
#define NEXTCODE_AST_SYNTHESIZEDFILEUNIT_H

#include "nextcode/AST/FileUnit.h"
#include "nextcode/Basic/Debug.h"

namespace nextcode {

/// A container for synthesized declarations, attached to a `SourceFile`.
///
/// Currently, only module-level synthesized declarations are supported.
class SynthesizedFileUnit final : public FileUnit {
  /// The parent source file.
  FileUnit &FU;

  /// Synthesized top level declarations.
  TinyPtrVector<Decl *> TopLevelDecls;

  /// A unique identifier representing this file; used to mark private decls
  /// within the file to keep them from conflicting with other files in the
  /// same module.
  mutable Identifier PrivateDiscriminator;

public:
  SynthesizedFileUnit(FileUnit &FU);
  ~SynthesizedFileUnit() = default;

  /// Returns the parent source file.
  FileUnit &getFileUnit() const { return FU; }

  /// Add a synthesized top-level declaration.
  void addTopLevelDecl(Decl *D) { TopLevelDecls.push_back(D); }

  virtual void lookupValue(DeclName name, NLKind lookupKind,
                           OptionSet<ModuleLookupFlags> Flags,
                           SmallVectorImpl<ValueDecl *> &result) const override;

  void lookupObjCMethods(
      ObjCSelector selector,
      SmallVectorImpl<AbstractFunctionDecl *> &results) const override;

  Identifier getDiscriminatorForPrivateDecl(const Decl *D) const override;

  void getTopLevelDecls(SmallVectorImpl<Decl*> &results) const override;

  ArrayRef<Decl *> getTopLevelDecls() const {
    return TopLevelDecls;
  };

  static bool classof(const FileUnit *file) {
    return file->getKind() == FileUnitKind::Synthesized;
  }
  static bool classof(const DeclContext *DC) {
    return isa<FileUnit>(DC) && classof(cast<FileUnit>(DC));
  }
};

} // namespace nextcode

#endif // NEXTCODE_AST_SYNTHESIZEDFILEUNIT_H
