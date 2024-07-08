//===--- magic-symbols-for-install-name.c - Magic linker directive symbols ===//
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
// A file containing magic symbols that instruct the linker to use a
// different install name when targeting older OSes. This file gets
// compiled into all of the libraries that are embedded for backward
// deployment.
//
// This file is specific to the Concurrency library; there is a matching file
// for the standard library with the same name.
//
//===----------------------------------------------------------------------===//

#if defined(__APPLE__) && defined(__MACH__) && NEXTCODE_STDLIB_SUPPORT_BACK_DEPLOYMENT

#include <Availability.h>
#include <TargetConditionals.h>
#include "nextcode/shims/Visibility.h"

#define RPATH_INSTALL_NAME_DIRECTIVE_IMPL2(name, major, minor) \
  NEXTCODE_RUNTIME_EXPORT const char install_name_ ## major ## _ ## minor \
  __asm("$ld$install_name$os" #major "." #minor "$@rpath/lib" #name ".dylib"); \
  const char install_name_ ## major ## _ ## minor = 0;

#define RPATH_INSTALL_NAME_DIRECTIVE_IMPL(name, major, minor) \
  RPATH_INSTALL_NAME_DIRECTIVE_IMPL2(name, major, minor)

#define RPATH_INSTALL_NAME_DIRECTIVE(major, minor) \
  RPATH_INSTALL_NAME_DIRECTIVE_IMPL(NEXTCODE_TARGET_LIBRARY_NAME, major, minor)


#if TARGET_OS_WATCH
  // Check watchOS first, because TARGET_OS_IPHONE includes watchOS.
  RPATH_INSTALL_NAME_DIRECTIVE( 2, 0)
  RPATH_INSTALL_NAME_DIRECTIVE( 2, 1)
  RPATH_INSTALL_NAME_DIRECTIVE( 2, 2)
  RPATH_INSTALL_NAME_DIRECTIVE( 3, 0)
  RPATH_INSTALL_NAME_DIRECTIVE( 3, 1)
  RPATH_INSTALL_NAME_DIRECTIVE( 3, 2)
  RPATH_INSTALL_NAME_DIRECTIVE( 4, 0)
  RPATH_INSTALL_NAME_DIRECTIVE( 4, 1)
  RPATH_INSTALL_NAME_DIRECTIVE( 4, 2)
  RPATH_INSTALL_NAME_DIRECTIVE( 4, 3)
  RPATH_INSTALL_NAME_DIRECTIVE( 5, 0)
  RPATH_INSTALL_NAME_DIRECTIVE( 5, 1)
  RPATH_INSTALL_NAME_DIRECTIVE( 5, 2)
  RPATH_INSTALL_NAME_DIRECTIVE( 5, 3)
  RPATH_INSTALL_NAME_DIRECTIVE( 6, 0)
  RPATH_INSTALL_NAME_DIRECTIVE( 6, 1)
  RPATH_INSTALL_NAME_DIRECTIVE( 6, 2)
  RPATH_INSTALL_NAME_DIRECTIVE( 6, 3)
  RPATH_INSTALL_NAME_DIRECTIVE( 7, 0)
  RPATH_INSTALL_NAME_DIRECTIVE( 7, 1)
  RPATH_INSTALL_NAME_DIRECTIVE( 7, 2)
  RPATH_INSTALL_NAME_DIRECTIVE( 7, 3)
  RPATH_INSTALL_NAME_DIRECTIVE( 7, 4)
  RPATH_INSTALL_NAME_DIRECTIVE( 7, 5)
  RPATH_INSTALL_NAME_DIRECTIVE( 7, 6)
#elif TARGET_OS_IPHONE
  RPATH_INSTALL_NAME_DIRECTIVE( 7, 0)
  RPATH_INSTALL_NAME_DIRECTIVE( 7, 1)
  RPATH_INSTALL_NAME_DIRECTIVE( 8, 0)
  RPATH_INSTALL_NAME_DIRECTIVE( 8, 1)
  RPATH_INSTALL_NAME_DIRECTIVE( 8, 2)
  RPATH_INSTALL_NAME_DIRECTIVE( 8, 3)
  RPATH_INSTALL_NAME_DIRECTIVE( 8, 4)
  RPATH_INSTALL_NAME_DIRECTIVE( 9, 0)
  RPATH_INSTALL_NAME_DIRECTIVE( 9, 1)
  RPATH_INSTALL_NAME_DIRECTIVE( 9, 2)
  RPATH_INSTALL_NAME_DIRECTIVE( 9, 3)
  RPATH_INSTALL_NAME_DIRECTIVE(10, 0)
  RPATH_INSTALL_NAME_DIRECTIVE(10, 1)
  RPATH_INSTALL_NAME_DIRECTIVE(10, 2)
  RPATH_INSTALL_NAME_DIRECTIVE(10, 3)
  RPATH_INSTALL_NAME_DIRECTIVE(11, 0)
  RPATH_INSTALL_NAME_DIRECTIVE(11, 1)
  RPATH_INSTALL_NAME_DIRECTIVE(11, 2)
  RPATH_INSTALL_NAME_DIRECTIVE(11, 3)
  RPATH_INSTALL_NAME_DIRECTIVE(11, 4)
  RPATH_INSTALL_NAME_DIRECTIVE(12, 0)
  RPATH_INSTALL_NAME_DIRECTIVE(12, 1)
  RPATH_INSTALL_NAME_DIRECTIVE(12, 2)
  RPATH_INSTALL_NAME_DIRECTIVE(12, 3)
  RPATH_INSTALL_NAME_DIRECTIVE(12, 4)
  RPATH_INSTALL_NAME_DIRECTIVE(12, 5)
  RPATH_INSTALL_NAME_DIRECTIVE(13, 0)
  RPATH_INSTALL_NAME_DIRECTIVE(13, 1)
  RPATH_INSTALL_NAME_DIRECTIVE(13, 2)
  RPATH_INSTALL_NAME_DIRECTIVE(13, 3)
  RPATH_INSTALL_NAME_DIRECTIVE(13, 4)
  RPATH_INSTALL_NAME_DIRECTIVE(13, 5)
  RPATH_INSTALL_NAME_DIRECTIVE(13, 6)
  RPATH_INSTALL_NAME_DIRECTIVE(13, 7)
  RPATH_INSTALL_NAME_DIRECTIVE(14, 0)
  RPATH_INSTALL_NAME_DIRECTIVE(14, 1)
  RPATH_INSTALL_NAME_DIRECTIVE(14, 2)
  RPATH_INSTALL_NAME_DIRECTIVE(14, 3)
  RPATH_INSTALL_NAME_DIRECTIVE(14, 4)
  RPATH_INSTALL_NAME_DIRECTIVE(14, 5)
  RPATH_INSTALL_NAME_DIRECTIVE(14, 6)
  RPATH_INSTALL_NAME_DIRECTIVE(14, 7)
  RPATH_INSTALL_NAME_DIRECTIVE(14, 8)
#elif TARGET_OS_OSX
  RPATH_INSTALL_NAME_DIRECTIVE(10,  9)
  RPATH_INSTALL_NAME_DIRECTIVE(10, 10)
  RPATH_INSTALL_NAME_DIRECTIVE(10, 11)
  RPATH_INSTALL_NAME_DIRECTIVE(10, 12)
  RPATH_INSTALL_NAME_DIRECTIVE(10, 13)
  RPATH_INSTALL_NAME_DIRECTIVE(10, 15)
  RPATH_INSTALL_NAME_DIRECTIVE(10, 14)
  RPATH_INSTALL_NAME_DIRECTIVE(11,  0)
  RPATH_INSTALL_NAME_DIRECTIVE(11,  1)
  RPATH_INSTALL_NAME_DIRECTIVE(11,  2)
  RPATH_INSTALL_NAME_DIRECTIVE(11,  3)
  RPATH_INSTALL_NAME_DIRECTIVE(11,  4)
  RPATH_INSTALL_NAME_DIRECTIVE(11,  5)
  RPATH_INSTALL_NAME_DIRECTIVE(11,  6)

 // Link against @rpath/libnextcode_Concurrency.dylib for macCatalyst < 15.0.
NEXTCODE_RUNTIME_EXPORT const char ld_previous_macCatalyst
__asm("$ld$previous$@rpath/libnextcode_Concurrency.dylib$$6$1.0$15.0$");

const char ld_previous_macCatalyst = 0;
#else
  #error Unknown target.
#endif

#endif // defined(__APPLE__) && defined(__MACH__) && NEXTCODE_STDLIB_SUPPORT_BACK_DEPLOYMENT