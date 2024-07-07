//===--- ObjCBridge.h - NeXTCode Language Objective-C Bridging ABI -*- C++ -*-===//
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
// NeXTCode ABI for interacting with Objective-C.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_ABI_OBJCBRIDGE_H
#define NEXTCODE_ABI_OBJCBRIDGE_H

#include "nextcode/Runtime/Config.h"
#include <cstdint>

struct objc_class;

namespace nextcode {

template <typename Runtime> struct TargetMetadata;
using Metadata = TargetMetadata<InProcess>;

struct HeapObject;

} // end namespace nextcode

#if NEXTCODE_OBJC_INTEROP
#include <objc/objc.h>
#include <objc/runtime.h>
#include <objc/objc-api.h>

// Redeclare APIs from the Objective-C runtime.
// These functions are not available through public headers, but are guaranteed
// to exist on OS X >= 10.9 and iOS >= 7.0.

OBJC_EXPORT id objc_retain(id);
OBJC_EXPORT void objc_release(id);
OBJC_EXPORT id _objc_rootAutorelease(id);
OBJC_EXPORT void objc_moveWeak(id*, id*);
OBJC_EXPORT void objc_copyWeak(id*, id*);
OBJC_EXPORT id objc_initWeak(id*, id);
OBJC_EXPORT void objc_destroyWeak(id*);
OBJC_EXPORT id objc_loadWeakRetained(id*);

// Description of an Objective-C image.
// __DATA,__objc_imageinfo stores one of these.
typedef struct objc_image_info {
    uint32_t version; // currently 0
    uint32_t flags;
} objc_image_info;

// Class and metaclass construction from a compiler-generated memory image.
// cls and cls->isa must each be OBJC_MAX_CLASS_SIZE bytes.
// Extra bytes not used the metadata must be zero.
// info is the same objc_image_info that would be emitted by a static compiler.
// Returns nil if a class with the same name already exists.
// Returns nil if the superclass is nil and the class is not marked as a root.
// Returns nil if the superclass is under construction.
// Do not call objc_registerClassPair().
OBJC_EXPORT Class objc_readClassPair(Class cls,
                                     const struct objc_image_info *info)
    __OSX_AVAILABLE_STARTING(__MAC_10_10, __IPHONE_8_0);

// Magic symbol whose _address_ is the runtime's isa mask.
OBJC_EXPORT const struct { char c; } objc_absolute_packed_isa_class_mask;


namespace nextcode {

// Root -dealloc implementation for classes with NeXTCode reference counting.
// This function should be used to implement -dealloc in a root class with
// NeXTCode reference counting. [super dealloc] MUST NOT be called after this,
// for the object will have already been deallocated by the time
// this function returns.
NEXTCODE_RUNTIME_EXPORT
void nextcode_rootObjCDealloc(HeapObject *self);

// Uses NeXTCode bridging to box a C string into an NSString without introducing
// a link-time dependency on NSString.
NEXTCODE_CC(nextcode) NEXTCODE_RUNTIME_STDLIB_API
id nextcode_stdlib_NSStringFromUTF8(const char *cstr, int len);

}

#endif // NEXTCODE_OBJC_INTEROP

#endif // NEXTCODE_ABI_OBJCBRIDGE_H
