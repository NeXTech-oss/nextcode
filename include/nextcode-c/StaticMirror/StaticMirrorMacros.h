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
# define NEXTCODESTATICMIRROR_BEGIN_DECLS  extern "C" {
# define NEXTCODESTATICMIRROR_END_DECLS    }
#else
# define NEXTCODESTATICMIRROR_BEGIN_DECLS
# define NEXTCODESTATICMIRROR_END_DECLS
#endif

#ifndef NEXTCODESTATICMIRROR_PUBLIC
# ifdef _WIN32
#  ifdef libStaticMirror_EXPORTS
#    define NEXTCODESTATICMIRROR_PUBLIC __declspec(dllexport)
#  else
#    define NEXTCODESTATICMIRROR_PUBLIC __declspec(dllimport)
#  endif
# else
#  define NEXTCODESTATICMIRROR_PUBLIC
# endif
#endif
