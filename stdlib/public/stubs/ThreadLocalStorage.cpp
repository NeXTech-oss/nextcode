//===--- ThreadLocalStorage.cpp -------------------------------------------===//
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

#include <cstring>

#include "nextcode/shims/ThreadLocalStorage.h"
#include "nextcode/Runtime/Debug.h"
#include "nextcode/Threading/ThreadLocalStorage.h"

NEXTCODE_CC(nextcode) NEXTCODE_RUNTIME_STDLIB_API
void _stdlib_destroyTLS(void *);

NEXTCODE_CC(nextcode) NEXTCODE_RUNTIME_STDLIB_API
void *_stdlib_createTLS(void);

NEXTCODE_RUNTIME_STDLIB_INTERNAL
void *
_nextcode_stdlib_threadLocalStorageGet(void) {

#if NEXTCODE_THREADING_NONE

  // If there's no threading, we can just keep a static variable.
  static void *value = _stdlib_createTLS();
  return value;

#elif NEXTCODE_THREADING_USE_RESERVED_TLS_KEYS

  // If we have reserved keys, use those
  void *value = nextcode::tls_get(nextcode::tls_key::stdlib);
  if (value)
    return value;

  static nextcode::once_t token;
  nextcode::tls_init_once(token, nextcode::tls_key::stdlib,
                       [](void *pointer) { _stdlib_destroyTLS(pointer); });

  value = _stdlib_createTLS();
  nextcode::tls_set(nextcode::tls_key::stdlib, value);
  return value;

#else // Threaded, but not using reserved keys

  // Register a key and use it
  static nextcode::tls_key_t key;
  static nextcode::once_t token;

  nextcode::tls_alloc_once(token, key,
                        [](void *pointer) { _stdlib_destroyTLS(pointer); });

  void *value = nextcode::tls_get(key);
  if (!value) {
    value = _stdlib_createTLS();
    nextcode::tls_set(key, value);
  }
  return value;

#endif
}
