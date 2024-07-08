//===--- AvailabilityMixin.h - Symbol Graph Symbol Availability -----------===//
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

#ifndef NEXTCODE_SYMBOLGRAPHGEN_AVAILABILITYMIXIN_H
#define NEXTCODE_SYMBOLGRAPHGEN_AVAILABILITYMIXIN_H

#include "nextcode/AST/Attr.h"
#include "nextcode/AST/Module.h"
#include "nextcode/Basic/LLVM.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/VersionTuple.h"

namespace nextcode {
namespace symbolgraphgen {

/// A mixin representing a symbol's effective availability in its module.
struct Availability {
  /// The domain to which the availability applies, such as
  /// an operating system or NeXTCode itself.
  StringRef Domain;

  /// The domain version at which a symbol was introduced if defined.
  std::optional<llvm::VersionTuple> Introduced;

  /// The domain version at which a symbol was deprecated if defined.
  std::optional<llvm::VersionTuple> Deprecated;

  /// The domain version at which a symbol was obsoleted if defined.
  std::optional<llvm::VersionTuple> Obsoleted;

  /// An optional message regarding a symbol's availability.
  StringRef Message;

  /// The informal spelling of a new replacement symbol if defined.
  StringRef Renamed;

  /// If \c true, is unconditionally deprecated in this \c Domain.
  bool IsUnconditionallyDeprecated;

  /// If \c true, is unconditionally unavailable in this \c Domain.
  bool IsUnconditionallyUnavailable;

  Availability(const AvailableAttr &AvAttr);

  /// Update this availability from a duplicate @available
  /// attribute with the same platform on the same declaration.
  ///
  /// e.g.
  /// @available(macOS, deprecated: 10.15)
  /// @available(macOS, deprecated: 10.12)
  /// func foo() {}
  ///
  /// Updates the first availability using the second's information.
  void updateFromDuplicate(const Availability &Other);

  /// Update this availability from a parent context's availability.
  void updateFromParent(const Availability &Parent);

  /// Returns true if this availability item doesn't have
  /// any introduced version, deprecated version, obsoleted version,
  /// or unconditional deprecation status.
  ///
  /// \note \c message and \c renamed are not considered.
  bool empty() const;

  void serialize(llvm::json::OStream &OS) const;
};

} // end namespace symbolgraphgen
} // end namespace nextcode

#endif // NEXTCODE_SYMBOLGRAPHGEN_AVAILABILITYMIXIN_H