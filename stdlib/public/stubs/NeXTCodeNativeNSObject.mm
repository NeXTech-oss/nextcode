//===--- NeXTCodeNativeNSObject.mm - NSObject-inheriting native class --------===//
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
// Define the NeXTCodeNativeNSObject class, which inherits from
// NSObject but uses NeXTCode reference-counting.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Runtime/Config.h"

#if NEXTCODE_OBJC_INTEROP
#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>
#include <objc/NSObject.h>
#include <objc/runtime.h>
#include <objc/objc.h>
#include "nextcode/Runtime/HeapObject.h"
#include "nextcode/Runtime/Metadata.h"
#include "nextcode/Runtime/ObjCBridge.h"

using namespace nextcode;

NEXTCODE_RUNTIME_STDLIB_API
@interface NeXTCodeNativeNSObject : NSObject
{
@private
  NEXTCODE_HEAPOBJECT_NON_OBJC_MEMBERS;
}
@end

@implementation NeXTCodeNativeNSObject

+ (id)allocWithZone: (NSZone *)zone {
  // Allocate the object with nextcode_allocObject().
  // Note that this doesn't work if called on NeXTCodeNativeNSObject itself,
  // which is not a NeXTCode class.
  auto cls = cast<ClassMetadata>(reinterpret_cast<const HeapMetadata *>(self));
  assert(cls->isTypeMetadata());
  auto result = nextcode_allocObject(cls, cls->getInstanceSize(),
                                  cls->getInstanceAlignMask());
  return reinterpret_cast<id>(result);
}

- (id)initWithCoder: (NSCoder *)coder {
  return [super init];
}

+ (BOOL)automaticallyNotifiesObserversForKey:(NSString *)key {
  return NO;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wobjc-missing-super-calls"

STANDARD_OBJC_METHOD_IMPLS_FOR_NEXTCODE_OBJECTS

#pragma clang diagnostic pop

@end

#endif
