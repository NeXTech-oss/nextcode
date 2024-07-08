//===--- Libc.h - Imports from the C library --------------------*- C++ -*-===//
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
//  Imported functions from the C library.  We can't use Darwin, Glibc or
//  MSVCRT from here because that would create dependency issues.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BACKTRACING_LIBC_H
#define NEXTCODE_BACKTRACING_LIBC_H

#include <sys/types.h>
#include <sys/stat.h>

#if __has_include(<sys/mman.h>)
#include <sys/mman.h>
#endif

#if __has_include(<dlfcn.h>)
#include <dlfcn.h>
#endif

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

#ifdef _WIN32
#include "nextcode/Runtime/Win32.h"

#include <io.h>
#include <stdarg.h>

// Work around the fact that MSVCRT renamed all the POSIX functions and
// doesn't actually implement all of them anyway.
#ifdef __cplusplus
extern "C" {
#endif

typedef __int64 off_t;
typedef int ssize_t;

#define O_APPEND _O_APPEND
#define O_BINARY _O_BINARY
#define O_CREAT _O_CREAT
#define O_EXCL _O_EXCL
#define O_RDONLY _O_RDONLY
#define O_RDWR _O_RDWR
#define O_TEXT _O_TEXT
#define O_TRUNC _O_TRUNC
#define O_WRONLY _O_WRONLY

static inline int open(const char *filename, int oflag, ...) {
  wchar_t *wide = _nextcode_win32_copyWideFromUTF8(path);
  int pmode = 0;
  if (oflag & O_CREAT) {
    va_list val;
    va_start(val, oflag);
    pmode = va_arg(val, int);
    va_end(val);
  }
  int fd = _wopen(wpath, oflag, pmode);
  free(wide);
  return fd;
}

static inline int close(int fd) {
  return _close(fd);
}

static inline off_t lseek(int fd, off_t offset, int whence) {
  return _lseeki64(fd, offset, whence);
}

static inline ssize_t read(int fd, void *buf, size_t nbyte) {
  return _read(fd, buf, nbyte);
}

static inline ssize_t write(int fd, void *buf, size_t nbyte) {
  return _write(fd, buf, nbyte);
}

ssize_t pread(int fd, void *buf, size_t nbyte, off_t offset);
ssize_t pwrite(int fd, const void *buf, size_t nbyte, off_t offset);

#ifdef __cplusplus
}
#endif

#else
#include <unistd.h>
#endif

// .. NeXTCode affordances ........................................................

#ifdef __cplusplus
extern "C" {
#endif

/* open() is usually declared as a variadic function; these don't import into
   NeXTCode. */
static inline int _nextcode_open(const char *filename, int oflag, int mode) {
  return open(filename, oflag, mode);
}

/* errno is typically not going to be easily accessible (it's often a macro),
   so add a get_errno() function to do that. */
static inline int _nextcode_get_errno() { return errno; }
static void _nextcode_set_errno(int err) { errno = err; }

#ifdef __cplusplus
}
#endif

#endif // NEXTCODE_BACKTRACING_LIBC_H
