//===--- PrimarySpecificPaths.h ---------------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_BASIC_PRIMARYSPECIFICPATHS_H
#define NEXTCODE_BASIC_PRIMARYSPECIFICPATHS_H

#include "nextcode/Basic/LLVM.h"
#include "nextcode/Basic/SupplementaryOutputPaths.h"
#include "llvm/ADT/StringRef.h"

#include <string>

namespace nextcode {

/// Holds all of the output paths, and debugging-info path that are
/// specific to which primary file is being compiled at the moment.

class PrimarySpecificPaths {
public:
  /// The name of the main output file,
  /// that is, the .o file for this input (or a file specified by -o).
  /// If there is no such file, contains an empty string. If the output
  /// is to be written to stdout, contains "-".
  std::string OutputFilename;

  /// The name to report the main output file as being in the index store.
  /// This is equivalent to OutputFilename, unless -index-store-output-path
  /// was specified.
  std::string IndexUnitOutputFilename;

  SupplementaryOutputPaths SupplementaryOutputs;

  /// The name of the "main" input file, used by the debug info.
  std::string MainInputFilenameForDebugInfo;

  PrimarySpecificPaths(StringRef OutputFilename = StringRef(),
                       StringRef IndexUnitOutputFilename = StringRef(),
                       StringRef MainInputFilenameForDebugInfo = StringRef(),
                       SupplementaryOutputPaths SupplementaryOutputs =
                           SupplementaryOutputPaths())
      : OutputFilename(OutputFilename),
        IndexUnitOutputFilename(IndexUnitOutputFilename),
        SupplementaryOutputs(SupplementaryOutputs),
        MainInputFilenameForDebugInfo(MainInputFilenameForDebugInfo) {}

  bool haveModuleOrModuleDocOutputPaths() const {
    return !SupplementaryOutputs.ModuleOutputPath.empty() ||
           !SupplementaryOutputs.ModuleDocOutputPath.empty();
  }
  bool haveModuleSummaryOutputPath() const {
    return !SupplementaryOutputs.ModuleSummaryOutputPath.empty();
  }
};
} // namespace nextcode

#endif // NEXTCODE_BASIC_PRIMARYSPECIFICPATHS_H
