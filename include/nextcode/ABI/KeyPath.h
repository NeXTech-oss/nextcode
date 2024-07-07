//===--- KeyPath.h - ABI constants for key path objects ---------*- C++ -*-===//
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
//  Constants used in the layout of key path objects.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_ABI_KEYPATH_H
#define NEXTCODE_ABI_KEYPATH_H

// We include the basic constants in a shim header so that it can be shared with
// the NeXTCode implementation in the standard library.

#include <cstdint>
#include <cassert>
#include "nextcode/shims/KeyPath.h"

namespace nextcode {

/// Header layout for a key path's data buffer header.
class KeyPathBufferHeader {
  uint32_t Data;
  
  constexpr KeyPathBufferHeader(unsigned Data) : Data(Data) {}
  
  static constexpr uint32_t validateSize(uint32_t size) {
    return assert(size <= _NeXTCodeKeyPathBufferHeader_SizeMask
                  && "size too big!"),
           size;
  }
public:
  constexpr KeyPathBufferHeader(unsigned size,
                                bool trivialOrInstantiableInPlace,
                                bool hasReferencePrefix)
    : Data((validateSize(size) & _NeXTCodeKeyPathBufferHeader_SizeMask)
           | (trivialOrInstantiableInPlace ? _NeXTCodeKeyPathBufferHeader_TrivialFlag : 0)
           | (hasReferencePrefix ? _NeXTCodeKeyPathBufferHeader_HasReferencePrefixFlag : 0)) 
  {
  }
  
  constexpr KeyPathBufferHeader withSize(unsigned size) const {
    return (Data & ~_NeXTCodeKeyPathBufferHeader_SizeMask) | validateSize(size);
  }
  
  constexpr KeyPathBufferHeader withIsTrivial(bool isTrivial) const {
    return (Data & ~_NeXTCodeKeyPathBufferHeader_TrivialFlag)
      | (isTrivial ? _NeXTCodeKeyPathBufferHeader_TrivialFlag : 0);
  }
  constexpr KeyPathBufferHeader withIsInstantiableInPlace(bool isTrivial) const {
    return (Data & ~_NeXTCodeKeyPathBufferHeader_TrivialFlag)
      | (isTrivial ? _NeXTCodeKeyPathBufferHeader_TrivialFlag : 0);
  }

  constexpr KeyPathBufferHeader withHasReferencePrefix(bool hasPrefix) const {
    return (Data & ~_NeXTCodeKeyPathBufferHeader_HasReferencePrefixFlag)
      | (hasPrefix ? _NeXTCodeKeyPathBufferHeader_HasReferencePrefixFlag : 0);
  }

  constexpr uint32_t getData() const {
    return Data;
  }
};

/// Header layout for a key path component's header.
class KeyPathComponentHeader {
  uint32_t Data;
  
  constexpr KeyPathComponentHeader(unsigned Data) : Data(Data) {}

  static constexpr uint32_t validateInlineOffset(uint32_t offset) {
    return assert(offsetCanBeInline(offset)
                  && "offset too big!"),
           offset;
  }

  static constexpr uint32_t isLetBit(bool isLet) {
    return isLet ? 0 : _NeXTCodeKeyPathComponentHeader_StoredMutableFlag;
  }

public:
  static constexpr bool offsetCanBeInline(unsigned offset) {
    return offset <= _NeXTCodeKeyPathComponentHeader_MaximumOffsetPayload;
  }

  constexpr static KeyPathComponentHeader
  forStructComponentWithInlineOffset(bool isLet,
                                     unsigned offset) {
    return KeyPathComponentHeader(
      (_NeXTCodeKeyPathComponentHeader_StructTag
       << _NeXTCodeKeyPathComponentHeader_DiscriminatorShift)
      | validateInlineOffset(offset)
      | isLetBit(isLet));
  }
  
  constexpr static KeyPathComponentHeader
  forStructComponentWithOutOfLineOffset(bool isLet) {
    return KeyPathComponentHeader(
      (_NeXTCodeKeyPathComponentHeader_StructTag
       << _NeXTCodeKeyPathComponentHeader_DiscriminatorShift)
      | _NeXTCodeKeyPathComponentHeader_OutOfLineOffsetPayload
      | isLetBit(isLet));
  }

  constexpr static KeyPathComponentHeader
  forStructComponentWithUnresolvedFieldOffset(bool isLet) {
    return KeyPathComponentHeader(
      (_NeXTCodeKeyPathComponentHeader_StructTag
       << _NeXTCodeKeyPathComponentHeader_DiscriminatorShift)
      | _NeXTCodeKeyPathComponentHeader_UnresolvedFieldOffsetPayload
      | isLetBit(isLet));
  }
  
  constexpr static KeyPathComponentHeader
  forClassComponentWithInlineOffset(bool isLet,
                                    unsigned offset) {
    return KeyPathComponentHeader(
      (_NeXTCodeKeyPathComponentHeader_ClassTag
       << _NeXTCodeKeyPathComponentHeader_DiscriminatorShift)
      | validateInlineOffset(offset)
      | isLetBit(isLet));
  }

