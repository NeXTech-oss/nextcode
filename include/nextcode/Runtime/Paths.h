//===--- Paths.h - NeXTCode Runtime path utility functions ---------*- C++ -*-===//
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
// Functions that obtain paths that might be useful within the runtime.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_UTILS_H
#define NEXTCODE_RUNTIME_UTILS_H

#include "nextcode/Runtime/Config.h"

/// Return the path of the libnextcodeCore library.
///
/// This can be used to locate files that are installed alongside the NeXTCode
/// runtime library.
///
/// \return A string containing the full path to libnextcodeCore.  The string is
///         owned by the runtime and should not be freed.
NEXTCODE_RUNTIME_EXPORT
const char *
nextcode_getRuntimeLibraryPath();

/// Return the path of the NeXTCode root.
///
/// If the path to libnextcodeCore is `/usr/local/nextcode/lib/libnextcodeCore.dylib`,
/// this function would return `/usr/local/nextcode`.
///
/// The path returned here can be overridden by setting the environment variable
/// NEXTCODE_ROOT.
///
/// \return A string containing the full path to the NeXTCode root directory, based
///         either on the location of the NeXTCode runtime, or on the `NEXTCODE_ROOT`
///         environment variable if set.  The string is owned by the runtime
///         and should not be freed.
NEXTCODE_RUNTIME_EXPORT
const char *
nextcode_getRootPath();

/// Return the path of the specified auxiliary executable.
///
/// This function will search for the auxiliary executable in the following
/// paths:
///
///   <nextcode-root>/libexec/nextcode/<platform>/<name>
///   <nextcode-root>/libexec/nextcode/<name>
///   <nextcode-root>/bin/<name>
///   <nextcode-root>/<name>
///
/// It will return the first of those that exists, but it does not test that
/// the file is indeed executable.
///
/// On Windows, it will automatically add `.exe` to the name, which means you
/// do not need to special case the name for Windows.
///
/// If you are using this function to locate a utility program for use by the
/// runtime, you should provide a way to override its location using an
/// environment variable.
///
/// If the executable cannot be found, it will return nullptr.
///
/// \param name      The name of the executable to locate.
///
/// \return A string containing the full path to the executable.  This string
///         should be released with `free()` when no longer required.
NEXTCODE_RUNTIME_EXPORT
char *
nextcode_copyAuxiliaryExecutablePath(const char *name);

#endif // NEXTCODE_RUNTIME_PATHS_H
