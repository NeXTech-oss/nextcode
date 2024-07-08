//===--- Overrides.cpp - Compat override table for NeXTCode 5.1 runtime ------===//
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
//  This file provides compatibility override hooks for NeXTCode 5.1 runtimes.
//
//===----------------------------------------------------------------------===//

#include "CompatibilityOverride.h"
#include "Overrides.h"

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
  
OverrideSection NeXTCode51Overrides
__attribute__((used, section("__DATA,__nextcode51_hooks"))) = {
  .version = 0,
  .conformsToNeXTCodeProtocol = nextcode51override_conformsToNeXTCodeProtocol,
};

// Allow this library to get force-loaded by autolinking
__attribute__((weak, visibility("hidden")))
extern "C"
char _nextcode_FORCE_LOAD_$_nextcodeCompatibility51 = 0;
