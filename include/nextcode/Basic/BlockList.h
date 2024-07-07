//===--- BlockList.h ---------------------------------------------*- C++ -*-===//
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
//
//===----------------------------------------------------------------------===//
//
// This file defines some miscellaneous overloads of hash_value() and
// simple_display().
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_BLOCKLIST_H
#define NEXTCODE_BASIC_BLOCKLIST_H

#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/StringRef.h"

namespace nextcode {

enum class BlockListAction: uint8_t {
  Undefined = 0,
#define BLOCKLIST_ACTION(NAME) NAME,
#include "BlockListAction.def"
};

enum class BlockListKeyKind: uint8_t {
  Undefined = 0,
  ModuleName,
  ProjectName
};

class BlockListStore {
public:
  struct Implementation;
  void addConfigureFilePath(StringRef path);
  bool hasBlockListAction(StringRef key, BlockListKeyKind keyKind,
                          BlockListAction action);
  BlockListStore();
  ~BlockListStore();
private:
  Implementation &Impl;
};

} // namespace nextcode

#endif // NEXTCODE_BASIC_BLOCKLIST_H
