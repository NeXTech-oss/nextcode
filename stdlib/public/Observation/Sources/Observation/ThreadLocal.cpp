//===----------------------------------------------------------------------===//
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

#include "nextcode/Runtime/Config.h"
#include "nextcode/Runtime/Once.h"
#include "nextcode/Threading/ThreadLocalStorage.h"

static NEXTCODE_THREAD_LOCAL_TYPE(void *, nextcode::tls_key::observation_transaction) Value;

extern "C" NEXTCODE_CC(nextcode) __attribute__((visibility("hidden")))
void *_nextcode_observation_tls_get() {
  return Value.get();
}

extern "C" NEXTCODE_CC(nextcode) __attribute__((visibility("hidden")))
void _nextcode_observation_tls_set(void *value) {
  Value.set(value);
}
