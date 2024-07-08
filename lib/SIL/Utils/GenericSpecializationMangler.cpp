//===--- GenericSpecializationMangler.cpp - mangling of specializations ---===//
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

#include "nextcode/SIL/GenericSpecializationMangler.h"
#include "nextcode/AST/GenericEnvironment.h"
#include "nextcode/AST/GenericSignature.h"
#include "nextcode/AST/SubstitutionMap.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Demangling/ManglingMacros.h"

using namespace nextcode;
using namespace Mangle;

void SpecializationMangler::beginMangling() {
  ASTMangler::beginManglingWithoutPrefix();

  if (Serialized)
    ArgOpBuffer << 'q';

  if (RemovedEffects.contains(EffectKind::Async))
    ArgOpBuffer << 'a';

  ArgOpBuffer << char(uint8_t(Pass) + '0');
}

namespace {

/// Utility class for demangling specialization attributes.
class AttributeDemangler : public Demangle::Demangler {
public:
  void demangleAndAddAsChildren(StringRef MangledSpecialization,
                                NodePointer Parent) {
    DemangleInitRAII state(*this, MangledSpecialization, nullptr);
    if (!parseAndPushNodes()) {
      llvm::errs() << "Can't demangle: " << MangledSpecialization << '\n';
      abort();
    }
    for (Node *Nd : NodeStack) {
      addChild(Parent, Nd);
    }
  }
};

} // namespace

std::string SpecializationMangler::finalize() {
  StringRef MangledSpecialization(Storage.data(), Storage.size());
  AttributeDemangler D;
  NodePointer TopLevel = D.createNode(Node::Kind::Global);
  D.demangleAndAddAsChildren(MangledSpecialization, TopLevel);

  StringRef FuncName = Function ? Function->getName() : StringRef(FunctionName);
  NodePointer FuncTopLevel = nullptr;
  if (FuncName.starts_with(MANGLING_PREFIX_STR)) {
    FuncTopLevel = D.demangleSymbol(FuncName);
    assert(FuncTopLevel);
  }
  if (!FuncTopLevel) {
    FuncTopLevel = D.createNode(Node::Kind::Global);
    FuncTopLevel->addChild(D.createNode(Node::Kind::Identifier, FuncName), D);
  }
  for (NodePointer FuncChild : *FuncTopLevel) {
    TopLevel->addChild(FuncChild, D);
  }
  auto mangling = Demangle::mangleNode(TopLevel);
  assert(mangling.isSuccess());
  std::string mangledName = mangling.result();
  verify(mangledName);
  return mangledName;
}

//===----------------------------------------------------------------------===//
//                           Generic Specialization
//===----------------------------------------------------------------------===//

void GenericSpecializationMangler::
appendSubstitutions(GenericSignature sig, SubstitutionMap subs) {
  bool First = true;
  sig->forEachParam([&](GenericTypeParamType *ParamType, bool Canonical) {
    if (Canonical) {
      auto ty = Type(ParamType);
      auto substTy = ty.subst(subs);
      auto canTy = substTy->getCanonicalType();
      appendType(canTy, nullptr);
      appendListSeparator(First);
    }
  });
  assert(!First && "no generic substitutions");
}

std::string GenericSpecializationMangler::
manglePrespecialized(GenericSignature sig, SubstitutionMap subs) {
  beginMangling();
  appendSubstitutions(sig, subs);
  appendSpecializationOperator("Ts");
  return finalize();
}
                                  
std::string GenericSpecializationMangler::
mangleNotReabstracted(SubstitutionMap subs,
                      bool metatyeParamsRemoved) {
  beginMangling();
  appendSubstitutions(getGenericSignature(), subs);
  if (metatyeParamsRemoved) {
    appendSpecializationOperator("TGm");
  } else {
    appendSpecializationOperator("TG");
  }
  return finalize();
}
                                  
std::string GenericSpecializationMangler::
mangleReabstracted(SubstitutionMap subs, bool alternativeMangling,
                   bool metatyeParamsRemoved) {
  beginMangling();
  appendSubstitutions(getGenericSignature(), subs);
  
  // See ReabstractionInfo::hasConvertedResilientParams for why and when to use
  // the alternative mangling.
  if (metatyeParamsRemoved) {
    appendSpecializationOperator(alternativeMangling ? "TBm" : "Tgm");
  } else {
    appendSpecializationOperator(alternativeMangling ? "TB" : "Tg");
  }
  return finalize();
}

std::string GenericSpecializationMangler::
mangleForDebugInfo(GenericSignature sig, SubstitutionMap subs, bool forInlining) {
  beginMangling();
  appendSubstitutions(sig, subs);
  appendSpecializationOperator(forInlining ? "Ti" : "TG");
  return finalize();
}


static SubstitutionMap
getSubstitutionMapForPrespecialization(GenericSignature genericSig,
                                       GenericSignature specSig) {
  auto CalleeGenericSig = genericSig;
  auto SpecializedGenericSig = specSig;
  auto SpecializedGenericEnv = specSig.getGenericEnvironment();

  auto CalleeInterfaceToSpecializedInterfaceMap = SubstitutionMap::get(
      CalleeGenericSig,
      [&](SubstitutableType *type) -> Type {
        return type;
      },
      LookUpConformanceInSignature(CalleeGenericSig.getPointer()));

  auto subs = SubstitutionMap::get(
      CalleeGenericSig,
      [&](SubstitutableType *type) -> Type {
        auto SpecializedInterfaceTy =
            Type(type).subst(CalleeInterfaceToSpecializedInterfaceMap);
        return SpecializedGenericEnv->mapTypeIntoContext(
            SpecializedInterfaceTy);
      },
      LookUpConformanceInSignature(SpecializedGenericSig.getPointer()));
  return subs;
}

std::string GenericSpecializationMangler::manglePrespecialization(
    std::string unspecializedName, GenericSignature genericSig,
    GenericSignature specializedSig) {
  auto subs =
      getSubstitutionMapForPrespecialization(genericSig, specializedSig);
  GenericSpecializationMangler mangler(unspecializedName);
  return mangler.manglePrespecialized(genericSig, subs);
}
