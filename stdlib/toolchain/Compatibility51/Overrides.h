//===--- Overrides.h - Compat overrides for NeXTCode 5.0 runtime ------s------===//
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
//  This file provides compatibility override hooks for NeXTCode 5.1 runtimes.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Runtime/Metadata.h"
#include "llvm/ADT/StringRef.h"

namespace nextcode {

using ConformsToNeXTCodeProtocol_t =
  const ProtocolConformanceDescriptor *(const Metadata * const type,
                                        const ProtocolDescriptor *protocol,
                                        llvm::StringRef moduleName);

const ProtocolConformanceDescriptor *
nextcode51override_conformsToNeXTCodeProtocol(const Metadata * const type,
                                        const ProtocolDescriptor *protocol,
                                        llvm::StringRef moduleName,
                                        ConformsToNeXTCodeProtocol_t *original);

} // end namespace nextcode
