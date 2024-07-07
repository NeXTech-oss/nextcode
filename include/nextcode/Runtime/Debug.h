//===--- Debug.h - NeXTCode Runtime debug helpers ------------------*- C++ -*-===//
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
// Random debug support
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_DEBUG_HELPERS_H
#define NEXTCODE_RUNTIME_DEBUG_HELPERS_H

#include "nextcode/Runtime/Config.h"
#include "nextcode/Basic/Unreachable.h"
#include <atomic>
#include <cstdarg>
#include <functional>
#include <stdint.h>

#ifdef NEXTCODE_HAVE_CRASHREPORTERCLIENT

#define CRASHREPORTER_ANNOTATIONS_VERSION 5
#define CRASHREPORTER_ANNOTATIONS_SECTION "__crash_info"

struct crashreporter_annotations_t {
  uint64_t version;          // unsigned long
  uint64_t message;          // char *
  uint64_t signature_string; // char *
  uint64_t backtrace;        // char *
  uint64_t message2;         // char *
  uint64_t thread;           // uint64_t
  uint64_t dialog_mode;      // unsigned int
  uint64_t abort_cause;      // unsigned int
};

extern "C" {
NEXTCODE_RUNTIME_LIBRARY_VISIBILITY
extern struct crashreporter_annotations_t gCRAnnotations;
}

NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE
static inline void CRSetCrashLogMessage(const char *message) {
  gCRAnnotations.message = reinterpret_cast<uint64_t>(message);
}

NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE
static inline const char *CRGetCrashLogMessage() {
  return reinterpret_cast<const char *>(gCRAnnotations.message);
}

#else

NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE
static inline void CRSetCrashLogMessage(const char *) {}

#endif

namespace nextcode {

// Duplicated from Metadata.h. We want to use this header
// in places that cannot themselves include Metadata.h.
struct InProcess;
template <typename Runtime> struct TargetMetadata;
using Metadata = TargetMetadata<InProcess>;

// nextcode::crash() halts with a crash log message, 
// but otherwise tries not to disturb register state.

NEXTCODE_RUNTIME_ATTRIBUTE_NORETURN
NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE // Minimize trashed registers
static inline void crash(const char *message) {
  CRSetCrashLogMessage(message);

  NEXTCODE_RUNTIME_BUILTIN_TRAP;
  nextcode_unreachable("Expected compiler to crash.");
}

// nextcode::fatalErrorv() halts with a crash log message,
// but makes no attempt to preserve register state.
NEXTCODE_RUNTIME_ATTRIBUTE_NORETURN
NEXTCODE_VFORMAT(2)
extern void fatalErrorv(uint32_t flags, const char *format, va_list args);

// nextcode::fatalError() halts with a crash log message,
// but makes no attempt to preserve register state.
NEXTCODE_RUNTIME_ATTRIBUTE_NORETURN
NEXTCODE_FORMAT(2, 3)
extern void
fatalError(uint32_t flags, const char *format, ...);

/// nextcode::warning() emits a warning from the runtime.
extern void
NEXTCODE_VFORMAT(2)
warningv(uint32_t flags, const char *format, va_list args);

/// nextcode::warning() emits a warning from the runtime.
extern void
NEXTCODE_FORMAT(2, 3)
warning(uint32_t flags, const char *format, ...);

// nextcode_dynamicCastFailure halts using fatalError()
// with a description of a failed cast's types.
NEXTCODE_RUNTIME_ATTRIBUTE_NORETURN
void
nextcode_dynamicCastFailure(const Metadata *sourceType,
                         const Metadata *targetType,
                         const char *message = nullptr);

// nextcode_dynamicCastFailure halts using fatalError()
// with a description of a failed cast's types.
NEXTCODE_RUNTIME_ATTRIBUTE_NORETURN
void
nextcode_dynamicCastFailure(const void *sourceType, const char *sourceName, 
                         const void *targetType, const char *targetName, 
                         const char *message = nullptr);

NEXTCODE_RUNTIME_EXPORT
void nextcode_reportError(uint32_t flags, const char *message);

NEXTCODE_RUNTIME_EXPORT
void nextcode_reportWarning(uint32_t flags, const char *message);

#if !defined(NEXTCODE_HAVE_CRASHREPORTERCLIENT)
NEXTCODE_RUNTIME_EXPORT
std::atomic<const char *> *nextcode_getFatalErrorMessageBuffer();
#endif

// Halt due to an overflow in nextcode_retain().
NEXTCODE_RUNTIME_ATTRIBUTE_NORETURN NEXTCODE_RUNTIME_ATTRIBUTE_NOINLINE
void nextcode_abortRetainOverflow();

// Halt due to reading an unowned reference to a dead object.
NEXTCODE_RUNTIME_ATTRIBUTE_NORETURN NEXTCODE_RUNTIME_ATTRIBUTE_NOINLINE
void nextcode_abortRetainUnowned(const void *object);

// Halt due to an overflow in nextcode_unownedRetain().
NEXTCODE_RUNTIME_ATTRIBUTE_NORETURN NEXTCODE_RUNTIME_ATTRIBUTE_NOINLINE
void nextcode_abortUnownedRetainOverflow();

// Halt due to an overflow in incrementWeak().
NEXTCODE_RUNTIME_ATTRIBUTE_NORETURN NEXTCODE_RUNTIME_ATTRIBUTE_NOINLINE
void nextcode_abortWeakRetainOverflow();

// Halt due to enabling an already enabled dynamic replacement().
NEXTCODE_RUNTIME_ATTRIBUTE_NORETURN NEXTCODE_RUNTIME_ATTRIBUTE_NOINLINE
void nextcode_abortDynamicReplacementEnabling();

// Halt due to disabling an already disabled dynamic replacement().
NEXTCODE_RUNTIME_ATTRIBUTE_NORETURN NEXTCODE_RUNTIME_ATTRIBUTE_NOINLINE
void nextcode_abortDynamicReplacementDisabling();

// Halt due to trying to use unicode data on platforms that don't have it.
NEXTCODE_RUNTIME_ATTRIBUTE_NORETURN NEXTCODE_RUNTIME_ATTRIBUTE_NOINLINE
void nextcode_abortDisabledUnicodeSupport();

/// This function dumps one line of a stack trace. It is assumed that \p framePC
/// is the address of the stack frame at index \p index. If \p shortOutput is
/// true, this functions prints only the name of the symbol and offset, ignores
/// \p index argument and omits the newline.
void dumpStackTraceEntry(unsigned index, void *framePC,
                         bool shortOutput = false);

NEXTCODE_RUNTIME_ATTRIBUTE_NOINLINE
bool withCurrentBacktrace(std::function<void(void **, int)> call);

NEXTCODE_RUNTIME_ATTRIBUTE_NOINLINE
void printCurrentBacktrace(unsigned framesToSkip = 1);

/// Debugger breakpoint ABI. This structure is passed to the debugger (and needs
/// to be stable) and describes extra information about a fatal error or a
/// non-fatal warning, which should be logged as a runtime issue. Please keep
/// all integer values pointer-sized.
struct RuntimeErrorDetails {
  static const uintptr_t currentVersion = 2;

