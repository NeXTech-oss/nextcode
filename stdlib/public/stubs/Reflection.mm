//===----------------------------------------------------------------------===//
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

#include "nextcode/Runtime/Config.h"

#if NEXTCODE_OBJC_INTEROP
#import <objc/runtime.h>
#import <objc/NSObject.h>

NEXTCODE_CC(nextcode)
NEXTCODE_RUNTIME_STDLIB_API
bool _nextcode_stdlib_NSObject_isKindOfClass(
    id _Nonnull object,
    char * _Nonnull className) {
  Class cls = objc_lookUpClass(className);
  return [object isKindOfClass:cls];
}
#endif