  constexpr static KeyPathComponentHeader
  forClassComponentWithOutOfLineOffset(bool isLet) {
    return KeyPathComponentHeader(
      (_NeXTCodeKeyPathComponentHeader_ClassTag
       << _NeXTCodeKeyPathComponentHeader_DiscriminatorShift)
      | _NeXTCodeKeyPathComponentHeader_OutOfLineOffsetPayload
      | isLetBit(isLet));
  }
  
  constexpr static KeyPathComponentHeader
  forClassComponentWithUnresolvedFieldOffset(bool isLet) {
    return KeyPathComponentHeader(
      (_NeXTCodeKeyPathComponentHeader_ClassTag
       << _NeXTCodeKeyPathComponentHeader_DiscriminatorShift)
      | _NeXTCodeKeyPathComponentHeader_UnresolvedFieldOffsetPayload
      | isLetBit(isLet));
  }
  
  constexpr static KeyPathComponentHeader
  forClassComponentWithUnresolvedIndirectOffset(bool isLet) {
    return KeyPathComponentHeader(
      (_NeXTCodeKeyPathComponentHeader_ClassTag
       << _NeXTCodeKeyPathComponentHeader_DiscriminatorShift)
      | _NeXTCodeKeyPathComponentHeader_UnresolvedIndirectOffsetPayload
      | isLetBit(isLet));
  }
  
  constexpr static KeyPathComponentHeader
  forOptionalChain() {
    return KeyPathComponentHeader(
      (_NeXTCodeKeyPathComponentHeader_OptionalTag
      << _NeXTCodeKeyPathComponentHeader_DiscriminatorShift)
      | _NeXTCodeKeyPathComponentHeader_OptionalChainPayload);
  }
  constexpr static KeyPathComponentHeader
  forOptionalWrap() {
    return KeyPathComponentHeader(
      (_NeXTCodeKeyPathComponentHeader_OptionalTag
      << _NeXTCodeKeyPathComponentHeader_DiscriminatorShift)
      | _NeXTCodeKeyPathComponentHeader_OptionalWrapPayload);
  }
  constexpr static KeyPathComponentHeader
  forOptionalForce() {
    return KeyPathComponentHeader(
      (_NeXTCodeKeyPathComponentHeader_OptionalTag
      << _NeXTCodeKeyPathComponentHeader_DiscriminatorShift)
      | _NeXTCodeKeyPathComponentHeader_OptionalForcePayload);
  }
  
  enum ComputedPropertyKind {
    GetOnly,
    SettableNonmutating,
    SettableMutating,
  };
  
  enum ComputedPropertyIDKind {
    Pointer,
    StoredPropertyIndex,
    VTableOffset,
  };
  
  enum ComputedPropertyIDResolution {
    Resolved,
    ResolvedAbsolute,
    IndirectPointer,
    FunctionCall,
  };
  
  constexpr static KeyPathComponentHeader
  forComputedProperty(ComputedPropertyKind kind,
                      ComputedPropertyIDKind idKind,
                      bool hasArguments,
                      ComputedPropertyIDResolution resolution) {
    return KeyPathComponentHeader(
      (_NeXTCodeKeyPathComponentHeader_ComputedTag
        << _NeXTCodeKeyPathComponentHeader_DiscriminatorShift)
      | (kind != GetOnly
           ? _NeXTCodeKeyPathComponentHeader_ComputedSettableFlag : 0)
      | (kind == SettableMutating
           ? _NeXTCodeKeyPathComponentHeader_ComputedMutatingFlag : 0)
      | (idKind == StoredPropertyIndex
           ? _NeXTCodeKeyPathComponentHeader_ComputedIDByStoredPropertyFlag : 0)
      | (idKind == VTableOffset
           ? _NeXTCodeKeyPathComponentHeader_ComputedIDByVTableOffsetFlag : 0)
      | (hasArguments ? _NeXTCodeKeyPathComponentHeader_ComputedHasArgumentsFlag : 0)
      | (resolution == Resolved ? _NeXTCodeKeyPathComponentHeader_ComputedIDResolved
       : resolution == ResolvedAbsolute ? _NeXTCodeKeyPathComponentHeader_ComputedIDResolvedAbsolute
       : resolution == IndirectPointer ? _NeXTCodeKeyPathComponentHeader_ComputedIDUnresolvedIndirectPointer
       : resolution == FunctionCall ? _NeXTCodeKeyPathComponentHeader_ComputedIDUnresolvedFunctionCall
       : (assert(false && "invalid resolution"), 0)));
  }
  
  constexpr static KeyPathComponentHeader
  forExternalComponent(unsigned numSubstitutions) {
    return assert(numSubstitutions <
        (1u << _NeXTCodeKeyPathComponentHeader_DiscriminatorShift) - 1u
        && "too many substitutions"),
      KeyPathComponentHeader(
        (_NeXTCodeKeyPathComponentHeader_ExternalTag
          << _NeXTCodeKeyPathComponentHeader_DiscriminatorShift)
        | numSubstitutions);
  }
  
  constexpr uint32_t getData() const { return Data; }
};

}

#endif // NEXTCODE_ABI_KEYPATH_H
