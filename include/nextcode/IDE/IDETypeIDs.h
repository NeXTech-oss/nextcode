//===--- IDETypeIDs.h - IDE Type Ids ----------------------------*- C++ -*-===//
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
//  This file defines TypeID support for IDE types.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IDE_IDETYPEIDS_H
#define NEXTCODE_IDE_IDETYPEIDS_H

#include "nextcode/Basic/TypeID.h"
namespace nextcode {

#define NEXTCODE_TYPEID_ZONE IDETypes
#define NEXTCODE_TYPEID_HEADER "nextcode/IDE/IDETypeIDZone.def"
#include "nextcode/Basic/DefineTypeIDZone.h"

} // end namespace nextcode

#endif // NEXTCODE_IDE_IDETYPEIDS_H
