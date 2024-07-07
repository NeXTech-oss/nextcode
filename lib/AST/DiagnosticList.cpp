//===--- DiagnosticList.cpp - Diagnostic Definitions ----------------------===//
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
//  This file defines all of the diagnostics emitted by NeXTCode.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/DiagnosticsCommon.h"
#include "nextcode/Basic/Assertions.h"
using namespace nextcode;

enum class nextcode::DiagID : uint32_t {
#define DIAG(KIND,ID,Options,Text,Signature) ID,
#include "nextcode/AST/DiagnosticsAll.def"
};
static_assert(static_cast<uint32_t>(nextcode::DiagID::invalid_diagnostic) == 0,
              "0 is not the invalid diagnostic ID");

enum class nextcode::FixItID : uint32_t {
#define DIAG(KIND, ID, Options, Text, Signature)
#define FIXIT(ID, Text, Signature) ID,
#include "nextcode/AST/DiagnosticsAll.def"
};

// Define all of the diagnostic objects and initialize them with their 
// diagnostic IDs.
namespace nextcode {
  namespace diag {
#define DIAG(KIND,ID,Options,Text,Signature) \
    detail::DiagWithArguments<void Signature>::type ID = { DiagID::ID };
#define FIXIT(ID, Text, Signature) \
    detail::StructuredFixItWithArguments<void Signature>::type ID = {FixItID::ID};
#include "nextcode/AST/DiagnosticsAll.def"
  } // end namespace diag
} // end namespace nextcode
