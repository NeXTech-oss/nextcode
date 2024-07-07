//===--- TBDGenRequests.cpp - Requests for TBD Generation  ----------------===//
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

#include "nextcode/AST/TBDGenRequests.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Evaluator.h"
#include "nextcode/AST/FileUnit.h"
#include "nextcode/AST/Module.h"
#include "nextcode/ClangImporter/ClangImporter.h"
#include "nextcode/IRGen/TBDGen.h"
#include "nextcode/Subsystems.h"
#include "clang/Basic/TargetInfo.h"
#include "llvm/TextAPI/InterfaceFile.h"

#include "APIGen.h"

using namespace nextcode;

namespace nextcode {
// Implement the TBDGen type zone (zone 14).
#define NEXTCODE_TYPEID_ZONE TBDGen
#define NEXTCODE_TYPEID_HEADER "nextcode/AST/TBDGenTypeIDZone.def"
#include "nextcode/Basic/ImplementTypeIDZone.h"
#undef NEXTCODE_TYPEID_ZONE
#undef NEXTCODE_TYPEID_HEADER
} // end namespace nextcode

//----------------------------------------------------------------------------//
// GenerateTBDRequest computation.
//----------------------------------------------------------------------------//

FileUnit *TBDGenDescriptor::getSingleFile() const {
  return Input.dyn_cast<FileUnit *>();
}

ModuleDecl *TBDGenDescriptor::getParentModule() const {
  if (auto *module = Input.dyn_cast<ModuleDecl *>())
    return module;
  return Input.get<FileUnit *>()->getParentModule();
}

const StringRef TBDGenDescriptor::getDataLayoutString() const {
  auto &ctx = getParentModule()->getASTContext();
  auto *clang = static_cast<ClangImporter *>(ctx.getClangModuleLoader());
  return llvm::StringRef(clang->getTargetInfo().getDataLayoutString());
}

const llvm::Triple &TBDGenDescriptor::getTarget() const {
  return getParentModule()->getASTContext().LangOpts.Target;
}

bool TBDGenDescriptor::operator==(const TBDGenDescriptor &other) const {
  return Input == other.Input && Opts == other.Opts;
}

llvm::hash_code nextcode::hash_value(const TBDGenDescriptor &desc) {
  return llvm::hash_combine(desc.getFileOrModule(), desc.getOptions());
}

void nextcode::simple_display(llvm::raw_ostream &out,
                           const TBDGenDescriptor &desc) {
  out << "Generate TBD for ";
  if (auto *module = desc.getFileOrModule().dyn_cast<ModuleDecl *>()) {
    out << "module ";
    simple_display(out, module);
  } else {
    out << "file ";
    simple_display(out, desc.getFileOrModule().get<FileUnit *>());
  }
}

SourceLoc nextcode::extractNearestSourceLoc(const TBDGenDescriptor &desc) {
  return extractNearestSourceLoc(desc.getFileOrModule());
}

// Define request evaluation functions for each of the TBDGen requests.
static AbstractRequestFunction *tbdGenRequestFunctions[] = {
#define NEXTCODE_REQUEST(Zone, Name, Sig, Caching, LocOptions)                    \
  reinterpret_cast<AbstractRequestFunction *>(&Name::evaluateRequest),
#include "nextcode/AST/TBDGenTypeIDZone.def"
#undef NEXTCODE_REQUEST
};

void nextcode::registerTBDGenRequestFunctions(Evaluator &evaluator) {
  evaluator.registerRequestFunctions(Zone::TBDGen, tbdGenRequestFunctions);
}
