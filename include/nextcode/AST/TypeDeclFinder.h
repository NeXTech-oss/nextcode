//===--- TypeDeclFinder.h - Finds TypeDecls in Types/TypeReprs --*- C++ -*-===//
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
#ifndef NEXTCODE_AST_TYPEDECLFINDER_H
#define NEXTCODE_AST_TYPEDECLFINDER_H

#include "nextcode/AST/ASTWalker.h"
#include "nextcode/AST/TypeWalker.h"
#include "llvm/ADT/STLExtras.h"

namespace nextcode {

class BoundGenericType;
class DeclRefTypeRepr;
class NominalType;
class TypeAliasType;

/// Walks a Type to find all NominalTypes, BoundGenericTypes, and
/// TypeAliasTypes.
class TypeDeclFinder : public TypeWalker {
  Action walkToTypePre(Type T) override;

public:
  virtual Action visitNominalType(NominalType *ty) {
    return Action::Continue;
  }
  virtual Action visitBoundGenericType(BoundGenericType *ty) {
    return Action::Continue;
  }
  virtual Action visitTypeAliasType(TypeAliasType *ty) {
    return Action::Continue;
  }
};

/// A TypeDeclFinder for use cases where all types should be treated
/// equivalently and where generic arguments can be walked to separately from
/// the generic type.
class SimpleTypeDeclFinder : public TypeDeclFinder {
  /// The function to call when a \c TypeDecl is seen.
  llvm::function_ref<Action(const TypeDecl *)> Callback;

  Action visitNominalType(NominalType *ty) override;
  Action visitBoundGenericType(BoundGenericType *ty) override;
  Action visitTypeAliasType(TypeAliasType *ty) override;

public:
  explicit SimpleTypeDeclFinder(
      llvm::function_ref<Action(const TypeDecl *)> callback)
    : Callback(callback) {}
};

/// Walks a `TypeRepr` and reports all `DeclRefTypeRepr` nodes with bound
/// type declarations by invoking a given callback. These nodes are reported in
/// depth- and base-first AST order. For example, nodes in `A<T>.B<U>` will be
/// reported in the following order: `TAUB`.
class DeclRefTypeReprFinder : public ASTWalker {
  /// The function to call when a `DeclRefTypeRepr` is seen.
  llvm::function_ref<bool(const DeclRefTypeRepr *)> Callback;

  MacroWalking getMacroWalkingBehavior() const override {
    return MacroWalking::Arguments;
  }

  PostWalkAction walkToTypeReprPost(TypeRepr *TR) override;

public:
  explicit DeclRefTypeReprFinder(
      llvm::function_ref<bool(const DeclRefTypeRepr *)> callback)
      : Callback(callback) {}
};
}

#endif
