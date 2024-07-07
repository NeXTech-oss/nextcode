//===--- SILProperty.h - Defines the SILProperty class ----------*- C++ -*-===//
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
// This file defines the SILProperty class, which is used to capture the
// metadata about a property definition necessary for it to be resiliently
// included in KeyPaths across modules.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SIL_SILPROPERTY_H
#define NEXTCODE_SIL_SILPROPERTY_H

#include "nextcode/AST/GenericSignature.h"
#include "nextcode/SIL/SILAllocated.h"
#include "nextcode/SIL/SILInstruction.h"
#include "llvm/ADT/ilist_node.h"
#include "llvm/ADT/ilist.h"

namespace nextcode {

class SILPrintContext;
  
/// A descriptor for a public property or subscript that can be resiliently
/// referenced from key paths in external modules.
class SILProperty : public llvm::ilist_node<SILProperty>,
                    public SILAllocated<SILProperty>
{
private:
  /// True if serialized.
  unsigned Serialized : 2;
  
  /// The declaration the descriptor represents.
  AbstractStorageDecl *Decl;
  
  /// The key path component that represents its implementation.
  std::optional<KeyPathPatternComponent> Component;

  SILProperty(unsigned Serialized, AbstractStorageDecl *Decl,
              std::optional<KeyPathPatternComponent> Component)
      : Serialized(Serialized), Decl(Decl), Component(Component) {}

public:
  static SILProperty *create(SILModule &M, unsigned Serialized,
                             AbstractStorageDecl *Decl,
                             std::optional<KeyPathPatternComponent> Component);

  bool isAnySerialized() const {
    return SerializedKind_t(Serialized) == IsSerialized ||
           SerializedKind_t(Serialized) == IsSerializedForPackage;
  }
  SerializedKind_t getSerializedKind() const {
    return SerializedKind_t(Serialized);
  }

  AbstractStorageDecl *getDecl() const { return Decl; }
  
  bool isTrivial() const {
    return !Component.has_value();
  }

  const std::optional<KeyPathPatternComponent> &getComponent() const {
    return Component;
  }

  void print(SILPrintContext &Ctx) const;
  void dump() const;
  
  void verify(const SILModule &M) const;
};
  
} // end namespace nextcode

namespace llvm {

//===----------------------------------------------------------------------===//
// ilist_traits for SILProperty
//===----------------------------------------------------------------------===//

template <>
struct ilist_traits<::nextcode::SILProperty>
    : public ilist_node_traits<::nextcode::SILProperty> {
  using SILProperty = ::nextcode::SILProperty;

public:
  static void deleteNode(SILProperty *VT) { VT->~SILProperty(); }

private:
  void createNode(const SILProperty &);
};

} // namespace llvm

#endif
