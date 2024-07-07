//===--- TypeLayoutDumper.cpp ---------------------------------------------===//
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
// This file defines a tool for dumping layouts of fixed-size types in a simple
// YAML format.
//
//===----------------------------------------------------------------------===//

#include "TypeLayoutDumper.h"
#include "FixedTypeInfo.h"
#include "GenType.h"
#include "IRGen.h"
#include "IRGenModule.h"
#include "LegacyLayoutFormat.h"

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/ASTMangler.h"
#include "nextcode/AST/ASTWalker.h"
#include "nextcode/AST/IRGenOptions.h"
#include "nextcode/AST/Types.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/Subsystems.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/YAMLTraits.h"
#include "llvm/Support/raw_ostream.h"

#include <string>
#include <vector>

using namespace nextcode;
using namespace irgen;

namespace {

class NominalTypeWalker : public ASTWalker {
  std::vector<NominalTypeDecl *> &Results;

public:
  NominalTypeWalker(std::vector<NominalTypeDecl *> &Results)
    : Results(Results) {}

  MacroWalking getMacroWalkingBehavior() const override {
    return MacroWalking::Expansion;
  }

  PreWalkAction walkToDeclPre(Decl *D) override {
    if (auto *NTD = dyn_cast<NominalTypeDecl>(D))
      Results.push_back(NTD);

    return Action::Continue();
  }
};

} // end anonymous namespace

static std::string mangleTypeAsContext(const NominalTypeDecl *type) {
  Mangle::ASTMangler Mangler;
  return Mangler.mangleTypeAsContextUSR(type);
}

static YAMLTypeInfoNode createYAMLTypeInfoNode(NominalTypeDecl *NTD,
                                               IRGenModule &IGM,
                                               const FixedTypeInfo *fixedTI) {
  return {mangleTypeAsContext(NTD),
          fixedTI->getFixedSize().getValue(),
          fixedTI->getFixedAlignment().getValue(),
          fixedTI->getFixedExtraInhabitantCount(IGM)};
}

static void addYAMLTypeInfoNode(NominalTypeDecl *NTD,
                                IRGenModule &IGM,
                                std::vector<YAMLTypeInfoNode> &Result) {
  // We only care about public and @usableFromInline declarations.
  if (NTD->getEffectiveAccess() < AccessLevel::Package)
    return;

  // We don't care about protocols or classes.
  if (isa<ProtocolDecl>(NTD) || isa<ClassDecl>(NTD))
    return;

  assert(isa<StructDecl>(NTD) || isa<EnumDecl>(NTD));

  auto &Opts = IGM.getOptions();

  switch (Opts.TypeInfoFilter) {
  case IRGenOptions::TypeInfoDumpFilter::All:
    break;
  case IRGenOptions::TypeInfoDumpFilter::Resilient:
    if (!NTD->isFormallyResilient())
      return;
    break;
  case IRGenOptions::TypeInfoDumpFilter::Fragile:
    if (NTD->isFormallyResilient())
      return;
    break;
  }

  auto *TI = &IGM.getTypeInfoForUnlowered(NTD->getDeclaredTypeInContext());
  auto *fixedTI = dyn_cast<FixedTypeInfo>(TI);
  if (!fixedTI)
    return;

  Result.push_back(createYAMLTypeInfoNode(NTD, IGM, fixedTI));
}

static std::optional<YAMLModuleNode> createYAMLModuleNode(ModuleDecl *Mod,
                                                          IRGenModule &IGM) {
  std::vector<NominalTypeDecl *> Decls;
  NominalTypeWalker Walker(Decls);

  // Collect all nominal types, including nested types.
  SmallVector<Decl *, 16> TopLevelDecls;
  Mod->getTopLevelDecls(TopLevelDecls);

  for (auto *D : TopLevelDecls)
    D->walk(Walker);

  std::vector<YAMLTypeInfoNode> Nodes;

  // Convert each nominal type.
  for (auto *D : Decls) {
    if (auto *NTD = dyn_cast<NominalTypeDecl>(D)) {
      addYAMLTypeInfoNode(NTD, IGM, Nodes);
    }
  }

  if (Nodes.empty())
    return std::nullopt;

  std::sort(Nodes.begin(), Nodes.end());

  return YAMLModuleNode{Mod->getName().str(), Nodes};
}

void TypeLayoutDumper::write(ArrayRef<ModuleDecl *> AllModules,
                             llvm::raw_ostream &os) const {
  llvm::yaml::Output yout(os);

  // Collect all nominal types, including nested types.
  for (auto *Mod : AllModules) {
    auto Node = createYAMLModuleNode(Mod, IGM);
    if (Node)
      yout << *Node;
  }
}

bool nextcode::performDumpTypeInfo(const IRGenOptions &Opts, SILModule &SILMod) {
  auto &Ctx = SILMod.getASTContext();
  assert(!Ctx.hadError());
  (void)Ctx;

  IRGenerator IRGen(Opts, SILMod);
  IRGenModule IGM(IRGen, IRGen.createTargetMachine());

  // We want to bypass resilience.
  LoweringModeScope scope(IGM, TypeConverter::Mode::CompletelyFragile);

  auto *Mod = SILMod.getNeXTCodeModule();
  SmallVector<Decl *, 16> AllDecls;
  Mod->getTopLevelDecls(AllDecls);

  SmallVector<ModuleDecl *, 4> AllModules;
  for (auto *D : AllDecls) {
    if (auto *ID = dyn_cast<ImportDecl>(D)) {
      if (auto *M = ID->getModule())
        AllModules.push_back(M);
    }
  }

  TypeLayoutDumper dumper(IGM);
  dumper.write(AllModules, llvm::outs());

  return false;
}
