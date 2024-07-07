//===--- TypeAlignments.h - Alignments of various NeXTCode types ---*- C++ -*-===//
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
/// \file This file defines the alignment of various NeXTCode AST classes.
///
/// It's useful to do this in a dedicated place to avoid recursive header
/// problems. To make sure we don't have any ODR violations, this header
/// should be included in every header that defines one of the forward-
/// declared types listed here.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_TYPEALIGNMENTS_H
#define NEXTCODE_TYPEALIGNMENTS_H

#include <cstddef>

namespace nextcode {
  class AbstractClosureExpr;
  class AbstractStorageDecl;
  class ArchetypeType;
  class AssociatedTypeDecl;
  class ASTContext;
  class AttributeBase;
  class BraceStmt;
  class CaptureListExpr;
  class Decl;
  class DeclContext;
  class DifferentiableAttr;
  class Expr;
  class ExtensionDecl;
  class FileUnit;
  class GenericEnvironment;
  class GenericParamList;
  class GenericTypeParamDecl;
  class NominalTypeDecl;
  class NormalProtocolConformance;
  class OpaqueValueExpr;
  class OperatorDecl;
  class PackConformance;
  class Pattern;
  class ProtocolDecl;
  class ProtocolConformance;
  class RequirementRepr;
  class SILFunction;
  class SILFunctionType;
  class SpecializeAttr;
  class Stmt;
  class TrailingWhereClause;
  class TypeVariableType;
  class TypeBase;
  class TypeDecl;
  class TypeRepr;
  class ValueDecl;
  class CaseLabelItem;
  class StmtConditionElement;

  /// We frequently use three tag bits on all of these types.
  constexpr size_t AttrAlignInBits = 3;
  constexpr size_t DeclAlignInBits = 3;
  constexpr size_t DeclContextAlignInBits = 3;
  constexpr size_t ExprAlignInBits = 3;
  constexpr size_t StmtAlignInBits = 3;
  constexpr size_t TypeAlignInBits = 3;
  constexpr size_t PatternAlignInBits = 3;
  constexpr size_t TypeReprAlignInBits = 3;

  constexpr size_t SILFunctionAlignInBits = 2;
  constexpr size_t ASTContextAlignInBits = 2;
  constexpr size_t TypeVariableAlignInBits = 4;
  constexpr size_t StoredDefaultArgumentAlignInBits = 3;

  // Well, this is the *minimum* pointer alignment; it's going to be 3 on
  // 64-bit targets, but that doesn't matter.
  constexpr size_t PointerAlignInBits = 2;
}

namespace llvm {
  /// Helper class for declaring the expected alignment of a pointer.
  /// TODO: LLVM should provide this.
  template <class T, size_t AlignInBits> struct MoreAlignedPointerTraits {
    enum { NumLowBitsAvailable = AlignInBits };
    static inline void *getAsVoidPointer(T *ptr) { return ptr; }
    static inline T *getFromVoidPointer(void *ptr) {
      return static_cast<T*>(ptr);
    }
  };

  template <class T> struct PointerLikeTypeTraits;
}

/// Declare the expected alignment of pointers to the given type.
/// This macro should be invoked from a top-level file context.
#define LLVM_DECLARE_TYPE_ALIGNMENT(CLASS, ALIGNMENT)     \
namespace llvm {                                          \
template <> struct PointerLikeTypeTraits<CLASS*>          \
  : public MoreAlignedPointerTraits<CLASS, ALIGNMENT> {}; \
}

LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::Decl, nextcode::DeclAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::AbstractStorageDecl, nextcode::DeclAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::AssociatedTypeDecl, nextcode::DeclAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::GenericTypeParamDecl, nextcode::DeclAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::OperatorDecl, nextcode::DeclAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::ProtocolDecl, nextcode::DeclAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::TypeDecl, nextcode::DeclAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::ValueDecl, nextcode::DeclAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::NominalTypeDecl, nextcode::DeclAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::ExtensionDecl, nextcode::DeclAlignInBits)

LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::TypeBase, nextcode::TypeAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::ArchetypeType, nextcode::TypeAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::TypeVariableType, nextcode::TypeVariableAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::SILFunctionType,
                            nextcode::TypeVariableAlignInBits)

LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::Stmt, nextcode::StmtAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::BraceStmt, nextcode::StmtAlignInBits)

LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::ASTContext, nextcode::ASTContextAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::DeclContext, nextcode::DeclContextAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::FileUnit, nextcode::DeclContextAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::DifferentiableAttr, nextcode::PointerAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::Expr, nextcode::ExprAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::CaptureListExpr, nextcode::ExprAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::AbstractClosureExpr, nextcode::ExprAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::OpaqueValueExpr, nextcode::ExprAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::PackConformance, nextcode::DeclAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::ProtocolConformance, nextcode::DeclAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::NormalProtocolConformance,
                            nextcode::DeclAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::GenericEnvironment,
                            nextcode::DeclAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::GenericParamList,
                            nextcode::PointerAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::Pattern,
                            nextcode::PatternAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::RequirementRepr,
                            nextcode::PointerAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::SILFunction,
                            nextcode::SILFunctionAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::SpecializeAttr, nextcode::PointerAlignInBits)
LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::TrailingWhereClause,
                            nextcode::PointerAlignInBits)

LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::AttributeBase, nextcode::AttrAlignInBits)

LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::TypeRepr, nextcode::TypeReprAlignInBits)

LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::CaseLabelItem, nextcode::PatternAlignInBits)

LLVM_DECLARE_TYPE_ALIGNMENT(nextcode::StmtConditionElement,
                            nextcode::PatternAlignInBits)

static_assert(alignof(void*) >= 2, "pointer alignment is too small");

#endif
