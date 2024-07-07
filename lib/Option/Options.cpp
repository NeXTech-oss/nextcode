//===--- Options.cpp - Option info & table --------------------------------===//
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

#include "nextcode/Option/Options.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/Option/OptTable.h"
#include "llvm/Option/Option.h"

using namespace nextcode::options;
using namespace llvm::opt;

#define PREFIX(NAME, VALUE)                                                    \
  constexpr llvm::StringLiteral NAME##_init[] = VALUE;                         \
  constexpr llvm::ArrayRef<llvm::StringLiteral> NAME(                          \
      NAME##_init, std::size(NAME##_init) - 1);
#include "nextcode/Option/Options.inc"
#undef PREFIX

static const llvm::opt::GenericOptTable::Info InfoTable[] = {
#define OPTION(PREFIX, NAME, ID, KIND, GROUP, ALIAS, ALIASARGS, FLAGS, PARAM,  \
               HELPTEXT, METAVAR, VALUES)                                      \
  {PREFIX, NAME,  HELPTEXT,    METAVAR,     OPT_##ID,  Option::KIND##Class,    \
   PARAM,  FLAGS, OPT_##GROUP, OPT_##ALIAS, ALIASARGS, VALUES},
#include "nextcode/Option/Options.inc"
#undef OPTION
};

namespace {

class NeXTCodeOptTable : public llvm::opt::GenericOptTable {
public:
  NeXTCodeOptTable() : GenericOptTable(InfoTable) {}
};

} // end anonymous namespace

std::unique_ptr<OptTable> nextcode::createNeXTCodeOptTable() {
  return std::unique_ptr<GenericOptTable>(new NeXTCodeOptTable());
}
