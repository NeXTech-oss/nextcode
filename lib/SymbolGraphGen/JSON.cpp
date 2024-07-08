//===--- JSON.cpp - Symbol Graph JSON Helpers -----------------------------===//
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
// Adds Symbol Graph JSON serialization to other types.
//===----------------------------------------------------------------------===//

#include "JSON.h"
#include "Symbol.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/FileUnit.h"
#include "nextcode/AST/GenericParamList.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/Type.h"
#include "nextcode/AST/USRGeneration.h"
#include "nextcode/ClangImporter/ClangModule.h"
#include "nextcode/Serialization/SerializedModuleLoader.h"

void nextcode::symbolgraphgen::serialize(const llvm::VersionTuple &VT,
                                      llvm::json::OStream &OS) {
  OS.object([&](){
    OS.attribute("major", VT.getMajor());
    if (VT.getMinor()) {
      OS.attribute("minor", *VT.getMinor());
    }
    if (VT.getSubminor()) {
      OS.attribute("patch", *VT.getSubminor());
    }
    // Despite the name,
    // this is not Semantic Versioning "build metadata"
    if (VT.getBuild()) {
      OS.attribute("prerelease", *VT.getBuild());
    }
  });
}

void nextcode::symbolgraphgen::serialize(const llvm::Triple &T,
                                      llvm::json::OStream &OS) {
  OS.object([&](){
    OS.attribute("architecture", T.getArchName());
    if (!T.getEnvironmentName().empty()) {
      OS.attribute("environment", T.getEnvironmentName());
    }
    OS.attribute("vendor", T.getVendorName());
    OS.attributeObject("operatingSystem", [&](){
      OS.attribute("name", T.getOSTypeName(T.getOS()));

      llvm::VersionTuple OSVersion = T.getOSVersion();
      if (!OSVersion.empty()) {
        OS.attributeBegin("minimumVersion");
        serialize(OSVersion, OS);
        OS.attributeEnd();
      }
    });
  });
}

void nextcode::symbolgraphgen::serialize(const ExtensionDecl *Extension,
                                      llvm::json::OStream &OS) {
  OS.attributeObject("nextcodeExtension", [&](){
    if (const auto *ExtendedNominal = Extension->getExtendedNominal()) {
      if (const auto *ExtendedModule = ExtendedNominal->getModuleContext()) {
        OS.attribute("extendedModule", ExtendedModule->getNameStr());
      }

      OS.attribute("typeKind", Symbol::getKind(ExtendedNominal).first);
    }

    SmallVector<Requirement, 4> FilteredRequirements;

    filterGenericRequirements(Extension,
                              FilteredRequirements);

    if (!FilteredRequirements.empty()) {
      OS.attributeArray("constraints", [&](){
        for (const auto &Requirement : FilteredRequirements) {
          serialize(Requirement, OS);
        }
      }); // end constraints:
    }
  }); // end nextcodeExtension:
}

void nextcode::symbolgraphgen::serialize(const Requirement &Req,
                                      llvm::json::OStream &OS) {
  StringRef Kind;
  switch (Req.getKind()) {
    case RequirementKind::SameShape:
      Kind = "sameShape";
      break;
    case RequirementKind::Conformance:
      Kind = "conformance";
      break;
    case RequirementKind::Superclass:
      Kind = "superclass";
      break;
    case RequirementKind::SameType:
      Kind = "sameType";
      break;
    case RequirementKind::Layout:
      return;
  }

  OS.object([&](){
    OS.attribute("kind", Kind);
    OS.attribute("lhs", Req.getFirstType()->getString());
    OS.attribute("rhs", Req.getSecondType()->getString());
    
    // If the RHS type has a USR we can link to, add it to the output
    if (auto *TyDecl = Req.getSecondType()->getAnyNominal()) {
      SmallString<256> USR;
      {
        llvm::raw_svector_ostream SOS(USR);
        ide::printDeclUSR(TyDecl, SOS);
      }
      OS.attribute("rhsPrecise", USR.str());
    }
  });
}

void nextcode::symbolgraphgen::serialize(const nextcode::GenericTypeParamType *Param,
                                      llvm::json::OStream &OS) {
  OS.object([&](){
    OS.attribute("name", Param->getName().str());
    OS.attribute("index", Param->getIndex());
    OS.attribute("depth", Param->getDepth());
  });
}

void nextcode::symbolgraphgen::serialize(const ModuleDecl &Module,
                                      llvm::json::OStream &OS,
                                      llvm::Triple Target) {
  auto *MainFile = Module.getFiles().front();
  switch (MainFile->getKind()) {
  case FileUnitKind::Builtin:
    llvm_unreachable("Unexpected module kind: Builtin");
  case FileUnitKind::DWARFModule:
    llvm_unreachable("Unexpected module kind: DWARFModule");
  case FileUnitKind::Synthesized:
    llvm_unreachable("Unexpected module kind: Synthesized");
    break;
  case FileUnitKind::Source:
    serialize(Module.getASTContext().LangOpts.Target, OS);
    break;
  case FileUnitKind::SerializedAST: {
    auto SerializedAST = cast<SerializedASTFile>(MainFile);
    auto Target = llvm::Triple(SerializedAST->getTargetTriple());
    serialize(Target, OS);
    break;
  }
  case FileUnitKind::ClangModule: {
    auto ClangModule = cast<ClangModuleUnit>(MainFile);
    if (const auto *Overlay = ClangModule->getOverlayModule()) {
      serialize(*Overlay, OS, Target);
    } else {
      serialize(Target, OS);
    }
    break;
  }
  }
}

