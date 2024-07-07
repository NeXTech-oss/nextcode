//===--- DependencyScan.h - C API for NeXTCode Dependency Scanning ---*- C -*-===//
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
// This C API is primarily intended to serve as the NeXTCode Driver's
// dependency scanning facility (https://github.com/apple/nextcode-driver).
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_C_DEPENDENCY_SCAN_H
#define NEXTCODE_C_DEPENDENCY_SCAN_H

#include "DependencyScanMacros.h"
#include "nextcode-c/CommonString/CommonString.h"

/// The version constants for the NeXTCodeDependencyScan C API.
/// NEXTCODESCAN_VERSION_MINOR should increase when there are API additions.
/// NEXTCODESCAN_VERSION_MAJOR is intended for "major" source/ABI breaking changes.
#define NEXTCODESCAN_VERSION_MAJOR 0
#define NEXTCODESCAN_VERSION_MINOR 10

NEXTCODESCAN_BEGIN_DECLS

//=== Public Scanner Data Types -------------------------------------------===//

typedef enum {
  // This dependency info encodes two ModuleDependencyKind types:
  // NeXTCodeInterface and NeXTCodeSource.
  NEXTCODESCAN_DEPENDENCY_INFO_NEXTCODE_TEXTUAL = 0,
  NEXTCODESCAN_DEPENDENCY_INFO_NEXTCODE_BINARY = 1,
  NEXTCODESCAN_DEPENDENCY_INFO_NEXTCODE_PLACEHOLDER = 2,
  NEXTCODESCAN_DEPENDENCY_INFO_CLANG = 3
} nextcodescan_dependency_info_kind_t;

/// Opaque container of the details specific to a given module dependency.
typedef struct nextcodescan_module_details_s *nextcodescan_module_details_t;

/// Opaque container to a dependency info of a given module.
typedef struct nextcodescan_dependency_info_s *nextcodescan_dependency_info_t;

/// Opaque container to a link library info.
typedef struct nextcodescan_link_library_info_s *nextcodescan_link_library_info_t;

/// Opaque container to an overall result of a dependency scan.
typedef struct nextcodescan_dependency_graph_s *nextcodescan_dependency_graph_t;

/// Opaque container to contain the result of a dependency prescan.
typedef struct nextcodescan_import_set_s *nextcodescan_import_set_t;

/// Opaque container to contain the info of a diagnostics emitted by the scanner.
typedef struct nextcodescan_diagnostic_info_s *nextcodescan_diagnostic_info_t;

/// Opaque container to contain the info of a source location.
typedef struct nextcodescan_source_location_s *nextcodescan_source_location_t;

/// Full Dependency Graph (Result)
typedef struct {
  nextcodescan_dependency_info_t *modules;
  size_t count;
} nextcodescan_dependency_set_t;

/// Set of linked libraries
typedef struct {
  nextcodescan_link_library_info_t *link_libraries;
  size_t count;
} nextcodescan_link_library_set_t;

typedef enum {
  NEXTCODESCAN_DIAGNOSTIC_SEVERITY_ERROR = 0,
  NEXTCODESCAN_DIAGNOSTIC_SEVERITY_WARNING = 1,
  NEXTCODESCAN_DIAGNOSTIC_SEVERITY_NOTE = 2,
  NEXTCODESCAN_DIAGNOSTIC_SEVERITY_REMARK = 3
} nextcodescan_diagnostic_severity_t;

typedef struct {
  nextcodescan_diagnostic_info_t *diagnostics;
  size_t count;
} nextcodescan_diagnostic_set_t;

//=== Batch Scan Input Specification --------------------------------------===//

/// Opaque container to a container of batch scan entry information.
typedef struct nextcodescan_batch_scan_entry_s *nextcodescan_batch_scan_entry_t;

typedef struct {
  nextcodescan_batch_scan_entry_t *modules;
  size_t count;
} nextcodescan_batch_scan_input_t;

typedef struct {
  nextcodescan_dependency_graph_t *results;
  size_t count;
} nextcodescan_batch_scan_result_t;

