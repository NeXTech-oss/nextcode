//===------------ DependencyScanImpl.cpp - NeXTCode Compiler -----------------===//
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
// Implementation of the dependency scanning C API
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/LLVMInitialize.h"
#include "nextcode/DependencyScan/StringUtils.h"
#include "nextcode/StaticMirror/BinaryScanImpl.h"
#include "nextcode/StaticMirror/BinaryScanningTool.h"

// FIXME: Code duplication with StringUtils.cpp
namespace nextcode {
namespace c_string_utils {

nextcodescan_string_ref_t create_null() {
  nextcodescan_string_ref_t str;
  str.data = nullptr;
  str.length = 0;
  return str;
}

nextcodescan_string_ref_t create_clone(const char *string) {
  if (!string)
    return create_null();

  if (string[0] == '\0')
    return create_null();

  nextcodescan_string_ref_t str;
  str.data = strdup(string);
  str.length = strlen(string);
  return str;
}

nextcodescan_string_set_t *create_set(const std::vector<std::string> &strings) {
  nextcodescan_string_set_t *set = new nextcodescan_string_set_t;
  set->count = strings.size();
  set->strings = new nextcodescan_string_ref_t[set->count];
  for (unsigned SI = 0, SE = set->count; SI < SE; ++SI)
    set->strings[SI] = create_clone(strings[SI].c_str());
  return set;
}
} // namespace c_string_utils
} // namespace nextcode

using namespace nextcode::static_mirror;

DEFINE_SIMPLE_CONVERSION_FUNCTIONS(BinaryScanningTool, nextcode_static_mirror_t)

//=== Private Cleanup Functions -------------------------------------------===//

/// Free the given string.
void nextcode_static_mirror_string_dispose(
    nextcode_static_mirror_string_ref_t string) {
  if (string.data)
    free(const_cast<void *>(string.data));
}

//=== Static Mirror Scan Functions ---------------------------------------===//
nextcode_static_mirror_t nextcode_static_mirror_create(int num_binaries,
                                                 const char **binary_paths,
                                                 const char *arch) {
  // INITIALIZE_LLVM();
  std::vector<std::string> inputBinaryPaths;
  for (unsigned SI = 0, SE = num_binaries; SI < SE; ++SI)
    inputBinaryPaths.push_back(binary_paths[SI]);

  return wrap(new BinaryScanningTool(inputBinaryPaths, arch));
}

void nextcode_static_mirror_dispose(nextcode_static_mirror_t c_static_mirror) {
  delete unwrap(c_static_mirror);
}

nextcode_static_mirror_conformances_set_t *
nextcode_static_mirror_conformances_set_create(nextcode_static_mirror_t static_mirror,
                                            int num_protocols,
                                            const char **protocol_names) {
  std::vector<std::string> protocols;
  for (unsigned SI = 0, SE = num_protocols; SI < SE; ++SI)
    protocols.push_back(protocol_names[SI]);
  BinaryScanningTool *scanTool = unwrap(static_mirror);
  auto scanResult = scanTool->collectConformances(protocols);

  // Bridge to the C interface
  nextcode_static_mirror_conformances_set_t *conformanceSet =
      new nextcode_static_mirror_conformances_set_t;
  conformanceSet->count = scanResult.Conformances.size();
  conformanceSet->conformances =
      new nextcode_static_mirror_conformance_info_t[conformanceSet->count];

  size_t idx = 0;
  for (auto &conformance : scanResult.Conformances) {
    nextcode_static_mirror_conformance_info_s *conformanceInfo =
        new nextcode_static_mirror_conformance_info_s;
    conformanceSet->conformances[idx] = conformanceInfo;
    conformanceInfo->type_name =
        nextcode::c_string_utils::create_clone(conformance.TypeName.c_str());
    conformanceInfo->mangled_type_name = nextcode::c_string_utils::create_clone(
        conformance.MangledTypeName.c_str());
    conformanceInfo->protocol_name =
        nextcode::c_string_utils::create_clone(conformance.ProtocolName.c_str());
    idx += 1;
  }
  return conformanceSet;
}

