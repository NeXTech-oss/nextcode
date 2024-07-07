//===--- Action.cpp - Abstract compilation steps --------------------------===//
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

#include "nextcode/Driver/Action.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/ErrorHandling.h"

using namespace nextcode::driver;
using namespace llvm::opt;

const char *Action::getClassName(Kind AC) {
  switch (AC) {
  case Kind::Input:  return "input";
  case Kind::CompileJob:  return "compile";
  case Kind::InterpretJob:  return "interpret";
  case Kind::BackendJob:  return "backend";
  case Kind::MergeModuleJob:  return "merge-module";
  case Kind::ModuleWrapJob:  return "modulewrap";
  case Kind::AutolinkExtractJob:  return "nextcode-autolink-extract";
  case Kind::REPLJob:  return "repl";
  case Kind::DynamicLinkJob:  return "link";
  case Kind::StaticLinkJob:  return "static-link";
  case Kind::GenerateDSYMJob:  return "generate-dSYM";
  case Kind::VerifyDebugInfoJob:  return "verify-debug-info";
  case Kind::GeneratePCHJob:  return "generate-pch";
  case Kind::VerifyModuleInterfaceJob: return "verify-module-interface";
  }

  llvm_unreachable("invalid class");
}

void InputAction::anchor() {}

void JobAction::anchor() {}

void CompileJobAction::anchor() {}

void InterpretJobAction::anchor() {}

void BackendJobAction::anchor() {}

void MergeModuleJobAction::anchor() {}

void ModuleWrapJobAction::anchor() {}

void AutolinkExtractJobAction::anchor() {}

void REPLJobAction::anchor() {}

void DynamicLinkJobAction::anchor() {}

void StaticLinkJobAction::anchor() {}

void GenerateDSYMJobAction::anchor() {}

void VerifyDebugInfoJobAction::anchor() {}

void GeneratePCHJobAction::anchor() {}

void VerifyModuleInterfaceJobAction::anchor() {}
