//===--- Nullability.h ----------------------------------------------------===//
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

#ifndef NEXTCODE_BASIC_NULLABILITY_H
#define NEXTCODE_BASIC_NULLABILITY_H

// TODO: These macro definitions are duplicated in Visibility.h. Move
// them to a single file if we find a location that both Visibility.h and
// BridgedNeXTCodeObject.h can import.
#if __has_feature(nullability)
// Provide macros to temporarily suppress warning about the use of
// _Nullable and _Nonnull.
#define NEXTCODE_BEGIN_NULLABILITY_ANNOTATIONS                                    \
  _Pragma("clang diagnostic push")                                             \
      _Pragma("clang diagnostic ignored \"-Wnullability-extension\"")
#define NEXTCODE_END_NULLABILITY_ANNOTATIONS _Pragma("clang diagnostic pop")

#define NEXTCODE_BEGIN_ASSUME_NONNULL _Pragma("clang assume_nonnull begin")
#define NEXTCODE_END_ASSUME_NONNULL _Pragma("clang assume_nonnull end")
#else
// #define _Nullable and _Nonnull to nothing if we're not being built
// with a compiler that supports them.
#define _Nullable
#define _Nonnull
#define _Null_unspecified
#define NEXTCODE_BEGIN_NULLABILITY_ANNOTATIONS
#define NEXTCODE_END_NULLABILITY_ANNOTATIONS
#define NEXTCODE_BEGIN_ASSUME_NONNULL
#define NEXTCODE_END_ASSUME_NONNULL
#endif

#endif
