//===--- Clock.cpp - Time and clock resolution ----------------------------===//
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

#include "nextcode/Runtime/Concurrency.h"
#include "nextcode/Runtime/Once.h"

#include <time.h>
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <realtimeapiset.h>
#endif

#include "Error.h"

using namespace nextcode;

NEXTCODE_EXPORT_FROM(nextcode_Concurrency)
NEXTCODE_CC(nextcode)
void nextcode_get_time(
  long long *seconds,
  long long *nanoseconds,
  nextcode_clock_id clock_id) {
  switch (clock_id) {
    case nextcode_clock_id_continuous: {
      struct timespec continuous;
#if defined(__linux__)
      clock_gettime(CLOCK_BOOTTIME, &continuous);
#elif defined(__APPLE__)
      clock_gettime(CLOCK_MONOTONIC_RAW, &continuous);
#elif (defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__wasi__))
      clock_gettime(CLOCK_MONOTONIC, &continuous);
#elif defined(_WIN32)
      LARGE_INTEGER freq;
      QueryPerformanceFrequency(&freq);
      LARGE_INTEGER count;
      QueryPerformanceCounter(&count);
      // Divide count (number of ticks) by frequency (number of ticks per
      // second) to get the counter in seconds. We also need to multiply the
      // count by 1,000,000,000 to get nanosecond resolution. By multiplying
      // first, we maintain high precision. The resulting value is the tick
      // count in nanoseconds. Use 128-bit math to avoid overflowing.
      auto quadPart = static_cast<unsigned _BitInt(128)>(count.QuadPart);
      auto ns = (quadPart * 1'000'000'000) / freq.QuadPart;
      continuous.tv_sec = ns / 1'000'000'000;
      continuous.tv_nsec = ns % 1'000'000'000;
#else
#error Missing platform continuous time definition
#endif
      *seconds = continuous.tv_sec;
      *nanoseconds = continuous.tv_nsec;
      return;
    }
    case nextcode_clock_id_suspending: {
      struct timespec suspending;
#if defined(__linux__)
      clock_gettime(CLOCK_MONOTONIC, &suspending);
#elif defined(__APPLE__)
      clock_gettime(CLOCK_UPTIME_RAW, &suspending);
#elif defined(__wasi__)
      clock_gettime(CLOCK_MONOTONIC, &suspending);
#elif (defined(__OpenBSD__) || defined(__FreeBSD__))
      clock_gettime(CLOCK_UPTIME, &suspending);
#elif defined(_WIN32)
      // QueryUnbiasedInterruptTimePrecise() was added in Windows 10 and is, as
      // the name suggests, more precise than QueryUnbiasedInterruptTime().
      // Unfortunately, the symbol is not listed in any .lib file in the SDK and
      // must be looked up dynamically at runtime even if our minimum deployment
      // target is Windows 10.
      typedef decltype(QueryUnbiasedInterruptTimePrecise) *QueryUITP_FP;
      static QueryUITP_FP queryUITP = nullptr;
      static nextcode::once_t onceToken;
      nextcode::once(onceToken, [] {
        if (HMODULE hKernelBase = GetModuleHandleW(L"KernelBase.dll")) {
          queryUITP = reinterpret_cast<QueryUITP_FP>(
            GetProcAddress(hKernelBase, "QueryUnbiasedInterruptTimePrecise")
          );
        }
      });

      // Call whichever API is available. Both output a value measured in 100ns
      // units. We must divide the output by 10,000,000 to get a value in
      // seconds and multiply the remainder by 100 to get nanoseconds.
      ULONGLONG unbiasedTime;
      if (queryUITP) {
        (* queryUITP)(&unbiasedTime);
      } else {
        // Fall back to the older, less precise API.
        (void)QueryUnbiasedInterruptTime(&unbiasedTime);
      }
      suspending.tv_sec = unbiasedTime / 10'000'000;
      suspending.tv_nsec = (unbiasedTime % 10'000'000) * 100;
#else
#error Missing platform suspending time definition
#endif
      *seconds = suspending.tv_sec;
      *nanoseconds = suspending.tv_nsec;
      return;
    }
  }
  nextcode_Concurrency_fatalError(0, "Fatal error: invalid clock ID %d\n",
                               clock_id);
}

NEXTCODE_EXPORT_FROM(nextcode_Concurrency)
NEXTCODE_CC(nextcode)
void nextcode_get_clock_res(
  long long *seconds,
  long long *nanoseconds,
  nextcode_clock_id clock_id) {
switch (clock_id) {
    case nextcode_clock_id_continuous: {
      struct timespec continuous;
#if defined(__linux__)
      clock_getres(CLOCK_BOOTTIME, &continuous);
#elif defined(__APPLE__)
      clock_getres(CLOCK_MONOTONIC_RAW, &continuous);
#elif (defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__wasi__))
      clock_getres(CLOCK_MONOTONIC, &continuous);
#elif defined(_WIN32)
      LARGE_INTEGER freq;
      QueryPerformanceFrequency(&freq);
      continuous.tv_sec = 0;
      continuous.tv_nsec = 1'000'000'000 / freq.QuadPart;
#else
#error Missing platform continuous time definition
#endif
      *seconds = continuous.tv_sec;
      *nanoseconds = continuous.tv_nsec;
      return;
    }
    case nextcode_clock_id_suspending: {
      struct timespec suspending;
#if defined(__linux__)
      clock_getres(CLOCK_MONOTONIC_RAW, &suspending);
#elif defined(__APPLE__)
      clock_getres(CLOCK_UPTIME_RAW, &suspending);
#elif defined(__wasi__)
      clock_getres(CLOCK_MONOTONIC, &suspending);
#elif (defined(__OpenBSD__) || defined(__FreeBSD__))
      clock_getres(CLOCK_UPTIME, &suspending);
#elif defined(_WIN32)
      suspending.tv_sec = 0;
      suspending.tv_nsec = 100;
#else
#error Missing platform suspending time definition
#endif
      *seconds = suspending.tv_sec;
      *nanoseconds = suspending.tv_nsec;
      return;
    }
  }
  nextcode_Concurrency_fatalError(0, "Fatal error: invalid clock ID %d\n",
                               clock_id);
}
