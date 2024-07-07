//===--- IndexSymbol.h - Index symbol data types ----------------*- C++ -*-===//
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

#ifndef NEXTCODE_INDEX_INDEXSYMBOL_H
#define NEXTCODE_INDEX_INDEXSYMBOL_H

#include "nextcode/Basic/LLVM.h"
#include "clang/Index/IndexSymbol.h"

namespace nextcode {
class Decl;
class ValueDecl;
class ModuleEntity;
enum class AccessorKind;

namespace index {

using clang::index::SymbolKind;
using clang::index::SymbolLanguage;
using clang::index::SymbolSubKind;
using clang::index::SymbolProperty;
using clang::index::SymbolPropertySet;
using clang::index::SymbolRole;
using clang::index::SymbolRoleSet;
using clang::index::SymbolRelation;
using clang::index::SymbolInfo;

inline SymbolPropertySet operator&(SymbolPropertySet SKSet, SymbolProperty SK) {
  return SKSet & (SymbolPropertySet)SK;
}
inline SymbolPropertySet operator|(SymbolPropertySet SKSet, SymbolProperty SK) {
  return SKSet | (SymbolPropertySet)SK;
}
inline SymbolPropertySet &operator|=(SymbolPropertySet &SKSet, SymbolProperty SK) {
  return SKSet = SKSet | SK;
}

struct IndexRelation {
  const Decl *decl;
  SymbolInfo symInfo;
  SymbolRoleSet roles = SymbolRoleSet(0);

  // The following strings are guaranteed to live at least as long as the
  // current indexing action.
  StringRef name;
  StringRef USR; // USR may be safely compared by pointer.
  StringRef group;

  IndexRelation(SymbolRoleSet Roles, const Decl *Sym, SymbolInfo SymInfo, StringRef Name, StringRef USR)
  : decl(Sym), symInfo(SymInfo), roles(Roles), name(Name), USR(USR) {}

  IndexRelation() = default;
};

struct IndexSymbol : IndexRelation {
  SmallVector<IndexRelation, 3> Relations;
  unsigned line = 0;
  unsigned column = 0;

  IndexSymbol() = default;

  StringRef getReceiverUSR() const {
    for (auto Relation: Relations) {
      if (Relation.roles & (SymbolRoleSet) SymbolRole::RelationReceivedBy)
        return Relation.USR;
    }
    return StringRef();
  }
};

SymbolInfo getSymbolInfoForModule(ModuleEntity Mod);
SymbolInfo getSymbolInfoForDecl(const Decl *D);
SymbolSubKind getSubKindForAccessor(AccessorKind AK);
bool isLocalSymbol(const Decl *D);

using clang::index::printSymbolProperties;

} // end namespace index
} // end namespace nextcode

#endif // NEXTCODE_INDEX_INDEXSYMBOL_H
