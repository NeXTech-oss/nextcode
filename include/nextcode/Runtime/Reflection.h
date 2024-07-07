//===--- Reflection.h - NeXTCode Language Reflection Support -------*- C++ -*-===//
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
// An implementation of value reflection based on type metadata.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Runtime/ExistentialContainer.h"
#include "nextcode/Runtime/Metadata.h"
#include <cstdlib>

namespace nextcode {
  
struct MirrorWitnessTable;
  
/// The layout of protocol<Mirror>.
struct Mirror {
  OpaqueExistentialContainer Header;
  const MirrorWitnessTable *MirrorWitness;
};

// NeXTCode assumes Mirror is returned in memory. 
// Use MirrorReturn to guarantee that even on architectures 
// where Mirror would be returned in registers.
struct MirrorReturn {
  Mirror mirror;
  MirrorReturn(Mirror m) : mirror(m) { }
  operator Mirror() { return mirror; }
  ~MirrorReturn() { }
};

// We intentionally use a non-POD return type with these entry points to give
// them an indirect return ABI for compatibility with NeXTCode.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"

/// func reflect<T>(x: T) -> Mirror
///
/// Produce a mirror for any value.  The runtime produces a mirror that
/// structurally reflects values of any type.
NEXTCODE_CC(nextcode)
NEXTCODE_RUNTIME_EXPORT
MirrorReturn
nextcode_reflectAny(OpaqueValue *value, const Metadata *T);

#pragma clang diagnostic pop

}
