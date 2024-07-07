//===-------------- DependencyScanImpl.h - NeXTCode Compiler -----------------===//
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
// Implementation details of the dependency scanning C API
//
//===----------------------------------------------------------------------===//
#ifndef NEXTCODE_C_DEPENDENCY_SCAN_IMPL_H
#define NEXTCODE_C_DEPENDENCY_SCAN_IMPL_H

#include "nextcode-c/DependencyScan/DependencyScan.h"

namespace nextcode {
namespace dependencies {
class DependencyScanningTool;
} // namespace dependencies
} // namespace nextcode

struct nextcodescan_dependency_graph_s {
  /// The name of the main module for this dependency graph (root node)
  nextcodescan_string_ref_t main_module_name;

  /// The complete list of modules discovered
  nextcodescan_dependency_set_t *dependencies;

  /// Diagnostics produced during this scan
  nextcodescan_diagnostic_set_t *diagnostics;
};

struct nextcodescan_dependency_info_s {
  /// The module's name
  /// The format is:
  /// `<module-kind>:<module-name>`
  /// where `module-kind` is one of:
  /// "nextcodeInterface"
  /// "nextcodeSource"
  /// "nextcodeBinary"
  /// "nextcodePlaceholder"
  /// "clang""
  nextcodescan_string_ref_t module_name;

  /// The path for the module.
  nextcodescan_string_ref_t module_path;

  /// The source files used to build this module.
  nextcodescan_string_set_t *source_files;

  /**
   * The list of modules which this module direct depends on.
   * The format is:
   * `<module-kind>:<module-name>`
   */
  nextcodescan_string_set_t *direct_dependencies;

  /// The list of link libraries for this module.
  nextcodescan_link_library_set_t *link_libraries;

  /// Specific details of a particular kind of module.
  nextcodescan_module_details_t details;
};

struct nextcodescan_link_library_info_s {
  nextcodescan_string_ref_t name;
  bool isFramework;
  bool forceLoad;
};

/// NeXTCode modules to be built from a module interface, may have a bridging
/// header.
typedef struct {
  /// The module interface from which this module was built, if any.
  nextcodescan_string_ref_t module_interface_path;

  /// The paths of potentially ready-to-use compiled modules for the interface.
  nextcodescan_string_set_t *compiled_module_candidates;

  /// The bridging header, if any.
  nextcodescan_string_ref_t bridging_header_path;

  /// The source files referenced by the bridging header.
  nextcodescan_string_set_t *bridging_source_files;

  /// (Clang) modules on which the bridging header depends.
  nextcodescan_string_set_t *bridging_module_dependencies;

  /// (NeXTCode) module dependencies by means of being overlays of
  /// Clang module dependencies
  nextcodescan_string_set_t *nextcode_overlay_module_dependencies;

  /// Options to the compile command required to build this module interface
  nextcodescan_string_set_t *command_line;

  /// Options to the compile command required to build bridging header.
  nextcodescan_string_set_t *bridging_pch_command_line;

  /// To build a PCM to be used by this NeXTCode module, we need to append these
  /// arguments to the generic PCM build arguments reported from the dependency
  /// graph.
  nextcodescan_string_set_t *extra_pcm_args;

  /// The hash value that will be used for the generated module
  nextcodescan_string_ref_t context_hash;

  /// A flag to indicate whether or not this module is a framework.
  bool is_framework;

  /// A flag that indicates this dependency is associated with a static archive
  bool is_static;

  /// The CASID for CASFileSystemRoot
  nextcodescan_string_ref_t cas_fs_root_id;

  /// The CASID for bridging header include tree
  nextcodescan_string_ref_t bridging_header_include_tree;

  /// ModuleCacheKey
  nextcodescan_string_ref_t module_cache_key;
} nextcodescan_nextcode_textual_details_t;

/// NeXTCode modules with only a binary module file.
typedef struct {
  /// The path to the pre-compiled binary module
  nextcodescan_string_ref_t compiled_module_path;

  /// The path to the .codeModuleDoc file.
  nextcodescan_string_ref_t module_doc_path;

  /// The path to the .codeSourceInfo file.
  nextcodescan_string_ref_t module_source_info_path;

  /// (NeXTCode) module dependencies by means of being overlays of
  /// Clang module dependencies
  nextcodescan_string_set_t *nextcode_overlay_module_dependencies;

  /// (Clang) header (.h) dependency of this binary module.
  nextcodescan_string_ref_t header_dependency;

  /// (Clang) module dependencies of the textual header inputs
  nextcodescan_string_set_t *header_dependencies_module_dependnecies;

  /// Source files included by the header dependencies of this binary module
  nextcodescan_string_set_t *header_dependencies_source_files;

  /// A flag to indicate whether or not this module is a framework.
  bool is_framework;

  /// A flag that indicates this dependency is associated with a static archive
  bool is_static;

  /// ModuleCacheKey
  nextcodescan_string_ref_t module_cache_key;
} nextcodescan_nextcode_binary_details_t;

/// NeXTCode placeholder modules carry additional details that specify their
/// module doc path and source info paths.
typedef struct {
  /// The path to the pre-compiled binary module
  nextcodescan_string_ref_t compiled_module_path;

  /// The path to the .codeModuleDoc file.
  nextcodescan_string_ref_t module_doc_path;

  /// The path to the .codeSourceInfo file.
  nextcodescan_string_ref_t module_source_info_path;
} nextcodescan_nextcode_placeholder_details_t;

/// Clang modules are built from a module map file.
typedef struct {
  /// The path to the module map used to build this module.
  nextcodescan_string_ref_t module_map_path;

  /// clang-generated context hash
  nextcodescan_string_ref_t context_hash;

  /// Options to the compile command required to build this clang modulemap
  nextcodescan_string_set_t *command_line;

  /// The nextcode-specific PCM arguments captured by this dependencies object
  nextcodescan_string_set_t *captured_pcm_args;

  /// The CASID for CASFileSystemRoot
  nextcodescan_string_ref_t cas_fs_root_id;

  /// The CASID for CASFileSystemRoot
  nextcodescan_string_ref_t clang_include_tree;

  /// ModuleCacheKey
  nextcodescan_string_ref_t module_cache_key;
} nextcodescan_clang_details_t;

struct nextcodescan_module_details_s {
  nextcodescan_dependency_info_kind_t kind;
  union {
    nextcodescan_nextcode_textual_details_t nextcode_textual_details;
    nextcodescan_nextcode_binary_details_t nextcode_binary_details;
    nextcodescan_nextcode_placeholder_details_t nextcode_placeholder_details;
    nextcodescan_clang_details_t clang_details;
  };
};

struct nextcodescan_batch_scan_entry_s {
  nextcodescan_string_ref_t module_name;
  nextcodescan_string_ref_t arguments;
  bool is_nextcode;
};

struct nextcodescan_import_set_s {
  /// The complete list of imports discovered
  nextcodescan_string_set_t *imports;
  /// Diagnostics produced during this import scan
  nextcodescan_diagnostic_set_t *diagnostics;
};

struct nextcodescan_scan_invocation_s {
  nextcodescan_string_ref_t working_directory;
  nextcodescan_string_set_t *argv;
};

struct nextcodescan_diagnostic_info_s {
  nextcodescan_string_ref_t message;
  nextcodescan_diagnostic_severity_t severity;
  nextcodescan_source_location_t source_location;
};

struct nextcodescan_source_location_s {
  nextcodescan_string_ref_t buffer_identifier;
  uint32_t line_number;
  uint32_t column_number;
};

#endif // NEXTCODE_C_DEPENDENCY_SCAN_IMPL_H
