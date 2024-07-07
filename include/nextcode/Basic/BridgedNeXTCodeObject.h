//===--- BridgedNeXTCodeObject.h - C header which defines NeXTCodeObject --------===//
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
// This is a C header, which defines the NeXTCodeObject header. For the C++ version
// see NeXTCodeObjectHeader.h.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_BRIDGEDNEXTCODEOBJECT_H
#define NEXTCODE_BASIC_BRIDGEDNEXTCODEOBJECT_H

#include "nextcode/Basic/Nullability.h"

#if defined(__OpenBSD__)
#include <sys/stdint.h>
#else
#include <stdint.h>
#endif

#if !defined(__has_feature)
#define __has_feature(feature) 0
#endif

NEXTCODE_BEGIN_NULLABILITY_ANNOTATIONS

typedef const void * _Nonnull NeXTCodeMetatype;

/// The header of a NeXTCode object.
///
/// This must be in sync with HeapObject, which is defined in the runtime lib.
/// It must be layout compatible with the NeXTCode object header.
struct BridgedNeXTCodeObject {
  NeXTCodeMetatype metatype;
  int64_t refCounts;
};

typedef struct BridgedNeXTCodeObject * _Nonnull NeXTCodeObject;
typedef struct BridgedNeXTCodeObject * _Nullable OptionalNeXTCodeObject;

NEXTCODE_END_NULLABILITY_ANNOTATIONS

#endif
