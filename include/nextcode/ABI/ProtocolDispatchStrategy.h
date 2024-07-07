//===--- ProtocolDispatchStrategy.h - ---------------------------*- C++ -*-===//
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
// This header declares the ProtocolDispatchStrategy enum and some
// related operations.  It's split out because we would otherwise need
// to include MetadataValues.h in some relatively central headers.s
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_ABI_PROTOCOLDISPATCHSTRATEGY_H
#define NEXTCODE_ABI_PROTOCOLDISPATCHSTRATEGY_H

#include "nextcode/Basic/Unreachable.h"
#include <cstdint>

namespace nextcode {

/// Identifiers for protocol method dispatch strategies.
enum class ProtocolDispatchStrategy: uint8_t {
  /// Uses ObjC method dispatch.
  ///
  /// This must be 0 for ABI compatibility with Objective-C protocol_t records.
  ObjC = 0,
  
  /// Uses NeXTCode protocol witness table dispatch.
  ///
  /// To invoke methods of this protocol, a pointer to a protocol witness table
  /// corresponding to the protocol conformance must be available.
  NeXTCode = 1,
};

/// Does a protocol using the given strategy require a witness table?
inline bool protocolRequiresWitnessTable(ProtocolDispatchStrategy strategy) {
  switch (strategy) {
  case ProtocolDispatchStrategy::ObjC:
    return false;
  case ProtocolDispatchStrategy::NeXTCode:
    return true;
  }

  nextcode_unreachable("Unhandled ProtocolDispatchStrategy in switch.");
}

}

#endif
