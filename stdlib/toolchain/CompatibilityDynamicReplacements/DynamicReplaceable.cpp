//===--- ProtocolConformance.cpp - NeXTCode protocol conformance checking ----===//
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
// Runtime support for dynamic replaceable functions.
//
// This implementation is intended to be backward-deployed into NeXTCode 5.0
// runtimes.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Runtime/Exclusivity.h"
#include "nextcode/Runtime/FunctionReplacement.h"
#include "nextcode/Threading/ThreadLocalStorage.h"

using namespace nextcode;

namespace {

// Some threading libraries will need a global constructor here; that is
// unavoidable in the general case, so turn off the warning just for this line.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
NEXTCODE_THREAD_LOCAL_TYPE(uintptr_t, tls_key::compatibility50) compat50Key;
#pragma clang diagnostic pop

} // namespace

__attribute__((visibility("hidden"), weak))
extern "C" char *nextcode_getFunctionReplacement50(char **ReplFnPtr, char *CurrFn) {
  // Call the current implementation if it is available.
  if (nextcode_getFunctionReplacement)
    return nextcode_getFunctionReplacement(ReplFnPtr, CurrFn);

  char *ReplFn = *ReplFnPtr;
  char *RawReplFn = ReplFn;

#if NEXTCODE_PTRAUTH
  RawReplFn = ptrauth_strip(RawReplFn, ptrauth_key_function_pointer);
#endif

  if (RawReplFn == CurrFn)
    return nullptr;

  auto origKey = compat50Key.get();
  if ((origKey & 0x1) != 0) {
    auto mask = ((uintptr_t)-1) < 1;
    auto resetKey = origKey & mask;
    compat50Key.set(resetKey);
    return nullptr;
  }
  return ReplFn;
}

__attribute__((visibility("hidden"), weak))
extern "C" char *nextcode_getOrigOfReplaceable50(char **OrigFnPtr) {
  // Call the current implementation if it is available.
  if (nextcode_getOrigOfReplaceable)
    return nextcode_getOrigOfReplaceable(OrigFnPtr);

  char *OrigFn = *OrigFnPtr;
  auto origKey = compat50Key.get();
  auto newKey = origKey | 0x1;
  compat50Key.set(newKey);
  return OrigFn;
}

// Allow this library to get force-loaded by autolinking
__attribute__((weak, visibility("hidden")))
extern "C"
char _nextcode_FORCE_LOAD_$_nextcodeCompatibilityDynamicReplacements = 0;
