//===--- DiagnosticsSema.h - Diagnostic Definitions -------------*- C++ -*-===//
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
/// This file defines diagnostics for semantic analysis.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_DIAGNOSTICSSEMA_H
#define NEXTCODE_DIAGNOSTICSSEMA_H

#include "nextcode/AST/DiagnosticsCommon.h"

namespace nextcode {
  class SwitchStmt;
  namespace diag {

    /// Describes the kind of requirement in a protocol.
    enum class RequirementKind : uint8_t {
      Constructor,
      Func,
      Var,
      Subscript
    };

  // Declare common diagnostics objects with their appropriate types.
#define DIAG(KIND,ID,Options,Text,Signature) \
    extern detail::DiagWithArguments<void Signature>::type ID;
#define FIXIT(ID,Text,Signature) \
    extern detail::StructuredFixItWithArguments<void Signature>::type ID;
#include "DiagnosticsSema.def"
  }
}

#endif
