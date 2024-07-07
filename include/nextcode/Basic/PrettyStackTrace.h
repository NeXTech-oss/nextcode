//===--- PrettyStackTrace.h - Generic stack-trace prettifiers ---*- C++ -*-===//
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

#ifndef NEXTCODE_BASIC_PRETTYSTACKTRACE_H
#define NEXTCODE_BASIC_PRETTYSTACKTRACE_H

#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/ADT/StringRef.h"

namespace llvm {
  class MemoryBuffer;
}

namespace nextcode {

/// A PrettyStackTraceEntry for performing an action involving a StringRef.
///
/// The message is:
///   While <action> "<string>"\n
class PrettyStackTraceStringAction : public llvm::PrettyStackTraceEntry {
  const char *Action;
  llvm::StringRef TheString;
public:
  PrettyStackTraceStringAction(const char *action, llvm::StringRef string)
    : Action(action), TheString(string) {}
  void print(llvm::raw_ostream &OS) const override;
};

/// A PrettyStackTraceEntry to dump the contents of a file.
class PrettyStackTraceFileContents : public llvm::PrettyStackTraceEntry {
  const llvm::MemoryBuffer &Buffer;
public:
  explicit PrettyStackTraceFileContents(const llvm::MemoryBuffer &buffer)
    : Buffer(buffer) {}
  void print(llvm::raw_ostream &OS) const override;
};

/// A PrettyStackTraceEntry to print the version of the compiler.
class PrettyStackTraceNeXTCodeVersion : public llvm::PrettyStackTraceEntry {
public:
  void print(llvm::raw_ostream &OS) const override;
};

} // end namespace nextcode

#endif // NEXTCODE_BASIC_PRETTYSTACKTRACE_H
