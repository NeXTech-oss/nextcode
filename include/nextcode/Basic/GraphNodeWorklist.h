//===--- GraphNodeWorklist.h ------------------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_BASIC_GRAPHNODEWORKLIST_H
#define NEXTCODE_BASIC_GRAPHNODEWORKLIST_H

#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"

/// Worklist of pointer-like things that have an invalid default value. This not
/// only avoids duplicates in the worklist, but also avoids revisiting
/// already-popped nodes. This makes it suitable for DAG traversal. This can
/// also be used within hybrid worklist/recursive traversal by recording the
/// size of the worklist at each level of recursion.
///
/// The primary API has two methods: initialize() and pop(). Others are provided
/// for flexibility.
template <typename T, unsigned SmallSize>
struct GraphNodeWorklist {
  llvm::SmallPtrSet<T, SmallSize> nodeVisited;
  llvm::SmallVector<T, SmallSize> nodeVector;

  GraphNodeWorklist() = default;

  GraphNodeWorklist(const GraphNodeWorklist &) = delete;

  void initialize(T t) {
    clear();
    insert(t);
  }

  template <typename R>
  void initializeRange(R &&range) {
    clear();
    nodeVisited.insert(range.begin(), range.end());
    nodeVector.append(range.begin(), range.end());
  }

  T pop() { return empty() ? T() : nodeVector.pop_back_val(); }

  bool empty() const { return nodeVector.empty(); }

  unsigned size() const { return nodeVector.size(); }

  void clear() {
    nodeVector.clear();
    nodeVisited.clear();
  }

  void insert(T t) {
    if (nodeVisited.insert(t).second)
      nodeVector.push_back(t);
  }
};

#endif // NEXTCODE_BASIC_GRAPHNODEWORKLIST_H