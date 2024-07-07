//===- IndexedViewRange.h - Iterators for indexed projections ---*- C++ -*-===//
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
//  This file defines IndexedViewRange, a template class which makes it
//  easy to define a range for a "collection" that is normally just vended
//  with an indexed accessor.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_INDEXEDVIEWRANGE_H
#define NEXTCODE_BASIC_INDEXEDVIEWRANGE_H

#include <iterator>
#include <memory>
#include "llvm/ADT/iterator_range.h"

namespace nextcode {

/// An iterator over a range of values provided by an indexed accessor
/// on a base type.
template <class BaseType, class ProjectedType,
          ProjectedType (&Project)(BaseType, size_t)>
class IndexedViewIterator {
public:
  using value_type = ProjectedType;
  using reference = ProjectedType;
  using pointer = void;
  using difference_type = ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;
private:
  BaseType Base;
  size_t Index;
public:
  IndexedViewIterator(BaseType base, size_t index)
    : Base(base), Index(index) {}
public:
  ProjectedType operator*() const { return Project(Base, Index); }
  ProjectedType operator->() const { return Project(Base, Index); }
  IndexedViewIterator &operator++() { Index++; return *this; }
  IndexedViewIterator operator++(int) { return iterator(Base, Index++); }
  IndexedViewIterator &operator--() { Index--; return *this; }
  IndexedViewIterator operator--(int) { return iterator(Base, Index--); }
  bool operator==(IndexedViewIterator rhs) const { return Index == rhs.Index; }
  bool operator!=(IndexedViewIterator rhs) const { return Index != rhs.Index; }

  IndexedViewIterator &operator+=(difference_type i) {
    Index += i;
    return *this;
  }
  IndexedViewIterator operator+(difference_type i) const {
    return IndexedViewIterator(Base, Index + i);
  }
  friend IndexedViewIterator operator+(difference_type i,
                                       IndexedViewIterator rhs) {
    return IndexedViewIterator(rhs.Base, rhs.Index + i);
  }
  IndexedViewIterator &operator-=(difference_type i) {
    Index -= i;
    return *this;
  }
  IndexedViewIterator operator-(difference_type i) const {
    return IndexedViewIterator(Base, Index - i);
  }
  difference_type operator-(IndexedViewIterator rhs) const {
    return Index - rhs.Index;
  }
  ProjectedType operator[](difference_type i) const {
    return Project(Base, Index + i);
  }
  bool operator<(IndexedViewIterator rhs) const {
    return Index < rhs.Index;
  }
  bool operator<=(IndexedViewIterator rhs) const {
    return Index <= rhs.Index;
  }
  bool operator>(IndexedViewIterator rhs) const {
    return Index > rhs.Index;
  }
  bool operator>=(IndexedViewIterator rhs) const {
    return Index >= rhs.Index;
  }
};

template <class BaseType, class ProjectedType,
          ProjectedType (&Project)(BaseType, size_t)>
using IndexedViewRange =
  llvm::iterator_range<IndexedViewIterator<BaseType, ProjectedType, Project>>;

} // end namespace nextcode

#endif // NEXTCODE_BASIC_INDEXEDVIEWRANGE_H