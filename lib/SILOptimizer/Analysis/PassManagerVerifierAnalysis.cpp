//===--- PassManagerVerifierAnalysis.cpp ----------------------------------===//
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

#define DEBUG_TYPE "sil-passmanager-verifier-analysis"
#include "nextcode/SILOptimizer/Analysis/PassManagerVerifierAnalysis.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILModule.h"
#include "llvm/Support/CommandLine.h"

static llvm::cl::opt<bool>
    EnableVerifier("enable-sil-passmanager-verifier-analysis",
                   llvm::cl::desc("Enable verification of the passmanagers "
                                  "function notification infrastructure"),
                   llvm::cl::init(true));

using namespace nextcode;

PassManagerVerifierAnalysis::PassManagerVerifierAnalysis(SILModule *mod)
    : SILAnalysis(SILAnalysisKind::PassManagerVerifier), mod(*mod) {
#ifndef NDEBUG
  if (!EnableVerifier)
    return;
  for (auto &fn : *mod) {
    LLVM_DEBUG(llvm::dbgs() << "PMVerifierAnalysis. Add: " << fn.getName()
                            << '\n');
    liveFunctionNames.insert(fn.getName());
  }
#endif
}

/// Validate that the analysis is able to look up all functions and that those
/// functions are live.
void PassManagerVerifierAnalysis::invalidate() {}

/// Validate that the analysis is able to look up the given function.
void PassManagerVerifierAnalysis::invalidate(SILFunction *f,
                                             InvalidationKind k) {}

/// If a function has not yet been seen start tracking it.
void PassManagerVerifierAnalysis::notifyAddedOrModifiedFunction(
    SILFunction *f) {
#ifndef NDEBUG
  if (!EnableVerifier)
    return;
  LLVM_DEBUG(llvm::dbgs() << "PMVerifierAnalysis. Add|Mod: " << f->getName()
                          << '\n');
  liveFunctionNames.insert(f->getName());
#endif
}

/// Stop tracking a function.
void PassManagerVerifierAnalysis::notifyWillDeleteFunction(SILFunction *f) {
#ifndef NDEBUG
  if (!EnableVerifier)
    return;
  LLVM_DEBUG(llvm::dbgs() << "PMVerifierAnalysis. Delete: " << f->getName()
                          << '\n');
  if (liveFunctionNames.erase(f->getName()))
    return;

  llvm::errs()
      << "Error! Tried to delete function that analysis was not aware of: "
      << f->getName() << '\n';
  llvm_unreachable("triggering standard assertion failure routine");
#endif
}

/// Make sure that when we invalidate a function table, make sure we can find
/// all functions for all witness tables.
void PassManagerVerifierAnalysis::invalidateFunctionTables() {}

/// Run the entire verification.
void PassManagerVerifierAnalysis::verifyFull() const {
#ifndef NDEBUG
  if (!EnableVerifier)
    return;

  // We check that liveFunctionNames is in sync with the module's function list
  // by going through the module's function list and attempting to remove all
  // functions in the module. If we fail to remove fn, then we know that a
  // function was added to the module without an appropriate message being sent
  // by the pass manager.
  bool foundError = false;

  unsigned count = 0;
  for (auto &fn : mod) {
    if (liveFunctionNames.count(fn.getName())) {
      ++count;
      continue;
    }
    llvm::errs() << "Found function in module that was not added to verifier: "
                 << fn.getName() << '\n';
    foundError = true;
  }

  // Ok, so now we know that function(mod) is a subset of
  // liveFunctionNames. Relying on the uniqueness provided by the module's
  // function list, we know that liveFunction should be exactly count in
  // size. Otherwise, we must have an error. If and only if we detect this
  // error, do the expensive work of finding the missing deletes. This is an
  // important performance optimization to avoid a large copy on the hot path.
  if (liveFunctionNames.size() != count) {
    auto liveFunctionNamesCopy = llvm::StringSet<>(liveFunctionNames);
    for (auto &fn : mod) {
      liveFunctionNamesCopy.erase(fn.getName());
    }
    for (auto &iter : liveFunctionNamesCopy) {
      llvm::errs() << "Missing delete message for function: " << iter.first()
                   << '\n';
      foundError = true;
    }
  }

  // We assert here so we emit /all/ errors before asserting.
  assert(!foundError && "triggering standard assertion failure routine");
#endif
}

//===----------------------------------------------------------------------===//
//                              Main Entry Point
//===----------------------------------------------------------------------===//

SILAnalysis *nextcode::createPassManagerVerifierAnalysis(SILModule *m) {
  return new PassManagerVerifierAnalysis(m);
}