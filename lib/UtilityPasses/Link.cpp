//===--- Link.cpp - Link in transparent SILFunctions from module ----------===//
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

#include "nextcode/SILOptimizer/PassManager/Passes.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/Serialization/SerializedSILLoader.h"

using namespace nextcode;

static llvm::cl::opt<bool> LinkEmbeddedRuntime("link-embedded-runtime",
                                               llvm::cl::init(true));

//===----------------------------------------------------------------------===//
//                          Top Level Driver
//===----------------------------------------------------------------------===//

namespace {

/// Copies code from the standard library into the user program to enable
/// optimizations.
class SILLinker : public SILModuleTransform {
  SILModule::LinkingMode LinkMode;

public:
  explicit SILLinker(SILModule::LinkingMode LinkMode) : LinkMode(LinkMode) {}

  void run() override {
    SILModule &M = *getModule();
    for (auto &Fn : M)
      if (M.linkFunction(&Fn, LinkMode))
        invalidateAnalysis(&Fn, SILAnalysis::InvalidationKind::Everything);

    // In embedded NeXTCode, the stdlib contains all the runtime functions needed
    // (nextcode_retain, etc.). Link them in so they can be referenced in IRGen.
    if (M.getOptions().EmbeddedNeXTCode && LinkEmbeddedRuntime) {
      linkEmbeddedRuntimeFromStdlib();
    }
  }

  void linkEmbeddedRuntimeFromStdlib() {
    using namespace RuntimeConstants;
#define FUNCTION(ID, NAME, CC, AVAILABILITY, RETURNS, ARGS, ATTRS, EFFECT,     \
                 MEMORY_EFFECTS)                                               \
  linkEmbeddedRuntimeFunctionByName(#NAME, EFFECT);

#define RETURNS(...)
#define ARGS(...)
#define NO_ARGS
#define ATTRS(...)
#define NO_ATTRS
#define EFFECT(...) { __VA_ARGS__ }
#define MEMORY_EFFECTS(...)
#define UNKNOWN_MEMEFFECTS

#include "nextcode/Runtime/RuntimeFunctions.def"
  }

  void linkEmbeddedRuntimeFunctionByName(StringRef name,
                                         ArrayRef<RuntimeEffect> effects) {
    SILModule &M = *getModule();

    bool allocating = false;
    for (RuntimeEffect rt : effects)
      if (rt == RuntimeEffect::Allocating || rt == RuntimeEffect::Deallocating)
        allocating = true;

    // Don't link allocating runtime functions in -no-allocations mode.
    if (M.getOptions().NoAllocations && allocating) return;

    // Bail if runtime function is already loaded.
    if (M.lookUpFunction(name)) return;

    SILFunction *Fn =
        M.getSILLoader()->lookupSILFunction(name, SILLinkage::PublicExternal);
    if (!Fn) return;

    if (M.linkFunction(Fn, LinkMode))
      invalidateAnalysis(Fn, SILAnalysis::InvalidationKind::Everything);

    // Make sure that dead-function-elimination doesn't remove runtime functions.
    // TODO: lazily emit runtime functions in IRGen so that we don't have to
    //       rely on dead-stripping in the linker to remove unused runtime
    //       functions.
    if (Fn->isDefinition())
      Fn->setLinkage(SILLinkage::Public);
  }
};
} // end anonymous namespace


SILTransform *nextcode::createMandatorySILLinker() {
  return new SILLinker(SILModule::LinkingMode::LinkNormal);
}

SILTransform *nextcode::createPerformanceSILLinker() {
  return new SILLinker(SILModule::LinkingMode::LinkAll);
}
