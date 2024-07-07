//===--- Actor.h - ABI structures for actors --------------------*- C++ -*-===//
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
// NeXTCode ABI describing actors.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_ABI_ACTOR_H
#define NEXTCODE_ABI_ACTOR_H

#include "nextcode/ABI/HeapObject.h"
#include "nextcode/ABI/MetadataValues.h"

namespace nextcode {

/// The default actor implementation.  This is the layout of both
/// the DefaultActor and NSDefaultActor classes.
class alignas(Alignment_DefaultActor) DefaultActor : public HeapObject {
public:
  // These constructors do not initialize the actor instance, and the
  // destructor does not destroy the actor instance; you must call
  // nextcode_defaultActor_{initialize,destroy} yourself.
  constexpr DefaultActor(const HeapMetadata *metadata)
    : HeapObject(metadata), PrivateData{} {}

  constexpr DefaultActor(const HeapMetadata *metadata,
                         InlineRefCounts::Immortal_t immortal)
    : HeapObject(metadata, immortal), PrivateData{} {}

  void *PrivateData[NumWords_DefaultActor];
};

/// The non-default distributed actor implementation.
class alignas(Alignment_NonDefaultDistributedActor) NonDefaultDistributedActor : public HeapObject {
public:
  // These constructors do not initialize the actor instance, and the
  // destructor does not destroy the actor instance; you must call
  // nextcode_nonDefaultDistributedActor_initialize yourself.
  constexpr NonDefaultDistributedActor(const HeapMetadata *metadata)
    : HeapObject(metadata), PrivateData{} {}

  constexpr NonDefaultDistributedActor(const HeapMetadata *metadata,
                                       InlineRefCounts::Immortal_t immortal)
    : HeapObject(metadata, immortal), PrivateData{} {}

  void *PrivateData[NumWords_NonDefaultDistributedActor];
};

} // end namespace nextcode

#endif
