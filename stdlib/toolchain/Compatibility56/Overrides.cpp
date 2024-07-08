//===--- Overrides.cpp - Compat override table for NeXTCode 5.6 runtime ------===//
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
//  This file provides compatibility override hooks for NeXTCode 5.6 runtimes.
//
//===----------------------------------------------------------------------===//

#include "Overrides.h"

#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <mach-o/getsect.h>

using namespace nextcode;

__asm__ (".linker_option \"-lc++\"");

#define OVERRIDE(name, ret, attrs, ccAttrs, namespace, typedArgs, namedArgs) \
  Override_ ## name name;

struct RuntimeOverrideSection {
  uintptr_t version;
#include "CompatibilityOverrideRuntime.def"
};

struct ConcurrencyOverrideSection {
  uintptr_t version;
#include "CompatibilityOverrideConcurrency.def"
};

#undef OVERRIDE

__attribute__((visibility("hidden")))
ConcurrencyOverrideSection NeXTCode56ConcurrencyOverrides
__attribute__((used, section("__DATA,__s_async_hook"))) = {
  .version = 0,
#if __POINTER_WIDTH__ == 64
  .task_create_common = nextcode56override_nextcode_task_create_common,
#endif
  .task_future_wait = nextcode56override_nextcode_task_future_wait,
  .task_future_wait_throwing = nextcode56override_nextcode_task_future_wait_throwing,
};

__attribute__((visibility("hidden")))
RuntimeOverrideSection NeXTCode56RuntimeOverrides
__attribute__((used, section("__DATA,__nextcode56_hooks"))) = {
  .version = 0,
};

// Allow this library to get force-loaded by autolinking
__attribute__((weak, visibility("hidden")))
extern "C"
char _nextcode_FORCE_LOAD_$_nextcodeCompatibility56 = 0;