//=== Scanner Invocation Specification ------------------------------------===//

/// Opaque container of all relevant context required to launch a dependency
/// scan (command line arguments, working directory, etc.)
typedef struct nextcodescan_scan_invocation_s *nextcodescan_scan_invocation_t;

//=== Dependency Result Functions -----------------------------------------===//

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_dependency_graph_get_main_module_name(
    nextcodescan_dependency_graph_t result);

NEXTCODESCAN_PUBLIC nextcodescan_dependency_set_t *
nextcodescan_dependency_graph_get_dependencies(
    nextcodescan_dependency_graph_t result);

// Return value disposed of together with the dependency_graph
// using `nextcodescan_dependency_graph_dispose`
NEXTCODESCAN_PUBLIC nextcodescan_diagnostic_set_t *
nextcodescan_dependency_graph_get_diagnostics(
    nextcodescan_dependency_graph_t result);

//=== Dependency Module Info Functions ------------------------------------===//

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_module_info_get_module_name(nextcodescan_dependency_info_t info);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_module_info_get_module_path(nextcodescan_dependency_info_t info);

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_module_info_get_source_files(nextcodescan_dependency_info_t info);

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_module_info_get_direct_dependencies(nextcodescan_dependency_info_t info);

NEXTCODESCAN_PUBLIC nextcodescan_link_library_set_t *
nextcodescan_module_info_get_link_libraries(nextcodescan_dependency_info_t info);

NEXTCODESCAN_PUBLIC nextcodescan_module_details_t
nextcodescan_module_info_get_details(nextcodescan_dependency_info_t info);

//=== Link Library Info Functions ------------------------------------===//
NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_link_library_info_get_link_name(
    nextcodescan_link_library_info_t info);
NEXTCODESCAN_PUBLIC bool nextcodescan_link_library_info_get_is_framework(
    nextcodescan_link_library_info_t info);
NEXTCODESCAN_PUBLIC bool nextcodescan_link_library_info_get_should_force_load(
    nextcodescan_link_library_info_t info);

//=== Dependency Module Info Details Functions ----------------------------===//

NEXTCODESCAN_PUBLIC nextcodescan_dependency_info_kind_t
nextcodescan_module_detail_get_kind(nextcodescan_module_details_t details);