void
nextcode::symbolgraphgen::filterGenericParams(
    ArrayRef<GenericTypeParamType *> GenericParams,
    SmallVectorImpl<const GenericTypeParamType*> &FilteredParams,
    SubstitutionMap SubMap) {

  for (auto Param : GenericParams) {
    if (const auto *GPD = Param->getDecl()) {

      // Ignore the implicit Self param
      if (GPD->isImplicit()) {
        if (!isa<ExtensionDecl>(GPD->getDeclContext()))
          continue;

        // Extension decls (and their children) refer to implicit copies of the
        // explicit params of the nominal they extend. Don't filter those out.
        auto *ED = cast<ExtensionDecl>(GPD->getDeclContext());
        if (auto *NTD = ED->getExtendedNominal()) {
          if (auto *GPL = NTD->getGenericParams()) {
            auto ImplicitAndSameName = [&](GenericTypeParamDecl *NominalGPD) {
              return NominalGPD->isImplicit() &&
                  GPD->getName() == NominalGPD->getName();
            };
            if (llvm::any_of(GPL->getParams(), ImplicitAndSameName))
              continue;
          }
        }
      }

      // Ignore parameters that have been substituted.
      if (!SubMap.empty()) {
        Type SubTy = Type(Param).subst(SubMap);
        if (!SubTy->hasError() && SubTy.getPointer() != Param) {
          if (!SubTy->is<ArchetypeType>()) {
            continue;
          }
          auto AT = SubTy->castTo<ArchetypeType>();
          if (!AT->getInterfaceType()->isEqual(Param)) {
            continue;
          }
        }
      }

      FilteredParams.push_back(Param);
    }
  }
}

// FIXME: This is wrong. We should instead compute the new requirements of a
// member declaration by comparing against the generic signature of its
// parent, with getRequirementsNotSatisfiedBy().
static bool containsParams(nextcode::Type Ty, llvm::ArrayRef<const nextcode::GenericTypeParamType*> Others) {
  return Ty.findIf([&](nextcode::Type T) -> bool {
    if (auto AT = T->getAs<nextcode::ArchetypeType>()) {
      T = AT->getInterfaceType();
    }

    for (auto *Param: Others) {
      if (T->isEqual(const_cast<nextcode::GenericTypeParamType*>(Param)))
        return true;
    }
    return false;
  });
}

void nextcode::symbolgraphgen::filterGenericRequirements(
    ArrayRef<Requirement> Requirements,
    const ProtocolDecl *Self,
    SmallVectorImpl<Requirement> &FilteredRequirements,
    SubstitutionMap SubMap,
    ArrayRef<const GenericTypeParamType *> FilteredParams) {

  for (const auto &Req : Requirements) {
    // FIXME: We're just dropping "Self: AnyObject", etc.
    if (Req.getKind() == RequirementKind::Layout) {
      continue;
    }
    // extension /* protocol */ Q {
    // func foo() {}
    // }
    // ignore Self : Q, obvious
    if (Self &&
        Req.getKind() == RequirementKind::Conformance &&
        Req.getProtocolDecl() == Self) {
      continue;
    }

    // Ignore requirements that don't involve the filtered set of generic
    // parameters after substitution.
    if (!SubMap.empty()) {
      Type SubFirst = Req.getFirstType().subst(SubMap);
      if (SubFirst->hasError())
        SubFirst = Req.getFirstType();
      Type SubSecond = Req.getSecondType().subst(SubMap);
      if (SubSecond->hasError())
        SubSecond = Req.getSecondType();

      if (!containsParams(SubFirst, FilteredParams) &&
          !containsParams(SubSecond, FilteredParams))
        continue;

      // Use the same requirement kind with the substituted types.
      FilteredRequirements.emplace_back(Req.getKind(), SubFirst, SubSecond);
    } else {
      // Use the original requirement.
      FilteredRequirements.push_back(Req);
    }
  }
}
void
nextcode::symbolgraphgen::filterGenericRequirements(const ExtensionDecl *Extension,
    SmallVectorImpl<Requirement> &FilteredRequirements) {
  auto Sig = Extension->getGenericSignature();
  if (!Sig)
    return;

  SmallVector<Requirement, 2> Reqs;
  SmallVector<InverseRequirement, 2> InverseReqs;
  Sig->getRequirementsWithInverses(Reqs, InverseReqs);
  // FIXME(noncopyable_generics): Do something with InverseReqs, or just use
  // getRequirements() above and update the tests.

  for (const auto &Req : Reqs) {
    if (Req.getKind() == RequirementKind::Layout) {
      continue;
    }

    // extension /* protocol */ Q
    // ignore Self : Q, obvious
    if (auto *Proto = Extension->getExtendedProtocolDecl()) {
      if (Req.getKind() == RequirementKind::Conformance &&
          Req.getFirstType()->isEqual(Extension->getSelfInterfaceType()) &&
          Req.getProtocolDecl() == Proto) {
        continue;
      }
    }

    FilteredRequirements.push_back(Req);
  }
}
