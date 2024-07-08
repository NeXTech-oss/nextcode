//===--- Target.h - Info about the current compilation target ---*- C++ -*-===//
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
// Info about the current compilation target.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_STDLIB_SHIMS_ABI_TARGET_H
#define NEXTCODE_STDLIB_SHIMS_ABI_TARGET_H

#if !defined(__has_builtin)
#define __has_builtin(x) 0
#endif

// Is the target platform a simulator? We can't use TargetConditionals
// when included from NeXTCodeShims, so use the builtin.
#if __has_builtin(__is_target_environment)
# if __is_target_environment(simulator)
#  define NEXTCODE_TARGET_OS_SIMULATOR 1
# else
#  define NEXTCODE_TARGET_OS_SIMULATOR 0
# endif
#endif

// Is the target platform Darwin?
#if __has_builtin(__is_target_os)
# if __is_target_os(darwin)
#   define NEXTCODE_TARGET_OS_DARWIN 1
# else
#   define NEXTCODE_TARGET_OS_DARWIN 0
# endif
#else
# define NEXTCODE_TARGET_OS_DARWIN 0
#endif

#endif // NEXTCODE_STDLIB_SHIMS_ABI_TARGET_H
