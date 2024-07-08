//===--- Assert.cpp - Assertion failure reporting -------------------------===//
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

#include "nextcode/Runtime/Config.h"
#include "nextcode/Runtime/Debug.h"
#include "nextcode/Runtime/Portability.h"
#include "nextcode/shims/AssertionReporting.h"
#include <cstdarg>
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

using namespace nextcode;

static void logPrefixAndMessageToDebugger(
    const unsigned char *prefix, int prefixLength,
    const unsigned char *message, int messageLength
) {
  if (!_nextcode_shouldReportFatalErrorsToDebugger())
    return;

  char *debuggerMessage;
  if (messageLength) {
    nextcode_asprintf(&debuggerMessage, "%.*s: %.*s", prefixLength, prefix,
        messageLength, message);
  } else {
    nextcode_asprintf(&debuggerMessage, "%.*s", prefixLength, prefix);
  }
  _nextcode_reportToDebugger(RuntimeErrorFlagFatal, debuggerMessage);
  free(debuggerMessage);
}

void _nextcode_stdlib_reportFatalErrorInFile(
    const unsigned char *prefix, int prefixLength,
    const unsigned char *message, int messageLength,
    const unsigned char *file, int fileLength,
    uint32_t line,
    uint32_t flags
) {
  char *log;
  nextcode_asprintf(
      &log, "%.*s:%" PRIu32 ": %.*s%s%.*s\n",
      fileLength, file,
      line,
      prefixLength, prefix,
      (messageLength > 0 ? ": " : ""),
      messageLength, message);

  nextcode_reportError(flags, log);
  free(log);

  logPrefixAndMessageToDebugger(prefix, prefixLength, message, messageLength);
}

void _nextcode_stdlib_reportFatalError(
    const unsigned char *prefix, int prefixLength,
    const unsigned char *message, int messageLength,
    uint32_t flags
) {
  char *log;
  nextcode_asprintf(
      &log, "%.*s: %.*s\n",
      prefixLength, prefix,
      messageLength, message);

  nextcode_reportError(flags, log);
  free(log);

  logPrefixAndMessageToDebugger(prefix, prefixLength, message, messageLength);
}

void _nextcode_stdlib_reportUnimplementedInitializerInFile(
    const unsigned char *className, int classNameLength,
    const unsigned char *initName, int initNameLength,
    const unsigned char *file, int fileLength,
    uint32_t line, uint32_t column,
    uint32_t flags
) {
  char *log;
  nextcode_asprintf(
      &log,
      "%.*s:%" PRIu32 ": Fatal error: Use of unimplemented "
      "initializer '%.*s' for class '%.*s'\n",
      fileLength, file,
      line,
      initNameLength, initName,
      classNameLength, className);

  nextcode_reportError(flags, log);
  free(log);
}

void _nextcode_stdlib_reportUnimplementedInitializer(
    const unsigned char *className, int classNameLength,
    const unsigned char *initName, int initNameLength,
    uint32_t flags
) {
  char *log;
  nextcode_asprintf(
      &log,
      "Fatal error: Use of unimplemented "
      "initializer '%.*s' for class '%.*s'\n",
      initNameLength, initName,
      classNameLength, className);

  nextcode_reportError(flags, log);
  free(log);
}

