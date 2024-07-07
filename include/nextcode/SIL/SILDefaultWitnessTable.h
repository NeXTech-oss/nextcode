//===--- SILDefaultWitnessTable.h -------------------------------*- C++ -*-===//
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
//
// This file defines the SILDefaultWitnessTable class, which is used to provide
// default implementations of protocol requirements for resilient protocols,
// allowing IRGen to generate the appropriate metadata so that the runtime can
// insert those requirements to witness tables that were emitted prior to the
// requirement being added.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SIL_SILDEFAULTWITNESSTABLE_H
#define NEXTCODE_SIL_SILDEFAULTWITNESSTABLE_H

#include "nextcode/SIL/SILAllocated.h"
#include "nextcode/SIL/SILDeclRef.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILWitnessTable.h"
#include "llvm/ADT/ilist_node.h"
#include "llvm/ADT/ilist.h"
#include <string>

namespace nextcode {

class ProtocolDecl;
class SILFunction;
class SILModule;

/// A mapping from each requirement of a protocol to the SIL-level entity
/// satisfying the requirement for conformances which do not explicitly
/// provide a witness.
class SILDefaultWitnessTable : public llvm::ilist_node<SILDefaultWitnessTable>,
                               public SILAllocated<SILDefaultWitnessTable>
{
public:
  /// A default witness table entry describing the default witness for a
  /// requirement.
  using Entry = SILWitnessTable::Entry;
 
private:
  /// The module which contains the SILDefaultWitnessTable.
  SILModule &Mod;

  /// The linkage of the witness table.
  SILLinkage Linkage;

  /// The protocol declaration to which this default witness table applies.
  const ProtocolDecl *Protocol;

  /// The minimum size of a valid witness table conforming to this protocol,
  /// with all resilient default requirements omitted.
  unsigned MinimumWitnessTableSizeInWords;

  /// The various witnesses containing in this default witness table.
  MutableArrayRef<Entry> Entries;

  /// Temporary state while SILGen is emitting a default witness table.
  /// We can never have a true declaration since there's no way to reference
  /// the default witness table from outside its defining translation unit.
  bool IsDeclaration;

  /// Private constructor for making SILDefaultWitnessTable declarations.
  SILDefaultWitnessTable(SILModule &M, SILLinkage Linkage,
                         const ProtocolDecl *Protocol);

  /// Private constructor for making SILDefaultWitnessTable definitions.
  SILDefaultWitnessTable(SILModule &M, SILLinkage Linkage,
                         const ProtocolDecl *Protocol,
                         ArrayRef<Entry> entries);

  void addDefaultWitnessTable();

public:
  /// Create a new SILDefaultWitnessTable declaration.
  static SILDefaultWitnessTable *create(SILModule &M, SILLinkage Linkage,
                                        const ProtocolDecl *Protocol);

  /// Create a new SILDefaultWitnessTable definition with the given entries.
  static SILDefaultWitnessTable *create(SILModule &M, SILLinkage Linkage,
                                        const ProtocolDecl *Protocol,
                                        ArrayRef<Entry> entries);

  /// Get a name that uniquely identifies this default witness table.
  ///
  /// Note that this is /not/ valid as a symbol name; it is only guaranteed to
  /// be unique among default witness tables, not all symbols.
  std::string getUniqueName() const;

  /// Get the linkage of the default witness table.
  SILLinkage getLinkage() const { return Linkage; }

  /// Set the linkage of the default witness table.
  void setLinkage(SILLinkage l) { Linkage = l; }

  void convertToDefinition(ArrayRef<Entry> entries);

  ~SILDefaultWitnessTable();

  SILModule &getModule() const { return Mod; }

  /// Return true if this is a declaration with no body.
  bool isDeclaration() const { return IsDeclaration; }

  /// Return the AST ProtocolDecl this default witness table is associated with.
  const ProtocolDecl *getProtocol() const { return Protocol; }

  /// Clears methods in witness entries.
  /// \p predicate Returns true if the passed entry should be set to null.
  template <typename Predicate> void clearMethods_if(Predicate predicate) {
    for (Entry &entry : Entries) {
      if (!entry.isValid())
        continue;
      if (entry.getKind() != SILWitnessTable::Method)
        continue;

      auto *MW = entry.getMethodWitness().Witness;
      if (MW && predicate(MW)) {
        entry.removeWitnessMethod();
      }
    }
  }

  /// Return all of the default witness table entries.
  ArrayRef<Entry> getEntries() const { return Entries; }

  /// Verify that the default witness table is well-formed.
  void verify(const SILModule &M) const;
  
  /// Print the default witness table.
  void print(llvm::raw_ostream &OS, bool Verbose = false) const;

  /// Dump the default witness table to stderr.
  void dump() const;
};
  
} // end nextcode namespace

//===----------------------------------------------------------------------===//
// ilist_traits for SILDefaultWitnessTable
//===----------------------------------------------------------------------===//

namespace llvm {
  
template <>
struct ilist_traits<::nextcode::SILDefaultWitnessTable> :
public ilist_node_traits<::nextcode::SILDefaultWitnessTable> {
  using SILDefaultWitnessTable = ::nextcode::SILDefaultWitnessTable;

public:
  static void deleteNode(SILDefaultWitnessTable *WT) { WT->~SILDefaultWitnessTable(); }
  
private:
  void createNode(const SILDefaultWitnessTable &);
};

} // end llvm namespace

#endif