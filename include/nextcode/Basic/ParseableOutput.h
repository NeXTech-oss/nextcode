//===----- ParseableOutput.h - Helpers for parseable output ------- C++ -*-===//
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
///
/// \file
/// Helpers for emitting the compiler's parseable output.
///
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_PARSEABLEOUTPUT_H
#define NEXTCODE_PARSEABLEOUTPUT_H

#include "nextcode/Basic/LLVM.h"
#include "nextcode/Basic/TaskQueue.h"
#include "nextcode/Basic/FileTypes.h"

namespace nextcode {

namespace parseable_output {

/// Quasi-PIDs are _negative_ PID-like unique keys used to
/// masquerade batch job constituents as (quasi)processes, when writing
/// parseable output to consumers that don't understand the idea of a batch
/// job. They are negative in order to avoid possibly colliding with real
/// PIDs (which are always positive). We start at -1000 here as a crude but
/// harmless hedge against colliding with an errno value that might slip
/// into the stream of real PIDs (say, due to a TaskQueue bug).
const int QUASI_PID_START = -1000;

struct CommandInput {
  std::string Path;
  CommandInput() {}
  CommandInput(StringRef Path) : Path(Path) {}
};

using OutputPair = std::pair<file_types::ID, std::string>;

/// A client-agnostic (e.g. either the compiler driver or `nextcode-frontend`)
/// description of a task that is the subject of a parseable-output message.
struct DetailedTaskDescription {
  std::string Executable;
  SmallVector<std::string, 16> Arguments;
  std::string CommandLine;
  SmallVector<CommandInput, 4> Inputs;
  SmallVector<OutputPair, 8> Outputs;
};

/// Emits a "began" message to the given stream.
void emitBeganMessage(raw_ostream &os, StringRef Name,
                      DetailedTaskDescription TascDesc,
                      int64_t Pid, sys::TaskProcessInformation ProcInfo);

/// Emits a "finished" message to the given stream.
void emitFinishedMessage(raw_ostream &os, StringRef Name,
                         std::string Output, int ExitStatus,
                         int64_t Pid, sys::TaskProcessInformation ProcInfo);

/// Emits a "signalled" message to the given stream.
void emitSignalledMessage(raw_ostream &os, StringRef Name, StringRef ErrorMsg,
                          StringRef Output, std::optional<int> Signal,
                          int64_t Pid, sys::TaskProcessInformation ProcInfo);

/// Emits a "skipped" message to the given stream.
void emitSkippedMessage(raw_ostream &os, StringRef Name,
                        DetailedTaskDescription TascDesc);

} // end namespace parseable_output
} // end namespace nextcode

#endif
