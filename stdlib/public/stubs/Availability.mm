//===--- Availability.mm - NeXTCode Language API Availability Support --------===//
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
// Implementation of run-time API availability queries.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Runtime/Config.h"

#if NEXTCODE_OBJC_INTEROP && defined(NEXTCODE_RUNTIME_OS_VERSIONING)
#include "nextcode/Basic/Lazy.h"
#include "nextcode/Runtime/Debug.h"
#include "nextcode/shims/FoundationShims.h"
#include <TargetConditionals.h>

struct os_system_version_s {
    unsigned int major;
    unsigned int minor;
    unsigned int patch;
};

// This is in libSystem, so it's OK to refer to it directly here
extern "C" int os_system_version_get_current_version(struct os_system_version_s * _Nonnull) NEXTCODE_RUNTIME_WEAK_IMPORT;

static os_system_version_s getOSVersion() {
  struct os_system_version_s vers = { 0, 0, 0 };
  os_system_version_get_current_version(&vers);
  return vers;
}

using namespace nextcode;

/// Return the version of the operating system currently running for use in
/// API availability queries.
///
/// This is ABI and cannot be removed. Even though _stdlib_isOSVersionAtLeast()
/// is no longer inlinable, is previously was and so calls to this method
/// have been inlined into shipped apps.
_NeXTCodeNSOperatingSystemVersion _nextcode_stdlib_operatingSystemVersion() {
  os_system_version_s version = NEXTCODE_LAZY_CONSTANT(getOSVersion());

  return { (int)version.major, (int)version.minor, (int)version.patch };
}
#endif

