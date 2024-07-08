//===--- AccessStorageDumper.cpp - Dump accessed storage ----------------===//
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

#define DEBUG_TYPE "sil-accessed-storage-dumper"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/MemAccessUtils.h"
#include "nextcode/SIL/PrettyStackTrace.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILValue.h"
#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"

using namespace nextcode;

static llvm::cl::opt<bool> EnableDumpUses(
    "enable-access-storage-dump-uses", llvm::cl::init(false),
    llvm::cl::desc("With --sil-access-storage-dumper, dump all uses"));

namespace {

/// Dumps sorage information for each access.
class AccessStorageDumper : public SILModuleTransform {
  llvm::SmallVector<Operand *, 32> uses;

  void dumpAccessStorage(Operand *operand) {
    SILFunction *function = operand->getParentFunction();
    // Print storage itself first, for comparison against AccessPath. They can
    // differ in rare cases of unidentified storage with phis.
    AccessStorage::compute(operand->get()).print(llvm::outs());
    // Now print the access path and base.
    auto pathAndBase = AccessPathWithBase::compute(operand->get());
    pathAndBase.print(llvm::outs());
    // If enable-accessed-storage-dump-uses is set, dump all types of uses.
    auto accessPath = pathAndBase.accessPath;
    if (!accessPath.isValid() || !EnableDumpUses)
      return;

    uses.clear();
    accessPath.collectUses(uses, AccessUseType::Exact, function);
    llvm::outs() << "Exact Uses {\n";
    for (auto *useOperand : uses) {
      llvm::outs() << *useOperand->getUser() << "  ";
      auto usePath = AccessPath::compute(useOperand->get());
      usePath.printPath(llvm::outs());
      assert(accessPath == usePath
             && "access path does not match use access path");
    }
    llvm::outs() << "}\n";
    uses.clear();
    accessPath.collectUses(uses, AccessUseType::Inner, function);
    llvm::outs() << "Inner Uses {\n";
    for (auto *useOperand : uses) {
      llvm::outs() << *useOperand->getUser() << "  ";
      auto usePath = AccessPath::compute(useOperand->get());
      usePath.printPath(llvm::outs());
      assert(accessPath.contains(usePath)
             && "access path does not contain use access path");
    }
    llvm::outs() << "}\n";
    uses.clear();
    accessPath.collectUses(uses, AccessUseType::Overlapping, function);
    llvm::outs() << "Overlapping Uses {\n";
    for (auto *useOperand : uses) {
      llvm::outs() << *useOperand->getUser() << "  ";
      auto usePath = AccessPath::compute(useOperand->get());
      usePath.printPath(llvm::outs());
      assert(accessPath.mayOverlap(usePath)
             && "access path does not overlap with use access path");
    }
    llvm::outs() << "}\n";
  }

  void run() override {
    for (auto &fn : *getModule()) {
      llvm::outs() << "@" << fn.getName() << "\n";
      if (fn.empty()) {
        llvm::outs() << "<unknown>\n";
        continue;
      }
      PrettyStackTraceSILFunction functionDumper("...", &fn);
      for (auto &bb : fn) {
        for (auto &inst : bb) {
          if (inst.mayReadOrWriteMemory()) {
            llvm::outs() << "###For MemOp: " << inst;
            visitAccessedAddress(&inst, [this](Operand *operand) {
              dumpAccessStorage(operand);
            });
          }
        }
      }
    }
  }
};

} // end anonymous namespace

SILTransform *nextcode::createAccessStorageDumper() {
  return new AccessStorageDumper();
}
