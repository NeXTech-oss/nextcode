//===--- Initializer.h - Initializer DeclContext ----------------*- C++ -*-===//
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
// This file defines the Initializer class, which is a kind of
// DeclContext used for expressions that are not part of a normal
// code-evaluation context, such as a global initializer or a default
// argument.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_INITIALIZER_H
#define NEXTCODE_INITIALIZER_H

#include "nextcode/AST/DeclContext.h"

namespace nextcode {
class ParamDecl;
class PatternBindingDecl;

enum class InitializerKind : uint8_t {
  /// The initializer expression of a PatternBindingDecl that declares
  /// a global variable or type member.
  PatternBinding,

  /// A function's default argument expression.
  DefaultArgument,

  /// A property wrapper initialization expression.
  PropertyWrapper,
};

/// An Initializer is a kind of DeclContext used for expressions that
/// aren't potentially evaluated as part of some function.
///
/// Generally, Initializers are created lazily, as most initializers
/// don't really require DeclContexts.
class Initializer : public DeclContext {
  unsigned Kind : 2;
protected:
  unsigned SpareBits : 30;
  
  Initializer(InitializerKind kind, DeclContext *parent)
    : DeclContext(DeclContextKind::Initializer, parent),
      Kind(unsigned(kind)) {
  }

  // Expose this to subclasses.
  using DeclContext::setParent;

public:
  /// Returns the kind of initializer this is.
  InitializerKind getInitializerKind() const {
    return InitializerKind(Kind);
  }

  static bool classof(const DeclContext *DC) {
    return DC->getContextKind() == DeclContextKind::Initializer;
  }
  static bool classof(const Initializer *I) { return true; }
};

/// The initializer expression of a non-local pattern binding
/// declaration, such as a field or global variable.
class PatternBindingInitializer : public Initializer {
  PatternBindingDecl *Binding;

  // created lazily for 'self' lookup from lazy property initializer
  ParamDecl *SelfParam;

  // Sets itself as the parent.
  friend class PatternBindingDecl;

  void setBinding(PatternBindingDecl *binding, unsigned bindingIndex);

  explicit PatternBindingInitializer(DeclContext *parent)
    : Initializer(InitializerKind::PatternBinding, parent),
      Binding(nullptr), SelfParam(nullptr) {
    SpareBits = 0;
  }

public:
  static PatternBindingInitializer *create(DeclContext *parent) {
    return new (parent->getASTContext()) PatternBindingInitializer(parent);
  }

  static PatternBindingInitializer *createDeserialized(PatternBindingDecl *PBD,
                                                       unsigned index);

  PatternBindingDecl *getBinding() const { return Binding; }

  unsigned getBindingIndex() const { return SpareBits; }

  /// If this initializes a single @lazy variable, return it.
  VarDecl *getInitializedLazyVar() const;

  /// If this initializes a single @lazy variable, lazily create a self
  /// declaration for it to refer to.
  ParamDecl *getImplicitSelfDecl() const;

  static bool classof(const DeclContext *DC) {
    if (auto init = dyn_cast<Initializer>(DC))
      return classof(init);
    return false;
  }
  static bool classof(const Initializer *I) {
    return I->getInitializerKind() == InitializerKind::PatternBinding;
  }
};

/// A default argument expression.  The parent context is the function
/// (possibly a closure) for which this is a default argument.
class DefaultArgumentInitializer : public Initializer {
public:
  explicit DefaultArgumentInitializer(DeclContext *parent, unsigned index)
      : Initializer(InitializerKind::DefaultArgument, parent) {
    SpareBits = index;
  }

  unsigned getIndex() const { return SpareBits; }

  /// Change the parent of this context.  This is necessary because
  /// the function signature is parsed before the function
  /// declaration/expression itself is built.
  void changeFunction(DeclContext *parent, ParameterList *paramLists);

  static bool classof(const DeclContext *DC) {
    if (auto init = dyn_cast<Initializer>(DC))
      return classof(init);
    return false;
  }
  static bool classof(const Initializer *I) {
    return I->getInitializerKind() == InitializerKind::DefaultArgument;
  }
};

/// A property wrapper initialization expression.  The parent context is the
/// function or closure which owns the property wrapper.
class PropertyWrapperInitializer : public Initializer {
public:
  enum class Kind {
    WrappedValue,
    ProjectedValue
  };

private:
  VarDecl *wrappedVar;
  Kind kind;

public:
  explicit PropertyWrapperInitializer(DeclContext *parent, VarDecl *wrappedVar,
                                      Kind kind)
      : Initializer(InitializerKind::PropertyWrapper, parent),
        wrappedVar(wrappedVar), kind(kind) {}

  VarDecl *getWrappedVar() const { return wrappedVar; }

  Kind getKind() const { return kind; }

  static bool classof(const DeclContext *DC) {
    if (auto init = dyn_cast<Initializer>(DC))
      return classof(init);
    return false;
  }

  static bool classof(const Initializer *I) {
    return I->getInitializerKind() == InitializerKind::PropertyWrapper;
  }
};

} // end namespace nextcode

#endif