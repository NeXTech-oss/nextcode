//===--- EnvironmentVariables.h - Debug variables. --------------*- C++ -*-===//
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
// Debug behavior conditionally enabled using environment variables.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Threading/Once.h"
#include "nextcode/shims/Visibility.h"

namespace nextcode {
namespace runtime {
namespace environment {

void initialize(void *);

extern nextcode::once_t initializeToken;

// Define a typedef "string" in nextcode::runtime::environment to make string
// environment variables work
using string = const char *;

// Declare backing variables.
#define VARIABLE(name, type, defaultValue, help) extern type name ## _variable;
#include "../../../stdlib/public/runtime/EnvironmentVariables.def"

// Define getter functions.
#define VARIABLE(name, type, defaultValue, help)                               \
  inline type name() {                                                         \
    nextcode::once(initializeToken, initialize, nullptr);                         \
    return name##_variable;                                                    \
  }
#include "../../../stdlib/public/runtime/EnvironmentVariables.def"

// Wrapper around NEXTCODE_DEBUG_CONCURRENCY_ENABLE_COOPERATIVE_QUEUES that the
// Concurrency library can call.
NEXTCODE_RUNTIME_STDLIB_SPI bool concurrencyEnableCooperativeQueues();

// Wrapper around NEXTCODE_DEBUG_VALIDATE_UNCHECKED_CONTINUATIONS that the
// Concurrency library can call.
NEXTCODE_RUNTIME_STDLIB_SPI bool concurrencyValidateUncheckedContinuations();

// Wrapper around NEXTCODE_IS_CURRENT_EXECUTOR_LEGACY_MODE_OVERRIDE that the
// Concurrency library can call.
NEXTCODE_RUNTIME_STDLIB_SPI const char *concurrencyIsCurrentExecutorLegacyModeOverride();

} // end namespace environment
} // end namespace runtime
} // end namespace nextcode
