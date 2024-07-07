//===--- ConformanceDescription.h - Conformance record ----------*- C++ -*-===//
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
// This file defines the ConformanceDescription type, which records a
// conformance which needs to be emitted.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_CONFORMANCEDESCRIPTION_H
#define NEXTCODE_IRGEN_CONFORMANCEDESCRIPTION_H

namespace llvm {
class Constant;
}

namespace nextcode {
class SILWitnessTable;

namespace irgen {

/// The description of a protocol conformance, including its witness table
/// and any additional information needed to produce the protocol conformance
/// descriptor.
class ConformanceDescription {
public:
  /// The conformance itself.
  RootProtocolConformance *conformance;

  /// The witness table.
  SILWitnessTable *wtable;

  /// The witness table pattern, which is also a complete witness table
  /// when \c requiresSpecialization is \c false.
  llvm::Constant *pattern;

  /// The size of the witness table.
  const uint16_t witnessTableSize;

  /// The private size of the witness table, allocated
  const uint16_t witnessTablePrivateSize;

  /// Whether this witness table requires runtime specialization.
  const unsigned requiresSpecialization : 1;

  /// The instantiation function, to be run at the end of witness table
  /// instantiation.
  llvm::Function *instantiationFn = nullptr;

  /// The resilient witnesses, if any.
  SmallVector<llvm::Constant *, 4> resilientWitnesses;

  ConformanceDescription(RootProtocolConformance *conformance,
                         SILWitnessTable *wtable,
                         llvm::Constant *pattern,
                         uint16_t witnessTableSize,
                         uint16_t witnessTablePrivateSize,
                         bool requiresSpecialization)
    : conformance(conformance), wtable(wtable), pattern(pattern),
      witnessTableSize(witnessTableSize),
      witnessTablePrivateSize(witnessTablePrivateSize),
      requiresSpecialization(requiresSpecialization)
  {
  }
};

} // end namespace irgen
} // end namespace nextcode

#endif
