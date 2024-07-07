//===--- GenericMetadataCacheEntry.h ----------------------------*- C++ -*-===//
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
//             Tural Ghuliev (turalquliyev@it-gss.com)
//
//===----------------------------------------------------------------------===//
//
// Declares a struct that mirrors the layout of GenericCacheEntry in
// Metadata.cpp and use a static assert to check that the offset of
// the member Value match between the two.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_REFLECTION_GENERICMETADATACACHEENTRY_H
#define NEXTCODE_REFLECTION_GENERICMETADATACACHEENTRY_H

#include <cstdint>

namespace nextcode {

template<typename StoredPointer>
struct GenericMetadataCacheEntry {
  StoredPointer TrackingInfo;
  uint16_t NumKeyParameters;
  uint16_t NumWitnessTables;
  uint16_t NumPacks;
  uint16_t NumShapeClasses;
  StoredPointer PackShapeDescriptors;
  uint32_t Hash;
  StoredPointer Value;
};

} // namespace nextcode

#endif // NEXTCODE_REFLECTION_GENERICMETADATACACHEENTRY_H
