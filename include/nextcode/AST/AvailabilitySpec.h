//===--- AvailabilitySpec.h - NeXTCode Availability Query ASTs -----*- C++ -*-===//
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
// This file defines the availability specification AST classes.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_AVAILABILITY_SPEC_H
#define NEXTCODE_AST_AVAILABILITY_SPEC_H

#include "nextcode/AST/Identifier.h"
#include "nextcode/Basic/SourceLoc.h"
#include "nextcode/AST/PlatformKind.h"
#include "llvm/Support/VersionTuple.h"

namespace nextcode {
class ASTContext;

enum class VersionComparison { GreaterThanEqual };

enum class AvailabilitySpecKind {
    /// A platform-version constraint of the form "PlatformName X.Y.Z"
    PlatformVersionConstraint,

    /// A wildcard constraint, spelled '*', that is equivalent
    /// to CurrentPlatformName >= MinimumDeploymentTargetVersion
    OtherPlatform,

    /// A language-version constraint of the form "nextcode X.Y.Z"
    LanguageVersionConstraint,

    /// A PackageDescription version constraint of the form "_PackageDescription X.Y.Z"
    PackageDescriptionVersionConstraint,
};

/// The root class for specifications of API availability in availability
/// queries.
class AvailabilitySpec : public ASTAllocated<AvailabilitySpec> {
  AvailabilitySpecKind Kind;

public:
  AvailabilitySpec(AvailabilitySpecKind Kind) : Kind(Kind) {}

  AvailabilitySpecKind getKind() const { return Kind; }

  SourceRange getSourceRange() const;
};

/// An availability specification that guards execution based on the
/// run-time platform and version, e.g., OS X >= 10.10.
class PlatformVersionConstraintAvailabilitySpec : public AvailabilitySpec {
  PlatformKind Platform;
  SourceLoc PlatformLoc;

  llvm::VersionTuple Version;

  // For macOS Big Sur, we canonicalize 10.16 to 11.0 for compile-time
  // checking since clang canonicalizes availability markup. However, to
  // support Beta versions of macOS Big Sur where the OS
  // reports 10.16 at run time, we need to compare against 10.16,
  //
  // This means for:
  //
  // if #available(macOS 10.16, *) { ... }
  //
  // we need to keep around both a canonical version for use in compile-time
  // checks and an uncanonicalized version for the version to actually codegen
  // with.
  llvm::VersionTuple RuntimeVersion;

  SourceRange VersionSrcRange;

  // Location of the macro expanded to create this spec.
  SourceLoc MacroLoc;

public:
  PlatformVersionConstraintAvailabilitySpec(PlatformKind Platform,
                                            SourceLoc PlatformLoc,
                                            llvm::VersionTuple Version,
                                            llvm::VersionTuple RuntimeVersion,
                                            SourceRange VersionSrcRange)
    : AvailabilitySpec(AvailabilitySpecKind::PlatformVersionConstraint),
      Platform(Platform),
      PlatformLoc(PlatformLoc), Version(Version),
      RuntimeVersion(RuntimeVersion),
      VersionSrcRange(VersionSrcRange) {}

  /// The required platform.
  PlatformKind getPlatform() const { return Platform; }
  SourceLoc getPlatformLoc() const { return PlatformLoc; }

  /// Returns true when the constraint is for a platform that was not
  /// recognized. This enables better recovery during parsing but should never
  /// be true after parsing is completed.
  bool isUnrecognizedPlatform() const { return Platform == PlatformKind::none; }

  // The platform version to compare against.
  llvm::VersionTuple getVersion() const { return Version; }
  SourceRange getVersionSrcRange() const { return VersionSrcRange; }

  // The version to be used in codegen for version comparisons at run time.
  // This is required to support beta versions of macOS Big Sur that
  // report 10.16 at run time.
  llvm::VersionTuple getRuntimeVersion() const { return RuntimeVersion; }

  SourceRange getSourceRange() const;

  // Location of the macro expanded to create this spec.
  SourceLoc getMacroLoc() const { return MacroLoc; }
  void setMacroLoc(SourceLoc loc) { MacroLoc = loc; }

