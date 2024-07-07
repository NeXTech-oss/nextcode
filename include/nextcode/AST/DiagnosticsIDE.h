//===--- DiagnosticsIDE.h - Diagnostic Definitions --------------*- C++ -*-===//
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
/// \file
/// This file defines diagnostics used only in IDE.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_DIAGNOSTICSIDE_H
#define NEXTCODE_DIAGNOSTICSIDE_H

#include "nextcode/AST/DiagnosticsCommon.h"

namespace nextcode {
  namespace diag {
  // Declare common diagnostics objects with their appropriate types.
#define DIAG(KIND,ID,Options,Text,Signature) \
    extern detail::DiagWithArguments<void Signature>::type ID;
#include "DiagnosticsIDE.def"
  }
}

#endif
