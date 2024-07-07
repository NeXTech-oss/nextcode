//===- TypeOrExtensionDecl.h - NeXTCode Language Declaration ASTs -*- C++ -*-===//
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
// This file defines the TypeOrExtensionDecl struct, separately to Decl.h so
// that this can be included in files that Decl.h includes.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_TYPE_OR_EXTENSION_DECL_H
#define NEXTCODE_TYPE_OR_EXTENSION_DECL_H

#include "nextcode/AST/TypeAlignments.h"
#include "llvm/ADT/PointerUnion.h"

namespace llvm {
class raw_ostream;
}

namespace nextcode {

class SourceLoc;
class DeclContext;
class IterableDeclContext;

/// Describes either a nominal type declaration or an extension
/// declaration.
struct TypeOrExtensionDecl {
  // (The definitions are in Decl.cpp.)
  llvm::PointerUnion<NominalTypeDecl *, ExtensionDecl *> Decl;

  TypeOrExtensionDecl() = default;

  TypeOrExtensionDecl(NominalTypeDecl *D);
  TypeOrExtensionDecl(ExtensionDecl *D);

  /// Return the contained *Decl as the Decl superclass.
  class Decl *getAsDecl() const;
  /// Return the contained *Decl as the DeclContext superclass.
  DeclContext *getAsDeclContext() const;
  /// Return the contained *Decl as the DeclContext superclass.
  IterableDeclContext *getAsIterableDeclContext() const;
  /// Return the contained NominalTypeDecl or that of the extended type
  /// in the ExtensionDecl.
  NominalTypeDecl *getBaseNominal() const;

  /// Is the contained pointer null?
  bool isNull() const;
  explicit operator bool() const { return !isNull(); }

  friend bool operator==(TypeOrExtensionDecl lhs, TypeOrExtensionDecl rhs) {
    return lhs.Decl == rhs.Decl;
  }
  friend bool operator!=(TypeOrExtensionDecl lhs, TypeOrExtensionDecl rhs) {
    return lhs.Decl != rhs.Decl;
  }
  friend bool operator<(TypeOrExtensionDecl lhs, TypeOrExtensionDecl rhs) {
    return lhs.Decl < rhs.Decl;
  }
  friend llvm::hash_code hash_value(TypeOrExtensionDecl decl) {
    return llvm::hash_value(decl.getAsDecl());
  }
};

void simple_display(llvm::raw_ostream &out, TypeOrExtensionDecl container);
SourceLoc extractNearestSourceLoc(TypeOrExtensionDecl container);

} // end namespace nextcode

#endif
