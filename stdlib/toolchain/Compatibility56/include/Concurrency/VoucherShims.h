//===--- VoucherShims.h - Shims for OS vouchers --------------------*- C++ -*-//
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
// Shims for interfacing with OS voucher calls.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_CONCURRENCY_VOUCHERSHIMS_BACKDEPLOY56_H
#define NEXTCODE_CONCURRENCY_VOUCHERSHIMS_BACKDEPLOY56_H

#include <cstdint>
#include "nextcode/Runtime/Config.h"

#include "nextcode/Basic/Lazy.h"
#include <dlfcn.h>

// nextcode-corelibs-libdispatch has os/voucher_private.h but it doesn't work for
// us yet, so only look for it on Apple platforms.
#if __APPLE__ && __has_include(<os/voucher_private.h>)
#define NEXTCODE_HAS_VOUCHER_HEADER 1
#include <os/voucher_private.h>
#endif

// A "dead" voucher pointer, indicating that a voucher has been removed from
// a Job, distinct from a NULL voucher that could just mean no voucher was
// present. This allows us to catch problems like adopting a voucher from the
// same Job twice without restoring it.
#define NEXTCODE_DEAD_VOUCHER ((voucher_t)-1)

// The OS has voucher support if it has the header or if it has ObjC interop.
#if NEXTCODE_HAS_VOUCHER_HEADER || NEXTCODE_OBJC_INTEROP
#define NEXTCODE_HAS_VOUCHERS 1
#endif

#if NEXTCODE_HAS_VOUCHERS

#if NEXTCODE_HAS_VOUCHER_HEADER

#else // NEXTCODE_HAS_VOUCHER_HEADER

// If the header isn't available, declare the necessary calls here.

#include <os/object.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
OS_OBJECT_DECL_CLASS(voucher);
#pragma clang diagnostic pop

extern "C" voucher_t _Nullable voucher_copy(void);

// Consumes argument, returns retained value.
extern "C" voucher_t _Nullable voucher_adopt(voucher_t _Nullable voucher);

#endif // __has_include(<os/voucher_private.h>)

static inline void nextcode_voucher_release(voucher_t _Nullable voucher) {
  // This NULL check isn't necessary, but NULL vouchers will be common, so
  // optimize for that.
  if (!voucher)
    return;
  if (voucher == NEXTCODE_DEAD_VOUCHER)
    return;
  os_release(voucher);
}

#else  // __APPLE__

// Declare some do-nothing stubs for OSes without voucher support.
typedef void *voucher_t;
static inline voucher_t _Nullable voucher_copy(void) { return nullptr; }
static inline voucher_t _Nullable voucher_adopt(voucher_t _Nullable voucher) {
  return nullptr;
}
static inline void nextcode_voucher_release(voucher_t _Nullable voucher) {}
#endif // __APPLE__

// Declare our own voucher_needs_adopt for when we don't get it from the SDK.
// This declaration deliberately takes `void *` instead of `voucher_t`. When the
// SDK provides one that takes `voucher_t`, then C++ overload resolution will
// favor that one. When the SDK does not provide a declaration, then the call
// site will invoke this stub instead.
static inline bool voucher_needs_adopt(void * _Nullable voucher) {
  return true;
}

static inline bool nextcode_voucher_needs_adopt(voucher_t _Nullable voucher) {
#if __APPLE__
  // _Z19voucher_needs_adoptP9voucher_s
  const auto voucherNeedsAdopt =
      reinterpret_cast<bool(*)(voucher_t)>(NEXTCODE_LAZY_CONSTANT(
          dlsym(RTLD_DEFAULT, "_Z19voucher_needs_adoptP9voucher_s")));

  if (voucherNeedsAdopt) {
    return voucherNeedsAdopt(voucher);
  }
  return true;
#else
  return voucher_needs_adopt(voucher);
#endif
}

#endif // NEXTCODE_CONCURRENCY_VOUCHERSHIMS_BACKDEPLOY56_H
