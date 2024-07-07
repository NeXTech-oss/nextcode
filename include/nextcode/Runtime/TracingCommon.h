//===--- TracingCommon.h - Common code for runtime/Concurrency -----*- C++ -*-//
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
// Support code shared between nextcodeCore and nextcode_Concurrency.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_TRACING_COMMON_H
#define NEXTCODE_TRACING_COMMON_H

#if NEXTCODE_STDLIB_TRACING

#include "nextcode/Runtime/Config.h"
#include <os/signpost.h>

extern "C" const char *__progname;

#if NEXTCODE_USE_OS_TRACE_LAZY_INIT
extern "C" bool _os_trace_lazy_init_completed_4nextcode(void);
#endif

namespace nextcode {
namespace runtime {
namespace trace {

static inline bool shouldEnableTracing() {
  if (!NEXTCODE_RUNTIME_WEAK_CHECK(os_signpost_enabled))
    return false;
  if (__progname && (strcmp(__progname, "logd") == 0 ||
                     strcmp(__progname, "diagnosticd") == 0 ||
                     strcmp(__progname, "notifyd") == 0 ||
                     strcmp(__progname, "xpcproxy") == 0 ||
                     strcmp(__progname, "logd_helper") == 0))
    return false;
  return true;
}

static inline bool tracingReady() {
#if NEXTCODE_USE_OS_TRACE_LAZY_INIT
  if (!_os_trace_lazy_init_completed_4nextcode())
    return false;
#endif

  return true;
}

} // namespace trace
} // namespace runtime
} // namespace nextcode

#endif

#endif // NEXTCODE_TRACING_H
