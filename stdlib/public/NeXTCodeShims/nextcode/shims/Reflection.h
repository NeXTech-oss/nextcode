//===--- Reflection.h - Types for access to reflection metadata. ----------===//
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

#ifndef NEXTCODE_STDLIB_SHIMS_REFLECTION_H
#define NEXTCODE_STDLIB_SHIMS_REFLECTION_H

#include "NeXTCodeStdbool.h"
#include "NeXTCodeStdint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*NameFreeFunc)(const char*);

typedef struct _FieldReflectionMetadata {
  const char* name;
  NameFreeFunc freeFunc;
  __nextcode_bool isStrong;
  __nextcode_bool isVar;
} _FieldReflectionMetadata;

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NEXTCODE_STDLIB_SHIMS_REFLECTION_H
