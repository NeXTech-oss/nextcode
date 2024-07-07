//===--- BinaryScan.h - C API for NeXTCode Binary Scanning ---*- C -*-===//
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
// This C API is primarily intended to serve as a "static mirror" library
// for querying NeXTCode type information from binary object files.
//
//===----------------------------------------------------------------------===//

#include "StaticMirrorMacros.h"
#include "nextcode-c/CommonString/CommonString.h"

#ifndef NEXTCODE_C_BINARY_SCAN_H
#define NEXTCODE_C_BINARY_SCAN_H

/// The version constants for the NeXTCodeStaticMirror C API.
/// NEXTCODESTATICMIRROR_VERSION_MINOR should increase when there are API additions.
/// NEXTCODESTATICMIRROR_VERSION_MAJOR is intended for "major" source/ABI breaking changes.
#define NEXTCODESTATICMIRROR_VERSION_MAJOR 0
#define NEXTCODESTATICMIRROR_VERSION_MINOR 5 // Added opaque associated type's same-type requirements

NEXTCODESTATICMIRROR_BEGIN_DECLS

//=== Public Binary Scanner Data Types ------------------------------------===//

typedef nextcodescan_string_ref_t nextcode_static_mirror_string_ref_t;
typedef nextcodescan_string_set_t nextcode_static_mirror_string_set_t;

/// Container of the configuration state for binary static mirror scanning
/// instance
typedef void *nextcode_static_mirror_t;

/// Opaque container to a conformance type info of a given protocol conformance.
typedef struct nextcode_static_mirror_conformance_info_s
    *nextcode_static_mirror_conformance_info_t;

/// Opaque container to a field info (property type or enum case)
typedef struct nextcode_static_mirror_field_info_s
    *nextcode_static_mirror_field_info_t;

/// Opaque container to a property type info
typedef struct nextcode_static_mirror_property_info_s
    *nextcode_static_mirror_property_info_t;

/// Opaque container to an enum case info
typedef struct nextcode_static_mirror_enum_case_info_s
    *nextcode_static_mirror_enum_case_info_t;

/// Opaque container to details of an associated type specification.
typedef struct nextcode_static_mirror_type_alias_s
    *nextcode_static_mirror_type_alias_t;

/// Opaque container to an associated type (typealias) info of a given type.
typedef struct nextcode_static_mirror_associated_type_info_s
    *nextcode_static_mirror_associated_type_info_t;

typedef struct {
  nextcode_static_mirror_type_alias_t *type_aliases;
  size_t count;
} nextcode_static_mirror_type_alias_set_t;

typedef struct {
  nextcode_static_mirror_associated_type_info_t *associated_type_infos;
  size_t count;
} nextcode_static_mirror_associated_type_info_set_t;

typedef struct {
  nextcode_static_mirror_conformance_info_t *conformances;
  size_t count;
} nextcode_static_mirror_conformances_set_t;

typedef struct {
  nextcode_static_mirror_property_info_t *properties;
  size_t count;
} nextcode_static_mirror_property_info_set_t;

typedef struct {
  nextcode_static_mirror_enum_case_info_t *enum_cases;
  size_t count;
} nextcode_static_mirror_enum_case_info_set_t;

typedef struct {
  nextcode_static_mirror_field_info_t *field_infos;
  size_t count;
} nextcode_static_mirror_field_info_set_t;

// nextcode_static_mirror_conformance_info query methods
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_string_ref_t
    nextcode_static_mirror_conformance_info_get_type_name(
        nextcode_static_mirror_conformance_info_t);
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_string_ref_t
    nextcode_static_mirror_conformance_info_get_protocol_name(
        nextcode_static_mirror_conformance_info_t);
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_string_ref_t
    nextcode_static_mirror_conformance_info_get_mangled_type_name(
        nextcode_static_mirror_conformance_info_t);
NEXTCODESTATICMIRROR_PUBLIC void nextcode_static_mirror_conformance_info_dispose(
    nextcode_static_mirror_conformance_info_t);

