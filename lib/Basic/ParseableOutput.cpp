//===-------- ParseableOutput.cpp - Helpers for parseable output ----------===//
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

#include "nextcode/Basic/ParseableOutput.h"
#include "nextcode/Basic/FileTypes.h"
#include "nextcode/Basic/JSONSerialization.h"
#include "nextcode/Basic/TaskQueue.h"
#include "nextcode/Driver/Action.h"
#include "nextcode/Driver/Job.h"
#include "llvm/Option/Arg.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/raw_ostream.h"

#include <sstream>

using namespace nextcode::parseable_output;
using namespace nextcode::driver;
using namespace nextcode::sys;
using namespace nextcode;

namespace nextcode {
namespace json {
template <> struct ScalarTraits<CommandInput> {
  static void output(const CommandInput &value, llvm::raw_ostream &os) {
    os << value.Path;
  }
  static bool mustQuote(StringRef) { return true; }
};

template <> struct ScalarEnumerationTraits<file_types::ID> {
  static void enumeration(Output &out, file_types::ID &value) {
    file_types::forAllTypes([&](file_types::ID ty) {
      std::string typeName = file_types::getTypeName(ty).str();
      out.enumCase(value, typeName.c_str(), ty);
    });
    out.enumCase(value, "unknown", file_types::ID::TY_INVALID);
  }
};

template <> struct ObjectTraits<std::pair<file_types::ID, std::string>> {
  static void mapping(Output &out,
                      std::pair<file_types::ID, std::string> &value) {
    out.mapRequired("type", value.first);
    out.mapRequired("path", value.second);
  }
};

template <typename T, unsigned N> struct ArrayTraits<SmallVector<T, N>> {
  static size_t size(Output &out, SmallVector<T, N> &seq) { return seq.size(); }

  static T &element(Output &out, SmallVector<T, N> &seq, size_t index) {
    if (index >= seq.size())
      seq.resize(index + 1);
    return seq[index];
  }
};
} // namespace json
} // namespace nextcode

namespace {

class Message {
  std::string Kind;
  std::string Name;

public:
  Message(StringRef Kind, StringRef Name) : Kind(Kind), Name(Name) {}
  virtual ~Message() = default;

  virtual void provideMapping(nextcode::json::Output &out) {
    out.mapRequired("kind", Kind);
    out.mapRequired("name", Name);
  }
};

class DetailedMessage : public Message {
  DetailedTaskDescription TascDesc;

public:
  DetailedMessage(StringRef Kind, StringRef Name,
                  DetailedTaskDescription TascDesc)
      : Message(Kind, Name), TascDesc(TascDesc) {}

  void provideMapping(nextcode::json::Output &out) override {
    Message::provideMapping(out);
    out.mapRequired("command",
                    TascDesc.CommandLine); // Deprecated, do not document
    out.mapRequired("command_executable", TascDesc.Executable);
    out.mapRequired("command_arguments", TascDesc.Arguments);
    out.mapOptional("inputs", TascDesc.Inputs);
    out.mapOptional("outputs", TascDesc.Outputs);
  }
};


class BeganMessage : public DetailedMessage {
  int64_t Pid;
  sys::TaskProcessInformation ProcInfo;

public:
  BeganMessage(StringRef Name, DetailedTaskDescription TascDesc,
               int64_t Pid, sys::TaskProcessInformation ProcInfo)
      : DetailedMessage("began", Name, TascDesc),
        Pid(Pid), ProcInfo(ProcInfo) {}

  void provideMapping(nextcode::json::Output &out) override {
    DetailedMessage::provideMapping(out);
    out.mapRequired("pid", Pid);
    out.mapRequired("process", ProcInfo);
  }
};

class SkippedMessage : public DetailedMessage {
public:
  SkippedMessage(StringRef Name, DetailedTaskDescription TascDesc)
      : DetailedMessage("skipped", Name, TascDesc) {}
};

class TaskOutputMessage : public Message {
  std::string Output;
  int64_t Pid;
  sys::TaskProcessInformation ProcInfo;

public:
  TaskOutputMessage(StringRef Kind, StringRef Name, std::string Output,
                    int64_t Pid, sys::TaskProcessInformation ProcInfo)
      : Message(Kind, Name),
        Output(Output), Pid(Pid), ProcInfo(ProcInfo) {}

  void provideMapping(nextcode::json::Output &out) override {
    Message::provideMapping(out);
    out.mapRequired("pid", Pid);
    out.mapOptional("output", Output, std::string());
    out.mapRequired("process", ProcInfo);
  }
};

class SignalledMessage : public TaskOutputMessage {
  std::string ErrorMsg;
  std::optional<int> Signal;

public:
  SignalledMessage(StringRef Name, std::string Output, int64_t Pid,
                   sys::TaskProcessInformation ProcInfo, StringRef ErrorMsg,
                   std::optional<int> Signal)
      : TaskOutputMessage("signalled", Name, Output, Pid, ProcInfo),
        ErrorMsg(ErrorMsg), Signal(Signal) {}

  void provideMapping(nextcode::json::Output &out) override {
    TaskOutputMessage::provideMapping(out);
    out.mapOptional("error-message", ErrorMsg, std::string());
    out.mapOptional("signal", Signal);
  }
};

class FinishedMessage : public TaskOutputMessage {
  int ExitStatus;
public:
  FinishedMessage(StringRef Name, std::string Output,
                  int64_t Pid, sys::TaskProcessInformation ProcInfo,
                  int ExitStatus)
      : TaskOutputMessage("finished", Name, Output, Pid, ProcInfo),
        ExitStatus(ExitStatus) {}

  void provideMapping(nextcode::json::Output &out) override {
    TaskOutputMessage::provideMapping(out);
    out.mapRequired("exit-status", ExitStatus);
  }
};

} // end anonymous namespace

namespace nextcode {
namespace json {

template <> struct ObjectTraits<Message> {
  static void mapping(Output &out, Message &msg) { msg.provideMapping(out); }
};

} // namespace json
} // namespace nextcode

static void emitMessage(raw_ostream &os, Message &msg) {
  std::string JSONString;
  llvm::raw_string_ostream BufferStream(JSONString);
  json::Output yout(BufferStream);
  yout << msg;
  BufferStream.flush();
  os << JSONString.length() << '\n';
  os << JSONString << '\n';
}

/// Emits a "began" message to the given stream.
void parseable_output::emitBeganMessage(raw_ostream &os, StringRef Name,
                                        DetailedTaskDescription TascDesc,
                                        int64_t Pid,
                                        sys::TaskProcessInformation ProcInfo) {
  BeganMessage msg(Name, TascDesc, Pid, ProcInfo);
  emitMessage(os, msg);
}

void parseable_output::emitFinishedMessage(
    raw_ostream &os, StringRef Name, std::string Output, int ExitStatus,
    int64_t Pid, sys::TaskProcessInformation ProcInfo) {
  FinishedMessage msg(Name, Output, Pid, ProcInfo, ExitStatus);
  emitMessage(os, msg);
}

void parseable_output::emitSignalledMessage(
    raw_ostream &os, StringRef Name, StringRef ErrorMsg, StringRef Output,
    std::optional<int> Signal, int64_t Pid, TaskProcessInformation ProcInfo) {
  SignalledMessage msg(Name, Output.str(), Pid, ProcInfo, ErrorMsg, Signal);
  emitMessage(os, msg);
}

void parseable_output::emitSkippedMessage(raw_ostream &os, StringRef Name,
                                          DetailedTaskDescription TascDesc) {
  SkippedMessage msg(Name, TascDesc);
  emitMessage(os, msg);
}

