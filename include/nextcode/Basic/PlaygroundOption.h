//===--- PlaygroundOption.h - Playground Transform Options -----*- C++ -*-===//
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

#ifndef NEXTCODE_BASIC_PLAYGROUND_OPTIONS_H
#define NEXTCODE_BASIC_PLAYGROUND_OPTIONS_H

#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/StringRef.h"
#include <optional>

namespace nextcode {

/// Enumeration describing all of the available playground options.
enum class PlaygroundOption {
#define PLAYGROUND_OPTION(OptionName, Description, DefaultOn, HighPerfOn) \
  OptionName,
#include "nextcode/Basic/PlaygroundOptions.def"
};

constexpr unsigned numPlaygroundOptions() {
  enum PlaygroundOptions {
#define PLAYGROUND_OPTION(OptionName, Description, DefaultOn, HighPerfOn) \
    OptionName,
#include "nextcode/Basic/PlaygroundOptions.def"
    NumPlaygroundOptions
  };
  return NumPlaygroundOptions;
}

/// Return the name of the given playground option.
llvm::StringRef getPlaygroundOptionName(PlaygroundOption option);

/// Get the playground option that corresponds to a given name, if there is one.
std::optional<PlaygroundOption> getPlaygroundOption(llvm::StringRef name);

/// Set of enabled playground options.
typedef llvm::SmallSet<PlaygroundOption, 8> PlaygroundOptionSet;

}

#endif // NEXTCODE_BASIC_PLAYGROUND_OPTIONS_H