//=== NeXTCode Textual Module Details query APIs -----------------------------===//
NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_nextcode_textual_detail_get_module_interface_path(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_nextcode_textual_detail_get_compiled_module_candidates(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_nextcode_textual_detail_get_bridging_header_path(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_nextcode_textual_detail_get_bridging_source_files(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_nextcode_textual_detail_get_bridging_module_dependencies(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_nextcode_textual_detail_get_command_line(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_nextcode_textual_detail_get_bridging_pch_command_line(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_nextcode_textual_detail_get_extra_pcm_args(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_nextcode_textual_detail_get_context_hash(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC bool nextcodescan_nextcode_textual_detail_get_is_framework(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_nextcode_textual_detail_get_nextcode_overlay_dependencies(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_nextcode_textual_detail_get_cas_fs_root_id(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_nextcode_textual_detail_get_module_cache_key(
    nextcodescan_module_details_t details);

//=== NeXTCode Binary Module Details query APIs ------------------------------===//

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_nextcode_binary_detail_get_compiled_module_path(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_nextcode_binary_detail_get_module_doc_path(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_nextcode_binary_detail_get_module_source_info_path(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_nextcode_binary_detail_get_nextcode_overlay_dependencies(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_nextcode_binary_detail_get_header_dependency(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_nextcode_binary_detail_get_header_dependency_module_dependencies(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC bool
nextcodescan_nextcode_binary_detail_get_is_framework(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_nextcode_binary_detail_get_module_cache_key(
    nextcodescan_module_details_t details);

//=== NeXTCode Placeholder Module Details query APIs -------------------------===//

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_nextcode_placeholder_detail_get_compiled_module_path(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_nextcode_placeholder_detail_get_module_doc_path(
    nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_nextcode_placeholder_detail_get_module_source_info_path(
    nextcodescan_module_details_t details);

//=== Clang Module Details query APIs -------------------------------------===//

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_clang_detail_get_module_map_path(nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_clang_detail_get_context_hash(nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_clang_detail_get_command_line(nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_clang_detail_get_captured_pcm_args(nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_clang_detail_get_cas_fs_root_id(nextcodescan_module_details_t details);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_clang_detail_get_module_cache_key(nextcodescan_module_details_t details);

//=== Batch Scan Input Functions ------------------------------------------===//

/// Create an \c nextcodescan_batch_scan_input_t instance.
/// The returned \c nextcodescan_batch_scan_input_t is owned by the caller and must be disposed
/// of using \c nextcodescan_batch_scan_input_dispose .
NEXTCODESCAN_PUBLIC nextcodescan_batch_scan_input_t *
nextcodescan_batch_scan_input_create();

NEXTCODESCAN_PUBLIC void
nextcodescan_batch_scan_input_set_modules(nextcodescan_batch_scan_input_t *input,
                                       int count,
                                       nextcodescan_batch_scan_entry_t *modules);

//=== Batch Scan Entry Functions ------------------------------------------===//

/// Create an \c nextcodescan_batch_scan_entry_t instance.
/// The returned \c nextcodescan_batch_scan_entry_t is owned by the caller and must be disposed
/// of using \c nextcodescan_batch_scan_entry_dispose .
NEXTCODESCAN_PUBLIC nextcodescan_batch_scan_entry_t
nextcodescan_batch_scan_entry_create();

NEXTCODESCAN_PUBLIC void
nextcodescan_batch_scan_entry_set_module_name(nextcodescan_batch_scan_entry_t entry,
                                           const char *name);

NEXTCODESCAN_PUBLIC void
nextcodescan_batch_scan_entry_set_arguments(nextcodescan_batch_scan_entry_t entry,
                                         const char *arguments);

NEXTCODESCAN_PUBLIC void
nextcodescan_batch_scan_entry_set_is_nextcode(nextcodescan_batch_scan_entry_t entry,
                                        bool is_nextcode);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_batch_scan_entry_get_module_name(nextcodescan_batch_scan_entry_t entry);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_batch_scan_entry_get_arguments(nextcodescan_batch_scan_entry_t entry);

NEXTCODESCAN_PUBLIC bool
nextcodescan_batch_scan_entry_get_is_nextcode(nextcodescan_batch_scan_entry_t entry);

//=== Prescan Result Functions --------------------------------------------===//

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_import_set_get_imports(nextcodescan_import_set_t result);

// Return value disposed of together with the dependency_graph
// using `nextcodescan_import_set_dispose`
NEXTCODESCAN_PUBLIC nextcodescan_diagnostic_set_t *
nextcodescan_import_set_get_diagnostics(nextcodescan_import_set_t result);

//=== Scanner Invocation Functions ----------------------------------------===//

/// Create an \c nextcodescan_scan_invocation_t instance.
/// The returned \c nextcodescan_scan_invocation_t is owned by the caller and must be disposed
/// of using \c nextcodescan_scan_invocation_dispose .
NEXTCODESCAN_PUBLIC nextcodescan_scan_invocation_t nextcodescan_scan_invocation_create();

NEXTCODESCAN_PUBLIC void nextcodescan_scan_invocation_set_working_directory(
    nextcodescan_scan_invocation_t invocation, const char *working_directory);

NEXTCODESCAN_PUBLIC void
nextcodescan_scan_invocation_set_argv(nextcodescan_scan_invocation_t invocation,
                                   int argc, const char **argv);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_scan_invocation_get_working_directory(
    nextcodescan_scan_invocation_t invocation);

NEXTCODESCAN_PUBLIC int
nextcodescan_scan_invocation_get_argc(nextcodescan_scan_invocation_t invocation);

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_scan_invocation_get_argv(nextcodescan_scan_invocation_t invocation);

//=== Cleanup Functions ---------------------------------------------------===//

NEXTCODESCAN_PUBLIC void
nextcodescan_string_set_dispose(nextcodescan_string_set_t *set);

NEXTCODESCAN_PUBLIC void
nextcodescan_string_dispose(nextcodescan_string_ref_t string);

NEXTCODESCAN_PUBLIC void
nextcodescan_dependency_graph_dispose(nextcodescan_dependency_graph_t result);

NEXTCODESCAN_PUBLIC void
nextcodescan_import_set_dispose(nextcodescan_import_set_t result);

NEXTCODESCAN_PUBLIC void
nextcodescan_batch_scan_entry_dispose(nextcodescan_batch_scan_entry_t entry);

NEXTCODESCAN_PUBLIC void
nextcodescan_batch_scan_input_dispose(nextcodescan_batch_scan_input_t *input);

NEXTCODESCAN_PUBLIC void
nextcodescan_batch_scan_result_dispose(nextcodescan_batch_scan_result_t *result);

NEXTCODESCAN_PUBLIC void
nextcodescan_scan_invocation_dispose(nextcodescan_scan_invocation_t invocation);

//=== Feature-Query Functions ---------------------------------------------===//
NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_compiler_supported_arguments_query();

NEXTCODESCAN_PUBLIC nextcodescan_string_set_t *
nextcodescan_compiler_supported_features_query();

//=== Target-Info Functions -----------------------------------------------===//
NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_compiler_target_info_query(nextcodescan_scan_invocation_t invocation);
NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_compiler_target_info_query_v2(nextcodescan_scan_invocation_t invocation,
                                        const char *main_executable_path);

//=== Scanner Functions ---------------------------------------------------===//

/// Container of the configuration state and shared cache for dependency
/// scanning.
typedef void *nextcodescan_scanner_t;

/// Create an \c nextcodescan_scanner_t instance.
/// The returned \c nextcodescan_scanner_t is owned by the caller and must be disposed
/// of using \c nextcodescan_scanner_dispose .
NEXTCODESCAN_PUBLIC nextcodescan_scanner_t nextcodescan_scanner_create(void);
NEXTCODESCAN_PUBLIC void nextcodescan_scanner_dispose(nextcodescan_scanner_t);

/// Invoke a dependency scan using arguments specified in the \c
/// nextcodescan_scan_invocation_t argument. The returned \c
/// nextcodescan_dependency_graph_t is owned by the caller and must be disposed of
/// using \c nextcodescan_dependency_graph_dispose .
NEXTCODESCAN_PUBLIC nextcodescan_dependency_graph_t nextcodescan_dependency_graph_create(
    nextcodescan_scanner_t scanner, nextcodescan_scan_invocation_t invocation);

/// Invoke the scan for an input batch of modules specified in the
/// \c nextcodescan_batch_scan_input_t argument. The returned
/// \c nextcodescan_batch_scan_result_t is owned by the caller and must be disposed
/// of using \c nextcodescan_batch_scan_result_dispose .
NEXTCODESCAN_PUBLIC nextcodescan_batch_scan_result_t *
nextcodescan_batch_scan_result_create(nextcodescan_scanner_t scanner,
                                   nextcodescan_batch_scan_input_t *batch_input,
                                   nextcodescan_scan_invocation_t invocation);

/// Invoke the import prescan using arguments specified in the \c
/// nextcodescan_scan_invocation_t argument. The returned \c nextcodescan_import_set_t
/// is owned by the caller and must be disposed of using \c
/// nextcodescan_import_set_dispose .
NEXTCODESCAN_PUBLIC nextcodescan_import_set_t nextcodescan_import_set_create(
    nextcodescan_scanner_t scanner, nextcodescan_scan_invocation_t invocation);


//=== Scanner Diagnostics -------------------------------------------------===//
/// For the specified \c scanner instance, query all insofar emitted diagnostics
NEXTCODESCAN_PUBLIC nextcodescan_diagnostic_set_t*
nextcodescan_scanner_diagnostics_query(nextcodescan_scanner_t scanner);

/// For the specified \c scanner instance, reset its diagnostic state
NEXTCODESCAN_PUBLIC void
nextcodescan_scanner_diagnostics_reset(nextcodescan_scanner_t scanner);

NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_diagnostic_get_message(nextcodescan_diagnostic_info_t diagnostic);

NEXTCODESCAN_PUBLIC nextcodescan_diagnostic_severity_t
nextcodescan_diagnostic_get_severity(nextcodescan_diagnostic_info_t diagnostic);

NEXTCODESCAN_PUBLIC nextcodescan_source_location_t
nextcodescan_diagnostic_get_source_location(nextcodescan_diagnostic_info_t diagnostic);

NEXTCODESCAN_PUBLIC void
nextcodescan_diagnostics_set_dispose(nextcodescan_diagnostic_set_t* diagnostics);

//=== Source Location -----------------------------------------------------===//
NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_source_location_get_buffer_identifier(nextcodescan_source_location_t source_location);

NEXTCODESCAN_PUBLIC int64_t
nextcodescan_source_location_get_line_number(nextcodescan_source_location_t source_location);

NEXTCODESCAN_PUBLIC int64_t
nextcodescan_source_location_get_column_number(nextcodescan_source_location_t source_location);

//=== Scanner Cache Operations --------------------------------------------===//
// The following operations expose an implementation detail of the dependency
// scanner: its module dependencies cache. This is done in order
// to allow clients to perform incremental dependency scans by having the
// scanner's state be serializable and re-usable.

/// For the specified \c scanner instance, serialize its state to the specified file-system \c path .
NEXTCODESCAN_PUBLIC void
nextcodescan_scanner_cache_serialize(nextcodescan_scanner_t scanner,
                                  const char * path);

/// For the specified \c scanner instance, load in scanner state from a file at
/// the specified file-system \c path .
NEXTCODESCAN_PUBLIC bool
nextcodescan_scanner_cache_load(nextcodescan_scanner_t scanner,
                             const char * path);

/// For the specified \c scanner instance, reset its internal state, ensuring subsequent
/// scanning queries are done "from-scratch".
NEXTCODESCAN_PUBLIC void
nextcodescan_scanner_cache_reset(nextcodescan_scanner_t scanner);

/// An entry point to invoke the compiler via a library call.
NEXTCODESCAN_PUBLIC int invoke_nextcode_compiler(int argc, const char **argv);

//=== Scanner CAS Operations ----------------------------------------------===//

/// Opaque container for a CASOptions that describe how CAS should be created.
typedef struct nextcodescan_cas_options_s *nextcodescan_cas_options_t;

/// Opaque container for a CAS instance that includes both ObjectStore and
/// ActionCache.
typedef struct nextcodescan_cas_s *nextcodescan_cas_t;

/// Opaque container for a cached compilation.
typedef struct nextcodescan_cached_compilation_s *nextcodescan_cached_compilation_t;

/// Opaque container for a cached compilation output.
typedef struct nextcodescan_cached_output_s *nextcodescan_cached_output_t;

/// Opaque type for a cache replay instance.
typedef struct nextcodescan_cache_replay_instance_s
    *nextcodescan_cache_replay_instance_t;

/// Opaque container for a cached compilation replay result.
typedef struct nextcodescan_cache_replay_result_s *nextcodescan_cache_replay_result_t;

/// Opaque type for a cancellation token for async cache operations.
typedef struct nextcodescan_cache_cancellation_token_s
    *nextcodescan_cache_cancellation_token_t;

/// Create a \c CASOptions for creating CAS inside scanner specified.
NEXTCODESCAN_PUBLIC nextcodescan_cas_options_t nextcodescan_cas_options_create(void);

/// Dispose \c CASOptions.
NEXTCODESCAN_PUBLIC void
nextcodescan_cas_options_dispose(nextcodescan_cas_options_t options);

/// Set on-disk path for the \c cas.
NEXTCODESCAN_PUBLIC void
nextcodescan_cas_options_set_ondisk_path(nextcodescan_cas_options_t options,
                                      const char *path);

/// Set plugin path for the \c cas.
NEXTCODESCAN_PUBLIC void
nextcodescan_cas_options_set_plugin_path(nextcodescan_cas_options_t options,
                                      const char *path);

/// Set option using a name/value pair. Return true if error.
/// If error happens, the error message is returned via `error` parameter, and
/// caller needs to free the error message via `nextcodescan_string_dispose`.
NEXTCODESCAN_PUBLIC bool
nextcodescan_cas_options_set_plugin_option(nextcodescan_cas_options_t options,
                                        const char *name, const char *value,
                                        nextcodescan_string_ref_t *error);

/// Create a \c cas instance from plugin. Return NULL if error.
/// If error happens, the error message is returned via `error` parameter, and
/// caller needs to free the error message via `nextcodescan_string_dispose`.
NEXTCODESCAN_PUBLIC nextcodescan_cas_t nextcodescan_cas_create_from_options(
    nextcodescan_cas_options_t options, nextcodescan_string_ref_t *error);

/// Store content into CAS. Return \c CASID as string. Return NULL on error.
/// If error happens, the error message is returned via `error` parameter, and
/// caller needs to free the error message via `nextcodescan_string_dispose`.
NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_cas_store(nextcodescan_cas_t cas, uint8_t *data, unsigned size,
                    nextcodescan_string_ref_t *error);

/// Get the local storage size for the CAS in bytes. Return the local storage
/// size of the CAS/cache data, or -1 if the implementation does not support
/// reporting such size, or -2 if an error occurred.
/// If error happens, the error message is returned via `error` parameter, and
/// caller needs to free the error message via `nextcodescan_string_dispose`.
NEXTCODESCAN_PUBLIC int64_t
nextcodescan_cas_get_ondisk_size(nextcodescan_cas_t, nextcodescan_string_ref_t *error);

/// Set the size for the limiting disk storage size for CAS. \c size_limit is
/// the maximum size limit in bytes (0 means no limit, negative is invalid).
/// Return true if error. If error happens, the error message is returned via
/// `error` parameter, and caller needs to free the error message via
/// `nextcodescan_string_dispose`.
NEXTCODESCAN_PUBLIC bool
nextcodescan_cas_set_ondisk_size_limit(nextcodescan_cas_t, int64_t size_limit,
                                    nextcodescan_string_ref_t *error);

/// Prune local CAS storage according to the size limit. Return true if error.
/// If error happens, the error message is returned via `error` parameter, and
/// caller needs to free the error message via `nextcodescan_string_dispose`.
NEXTCODESCAN_PUBLIC bool
nextcodescan_cas_prune_ondisk_data(nextcodescan_cas_t, nextcodescan_string_ref_t *error);

/// Dispose the \c cas instance.
NEXTCODESCAN_PUBLIC void nextcodescan_cas_dispose(nextcodescan_cas_t cas);

/// Compute \c CacheKey for the outputs of a primary input file from a compiler
/// invocation with command-line \c argc and \c argv. When primary input file
/// is not available for compilation, e.g., using WMO, primary file is the first
/// nextcode input on the command-line by convention. Return \c CacheKey as string.
/// If error happens, the error message is returned via `error` parameter, and
/// caller needs to free the error message via `nextcodescan_string_dispose`.
/// This API is DEPRECATED and in favor of using
/// `nextcodescan_cache_compute_key_from_input_index`.
NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_cache_compute_key(nextcodescan_cas_t cas, int argc, const char **argv,
                            const char *input, nextcodescan_string_ref_t *error);

/// Compute \c CacheKey for the outputs of a primary input file from a compiler
/// invocation with command-line \c argc and \c argv and the index for the
/// input. The index of the input is computed from the position of the input
/// file from all input files. When primary input file is not available for
/// compilation, e.g., using WMO, primary file is the first nextcode input on the
/// command-line by convention. Return \c CacheKey as string. If error happens,
/// the error message is returned via `error` parameter, and caller needs to
/// free the error message via `nextcodescan_string_dispose`.
NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
nextcodescan_cache_compute_key_from_input_index(nextcodescan_cas_t cas, int argc,
                                             const char **argv,
                                             unsigned input_index,
                                             nextcodescan_string_ref_t *error);

/// Query the result of the compilation using the output cache key. \c globally
/// suggests if the lookup should check remote cache if such operation exists.
/// Returns the cached compilation of the result if found, or nullptr if output
/// is not found or an error occurs. When an error occurs, the error message is
/// returned via \c error parameter and its caller needs to free the message
/// using `nextcodescan_string_dispose`. The returned cached compilation needs to
/// be freed via `nextcodescan_cached_compilation_dispose`.
NEXTCODESCAN_PUBLIC nextcodescan_cached_compilation_t
nextcodescan_cache_query(nextcodescan_cas_t cas, const char *key, bool globally,
                      nextcodescan_string_ref_t *error);

/// Async version of `nextcodescan_cache_query` where result is returned via
/// callback. Both cache_result enum and cached compilation will be provided to
/// callback. \c ctx is an opaque value that passed to the callback and \c
/// nextcodescan_cache_cancellation_token_t will return an token that can be used
/// to cancel the async operation. The token needs to be freed by caller using
/// `nextcodescan_cache_cancellation_token_dispose`. If no token is needed, nullptr
/// can be passed and no token will be returned.
NEXTCODESCAN_PUBLIC void nextcodescan_cache_query_async(
    nextcodescan_cas_t cas, const char *key, bool globally, void *ctx,
    void (*callback)(void *ctx, nextcodescan_cached_compilation_t,
                     nextcodescan_string_ref_t error),
    nextcodescan_cache_cancellation_token_t *);

/// Query the number of outputs from a cached compilation.
NEXTCODESCAN_PUBLIC unsigned nextcodescan_cached_compilation_get_num_outputs(
    nextcodescan_cached_compilation_t);

/// Get the cached output for the given index in the cached compilation.
NEXTCODESCAN_PUBLIC nextcodescan_cached_output_t
nextcodescan_cached_compilation_get_output(nextcodescan_cached_compilation_t,
                                        unsigned idx);

/// Check if the requested cached compilation is uncacheable. That means the
/// compiler decides to skip caching its output even the compilation is
/// successful.
NEXTCODESCAN_PUBLIC bool
    nextcodescan_cached_compilation_is_uncacheable(nextcodescan_cached_compilation_t);

/// Make the cache compilation available globally. \c callback will be called
/// on completion.
/// \c nextcodescan_cache_cancellation_token_t will return an token that can be
/// used to cancel the async operation. The token needs to be freed by caller
/// using `nextcodescan_cache_cancellation_token_dispose`. If no token is needed,
/// nullptr can be passed and no token will be returned.
NEXTCODESCAN_PUBLIC void nextcodescan_cached_compilation_make_global_async(
    nextcodescan_cached_compilation_t, void *ctx,
    void (*callback)(void *ctx, nextcodescan_string_ref_t error),
    nextcodescan_cache_cancellation_token_t *);

/// Dispose a cached compilation.
NEXTCODESCAN_PUBLIC
void nextcodescan_cached_compilation_dispose(nextcodescan_cached_compilation_t);

/// Download and materialize the cached output if needed from a remote CAS.
/// Return true if load is successful, else false if not found or error. If
/// error, the error message is returned via \c error parameter and its caller
/// needs to free the message using `nextcodescan_string_dispose`.
NEXTCODESCAN_PUBLIC bool
nextcodescan_cached_output_load(nextcodescan_cached_output_t,
                             nextcodescan_string_ref_t *error);

/// Async version of `nextcodescan_cached_output_load` where result is
/// returned via callback. \c ctx is an opaque value that passed to the callback
/// and \c nextcodescan_cache_cancellation_token_t will return an token that can be
/// used to cancel the async operation. The token needs to be freed by caller
/// using `nextcodescan_cache_cancellation_token_dispose`. If no token is needed,
/// nullptr can be passed and no token will be returned.
NEXTCODESCAN_PUBLIC void nextcodescan_cached_output_load_async(
    nextcodescan_cached_output_t, void *ctx,
    void (*callback)(void *ctx, bool success, nextcodescan_string_ref_t error),
    nextcodescan_cache_cancellation_token_t *);

/// Check if cached output is materialized locally and can be accessed
/// without downloading.
NEXTCODESCAN_PUBLIC bool
    nextcodescan_cached_output_is_materialized(nextcodescan_cached_output_t);

/// Return the casid for the cached output as \c nextcodescan_string_ref_t and the
/// returned string needs to be freed using `nextcodescan_string_dispose`. CASID
/// can be requested before loading/materializing.
NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
    nextcodescan_cached_output_get_casid(nextcodescan_cached_output_t);

/// Get the output name for cached compilation. The
/// returned name needs to be freed by `nextcodescan_string_dispose`.
NEXTCODESCAN_PUBLIC nextcodescan_string_ref_t
    nextcodescan_cached_output_get_name(nextcodescan_cached_output_t);

/// Dispose a cached output.
NEXTCODESCAN_PUBLIC
void nextcodescan_cached_output_dispose(nextcodescan_cached_output_t);

/// Cancel the async cache action that is associated with token.
NEXTCODESCAN_PUBLIC void
    nextcodescan_cache_action_cancel(nextcodescan_cache_cancellation_token_t);

/// Dispose the cancellation token.
NEXTCODESCAN_PUBLIC void nextcodescan_cache_cancellation_token_dispose(
    nextcodescan_cache_cancellation_token_t);

/// Async load CASObject using CASID. This is only to perform the download to
/// local CAS where result is returned via callback. \c ctx is an opaque value
/// that passed to the callback and \c nextcodescan_cache_cancellation_token_t will
/// return an token that can be used to cancel the async operation. The token
/// needs to be freed by caller using
/// `nextcodescan_cache_cancellation_token_dispose`. If no token is needed, nullptr
/// can be passed and no token will be returned.
NEXTCODESCAN_PUBLIC void nextcodescan_cache_download_cas_object_async(
    nextcodescan_cas_t, const char *id, void *ctx,
    void (*callback)(void *ctx, bool success, nextcodescan_string_ref_t error),
    nextcodescan_cache_cancellation_token_t *);

/// Create a nextcode cached compilation replay instance with its command-line
/// invocation. Return nullptr when errors occurs and the error message is
/// returned via \c error parameter and its caller needs to free the message
/// using `nextcodescan_string_dispose`.
NEXTCODESCAN_PUBLIC nextcodescan_cache_replay_instance_t
nextcodescan_cache_replay_instance_create(int argc, const char **argv,
                                       nextcodescan_string_ref_t *error);

/// Dispose nextcode cached compilation replay instance.
NEXTCODESCAN_PUBLIC void
    nextcodescan_cache_replay_instance_dispose(nextcodescan_cache_replay_instance_t);

/// Replay the cached compilation using cached compliation replay instance.
/// Returns replay result or nullptr if output not found or error occurs. If
/// error, the error message is returned via \c error parameter and its caller
/// needs to free the message using `nextcodescan_string_dispose`.
NEXTCODESCAN_PUBLIC nextcodescan_cache_replay_result_t
nextcodescan_cache_replay_compilation(nextcodescan_cache_replay_instance_t,
                                   nextcodescan_cached_compilation_t,
                                   nextcodescan_string_ref_t *error);

/// Get stdout from cached replay result. The returning nextcodescan_string_ref_t
/// is owned by replay result and should not be disposed.
NEXTCODESCAN_PUBLIC
nextcodescan_string_ref_t
    nextcodescan_cache_replay_result_get_stdout(nextcodescan_cache_replay_result_t);

/// Get stderr from cached replay result. The returning nextcodescan_string_ref_t
/// is owned by replay result and should not be disposed.
NEXTCODESCAN_PUBLIC
nextcodescan_string_ref_t
    nextcodescan_cache_replay_result_get_stderr(nextcodescan_cache_replay_result_t);

/// Dispose a cached replay result.
NEXTCODESCAN_PUBLIC
void nextcodescan_cache_replay_result_dispose(nextcodescan_cache_replay_result_t);

//===----------------------------------------------------------------------===//

NEXTCODESCAN_END_DECLS

#endif // NEXTCODE_C_DEPENDENCY_SCAN_H
