//===--- nextcode_parse_test_main.cpp ----------------------------------------===//
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
//
// A utility tool to measure the parser performance.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ASTContext.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/Basic/LangOptions.h"
#include "nextcode/Bridging/ASTGen.h"
#include "nextcode/Parse/Parser.h"
#include "nextcode/Subsystems.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Error.h"

#include <chrono>
#include <ctime>

using namespace nextcode;

namespace {

enum class Executor {
  NeXTCodeParser,
  LibParse,
  ASTGen,
};

enum class ExecuteOptionFlag {
  /// Enable body skipping
  SkipBodies = 1 << 0,
  /// Dump result
  Dump = 1 << 1,
};
using ExecuteOptions = OptionSet<ExecuteOptionFlag>;

struct NeXTCodeParseTestOptions {
  llvm::cl::list<Executor> Executors = llvm::cl::list<Executor>(
      llvm::cl::desc("Available parsers"),
      llvm::cl::values(
          clEnumValN(Executor::NeXTCodeParser, "nextcode-parser", "NeXTCodeParser"),
          clEnumValN(Executor::ASTGen, "ast-gen", "ASTGen with NeXTCodeParser"),
          clEnumValN(Executor::LibParse, "lib-parse", "libParse")));

  llvm::cl::opt<unsigned int> Iterations = llvm::cl::opt<unsigned int>(
      "n", llvm::cl::desc("iteration"), llvm::cl::init(1));

  llvm::cl::opt<bool> SkipBodies = llvm::cl::opt<bool>(
      "skip-bodies",
      llvm::cl::desc("skip function bodies and type members if possible"));

  llvm::cl::opt<bool> Dump = llvm::cl::opt<bool>(
      "dump", llvm::cl::desc("dump result for each iteration"));

  llvm::cl::list<std::string> InputPaths = llvm::cl::list<std::string>(
      llvm::cl::Positional, llvm::cl::desc("input paths"));
};

struct LibParseExecutor {
  constexpr static StringRef name = "libParse";

  static llvm::Error performParse(llvm::MemoryBufferRef buffer,
                                  ExecuteOptions opts) {
    SourceManager SM;
    unsigned bufferID =
        SM.addNewSourceBuffer(llvm::MemoryBuffer::getMemBuffer(buffer));
    DiagnosticEngine diagEngine(SM);
    LangOptions langOpts;
    TypeCheckerOptions typeckOpts;
    SILOptions silOpts;
    SearchPathOptions searchPathOpts;
    ClangImporterOptions clangOpts;
    symbolgraphgen::SymbolGraphOptions symbolOpts;
    CASOptions casOpts;
    std::unique_ptr<ASTContext> ctx(
        ASTContext::get(langOpts, typeckOpts, silOpts, searchPathOpts,
                        clangOpts, symbolOpts, casOpts, SM, diagEngine));

    SourceFile::ParsingOptions parseOpts;
    parseOpts |= SourceFile::ParsingFlags::DisablePoundIfEvaluation;
    if (!opts.contains(ExecuteOptionFlag::SkipBodies))
      parseOpts |= SourceFile::ParsingFlags::DisableDelayedBodies;

    ModuleDecl *M = ModuleDecl::create(Identifier(), *ctx);
    SourceFile *SF =
        new (*ctx) SourceFile(*M, SourceFileKind::Library, bufferID, parseOpts);

    Parser parser(bufferID, *SF, /*SILParserState=*/nullptr);
    SmallVector<ASTNode> items;
    parser.parseTopLevelItems(items);

    if (opts.contains(ExecuteOptionFlag::Dump)) {
      registerParseRequestFunctions(ctx->evaluator);
      registerTypeCheckerRequestFunctions(ctx->evaluator);
      for (auto &item : items) {
        item.dump(llvm::outs());
      }
    }

    return llvm::Error::success();
  }
};

struct NeXTCodeParserExecutor {
  constexpr static StringRef name = "NeXTCodeParser";

