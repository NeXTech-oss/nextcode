//===--- PrettyStackTrace.h - PrettyStackTrace for the Driver ---*- C++ -*-===//
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

#ifndef NEXTCODE_DRIVER_PRETTYSTACKTRACE_H
#define NEXTCODE_DRIVER_PRETTYSTACKTRACE_H

#include "nextcode/Basic/FileTypes.h"
#include "llvm/Support/PrettyStackTrace.h"

namespace nextcode {
namespace driver {

class Action;
class Job;
class CommandOutput;

class PrettyStackTraceDriverAction : public llvm::PrettyStackTraceEntry {
  const Action *TheAction;
  const char *Description;

public:
  PrettyStackTraceDriverAction(const char *desc, const Action *A)
      : TheAction(A), Description(desc) {}
  void print(llvm::raw_ostream &OS) const override;
};

class PrettyStackTraceDriverJob : public llvm::PrettyStackTraceEntry {
  const Job *TheJob;
  const char *Description;

public:
  PrettyStackTraceDriverJob(const char *desc, const Job *A)
      : TheJob(A), Description(desc) {}
  void print(llvm::raw_ostream &OS) const override;
};

class PrettyStackTraceDriverCommandOutput : public llvm::PrettyStackTraceEntry {
  const CommandOutput *TheCommandOutput;
  const char *Description;

public:
  PrettyStackTraceDriverCommandOutput(const char *desc, const CommandOutput *A)
      : TheCommandOutput(A), Description(desc) {}
  void print(llvm::raw_ostream &OS) const override;
};

class PrettyStackTraceDriverCommandOutputAddition
    : public llvm::PrettyStackTraceEntry {
  const CommandOutput *TheCommandOutput;
  StringRef PrimaryInput;
  file_types::ID NewOutputType;
  StringRef NewOutputName;
  const char *Description;

public:
  PrettyStackTraceDriverCommandOutputAddition(const char *desc,
                                              const CommandOutput *A,
                                              StringRef Primary,
                                              file_types::ID type,
                                              StringRef New)
      : TheCommandOutput(A), PrimaryInput(Primary), NewOutputType(type),
        NewOutputName(New), Description(desc) {}
  void print(llvm::raw_ostream &OS) const override;
};
}
}

#endif