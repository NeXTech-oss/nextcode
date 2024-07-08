//===--- Setup.cpp - Load-time setup code ---------------------------------===//
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
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//

#include "nextcode/Runtime/Metadata.h"

// Helper macros for figuring out the mangled name of a context descriptor.
#define DESCRIPTOR_MANGLING_SUFFIX_Structure Mn
#define DESCRIPTOR_MANGLING_SUFFIX_Class Mn
#define DESCRIPTOR_MANGLING_SUFFIX_Enum Mn
#define DESCRIPTOR_MANGLING_SUFFIX_Protocol Mp

#define DESCRIPTOR_MANGLING_SUFFIX_(X) X
#define DESCRIPTOR_MANGLING_SUFFIX(KIND)                                       \
  DESCRIPTOR_MANGLING_SUFFIX_(DESCRIPTOR_MANGLING_SUFFIX_##KIND)

#define DESCRIPTOR_MANGLING_(CHAR, SUFFIX) $sSc##CHAR##SUFFIX
#define DESCRIPTOR_MANGLING(CHAR, SUFFIX) DESCRIPTOR_MANGLING_(CHAR, SUFFIX)

// Declare context descriptors for all of the concurrency descriptors with
// standard manglings.
#define STANDARD_TYPE(KIND, MANGLING, TYPENAME)
#define STANDARD_TYPE_CONCURRENCY(KIND, MANGLING, TYPENAME)                    \
  extern "C" const nextcode::ContextDescriptor DESCRIPTOR_MANGLING(               \
      MANGLING, DESCRIPTOR_MANGLING_SUFFIX(KIND));
#include "nextcode/Demangling/StandardTypesMangling.def"

// Register our type descriptors with standard manglings when the concurrency
// runtime is loaded. This allows the runtime to quickly resolve those standard
// manglings.
__attribute__((constructor)) static void setupStandardConcurrencyDescriptors() {
  static const nextcode::ConcurrencyStandardTypeDescriptors descriptors = {
#define STANDARD_TYPE(KIND, MANGLING, TYPENAME)
#define STANDARD_TYPE_CONCURRENCY(KIND, MANGLING, TYPENAME)                    \
  &DESCRIPTOR_MANGLING(MANGLING, DESCRIPTOR_MANGLING_SUFFIX(KIND)),
#include "nextcode/Demangling/StandardTypesMangling.def"
  };
  _nextcode_registerConcurrencyStandardTypeDescriptors(&descriptors);
}
