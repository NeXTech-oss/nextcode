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

#include "nextcode-c/DependencyScan/DependencyScan.h"
#include "nextcode/Basic/LLVMInitialize.h"
#include "nextcode/Basic/InitializeNeXTCodeModules.h"
#include "nextcode/DependencyScan/DependencyScanImpl.h"
#include "nextcode/DependencyScan/DependencyScanningTool.h"
#include "nextcode/DependencyScan/StringUtils.h"
#include "nextcode/DriverTool/DriverTool.h"
#include "nextcode/Option/Options.h"
#include "nextcode/SIL/SILBridging.h"

using namespace nextcode::dependencies;

DEFINE_SIMPLE_CONVERSION_FUNCTIONS(DependencyScanningTool, nextcodescan_scanner_t)

//=== Private Cleanup Functions -------------------------------------------===//

void nextcodescan_dependency_info_details_dispose(
    nextcodescan_module_details_t details) {
  nextcodescan_module_details_s *details_impl = details;
  switch (details_impl->kind) {
  case NEXTCODESCAN_DEPENDENCY_INFO_NEXTCODE_TEXTUAL:
    nextcodescan_string_dispose(
        details_impl->nextcode_textual_details.module_interface_path);
    nextcodescan_string_set_dispose(
        details_impl->nextcode_textual_details.compiled_module_candidates);
    nextcodescan_string_dispose(
        details_impl->nextcode_textual_details.bridging_header_path);
    nextcodescan_string_set_dispose(
        details_impl->nextcode_textual_details.bridging_source_files);
    nextcodescan_string_set_dispose(
        details_impl->nextcode_textual_details.bridging_module_dependencies);
    nextcodescan_string_set_dispose(
        details_impl->nextcode_textual_details.code_overlay_module_dependencies);
    nextcodescan_string_set_dispose(
        details_impl->nextcode_textual_details.command_line);
    nextcodescan_string_set_dispose(
        details_impl->nextcode_textual_details.extra_pcm_args);
    nextcodescan_string_dispose(details_impl->nextcode_textual_details.context_hash);
    nextcodescan_string_dispose(
        details_impl->nextcode_textual_details.cas_fs_root_id);
    nextcodescan_string_dispose(
        details_impl->nextcode_textual_details.bridging_header_include_tree);
    nextcodescan_string_dispose(
        details_impl->nextcode_textual_details.module_cache_key);
    break;
  case NEXTCODESCAN_DEPENDENCY_INFO_NEXTCODE_BINARY:
    nextcodescan_string_dispose(
        details_impl->nextcode_binary_details.compiled_module_path);
    nextcodescan_string_dispose(
        details_impl->nextcode_binary_details.module_doc_path);
    nextcodescan_string_dispose(
        details_impl->nextcode_binary_details.module_source_info_path);
    nextcodescan_string_set_dispose(
        details_impl->nextcode_binary_details.code_overlay_module_dependencies);
      nextcodescan_string_dispose(
        details_impl->nextcode_binary_details.header_dependency);
    nextcodescan_string_dispose(
        details_impl->nextcode_binary_details.module_cache_key);
    break;
  case NEXTCODESCAN_DEPENDENCY_INFO_NEXTCODE_PLACEHOLDER:
    nextcodescan_string_dispose(
        details_impl->nextcode_placeholder_details.compiled_module_path);
    nextcodescan_string_dispose(
        details_impl->nextcode_placeholder_details.module_doc_path);
    nextcodescan_string_dispose(
        details_impl->nextcode_placeholder_details.module_source_info_path);
    break;
  case NEXTCODESCAN_DEPENDENCY_INFO_CLANG:
    nextcodescan_string_dispose(details_impl->clang_details.module_map_path);
    nextcodescan_string_dispose(details_impl->clang_details.context_hash);
    nextcodescan_string_set_dispose(details_impl->clang_details.command_line);
    nextcodescan_string_set_dispose(details_impl->clang_details.captured_pcm_args);
    nextcodescan_string_dispose(details_impl->clang_details.cas_fs_root_id);
    nextcodescan_string_dispose(details_impl->clang_details.module_cache_key);
    break;
  }
  delete details_impl;
}

void nextcodescan_link_library_set_dispose(nextcodescan_link_library_set_t *set) {
  for (size_t i = 0; i < set->count; ++i) {
    auto info = set->link_libraries[i];
    nextcodescan_string_dispose(info->name);
    delete info;
  }
  delete[] set->link_libraries;
  delete set;
}

