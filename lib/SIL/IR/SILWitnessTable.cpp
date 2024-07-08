//===--- SILWitnessTable.cpp - Defines the SILWitnessTable class ----------===//
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
// This file defines the SILWitnessTable class, which is used to map a protocol
// conformance for a type to its implementing SILFunctions. This information is
// used by IRGen to create witness tables for protocol dispatch.
//
// It can also be used by generic specialization and existential
// devirtualization passes to promote witness_method instructions to static
// function_refs.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "sil-witness-table"
#include "nextcode/SIL/SILWitnessTable.h"
#include "nextcode/AST/ASTMangler.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/ProtocolConformance.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILModule.h"
#include "llvm/ADT/SmallString.h"

using namespace nextcode;

static std::string mangleConstant(RootProtocolConformance *C) {
  Mangle::ASTMangler Mangler;
  return Mangler.mangleWitnessTable(C);
}

DeclContext *SILWitnessTable::getDeclContext() const {
  return getConformance()->getDeclContext();
}

ProtocolDecl *SILWitnessTable::getProtocol() const {
  return getConformance()->getProtocol();
}

NominalTypeDecl *SILWitnessTable::getConformingNominal() const {
  return getConformance()->getDeclContext()->getSelfNominalTypeDecl();
}

void SILWitnessTable::addWitnessTable() {
  // Make sure we have not seen this witness table yet.
  assert(Mod.WitnessTableMap.find(Conformance) ==
         Mod.WitnessTableMap.end() && "Attempting to create duplicate "
         "witness table.");
  Mod.WitnessTableMap[Conformance] = this;
  Mod.witnessTables.push_back(this);
}

SILWitnessTable *SILWitnessTable::create(
    SILModule &M, SILLinkage Linkage, SerializedKind_t SerializedKind,
    RootProtocolConformance *Conformance,
    ArrayRef<SILWitnessTable::Entry> entries,
    ArrayRef<ConditionalConformance> conditionalConformances) {
  assert(Conformance && "Cannot create a witness table for a null "
         "conformance.");

  // Create the mangled name of our witness table...
  Identifier Name = M.getASTContext().getIdentifier(mangleConstant(Conformance));

  LLVM_DEBUG(llvm::dbgs() << "SILWitnessTable Creating: " << Name.str() << '\n');

  // Allocate the witness table and initialize it.
  void *buf = M.allocate(sizeof(SILWitnessTable), alignof(SILWitnessTable));
  SILWitnessTable *wt = ::new (buf)
      SILWitnessTable(M, Linkage, SerializedKind, Name.str(), Conformance, entries,
                      conditionalConformances);

  wt->addWitnessTable();

  // Return the resulting witness table.
  return wt;
}

SILWitnessTable *
SILWitnessTable::create(SILModule &M, SILLinkage Linkage,
                        RootProtocolConformance *Conformance) {
  assert(Conformance && "Cannot create a witness table for a null "
         "conformance.");

  // Create the mangled name of our witness table...
  Identifier Name = M.getASTContext().getIdentifier(mangleConstant(Conformance));


  // Allocate the witness table and initialize it.
  void *buf = M.allocate(sizeof(SILWitnessTable), alignof(SILWitnessTable));
  SILWitnessTable *wt = ::new (buf) SILWitnessTable(M, Linkage, Name.str(),
                                                    Conformance);

  wt->addWitnessTable();

  // Return the resulting witness table.
  return wt;
}

SILWitnessTable::SILWitnessTable(
    SILModule &M, SILLinkage Linkage, SerializedKind_t SerializedKind, StringRef N,
    RootProtocolConformance *Conformance, ArrayRef<Entry> entries,
    ArrayRef<ConditionalConformance> conditionalConformances)
    : Mod(M), Name(N), Linkage(Linkage), Conformance(Conformance), Entries(),
      ConditionalConformances(), IsDeclaration(true),
      SerializedKind(SerializedKind) {
  convertToDefinition(entries, conditionalConformances, SerializedKind);
}

