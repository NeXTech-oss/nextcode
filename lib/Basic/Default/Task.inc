//===--- Task.inc - ---------------------------------------------*- C++ -*-===//
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
/// This file contains the default implementation of the class Task
///
//===----------------------------------------------------------------------===//

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/Signals.h"
#include <optional>

namespace nextcode {
namespace sys {

// Platform-independent implementation of Task,
// a particular platform can provide its own more efficient version.
class Task {
public:
  /// The path to the executable which this Task will execute.
  const char *ExecPath;

  /// Any arguments which should be passed during execution.
  llvm::ArrayRef<const char *> Args;

  /// The environment which should be used during execution. If empty,
  /// the current process's environment will be used instead.
  llvm::ArrayRef<const char *> Env;

  /// Context associated with this Task.
  void *Context;

  /// True if the errors of the Task should be stored in Errors instead of
  /// Output.
  bool SeparateErrors;

  llvm::SmallString<64> StdoutPath;

  llvm::SmallString<64> StderrPath;

  llvm::sys::ProcessInfo PI;

  Task(const char *ExecPath, llvm::ArrayRef<const char *> Args,
       llvm::ArrayRef<const char *> Env = std::nullopt, void *Context = nullptr,
       bool SeparateErrors = false)
      : ExecPath(ExecPath), Args(Args), Env(Env), Context(Context),
        SeparateErrors(SeparateErrors) {}

  /// Begins execution of this Task.
  /// \returns true on error.
  bool execute();
};

bool Task::execute() {
  llvm::SmallVector<const char *, 128> Argv;
  Argv.push_back(ExecPath);
  Argv.append(Args.begin(), Args.end());
  Argv.push_back(nullptr);

  std::optional<llvm::ArrayRef<llvm::StringRef>> Envp =
      Env.empty() ? decltype(Envp)(std::nullopt)
                  : decltype(Envp)(llvm::toStringRefArray(Env.data()));

  if (llvm::sys::fs::createTemporaryFile("stdout", "tmp", StdoutPath))
    return true;
  llvm::sys::RemoveFileOnSignal(StdoutPath);

  if (SeparateErrors) {
    if (llvm::sys::fs::createTemporaryFile("stderr", "tmp", StderrPath))
      return true;
    llvm::sys::RemoveFileOnSignal(StderrPath);
  }

  std::optional<llvm::StringRef> Redirects[] = {
      std::nullopt,
      llvm::StringRef{StdoutPath},
      llvm::StringRef{SeparateErrors ? StderrPath : StdoutPath},
  };

  bool ExecutionFailed = false;
  PI = llvm::sys::ExecuteNoWait(ExecPath, llvm::toStringRefArray(Argv.data()),
                                Envp, Redirects, /*memoryLimit*/ 0,
                                /*ErrMsg*/ nullptr, &ExecutionFailed);
  return ExecutionFailed;
}

} // namespace sys
} // namespace nextcode