void nextcodescan_dependency_info_dispose(nextcodescan_dependency_info_t info) {
  nextcodescan_string_dispose(info->module_name);
  nextcodescan_string_dispose(info->module_path);
  nextcodescan_string_set_dispose(info->source_files);
  nextcodescan_string_set_dispose(info->direct_dependencies);
  nextcodescan_link_library_set_dispose(info->link_libraries);
  nextcodescan_dependency_info_details_dispose(info->details);
  delete info;
}

void nextcodescan_dependency_set_dispose(nextcodescan_dependency_set_t *set) {
  for (size_t i = 0; i < set->count; ++i) {
    nextcodescan_dependency_info_dispose(set->modules[i]);
  }
  delete[] set->modules;
  delete set;
}

//=== Scanner Cache Operations --------------------------------------------===//

void nextcodescan_scanner_cache_serialize(nextcodescan_scanner_t scanner,
                                       const char * path) {
  DependencyScanningTool *ScanningTool = unwrap(scanner);
  ScanningTool->serializeCache(path);
}

bool nextcodescan_scanner_cache_load(nextcodescan_scanner_t scanner,
                                  const char * path) {
  DependencyScanningTool *ScanningTool = unwrap(scanner);
  return ScanningTool->loadCache(path);
}

void nextcodescan_scanner_cache_reset(nextcodescan_scanner_t scanner) {
  DependencyScanningTool *ScanningTool = unwrap(scanner);
  ScanningTool->resetCache();
}

//=== Scanner Functions ---------------------------------------------------===//

nextcodescan_scanner_t nextcodescan_scanner_create(void) {
  static std::mutex initializationMutex;
  std::lock_guard<std::mutex> lock(initializationMutex);
  INITIALIZE_LLVM();
  if (!nextcodeModulesInitialized())
    initializeNeXTCodeModules();
  return wrap(new DependencyScanningTool());
}

void nextcodescan_scanner_dispose(nextcodescan_scanner_t c_scanner) {
  delete unwrap(c_scanner);
}

nextcodescan_dependency_graph_t
nextcodescan_dependency_graph_create(nextcodescan_scanner_t scanner,
                                  nextcodescan_scan_invocation_t invocation) {
  DependencyScanningTool *ScanningTool = unwrap(scanner);
  int argc = invocation->argv->count;
  std::vector<const char *> Compilation;
  for (int i = 0; i < argc; ++i)
    Compilation.push_back(nextcode::c_string_utils::get_C_string(invocation->argv->strings[i]));

  // Execute the scan and bridge the result
  auto ScanResult = ScanningTool->getDependencies(
      Compilation, {},
      nextcode::c_string_utils::get_C_string(invocation->working_directory));
  if (ScanResult.getError())
    return nullptr;
  auto DependencyGraph = std::move(*ScanResult);
  return DependencyGraph;
}

nextcodescan_batch_scan_result_t *
nextcodescan_batch_scan_result_create(nextcodescan_scanner_t scanner,
                                   nextcodescan_batch_scan_input_t *batch_input,
                                   nextcodescan_scan_invocation_t invocation) {
  DependencyScanningTool *ScanningTool = unwrap(scanner);
  int argc = invocation->argv->count;
  std::vector<const char *> Compilation;
  for (int i = 0; i < argc; ++i)
    Compilation.push_back(nextcode::c_string_utils::get_C_string(invocation->argv->strings[i]));

  std::vector<BatchScanInput> BatchInput;
  for (size_t i = 0; i < batch_input->count; ++i) {
    nextcodescan_batch_scan_entry_s *Entry = batch_input->modules[i];
    BatchInput.push_back({nextcode::c_string_utils::get_C_string(Entry->module_name),
        nextcode::c_string_utils::get_C_string(Entry->arguments),
                          /*outputPath*/ "", Entry->is_nextcode});
  }

  // Execute the scan and bridge the result
  auto BatchScanResult = ScanningTool->getDependencies(
      Compilation, BatchInput, {},
      nextcode::c_string_utils::get_C_string(invocation->working_directory));
  nextcodescan_batch_scan_result_t *Result = new nextcodescan_batch_scan_result_t;
  auto ResultGraphs = new nextcodescan_dependency_graph_t[BatchScanResult.size()];
  for (size_t i = 0; i < BatchScanResult.size(); ++i) {
    auto &ResultOrErr = BatchScanResult[i];
    if (ResultOrErr.getError()) {
      ResultGraphs[i] = nullptr;
      continue;
    }

    ResultGraphs[i] = ResultOrErr.get();
  }

  Result->results = ResultGraphs;
  Result->count = BatchScanResult.size();
  return Result;
}

