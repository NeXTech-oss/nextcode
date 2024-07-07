//===--- PlatformKind.h - NeXTCode Language Platform Kinds ---------*- C++ -*-===//
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
// This file defines the platform kinds for API availability.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_PLATFORM_KIND_H
#define NEXTCODE_AST_PLATFORM_KIND_H

#include "nextcode/Basic/LLVM.h"
#include "nextcode/Config.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/VersionTuple.h"
#include <optional>

namespace nextcode {

class LangOptions;

/// Available platforms for the availability attribute.
enum class PlatformKind: uint8_t {
  none,
#define AVAILABILITY_PLATFORM(X, PrettyName) X,
#include "nextcode/AST/PlatformKinds.def"
};

/// Returns the short string representing the platform, suitable for
/// use in availability specifications (e.g., "OSX").
StringRef platformString(PlatformKind platform);
  
/// Returns the platform kind corresponding to the passed-in short platform name
/// or None if such a platform kind does not exist.
std::optional<PlatformKind> platformFromString(StringRef Name);

/// Safely converts the given unsigned value to a valid \c PlatformKind value or
/// \c nullopt otherwise.
std::optional<PlatformKind> platformFromUnsigned(unsigned value);

/// Returns a valid platform string that is closest to the candidate string
/// based on edit distance. Returns \c None if the closest valid platform
/// distance is not within a minimum threshold.
std::optional<StringRef> closestCorrectedPlatformString(StringRef candidate);

/// Returns a human-readable version of the platform name as a string, suitable
/// for emission in diagnostics (e.g., "macOS").
StringRef prettyPlatformString(PlatformKind platform);

/// Returns the base platform for an application-extension platform. For example
/// `iOS` would be returned for `iOSApplicationExtension`. Returns `None` for
/// platforms which are not application extension platforms.
std::optional<PlatformKind>
basePlatformForExtensionPlatform(PlatformKind Platform);

/// Returns true if \p Platform represents and application extension platform,
/// e.g. `iOSApplicationExtension`.
inline bool isApplicationExtensionPlatform(PlatformKind Platform) {
  return basePlatformForExtensionPlatform(Platform).has_value();
}

/// Returns whether the passed-in platform is active, given the language
/// options. A platform is active if either it is the target platform or its
/// AppExtension variant is the target platform. For example, OS X is
/// considered active when the target operating system is OS X and app extension
/// restrictions are enabled, but OSXApplicationExtension is not considered
/// active when the target platform is OS X and app extension restrictions are
/// disabled. PlatformKind::none is always considered active.
/// If ForTargetVariant is true then for zippered builds the target-variant
/// triple will be used rather than the target to determine whether the
/// platform is active.
bool isPlatformActive(PlatformKind Platform, const LangOptions &LangOpts,
                      bool ForTargetVariant = false, bool ForRuntimeQuery = false);

/// Returns the target platform for the given language options.
PlatformKind targetPlatform(const LangOptions &LangOpts);

/// Returns true when availability attributes from the "parent" platform
/// should also apply to the "child" platform for declarations without
/// an explicit attribute for the child.
bool inheritsAvailabilityFromPlatform(PlatformKind Child, PlatformKind Parent);

llvm::VersionTuple canonicalizePlatformVersion(
    PlatformKind platform, const llvm::VersionTuple &version);

} // end namespace nextcode

#endif
