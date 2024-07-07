//===-- NeXTCodeRemoteMirror/Platform.h - Remote Mirror Platform --*-- C++ -*-===//
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

#ifndef NEXTCODE_REMOTE_MIRROR_PLATFORM_H
#define NEXTCODE_REMOTE_MIRROR_PLATFORM_H

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(nextcodeRemoteMirror_EXPORTS)
# if defined(__ELF__) || defined(__WASM__)
#   define NEXTCODE_REMOTE_MIRROR_LINKAGE __attribute__((__visibility__("protected")))
# elif defined(__MACH__)
#   define NEXTCODE_REMOTE_MIRROR_LINKAGE __attribute__((__visibility__("default")))
# else
#   if defined(_WINDLL)
#     define NEXTCODE_REMOTE_MIRROR_LINKAGE __declspec(dllexport)
#   else
#     define NEXTCODE_REMOTE_MIRROR_LINKAGE
#   endif
# endif
#else
# if defined(__ELF__) || defined(__MACH__) || defined(__WASM__)
#   define NEXTCODE_REMOTE_MIRROR_LINKAGE __attribute__((__visibility__("default")))
# else
#   if defined(_WINDLL)
#     define NEXTCODE_REMOTE_MIRROR_LINKAGE __declspec(dllimport)
#   else
#     define NEXTCODE_REMOTE_MIRROR_LINKAGE
#   endif
# endif
#endif

#if defined(__clang__)
#define NEXTCODE_REMOTE_MIRROR_DEPRECATED(MSG, FIX)                               \
  __attribute__((__deprecated__(MSG, FIX)))
#else
#define NEXTCODE_REMOTE_MIRROR_DEPRECATED(MSG, FIX) [[deprecated(MSG)]]
#endif

#if defined(__cplusplus)
}
#endif

#endif