nextcodescan_import_set_t
nextcodescan_import_set_create(nextcodescan_scanner_t scanner,
                            nextcodescan_scan_invocation_t invocation) {
  DependencyScanningTool *ScanningTool = unwrap(scanner);
  int argc = invocation->argv->count;
  std::vector<const char *> Compilation;
  for (int i = 0; i < argc; ++i)
    Compilation.push_back(nextcode::c_string_utils::get_C_string(invocation->argv->strings[i]));

  // Execute the scan and bridge the result
  auto PreScanResult = ScanningTool->getImports(
      Compilation,
      nextcode::c_string_utils::get_C_string(invocation->working_directory));
  if (PreScanResult.getError())
    return nullptr;
  auto ImportSet = std::move(*PreScanResult);
  return ImportSet;
}

//=== Dependency Result Functions -----------------------------------------===//

nextcodescan_string_ref_t nextcodescan_dependency_graph_get_main_module_name(
    nextcodescan_dependency_graph_t result) {
  return result->main_module_name;
}

nextcodescan_dependency_set_t *nextcodescan_dependency_graph_get_dependencies(
    nextcodescan_dependency_graph_t result) {
  return result->dependencies;
}

nextcodescan_diagnostic_set_t *nextcodescan_dependency_graph_get_diagnostics(
    nextcodescan_dependency_graph_t result) {
  return result->diagnostics;
}

//=== Module Dependency Info query APIs -----------------------------------===//

nextcodescan_string_ref_t
nextcodescan_module_info_get_module_name(nextcodescan_dependency_info_t info) {
  return info->module_name;
}

nextcodescan_string_ref_t
nextcodescan_module_info_get_module_path(nextcodescan_dependency_info_t info) {
  return info->module_path;
}

nextcodescan_string_set_t *
nextcodescan_module_info_get_source_files(nextcodescan_dependency_info_t info) {
  return info->source_files;
}

nextcodescan_string_set_t *nextcodescan_module_info_get_direct_dependencies(
    nextcodescan_dependency_info_t info) {
  return info->direct_dependencies;
}


nextcodescan_link_library_set_t *nextcodescan_module_info_get_link_libraries(
    nextcodescan_dependency_info_t info) {
  return info->link_libraries;
}

nextcodescan_module_details_t
nextcodescan_module_info_get_details(nextcodescan_dependency_info_t info) {
  return info->details;
}

//=== Link Library Info query APIs -----------------------------------===//

nextcodescan_string_ref_t
nextcodescan_link_library_info_get_link_name(nextcodescan_link_library_info_t info) {
  return info->name;
}
bool
nextcodescan_link_library_info_get_is_framework(nextcodescan_link_library_info_t info) {
  return info->isFramework;
}
bool
nextcodescan_link_library_info_get_should_force_load(nextcodescan_link_library_info_t info) {
  return info->forceLoad;
}

//=== NeXTCode Textual Module Details query APIs -----------------------------===//

nextcodescan_dependency_info_kind_t
nextcodescan_module_detail_get_kind(nextcodescan_module_details_t details) {
  return details->kind;
}

nextcodescan_string_ref_t nextcodescan_nextcode_textual_detail_get_module_interface_path(
    nextcodescan_module_details_t details) {
  return details->nextcode_textual_details.module_interface_path;
}

nextcodescan_string_set_t *
nextcodescan_nextcode_textual_detail_get_compiled_module_candidates(
    nextcodescan_module_details_t details) {
  return details->nextcode_textual_details.compiled_module_candidates;
}

nextcodescan_string_ref_t nextcodescan_nextcode_textual_detail_get_bridging_header_path(
    nextcodescan_module_details_t details) {
  return details->nextcode_textual_details.bridging_header_path;
}

nextcodescan_string_set_t *
nextcodescan_nextcode_textual_detail_get_bridging_source_files(
    nextcodescan_module_details_t details) {
  return details->nextcode_textual_details.bridging_source_files;
}

