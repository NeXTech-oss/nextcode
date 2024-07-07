//===--- Term.h - A term in the generics rewrite system ---------*- C++ -*-===//
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

#include "Symbol.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include <optional>

#ifndef NEXTCODE_RQM_TERM_H
#define NEXTCODE_RQM_TERM_H

namespace llvm {
  class raw_ostream;
}

namespace nextcode {

namespace rewriting {

/// A term is a sequence of one or more symbols.
///
/// The Term type is a uniqued, permanently-allocated representation,
/// used to represent terms in the rewrite rules themselves. See also
/// MutableTerm for the other representation.
///
/// The first symbol in the term must be a protocol, generic parameter, or
/// associated type symbol.
///
/// A layout, superclass or concrete type symbol must only appear at the
/// end of a term.
///
/// Out-of-line methods are documented in Term.cpp.
class Term final {
  friend class RewriteContext;

  struct Storage;

  const Storage *Ptr;

  Term(const Storage *ptr) : Ptr(ptr) {}

public:
  size_t size() const;

  const Symbol *begin() const;
  const Symbol *end() const;

  std::reverse_iterator<const Symbol *> rbegin() const;
  std::reverse_iterator<const Symbol *> rend() const;

  Symbol back() const;

  Symbol operator[](size_t index) const;

  /// Returns an opaque pointer that uniquely identifies this term.
  const void *getOpaquePointer() const {
    return Ptr;
  }

  static Term fromOpaquePointer(void *ptr) {
    return Term((Storage *) ptr);
  }

  static Term get(const MutableTerm &term, RewriteContext &ctx);

  const ProtocolDecl *getRootProtocol() const {
    return begin()->getRootProtocol();
  }

  bool containsUnresolvedSymbols() const;

  void dump(llvm::raw_ostream &out) const;

  std::optional<int> compare(Term other, RewriteContext &ctx) const;

  friend bool operator==(Term lhs, Term rhs) {
    return lhs.Ptr == rhs.Ptr;
  }

  friend bool operator!=(Term lhs, Term rhs) {
    return !(lhs == rhs);
  }

  friend llvm::raw_ostream &operator<<(llvm::raw_ostream &out, Term term) {
    term.dump(out);
    return out;
  }
};

/// A term is a sequence of one or more symbols.
///
/// The MutableTerm type is a dynamically-allocated representation,
/// used to represent temporary values in simplification and completion.
/// See also Term for the other representation.
///
/// The first symbol in the term must be a protocol, generic parameter, or
/// associated type symbol.
///
/// A layout constraint symbol must only appear at the end of a term.
///
/// Out-of-line methods are documented in RewriteSystem.cpp.
class MutableTerm final {
  llvm::SmallVector<Symbol, 3> Symbols;

public:
  /// Creates an empty term. At least one symbol must be added for the term
  /// to become valid.
  MutableTerm() {}

  explicit MutableTerm(const Symbol *begin,
                       const Symbol *end)
    : Symbols(begin, end) {}

  explicit MutableTerm(llvm::SmallVector<Symbol, 3> &&symbols)
    : Symbols(std::move(symbols)) {}

  explicit MutableTerm(llvm::ArrayRef<Symbol> symbols)
      : Symbols(symbols.begin(), symbols.end()) {}

  explicit MutableTerm(Term term)
    : Symbols(term.begin(), term.end()) {}

  void add(Symbol symbol) {
    Symbols.push_back(symbol);
  }

  void append(Term other) {
    Symbols.append(other.begin(), other.end());
  }

  void append(const MutableTerm &other) {
    Symbols.append(other.begin(), other.end());
  }

  void append(const Symbol *from, const Symbol *to) {
    Symbols.append(from, to);
  }

  std::optional<int> compare(const MutableTerm &other,
                             RewriteContext &ctx) const;

  bool empty() const { return Symbols.empty(); }

  size_t size() const { return Symbols.size(); }

  const ProtocolDecl *getRootProtocol() const {
    return begin()->getRootProtocol();
  }

  const Symbol *begin() const { return Symbols.begin(); }
  const Symbol *end() const { return Symbols.end(); }

  Symbol *begin() { return Symbols.begin(); }
  Symbol *end() { return Symbols.end(); }

  std::reverse_iterator<const Symbol *> rbegin() const { return Symbols.rbegin(); }
  std::reverse_iterator<const Symbol *> rend() const { return Symbols.rend(); }

  std::reverse_iterator<Symbol *> rbegin() { return Symbols.rbegin(); }
  std::reverse_iterator<Symbol *> rend() { return Symbols.rend(); }

  Symbol back() const {
    return Symbols.back();
  }

  Symbol &back() {
    return Symbols.back();
  }

  Symbol operator[](size_t index) const {
    return Symbols[index];
  }

  Symbol &operator[](size_t index) {
    return Symbols[index];
  }

  void rewriteSubTerm(Symbol *from, Symbol *to, Term rhs);

  void dump(llvm::raw_ostream &out) const;

  friend bool operator==(const MutableTerm &lhs, const MutableTerm &rhs) {
    if (lhs.size() != rhs.size())
      return false;

    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  friend bool operator!=(const MutableTerm &lhs, const MutableTerm &rhs) {
    return !(lhs == rhs);
  }

  friend llvm::raw_ostream &operator<<(llvm::raw_ostream &out,
                                       const MutableTerm &term) {
    term.dump(out);
    return out;
  }
};

} // end namespace rewriting

} // end namespace nextcode

namespace llvm {
  template<> struct DenseMapInfo<nextcode::rewriting::Term> {
    static nextcode::rewriting::Term getEmptyKey() {
      return nextcode::rewriting::Term::fromOpaquePointer(
        llvm::DenseMapInfo<void *>::getEmptyKey());
    }
    static nextcode::rewriting::Term getTombstoneKey() {
      return nextcode::rewriting::Term::fromOpaquePointer(
        llvm::DenseMapInfo<void *>::getTombstoneKey());
    }
    static unsigned getHashValue(nextcode::rewriting::Term Val) {
      return DenseMapInfo<void *>::getHashValue(Val.getOpaquePointer());
    }
    static bool isEqual(nextcode::rewriting::Term LHS,
                        nextcode::rewriting::Term RHS) {
      return LHS == RHS;
    }
  };

  template<>
  struct PointerLikeTypeTraits<nextcode::rewriting::Term> {
  public:
    static inline void *getAsVoidPointer(nextcode::rewriting::Term Val) {
      return const_cast<void *>(Val.getOpaquePointer());
    }
    static inline nextcode::rewriting::Term getFromVoidPointer(void *Ptr) {
      return nextcode::rewriting::Term::fromOpaquePointer(Ptr);
    }
    enum { NumLowBitsAvailable = 1 };
  };
} // end namespace llvm

#endif