// nextcode_static_mirror_associated_type_info query methods
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_string_ref_t
    nextcode_static_mirror_type_alias_get_type_alias_name(
        nextcode_static_mirror_type_alias_t);
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_string_ref_t
    nextcode_static_mirror_type_alias_get_substituted_type_name(
        nextcode_static_mirror_type_alias_t);
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_string_ref_t
    nextcode_static_mirror_type_alias_get_substituted_type_mangled_name(
        nextcode_static_mirror_type_alias_t);
NEXTCODESTATICMIRROR_PUBLIC nextcodescan_string_set_t *
nextcode_static_mirror_type_alias_get_opaque_type_protocol_requirements(
        nextcode_static_mirror_type_alias_t);
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_type_alias_set_t *
nextcode_static_mirror_type_alias_get_opaque_type_same_type_requirements(
        nextcode_static_mirror_type_alias_t);

// nextcode_static_mirror_associated_type_info query methods
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_string_ref_t
    nextcode_static_mirror_associated_type_info_get_mangled_type_name(
        nextcode_static_mirror_associated_type_info_t);
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_type_alias_set_t *
    nextcode_static_mirror_associated_type_info_get_type_alias_set(
        nextcode_static_mirror_associated_type_info_t);

// nextcode_static_mirror_field_info query methods
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_string_ref_t
    nextcode_static_mirror_field_info_get_mangled_type_name(
        nextcode_static_mirror_field_info_t);
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_property_info_set_t *
    nextcode_static_mirror_field_info_get_property_info_set(
        nextcode_static_mirror_field_info_t);
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_enum_case_info_set_t *
    nextcode_static_mirror_field_info_get_enum_case_info_set(
        nextcode_static_mirror_field_info_t);
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_string_ref_t
    nextcode_static_mirror_property_info_get_label(
        nextcode_static_mirror_property_info_t);
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_string_ref_t
    nextcode_static_mirror_property_info_get_type_name(
        nextcode_static_mirror_property_info_t);
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_string_ref_t
    nextcode_static_mirror_property_info_get_mangled_type_name(
        nextcode_static_mirror_property_info_t);
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_string_ref_t
    nextcode_static_mirror_enum_case_info_get_label(
        nextcode_static_mirror_enum_case_info_t);

/// Create an \c nextcode_static_mirror_t instance.
/// The returned \c nextcode_static_mirror_t is owned by the caller and must be
/// disposed of using \c nextcode_static_mirror_dispose .
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_t
nextcode_static_mirror_create(int, const char **, const char *);

NEXTCODESTATICMIRROR_PUBLIC void
    nextcode_static_mirror_dispose(nextcode_static_mirror_t);

/// Identify and collect all types conforming to any of the protocol names
/// specified as arguments
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_conformances_set_t *
nextcode_static_mirror_conformances_set_create(nextcode_static_mirror_t, int,
                                            const char **);

NEXTCODESTATICMIRROR_PUBLIC void nextcode_static_mirror_conformances_set_dispose(
    nextcode_static_mirror_conformances_set_t *);

/// Identify and collect all associated types of a given input type (by mangled
/// name)
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_associated_type_info_set_t *
nextcode_static_mirror_associated_type_info_set_create(nextcode_static_mirror_t,
                                                    const char *);

/// Identify and collect associated types of all discovered types.
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_associated_type_info_set_t *
    nextcode_static_mirror_all_associated_type_info_set_create(
        nextcode_static_mirror_t);

NEXTCODESTATICMIRROR_PUBLIC void
nextcode_static_mirror_associated_type_info_set_dispose(
    nextcode_static_mirror_associated_type_info_set_t *);

/// Identify and collect all field types of a given input type (by mangled name)
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_field_info_set_t *
nextcode_static_mirror_field_info_set_create(nextcode_static_mirror_t, const char *);

/// Identify and collect field types of all discovered types.
NEXTCODESTATICMIRROR_PUBLIC nextcode_static_mirror_field_info_set_t *
    nextcode_static_mirror_all_field_info_set_create(nextcode_static_mirror_t);

NEXTCODESTATICMIRROR_PUBLIC void nextcode_static_mirror_field_info_set_dispose(
    nextcode_static_mirror_field_info_set_t *);

NEXTCODESTATICMIRROR_END_DECLS

#endif // NEXTCODE_C_BINARY_SCAN_H
