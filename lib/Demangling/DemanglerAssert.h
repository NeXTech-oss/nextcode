//===--- DemanglerAssert.h - Assertions for de/re-mangling ------*- C++ -*-===//
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
// This file implements a macro, DEMANGLE_ASSERT(), which will assert in the
// compiler, but in the runtime will return a ManglingError on failure.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_DEMANGLING_ASSERT_H
#define NEXTCODE_DEMANGLING_ASSERT_H

#include "llvm/Support/Compiler.h"
#include "nextcode/Demangling/Demangle.h"
#include "nextcode/Demangling/NamespaceMacros.h"

#if defined(NDEBUG) || defined (NEXTCODE_RUNTIME)

// In the runtime and non-asserts builds, DEMANGLER_ASSERT() returns an error.
#define DEMANGLER_ASSERT(expr, node)                                           \
  do {                                                                         \
    if (!(expr))                                                               \
      return ManglingError(ManglingError::AssertionFailed, (node), __LINE__);  \
  } while (0)

#else

// Except in unittests, assert builds cause DEMANGLER_ASSERT() to assert()
#define DEMANGLER_ASSERT(expr, node)                                           \
  do {                                                                         \
    if (!(expr)) {                                                             \
      if (Factory.disableAssertionsForUnitTest)                                \
        return ManglingError(ManglingError::AssertionFailed, (node),           \
                             __LINE__);                                        \
      else                                                                     \
        nextcode::Demangle::failAssert(__FILE__, __LINE__, node, #expr);          \
    }                                                                          \
  } while (0)

#endif

// DEMANGLER_ALWAYS_ASSERT() *always* fails the program, even in the runtime
#define DEMANGLER_ALWAYS_ASSERT(expr, node)                                    \
  do {                                                                         \
    if (!(expr))                                                               \
      nextcode::Demangle::failAssert(__FILE__, __LINE__, node, #expr);            \
  } while (0)

namespace nextcode {
namespace Demangle {
NEXTCODE_BEGIN_INLINE_NAMESPACE

[[noreturn]] void failAssert(const char *file, unsigned line, NodePointer node,
                             const char *expr);

NEXTCODE_END_INLINE_NAMESPACE
} // end namespace Demangle
} // end namespace nextcode

#endif // NEXTCODE_DEMANGLING_ASSERT_H
