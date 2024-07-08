//===--- nextcode_indent_main.cpp - NeXTCode code formatting tool ---------------===//
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
//
//  Formats NeXTCode files or file ranges according to a set of parameters.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/DiagnosticsFrontend.h"
#include "nextcode/Basic/SourceManager.h"
#include "nextcode/Frontend/Frontend.h"
#include "nextcode/Frontend/PrintingDiagnosticConsumer.h"
#include "nextcode/IDE/Indenting.h"
#include "nextcode/Option/Options.h"
#include "nextcode/Subsystems.h"
#include "llvm/Option/Arg.h"
#include "llvm/Option/Option.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"

#include <string>
#include <vector>

using namespace nextcode;
using namespace nextcode::ide;
using namespace llvm::opt;

class FormatterDocument {
private:
  SourceManager SM;
  unsigned BufferID;
  CompilerInvocation CompInv;
  std::unique_ptr<ParserUnit> Parser;
  class FormatterDiagConsumer : public nextcode::DiagnosticConsumer {
    void handleDiagnostic(SourceManager &SM,
                          const nextcode::DiagnosticInfo &Info) override {
      llvm::errs() << "Parse error: ";
      DiagnosticEngine::formatDiagnosticText(llvm::errs(), Info.FormatString,
                                             Info.FormatArgs);
      llvm::errs() << "\n";
    }
  } DiagConsumer;

public:
  FormatterDocument(std::unique_ptr<llvm::MemoryBuffer> Buffer) {
    // Formatting logic requires tokens on source file.
    CompInv.getLangOptions().CollectParsedToken = true;
    updateCode(std::move(Buffer));
  }

  void updateCode(std::unique_ptr<llvm::MemoryBuffer> Buffer) {
    BufferID = SM.addNewSourceBuffer(std::move(Buffer));
    Parser.reset(new ParserUnit(
        SM, SourceFileKind::Main, BufferID, CompInv.getLangOptions(),
        CompInv.getTypeCheckerOptions(), CompInv.getSILOptions(),
        CompInv.getModuleName()));
    Parser->getDiagnosticEngine().addConsumer(DiagConsumer);
    Parser->parse();
  }

  std::pair<LineRange, std::string> reformat(LineRange Range,
                                             CodeFormatOptions Options) {
    return ::reformat(Range, Options, SM, Parser->getSourceFile());
  }

  const llvm::MemoryBuffer &memBuffer() const {
    return *SM.getLLVMSourceMgr().getMemoryBuffer(BufferID);
  }
};

class NeXTCodeIndentInvocation {
private:
  std::string MainExecutablePath;
  std::string OutputFilename = "-";
  std::vector<std::string> InputFilenames;
  CodeFormatOptions FormatOptions;
  bool InPlace = false;
  std::vector<std::string> LineRanges;

  bool parseLineRange(StringRef Input, unsigned &FromLine, unsigned &ToLine) {
    std::pair<StringRef, StringRef> LineRange = Input.split(":");
    return LineRange.first.getAsInteger(0, FromLine) ||
           LineRange.second.getAsInteger(0, ToLine);
  }

public:
  NeXTCodeIndentInvocation(const std::string &ExecPath)
      : MainExecutablePath(ExecPath) {}

  const std::string &getOutputFilename() { return OutputFilename; }

  const std::vector<std::string> &getInputFilenames() { return InputFilenames; }

  const std::vector<std::string> &getLineRanges() { return LineRanges; }

  int parseArgs(ArrayRef<const char *> Args, DiagnosticEngine &Diags) {
    using namespace options;

    std::unique_ptr<llvm::opt::OptTable> Table = createNeXTCodeOptTable();
    unsigned MissingIndex;
    unsigned MissingCount;
    llvm::opt::InputArgList ParsedArgs =
        Table->ParseArgs(Args, MissingIndex, MissingCount, NeXTCodeIndentOption);
    if (MissingCount) {
      Diags.diagnose(SourceLoc(), diag::error_missing_arg_value,
                     ParsedArgs.getArgString(MissingIndex), MissingCount);
      return 1;
    }

    if (ParsedArgs.getLastArg(OPT_use_tabs))
      FormatOptions.UseTabs = true;

    if (ParsedArgs.getLastArg(OPT_indent_switch_case))
      FormatOptions.IndentSwitchCase = true;

    if (ParsedArgs.getLastArg(OPT_in_place))
      InPlace = true;

    if (const Arg *A = ParsedArgs.getLastArg(OPT_tab_width))
      if (StringRef(A->getValue()).getAsInteger(10, FormatOptions.TabWidth))
        Diags.diagnose(SourceLoc(), diag::error_invalid_arg_value,
                       A->getAsString(ParsedArgs), A->getValue());

    if (const Arg *A = ParsedArgs.getLastArg(OPT_indent_width))
      if (StringRef(A->getValue()).getAsInteger(10, FormatOptions.IndentWidth))
        Diags.diagnose(SourceLoc(), diag::error_invalid_arg_value,
                       A->getAsString(ParsedArgs), A->getValue());

    for (const Arg *A : ParsedArgs.filtered(OPT_line_range))
      LineRanges.push_back(A->getValue());

    if (ParsedArgs.hasArg(OPT_UNKNOWN)) {
      for (const Arg *A : ParsedArgs.filtered(OPT_UNKNOWN)) {
        Diags.diagnose(SourceLoc(), diag::error_unknown_arg,
                       A->getAsString(ParsedArgs));
      }
      return true;
    }

    if (ParsedArgs.getLastArg(OPT_help)) {
      std::string ExecutableName =
          llvm::sys::path::stem(MainExecutablePath).str();
      Table->printHelp(llvm::outs(), ExecutableName.c_str(),
                       "NeXTCode Format Tool", options::NeXTCodeIndentOption, 0,
                       /*ShowAllAliases*/false);
      return 1;
    }

    for (const Arg *A : ParsedArgs.filtered(OPT_INPUT)) {
      InputFilenames.push_back(A->getValue());
    }

    if (const Arg *A = ParsedArgs.getLastArg(OPT_o)) {
      OutputFilename = A->getValue();
    }

    return 0;
  }

