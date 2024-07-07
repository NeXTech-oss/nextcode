//===--- ConstraintGraphScope.h - Constraint Graph Scope --------*- C++ -*-===//
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
// This file defines the \c ConstraintGraphScope class, an RAII object that
// introduces a new scope in which changes to the constraint graph are
// capture and will be reverted when the scope disappears.
//
//===----------------------------------------------------------------------===//
#ifndef NEXTCODE_SEMA_CONSTRAINT_GRAPH_SCOPE_H
#define NEXTCODE_SEMA_CONSTRAINT_GRAPH_SCOPE_H

namespace nextcode {
namespace constraints {

class ConstraintGraph;

/// RAII object that introduces a new constraint graph scope to capture
/// modifications made to the constraint graph.
///
/// Scopes are arranged in a stack, where the active scope is the top of the
/// stack. All changes made to the constraint graph are recorded in the
/// active scope. When the scope is popped (via the RAII object destructor),
/// those changes are reverted.
class ConstraintGraphScope {
  ConstraintGraph &CG;

  /// The parent scope, or null if this is the topmost scope.
  ConstraintGraphScope *ParentScope;

  /// The number of recorded changes that existed at the time this scope was
  /// formed.
  ///
  /// When this scope exits, any changes beyond this will be reverted to
  /// bring the graph back to its state prior to the introduction of this
  /// variable.
  unsigned NumChanges;

  ConstraintGraphScope(const ConstraintGraphScope&) = delete;
  ConstraintGraphScope &operator=(const ConstraintGraphScope&) = delete;

public:
  explicit ConstraintGraphScope(ConstraintGraph &CG);
  ~ConstraintGraphScope();
  
  /// Get number of changes recorded at the start of the current active scope.
  unsigned getStartIdx() {
    return NumChanges;
  }
};

} // end namespace constraints
} // end namespace nextcode

#endif // LLVM_NEXTCODE_SEMA_CONSTRAINT_GRAPH_SCOPE_H
