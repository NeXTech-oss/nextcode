//===--- ModuleDiagsConsumer.h - Print module differ diagnostics --*- C++ -*-===//
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
//  This file defines the ModuleDifferDiagsConsumer class, which displays
//  diagnostics from the module differ as text to an output.
//
//===----------------------------------------------------------------------===//

#ifndef __NEXTCODE_MODULE_DIFFER_DIAGS_CONSUMER_H__
#define __NEXTCODE_MODULE_DIFFER_DIAGS_CONSUMER_H__

#include "llvm/ADT/MapVector.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/AST/DiagnosticConsumer.h"
#include "nextcode/Frontend/PrintingDiagnosticConsumer.h"

#include "llvm/Support/raw_ostream.h"
#include <set>

namespace nextcode {
namespace ide {
namespace api {

/// Diagnostic consumer that displays diagnostics to standard output.
class ModuleDifferDiagsConsumer: public PrintingDiagnosticConsumer {
  llvm::raw_ostream &OS;
  bool DiagnoseModuleDiff;
  llvm::MapVector<StringRef, std::set<std::string>> AllDiags;
public:
  ModuleDifferDiagsConsumer(bool DiagnoseModuleDiff,
                            llvm::raw_ostream &OS = llvm::errs());
  ~ModuleDifferDiagsConsumer();
  void handleDiagnostic(SourceManager &SM, const DiagnosticInfo &Info) override;
};

class FilteringDiagnosticConsumer: public DiagnosticConsumer {
  bool HasError = false;
  std::vector<std::unique_ptr<DiagnosticConsumer>> subConsumers;
  std::unique_ptr<llvm::StringSet<>> allowedBreakages;
  bool DowngradeToWarning;
  bool shouldProceed(const DiagnosticInfo &Info);
public:
  FilteringDiagnosticConsumer(std::vector<std::unique_ptr<DiagnosticConsumer>> subConsumers,
                              std::unique_ptr<llvm::StringSet<>> allowedBreakages,
                              bool DowngradeToWarning):
    subConsumers(std::move(subConsumers)),
    allowedBreakages(std::move(allowedBreakages)),
    DowngradeToWarning(DowngradeToWarning) {}
  ~FilteringDiagnosticConsumer() = default;

  void flush() override;
  void informDriverOfIncompleteBatchModeCompilation() override;
  bool finishProcessing() override;
  bool hasError() const { return HasError; }

  void handleDiagnostic(SourceManager &SM,
                        const DiagnosticInfo &Info) override;
};
}
}
}

#endif
