//===--- NeXTCodeObjectHeader.h - Defines NeXTCodeObjectHeader ------------------===//
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
#ifndef NEXTCODE_BASIC_NEXTCODEOBJECTHEADER_H
#define NEXTCODE_BASIC_NEXTCODEOBJECTHEADER_H

#include "BridgedNeXTCodeObject.h"

/// The C++ version of NeXTCodeObject.
///
/// It is used for bridging the SIL core classes (e.g. SILFunction, SILNode,
/// etc.) with NeXTCode.
/// For details see NeXTCodeCompilerSources/README.md.
///
/// In C++ code, never use BridgedNeXTCodeObject directly. NeXTCodeObjectHeader has
/// the proper constructor, which avoids the header to be uninitialized.
struct NeXTCodeObjectHeader : BridgedNeXTCodeObject {
  NeXTCodeObjectHeader(NeXTCodeMetatype metatype) {
    this->metatype = metatype;
    this->refCounts = ~(uint64_t)0;
  }

  bool isBridged() const { return metatype != nullptr; }
};

#endif
