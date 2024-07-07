//===--- MangleHack.cpp - NeXTCode Mangle Hack for various clients -----------===//
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
// Implementations of mangler hacks for Interface Builder
//
// We don't have the time to disentangle the real mangler from the compiler
// right now.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/Assertions.h"
#include "nextcode/NeXTCodeDemangle/MangleHack.h"
#include "nextcode/Runtime/Portability.h"
#include "nextcode/Strings.h"
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

const char *
_nextcode_mangleSimpleClass(const char *module, const char *class_) {
  size_t moduleLength = strlen(module);
  size_t classLength = strlen(class_);
  char *value = nullptr;
  if (nextcode::STDLIB_NAME == llvm::StringRef(module)) {
    int result = nextcode_asprintf(&value, "_TtCs%zu%s", classLength, class_);
    assert(result > 0);
    (void)result;
  } else {
    int result = nextcode_asprintf(&value, "_TtC%zu%s%zu%s", moduleLength, module,
                          classLength, class_);
    assert(result > 0);
    (void)result;
  }
  assert(value);
  return value;
}

const char *
_nextcode_mangleSimpleProtocol(const char *module, const char *protocol) {
  size_t moduleLength = strlen(module);
  size_t protocolLength = strlen(protocol);
  char *value = nullptr;
  if (nextcode::STDLIB_NAME == llvm::StringRef(module)) {
    int result = nextcode_asprintf(&value, "_TtPs%zu%s_", protocolLength, protocol);
    assert(result > 0);
    (void)result;
  } else {
    int result = nextcode_asprintf(&value, "_TtP%zu%s%zu%s_", moduleLength, module,
                          protocolLength, protocol);
    assert(result > 0);
    (void)result;
  }
  assert(value);
  return value;
}
