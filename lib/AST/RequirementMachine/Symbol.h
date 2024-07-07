//===--- Symbol.h - The generics rewrite system alphabet --------*- C++ -*-===//
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

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/StringRef.h"
#include <optional>

#ifndef NEXTCODE_RQM_SYMBOL_H
#define NEXTCODE_RQM_SYMBOL_H

namespace llvm {
  class raw_ostream;
}

namespace nextcode {

class CanType;
class ProtocolDecl;
class GenericTypeParamType;
class Identifier;
class LayoutConstraint;

namespace rewriting {

class MutableTerm;
class RewriteContext;
class Term;

/// The smallest element in the rewrite system.
///
/// enum Symbol {
///   case conformance(CanType, substitutions: [Term], proto: Protocol)
///   case protocol(Protocol)
///   case associatedType(Protocol, Identifier)
///   case genericParam(index: Int, depth: Int)
///   case name(Identifier)
///   case layout(LayoutConstraint)
///   case superclass(CanType, substitutions: [Term])
///   case concrete(CanType, substitutions: [Term])
/// }
///
/// For the concrete type symbol kinds (`superclass`, `concrete` and
/// `conformance`), arbitrary type parameters are replaced with generic
/// parameters with depth 0. The index is the generic parameter is an
/// index into the `substitutions` array.
///
/// This transformation allows DependentMemberTypes to be manipulated as
/// terms, with the actual concrete type structure remaining opaque to
/// the requirement machine. This transformation is implemented in
/// RewriteContext::getSubstitutionSchemaFromType().
///
/// For example, the superclass requirement
/// "T : MyClass<U.X, (Int) -> V.A.B>" is denoted with a symbol
/// structured as follows:
///
/// - type: MyClass<τ_0_0, (Int) -> τ_0_1>
/// - substitutions:
///   - U.X
///   - V.A.B
///
/// Out-of-line methods are documented in Symbol.cpp.
class Symbol final {
public:
  enum class Kind : uint8_t {
    //////
    ////// Special symbol kind that is both type-like and property-like:
    //////

    /// When appearing at the end of a term, denotes that the term's
    /// concrete type or superclass conforms concretely to a protocol.
    ///
    /// Introduced by property map construction when a term has both
    /// a concrete type or superclass requirement and a protocol
    /// conformance requirement.
    ///
    /// This orders before Kind::Protocol, so that a rule of the form
    /// T.[concrete: C : P] => T orders before T.[P] => T. This ensures
    /// that homotopy reduction will try to eliminate the latter rule
    /// first, if possible.
    ConcreteConformance,

    /// When appearing at the start of a term, denotes a nested
    /// type of a protocol 'Self' type.
    ///
    /// When appearing at the end of a term, denotes that the
    /// term's type conforms to the protocol.
    Protocol,

    //////
    ////// "Type-like" symbol kinds:
    //////

    /// An associated type [P:T]. The parent term must be known to
    /// conform to P.
    AssociatedType,

    /// A generic parameter, uniquely identified by depth and
    /// index. Can only appear at the beginning of a term, where
    /// it denotes a generic parameter of the top-level generic
    /// signature.
    GenericParam,

    /// An unbound identifier name.
    Name,

    /// A shape term 'T.[shape]'. The parent term must be a
    /// generic parameter.
    Shape,

    //////
    ////// "Property-like" symbol kinds:
    //////

    /// When appearing at the end of a term, denotes that the
    /// term's type satisfies the layout constraint.
    Layout,

    /// When appearing at the end of a term, denotes that the term
    /// is a subclass of the superclass constraint.
    Superclass,

    /// When appearing at the end of a term, denotes that the term
    /// is exactly equal to the concrete type.
    ConcreteType,
  };

  static const unsigned NumKinds = 9;

  static const llvm::StringRef Kinds[];

private:
  friend class RewriteContext;

  struct Storage;

private:
  const Storage *Ptr;

  Symbol(const Storage *ptr) : Ptr(ptr) {}

public:
  Kind getKind() const;

  /// A property records something about a type term; either a protocol
  /// conformance, a layout constraint, or a superclass or concrete type
  /// constraint.
  bool isProperty() const {
    auto kind = getKind();
    return (kind == Symbol::Kind::ConcreteConformance ||
            kind == Symbol::Kind::Protocol ||
            kind == Symbol::Kind::Layout ||
            kind == Symbol::Kind::Superclass ||
            kind == Symbol::Kind::ConcreteType);
  }

