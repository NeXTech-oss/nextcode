//===---------------- BinaryScanImpl.h - NeXTCode Compiler ------------------===//
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
// Implementation details of the binary scanning C API
//
//===----------------------------------------------------------------------===//
#ifndef NEXTCODE_C_BINARY_SCAN_IMPL_H
#define NEXTCODE_C_BINARY_SCAN_IMPL_H

#include "nextcode-c/StaticMirror/BinaryScan.h"

namespace nextcode {
namespace static_mirror {
class BinaryScanningTool;
}
} // namespace nextcode

struct nextcode_static_mirror_conformance_info_s {
  nextcode_static_mirror_string_ref_t type_name;
  nextcode_static_mirror_string_ref_t mangled_type_name;
  nextcode_static_mirror_string_ref_t protocol_name;
};

struct nextcode_static_mirror_type_alias_s {
  nextcode_static_mirror_string_ref_t type_alias_name;
  nextcode_static_mirror_string_ref_t substituted_type_name;
  nextcode_static_mirror_string_ref_t substituted_type_mangled_name;
  nextcode_static_mirror_string_set_t *opaque_protocol_requirements_set;
  nextcode_static_mirror_type_alias_set_t *opaque_same_type_requirements_set;
};

struct nextcode_static_mirror_associated_type_info_s {
  nextcode_static_mirror_string_ref_t mangled_type_name;
  nextcode_static_mirror_type_alias_set_t *type_alias_set;
};

struct nextcode_static_mirror_enum_case_info_s {
  nextcode_static_mirror_string_ref_t label;
};

struct nextcode_static_mirror_property_info_s {
  nextcode_static_mirror_string_ref_t label;
  nextcode_static_mirror_string_ref_t type_name;
  nextcode_static_mirror_string_ref_t mangled_type_name;
};

struct nextcode_static_mirror_field_info_s {
  nextcode_static_mirror_string_ref_t mangled_type_name;
  nextcode_static_mirror_property_info_set_t *property_set;
  nextcode_static_mirror_enum_case_info_set_t *enum_case_set;
};

#endif // NEXTCODE_C_BINARY_SCAN_IMPL_H
