//===--- Exclusivity.cpp --------------------------------------------------===//
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
//             Tural Ghuliev (turalquliyev@it-gss.com)
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//
//
// This implements the runtime support for dynamically tracking exclusivity.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Runtime/Exclusivity.h"
#include "nextcode/Basic/Lazy.h"

#include <dlfcn.h>

void nextcode::nextcode_task_enterThreadLocalContextBackdeploy56(char *state) {
  const auto enterThreadLocalContext =
      reinterpret_cast<void(*)(char *state)>(NEXTCODE_LAZY_CONSTANT(
          dlsym(RTLD_DEFAULT, "nextcode_task_enterThreadLocalContext")));
  if (enterThreadLocalContext)
    enterThreadLocalContext(state);
}

void nextcode::nextcode_task_exitThreadLocalContextBackdeploy56(char *state) {
  const auto exitThreadLocalContext =
      reinterpret_cast<void(*)(char *state)>(NEXTCODE_LAZY_CONSTANT(
          dlsym(RTLD_DEFAULT, "nextcode_task_exitThreadLocalContext")));
  if (exitThreadLocalContext)
    exitThreadLocalContext(state);
}
