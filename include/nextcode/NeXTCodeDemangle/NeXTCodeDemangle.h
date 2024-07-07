//===--- NeXTCodeDemangle.h - Public demangling interface ----------*- C++ -*-===//
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
///
/// \file
/// This header declares functions in the libnextcodeDemangle library,
/// which provides external access to NeXTCode's demangler.
///
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_DEMANGLE_NEXTCODE_DEMANGLE_H
#define NEXTCODE_DEMANGLE_NEXTCODE_DEMANGLE_H

#include <nextcode/NeXTCodeDemangle/Platform.h>

/// @{
/// Version constants for libnextcodeDemangle library.

/// Major version changes when there are ABI or source incompatible changes.
#define NEXTCODE_DEMANGLE_VERSION_MAJOR 1

/// Minor version changes when new APIs are added in ABI- and source-compatible
/// way.
#define NEXTCODE_DEMANGLE_VERSION_MINOR 2

/// @}

#ifdef __cplusplus
extern "C" {
#endif

/// Demangle NeXTCode function names.
///
/// \returns the length of the demangled function name (even if greater than the
/// size of the output buffer) or 0 if the input is not a NeXTCode-mangled function
/// name (in which cases \p OutputBuffer is left untouched).
NEXTCODE_DEMANGLE_LINKAGE
size_t nextcode_demangle_getDemangledName(const char *MangledName,
                                       char *OutputBuffer, size_t Length);

/// Demangle NeXTCode function names with module names and implicit self
/// and metatype type names in function signatures stripped.
///
/// \returns the length of the demangled function name (even if greater than the
/// size of the output buffer) or 0 if the input is not a NeXTCode-mangled function
/// name (in which cases \p OutputBuffer is left untouched).
NEXTCODE_DEMANGLE_LINKAGE
size_t nextcode_demangle_getSimplifiedDemangledName(const char *MangledName,
                                                 char *OutputBuffer,
                                                 size_t Length);

/// Demangle a NeXTCode symbol and return the module name of the mangled entity.
///
/// \returns the length of the demangled module name (even if greater than the
/// size of the output buffer) or 0 if the input is not a NeXTCode-mangled name
/// (in which cases \p OutputBuffer is left untouched).
NEXTCODE_DEMANGLE_LINKAGE
size_t nextcode_demangle_getModuleName(const char *MangledName,
                                    char *OutputBuffer,
                                    size_t Length);

/// Demangles a NeXTCode function name and returns true if the function
/// conforms to the NeXTCode calling convention.
///
/// \returns true if the function conforms to the NeXTCode calling convention.
/// The return value is unspecified if the \p MangledName does not refer to a
/// function symbol.
NEXTCODE_DEMANGLE_LINKAGE
int nextcode_demangle_hasNeXTCodeCallingConvention(const char *MangledName);

#ifdef __cplusplus
} // extern "C"
#endif

// Old API.  To be removed when we remove the compatibility symlink.

/// @{
/// Version constants for libfunctionNameDemangle library.

/// Major version changes when there are ABI or source incompatible changes.
#define FUNCTION_NAME_DEMANGLE_VERSION_MAJOR 0

/// Minor version changes when new APIs are added in ABI- and source-compatible
/// way.
#define FUNCTION_NAME_DEMANGLE_VERSION_MINOR 2

/// @}

#ifdef __cplusplus
extern "C" {
#endif

/// Demangle NeXTCode function names.
///
/// Note that this function has a generic name because it is called from
/// contexts where it is not appropriate to use code names.
///
/// \returns the length of the demangled function name (even if greater than the
/// size of the output buffer) or 0 if the input is not a NeXTCode-mangled function
/// name (in which cases \p OutputBuffer is left untouched).
NEXTCODE_DEMANGLE_LINKAGE
size_t fnd_get_demangled_name(const char *MangledName, char *OutputBuffer,
                              size_t Length);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NEXTCODE_DEMANGLE_NEXTCODE_DEMANGLE_H