nextcodescan_string_set_t *
nextcodescan_nextcode_textual_detail_get_bridging_module_dependencies(
    nextcodescan_module_details_t details) {
  return details->nextcode_textual_details.bridging_module_dependencies;
}

nextcodescan_string_set_t *nextcodescan_nextcode_textual_detail_get_command_line(
    nextcodescan_module_details_t details) {
  return details->nextcode_textual_details.command_line;
}

nextcodescan_string_set_t *
nextcodescan_nextcode_textual_detail_get_bridging_pch_command_line(
    nextcodescan_module_details_t details) {
  return details->nextcode_textual_details.bridging_pch_command_line;
}

nextcodescan_string_set_t *nextcodescan_nextcode_textual_detail_get_extra_pcm_args(
    nextcodescan_module_details_t details) {
  return details->nextcode_textual_details.extra_pcm_args;
}

nextcodescan_string_ref_t nextcodescan_nextcode_textual_detail_get_context_hash(
    nextcodescan_module_details_t details) {
  return details->nextcode_textual_details.context_hash;
}

bool nextcodescan_nextcode_textual_detail_get_is_framework(
    nextcodescan_module_details_t details) {
  return details->nextcode_textual_details.is_framework;
}

nextcodescan_string_set_t *nextcodescan_nextcode_textual_detail_get_nextcode_overlay_dependencies(
    nextcodescan_module_details_t details) {
  return details->nextcode_textual_details.code_overlay_module_dependencies;
}

nextcodescan_string_ref_t nextcodescan_nextcode_textual_detail_get_cas_fs_root_id(
    nextcodescan_module_details_t details) {
  return details->nextcode_textual_details.cas_fs_root_id;
}

nextcodescan_string_ref_t nextcodescan_nextcode_textual_detail_get_module_cache_key(
    nextcodescan_module_details_t details) {
  return details->nextcode_textual_details.module_cache_key;
}

//=== NeXTCode Binary Module Details query APIs ------------------------------===//

nextcodescan_string_ref_t nextcodescan_nextcode_binary_detail_get_compiled_module_path(
    nextcodescan_module_details_t details) {
  return details->nextcode_binary_details.compiled_module_path;
}

nextcodescan_string_ref_t nextcodescan_nextcode_binary_detail_get_module_doc_path(
    nextcodescan_module_details_t details) {
  return details->nextcode_binary_details.module_doc_path;
}

nextcodescan_string_ref_t
nextcodescan_nextcode_binary_detail_get_module_source_info_path(
    nextcodescan_module_details_t details) {
  return details->nextcode_binary_details.module_source_info_path;
}

nextcodescan_string_set_t *
nextcodescan_nextcode_binary_detail_get_nextcode_overlay_dependencies(
    nextcodescan_module_details_t details) {
  return details->nextcode_binary_details.code_overlay_module_dependencies;
}

nextcodescan_string_ref_t
nextcodescan_nextcode_binary_detail_get_header_dependency(
    nextcodescan_module_details_t details) {
  return details->nextcode_binary_details.header_dependency;
}

nextcodescan_string_set_t *
nextcodescan_nextcode_binary_detail_get_header_dependency_module_dependencies(
    nextcodescan_module_details_t details) {
  return details->nextcode_binary_details.header_dependencies_module_dependnecies;
}

bool nextcodescan_nextcode_binary_detail_get_is_framework(
    nextcodescan_module_details_t details) {
  return details->nextcode_binary_details.is_framework;
}

nextcodescan_string_ref_t nextcodescan_nextcode_binary_detail_get_module_cache_key(
    nextcodescan_module_details_t details) {
  return details->nextcode_binary_details.module_cache_key;
}


//=== NeXTCode Placeholder Module Details query APIs -------------------------===//

nextcodescan_string_ref_t
nextcodescan_nextcode_placeholder_detail_get_compiled_module_path(
    nextcodescan_module_details_t details) {
  return details->nextcode_placeholder_details.module_source_info_path;
}

nextcodescan_string_ref_t nextcodescan_nextcode_placeholder_detail_get_module_doc_path(
    nextcodescan_module_details_t details) {
  return details->nextcode_placeholder_details.module_source_info_path;
}

nextcodescan_string_ref_t
nextcodescan_nextcode_placeholder_detail_get_module_source_info_path(
    nextcodescan_module_details_t details) {
  return details->nextcode_placeholder_details.module_source_info_path;
}

