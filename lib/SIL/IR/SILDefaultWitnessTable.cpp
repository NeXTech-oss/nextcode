//===--- SILDefaultWitnessTable.cpp ---------------------------------------===//
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
// This file defines the SILDefaultWitnessTable class, which is used to provide
// default implementations of protocol requirements for resilient protocols,
// allowing IRGen to generate the appropriate metadata so that the runtime can
// insert those requirements to witness tables that were emitted prior to the
// requirement being added.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ASTMangler.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/SILDefaultWitnessTable.h"
#include "nextcode/SIL/SILModule.h"
#include "llvm/ADT/SmallString.h"

using namespace nextcode;

void SILDefaultWitnessTable::addDefaultWitnessTable() {
  // Make sure we have not seen this witness table yet.
  assert(Mod.DefaultWitnessTableMap.find(Protocol) ==
         Mod.DefaultWitnessTableMap.end() && "Attempting to create duplicate "
         "default witness table.");
  Mod.DefaultWitnessTableMap[Protocol] = this;
  Mod.defaultWitnessTables.push_back(this);
}

SILDefaultWitnessTable *
SILDefaultWitnessTable::create(SILModule &M, SILLinkage Linkage,
                               const ProtocolDecl *Protocol,
                               ArrayRef<SILDefaultWitnessTable::Entry> entries){
  // Allocate the witness table and initialize it.
  auto *buf = M.allocate<SILDefaultWitnessTable>(1);
  SILDefaultWitnessTable *wt =
      ::new (buf) SILDefaultWitnessTable(M, Linkage, Protocol, entries);

  wt->addDefaultWitnessTable();

  // Return the resulting default witness table.
  return wt;
}

SILDefaultWitnessTable *
SILDefaultWitnessTable::create(SILModule &M, SILLinkage Linkage,
                               const ProtocolDecl *Protocol) {
  // Allocate the witness table and initialize it.
  auto *buf = M.allocate<SILDefaultWitnessTable>(1);
  SILDefaultWitnessTable *wt =
      ::new (buf) SILDefaultWitnessTable(M, Linkage, Protocol);

  wt->addDefaultWitnessTable();

  // Return the resulting default witness table.
  return wt;
}

SILDefaultWitnessTable::
SILDefaultWitnessTable(SILModule &M,
                       SILLinkage Linkage,
                       const ProtocolDecl *Protocol,
                       ArrayRef<Entry> entries)
  : Mod(M), Linkage(Linkage), Protocol(Protocol), Entries(),
    IsDeclaration(true) {

  convertToDefinition(entries);
}

SILDefaultWitnessTable::SILDefaultWitnessTable(SILModule &M,
                                               SILLinkage Linkage,
                                               const ProtocolDecl *Protocol)
  : Mod(M), Linkage(Linkage), Protocol(Protocol), Entries(),
    IsDeclaration(true) {}

void SILDefaultWitnessTable::
convertToDefinition(ArrayRef<Entry> entries) {
  assert(IsDeclaration);
  IsDeclaration = false;

  Entries = Mod.allocateCopy(entries);

  // Bump the reference count of witness functions referenced by this table.
  for (auto entry : getEntries()) {
    if (entry.isValid() && entry.getKind() == SILWitnessTable::Method) {
      if (SILFunction *f = entry.getMethodWitness().Witness)
        f->incrementRefCount();
    }
  }
}

std::string SILDefaultWitnessTable::getUniqueName() const {
  Mangle::ASTMangler Mangler;
  return Mangler.mangleTypeWithoutPrefix(
    getProtocol()->getDeclaredInterfaceType());
}

SILDefaultWitnessTable::~SILDefaultWitnessTable() {
  // Drop the reference count of witness functions referenced by this table.
  for (auto entry : getEntries()) {
    if (entry.isValid() && entry.getKind() == SILWitnessTable::Method) {
      if (SILFunction *f = entry.getMethodWitness().Witness)
        f->decrementRefCount();
    }
  }
}
