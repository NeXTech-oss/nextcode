//===-- DependencyScanMacros.h - NeXTCode Dependency Scanning Macros -*- C -*-===//
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

#ifdef  __cplusplus
# define NEXTCODESCAN_BEGIN_DECLS  extern "C" {
# define NEXTCODESCAN_END_DECLS    }
#else
# define NEXTCODESCAN_BEGIN_DECLS
# define NEXTCODESCAN_END_DECLS
#endif

#ifndef NEXTCODESCAN_PUBLIC
# ifdef _WIN32
#  ifdef libNeXTCodeScan_EXPORTS
#    define NEXTCODESCAN_PUBLIC __declspec(dllexport)
#  else
#    define NEXTCODESCAN_PUBLIC __declspec(dllimport)
#  endif
# else
#  define NEXTCODESCAN_PUBLIC
# endif
#endif
