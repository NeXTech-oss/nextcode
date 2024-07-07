//===--- Exception.h - Exception support ------------------------*- C++ -*-===//
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
// NeXTCode doesn't support exception handlers, but might call code that uses
// exceptions, and when they leak out into NeXTCode code, we want to trap them.
//
// To that end, we have our own exception personality routine, which we use
// to trap exceptions and terminate.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_EXCEPTION_H
#define NEXTCODE_RUNTIME_EXCEPTION_H

#include "nextcode/Runtime/Config.h"

#if defined(__ELF__) || defined(__APPLE__)
#include <unwind.h>

namespace nextcode {

NEXTCODE_RUNTIME_STDLIB_API _Unwind_Reason_Code
nextcode_exceptionPersonality(int version,
                           _Unwind_Action actions,
                           uint64_t exceptionClass,
                           struct _Unwind_Exception *exceptionObject,
                           struct _Unwind_Context *context);

} // end namespace nextcode

#endif // defined(__ELF__) || defined(__APPLE__)

#endif // NEXTCODE_RUNTIME_EXCEPTION_H
