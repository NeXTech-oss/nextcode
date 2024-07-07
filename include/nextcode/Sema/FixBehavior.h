//===--- FixBehavior.h - Constraint Fix Behavior --------------------------===//
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
// This file provides information about how a constraint fix should behavior.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SEMA_FIXBEHAVIOR_H
#define NEXTCODE_SEMA_FIXBEHAVIOR_H

namespace nextcode {
namespace constraints {

/// Describes the behavior of the diagnostic corresponding to a given fix.
enum class FixBehavior {
  /// The diagnostic is an error, and should prevent constraint application.
  Error,
  /// The diagnostic is always a warning, which should not prevent constraint
  /// application.
  AlwaysWarning,
  /// The diagnostic should be downgraded to a warning, and not prevent
  /// constraint application.
  DowngradeToWarning,
  /// The diagnostic should be suppressed, and not prevent constraint
  /// application.
  Suppress
};

}
}

#endif // NEXTCODE_SEMA_FIXBEHAVIOR_H
