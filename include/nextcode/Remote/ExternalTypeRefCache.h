//===--- ExternalTypeRefCache.h - Abstract access to external caches of
//typeref ------*- C++ -*-===//
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
/// @file
/// This file declares an abstract interface for external caches of
/// typeref information.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_REMOTE_EXTERNALTYPEREFCACHE_H
#define NEXTCODE_REMOTE_EXTERNALTYPEREFCACHE_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include <optional>

#include <string>

namespace nextcode {
namespace reflection {

template <typename T>
class ReflectionSection;
class FieldDescriptorIterator;
using FieldSection = ReflectionSection<FieldDescriptorIterator>;
}

namespace remote {
/// A struct with the information required to locate a specific field
/// descriptor.
struct FieldDescriptorLocator {
  /// The reflection info ID the field descriptor belongs to.
  uint64_t InfoID;

  /// The offset of the field descriptor in the FieldSection buffer.
  uint64_t Offset;
};

/// An abstract interface for providing external type layout information.
struct ExternalTypeRefCache {
  virtual ~ExternalTypeRefCache() = default;

  /// Cache the field descriptors of a reflection info with a given id with
  /// their corresponding mangled names. The amount of field descriptors and
  /// mangled names must be the same. If a field descriptor does not have a
  /// mangled name a corresponding empty string must be in the mangled_names
  /// array.
  virtual void
  cacheFieldDescriptors(uint64_t InfoID,
                        const nextcode::reflection::FieldSection &FieldDescriptors,
                        llvm::ArrayRef<std::string> MangledNames) = 0;

  /// Retrieve a pair representing the reflection info id and the offset of a
  /// field descriptor in the field section buffer, if available.
  virtual std::optional<FieldDescriptorLocator>
  getFieldDescriptorLocator(const std::string &Name) = 0;

  /// Returns whether the reflection info with the corresponding ID has been
  /// cached already.
  virtual bool isReflectionInfoCached(uint64_t InfoID) = 0;
};

} // namespace remote
} // namespace nextcode
#endif