//===--- IRGenRequests.cpp - Requests for LLVM IR Generation --------------===//
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

#include "nextcode/AST/IRGenRequests.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/FileUnit.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/AST/TBDGenRequests.h"
#include "nextcode/Subsystems.h"
#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"

using namespace nextcode;

namespace nextcode {
// Implement the IRGen type zone (zone 20).
#define NEXTCODE_TYPEID_ZONE IRGen
#define NEXTCODE_TYPEID_HEADER "nextcode/AST/IRGenTypeIDZone.def"
#include "nextcode/Basic/ImplementTypeIDZone.h"
#undef NEXTCODE_TYPEID_ZONE
#undef NEXTCODE_TYPEID_HEADER
} // end namespace nextcode

llvm::orc::ThreadSafeModule GeneratedModule::intoThreadSafeContext() && {
  return {std::move(Module), std::move(Context)};
}

void nextcode::simple_display(llvm::raw_ostream &out,
                           const IRGenDescriptor &desc) {
  auto *MD = desc.Ctx.dyn_cast<ModuleDecl *>();
  if (MD) {
    out << "IR Generation for module " << MD->getName();
  } else {
    auto *file = desc.Ctx.get<FileUnit *>();
    out << "IR Generation for file ";
    simple_display(out, file);
  }
}

SourceLoc nextcode::extractNearestSourceLoc(const IRGenDescriptor &desc) {
  return SourceLoc();
}

TinyPtrVector<FileUnit *> IRGenDescriptor::getFilesToEmit() const {
  // If we've been asked to emit a specific set of symbols, we don't emit any
  // whole files.
  if (SymbolsToEmit)
    return {};

  // For a whole module, we emit IR for all files.
  if (auto *mod = Ctx.dyn_cast<ModuleDecl *>())
    return TinyPtrVector<FileUnit *>(mod->getFiles());

  // For a primary file, we emit IR for both it and potentially its
  // SynthesizedFileUnit.
  auto *primary = Ctx.get<FileUnit *>();
  TinyPtrVector<FileUnit *> files;
  files.push_back(primary);

  return files;
}

ModuleDecl *IRGenDescriptor::getParentModule() const {
  if (auto *file = Ctx.dyn_cast<FileUnit *>())
    return file->getParentModule();
  return Ctx.get<ModuleDecl *>();
}

TBDGenDescriptor IRGenDescriptor::getTBDGenDescriptor() const {
  if (auto *file = Ctx.dyn_cast<FileUnit *>()) {
    return TBDGenDescriptor::forFile(file, TBDOpts);
  } else {
    auto *M = Ctx.get<ModuleDecl *>();
    return TBDGenDescriptor::forModule(M, TBDOpts);
  }
}

std::vector<std::string> IRGenDescriptor::getLinkerDirectives() const {
  auto desc = getTBDGenDescriptor();
  desc.getOptions().LinkerDirectivesOnly = true;
  return getPublicSymbols(std::move(desc));
}

evaluator::DependencySource IRGenRequest::readDependencySource(
    const evaluator::DependencyRecorder &e) const {
  auto &desc = std::get<0>(getStorage());

  // We don't track dependencies in whole-module mode.
  if (auto *mod = desc.Ctx.dyn_cast<ModuleDecl *>()) {
    return nullptr;
  }

  auto *primary = desc.Ctx.get<FileUnit *>();
  return dyn_cast<SourceFile>(primary);
}

// Define request evaluation functions for each of the IRGen requests.
static AbstractRequestFunction *irGenRequestFunctions[] = {
#define NEXTCODE_REQUEST(Zone, Name, Sig, Caching, LocOptions)                    \
  reinterpret_cast<AbstractRequestFunction *>(&Name::evaluateRequest),
#include "nextcode/AST/IRGenTypeIDZone.def"
#undef NEXTCODE_REQUEST
};

void nextcode::registerIRGenRequestFunctions(Evaluator &evaluator) {
  evaluator.registerRequestFunctions(Zone::IRGen, irGenRequestFunctions);
}
