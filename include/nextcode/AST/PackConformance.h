//===--- PackConformance.h - Variadic Protocol Conformance ------*- C++ -*-===//
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
//
//===----------------------------------------------------------------------===//
//
// This file defines the PackConformance structure, which describes the
// conformance of a type pack parameter to a protocol.
//
//===----------------------------------------------------------------------===//
#ifndef NEXTCODE_AST_PACKCONFORMANCE_H
#define NEXTCODE_AST_PACKCONFORMANCE_H

#include "nextcode/AST/ASTAllocated.h"
#include "nextcode/AST/ProtocolConformanceRef.h"
#include "nextcode/AST/Type.h"
#include "nextcode/AST/TypeAlignments.h"
#include "nextcode/Basic/Compiler.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/FoldingSet.h"
#include "llvm/Support/TrailingObjects.h"

namespace nextcode {

class PackType;

class alignas(1 << DeclAlignInBits) PackConformance final
  : public ASTAllocated<PackConformance>,
    public llvm::FoldingSetNode,
    private llvm::TrailingObjects<PackConformance, ProtocolConformanceRef> {
  friend class ASTContext;
  friend TrailingObjects;

  /// The pack type conforming to the protocol.
  PackType *ConformingType;

  /// The conformed-to protocol.
  ProtocolDecl *Protocol;

public:
  void Profile(llvm::FoldingSetNodeID &ID) const;
  static void Profile(llvm::FoldingSetNodeID &ID,
                      PackType *conformingType,
                      ProtocolDecl *protocol,
                      ArrayRef<ProtocolConformanceRef> conformances);

private:
  PackConformance(PackType *conformingType,
                  ProtocolDecl *protocol,
                  ArrayRef<ProtocolConformanceRef> conformances);

  size_t numTrailingObjects(OverloadToken<ProtocolConformanceRef>) const;

public:
  static PackConformance *get(PackType *conformingType,
                              ProtocolDecl *protocol,
                              ArrayRef<ProtocolConformanceRef> conformances);

  PackType *getType() const { return ConformingType; }

  ProtocolDecl *getProtocol() const { return Protocol; }

  ArrayRef<ProtocolConformanceRef> getPatternConformances() const;

  bool isInvalid() const;

  bool isCanonical() const;

  PackConformance *getCanonicalConformance() const;

  PackType *getAssociatedType(Type assocType) const;

  PackConformance *
  getAssociatedConformance(Type assocType, ProtocolDecl *protocol) const;

  /// The ProtocolConformanceRef either stores a pack conformance, or
  /// it is invalid in the case of substitution failure.
  ProtocolConformanceRef subst(SubstitutionMap subMap,
                               SubstOptions options = std::nullopt) const;

  /// The ProtocolConformanceRef either stores a pack conformance, or
  /// it is invalid in the case of substitution failure.
  ProtocolConformanceRef subst(TypeSubstitutionFn subs,
                               LookupConformanceFn conformances,
                               SubstOptions options = std::nullopt) const;

  /// Apply an in-flight substitution to the conformances in this
  /// protocol conformance ref.
  ///
  /// This function should generally not be used outside of the
  /// substitution subsystem.
  ProtocolConformanceRef subst(InFlightSubstitution &IFS) const;

  NEXTCODE_DEBUG_DUMP;
  void dump(llvm::raw_ostream &out, unsigned indent = 0) const;
};

void simple_display(llvm::raw_ostream &out, PackConformance *conformance);

} // end namespace nextcode

#endif // NEXTCODE_AST_PACKCONFORMANCE_H
