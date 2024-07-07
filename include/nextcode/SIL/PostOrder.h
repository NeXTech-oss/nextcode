//===--- PostOrder.h --------------------------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_SIL_POSTORDER_H
#define NEXTCODE_SIL_POSTORDER_H

#include "nextcode/Basic/Range.h"
#include "nextcode/SIL/CFG.h"
#include "nextcode/SIL/SILBasicBlock.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SILOptimizer/Analysis/Analysis.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/iterator_range.h"
#include <vector>

namespace nextcode {

class PostOrderFunctionInfo {
  std::vector<SILBasicBlock *> PostOrder;
  llvm::DenseMap<SILBasicBlock *, unsigned> BBToPOMap;

public:
  PostOrderFunctionInfo(SILFunction *F) {
    for (auto *BB : make_range(po_begin(F), po_end(F))) {
      BBToPOMap[BB] = PostOrder.size();
      PostOrder.push_back(BB);
    }
  }

  using iterator = decltype(PostOrder)::iterator;
  using const_iterator = decltype(PostOrder)::const_iterator;
  using reverse_iterator = decltype(PostOrder)::reverse_iterator;
  using const_reverse_iterator = decltype(PostOrder)::const_reverse_iterator;

  using range = iterator_range<iterator>;
  using const_range = iterator_range<const_iterator>;
  using reverse_range = iterator_range<reverse_iterator>;
  using const_reverse_range = iterator_range<const_reverse_iterator>;

  range getPostOrder() {
    return make_range(PostOrder.begin(), PostOrder.end());
  }
  const_range getPostOrder() const {
    return make_range(PostOrder.begin(), PostOrder.end());
  }
  reverse_range getReversePostOrder() {
    return make_range(PostOrder.rbegin(), PostOrder.rend());
  }
  const_reverse_range getReversePostOrder() const {
    return make_range(PostOrder.rbegin(), PostOrder.rend());
  }

  const_reverse_range getReversePostOrder(SILBasicBlock *StartBlock) const {
    unsigned RPONumber = getRPONumber(StartBlock).value();
    return getReversePostOrder(RPONumber);
  }

  const_reverse_range getReversePostOrder(unsigned RPONumber) const {
    return make_range(std::next(PostOrder.rbegin(), RPONumber),
                      PostOrder.rend());
  }

  unsigned size() const { return PostOrder.size(); }

  std::optional<unsigned> getPONumber(SILBasicBlock *BB) const {
    auto Iter = BBToPOMap.find(BB);
    if (Iter != BBToPOMap.end())
      return Iter->second;
    return std::nullopt;
  }

  std::optional<unsigned> getRPONumber(SILBasicBlock *BB) const {
    auto Iter = BBToPOMap.find(BB);
    if (Iter != BBToPOMap.end())
      return PostOrder.size() - Iter->second - 1;
    return std::nullopt;
  }
};

} // end nextcode namespace

#endif