nextcode_static_mirror_string_ref_t
nextcode_static_mirror_conformance_info_get_type_name(
    nextcode_static_mirror_conformance_info_t info) {
  return info->type_name;
}

nextcode_static_mirror_string_ref_t
nextcode_static_mirror_conformance_info_get_protocol_name(
    nextcode_static_mirror_conformance_info_t info) {
  return info->protocol_name;
}

nextcode_static_mirror_string_ref_t
nextcode_static_mirror_conformance_info_get_mangled_type_name(
    nextcode_static_mirror_conformance_info_t info) {
  return info->mangled_type_name;
}

void nextcode_static_mirror_conformance_info_dispose(
    nextcode_static_mirror_conformance_info_t info) {
  nextcode_static_mirror_conformance_info_s *info_impl = info;
  nextcode_static_mirror_string_dispose(info_impl->type_name);
  nextcode_static_mirror_string_dispose(info_impl->mangled_type_name);
  nextcode_static_mirror_string_dispose(info_impl->protocol_name);
}

void nextcode_static_mirror_conformances_set_dispose(
    nextcode_static_mirror_conformances_set_t *set) {
  for (size_t i = 0; i < set->count; ++i) {
    nextcode_static_mirror_conformance_info_dispose(set->conformances[i]);
  }
  delete[] set->conformances;
  delete set;
}

static nextcode_static_mirror_associated_type_info_set_t *
convertAssociatedTypeQueryResult(
    const nextcode::reflection::AssociatedTypeCollectionResult &scanResult) {
  nextcode_static_mirror_associated_type_info_set_t *result =
      new nextcode_static_mirror_associated_type_info_set_t;
  result->count = scanResult.AssociatedTypeInfos.size();
  result->associated_type_infos = new nextcode_static_mirror_associated_type_info_t
      [scanResult.AssociatedTypeInfos.size()];

  int associatedTypeInfoIndex = 0;
  for (const auto &assocTypeInfo : scanResult.AssociatedTypeInfos) {
    nextcode_static_mirror_associated_type_info_s *info =
        new nextcode_static_mirror_associated_type_info_s;
    info->mangled_type_name = nextcode::c_string_utils::create_clone(
        assocTypeInfo.MangledTypeName.c_str());
    info->type_alias_set = new nextcode_static_mirror_type_alias_set_t;
    info->type_alias_set->count = assocTypeInfo.AssociatedTypes.size();
    info->type_alias_set->type_aliases =
        new nextcode_static_mirror_type_alias_t[assocTypeInfo.AssociatedTypes
                                                 .size()];
    int typealiasIndex = 0;
    for (const auto &typeAliasInfo : assocTypeInfo.AssociatedTypes) {
      nextcode_static_mirror_type_alias_s *typeAliasDetails =
          new nextcode_static_mirror_type_alias_s;
      typeAliasDetails->type_alias_name = nextcode::c_string_utils::create_clone(
          typeAliasInfo.SubstitutionInfo.TypeAliasName.c_str());
      typeAliasDetails->substituted_type_name =
          nextcode::c_string_utils::create_clone(
              typeAliasInfo.SubstitutionInfo.SubstitutedTypeFullyQualifiedName.c_str());
      typeAliasDetails->substituted_type_mangled_name =
          nextcode::c_string_utils::create_clone(
              typeAliasInfo.SubstitutionInfo.SubstitutedTypeMangledName.c_str());

      // Opaque type's protocol conformance requirements
      typeAliasDetails->opaque_protocol_requirements_set =
          nextcode::c_string_utils::create_set(
              typeAliasInfo.OpaqueTypeProtocolConformanceRequirements);

      // Opaque type's same-type requirements
      typeAliasDetails->opaque_same_type_requirements_set =
          new nextcode_static_mirror_type_alias_set_t;
      typeAliasDetails->opaque_same_type_requirements_set->count =
          typeAliasInfo.OpaqueTypeSameTypeRequirements.size();
      typeAliasDetails->opaque_same_type_requirements_set->type_aliases =
          new nextcode_static_mirror_type_alias_t
              [typeAliasInfo.OpaqueTypeSameTypeRequirements.size()];
      int sameTypeReqIndex = 0;
      for (const auto &sameTypeReq :
           typeAliasInfo.OpaqueTypeSameTypeRequirements) {
        nextcode_static_mirror_type_alias_s *sameTypeReqDetails =
            new nextcode_static_mirror_type_alias_s;
        sameTypeReqDetails->type_alias_name =
            nextcode::c_string_utils::create_clone(
                sameTypeReq.TypeAliasName.c_str());
        sameTypeReqDetails->substituted_type_mangled_name =
            nextcode::c_string_utils::create_clone(
                sameTypeReq.SubstitutedTypeMangledName.c_str());
        sameTypeReqDetails->substituted_type_name =
            nextcode::c_string_utils::create_clone(
                sameTypeReq.SubstitutedTypeFullyQualifiedName.c_str());
        typeAliasDetails->opaque_same_type_requirements_set
            ->type_aliases[sameTypeReqIndex] = sameTypeReqDetails;
        sameTypeReqIndex += 1;
      }

      info->type_alias_set->type_aliases[typealiasIndex] = typeAliasDetails;
      typealiasIndex += 1;
    }
    result->associated_type_infos[associatedTypeInfoIndex] = info;
    associatedTypeInfoIndex += 1;
  }
  return result;
}

