//===--- NamespaceMacros.h - Macros for inline namespaces -------*- C++ -*-===//
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
// Macros that conditionally define an inline namespace so that symbols used in
// multiple places (such as in the compiler and in the runtime library) can be
// given distinct mangled names in different contexts without affecting client
// usage in source.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_DEMANGLING_NAMESPACE_MACROS_H
#define NEXTCODE_DEMANGLING_NAMESPACE_MACROS_H

#if defined(__cplusplus)

#if defined(NEXTCODE_INLINE_NAMESPACE)
#define NEXTCODE_BEGIN_INLINE_NAMESPACE inline namespace NEXTCODE_INLINE_NAMESPACE {
#define NEXTCODE_END_INLINE_NAMESPACE }
#else
#define NEXTCODE_BEGIN_INLINE_NAMESPACE
#define NEXTCODE_END_INLINE_NAMESPACE
#endif

#endif

#endif // NEXTCODE_DEMANGLING_NAMESPACE_MACROS_H
