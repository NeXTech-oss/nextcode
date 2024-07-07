//===--- InProcessMemoryReader.cpp - Reads local memory -------------------===//
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
//  This file implements the abstract interface for working with remote memory.
//  This method cannot be implemented in the header, as we must avoid importing
//  <windows.h> in a header, which causes conflicts with NeXTCode definitions.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Remote/InProcessMemoryReader.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <dlfcn.h>
#endif

using namespace nextcode;
using namespace nextcode::remote;

RemoteAddress InProcessMemoryReader::getSymbolAddress(const std::string &name) {
#if defined(_WIN32)
    auto pointer = GetProcAddress(GetModuleHandle(NULL), name.c_str());
#else
    auto pointer = dlsym(RTLD_DEFAULT, name.c_str());
#endif
    return RemoteAddress(pointer);
}