//=== Clang Module Details query APIs -------------------------------------===//

nextcodescan_string_ref_t
nextcodescan_clang_detail_get_module_map_path(nextcodescan_module_details_t details) {
  return details->clang_details.module_map_path;
}

nextcodescan_string_ref_t
nextcodescan_clang_detail_get_context_hash(nextcodescan_module_details_t details) {
  return details->clang_details.context_hash;
}

nextcodescan_string_set_t *
nextcodescan_clang_detail_get_command_line(nextcodescan_module_details_t details) {
  return details->clang_details.command_line;
}

nextcodescan_string_set_t *
nextcodescan_clang_detail_get_captured_pcm_args(nextcodescan_module_details_t details) {
  return details->clang_details.captured_pcm_args;
}

nextcodescan_string_ref_t
nextcodescan_clang_detail_get_cas_fs_root_id(nextcodescan_module_details_t details) {
  return details->clang_details.cas_fs_root_id;
}

nextcodescan_string_ref_t nextcodescan_clang_detail_get_module_cache_key(
    nextcodescan_module_details_t details) {
  return details->clang_details.module_cache_key;
}

//=== Batch Scan Input Functions ------------------------------------------===//

nextcodescan_batch_scan_input_t *nextcodescan_batch_scan_input_create() {
  return new nextcodescan_batch_scan_input_t;
}

void nextcodescan_batch_scan_input_set_modules(
    nextcodescan_batch_scan_input_t *input, int count,
    nextcodescan_batch_scan_entry_t *modules) {
  input->count = count;
  input->modules = modules;
}

//=== Batch Scan Entry Functions ------------------------------------------===//

nextcodescan_batch_scan_entry_t nextcodescan_batch_scan_entry_create() {
  return new nextcodescan_batch_scan_entry_s;
}

void nextcodescan_batch_scan_entry_set_module_name(
    nextcodescan_batch_scan_entry_t entry, const char *name) {
  entry->module_name = nextcode::c_string_utils::create_clone(name);
}

void nextcodescan_batch_scan_entry_set_arguments(
    nextcodescan_batch_scan_entry_t entry, const char *arguments) {
  entry->arguments = nextcode::c_string_utils::create_clone(arguments);
}

void nextcodescan_batch_scan_entry_set_is_nextcode(nextcodescan_batch_scan_entry_t entry,
                                             bool is_nextcode) {
  entry->is_nextcode = is_nextcode;
}

nextcodescan_string_ref_t
nextcodescan_batch_scan_entry_get_module_name(nextcodescan_batch_scan_entry_t entry) {
  return entry->module_name;
}

nextcodescan_string_ref_t
nextcodescan_batch_scan_entry_get_arguments(nextcodescan_batch_scan_entry_t entry) {
  return entry->arguments;
}

bool nextcodescan_batch_scan_entry_get_is_nextcode(
    nextcodescan_batch_scan_entry_t entry) {
  return entry->is_nextcode;
}

//=== Prescan Result Functions --------------------------------------------===//

nextcodescan_string_set_t *
nextcodescan_import_set_get_imports(nextcodescan_import_set_t result) {
  return result->imports;
}

nextcodescan_diagnostic_set_t *
nextcodescan_import_set_get_diagnostics(nextcodescan_import_set_t result) {
  return result->diagnostics;
}

//=== Scanner Invocation Functions ----------------------------------------===//

nextcodescan_scan_invocation_t nextcodescan_scan_invocation_create() {
  return new nextcodescan_scan_invocation_s;
}

void nextcodescan_scan_invocation_set_working_directory(
    nextcodescan_scan_invocation_t invocation, const char *working_directory) {
  invocation->working_directory = nextcode::c_string_utils::create_clone(working_directory);
}

void
nextcodescan_scan_invocation_set_argv(nextcodescan_scan_invocation_t invocation,
                                   int argc, const char **argv) {
  invocation->argv = nextcode::c_string_utils::create_set(argc, argv);
}

nextcodescan_string_ref_t nextcodescan_scan_invocation_get_working_directory(
    nextcodescan_scan_invocation_t invocation) {
  return invocation->working_directory;
}

int nextcodescan_scan_invocation_get_argc(nextcodescan_scan_invocation_t invocation) {
  return invocation->argv->count;
}

