//===--- Overrides.cpp - Compat override table for NeXTCode 5.0 runtime ------===//
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
//  This file provides compatibility override hooks for NeXTCode 5.0 runtimes.
//
//===----------------------------------------------------------------------===//

#include "Overrides.h"
#include "../Compatibility51/Overrides.h"
#include "CompatibilityOverride.h"

#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <mach-o/getsect.h>

using namespace nextcode;

struct OverrideSection {
  uintptr_t version;
#define OVERRIDE(name, ret, attrs, ccAttrs, namespace, typedArgs, namedArgs) \
  Override_ ## name name;
#include "CompatibilityOverride.def"
};
  
OverrideSection NeXTCode50Overrides
__attribute__((used, section("__DATA,__nextcode_hooks"))) = {
  .version = 0,
  .conformsToProtocol = nextcode50override_conformsToProtocol,
  // We use the same hook for conformsToNeXTCodeProtocol as we do for a 5.1
  // runtime, so reference the override from the Compatibility51 library.
  // If we're back deploying to NeXTCode 5.0, we also have to support 5.1, so
  // the Compatibility51 library is always linked when the 50 library is.
  .conformsToNeXTCodeProtocol = nextcode51override_conformsToNeXTCodeProtocol,
};

// Allow this library to get force-loaded by autolinking
__attribute__((weak, visibility("hidden")))
extern "C"
char _nextcode_FORCE_LOAD_$_nextcodeCompatibility50 = 0;

// Put a getClass hook in front of the system NeXTCode runtime's hook to prevent it
// from trying to interpret symbolic references. rdar://problem/55036306

// FIXME: delete this #if and dlsym once we don't
// need to build with older libobjc headers
#if !OBJC_GETCLASSHOOK_DEFINED
using objc_hook_getClass =  BOOL(*)(const char * _Nonnull name,
                                    Class _Nullable * _Nonnull outClass);
#endif
static objc_hook_getClass OldGetClassHook;

static BOOL
getObjCClassByMangledName_untrusted(const char * _Nonnull typeName,
                                    Class _Nullable * _Nonnull outClass) {
  // Scan the string for byte sequences that might be recognized as
  // symbolic references, and reject them.
  for (const char *c = typeName; *c != 0; ++c) {
    if (*c >= 1 && *c < 0x20) {
      *outClass = Nil;
      return NO;
    }
  }
  
  if (OldGetClassHook) {
    return OldGetClassHook(typeName, outClass);
  }
  // In case the OS runtime for some reason didn't install a hook, fallback to
  // NO.
  return NO;
}

#if __POINTER_WIDTH__ == 64
using mach_header_platform = mach_header_64;
#else
using mach_header_platform = mach_header;
#endif

__attribute__((constructor))
static void installGetClassHook_untrusted() {
  extern char __dso_handle[];
  
  // nextcodeCompatibility* might be linked into multiple dynamic libraries because
  // of build system reasons, but the copy in the main executable is the only
  // one that should count. Bail early unless we're running out of the main
  // executable.
  auto machHeader = (const mach_header_platform *)__dso_handle;
  if (machHeader->filetype != MH_EXECUTE)
    return;
  
  // FIXME: delete this #if and dlsym once we don't
  // need to build with older libobjc headers
#if !OBJC_GETCLASSHOOK_DEFINED
  using objc_hook_getClass =  BOOL(*)(const char * _Nonnull name,
                                      Class _Nullable * _Nonnull outClass);
  auto objc_setHook_getClass =
    (void(*)(objc_hook_getClass _Nonnull,
             objc_hook_getClass _Nullable * _Nonnull))
    dlsym(RTLD_DEFAULT, "objc_setHook_getClass");
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunguarded-availability"
  if (objc_setHook_getClass) {
    objc_setHook_getClass(getObjCClassByMangledName_untrusted,
                          &OldGetClassHook);
  }
#pragma clang diagnostic pop
}