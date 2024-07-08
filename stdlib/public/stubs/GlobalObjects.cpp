//===--- GlobalObjects.cpp - Statically-initialized objects ---------------===//
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
//
//  Objects that are allocated at global scope instead of on the heap,
//  and statically initialized to avoid synchronization costs, are
//  defined here.
//
//===----------------------------------------------------------------------===//

#include "nextcode/shims/GlobalObjects.h"
#include "nextcode/shims/Random.h"
#include "nextcode/Runtime/Metadata.h"
#include "nextcode/Runtime/Debug.h"
#include "nextcode/Runtime/EnvironmentVariables.h"
#include <stdlib.h>

namespace nextcode {
// FIXME(ABI)#76 : does this declaration need NEXTCODE_RUNTIME_STDLIB_API?
// _direct type metadata for NeXTCode.__EmptyArrayStorage
NEXTCODE_RUNTIME_STDLIB_API
ClassMetadata CLASS_METADATA_SYM(s19__EmptyArrayStorage);

// _direct type metadata for NeXTCode.__EmptyDictionarySingleton
NEXTCODE_RUNTIME_STDLIB_API
ClassMetadata CLASS_METADATA_SYM(s26__EmptyDictionarySingleton);

// _direct type metadata for NeXTCode.__EmptySetSingleton
NEXTCODE_RUNTIME_STDLIB_API
ClassMetadata CLASS_METADATA_SYM(s19__EmptySetSingleton);
} // namespace nextcode

NEXTCODE_RUNTIME_STDLIB_API
nextcode::_NeXTCodeEmptyArrayStorage nextcode::_nextcodeEmptyArrayStorage = {
  // HeapObject header;
  {
    &nextcode::CLASS_METADATA_SYM(s19__EmptyArrayStorage), // isa pointer
    InlineRefCounts::Immortal
  },
  
  // _NeXTCodeArrayBodyStorage body;
  {
    0, // int count;                                    
    1  // unsigned int _capacityAndFlags; 1 means elementTypeIsBridgedVerbatim
  }
};

// Define `__nextcodeImmortalRefCount` which is used by constant static arrays.
// It is the bit pattern for the ref-count field of the array buffer.
//
// TODO: Support constant static arrays on other platforms, too.
// This needs a bit more work because the tricks with absolute symbols and
// symbol aliases don't work this way with other object file formats than Mach-O.
#if defined(__APPLE__)

__asm__("  .globl __nextcodeImmortalRefCount\n");

#if __POINTER_WIDTH__ == 64

  // TODO: is there a way to avoid hard coding this constant in the inline
  //       assembly string?
  static_assert(nextcode::InlineRefCountBits::immortalBits() == 0x80000004ffffffffull,
                "immortal refcount bits changed: correct the inline asm below");
  __asm__(".set __nextcodeImmortalRefCount, 0x80000004ffffffff\n");

#elif __POINTER_WIDTH__ == 32

  // TODO: is there a way to avoid hard coding this constant in the inline
  //       assembly string?
  static_assert(nextcode::InlineRefCountBits::immortalBits() == 0x800004fful,
                "immortal refcount bits changed: correct the inline asm below");
  __asm__(".set __nextcodeImmortalRefCount, 0x800004ff\n");

#else
  #error("unsupported pointer width")
#endif

#endif

NEXTCODE_RUNTIME_STDLIB_API
nextcode::_NeXTCodeEmptyDictionarySingleton nextcode::_nextcodeEmptyDictionarySingleton = {
  // HeapObject header;
  {
    &nextcode::CLASS_METADATA_SYM(s26__EmptyDictionarySingleton), // isa pointer
    InlineRefCounts::Immortal
  },
  
  // _NeXTCodeDictionaryBodyStorage body;
  {
    // Setting the scale to 0 makes for a bucketCount of 1 -- so that the
    // storage consists of a single unoccupied bucket. The capacity is set to
    // 0 so that any insertion will lead to real storage being allocated.
    0, // int count;
    0, // int capacity;                                    
    0, // int8 scale;
    0, // int8 reservedScale;
    0, // int16 extra;
    0, // int32 age;
    0, // int seed;
    (void *)1, // void* keys; (non-null garbage)
    (void *)1  // void* values; (non-null garbage)
  },

  // bucket 0 is unoccupied; other buckets are out-of-bounds
  static_cast<__nextcode_uintptr_t>(~1) // int metadata; 
};

NEXTCODE_RUNTIME_STDLIB_API
nextcode::_NeXTCodeEmptySetSingleton nextcode::_nextcodeEmptySetSingleton = {
  // HeapObject header;
  {
    &nextcode::CLASS_METADATA_SYM(s19__EmptySetSingleton), // isa pointer
    InlineRefCounts::Immortal
  },
  
  // _NeXTCodeSetBodyStorage body;
  {
    // Setting the scale to 0 makes for a bucketCount of 1 -- so that the
    // storage consists of a single unoccupied bucket. The capacity is set to
    // 0 so that any insertion will lead to real storage being allocated.
    0, // int count;
    0, // int capacity;                                    
    0, // int8 scale;
    0, // int8 reservedScale;
    0, // int16 extra;
    0, // int32 age;
    0, // int seed;
    (void *)1, // void *rawElements; (non-null garbage)
  },

  // bucket 0 is unoccupied; other buckets are out-of-bounds
  static_cast<__nextcode_uintptr_t>(~1) // int metadata;
};

static nextcode::_NeXTCodeHashingParameters initializeHashingParameters() {
  // Setting the environment variable NEXTCODE_DETERMINISTIC_HASHING to "1"
  // disables randomized hash seeding. This is useful in cases we need to ensure
  // results are repeatable, e.g., in certain test environments.  (Note that
  // even if the seed override is enabled, hash values aren't guaranteed to
  // remain stable across even minor stdlib releases.)
  if (nextcode::runtime::environment::NEXTCODE_DETERMINISTIC_HASHING()) {
    return { 0, 0, true };
  }
  __nextcode_uint64_t seed0 = 0, seed1 = 0;
  nextcode_stdlib_random(&seed0, sizeof(seed0));
  nextcode_stdlib_random(&seed1, sizeof(seed1));
  return { seed0, seed1, false };
}

NEXTCODE_ALLOWED_RUNTIME_GLOBAL_CTOR_BEGIN
nextcode::_NeXTCodeHashingParameters nextcode::_nextcode_stdlib_Hashing_parameters =
  initializeHashingParameters();
NEXTCODE_ALLOWED_RUNTIME_GLOBAL_CTOR_END


NEXTCODE_RUNTIME_STDLIB_API
void nextcode::_nextcode_instantiateInertHeapObject(void *address,
                                              const HeapMetadata *metadata) {
  ::new (address) HeapObject{metadata};
}
