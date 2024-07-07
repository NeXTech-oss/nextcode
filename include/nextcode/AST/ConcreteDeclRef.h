//===--- ConcreteDeclRef.h - Reference to a concrete decl -------*- C++ -*-===//
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
// This file defines the ConcreteDeclRef class, which provides a reference to
// a declaration that is potentially specialized.
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_CONCRETEDECLREF_H
#define NEXTCODE_AST_CONCRETEDECLREF_H

#include "nextcode/Basic/Debug.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/AST/SubstitutionMap.h"
#include "nextcode/AST/TypeAlignments.h"
#include "llvm/ADT/PointerUnion.h"
#include "llvm/Support/Compiler.h"
#include <cstring>

namespace nextcode {

class ASTContext;
class SourceManager;
class ValueDecl;

/// A reference to a concrete representation of a particular declaration,
/// providing substitutions for all type parameters of the original,
/// underlying declaration.
class ConcreteDeclRef {
  /// The declaration.
  ValueDecl *decl = nullptr;

  /// The substitutions.
  SubstitutionMap substitutions;

public:
  /// Create an empty declaration reference.
  ConcreteDeclRef() { }

  /// Construct a reference to the given value.
  ConcreteDeclRef(ValueDecl *decl) : decl(decl) { }

  /// Construct a reference to the given value, specialized with the given
  /// substitutions.
  ///
  /// \param decl The declaration to which this reference refers, which will
  /// be specialized by applying the given substitutions.
  ///
  /// \param substitutions The complete set of substitutions to apply to the
  /// given declaration.
  ConcreteDeclRef(ValueDecl *decl, SubstitutionMap substitutions)
    : decl(decl), substitutions(substitutions) { }

  /// Determine whether this declaration reference refers to anything.
  explicit operator bool() const { return decl != nullptr; }

  /// Retrieve the declarations to which this reference refers.
  ValueDecl *getDecl() const { return decl; }

  /// Retrieve a reference to the declaration this one overrides.
  ConcreteDeclRef getOverriddenDecl() const;

  /// Retrieve a reference to the given declaration that this one overrides.
  ConcreteDeclRef getOverriddenDecl(ValueDecl *overriddenDecl) const;

  /// Determine whether this reference specializes the declaration to which
  /// it refers.
  bool isSpecialized() const { return !substitutions.empty(); }

  /// For a specialized reference, return the set of substitutions applied to
  /// the declaration reference.
  SubstitutionMap getSubstitutions() const { return substitutions; }

  friend bool operator==(ConcreteDeclRef lhs, ConcreteDeclRef rhs) {
    return lhs.decl == rhs.decl && lhs.substitutions == rhs.substitutions;
  }
  
  /// Dump a debug representation of this reference.
  void dump(raw_ostream &os) const;
  NEXTCODE_DEBUG_DUMP;
};

} // end namespace nextcode

#endif