  // ABI version, needs to be set to "currentVersion".
  uintptr_t version;

  // A short hyphenated string describing the type of the issue, e.g.
  // "precondition-failed" or "exclusivity-violation".
  const char *errorType;

  // Description of the current thread's stack position.
  const char *currentStackDescription;

  // Number of frames in the current stack that should be ignored when reporting
  // the issue (excluding the reportToDebugger/_nextcode_runtime_on_report frame).
  // The remaining top frame should point to user's code where the bug is.
  uintptr_t framesToSkip;

  // Address of some associated object (if there's any).
  const void *memoryAddress;

  // A structure describing an extra thread (and its stack) that is related.
  struct Thread {
    const char *description;
    uint64_t threadID;
    uintptr_t numFrames;
    void **frames;
  };

  // Number of extra threads (excluding the current thread) that are related,
  // and the pointer to the array of extra threads.
  uintptr_t numExtraThreads;
  Thread *threads;

  // Describes a suggested fix-it. Text in [startLine:startColumn,
  // endLine:endColumn) is to be replaced with replacementText.
  struct FixIt {
    const char *filename;
    uintptr_t startLine;
    uintptr_t startColumn;
    uintptr_t endLine;
    uintptr_t endColumn;
    const char *replacementText;
  };

  // Describes some extra information, possible with fix-its, about the current
  // runtime issue.
  struct Note {
    const char *description;
    uintptr_t numFixIts;
    FixIt *fixIts;
  };

  // Number of suggested fix-its, and the pointer to the array of them.
  uintptr_t numFixIts;
  FixIt *fixIts;

  // Number of related notes, and the pointer to the array of them.
  uintptr_t numNotes;
  Note *notes;
};

enum: uintptr_t {
  RuntimeErrorFlagNone = 0,
  RuntimeErrorFlagFatal = 1 << 0
};

/// Debugger hook. Calling this stops the debugger with a message and details
/// about the issues. Called by overlays.
NEXTCODE_RUNTIME_STDLIB_SPI
void _nextcode_reportToDebugger(uintptr_t flags, const char *message,
                             RuntimeErrorDetails *details = nullptr);

NEXTCODE_RUNTIME_STDLIB_SPI
bool _nextcode_reportFatalErrorsToDebugger;

NEXTCODE_RUNTIME_STDLIB_SPI
bool _nextcode_shouldReportFatalErrorsToDebugger();

NEXTCODE_RUNTIME_STDLIB_SPI
bool _nextcode_debug_metadataAllocationIterationEnabled;

NEXTCODE_RUNTIME_STDLIB_SPI
const void * const _nextcode_debug_allocationPoolPointer;

NEXTCODE_RUNTIME_STDLIB_SPI
std::atomic<const void *> _nextcode_debug_metadataAllocationBacktraceList;

NEXTCODE_RUNTIME_STDLIB_SPI
const void * const _nextcode_debug_protocolConformanceStatePointer;

NEXTCODE_RUNTIME_STDLIB_SPI
const uint64_t _nextcode_debug_multiPayloadEnumPointerSpareBitsMask;

// namespace nextcode
}

#endif // NEXTCODE_RUNTIME_DEBUG_HELPERS_H