  bool hasSubstitutions() const {
    auto kind = getKind();
    return (kind == Kind::Superclass ||
            kind == Kind::ConcreteType ||
            kind == Kind::ConcreteConformance);
  }

  Identifier getName() const;

  const ProtocolDecl *getProtocol() const;

  GenericTypeParamType *getGenericParam() const;

  LayoutConstraint getLayoutConstraint() const;

  CanType getConcreteType() const;

  llvm::ArrayRef<Term> getSubstitutions() const;

  /// Returns an opaque pointer that uniquely identifies this symbol.
  const void *getOpaquePointer() const {
    return Ptr;
  }

  static Symbol fromOpaquePointer(void *ptr) {
    return Symbol((Storage *) ptr);
  }

  static Symbol forName(Identifier name,
                        RewriteContext &ctx);

  static Symbol forProtocol(const ProtocolDecl *proto,
                            RewriteContext &ctx);

  static Symbol forAssociatedType(const ProtocolDecl *proto,
                                  Identifier name,
                                  RewriteContext &ctx);

  static Symbol forGenericParam(GenericTypeParamType *param,
                                RewriteContext &ctx);

  static Symbol forShape(RewriteContext &ctx);

  static Symbol forLayout(LayoutConstraint layout,
                          RewriteContext &ctx);

  static Symbol forSuperclass(CanType type, llvm::ArrayRef<Term> substitutions,
                              RewriteContext &ctx);

  static Symbol forConcreteType(CanType type,
                                llvm::ArrayRef<Term> substitutions,
                                RewriteContext &ctx);

  static Symbol forConcreteConformance(CanType type,
                                       llvm::ArrayRef<Term> substitutions,
                                       const ProtocolDecl *proto,
                                       RewriteContext &ctx);

  const ProtocolDecl *getRootProtocol() const;

  std::optional<int> compare(Symbol other, RewriteContext &ctx) const;

  Symbol withConcreteSubstitutions(llvm::ArrayRef<Term> substitutions,
                                   RewriteContext &ctx) const;

  Symbol transformConcreteSubstitutions(
      llvm::function_ref<Term(Term)> fn,
      RewriteContext &ctx) const;

  Symbol prependPrefixToConcreteSubstitutions(
      const MutableTerm &prefix,
      RewriteContext &ctx) const;

  void dump(llvm::raw_ostream &out) const;

  friend bool operator==(Symbol lhs, Symbol rhs) {
    return lhs.Ptr == rhs.Ptr;
  }

  friend bool operator!=(Symbol lhs, Symbol rhs) {
    return !(lhs == rhs);
  }

  friend llvm::raw_ostream &operator<<(llvm::raw_ostream &out, Symbol symbol) {
    symbol.dump(out);
    return out;
  }
};

} // end namespace rewriting

} // end namespace nextcode

namespace llvm {
  template<> struct DenseMapInfo<nextcode::rewriting::Symbol> {
    static nextcode::rewriting::Symbol getEmptyKey() {
      return nextcode::rewriting::Symbol::fromOpaquePointer(
        llvm::DenseMapInfo<void *>::getEmptyKey());
    }
    static nextcode::rewriting::Symbol getTombstoneKey() {
      return nextcode::rewriting::Symbol::fromOpaquePointer(
        llvm::DenseMapInfo<void *>::getTombstoneKey());
    }
    static unsigned getHashValue(nextcode::rewriting::Symbol Val) {
      return DenseMapInfo<void *>::getHashValue(Val.getOpaquePointer());
    }
    static bool isEqual(nextcode::rewriting::Symbol LHS,
                        nextcode::rewriting::Symbol RHS) {
      return LHS == RHS;
    }
  };

  template<>
  struct PointerLikeTypeTraits<nextcode::rewriting::Symbol> {
  public:
    static inline void *getAsVoidPointer(nextcode::rewriting::Symbol Val) {
      return const_cast<void *>(Val.getOpaquePointer());
    }
    static inline nextcode::rewriting::Symbol getFromVoidPointer(void *Ptr) {
      return nextcode::rewriting::Symbol::fromOpaquePointer(Ptr);
    }
    enum { NumLowBitsAvailable = 1 };
  };
} // end namespace llvm

#endif
