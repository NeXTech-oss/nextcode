//===--- TupleMetadataVisitor.h - CRTP for tuple metadata ------*- C++ -*-===//
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
// A CRTP class useful for laying out tuple metadata.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_TUPLEMETADATALAYOUT_H
#define NEXTCODE_IRGEN_TUPLEMETADATALAYOUT_H

//#include "NominalMetadataVisitor.h"

namespace nextcode {
namespace irgen {

/// A CRTP class for laying out tuple metadata.
///
/// This produces an object corresponding to a TupleTypeMetadata type.
/// It does not itself doing anything special for metadata templates.
template <class Impl> struct TupleMetadataVisitor
       : public MetadataVisitor<Impl> {
  using super = MetadataVisitor<Impl>;

protected:
  using super::asImpl;

  TupleType *const Target;

  TupleMetadataVisitor(IRGenModule &IGM, TupleType *const target)
    : super(IGM), Target(target) {}

public:
  void layout() {
    super::layout();

    asImpl().addNumElementsInfo();
    asImpl().addLabelsInfo();

    for (unsigned i = 0, n = Target->getNumElements(); i < n; ++i) {
      asImpl().addElement(i, Target->getElement(i));
    }
  }
};

/// An "implementation" of TupleMetadataVisitor that just scans through
/// the metadata layout, maintaining the offset of all tuple elements.
template <class Impl> class TupleMetadataScanner
       : public TupleMetadataVisitor<Impl> {
  using super = TupleMetadataVisitor<Impl>;

protected:
  Size NextOffset = Size(0);

  TupleMetadataScanner(IRGenModule &IGM, TupleType *const target)
    : super(IGM, target) {}

public:
  void addValueWitnessTable() { addPointer(); }
  void addMetadataFlags() { addPointer(); }
  void addNumElementsInfo() { addPointer(); }
  void addLabelsInfo() { addPointer(); }
  void addElement(unsigned idx,
                  const TupleTypeElt &e) { addPointer(); addPointer(); }

private:
  void addPointer() { NextOffset += super::IGM.getPointerSize(); }
};

} // end namespace irgen
} // end namespace nextcode

#endif