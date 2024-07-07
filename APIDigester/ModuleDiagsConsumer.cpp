//===--- ModuleDiagsConsumer.cpp - Print module differ diagnostics --*- C++ -*-===//
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
//  This file implements the ModuleDifferDiagsConsumer class, which displays
//  diagnostics from the module differ as text to an output.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/AST/DiagnosticsModuleDiffer.h"
#include "nextcode/APIDigester/ModuleDiagsConsumer.h"
#include "nextcode/Basic/Assertions.h"

using namespace nextcode;

namespace {
// Reproduce the DiagIDs, as we want both the size and access to the raw ids
// themselves.
enum LocalDiagID : uint32_t {
#define DIAG(KIND, ID, Options, Text, Signature) ID,
#include "nextcode/AST/DiagnosticsAll.def"
  NumDiags
};

static StringRef getCategoryName(uint32_t ID) {
  switch(ID) {
  case LocalDiagID::removed_decl:
    return "/* Removed Decls */";
  case LocalDiagID::moved_decl:
  case LocalDiagID::decl_kind_changed:
    return "/* Moved Decls */";
  case LocalDiagID::renamed_decl:
  case LocalDiagID::objc_name_change:
    return "/* Renamed Decls */";
  case LocalDiagID::decl_attr_change:
  case LocalDiagID::decl_new_attr:
  case LocalDiagID::func_self_access_change:
  case LocalDiagID::new_decl_without_intro:
    return "/* Decl Attribute changes */";
  case LocalDiagID::default_arg_removed:
  case LocalDiagID::decl_type_change:
  case LocalDiagID::func_type_escaping_changed:
  case LocalDiagID::param_ownership_change:
  case LocalDiagID::type_witness_change:
    return "/* Type Changes */";
  case LocalDiagID::raw_type_change:
    return "/* RawRepresentable Changes */";
  case LocalDiagID::generic_sig_change:
  case LocalDiagID::demangled_name_changed:
    return "/* Generic Signature Changes */";
  case LocalDiagID::enum_case_added:
  case LocalDiagID::decl_added:
  case LocalDiagID::decl_reorder:
  case LocalDiagID::var_has_fixed_order_change:
  case LocalDiagID::func_has_fixed_order_change:
    return "/* Fixed-layout Type Changes */";
  case LocalDiagID::conformance_added:
  case LocalDiagID::conformance_removed:
  case LocalDiagID::optional_req_changed:
  case LocalDiagID::existing_conformance_added:
    return "/* Protocol Conformance Change */";
  case LocalDiagID::default_associated_type_removed:
  case LocalDiagID::protocol_req_added:
  case LocalDiagID::decl_new_witness_table_entry:
    return "/* Protocol Requirement Change */";
  case LocalDiagID::super_class_removed:
  case LocalDiagID::super_class_changed:
  case LocalDiagID::no_longer_open:
  case LocalDiagID::desig_init_added:
  case LocalDiagID::added_invisible_designated_init:
  case LocalDiagID::not_inheriting_convenience_inits:
    return "/* Class Inheritance Change */";
  default:
    return "/* Others */";
  }
}
}

nextcode::ide::api::
ModuleDifferDiagsConsumer::ModuleDifferDiagsConsumer(bool DiagnoseModuleDiff,
                                                     llvm::raw_ostream &OS):
    PrintingDiagnosticConsumer(OS), OS(OS),
    DiagnoseModuleDiff(DiagnoseModuleDiff) {
#define DIAG(KIND, ID, Options, Text, Signature)                              \
  auto ID = getCategoryName(LocalDiagID::ID);                                 \
  assert(!ID.empty());                                                        \
  AllDiags[ID] = std::set<std::string>();
#include "nextcode/AST/DiagnosticsModuleDiffer.def"
}

void nextcode::ide::api::ModuleDifferDiagsConsumer::handleDiagnostic(
    SourceManager &SM, const DiagnosticInfo &Info) {
  auto Category = getCategoryName((uint32_t)Info.ID);
  if (Category.empty()) {
    PrintingDiagnosticConsumer::handleDiagnostic(SM, Info);
    return;
  }
  if (!DiagnoseModuleDiff)
    return;
  llvm::SmallString<256> Text;
  {
    llvm::raw_svector_ostream Out(Text);
    DiagnosticEngine::formatDiagnosticText(Out, Info.FormatString,
                                           Info.FormatArgs);
  }
  AllDiags[Category].insert(Text.str().str());
}

nextcode::ide::api::ModuleDifferDiagsConsumer::~ModuleDifferDiagsConsumer() {
  for (auto &Pair: AllDiags) {
    OS << "\n";
    OS << Pair.first << "\n";
    for (auto &Item: Pair.second) {
      OS << Item << "\n";
    }
  }
}

void nextcode::ide::api::FilteringDiagnosticConsumer::flush() {
  for (auto &consumer: subConsumers) {
    consumer->flush();
  }
}

void nextcode::ide::api::
FilteringDiagnosticConsumer::informDriverOfIncompleteBatchModeCompilation() {
  for (auto &consumer: subConsumers) {
    consumer->informDriverOfIncompleteBatchModeCompilation();
  }
}

bool nextcode::ide::api::FilteringDiagnosticConsumer::finishProcessing() {
  for (auto &consumer: subConsumers) {
    consumer->finishProcessing();
  }
  return false;
}

bool nextcode::ide::api::FilteringDiagnosticConsumer::shouldProceed(const DiagnosticInfo &Info) {
  if (allowedBreakages->empty()) {
    return true;
  }
  llvm::SmallString<256> Text;
  {
    llvm::raw_svector_ostream Out(Text);
    DiagnosticEngine::formatDiagnosticText(Out, Info.FormatString,
                                           Info.FormatArgs);
  }
  return allowedBreakages->count(Text.str()) == 0;
}

void nextcode::ide::api::
FilteringDiagnosticConsumer::handleDiagnostic(SourceManager &SM,
                                              const DiagnosticInfo &RawInfo) {
  if (shouldProceed(RawInfo)) {
    DiagnosticInfo Info = RawInfo;
    if (DowngradeToWarning && Info.Kind == DiagnosticKind::Error) {
      Info.Kind = DiagnosticKind::Warning;
    }
    if (Info.Kind == DiagnosticKind::Error) {
      HasError = true;
    }
    for (auto &consumer: subConsumers) {
      consumer->handleDiagnostic(SM, Info);
    }
  }
}