static nextcode_static_mirror_field_info_set_t *convertFieldTypeQueryResult(
    const nextcode::reflection::FieldTypeCollectionResult &scanResult) {
  nextcode_static_mirror_field_info_set_t *result =
      new nextcode_static_mirror_field_info_set_t;
  result->count = scanResult.FieldInfos.size();
  result->field_infos =
      new nextcode_static_mirror_field_info_t[scanResult.FieldInfos.size()];

  int fieldInfoIndex = 0;
  for (const auto &fieldInfo : scanResult.FieldInfos) {
    nextcode_static_mirror_field_info_s *info =
        new nextcode_static_mirror_field_info_s;
    info->mangled_type_name =
        nextcode::c_string_utils::create_clone(fieldInfo.MangledTypeName.c_str());
    info->property_set = new nextcode_static_mirror_property_info_set_t;
    info->property_set->count = fieldInfo.Properties.size();
    info->property_set->properties =
        new nextcode_static_mirror_property_info_t[fieldInfo.Properties.size()];
    info->enum_case_set = new nextcode_static_mirror_enum_case_info_set_t;
    info->enum_case_set->count = fieldInfo.EnumCases.size();
    info->enum_case_set->enum_cases =
        new nextcode_static_mirror_enum_case_info_t[fieldInfo.EnumCases.size()];

    int propertyIndex = 0;
    for (const auto &propertyInfo : fieldInfo.Properties) {
      nextcode_static_mirror_property_info_s *propertyDetails =
          new nextcode_static_mirror_property_info_s;
      propertyDetails->label =
          nextcode::c_string_utils::create_clone(propertyInfo.Label.c_str());
      propertyDetails->type_name = nextcode::c_string_utils::create_clone(
          propertyInfo.TypeFullyQualifiedName.c_str());
      propertyDetails->mangled_type_name = nextcode::c_string_utils::create_clone(
          propertyInfo.TypeMangledName.c_str());
      info->property_set->properties[propertyIndex] = propertyDetails;
      propertyIndex += 1;
    }

    int enumCaseIndex = 0;
    for (const auto &enumCaseInfo : fieldInfo.EnumCases) {
      nextcode_static_mirror_enum_case_info_s *enumCaseDetails =
          new nextcode_static_mirror_enum_case_info_s;
      enumCaseDetails->label =
          nextcode::c_string_utils::create_clone(enumCaseInfo.Label.c_str());
      info->enum_case_set->enum_cases[enumCaseIndex] = enumCaseDetails;
      enumCaseIndex += 1;
    }

    result->field_infos[fieldInfoIndex] = info;
    fieldInfoIndex += 1;
  }
  return result;
}

