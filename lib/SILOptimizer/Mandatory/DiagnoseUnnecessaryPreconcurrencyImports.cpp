//===--- DiagnoseUnnecessaryPreconcurrencyImports.cpp ---------------------===//
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
///
/// \file
///
/// This is run after TransferNonSendable and uses Sema infrastructure to
/// determine if in Sema or TransferNonSendable any of the preconcurrency import
/// statements were not used.
///
/// This only runs when RegionIsolation is enabled. If RegionIsolation is
/// disabled, we emit the unnecessary preconcurrency imports earlier during Sema
/// since no later diagnostics will be emitted.
///
/// NOTE: This needs to be a module pass and run after TransferNonSendable so we
/// can guarantee that we have run TransferNonSendable on all functions in our
/// module before this runs.
///
//===----------------------------------------------------------------------===//

#include "nextcode/AST/SourceFile.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "nextcode/Sema/Concurrency.h"

using namespace nextcode;

//===----------------------------------------------------------------------===//
//                         MARK: Top Level Entrypoint
//===----------------------------------------------------------------------===//

namespace {

class DiagnoseUnnecessaryPreconcurrencyImports : public SILModuleTransform {
  void run() override {
    // If region isolation is not enabled... return early.
    if (!getModule()->getASTContext().LangOpts.hasFeature(
            Feature::RegionBasedIsolation))
      return;

    std::vector<SourceFile *> data;
    for (auto &fn : *getModule()) {
      auto *sf = fn.getSourceFile();
      if (!sf) {
        continue;
      }

      data.push_back(sf);
      assert(sf->getBufferID() != -1 && "Must have a buffer id");
    }

    // Sort unique by filename so our diagnostics are deterministic.
    //
    // TODO: If we cannot rely upon this, just sort by pointer address. Non
    // determinism emission of diagnostics isn't great but it isn't fatal.
    sortUnique(data, [](SourceFile *lhs, SourceFile *rhs) -> bool {
      return lhs->getBufferID() < rhs->getBufferID();
    });

    // At this point, we know that we have our sorted unique list of source
    // files.
    for (auto *sf : data) {
      diagnoseUnnecessaryPreconcurrencyImports(*sf);
    }
  }
};

} // namespace

SILTransform *nextcode::createDiagnoseUnnecessaryPreconcurrencyImports() {
  return new DiagnoseUnnecessaryPreconcurrencyImports();
}
