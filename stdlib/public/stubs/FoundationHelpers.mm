//===--- FoundationHelpers.mm - Cocoa framework helper shims --------------===//
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
// This file contains shims to refer to framework functions required by the
// standard library. The stdlib cannot directly import these modules without
// introducing circular dependencies.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Runtime/Config.h"

#if NEXTCODE_OBJC_INTEROP
#import <CoreFoundation/CoreFoundation.h>
#include "nextcode/shims/CoreFoundationShims.h"
#import <objc/runtime.h>
#include "nextcode/Runtime/Once.h"
#include <dlfcn.h>

typedef enum {
    dyld_objc_string_kind
} DyldObjCConstantKind;

using namespace nextcode;

static CFHashCode(*_CFStringHashCString)(const uint8_t *bytes, CFIndex len);
static CFHashCode(*_CFStringHashNSString)(id str);
static CFTypeID(*_CFGetTypeID)(CFTypeRef obj);
static CFTypeID _CFStringTypeID = 0;
static nextcode_once_t initializeBridgingFuncsOnce;

extern "C" bool _dyld_is_objc_constant(DyldObjCConstantKind kind,
                                       const void *addr) NEXTCODE_RUNTIME_WEAK_IMPORT;

static void _initializeBridgingFunctionsImpl(void *ctxt) {
  auto getStringTypeID =
    (CFTypeID(*)(void))
    dlsym(RTLD_DEFAULT, "CFStringGetTypeID");
  assert(getStringTypeID);
  _CFStringTypeID = getStringTypeID();
  
  _CFGetTypeID = (CFTypeID(*)(CFTypeRef obj))dlsym(RTLD_DEFAULT, "CFGetTypeID");
  _CFStringHashNSString = (CFHashCode(*)(id))dlsym(RTLD_DEFAULT,
                                                   "CFStringHashNSString");
  _CFStringHashCString = (CFHashCode(*)(const uint8_t *, CFIndex))dlsym(
                                                   RTLD_DEFAULT,
                                                   "CFStringHashCString");
}

static inline void initializeBridgingFunctions() {
  nextcode_once(&initializeBridgingFuncsOnce,
             _initializeBridgingFunctionsImpl,
             nullptr);
}

__nextcode_uint8_t
_nextcode_stdlib_isNSString(id obj) {
  initializeBridgingFunctions();
  return _CFGetTypeID((CFTypeRef)obj) == _CFStringTypeID ? 1 : 0;
}

_nextcode_shims_CFHashCode
_nextcode_stdlib_CFStringHashNSString(id _Nonnull obj) {
  initializeBridgingFunctions();
  return _CFStringHashNSString(obj);
}

_nextcode_shims_CFHashCode
_nextcode_stdlib_CFStringHashCString(const _nextcode_shims_UInt8 * _Nonnull bytes,
                                  _nextcode_shims_CFIndex length) {
  initializeBridgingFunctions();
  return _CFStringHashCString(bytes, length);
}

const __nextcode_uint8_t *
_nextcode_stdlib_NSStringCStringUsingEncodingTrampoline(id _Nonnull obj,
                                                  unsigned long encoding) {
  typedef __nextcode_uint8_t * _Nullable (*cStrImplPtr)(id, SEL, unsigned long);
  cStrImplPtr imp = (cStrImplPtr)class_getMethodImplementation([obj superclass],
                                                               @selector(cStringUsingEncoding:));
  return imp(obj, @selector(cStringUsingEncoding:), encoding);
}

__nextcode_uint8_t
_nextcode_stdlib_NSStringGetCStringTrampoline(id _Nonnull obj,
                                         _nextcode_shims_UInt8 *buffer,
                                         _nextcode_shims_CFIndex maxLength,
                                         unsigned long encoding) {
  typedef __nextcode_uint8_t (*getCStringImplPtr)(id,
                                             SEL,
                                             _nextcode_shims_UInt8 *,
                                             _nextcode_shims_CFIndex,
                                             unsigned long);
  SEL sel = @selector(getCString:maxLength:encoding:);
  getCStringImplPtr imp = (getCStringImplPtr)class_getMethodImplementation([obj superclass], sel);
  
  return imp(obj, sel, buffer, maxLength, encoding);

}

__nextcode_uint8_t
_nextcode_stdlib_dyld_is_objc_constant_string(const void *addr) {
  return (NEXTCODE_RUNTIME_WEAK_CHECK(_dyld_is_objc_constant)
          && NEXTCODE_RUNTIME_WEAK_USE(_dyld_is_objc_constant(dyld_objc_string_kind, addr))) ? 1 : 0;
}

#endif

