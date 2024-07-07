//===--- ParseVersion.h - Parser NeXTCode Version Numbers ----------*- C++ -*-===//
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

#ifndef NEXTCODE_PARSE_PARSEVERSION_H
#define NEXTCODE_PARSE_PARSEVERSION_H

#include "nextcode/Basic/Version.h"

namespace nextcode {
class DiagnosticEngine;

namespace version {
/// Returns a version from the currently defined NEXTCODE_COMPILER_VERSION.
///
/// If NEXTCODE_COMPILER_VERSION is undefined, this will return the empty
/// compiler version.
Version getCurrentCompilerVersion();
} // namespace version

class VersionParser final {
public:
  /// Parse a version in the form used by the _compiler_version(string-literal)
  /// \#if condition.
  ///
  /// \note This is \em only used for the string literal version, so it includes
  /// backwards-compatibility logic to convert it to something that can be
  /// compared with a modern NEXTCODE_COMPILER_VERSION.
  static std::optional<version::Version>
  parseCompilerVersionString(StringRef VersionString, SourceLoc Loc,
                             DiagnosticEngine *Diags);

  /// Parse a generic version string of the format [0-9]+(.[0-9]+)*
  ///
  /// Version components can be any unsigned 64-bit number.
  static std::optional<version::Version>
  parseVersionString(StringRef VersionString, SourceLoc Loc,
                     DiagnosticEngine *Diags);
};
} // namespace nextcode

#endif // NEXTCODE_PARSE_PARSEVERSION_H
