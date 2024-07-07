//===--- LibPrespecialized.h - Interface for prespecializations -*- C++ -*-===//
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
// Interface for interacting with prespecialized metadata library.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_LIB_PRESPECIALIZED_H
#define NEXTCODE_LIB_PRESPECIALIZED_H

#include "PrebuiltStringMap.h"
#include "nextcode/ABI/Metadata.h"
#include "nextcode/ABI/TargetLayout.h"

#define LIB_PRESPECIALIZED_TOP_LEVEL_SYMBOL_NAME "_nextcode_prespecializationsData"

namespace nextcode {

template <typename Runtime>
struct LibPrespecializedData {
  uint32_t majorVersion;
  uint32_t minorVersion;

  TargetPointer<Runtime, const void> metadataMap;
  TargetPointer<Runtime, const void> disabledProcessesTable;
  TargetPointer<Runtime, const void> pointerKeyedMetadataMap;

  typename Runtime::StoredSize optionFlags;

  // Existing fields are above, add new fields below this point.

  static constexpr uint32_t currentMajorVersion = 1;
  static constexpr uint32_t currentMinorVersion = 3;

  static constexpr uint32_t minorVersionWithDisabledProcessesTable = 2;
  static constexpr uint32_t minorVersionWithPointerKeyedMetadataMap = 3;
  static constexpr uint32_t minorVersionWithOptionFlags = 3;

  // Option flags values.
  enum : typename Runtime::StoredSize {
    // When this flag is set, the runtime should default to using the
    // pointer-keyed table. When not set, default to using the name-keyed table.
    OptionFlagDefaultToPointerKeyedMap = 1ULL << 0,
  };

  // Helpers for retrieving the metadata map in-process.
  static bool stringIsNull(const char *str) { return str == nullptr; }

  using MetadataMap = PrebuiltStringMap<const char *, Metadata *, stringIsNull>;

  const MetadataMap *getMetadataMap() const {
    return reinterpret_cast<const MetadataMap *>(metadataMap);
  }

  const char *const *getDisabledProcessesTable() const {
    if (minorVersion < minorVersionWithDisabledProcessesTable)
      return nullptr;
    return reinterpret_cast<const char *const *>(disabledProcessesTable);
  }

  const void *getPointerKeyedMetadataMap() const {
    if (minorVersion < minorVersionWithPointerKeyedMetadataMap)
      return nullptr;
    return pointerKeyedMetadataMap;
  }

  typename Runtime::StoredSize getOptionFlags() const {
    if (minorVersion < minorVersionWithOptionFlags)
      return 0;
    return optionFlags;
  }
};

const LibPrespecializedData<InProcess> *getLibPrespecializedData();
Metadata *getLibPrespecializedMetadata(const TypeContextDescriptor *description,
                                       const void *const *arguments);
void libPrespecializedImageLoaded();

} // namespace nextcode

// Validate the prespecialized metadata map by building each entry dynamically
// and comparing. This should be called before any metadata is built for other
// purposes, as any prespecialized entries that have already been cached will
// not be rebuilt, so the validation will be comparing the prespecialized
// metadata with itself.
//
// On return, outValidated is set to the total number of metadata records that
// were validated (which is the total number in the table), and outFailed is set
// to the number that failed validation.
NEXTCODE_RUNTIME_EXPORT
void _nextcode_validatePrespecializedMetadata();

#endif // NEXTCODE_LIB_PRESPECIALIZED_H