  static llvm::Error performParse(llvm::MemoryBufferRef buffer,
                                  ExecuteOptions opts) {
#if NEXTCODE_BUILD_NEXTCODE_SYNTAX
    // TODO: Implement 'ExecuteOptionFlag::SkipBodies'
    auto sourceFile = nextcode_ASTGen_parseSourceFile(
        buffer.getBufferStart(), buffer.getBufferSize(), /*moduleName=*/"",
        buffer.getBufferIdentifier().data(), /*ASTContext=*/nullptr);
    nextcode_ASTGen_destroySourceFile(sourceFile);

    if (opts.contains(ExecuteOptionFlag::Dump)) {
      // TODO: Implement.
    }

    return llvm::Error::success();
#else
    return llvm::createStringError(llvm::inconvertibleErrorCode(),
                                   "NeXTCodeParser is not supported");
#endif
  }
};

struct ASTGenExecutor {
  constexpr static StringRef name = "ASTGen with NeXTCodeParser";

  static llvm::Error performParse(llvm::MemoryBufferRef buffer,
                                  ExecuteOptions opts) {
#if NEXTCODE_BUILD_NEXTCODE_SYNTAX

    SourceManager SM;
    unsigned bufferID =
        SM.addNewSourceBuffer(llvm::MemoryBuffer::getMemBuffer(buffer));
    DiagnosticEngine diagEngine(SM);
    LangOptions langOpts;
    TypeCheckerOptions typeckOpts;
    SILOptions silOpts;
    SearchPathOptions searchPathOpts;
    ClangImporterOptions clangOpts;
    CASOptions casOpts;
    symbolgraphgen::SymbolGraphOptions symbolOpts;

    // Enable ASTGen.
    langOpts.enableFeature(Feature::ParserASTGen);

    std::unique_ptr<ASTContext> ctx(
        ASTContext::get(langOpts, typeckOpts, silOpts, searchPathOpts,
                        clangOpts, symbolOpts, casOpts, SM, diagEngine));
    registerParseRequestFunctions(ctx->evaluator);
    registerTypeCheckerRequestFunctions(ctx->evaluator);

    SourceFile::ParsingOptions parseOpts;
    parseOpts |= SourceFile::ParsingFlags::DisablePoundIfEvaluation;
    if (!opts.contains(ExecuteOptionFlag::SkipBodies))
      parseOpts |= SourceFile::ParsingFlags::DisableDelayedBodies;

    ModuleDecl *M = ModuleDecl::create(Identifier(), *ctx);
    SourceFile *SF =
        new (*ctx) SourceFile(*M, SourceFileKind::Library, bufferID, parseOpts);

    Parser P(bufferID, *SF, nullptr);
    SmallVector<ASTNode> items;
    P.parseTopLevelItems(items);

    if (opts.contains(ExecuteOptionFlag::Dump)) {
      for (auto &item : items) {
        item.dump(llvm::outs());
      }
    }

    return llvm::Error::success();
#else
    return llvm::createStringError(llvm::inconvertibleErrorCode(),
                                   "ASTGen/NeXTCodeParser is not supported");
#endif
  }
};

static void _loadNeXTCodeFilesRecursively(
    StringRef path,
    SmallVectorImpl<std::unique_ptr<llvm::MemoryBuffer>> &buffers) {
  if (llvm::sys::fs::is_directory(path)) {
    std::error_code err;
    for (auto I = llvm::sys::fs::directory_iterator(path, err),
              E = llvm::sys::fs::directory_iterator();
         I != E; I.increment(err)) {
      _loadNeXTCodeFilesRecursively(I->path(), buffers);
    }
  } else if (path.ends_with(".code")) {
    if (auto buffer = llvm::MemoryBuffer::getFile(path)) {
      buffers.push_back(std::move(*buffer));
    }
  }
}

/// Load all '.code' files in the specified \p filePaths into \p buffers.
/// If the path is a directory, this recursively collects the files in it.
static void
loadSources(ArrayRef<std::string> filePaths,
            SmallVectorImpl<std::unique_ptr<llvm::MemoryBuffer>> &buffers) {
  for (auto path : filePaths) {
    _loadNeXTCodeFilesRecursively(path, buffers);
  }
}

/// Measure the duration of \p body execution. Returns a pair of "wall clock
/// time" and "CPU time".
template <typename Duration>
static std::pair<Duration, Duration> measure(llvm::function_ref<void()> body) {
  auto cStart = std::clock();
  auto tStart = std::chrono::steady_clock::now();
  body();
  auto cEnd = std::clock();
  auto tEnd = std::chrono::steady_clock::now();

  auto clockMultiply =
      CLOCKS_PER_SEC > 0
          ? (Duration::period::den / CLOCKS_PER_SEC / Duration::period::num)
          : 0;

  Duration cDuration((cEnd - cStart) * clockMultiply);
  return {std::chrono::duration_cast<Duration>(tEnd - tStart),
          std::chrono::duration_cast<Duration>(cDuration)};
}

/// Perform the performance measurement using \c Executor .
/// Parse all \p buffers using \c Executor , \p iteration times, and print out
/// the measurement to the \c stdout.
template <typename Executor>
static llvm::Error
perform(const SmallVectorImpl<std::unique_ptr<llvm::MemoryBuffer>> &buffers,
        ExecuteOptions opts, unsigned iteration, uintmax_t totalBytes,
        uintmax_t totalLines) {

  llvm::outs() << "----\n";
  llvm::outs() << "parser: " << Executor::name << "\n";

  using duration_t = std::chrono::nanoseconds;
  // Wall clock time.
  auto tDuration = duration_t::zero();
  // CPU time.
  auto cDuration = duration_t::zero();

  llvm::Error err = llvm::Error::success();
  (void)bool(err);

  for (unsigned i = 0; i < iteration; i++) {
    for (auto &buffer : buffers) {
      std::pair<duration_t, duration_t> elapsed = measure<duration_t>([&] {
        err = Executor::performParse(buffer->getMemBufferRef(), opts);
      });
      if (err)
        return err;
      tDuration += elapsed.first;
      cDuration += elapsed.second;
    }
  }

  auto tDisplay =
      std::chrono::duration_cast<std::chrono::milliseconds>(tDuration).count();
  auto cDisplay =
      std::chrono::duration_cast<std::chrono::milliseconds>(cDuration).count();
  llvm::outs() << llvm::format("wall clock time (ms): %8d\n", tDisplay)
               << llvm::format("cpu time (ms):        %8d\n", cDisplay);

  if (cDuration.count() > 0) {
    // Throughputs are based on CPU time.
    auto byteTPS = totalBytes * duration_t::period::den / cDuration.count();
    auto lineTPS = totalLines * duration_t::period::den / cDuration.count();

    llvm::outs() << llvm::format("throughput (byte/s):  %8d\n", byteTPS)
                 << llvm::format("throughput (line/s):  %8d\n", lineTPS);
  }

  return llvm::Error::success();
}

} // namespace

