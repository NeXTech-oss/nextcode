//===--- IndirectTypeInfo.h - Convenience for indirected types --*- C++ -*-===//
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
// This file defines IndirectTypeInfo, which is a convenient abstract
// implementation of TypeInfo for working with types that are always
// passed or returned indirectly.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_INDIRECTTYPEINFO_H
#define NEXTCODE_IRGEN_INDIRECTTYPEINFO_H

#include "Explosion.h"
#include "TypeInfo.h"
#include "IRGenFunction.h"

namespace nextcode {
namespace irgen {

/// IndirectTypeInfo - An abstract class designed for use when
/// implementing a type which is always passed indirectly.
///
/// Subclasses must implement the following operations:
///   allocateStack
///   assignWithCopy
///   initializeWithCopy
///   destroy
template <class Derived, class Base>
class IndirectTypeInfo : public Base {
protected:
  template <class... T> IndirectTypeInfo(T &&...args)
    : Base(::std::forward<T>(args)...) {}

  const Derived &asDerived() const {
    return static_cast<const Derived &>(*this);
  }

public:
  void getSchema(ExplosionSchema &schema) const override {
    schema.add(ExplosionSchema::Element::forAggregate(this->getStorageType(),
                                              this->getBestKnownAlignment()));
  }

  void initializeFromParams(IRGenFunction &IGF, Explosion &params, Address dest,
                            SILType T, bool isOutlined) const override {
    Address src = this->getAddressForPointer(params.claimNext());
    asDerived().Derived::initializeWithTake(IGF, dest, src, T, isOutlined,
                                            /*zeroizeIfSensitive=*/ true);
  }

  void assignWithTake(IRGenFunction &IGF, Address dest, Address src, SILType T,
                      bool isOutlined) const override {
    asDerived().Derived::destroy(IGF, dest, T, isOutlined);
    asDerived().Derived::initializeWithTake(IGF, dest, src, T, isOutlined,
                                            /*zeroizeIfSensitive=*/ true);
  }
};

}
}

#endif
