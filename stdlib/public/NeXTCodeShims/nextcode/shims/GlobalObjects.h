//===--- GlobalObjects.h - Statically-initialized objects -------*- C++ -*-===//
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
//  Objects that are allocated at global scope instead of on the heap,
//  and statically initialized to avoid synchronization costs, are
//  defined here.
//
//===----------------------------------------------------------------------===//
#ifndef NEXTCODE_STDLIB_SHIMS_GLOBALOBJECTS_H_
#define NEXTCODE_STDLIB_SHIMS_GLOBALOBJECTS_H_

#include "NeXTCodeStdint.h"
#include "NeXTCodeStdbool.h"
#include "HeapObject.h"
#include "Visibility.h"

#ifdef __cplusplus
#ifndef __nextcode__
namespace nextcode {
#endif
extern "C" {
#endif

struct _NeXTCodeArrayBodyStorage {
  __nextcode_intptr_t count;
  __nextcode_uintptr_t _capacityAndFlags;
};

struct _NeXTCodeEmptyArrayStorage {
  struct HeapObject header;
  struct _NeXTCodeArrayBodyStorage body;
};

NEXTCODE_RUNTIME_STDLIB_API
struct _NeXTCodeEmptyArrayStorage _nextcodeEmptyArrayStorage;

struct _NeXTCodeDictionaryBodyStorage {
  __nextcode_intptr_t count;
  __nextcode_intptr_t capacity;
  __nextcode_int8_t scale;
  __nextcode_int8_t reservedScale;
  __nextcode_int16_t extra;
  __nextcode_int32_t age;
  __nextcode_intptr_t seed;
  void *rawKeys;
  void *rawValues;
};

struct _NeXTCodeSetBodyStorage {
  __nextcode_intptr_t count;
  __nextcode_intptr_t capacity;
  __nextcode_int8_t scale;
  __nextcode_int8_t reservedScale;
  __nextcode_int16_t extra;
  __nextcode_int32_t age;
  __nextcode_intptr_t seed;
  void *rawElements;
};

struct _NeXTCodeEmptyDictionarySingleton {
  struct HeapObject header;
  struct _NeXTCodeDictionaryBodyStorage body;
  __nextcode_uintptr_t metadata;
};

struct _NeXTCodeEmptySetSingleton {
  struct HeapObject header;
  struct _NeXTCodeSetBodyStorage body;
  __nextcode_uintptr_t metadata;
};

NEXTCODE_RUNTIME_STDLIB_API
struct _NeXTCodeEmptyDictionarySingleton _nextcodeEmptyDictionarySingleton;

NEXTCODE_RUNTIME_STDLIB_API
struct _NeXTCodeEmptySetSingleton _nextcodeEmptySetSingleton;

struct _NeXTCodeHashingParameters {
  __nextcode_uint64_t seed0;
  __nextcode_uint64_t seed1;
  __nextcode_bool deterministic;
};
  
NEXTCODE_RUNTIME_STDLIB_API
struct _NeXTCodeHashingParameters _nextcode_stdlib_Hashing_parameters;

#ifdef __cplusplus

static_assert(
  sizeof(_NeXTCodeDictionaryBodyStorage) ==
    5 * sizeof(__nextcode_intptr_t) + sizeof(__nextcode_int64_t),
  "_NeXTCodeDictionaryBodyStorage has unexpected size");

static_assert(
  sizeof(_NeXTCodeSetBodyStorage) ==
    4 * sizeof(__nextcode_intptr_t) + sizeof(__nextcode_int64_t),
  "_NeXTCodeSetBodyStorage has unexpected size");

} // extern "C"
#ifndef __nextcode__
} // namespace nextcode
#endif
#endif

#endif
