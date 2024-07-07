//==--- Win32Defs.h - Windows API definitions ------------------ -*-C++ -*-===//
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
// We cannot include <windows.h> from the Threading headers because they get
// included all over the place and <windows.h> defines a large number of
// obnoxious macros.  Instead, this header declares *just* what we need.
//
// If you need <windows.h> in a file, please make sure to include it *before*
// this file, or you'll get errors about RTL_SRWLOCK.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_THREADING_IMPL_WIN32_DEFS_H
#define NEXTCODE_THREADING_IMPL_WIN32_DEFS_H

#define DECLSPEC_IMPORT __declspec(dllimport)
#define WINBASEAPI DECLSPEC_IMPORT
#define WINAPI __stdcall
#define NTAPI __stdcall

// <windows.h> #defines VOID rather than typedefing it(!)  Changing that
// to use a typedef instead isn't problematic later on, so let's do that.
#undef VOID

typedef void VOID, *PVOID;
typedef unsigned char BYTE;
typedef BYTE BOOLEAN;
typedef int BOOL;
typedef unsigned long DWORD;
typedef unsigned long ULONG;

typedef VOID(NTAPI *PFLS_CALLBACK_FUNCTION)(PVOID lpFlsData);

typedef struct _RTL_SRWLOCK *PRTL_SRWLOCK;
typedef PRTL_SRWLOCK PSRWLOCK;

typedef struct _RTL_CONDITION_VARIABLE *PRTL_CONDITION_VARIABLE;
typedef PRTL_CONDITION_VARIABLE PCONDITION_VARIABLE;

// These have to be #defines, to avoid problems with <windows.h>
#define RTL_SRWLOCK_INIT {0}
#define SRWLOCK_INIT RTL_SRWLOCK_INIT
#define FLS_OUT_OF_INDEXES ((DWORD)0xFFFFFFFF)

#define RTL_CONDITION_VARIABLE_INIT {0}
#define CONDITION_VARIABLE_INIT RTL_CONDITION_VARIABLE_INIT

#define RTL_CONDITION_VARIABLE_LOCKMODE_SHARED 0x1
#define CONDITION_VARIABLE_LOCKMODE_SHARED RTL_CONDITION_VARIABLE_LOCKMODE_SHARED

#define INFINITE 0xFFFFFFFF // Infinite timeout

extern "C" {
WINBASEAPI DWORD WINAPI GetCurrentThreadId(VOID);

WINBASEAPI VOID WINAPI InitializeSRWLock(PSRWLOCK SRWLock);
WINBASEAPI VOID WINAPI ReleaseSRWLockExclusive(PSRWLOCK SRWLock);
WINBASEAPI VOID WINAPI AcquireSRWLockExclusive(PSRWLOCK SRWLock);
WINBASEAPI BOOLEAN WINAPI TryAcquireSRWLockExclusive(PSRWLOCK SRWLock);

WINBASEAPI VOID WINAPI InitializeConditionVariable(
  PCONDITION_VARIABLE ConditionVariable
);
WINBASEAPI VOID WINAPI WakeConditionVariable(
  PCONDITION_VARIABLE ConditionVariable
);
WINBASEAPI VOID WINAPI WakeAllConditionVariable(
  PCONDITION_VARIABLE ConditionVariable
);
WINBASEAPI BOOL WINAPI SleepConditionVariableSRW(
  PCONDITION_VARIABLE ConditionVariable,
  PSRWLOCK SRWLock,
  DWORD dwMilliseconds,
  ULONG Flags
);

WINBASEAPI DWORD WINAPI FlsAlloc(PFLS_CALLBACK_FUNCTION lpCallback);
WINBASEAPI PVOID WINAPI FlsGetValue(DWORD dwFlsIndex);
WINBASEAPI BOOL WINAPI FlsSetValue(DWORD dwFlsIndex, PVOID lpFlsData);
WINBASEAPI BOOL WINAPI FlsFree(DWORD dwFlsIndex);
}

namespace nextcode {
namespace threading_impl {

// We do this because we can't declare _RTL_SRWLOCK here in case someone
// later includes <windows.h>
struct NEXTCODE_SRWLOCK {
  PVOID Ptr;
};

typedef NEXTCODE_SRWLOCK *PNEXTCODE_SRWLOCK;

inline VOID InitializeSRWLock(PNEXTCODE_SRWLOCK SRWLock) {
  ::InitializeSRWLock(reinterpret_cast<PSRWLOCK>(SRWLock));
}
inline VOID ReleaseSRWLockExclusive(PNEXTCODE_SRWLOCK SRWLock) {
  ::ReleaseSRWLockExclusive(reinterpret_cast<PSRWLOCK>(SRWLock));
}
inline VOID AcquireSRWLockExclusive(PNEXTCODE_SRWLOCK SRWLock) {
  ::AcquireSRWLockExclusive(reinterpret_cast<PSRWLOCK>(SRWLock));
}
inline BOOLEAN TryAcquireSRWLockExclusive(PNEXTCODE_SRWLOCK SRWLock) {
  return ::TryAcquireSRWLockExclusive(reinterpret_cast<PSRWLOCK>(SRWLock));
}

// Similarly we have the same problem with _RTL_CONDITION_VARIABLE
struct NEXTCODE_CONDITION_VARIABLE {
  PVOID Ptr;
};

typedef NEXTCODE_CONDITION_VARIABLE *PNEXTCODE_CONDITION_VARIABLE;

inline VOID InitializeConditionVariable(PNEXTCODE_CONDITION_VARIABLE CondVar) {
  ::InitializeConditionVariable(reinterpret_cast<PCONDITION_VARIABLE>(CondVar));
}
inline VOID WakeConditionVariable(PNEXTCODE_CONDITION_VARIABLE CondVar) {
  ::WakeConditionVariable(reinterpret_cast<PCONDITION_VARIABLE>(CondVar));
}
inline VOID WakeAllConditionVariable(PNEXTCODE_CONDITION_VARIABLE CondVar) {
  ::WakeAllConditionVariable(reinterpret_cast<PCONDITION_VARIABLE>(CondVar));
}
inline BOOL SleepConditionVariableSRW(PNEXTCODE_CONDITION_VARIABLE CondVar,
                                      PNEXTCODE_SRWLOCK SRWLock,
                                      DWORD dwMilliseconds,
                                      ULONG Flags) {
  return ::SleepConditionVariableSRW(
    reinterpret_cast<PCONDITION_VARIABLE>(CondVar),
    reinterpret_cast<PSRWLOCK>(SRWLock),
    dwMilliseconds,
    Flags);
}

} // namespace threading_impl
} // namespace nextcode

#endif // NEXTCODE_THREADING_IMPL_WIN32_DEFS_H
