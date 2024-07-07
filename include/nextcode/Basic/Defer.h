//===--- Defer.h - 'defer' helper macro -------------------------*- C++ -*-===//
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
// This file defines a 'NEXTCODE_DEFER' macro for performing a cleanup on any exit
// out of a scope.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_DEFER_H
#define NEXTCODE_BASIC_DEFER_H

#include "llvm/ADT/ScopeExit.h"

namespace nextcode {
  namespace detail {
    struct DeferTask {};
    template<typename F>
    auto operator+(DeferTask, F &&fn) ->
        decltype(llvm::make_scope_exit(std::forward<F>(fn))) {
      return llvm::make_scope_exit(std::forward<F>(fn));
    }
  }
} // end namespace nextcode


#define DEFER_CONCAT_IMPL(x, y) x##y
#define DEFER_MACRO_CONCAT(x, y) DEFER_CONCAT_IMPL(x, y)

/// This macro is used to register a function / lambda to be run on exit from a
/// scope.  Its typical use looks like:
///
///   NEXTCODE_DEFER {
///     stuff
///   };
///
#define NEXTCODE_DEFER                                                            \
  auto DEFER_MACRO_CONCAT(defer_func, __COUNTER__) =                           \
      ::nextcode::detail::DeferTask() + [&]()

#endif // NEXTCODE_BASIC_DEFER_H
