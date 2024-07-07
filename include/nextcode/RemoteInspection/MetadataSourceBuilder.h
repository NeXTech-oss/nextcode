//===--- MetadataSourceBuilder.h - Metadata Source Builder ------*- C++ -*-===//
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
// Implements utilities for constructing MetadataSources.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_REFLECTION_METADATASOURCEBUILDER_H
#define NEXTCODE_REFLECTION_METADATASOURCEBUILDER_H

#include "nextcode/RemoteInspection/MetadataSource.h"

namespace nextcode {
namespace reflection {

class MetadataSourceBuilder {
  std::vector<std::unique_ptr<const MetadataSource>> MetadataSourcePool;
public:
  using Source = const MetadataSource *;

  MetadataSourceBuilder() {}

  MetadataSourceBuilder(const MetadataSourceBuilder &Other) = delete;
  MetadataSourceBuilder &operator=(const MetadataSourceBuilder &Other) = delete;

  template <typename MetadataSourceTy, typename... Args>
  MetadataSourceTy *make_source(Args... args) {
    auto MS = new MetadataSourceTy(::std::forward<Args>(args)...);
    MetadataSourcePool.push_back(std::unique_ptr<const MetadataSource>(MS));
    return MS;
  }

  const GenericArgumentMetadataSource *
  createGenericArgument(unsigned Index, const MetadataSource *Source) {
    return GenericArgumentMetadataSource::create(*this, Index, Source);
  }

  const MetadataCaptureMetadataSource *
  createMetadataCapture(unsigned Index) {
    return MetadataCaptureMetadataSource::create(*this, Index);
  }

  const ReferenceCaptureMetadataSource *
  createReferenceCapture(unsigned Index) {
    return ReferenceCaptureMetadataSource::create(*this, Index);
  }

  const ClosureBindingMetadataSource *
  createClosureBinding(unsigned Index) {
    return ClosureBindingMetadataSource::create(*this, Index);
  }

  const SelfMetadataSource *
  createSelf() {
    return SelfMetadataSource::create(*this);
  }

  const SelfWitnessTableMetadataSource *
  createSelfWitnessTable() {
    return SelfWitnessTableMetadataSource::create(*this);
  }
};

} // end namespace reflection
} // end namespace nextcode

#endif // NEXTCODE_REFLECTION_METADATASOURCEBUILDER_H
