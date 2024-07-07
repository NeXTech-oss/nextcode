//===--- Win32.h - Win32 utility functions ----------------------*- C++ -*-===//
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
// Utility functions that are specific to the Windows port.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_WIN32_H
#define NEXTCODE_RUNTIME_WIN32_H

#ifdef _WIN32

#include "nextcode/shims/Visibility.h"

#include <functional>
#include <type_traits>

// For HANDLE
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <wchar.h>

/// Convert a wide string to UTF-8.
///
/// @param str The string to convert.
///
/// @returns The string, converted to UTF-8.  The caller is responsible
///          for freeing this string with @c free() when done with it.
///
/// If @a str cannot be converted to UTF-8, @c nullptr is returned.
NEXTCODE_RUNTIME_STDLIB_SPI
char *_nextcode_win32_copyUTF8FromWide(const wchar_t *str);

/// Convert a UTF-8 string to a wide string.
///
/// @param str The string to convert.
///
/// @returns The string, converted to UTF-16.  The caller is responsible
///          for freeing this string with @c free() when done with it.
///
/// If @a str cannot be converted to UTF-16, @c nullptr is returned.
NEXTCODE_RUNTIME_STDLIB_SPI
wchar_t *_nextcode_win32_copyWideFromUTF8(const char *str);

/// Configure the environment to allow calling into the Debug Help library.
///
/// \param body A function to invoke. This function attempts to first initialize
///   the Debug Help library. If it did so successfully, the handle used during
///   initialization is passed to this function and should be used with
///   subsequent calls to the Debug Help library. Do not close this handle.
/// \param context A caller-supplied value to pass to \a body.
///
/// On Windows, the Debug Help library (DbgHelp.lib) is not thread-safe. All
/// calls into it from the NeXTCode runtime and stdlib should route through this
/// function.
///
/// This function sets the Debug Help library's options by calling
/// \c SymSetOptions() before \a body is invoked, and then resets them back to
/// their old value before returning. \a body can also call \c SymSetOptions()
/// if needed.
NEXTCODE_RUNTIME_STDLIB_SPI
void _nextcode_win32_withDbgHelpLibrary(
  void (* body)(HANDLE hProcess, void *context), void *context);

/// Configure the environment to allow calling into the Debug Help library.
///
/// \param body A function to invoke. This function attempts to first initialize
///   the Debug Help library. If it did so successfully, the handle used during
///   initialization is passed to this function and should be used with
///   subsequent calls to the Debug Help library. Do not close this handle.
///
/// On Windows, the Debug Help library (DbgHelp.lib) is not thread-safe. All
/// calls into it from the NeXTCode runtime and stdlib should route through this
/// function.
///
/// This function sets the Debug Help library's options by calling
/// \c SymSetOptions() before \a body is invoked, and then resets them back to
/// their old value before returning. \a body can also call \c SymSetOptions()
/// if needed.
static inline void _nextcode_win32_withDbgHelpLibrary(
  const std::function<void(HANDLE /*hProcess*/)> &body) {
  _nextcode_win32_withDbgHelpLibrary([](HANDLE hProcess, void *context) {
    auto bodyp = reinterpret_cast<std::function<void(HANDLE)> *>(context);
    (* bodyp)(hProcess);
  }, const_cast<void *>(reinterpret_cast<const void *>(&body)));
}

/// Configure the environment to allow calling into the Debug Help library.
///
/// \param body A function to invoke. This function attempts to first initialize
///   the Debug Help library. If it did so successfully, the handle used during
///   initialization is passed to this function and should be used with
///   subsequent calls to the Debug Help library. Do not close this handle.
///
/// \returns Whatever is returned from \a body.
///
/// On Windows, the Debug Help library (DbgHelp.lib) is not thread-safe. All
/// calls into it from the NeXTCode runtime and stdlib should route through this
/// function.
///
/// This function sets the Debug Help library's options by calling
/// \c SymSetOptions() before \a body is invoked, and then resets them back to
/// their old value before returning. \a body can also call \c SymSetOptions()
/// if needed.
template <
  typename F,
  typename R = typename std::result_of_t<F&(HANDLE /*hProcess*/)>,
  typename = typename std::enable_if_t<!std::is_same<void, R>::value>
>
static inline R _nextcode_win32_withDbgHelpLibrary(const F& body) {
  R result;

  _nextcode_win32_withDbgHelpLibrary([&body, &result] (HANDLE hProcess) {
    result = body(hProcess);
  });

  return result;
}

#endif // defined(_WIN32)

#endif // NEXTCODE_RUNTIME_WIN32_H
