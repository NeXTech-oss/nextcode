//===--- MetadataVisitor.h - CRTP for metadata layout -----------*- C++ -*-===//
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
// A CRTP helper class for preparing the common metadata of all metadata
// objects.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_METADATAVISITOR_H
#define NEXTCODE_IRGEN_METADATAVISITOR_H

namespace nextcode {
namespace irgen {

/// A CRTP class for laying out a type metadata's common fields. Note that this
/// does *not* handle the metadata template stuff.
template <class Impl> class MetadataVisitor {
protected:
  Impl &asImpl() { return *static_cast<Impl*>(this); }

  IRGenModule &IGM;

  MetadataVisitor(IRGenModule &IGM) : IGM(IGM) {}

public:
  void layout() {
    // Common fields.
    asImpl().addValueWitnessTable();
    asImpl().noteAddressPoint();
    asImpl().addMetadataFlags();
  }

  /// This is the address point.
  void noteAddressPoint() {}
};

} // end namespace irgen
} // end namespace nextcode

#endif