  /// Formats a filename and returns false if successful, true otherwise.
  bool format(StringRef Filename, DiagnosticEngine &Diags) {
    auto ErrOrBuf = llvm::MemoryBuffer::getFileOrSTDIN(Filename);
    if (!ErrOrBuf) {
      Diags.diagnose(SourceLoc(), diag::error_no_such_file_or_directory,
                     Filename);
      return true;
    }
    std::unique_ptr<llvm::MemoryBuffer> Code = std::move(ErrOrBuf.get());
    if (Code->getBufferSize() == 0) {
      // Assume empty files are formatted successfully.
      return false;
    }
    FormatterDocument Doc(std::move(Code));
    if (LineRanges.empty()) {
      LineRanges.push_back("1:" + std::to_string(UINT_MAX));
    }

    std::string Output = Doc.memBuffer().getBuffer().str();
    for (unsigned Range = 0; Range < LineRanges.size(); ++Range) {
      unsigned FromLine;
      unsigned ToLine;
      if (parseLineRange(LineRanges[Range], FromLine, ToLine)) {
        Diags.diagnose(SourceLoc(), diag::error_formatting_invalid_range);
        return true;
      }
      if (FromLine > ToLine) {
        Diags.diagnose(SourceLoc(), diag::error_formatting_invalid_range);
        return true;
      }
      for (unsigned Line = FromLine; Line <= ToLine; ++Line) {
        size_t Offset = getOffsetOfLine(Line, Output);
        ssize_t Length = getOffsetOfLine(Line + 1, Output) - 1 - Offset;
        if (Length < 0)
          break;

        std::string Formatted =
            Doc.reformat(LineRange(Line, 1), FormatOptions).second;
        if (Formatted.find_first_not_of(" \t\v\f", 0) == StringRef::npos)
          Formatted = "";

        Output.replace(Offset, Length, Formatted);
        Doc.updateCode(llvm::MemoryBuffer::getMemBufferCopy(Output));
      }
      if (Filename == "-" || (!InPlace && OutputFilename == "-")) {
        llvm::outs() << Output;
        return false;
      }
      std::error_code EC;
      StringRef Destination;
      if (InPlace)
        Destination = Filename;
      else
        Destination = OutputFilename;
      llvm::raw_fd_ostream out(Destination, EC, llvm::sys::fs::OF_None);
      if (out.has_error() || EC) {
        Diags.diagnose(SourceLoc(), diag::error_opening_output, Filename,
                       EC.message());
        out.clear_error();
        return true;
      }
      out << Output;
    }
    return false;
  }
};

int nextcode_indent_main(ArrayRef<const char *> Args, const char *Argv0,
                      void *MainAddr) {
  CompilerInstance Instance;
  PrintingDiagnosticConsumer PDC;
  Instance.addDiagnosticConsumer(&PDC);

  NeXTCodeIndentInvocation Invocation(
      llvm::sys::fs::getMainExecutable(Argv0, MainAddr));

  DiagnosticEngine &Diags = Instance.getDiags();
  if (Invocation.parseArgs(Args, Diags) != 0)
    return EXIT_FAILURE;

  std::vector<std::string> InputFiles = Invocation.getInputFilenames();
  unsigned NumInputFiles = InputFiles.size();
  if (NumInputFiles == 0) {
    // Read source code from standard input.
    Invocation.format("-", Diags);
  } else if (NumInputFiles == 1) {
    Invocation.format(InputFiles[0], Diags);
  } else {
    if (!Invocation.getLineRanges().empty()) {
      // We don't support formatting file ranges for multiple files.
      Instance.getDiags().diagnose(SourceLoc(),
                                   diag::error_formatting_multiple_file_ranges);
      return EXIT_FAILURE;
    }
    for (unsigned i = 0; i < NumInputFiles; ++i)
      Invocation.format(InputFiles[i], Diags);
  }

  return EXIT_SUCCESS;
}
