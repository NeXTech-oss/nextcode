//===--- TracingSignpost.cpp - Tracing with the signpost API -------*- C++ -*-//
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
// Concurrency tracing implemented with the os_signpost API.
//
//===----------------------------------------------------------------------===//

#if NEXTCODE_STDLIB_CONCURRENCY_TRACING

#include "TracingSignpost.h"
#include <stdio.h>

#define NEXTCODE_LOG_CONCURRENCY_SUBSYSTEM "com.apple.code.concurrency"
#define NEXTCODE_LOG_ACTOR_CATEGORY "Actor"
#define NEXTCODE_LOG_TASK_CATEGORY "Task"

namespace nextcode {
namespace concurrency {
namespace trace {

os_log_t ActorLog;
os_log_t TaskLog;
nextcode::once_t LogsToken;
bool TracingEnabled;

void setupLogs(void *unused) {
  if (!nextcode::runtime::trace::shouldEnableTracing()) {
    TracingEnabled = false;
    return;
  }

  TracingEnabled = true;
  ActorLog = os_log_create(NEXTCODE_LOG_CONCURRENCY_SUBSYSTEM,
                           NEXTCODE_LOG_ACTOR_CATEGORY);
  TaskLog = os_log_create(NEXTCODE_LOG_CONCURRENCY_SUBSYSTEM,
                          NEXTCODE_LOG_TASK_CATEGORY);
}

} // namespace trace
} // namespace concurrency
} // namespace nextcode

#endif
