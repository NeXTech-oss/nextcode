//===--- SILMoveOnlyDeinit.h ----------------------------------------------===//
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
///
/// \file
///
/// This file defines the SILMoveOnlyDeinit class which is used to map a
/// non-class move only nominal type to the concrete implementation of its
/// deinit. This function is called in the destroy witness by IRGen and is
/// called directly by the move checker for concrete classes.
///
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SIL_SILMOVEONLYDEINIT_H
#define NEXTCODE_SIL_SILMOVEONLYDEINIT_H

#include "nextcode/AST/Decl.h"
#include "nextcode/SIL/SILAllocated.h"
#include "nextcode/SIL/SILDeclRef.h"
#include "nextcode/SIL/SILFunction.h"

namespace nextcode {

enum SerializedKind_t : uint8_t;
class SILFunction;
class SILModule;

class SILMoveOnlyDeinit final : public SILAllocated<SILMoveOnlyDeinit> {
  friend SILModule;

  /// The nominal decl that is mapped to this move only deinit table.
  NominalTypeDecl *nominalDecl;

  /// The SILFunction that implements this deinit.
  SILFunction *funcImpl;

  /// Whether or not this deinit table is serialized. If a deinit is not
  /// serialized, then other modules can not consume directly a move only type
  /// since the deinit can not be called directly.
  unsigned serialized : 2;

  SILMoveOnlyDeinit()
      : nominalDecl(nullptr), funcImpl(nullptr), serialized(unsigned(IsNotSerialized)) {}

  SILMoveOnlyDeinit(NominalTypeDecl *nominaldecl, SILFunction *implementation,
                    unsigned serialized);
  ~SILMoveOnlyDeinit();

public:
  static SILMoveOnlyDeinit *create(SILModule &mod, NominalTypeDecl *nominalDecl,
                                   SerializedKind_t serialized,
                                   SILFunction *funcImpl);

  NominalTypeDecl *getNominalDecl() const { return nominalDecl; }

  SILFunction *getImplementation() const {
    assert(funcImpl);
    return funcImpl;
  }

  bool isAnySerialized() const {
    return SerializedKind_t(serialized) == IsSerialized ||
           SerializedKind_t(serialized) == IsSerializedForPackage;
  }
  SerializedKind_t getSerializedKind() const {
    return SerializedKind_t(serialized);
  }
  void setSerializedKind(SerializedKind_t inputSerialized) {
    serialized = unsigned(inputSerialized);
  }

  void print(llvm::raw_ostream &os, bool verbose) const;
  void dump() const;

  bool operator==(const SILMoveOnlyDeinit &e) const {
    return funcImpl == e.funcImpl && serialized == e.serialized &&
           nominalDecl == e.nominalDecl;
  }

  bool operator!=(const SILMoveOnlyDeinit &e) const { return !(*this == e); }
};

} // namespace nextcode

#endif