nextcodescan_string_set_t *
nextcodescan_scan_invocation_get_argv(nextcodescan_scan_invocation_t invocation) {
  return invocation->argv;
}

//=== Public Cleanup Functions --------------------------------------------===//

void nextcodescan_string_dispose(nextcodescan_string_ref_t string) {
  if (string.data)
    free(const_cast<void *>(string.data));
}

void nextcodescan_string_set_dispose(nextcodescan_string_set_t *set) {
  for (unsigned SI = 0, SE = set->count; SI < SE; ++SI)
    nextcodescan_string_dispose(set->strings[SI]);
  if (set->count > 0)
    delete[] set->strings;
  delete set;
}

void nextcodescan_dependency_graph_dispose(nextcodescan_dependency_graph_t result) {
  nextcodescan_string_dispose(result->main_module_name);
  nextcodescan_dependency_set_dispose(result->dependencies);
  nextcodescan_diagnostics_set_dispose(result->diagnostics);
  delete result;
}

void nextcodescan_import_set_dispose(nextcodescan_import_set_t result) {
  nextcodescan_string_set_dispose(result->imports);
  nextcodescan_diagnostics_set_dispose(result->diagnostics);
  delete result;
}

void nextcodescan_batch_scan_entry_dispose(nextcodescan_batch_scan_entry_t entry) {
  nextcodescan_string_dispose(entry->module_name);
  nextcodescan_string_dispose(entry->arguments);
  delete entry;
}

void nextcodescan_batch_scan_input_dispose(nextcodescan_batch_scan_input_t *input) {
  for (size_t i = 0; i < input->count; ++i) {
    nextcodescan_batch_scan_entry_dispose(input->modules[i]);
  }
  delete[] input->modules;
  delete input;
}

void nextcodescan_batch_scan_result_dispose(
    nextcodescan_batch_scan_result_t *result) {
  for (size_t i = 0; i < result->count; ++i) {
    nextcodescan_dependency_graph_dispose(result->results[i]);
  }
  delete[] result->results;
  delete result;
}

void nextcodescan_scan_invocation_dispose(nextcodescan_scan_invocation_t invocation) {
  nextcodescan_string_dispose(invocation->working_directory);
  nextcodescan_string_set_dispose(invocation->argv);
  delete invocation;
}

//=== Feature-Query Functions -----------------------------------------===//
static void addFrontendFlagOption(llvm::opt::OptTable &table,
                                  nextcode::options::ID id,
                                  std::vector<std::string> &frontendOptions) {
  if (table.getOption(id).hasFlag(nextcode::options::FrontendOption)) {
    auto name = table.getOptionName(id);
    if (!name.empty()) {
      frontendOptions.push_back(std::string(name));
    }
  }
}

nextcodescan_string_ref_t
nextcodescan_compiler_target_info_query(nextcodescan_scan_invocation_t invocation) {
  return nextcodescan_compiler_target_info_query_v2(invocation, nullptr);
}

nextcodescan_string_ref_t
nextcodescan_compiler_target_info_query_v2(nextcodescan_scan_invocation_t invocation,
                                        const char *main_executable_path) {
  int argc = invocation->argv->count;
  std::vector<const char *> Compilation;
  for (int i = 0; i < argc; ++i)
    Compilation.push_back(nextcode::c_string_utils::get_C_string(invocation->argv->strings[i]));

  auto TargetInfo = nextcode::dependencies::getTargetInfo(Compilation, main_executable_path);
  if (TargetInfo.getError())
    return nextcode::c_string_utils::create_null();
  return TargetInfo.get();
}

nextcodescan_string_set_t *
nextcodescan_compiler_supported_arguments_query() {
  std::unique_ptr<llvm::opt::OptTable> table = nextcode::createNeXTCodeOptTable();
  std::vector<std::string> frontendFlags;
#define OPTION(PREFIX, NAME, ID, KIND, GROUP, ALIAS, ALIASARGS, FLAGS, PARAM,  \
               HELPTEXT, METAVAR, VALUES)                                      \
  addFrontendFlagOption(*table, nextcode::options::OPT_##ID, frontendFlags);
#include "nextcode/Option/Options.inc"
#undef OPTION
  return nextcode::c_string_utils::create_set(frontendFlags);
}

