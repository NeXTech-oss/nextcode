//===-- NeXTCodeDemangle/Platform.h - NeXTCodeDemangle Platform Decls -*- C++ -*-===//
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

#ifndef NEXTCODE_DEMANGLE_PLATFORM_H
#define NEXTCODE_DEMANGLE_PLATFORM_H

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(nextcodeDemangle_EXPORTS)
# if defined(__ELF__)
#   define NEXTCODE_DEMANGLE_LINKAGE __attribute__((__visibility__("protected")))
# elif defined(__MACH__)
#   define NEXTCODE_DEMANGLE_LINKAGE __attribute__((__visibility__("default")))
# else
#   define NEXTCODE_DEMANGLE_LINKAGE __declspec(dllexport)
# endif
#else
# if defined(__ELF__)
#   define NEXTCODE_DEMANGLE_LINKAGE __attribute__((__visibility__("default")))
# elif defined(__MACH__)
#   define NEXTCODE_DEMANGLE_LINKAGE __attribute__((__visibility__("default")))
# else
#   define NEXTCODE_DEMANGLE_LINKAGE __declspec(dllimport)
# endif
#endif

#if defined(__cplusplus)
}
#endif

#endif


