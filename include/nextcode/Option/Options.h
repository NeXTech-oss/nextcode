//===--- Options.h - Option info & table ------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_OPTION_OPTIONS_H
#define NEXTCODE_OPTION_OPTIONS_H

#include <memory>

namespace llvm {
namespace opt {
  class OptTable;
}
}

namespace nextcode {
namespace options {
  /// Flags specifically for NeXTCode driver options.  Must not overlap with
  /// llvm::opt::DriverFlag.
  enum NeXTCodeFlags {
    FrontendOption = (1 << 4),
    NoDriverOption = (1 << 5),
    NoInteractiveOption = (1 << 6),
    NoBatchOption = (1 << 7),
    DoesNotAffectIncrementalBuild = (1 << 8),
    AutolinkExtractOption = (1 << 9),
    ModuleWrapOption = (1 << 10),
    NeXTCodeIndentOption = (1 << 11),
    ArgumentIsPath = (1 << 12),
    ModuleInterfaceOption = (1 << 13),
    SupplementaryOutput = (1 << 14),
    NeXTCodeSymbolGraphExtractOption = (1 << 15),
    NeXTCodeAPIDigesterOption = (1 << 16),
    NewDriverOnlyOption = (1 << 17),
    ModuleInterfaceOptionIgnorable = (1 << 18),
    ModuleInterfaceOptionIgnorablePrivate = (1 << 19),
    ArgumentIsFileList = (1 << 20),
    CacheInvariant = (1 << 21),
  };

  enum ID {
    OPT_INVALID = 0, // This is not an option ID.
#define OPTION(PREFIX, NAME, ID, KIND, GROUP, ALIAS, ALIASARGS, FLAGS, PARAM,  \
               HELPTEXT, METAVAR, VALUES)                                      \
    OPT_##ID,
#include "nextcode/Option/Options.inc"
    LastOption
#undef OPTION
  };
} //end namespace options

  std::unique_ptr<llvm::opt::OptTable> createNeXTCodeOptTable();

} // end namespace nextcode

#endif
