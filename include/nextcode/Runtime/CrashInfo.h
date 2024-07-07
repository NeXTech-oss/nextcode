//===--- CrashInfo.h - NeXTCode Backtracing Crash Information ------*- C++ -*-===//
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
//  Defines the CrashInfo type that holds information about why the program
//  crashed.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_CRASHINFO_H
#define NEXTCODE_CRASHINFO_H

#include <inttypes.h>

#ifdef __cplusplus
namespace nextcode {
namespace runtime {
namespace backtrace {
extern "C" {
#endif

// Note: The "pointers" below are pointers in a different process's address
//       space, which might not even share our bitness.  That is why they are
//       `uint64_t`s, rather than pointers or `uintptr_t`s.

// The address of this structure in memory is passed to nextcode-backtrace.
struct CrashInfo {
  // The thread ID for the crashing thread.
  uint64_t crashing_thread;

  // The signal number corresponding to this crash.
  uint64_t signal;

  // The fault address.
  uint64_t fault_address;

#ifdef __APPLE__
  // Points to the mcontext_t structure for the crashing thread; other
  // threads' contexts can be recovered using Mach APIs later.
  uint64_t mctx;
#elif defined(__linux__)
  // The head of the thread list; points at a "struct thread" (see below).
  uint64_t thread_list;
#endif
};

#ifdef __linux__

// A memory server request packet.
struct memserver_req {
  // Address to read.
  uint64_t addr;

  // Number of bytes to read.
  uint64_t len;
};

// A memory server response packet.
struct memserver_resp {
  // Address that was read from.
  uint64_t addr;

  // Number of bytes, *or* negative to indicate an error.
  int64_t  len;

  // Followed by len bytes of data if len > 0
};

// Holds details of a running thread.
struct thread {
  // Points at the next thread.
  uint64_t next;

  // The thread ID for this thread.
  int64_t  tid;

  // Points to the Linux ucontext_t structure.
  uint64_t uctx;
};

#endif

#ifdef __cplusplus
} // extern "C"
} // namespace backtrace
} // namespace runtime
} // namespace nextcode
#endif

#endif // NEXTCODE_CRASHINFO_H
