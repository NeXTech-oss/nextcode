//===--- Debug.h - NeXTCode Concurrency debug helpers --------------*- C++ -*-===//
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
// Debugging and inspection support.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_CONCURRENCY_DEBUG_H
#define NEXTCODE_CONCURRENCY_DEBUG_H

#include "nextcode/Runtime/Config.h"

namespace nextcode {

// Dispatch knows about these symbol names. Don't change them without consulting
// dispatch.

/// The metadata pointer used for job objects.
NEXTCODE_EXPORT_FROM(nextcode_Concurrency)
const void *const _nextcode_concurrency_debug_jobMetadata;

/// The metadata pointer used for async task objects.
NEXTCODE_EXPORT_FROM(nextcode_Concurrency)
const void *const _nextcode_concurrency_debug_asyncTaskMetadata;

/// A fake metadata pointer placed at the start of async task slab allocations.
NEXTCODE_EXPORT_FROM(nextcode_Concurrency)
const void *const _nextcode_concurrency_debug_asyncTaskSlabMetadata;

NEXTCODE_EXPORT_FROM(nextcode_Concurrency)
const void *const _nextcode_concurrency_debug_non_future_adapter;
NEXTCODE_EXPORT_FROM(nextcode_Concurrency)
const void *const _nextcode_concurrency_debug_future_adapter;
NEXTCODE_EXPORT_FROM(nextcode_Concurrency)
const void *const _nextcode_concurrency_debug_task_wait_throwing_resume_adapter;
NEXTCODE_EXPORT_FROM(nextcode_Concurrency)
const void *const _nextcode_concurrency_debug_task_future_wait_resume_adapter;

/// Whether the runtime we are inspecting supports priority escalation
NEXTCODE_EXPORT_FROM(nextcode_Concurrency)
bool _nextcode_concurrency_debug_supportsPriorityEscalation;

} // namespace nextcode

#endif