nextcode_static_mirror_associated_type_info_set_t *
nextcode_static_mirror_associated_type_info_set_create(
    nextcode_static_mirror_t static_mirror, const char *forTypeName) {
  BinaryScanningTool *scanTool = unwrap(static_mirror);
  auto scanResult = scanTool->collectAssociatedTypes(forTypeName);
  return convertAssociatedTypeQueryResult(scanResult);
}

/// Identify and collect associated types of all discovered types.
nextcode_static_mirror_associated_type_info_set_t *
nextcode_static_mirror_all_associated_type_info_set_create(
    nextcode_static_mirror_t static_mirror) {
  BinaryScanningTool *scanTool = unwrap(static_mirror);
  auto scanResult = scanTool->collectAllAssociatedTypes();
  return convertAssociatedTypeQueryResult(scanResult);
}

// nextcode_static_mirror_associated_type query methods
nextcode_static_mirror_string_ref_t
nextcode_static_mirror_type_alias_get_type_alias_name(
    nextcode_static_mirror_type_alias_t type_alias) {
  return type_alias->type_alias_name;
}
nextcode_static_mirror_string_ref_t
nextcode_static_mirror_type_alias_get_substituted_type_name(
    nextcode_static_mirror_type_alias_t type_alias) {
  return type_alias->substituted_type_name;
}
nextcode_static_mirror_string_ref_t
nextcode_static_mirror_type_alias_get_substituted_type_mangled_name(
    nextcode_static_mirror_type_alias_t type_alias) {
  return type_alias->substituted_type_mangled_name;
}
nextcodescan_string_set_t *
nextcode_static_mirror_type_alias_get_opaque_type_protocol_requirements(
    nextcode_static_mirror_type_alias_t type_alias) {
  return type_alias->opaque_protocol_requirements_set;
}
nextcode_static_mirror_type_alias_set_t *
nextcode_static_mirror_type_alias_get_opaque_type_same_type_requirements(
    nextcode_static_mirror_type_alias_t type_alias) {
  return type_alias->opaque_same_type_requirements_set;
}

// nextcode_static_mirror_associated_type_info query methods
nextcode_static_mirror_string_ref_t
nextcode_static_mirror_associated_type_info_get_mangled_type_name(
    nextcode_static_mirror_associated_type_info_t associated_type_info) {
  return associated_type_info->mangled_type_name;
}
nextcode_static_mirror_type_alias_set_t *
nextcode_static_mirror_associated_type_info_get_type_alias_set(
    nextcode_static_mirror_associated_type_info_t associated_type_info) {
  return associated_type_info->type_alias_set;
}

void nextcode_static_mirror_type_alias_dispose(
    nextcode_static_mirror_type_alias_t type_alias) {
  nextcode_static_mirror_string_dispose(type_alias->substituted_type_mangled_name);
  nextcode_static_mirror_string_dispose(type_alias->substituted_type_name);
  nextcode_static_mirror_string_dispose(type_alias->type_alias_name);
  delete type_alias;
}

void nextcode_static_mirror_type_alias_set_dispose(
    nextcode_static_mirror_type_alias_set_t *type_alias_set) {
  for (size_t i = 0; i < type_alias_set->count; ++i) {
    nextcode_static_mirror_type_alias_dispose(type_alias_set->type_aliases[i]);
  }
  delete[] type_alias_set->type_aliases;
  delete type_alias_set;
}

void nextcode_static_mirror_associated_type_info_dispose(
    nextcode_static_mirror_associated_type_info_t associated_type_info) {
  nextcode_static_mirror_string_dispose(associated_type_info->mangled_type_name);
  nextcode_static_mirror_type_alias_set_dispose(
      associated_type_info->type_alias_set);
}

void nextcode_static_mirror_associated_type_info_set_dispose(
    nextcode_static_mirror_associated_type_info_set_t *associated_type_info_set) {
  for (size_t i = 0; i < associated_type_info_set->count; ++i) {
    nextcode_static_mirror_associated_type_info_dispose(
        associated_type_info_set->associated_type_infos[i]);
  }
  delete[] associated_type_info_set->associated_type_infos;
  delete associated_type_info_set;
}

nextcode_static_mirror_string_ref_t
nextcode_static_mirror_field_info_get_mangled_type_name(
    nextcode_static_mirror_field_info_t field_info) {
  return field_info->mangled_type_name;
}