nextcodescan_string_set_t *
nextcodescan_compiler_supported_features_query() {
  std::vector<std::string> allFeatures;
  allFeatures.emplace_back("library-level");
  allFeatures.emplace_back("emit-abi-descriptor");
  return nextcode::c_string_utils::create_set(allFeatures);
}

//=== Scanner Diagnostics -------------------------------------------------===//
nextcodescan_diagnostic_set_t*
nextcodescan_scanner_diagnostics_query(nextcodescan_scanner_t scanner) {
  DependencyScanningTool *ScanningTool = unwrap(scanner);
  auto Diagnostics = ScanningTool->getDiagnostics();
  auto NumDiagnostics = Diagnostics.size();

  nextcodescan_diagnostic_set_t *Result = new nextcodescan_diagnostic_set_t;
  Result->count = NumDiagnostics;
  Result->diagnostics = new nextcodescan_diagnostic_info_t[NumDiagnostics];
  
  for (size_t i = 0; i < NumDiagnostics; ++i) {
    const auto &Diagnostic = Diagnostics[i];
    nextcodescan_diagnostic_info_s *DiagnosticInfo = new nextcodescan_diagnostic_info_s;
    DiagnosticInfo->message = nextcode::c_string_utils::create_clone(Diagnostic.Message.c_str());
    switch (Diagnostic.Severity) {
    case llvm::SourceMgr::DK_Error:
      DiagnosticInfo->severity = NEXTCODESCAN_DIAGNOSTIC_SEVERITY_ERROR;
      break;
    case llvm::SourceMgr::DK_Warning:
      DiagnosticInfo->severity = NEXTCODESCAN_DIAGNOSTIC_SEVERITY_WARNING;
      break;
    case llvm::SourceMgr::DK_Note:
      DiagnosticInfo->severity = NEXTCODESCAN_DIAGNOSTIC_SEVERITY_NOTE;
      break;
    case llvm::SourceMgr::DK_Remark:
      DiagnosticInfo->severity = NEXTCODESCAN_DIAGNOSTIC_SEVERITY_REMARK;
      break;
    }
    // nextcodescan_scanner_diagnostics_query is deprecated,
    // so it does not support source locations.
    DiagnosticInfo->source_location = nullptr;
    Result->diagnostics[i] = DiagnosticInfo;
  }

  return Result;
}

void
nextcodescan_scanner_diagnostics_reset(nextcodescan_scanner_t scanner) {
  DependencyScanningTool *ScanningTool = unwrap(scanner);
  ScanningTool->resetDiagnostics();
}

nextcodescan_string_ref_t
nextcodescan_diagnostic_get_message(nextcodescan_diagnostic_info_t diagnostic) {
  return diagnostic->message;
}

nextcodescan_diagnostic_severity_t
nextcodescan_diagnostic_get_severity(nextcodescan_diagnostic_info_t diagnostic) {
  return diagnostic->severity;
}

nextcodescan_source_location_t
nextcodescan_diagnostic_get_source_location(nextcodescan_diagnostic_info_t diagnostic) {
  return diagnostic->source_location;
}

void nextcodescan_diagnostic_dispose(nextcodescan_diagnostic_info_t diagnostic) {
  nextcodescan_string_dispose(diagnostic->message);
  if (diagnostic->source_location) {
    nextcodescan_string_dispose(diagnostic->source_location->buffer_identifier);
    delete diagnostic->source_location;
  }
  delete diagnostic;
}

void
nextcodescan_diagnostics_set_dispose(nextcodescan_diagnostic_set_t* diagnostics){
  for (size_t i = 0; i < diagnostics->count; ++i) {
    nextcodescan_diagnostic_dispose(diagnostics->diagnostics[i]);
  }
  delete[] diagnostics->diagnostics;
  delete diagnostics;
}

//=== Source Location -----------------------------------------------------===//

nextcodescan_string_ref_t
nextcodescan_source_location_get_buffer_identifier(nextcodescan_source_location_t source_location) {
  return source_location->buffer_identifier;
}

int64_t
nextcodescan_source_location_get_line_number(nextcodescan_source_location_t source_location) {
  return source_location->line_number;
}

int64_t
nextcodescan_source_location_get_column_number(nextcodescan_source_location_t source_location) {
  return source_location->column_number;
}

//=== Experimental Compiler Invocation Functions ------------------------===//

int invoke_nextcode_compiler(int argc, const char **argv) {
  return nextcode::mainEntry(argc, argv);
}
