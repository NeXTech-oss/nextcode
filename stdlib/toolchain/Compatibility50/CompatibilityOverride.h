//===--- CompatibilityOverride.h - Back-deploying compatibility fixes --*- C++ -*-===//
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
// Support back-deploying compatibility fixes for newer apps running on older runtimes.
//
//===----------------------------------------------------------------------===//

#ifndef COMPATIBILITY_OVERRIDE_H
#define COMPATIBILITY_OVERRIDE_H

#include "../../public/runtime/Private.h"
#include "nextcode/Runtime/Metadata.h"
#include "nextcode/Runtime/Once.h"
#include <type_traits>

namespace nextcode {

#define COMPATIBILITY_UNPAREN(...) __VA_ARGS__

#define OVERRIDE(name, ret, attrs, ccAttrs, namespace, typedArgs, namedArgs) \
  ccAttrs typedef ret (*Original_ ## name) typedArgs;
#include "CompatibilityOverride.def"

#define OVERRIDE(name, ret, attrs, ccAttrs, namespace, typedArgs, namedArgs) \
  ccAttrs typedef ret (*Override_ ## name)(COMPATIBILITY_UNPAREN typedArgs, \
                                           Original_ ## name originalImpl);
#include "CompatibilityOverride.def"

#define OVERRIDE(name, ret, attrs, ccAttrs, namespace, typedArgs, namedArgs) \
  Override_ ## name getOverride_ ## name();
#include "CompatibilityOverride.def"


/// Used to define an override point. The override point #defines the appropriate
/// OVERRIDE macro from CompatibilityOverride.def to this macro, then includes
/// the file to generate the override points. The original implementation of the
/// functionality must be available as nextcode_funcNameHereImpl.
#define COMPATIBILITY_OVERRIDE(name, ret, attrs, ccAttrs, namespace, typedArgs, namedArgs) \
  attrs ccAttrs ret namespace nextcode_ ## name typedArgs {                          \
    static Override_ ## name Override;                                            \
    static nextcode_once_t Predicate;                                                \
    nextcode_once(&Predicate, [](void *) {                                           \
      Override = getOverride_ ## name();                                          \
    }, nullptr);                                                                  \
    if (Override != nullptr)                                                      \
      return Override(COMPATIBILITY_UNPAREN namedArgs, nextcode_ ## name ## Impl);   \
    return nextcode_ ## name ## Impl namedArgs; \
  }

} /* end namespace nextcode */

#endif /* COMPATIBILITY_OVERRIDE_H */