nextcode_static_mirror_property_info_set_t *
nextcode_static_mirror_field_info_get_property_info_set(
    nextcode_static_mirror_field_info_t field_info) {
  return field_info->property_set;
}

nextcode_static_mirror_enum_case_info_set_t *
nextcode_static_mirror_field_info_get_enum_case_info_set(
    nextcode_static_mirror_field_info_t field_info) {
  return field_info->enum_case_set;
}

nextcode_static_mirror_string_ref_t nextcode_static_mirror_property_info_get_label(
    nextcode_static_mirror_property_info_t property_info) {
  return property_info->label;
}

nextcode_static_mirror_string_ref_t
nextcode_static_mirror_property_info_get_type_name(
    nextcode_static_mirror_property_info_t property_info) {
  return property_info->type_name;
}

nextcode_static_mirror_string_ref_t
nextcode_static_mirror_property_info_get_mangled_type_name(
    nextcode_static_mirror_property_info_t property_info) {
  return property_info->mangled_type_name;
}

nextcode_static_mirror_string_ref_t nextcode_static_mirror_enum_case_info_get_label(
    nextcode_static_mirror_enum_case_info_t enum_case_info) {
  return enum_case_info->label;
}

nextcode_static_mirror_field_info_set_t *
nextcode_static_mirror_field_info_set_create(nextcode_static_mirror_t static_mirror,
                                          const char *mangled_name) {
  BinaryScanningTool *scanTool = unwrap(static_mirror);
  auto scanResult = scanTool->collectFieldTypes(mangled_name);
  return convertFieldTypeQueryResult(scanResult);
}

nextcode_static_mirror_field_info_set_t *
nextcode_static_mirror_all_field_info_set_create(
    nextcode_static_mirror_t static_mirror) {
  BinaryScanningTool *scanTool = unwrap(static_mirror);
  auto scanResult = scanTool->collectAllFieldTypes();
  return convertFieldTypeQueryResult(scanResult);
}

void nextcode_static_mirror_property_info_dispose(
    nextcode_static_mirror_property_info_t property_info) {
  nextcode_static_mirror_string_dispose(property_info->mangled_type_name);
  nextcode_static_mirror_string_dispose(property_info->type_name);
  nextcode_static_mirror_string_dispose(property_info->label);
  delete property_info;
}

void nextcode_static_mirror_property_info_set_dispose(
    nextcode_static_mirror_property_info_set_t *property_set) {
  for (size_t i = 0; i < property_set->count; ++i) {
    nextcode_static_mirror_property_info_dispose(property_set->properties[i]);
  }
  delete[] property_set->properties;
  delete property_set;
}

void nextcode_static_mirror_enum_case_info_dispose(
    nextcode_static_mirror_enum_case_info_t enum_case_info) {
  nextcode_static_mirror_string_dispose(enum_case_info->label);
  delete enum_case_info;
}

void nextcode_static_mirror_enum_case_info_set_dispose(
    nextcode_static_mirror_enum_case_info_set_t *enum_case_set) {
  for (size_t i = 0; i < enum_case_set->count; ++i) {
    nextcode_static_mirror_enum_case_info_dispose(enum_case_set->enum_cases[i]);
  }
  delete[] enum_case_set->enum_cases;
  delete enum_case_set;
}

void nextcode_static_mirror_field_info_dispose(
    nextcode_static_mirror_field_info_t field_info) {
  nextcode_static_mirror_string_dispose(field_info->mangled_type_name);
  nextcode_static_mirror_property_info_set_dispose(field_info->property_set);
  nextcode_static_mirror_enum_case_info_set_dispose(field_info->enum_case_set);
}

void nextcode_static_mirror_field_info_set_dispose(
    nextcode_static_mirror_field_info_set_t *field_info_set) {
  for (size_t i = 0; i < field_info_set->count; ++i) {
    nextcode_static_mirror_field_info_dispose(field_info_set->field_infos[i]);
  }
  delete[] field_info_set->field_infos;
  delete field_info_set;
}
