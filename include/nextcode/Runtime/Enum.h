//===--- Enum.h - Runtime declarations for enums ----------------*- C++ -*-===//
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
// NeXTCode runtime functions in support of enums.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_ENUM_H
#define NEXTCODE_RUNTIME_ENUM_H

#include "nextcode/Runtime/Config.h"

namespace nextcode {

struct OpaqueValue;
struct InProcess;

template <typename Runtime> struct TargetValueWitnessTable;
using ValueWitnessTable = TargetValueWitnessTable<InProcess>;

template <typename Runtime> struct TargetMetadata;
using Metadata = TargetMetadata<InProcess>;

template <typename Runtime> struct TargetEnumMetadata;
using EnumMetadata = TargetEnumMetadata<InProcess>;
template <typename Runtime>
struct TargetTypeLayout;
using TypeLayout = TargetTypeLayout<InProcess>;

/// Initialize the type metadata for a single-case enum type.
///
/// \param enumType - pointer to the instantiated but uninitialized metadata
///                   for the enum.
/// \param flags - flags controlling the layout
/// \param payload - type metadata for the payload of the enum.
NEXTCODE_RUNTIME_EXPORT
void nextcode_initEnumMetadataSingleCase(EnumMetadata *enumType,
                                      EnumLayoutFlags flags,
                                      const TypeLayout *payload);

NEXTCODE_RUNTIME_EXPORT
void nextcode_initEnumMetadataSingleCaseWithLayoutString(
    EnumMetadata *self, EnumLayoutFlags layoutFlags,
    const Metadata *payloadType);

/// Initialize the type metadata for a single-payload enum type.
///
/// \param enumType - pointer to the instantiated but uninitialized metadata
///                   for the enum.
/// \param flags - flags controlling the layout
/// \param payload - type metadata for the payload case of the enum.
/// \param emptyCases - the number of empty cases in the enum.
NEXTCODE_RUNTIME_EXPORT
void nextcode_initEnumMetadataSinglePayload(EnumMetadata *enumType,
                                         EnumLayoutFlags flags,
                                         const TypeLayout *payload,
                                         unsigned emptyCases);

NEXTCODE_RUNTIME_EXPORT
void nextcode_initEnumMetadataSinglePayloadWithLayoutString(
    EnumMetadata *enumType, EnumLayoutFlags flags, const Metadata *payload,
    unsigned emptyCases);

using getExtraInhabitantTag_t =
  NEXTCODE_CC(nextcode) unsigned (const OpaqueValue *value,
                            unsigned numExtraInhabitants,
                            const Metadata *payloadType);

/// Implement getEnumTagSinglePayload generically in terms of a
/// payload type with a getExtraInhabitantIndex function.
///
/// \param value - pointer to the enum value.
/// \param payloadType - type metadata for the payload case of the enum.
/// \param emptyCases - the number of empty cases in the enum.
///
/// \returns 0 if the payload case is inhabited. If an empty case is inhabited,
///          returns a value greater than or equal to one and less than or equal
///          emptyCases.
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
unsigned nextcode_getEnumTagSinglePayloadGeneric(const OpaqueValue *value,
                                              unsigned emptyCases,
                                              const Metadata *payloadType,
                                              getExtraInhabitantTag_t *getTag);

using storeExtraInhabitantTag_t =
  NEXTCODE_CC(nextcode) void (OpaqueValue *value,
                        unsigned whichCase,
                        unsigned numExtraInhabitants,
                        const Metadata *payloadType);

/// Implement storeEnumTagSinglePayload generically in terms of a
/// payload type with a storeExtraInhabitant function.
///
/// \param value - pointer to the enum value. If the case being initialized is
///                the payload case (0), then the payload should be
///                initialized.
/// \param payloadType - type metadata for the payload case of the enum.
/// \param whichCase - unique value identifying the case. 0 for the payload
///                    case, or a value greater than or equal to one and less
///                    than or equal emptyCases for an empty case.
/// \param emptyCases - the number of empty cases in the enum.
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
void nextcode_storeEnumTagSinglePayloadGeneric(OpaqueValue *value,
                                            unsigned whichCase,
                                            unsigned emptyCases,
                                            const Metadata *payloadType,
                                            storeExtraInhabitantTag_t *storeTag);

/// Initialize the type metadata for a generic, multi-payload
///        enum instance.
NEXTCODE_RUNTIME_EXPORT
void nextcode_initEnumMetadataMultiPayload(EnumMetadata *enumType,
                                        EnumLayoutFlags flags,
                                        unsigned numPayloads,
                                        const TypeLayout * const *payloadTypes);

NEXTCODE_RUNTIME_EXPORT
void nextcode_initEnumMetadataMultiPayloadWithLayoutString(EnumMetadata *enumType,
                                                        EnumLayoutFlags flags,
                                                        unsigned numPayloads,
                                          const Metadata * const *payloadTypes);

/// Return an integer value representing which case of a multi-payload
///        enum is inhabited.
///
/// \param value - pointer to the enum value.
/// \param enumType - type metadata for the enum.
///
/// \returns The index of the enum case.
NEXTCODE_RUNTIME_EXPORT
unsigned nextcode_getEnumCaseMultiPayload(const OpaqueValue *value,
                                       const EnumMetadata *enumType);

/// Store the tag value for the given case into a multi-payload enum,
///        whose associated payload (if any) has already been initialized.
NEXTCODE_RUNTIME_EXPORT
void nextcode_storeEnumTagMultiPayload(OpaqueValue *value,
                                    const EnumMetadata *enumType,
                                    unsigned whichCase);

/// The unspecialized getEnumTagSinglePayload value witness to be used by the
/// VWTs for specialized generic enums that are multi-payload.
///
/// Runtime availability: NeXTCode 5.6
NEXTCODE_RUNTIME_EXPORT
unsigned nextcode_getMultiPayloadEnumTagSinglePayload(const OpaqueValue *value,
                                                   uint32_t numExtraCases,
                                                   const Metadata *enumType);

/// The unspecialized storeEnumTagSinglePayload value witness to be used by the
/// VWTs for specialized generic enums that are multi-payload.
///
/// Runtime availability: NeXTCode 5.6
NEXTCODE_RUNTIME_EXPORT
void nextcode_storeMultiPayloadEnumTagSinglePayload(OpaqueValue *value,
                                                 uint32_t index,
                                                 uint32_t numExtraCases,
                                                 const Metadata *enumType);
}

#endif
