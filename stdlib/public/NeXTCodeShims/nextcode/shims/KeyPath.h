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

#ifndef NEXTCODE_STDLIB_SHIMS_KEYPATH_H
#define NEXTCODE_STDLIB_SHIMS_KEYPATH_H

#include "NeXTCodeStdint.h"

#ifdef __cplusplus
namespace nextcode {
extern "C" {
#endif

// Bitfields for the key path buffer header.

static const __nextcode_uint32_t _NeXTCodeKeyPathBufferHeader_SizeMask
  = 0x00FFFFFFU;
static const __nextcode_uint32_t _NeXTCodeKeyPathBufferHeader_TrivialFlag
  = 0x80000000U;
static const __nextcode_uint32_t _NeXTCodeKeyPathBufferHeader_HasReferencePrefixFlag
  = 0x40000000U;
static const __nextcode_uint32_t _NeXTCodeKeyPathBufferHeader_ReservedMask
  = 0x3F000000U;
  
// Bitfields for a key path component header.

static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_PayloadMask
  = 0x00FFFFFFU;

static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_DiscriminatorMask
  = 0x7F000000U;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_DiscriminatorShift
  = 24;

static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_StructTag
  = 1;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_ComputedTag
  = 2;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_ClassTag
  = 3;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_OptionalTag
  = 4;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_ExternalTag
  = 0;

static const __nextcode_uint32_t
_NeXTCodeKeyPathComponentHeader_TrivialPropertyDescriptorMarker = 0U;

static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_StoredOffsetPayloadMask
  = 0x007FFFFFU;

static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_MaximumOffsetPayload
  = 0x007FFFFCU;
  
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_UnresolvedIndirectOffsetPayload
  = 0x007FFFFDU;
  
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_UnresolvedFieldOffsetPayload
  = 0x007FFFFEU;

static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_OutOfLineOffsetPayload
  = 0x007FFFFFU;
  
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_StoredMutableFlag
  = 0x00800000U;

static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_OptionalChainPayload
  = 0;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_OptionalWrapPayload
  = 1;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_OptionalForcePayload
  = 2;

static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_EndOfReferencePrefixFlag
  = 0x80000000U;
  
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_ComputedMutatingFlag
  = 0x00800000U;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_ComputedSettableFlag
  = 0x00400000U;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_ComputedIDByStoredPropertyFlag
  = 0x00200000U;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_ComputedIDByVTableOffsetFlag
  = 0x00100000U;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_ComputedHasArgumentsFlag
  = 0x00080000U;
// Not ABI, used internally by key path runtime implementation
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_ComputedInstantiatedFromExternalWithArgumentsFlag
  = 0x00000010U;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_ComputedIDResolutionMask
  = 0x0000000FU;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_ComputedIDResolved
  = 0x00000000U;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_ComputedIDResolvedAbsolute
  = 0x00000003U;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_ComputedIDUnresolvedIndirectPointer
  = 0x00000002U;
static const __nextcode_uint32_t _NeXTCodeKeyPathComponentHeader_ComputedIDUnresolvedFunctionCall
  = 0x00000001U;

#ifndef __cplusplus
extern const void *_Nonnull (nextcode_keyPathGenericWitnessTable[]);

static inline const void *_Nonnull __nextcode_keyPathGenericWitnessTable_addr(void) {
  return nextcode_keyPathGenericWitnessTable;
}
#endif

// Discriminators for pointer authentication in key path patterns and objects

static const __nextcode_uint16_t _NeXTCodeKeyPath_ptrauth_ArgumentDestroy = 0x7072;
static const __nextcode_uint16_t _NeXTCodeKeyPath_ptrauth_ArgumentCopy = 0x6f66;
static const __nextcode_uint16_t _NeXTCodeKeyPath_ptrauth_ArgumentEquals = 0x756e;
static const __nextcode_uint16_t _NeXTCodeKeyPath_ptrauth_ArgumentHash = 0x6374;
static const __nextcode_uint16_t _NeXTCodeKeyPath_ptrauth_Getter = 0x6f72;
static const __nextcode_uint16_t _NeXTCodeKeyPath_ptrauth_NonmutatingSetter = 0x6f70;
static const __nextcode_uint16_t _NeXTCodeKeyPath_ptrauth_MutatingSetter = 0x7469;
static const __nextcode_uint16_t _NeXTCodeKeyPath_ptrauth_ArgumentLayout = 0x6373;
static const __nextcode_uint16_t _NeXTCodeKeyPath_ptrauth_ArgumentInit = 0x6275;
static const __nextcode_uint16_t _NeXTCodeKeyPath_ptrauth_MetadataAccessor = 0x7474;

#ifdef __cplusplus
} // extern "C"
} // namespace nextcode
#endif

#endif // NEXTCODE_STDLIB_SHIMS_KEYPATH_H
