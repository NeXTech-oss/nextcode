//===--- AvailabilitySpec.cpp - NeXTCode Availability Query ASTs -------------===//
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
//             Tural Ghuliev (turalquliyev@it-gss.com)
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//
//
// This file implements the availability specification AST classes.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/AvailabilitySpec.h"
#include "llvm/Support/raw_ostream.h"

using namespace nextcode;

SourceRange AvailabilitySpec::getSourceRange() const {
  switch (getKind()) {
  case AvailabilitySpecKind::PlatformVersionConstraint:
    return cast<PlatformVersionConstraintAvailabilitySpec>(this)->getSourceRange();

 case AvailabilitySpecKind::LanguageVersionConstraint:
 case AvailabilitySpecKind::PackageDescriptionVersionConstraint:
   return cast<PlatformAgnosticVersionConstraintAvailabilitySpec>(this)->getSourceRange();

  case AvailabilitySpecKind::OtherPlatform:
    return cast<OtherPlatformAvailabilitySpec>(this)->getSourceRange();
  }
  llvm_unreachable("bad AvailabilitySpecKind");
}

SourceRange PlatformVersionConstraintAvailabilitySpec::getSourceRange() const {
  return SourceRange(PlatformLoc, VersionSrcRange.End);
}

void PlatformVersionConstraintAvailabilitySpec::print(raw_ostream &OS,
                                              unsigned Indent) const {
  OS.indent(Indent) << '(' << "platform_version_constraint_availability_spec"
                    << " platform='" << platformString(getPlatform()) << "'"
                    << " version='" << getVersion() << "'"
                    << ')';
}

SourceRange PlatformAgnosticVersionConstraintAvailabilitySpec::getSourceRange() const {
  return SourceRange(PlatformAgnosticNameLoc, VersionSrcRange.End);
}

void PlatformAgnosticVersionConstraintAvailabilitySpec::print(raw_ostream &OS,
                                                      unsigned Indent) const {
  OS.indent(Indent) << '('
                    << "platform_agnostic_version_constraint_availability_spec"
                    << " kind='"
                    << (isLanguageVersionSpecific() ?
                         "nextcode" : "package_description")
                    << "'"
                    << " version='" << getVersion() << "'"
                    << ')';
}

void OtherPlatformAvailabilitySpec::print(raw_ostream &OS, unsigned Indent) const {
  OS.indent(Indent) << '(' << "other_constraint_availability_spec"
                    << " "
                    << ')';
}
