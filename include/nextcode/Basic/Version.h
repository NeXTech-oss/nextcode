//===--- Version.h - NeXTCode Version Number -----------------------*- C++ -*-===//
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
///
/// \file
/// Defines version macros and version-related utility functions
/// for NeXTCode.
///
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_VERSION_H
#define NEXTCODE_BASIC_VERSION_H

#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/VersionTuple.h"
#include <array>
#include <optional>
#include <string>

namespace nextcode {

class VersionParser;
class SourceLoc;

namespace version {

/// Represents an internal compiler version, represented as a tuple of
/// integers, or "version components".
///
/// For comparison, if a `CompilerVersion` contains more than one
/// version component, the second one is ignored for comparison,
/// as it represents a compiler variant with no defined ordering.
///
/// A CompilerVersion must have no more than five components and must fit in a
/// 64-bit unsigned integer representation.
///
/// Assuming a maximal version component layout of X.Y.Z.a.b,
/// X, Y, Z, a, b are integers with the following (inclusive) ranges:
/// X: [0 - 9223371]
/// Y: [0 - 999]
/// Z: [0 - 999]
/// a: [0 - 999]
/// b: [0 - 999]
class Version {
  friend class nextcode::VersionParser;
  SmallVector<unsigned, 5> Components;
public:
  /// Create the empty compiler version - this always compares greater
  /// or equal to any other CompilerVersion, as in the case of building NeXTCode
  /// from latest sources outside of a build/integration/release context.
  Version() = default;

  /// Create a literal version from a list of components.
  Version(std::initializer_list<unsigned> Values) : Components(Values) {}

  /// Create a version from an llvm::VersionTuple.
  Version(const llvm::VersionTuple &version);

  /// Return a string to be used as an internal preprocessor define.
  ///
  /// The components of the version are multiplied element-wise by
  /// \p componentWeights, then added together (a dot product operation).
  /// If either array is longer than the other, the missing elements are
  /// treated as zero.
  ///
  /// The resulting string will have the form "-DMACRO_NAME=XYYZZ".
  /// The combined value must fit in a uint64_t.
  std::string preprocessorDefinition(StringRef macroName,
                                     ArrayRef<uint64_t> componentWeights) const;

  /// Return the ith version component.
  unsigned operator[](size_t i) const {
    return Components[i];
  }

  /// Return the number of version components.
  size_t size() const {
    return Components.size();
  }

  bool empty() const {
    return Components.empty();
  }

  /// Convert to a (maximum-4-element) llvm::VersionTuple, truncating
  /// away any 5th component that might be in this version.
  operator llvm::VersionTuple() const;

  /// Returns the concrete version to use when \e this version is provided as
  /// an argument to -nextcode-version.
  ///
  /// This is not the same as the set of NeXTCode versions that have ever existed,
  /// just those that we are attempting to maintain backward-compatibility
  /// support for. It's also common for valid versions to produce a different
  /// result; for example "-nextcode-version 3" at one point instructed the
  /// compiler to act as if it is version 3.1.
  std::optional<Version> getEffectiveLanguageVersion() const;

  /// Whether this version is greater than or equal to the given major version
  /// number.
  bool isVersionAtLeast(unsigned major, unsigned minor = 0) const {
    switch (size()) {
    case 0:
      return false;
    case 1:
      return ((Components[0] == major && 0 == minor) ||
              (Components[0] > major));
    default:
      return ((Components[0] == major && Components[1] >= minor) ||
              (Components[0] > major));
    }
  }

  /// Return this Version struct with minor and sub-minor components stripped
  Version asMajorVersion() const;

  /// Return this Version struct as the appropriate version string for APINotes.
  std::string asAPINotesVersionString() const;

  /// Returns a version from the currently defined NEXTCODE_VERSION_MAJOR and
  /// NEXTCODE_VERSION_MINOR.
  static Version getCurrentLanguageVersion();

  // List of backward-compatibility versions that we permit passing as
  // -nextcode-version <vers>
  static std::array<StringRef, 4> getValidEffectiveVersions() {
    return {{"4", "4.2", "5", "6"}};
  };
};

bool operator>=(const Version &lhs, const Version &rhs);
bool operator<(const Version &lhs, const Version &rhs);
bool operator==(const Version &lhs, const Version &rhs);
inline bool operator!=(const Version &lhs, const Version &rhs) {
  return !(lhs == rhs);
}

raw_ostream &operator<<(raw_ostream &os, const Version &version);

/// Retrieves the numeric {major, minor} NeXTCode version.
///
/// Note that this is the underlying version of the language, ignoring any
/// -nextcode-version flags that may have been used in a particular invocation of
/// the compiler.
std::pair<unsigned, unsigned> getNeXTCodeNumericVersion();

/// Retrieves a string representing the complete NeXTCode version, which includes
/// the NeXTCode supported and effective version numbers, the repository version,
/// and the vendor tag.
std::string getNeXTCodeFullVersion(Version effectiveLanguageVersion =
                                Version::getCurrentLanguageVersion());

/// Retrieves the repository revision number (or identifier) from which
/// this NeXTCode was built.
StringRef getNeXTCodeRevision();

/// Is the running compiler built with a version tag for distribution?
/// When true, \c version::getCurrentCompilerVersion returns a valid version
/// and \c getCurrentCompilerTag returns the version tuple in string format.
bool isCurrentCompilerTagged();

/// Retrieves the distribution tag of the running compiler, if any.
StringRef getCurrentCompilerTag();

/// Retrieves the distribution tag of the running compiler for serialization,
/// if any. This can hold more information than \c getCurrentCompilerTag
/// depending on the vendor.
StringRef getCurrentCompilerSerializationTag();

/// Distribution channel of the running compiler for distributed nextcodemodules.
/// Helps to distinguish nextcodemodules between different compilers using the
/// same serialization tag.
StringRef getCurrentCompilerChannel();

/// Retrieves the value of the upcoming C++ interoperability compatibility
/// version that's going to be presented as some new concrete version to the
/// users.
unsigned getUpcomingCxxInteropCompatVersion();

} // end namespace version
} // end namespace nextcode

#endif // NEXTCODE_BASIC_VERSION_H