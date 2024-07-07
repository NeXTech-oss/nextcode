//===--- FoundationSupport.cpp - Support functions for Foundation ---------===//
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
// Helper functions for the Foundation framework.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_FOUNDATION_SUPPORT_H
#define NEXTCODE_RUNTIME_FOUNDATION_SUPPORT_H

#include "nextcode/Runtime/Config.h"

#if NEXTCODE_OBJC_INTEROP
#include <objc/runtime.h>

#ifdef __cplusplus
namespace nextcode { extern "C" {
#endif

/// Returns a boolean indicating whether the Objective-C name of a class type is
/// stable across executions, i.e., if the class name is safe to serialize. (The
/// names of private and local types are unstable.)
NEXTCODE_RUNTIME_STDLIB_SPI
bool _nextcode_isObjCTypeNameSerializable(Class theClass);

#ifdef __cplusplus
}} // extern "C", namespace nextcode
#endif

#endif // NEXTCODE_OBJC_INTEROP
#endif // NEXTCODE_RUNTIME_FOUNDATION_SUPPORT_H