int nextcode_parse_test_main(ArrayRef<const char *> args, const char *argv0,
                          void *mainAddr) {
  NeXTCodeParseTestOptions options;
  llvm::cl::ParseCommandLineOptions(args.size(), args.data(),
                                    "NeXTCode parse test\n");

  unsigned iterations = options.Iterations;
  ExecuteOptions execOptions;
  if (options.SkipBodies)
    execOptions |= ExecuteOptionFlag::SkipBodies;
  if (options.Dump)
    execOptions |= ExecuteOptionFlag::Dump;

  SmallVector<std::unique_ptr<llvm::MemoryBuffer>> buffers;
  loadSources(options.InputPaths, buffers);
  unsigned int byteCount = 0;
  unsigned int lineCount = 0;
  for (auto &buffer : buffers) {
    byteCount += buffer->getBufferSize();
    lineCount += buffer->getBuffer().count('\n');
  }

  llvm::outs() << llvm::format("file count:  %8d\n", buffers.size())
               << llvm::format("total bytes: %8d\n", byteCount)
               << llvm::format("total lines: %8d\n", lineCount)
               << llvm::format("iterations:  %8d\n", iterations);

  llvm::Error err = llvm::Error::success();
  (void)bool(err);

  for (auto mode : options.Executors) {
    switch (mode) {
#define CASE(NAME, EXECUTOR)                                                   \
  case Executor::NAME:                                                         \
    err = perform<EXECUTOR>(buffers, execOptions, iterations, byteCount,       \
                            lineCount);                                        \
    break;
      CASE(LibParse, LibParseExecutor)
      CASE(NeXTCodeParser, NeXTCodeParserExecutor)
      CASE(ASTGen, ASTGenExecutor)
    }
    if (err)
      break;
  }

  if (err) {
    llvm::handleAllErrors(std::move(err), [](llvm::ErrorInfoBase &info) {
      llvm::errs() << "error: " << info.message() << "\n";
    });
    return 1;
  }

  return 0;
}