SILWitnessTable::SILWitnessTable(SILModule &M, SILLinkage Linkage, StringRef N,
                                 RootProtocolConformance *Conformance)
    : Mod(M), Name(N), Linkage(Linkage), Conformance(Conformance), Entries(),
      ConditionalConformances(), IsDeclaration(true),
      SerializedKind(IsNotSerialized) {}

SILWitnessTable::~SILWitnessTable() {
  if (isDeclaration())
    return;

  // Drop the reference count of witness functions referenced by this table.
  for (auto entry : getEntries()) {
    switch (entry.getKind()) {
    case Method:
      if (entry.getMethodWitness().Witness) {
        entry.getMethodWitness().Witness->decrementRefCount();
      }
      break;
    case AssociatedType:
    case AssociatedTypeProtocol:
    case BaseProtocol:
    case Invalid:
      break;
    }
  }
}

void SILWitnessTable::convertToDefinition(
    ArrayRef<Entry> entries,
    ArrayRef<ConditionalConformance> conditionalConformances,
    SerializedKind_t serializedKind) {
  assert(isDeclaration() && "Definitions should never call this method.");
  IsDeclaration = false;
  SerializedKind = serializedKind;

  Entries = Mod.allocateCopy(entries);
  ConditionalConformances = Mod.allocateCopy(conditionalConformances);

  // Bump the reference count of witness functions referenced by this table.
  for (auto entry : getEntries()) {
    switch (entry.getKind()) {
    case Method:
      if (entry.getMethodWitness().Witness) {
        entry.getMethodWitness().Witness->incrementRefCount();
      }
      break;
    case AssociatedType:
    case AssociatedTypeProtocol:
    case BaseProtocol:
    case Invalid:
      break;
    }
  }
}

SerializedKind_t SILWitnessTable::conformanceSerializedKind(
                                                            const RootProtocolConformance *conformance) {

  auto optInPackage = conformance->getDeclContext()->getParentModule()->serializePackageEnabled();
  auto accessLevelToCheck =
  optInPackage ? AccessLevel::Package : AccessLevel::Public;

  auto normalConformance = dyn_cast<NormalProtocolConformance>(conformance);
  if (normalConformance && normalConformance->isResilient() && !optInPackage)
    return IsNotSerialized;

  if (conformance->getProtocol()->getEffectiveAccess() < accessLevelToCheck)
    return IsNotSerialized;

  auto *nominal = conformance->getDeclContext()->getSelfNominalTypeDecl();
  if (nominal->getEffectiveAccess() >= accessLevelToCheck)
    return IsSerialized;

  return IsNotSerialized;
}

bool SILWitnessTable::enumerateWitnessTableConditionalConformances(
    const ProtocolConformance *conformance,
    llvm::function_ref<bool(unsigned, CanType, ProtocolDecl *)> fn) {
  unsigned conformanceIndex = 0;

  // For a protocol P that conforms to another protocol, introduce a conditional
  // requirement for that P's Self: P.
  if (isa<NormalProtocolConformance>(conformance->getRootConformance())) {
    if (auto selfProto = conformance->getDeclContext()->getSelfProtocolDecl()) {
      if (Lowering::TypeConverter::protocolRequiresWitnessTable(selfProto)) {
        auto selfType = selfProto->getSelfInterfaceType()->getCanonicalType();
        if (fn(conformanceIndex, selfType, selfProto))
          return true;

        ++conformanceIndex;
      }
    }
  }

  for (auto req : conformance->getConditionalRequirements()) {
    if (req.getKind() != RequirementKind::Conformance)
      continue;

    auto proto = req.getProtocolDecl();

    if (Lowering::TypeConverter::protocolRequiresWitnessTable(proto)) {
      if (fn(conformanceIndex, req.getFirstType()->getCanonicalType(), proto))
        return true;

      ++conformanceIndex;
    }
  }
  return false;
}
