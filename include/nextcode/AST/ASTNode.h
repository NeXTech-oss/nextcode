//===--- ASTNode.h - NeXTCode Language ASTs ------------------------*- C++ -*-===//
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
// This file defines the ASTNode, which is a union of Stmt, Expr, Decl,
// Pattern, TypeRepr, StmtCondition, and CaseLabelItem.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_AST_NODE_H
#define NEXTCODE_AST_AST_NODE_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/PointerUnion.h"
#include "nextcode/Basic/Debug.h"
#include "nextcode/AST/TypeAlignments.h"

namespace llvm {
  class raw_ostream;
}

namespace nextcode {
  class Expr;
  class Stmt;
  class Decl;
  class Pattern;
  class TypeRepr;
  class DeclContext;
  class SourceLoc;
  class SourceRange;
  class ASTWalker;
  class StmtConditionElement;
  class CaseLabelItem;
  enum class ExprKind : uint8_t;
  enum class DeclKind : uint8_t;
  enum class PatternKind : uint8_t;
  enum class StmtKind;

  struct ASTNode
      : public llvm::PointerUnion<Expr *, Stmt *, Decl *, Pattern *, TypeRepr *,
                                  StmtConditionElement *, CaseLabelItem *> {
    // Inherit the constructors from PointerUnion.
    using PointerUnion::PointerUnion;

    // These are needed for lldb.
    ASTNode(Expr *E) : PointerUnion(E) {}
    ASTNode(Stmt *S) : PointerUnion(S) {}
    ASTNode(Decl *D) : PointerUnion(D) {}
    ASTNode(Pattern *P) : PointerUnion(P) {}
    ASTNode(TypeRepr *T) : PointerUnion(T) {}
    ASTNode(StmtConditionElement *S) : PointerUnion(S) {}
    ASTNode(CaseLabelItem *C) : PointerUnion(C) {}

    SourceRange getSourceRange() const;

    /// Return the location of the start of the statement.
    SourceLoc getStartLoc() const;
  
    /// Return the location of the end of the statement.
    SourceLoc getEndLoc() const;

    void walk(ASTWalker &Walker);
    void walk(ASTWalker &&walker) { walk(walker); }

    /// get the underlying entity as a decl context if it is one,
    /// otherwise, return nullptr;
    DeclContext *getAsDeclContext() const;

    /// Provides some utilities to decide detailed node kind.
#define FUNC(T) bool is##T(T##Kind Kind) const;
    FUNC(Stmt)
    FUNC(Expr)
    FUNC(Decl)
    FUNC(Pattern)
#undef FUNC

    static inline ASTNode getFromOpaqueValue(void *ptr) {
      ASTNode result;
      result.Val = decltype(result.Val)::getFromOpaqueValue(ptr);
      return result;
    }

    NEXTCODE_DEBUG_DUMP;
    void dump(llvm::raw_ostream &OS, unsigned Indent = 0) const;

    /// Whether the AST node is implicit.
    bool isImplicit() const;

    friend llvm::hash_code hash_value(ASTNode N) {
      return llvm::hash_value(N.getOpaqueValue());
    }
  };
} // namespace nextcode

namespace llvm {
  using nextcode::ASTNode;
  template <> struct DenseMapInfo<ASTNode> {
    static inline ASTNode getEmptyKey() {
      return DenseMapInfo<nextcode::Expr *>::getEmptyKey();
    }
    static inline ASTNode getTombstoneKey() {
      return DenseMapInfo<nextcode::Expr *>::getTombstoneKey();
    }
    static unsigned getHashValue(const ASTNode Val) {
      return DenseMapInfo<void *>::getHashValue(Val.getOpaqueValue());
    }
    static bool isEqual(const ASTNode LHS, const ASTNode RHS) {
      return LHS.getOpaqueValue() == RHS.getOpaqueValue();
    }
  };

  // A ASTNode is "pointer like".
  template <>
  struct PointerLikeTypeTraits<ASTNode> {
  public:
    static inline void *getAsVoidPointer(ASTNode N) {
      return (void *)N.getOpaqueValue();
    }
    static inline ASTNode getFromVoidPointer(void *P) {
      return ASTNode::getFromOpaqueValue(P);
    }
    enum { NumLowBitsAvailable = nextcode::TypeAlignInBits };
  };
}

#endif // LLVM_NEXTCODE_AST_AST_NODE_H