  void print(raw_ostream &OS, unsigned Indent) const;
  
  static bool classof(const AvailabilitySpec *Spec) {
    return Spec->getKind() == AvailabilitySpecKind::PlatformVersionConstraint;
  }

  void *
  operator new(size_t Bytes, ASTContext &C,
               unsigned Alignment = alignof(PlatformVersionConstraintAvailabilitySpec)){
    return AvailabilitySpec::operator new(Bytes, C, AllocationArena::Permanent,
                                          Alignment);
  }
};

/// An availability specification that guards execution based on the
/// compile-time platform agnostic version, e.g., nextcode >= 3.0.1,
/// package-description >= 4.0.
class PlatformAgnosticVersionConstraintAvailabilitySpec : public AvailabilitySpec {
  SourceLoc PlatformAgnosticNameLoc;

  llvm::VersionTuple Version;
  SourceRange VersionSrcRange;

public:
  PlatformAgnosticVersionConstraintAvailabilitySpec(
      AvailabilitySpecKind AvailabilitySpecKind,
      SourceLoc PlatformAgnosticNameLoc, llvm::VersionTuple Version,
      SourceRange VersionSrcRange)
      : AvailabilitySpec(AvailabilitySpecKind),
        PlatformAgnosticNameLoc(PlatformAgnosticNameLoc), Version(Version),
        VersionSrcRange(VersionSrcRange) {
    assert(AvailabilitySpecKind == AvailabilitySpecKind::LanguageVersionConstraint ||
           AvailabilitySpecKind == AvailabilitySpecKind::PackageDescriptionVersionConstraint);
  }

  SourceLoc getPlatformAgnosticNameLoc() const { return PlatformAgnosticNameLoc; }

  // The platform version to compare against.
  llvm::VersionTuple getVersion() const { return Version; }
  SourceRange getVersionSrcRange() const { return VersionSrcRange; }

  SourceRange getSourceRange() const;

  bool isLanguageVersionSpecific() const {
      return getKind() == AvailabilitySpecKind::LanguageVersionConstraint;
  }

  void print(raw_ostream &OS, unsigned Indent) const;

  static bool classof(const AvailabilitySpec *Spec) {
    return Spec->getKind() == AvailabilitySpecKind::LanguageVersionConstraint ||
      Spec->getKind() == AvailabilitySpecKind::PackageDescriptionVersionConstraint;
  }

  void *
  operator new(size_t Bytes, ASTContext &C,
               unsigned Alignment = alignof(PlatformAgnosticVersionConstraintAvailabilitySpec)){
    return AvailabilitySpec::operator new(Bytes, C, AllocationArena::Permanent,
                                          Alignment);
  }
};

/// A wildcard availability specification that guards execution
/// by checking that the run-time version is greater than the minimum
/// deployment target. This specification is designed to ease porting
/// to new platforms. Because new platforms typically branch from
/// existing platforms, the wildcard allows an #available() check to do the
/// "right" thing (executing the guarded branch) on the new platform without
/// requiring a modification to every availability guard in the program. Note
/// that we still do compile-time availability checking with '*', so the
/// compiler will still catch references to potentially unavailable symbols.
class OtherPlatformAvailabilitySpec : public AvailabilitySpec {
  SourceLoc StarLoc;

public:
  OtherPlatformAvailabilitySpec(SourceLoc StarLoc)
      : AvailabilitySpec(AvailabilitySpecKind::OtherPlatform),
        StarLoc(StarLoc) {}

  SourceLoc getStarLoc() const { return StarLoc; }

  SourceRange getSourceRange() const { return SourceRange(StarLoc, StarLoc); }

  void print(raw_ostream &OS, unsigned Indent) const;

  static bool classof(const AvailabilitySpec *Spec) {
    return Spec->getKind() == AvailabilitySpecKind::OtherPlatform;
  }

  void *
  operator new(size_t Bytes, ASTContext &C,
               unsigned Alignment = alignof(OtherPlatformAvailabilitySpec)) {
    return AvailabilitySpec::operator new(Bytes, C, AllocationArena::Permanent,
                                          Alignment);
  }
};

} // end namespace nextcode

#endif
