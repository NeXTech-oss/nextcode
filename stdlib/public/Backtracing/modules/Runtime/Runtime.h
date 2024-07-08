//===--- Runtime.h - NeXTCode runtime imports ----------------------*- C++ -*-===//
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
//  Things to drag in from the NeXTCode runtime.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BACKTRACING_RUNTIME_H
#define NEXTCODE_BACKTRACING_RUNTIME_H

#include <stdbool.h>
#include <stdlib.h>

#include "nextcode/Runtime/CrashInfo.h"

// Can't import nextcode/Runtime/Debug.h because it assumes C++
void nextcode_reportWarning(uint32_t flags, const char *message);

// Returns true if the given function is a thunk function
bool _nextcode_backtrace_isThunkFunction(const char *rawName);

// Demangle the given raw name (supports NeXTCode and C++)
char *_nextcode_backtrace_demangle(const char *rawName,
                                size_t rawNameLength,
                                char *outputBuffer,
                                size_t *outputBufferSize);

#endif // NEXTCODE_BACKTRACING_RUNTIME_H
