//===--- ProtocolAssociations.h - Associated types/conformances -*- C++ -*-===//
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
// This file defines types for representing types and conformances
// associated with a protocol.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_PROTOCOLASSOCIATIONS_H
#define NEXTCODE_AST_PROTOCOLASSOCIATIONS_H

#include "nextcode/AST/Decl.h"
#include "llvm/ADT/DenseMapInfo.h"

namespace nextcode {

/// A type associated with a protocol.
///
/// This struct exists largely so that we can maybe eventually
/// generalize it to an arbitrary path.
class AssociatedType {
  AssociatedTypeDecl *Association;
  using AssociationInfo = llvm::DenseMapInfo<AssociatedTypeDecl*>;

  struct SpecialValue {};
  explicit AssociatedType(SpecialValue _, AssociatedTypeDecl *specialValue)
      : Association(specialValue) {}

public:
  explicit AssociatedType(AssociatedTypeDecl *association)
      : Association(association) {
    assert(association);
  }

  ProtocolDecl *getSourceProtocol() const {
    return Association->getProtocol();
  }

  AssociatedTypeDecl *getAssociation() const {
    return Association;
  }

  friend bool operator==(AssociatedType lhs, AssociatedType rhs) {
    return lhs.Association == rhs.Association;
  }
  friend bool operator!=(AssociatedType lhs, AssociatedType rhs) {
    return !(lhs == rhs);
  }

  unsigned getHashValue() const {
    return llvm::hash_value(Association);
  }

  static AssociatedType getEmptyKey() {
    return AssociatedType(SpecialValue(), AssociationInfo::getEmptyKey());
  }
  static AssociatedType getTombstoneKey() {
    return AssociatedType(SpecialValue(), AssociationInfo::getTombstoneKey());
  }
};

/// A base conformance of a protocol.
class BaseConformance {
  ProtocolDecl *Source;
  ProtocolDecl *Requirement;

public:
  explicit BaseConformance(ProtocolDecl *source,
                                 ProtocolDecl *requirement)
      : Source(source), Requirement(requirement) {
    assert(source && requirement);
  }

  ProtocolDecl *getSourceProtocol() const {
    return Source;
  }

  ProtocolDecl *getBaseRequirement() const {
    return Requirement;
  }
};

/// A conformance associated with a protocol.
class AssociatedConformance {
  ProtocolDecl *Source;
  CanType Association;
  ProtocolDecl *Requirement;

  using SourceInfo = llvm::DenseMapInfo<ProtocolDecl*>;

  explicit AssociatedConformance(ProtocolDecl *specialValue)
      : Source(specialValue), Association(CanType()), Requirement(nullptr) {}

public:
  explicit AssociatedConformance(ProtocolDecl *source, CanType association,
                                 ProtocolDecl *requirement)
      : Source(source), Association(association), Requirement(requirement) {
    assert(source && association && requirement);
  }

  ProtocolDecl *getSourceProtocol() const {
    return Source;
  }

  CanType getAssociation() const {
    return Association;
  }

  ProtocolDecl *getAssociatedRequirement() const {
    return Requirement;
  }

  friend bool operator==(const AssociatedConformance &lhs,
                         const AssociatedConformance &rhs) {
    return lhs.Source == rhs.Source &&
           lhs.Association == rhs.Association &&
           lhs.Requirement == rhs.Requirement;
  }
  friend bool operator!=(const AssociatedConformance &lhs,
                         const AssociatedConformance &rhs) {
    return !(lhs == rhs);
  }

  unsigned getHashValue() const {
    return hash_value(llvm::hash_combine(Source,
                                         Association.getPointer(),
                                         Requirement));
  }

  static AssociatedConformance getEmptyKey() {
    return AssociatedConformance(SourceInfo::getEmptyKey());
  }
  static AssociatedConformance getTombstoneKey() {
    return AssociatedConformance(SourceInfo::getTombstoneKey());
  }
};

} // end namespace nextcode

namespace llvm {
  template <> struct DenseMapInfo<nextcode::AssociatedType> {
    static inline nextcode::AssociatedType getEmptyKey() {
      return nextcode::AssociatedType::getEmptyKey();
    }

    static inline nextcode::AssociatedType getTombstoneKey() {
      return nextcode::AssociatedType::getTombstoneKey();
    }

    static unsigned getHashValue(nextcode::AssociatedType val) {
      return val.getHashValue();
    }

    static bool isEqual(nextcode::AssociatedType lhs, nextcode::AssociatedType rhs) {
      return lhs == rhs;
    }
  };

  template <> struct DenseMapInfo<nextcode::AssociatedConformance> {
    static inline nextcode::AssociatedConformance getEmptyKey() {
      return nextcode::AssociatedConformance::getEmptyKey();
    }

    static inline nextcode::AssociatedConformance getTombstoneKey() {
      return nextcode::AssociatedConformance::getTombstoneKey();
    }

    static unsigned getHashValue(nextcode::AssociatedConformance val) {
      return val.getHashValue();
    }

    static bool isEqual(nextcode::AssociatedConformance lhs,
                        nextcode::AssociatedConformance rhs) {
      return lhs == rhs;
    }
  };
}

#endif
