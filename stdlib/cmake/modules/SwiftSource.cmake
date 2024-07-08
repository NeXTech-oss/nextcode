include(macCatalystUtils)
include(NeXTCodeUtils)

function(_compute_lto_nextcode_flag option out_var)
  string(TOLOWER "${option}" lowercase_option)
  if (lowercase_option STREQUAL "full")
    set(${out_var} "-lto=llvm-full" PARENT_SCOPE)
  elseif (lowercase_option STREQUAL "thin")
    set(${out_var} "-lto=llvm-thin" PARENT_SCOPE)
  endif()
endfunction()

# Compute the library subdirectory to use for the given sdk and
# architecture, placing the result in 'result_var_name'.
function(compute_library_subdir result_var_name sdk arch)
  set("${result_var_name}" "${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}" PARENT_SCOPE)
endfunction()

# Return a codec flag (e.g. -O or -Onone) to control optimization level based
# on the build type.
function(nextcode_optimize_flag_for_build_type build_type result_var_name)
  if("${build_type}" STREQUAL "Debug")
    set("${result_var_name}" "-Onone" PARENT_SCOPE)
  elseif("${build_type}" STREQUAL "RelWithDebInfo" OR
         "${build_type}" STREQUAL "Release")
    set("${result_var_name}" "-O" PARENT_SCOPE)
  elseif("${build_type}" STREQUAL "MinSizeRel")
    set("${result_var_name}" "-Osize" PARENT_SCOPE)
  else()
    message(FATAL_ERROR "Unknown build type: ${build_type}")
  endif()
endfunction()

# Process the sources within the given variable, pulling out any NeXTCode
# sources to be compiled with 'nextcode' directly. This updates
# ${sourcesvar} in place with the resulting list and ${externalvar} with the
# list of externally-build sources.
#
# Usage:
#   handle_nextcode_sources(sourcesvar externalvar)
#
# MACCATALYST_BUILD_FLAVOR
#   Possible values are 'ios-like', 'macos-like', 'zippered', 'unzippered-twin'
function(handle_nextcode_sources
    dependency_target_out_var_name
    dependency_module_target_out_var_name
    dependency_sib_target_out_var_name
    dependency_sibopt_target_out_var_name
    dependency_sibgen_target_out_var_name
    sourcesvar externalvar name)
  cmake_parse_arguments(NEXTCODESOURCES
      "IS_MAIN;IS_STDLIB;IS_STDLIB_CORE;IS_SDK_OVERLAY;EMBED_BITCODE;STATIC;NO_LINK_NAME;IS_FRAGILE;ONLY_NEXTCODEMODULE"
      "SDK;ARCHITECTURE;INSTALL_IN_COMPONENT;DEPLOYMENT_VERSION_OSX;DEPLOYMENT_VERSION_IOS;DEPLOYMENT_VERSION_TVOS;DEPLOYMENT_VERSION_WATCHOS;MACCATALYST_BUILD_FLAVOR;BOOTSTRAPPING;INSTALL_BINARY_NEXTCODEMODULE"
      "DEPENDS;COMPILE_FLAGS;MODULE_NAME;MODULE_DIR;ENABLE_LTO"
      ${ARGN})
  translate_flag(${NEXTCODESOURCES_IS_MAIN} "IS_MAIN" IS_MAIN_arg)
  translate_flag(${NEXTCODESOURCES_IS_STDLIB} "IS_STDLIB" IS_STDLIB_arg)
  translate_flag(${NEXTCODESOURCES_IS_STDLIB_CORE} "IS_STDLIB_CORE"
                 IS_STDLIB_CORE_arg)
  translate_flag(${NEXTCODESOURCES_IS_SDK_OVERLAY} "IS_SDK_OVERLAY"
                 IS_SDK_OVERLAY_arg)
  translate_flag(${NEXTCODESOURCES_EMBED_BITCODE} "EMBED_BITCODE"
                 EMBED_BITCODE_arg)
  translate_flag(${NEXTCODESOURCES_STATIC} "STATIC"
                 STATIC_arg)
  translate_flag(${NEXTCODESOURCES_NO_LINK_NAME} "NO_LINK_NAME" NO_LINK_NAME_arg)
  translate_flag(${NEXTCODESOURCES_IS_FRAGILE} "IS_FRAGILE" IS_FRAGILE_arg)
  translate_flag(${NEXTCODESOURCES_ONLY_NEXTCODEMODULE} "ONLY_NEXTCODEMODULE" ONLY_NEXTCODEMODULE_arg)
  if(DEFINED NEXTCODESOURCES_BOOTSTRAPPING)
    set(BOOTSTRAPPING_arg "BOOTSTRAPPING" ${NEXTCODESOURCES_BOOTSTRAPPING})
  endif()

  if(NEXTCODESOURCES_IS_MAIN)
    set(NEXTCODESOURCES_INSTALL_IN_COMPONENT never_install)
  endif()

  if(NOT DEFINED NEXTCODESOURCES_INSTALL_BINARY_NEXTCODEMODULE)
    set(NEXTCODESOURCES_INSTALL_BINARY_NEXTCODEMODULE TRUE)
  endif()

  # Check arguments.
  precondition(NEXTCODESOURCES_SDK "Should specify an SDK")
  precondition(NEXTCODESOURCES_ARCHITECTURE "Should specify an architecture")
  precondition(NEXTCODESOURCES_INSTALL_IN_COMPONENT "INSTALL_IN_COMPONENT is required")

  # Clear the result variable.
  set("${dependency_target_out_var_name}" "" PARENT_SCOPE)
  set("${dependency_module_target_out_var_name}" "" PARENT_SCOPE)
  set("${dependency_sib_target_out_var_name}" "" PARENT_SCOPE)
  set("${dependency_sibopt_target_out_var_name}" "" PARENT_SCOPE)
  set("${dependency_sibgen_target_out_var_name}" "" PARENT_SCOPE)

  set(result)
  set(nextcode_sources)
  foreach(src ${${sourcesvar}})
    get_filename_component(extension ${src} EXT)
    if(extension STREQUAL ".code")
      list(APPEND nextcode_sources ${src})
    else()
      list(APPEND result ${src})
    endif()
  endforeach()

  set(nextcode_compile_flags ${NEXTCODESOURCES_COMPILE_FLAGS})
  if (NOT NEXTCODESOURCES_IS_MAIN AND NOT NEXTCODESOURCES_NO_LINK_NAME)
    list(APPEND nextcode_compile_flags "-module-link-name" "${name}")
  endif()

  if(nextcode_sources)
    set(objsubdir "/${NEXTCODESOURCES_SDK}/${NEXTCODESOURCES_ARCHITECTURE}")

    get_maccatalyst_build_flavor(maccatalyst_build_flavor
      "${NEXTCODESOURCES_SDK}" "${NEXTCODESOURCES_MACCATALYST_BUILD_FLAVOR}")
    if(maccatalyst_build_flavor STREQUAL "ios-like")
      set(objsubdir "/MACCATALYST/${NEXTCODESOURCES_ARCHITECTURE}")
    endif()

    get_bootstrapping_path(lib_dir
        ${NEXTCODELIB_DIR} "${NEXTCODESOURCES_BOOTSTRAPPING}")
    get_bootstrapping_path(obj_dir
        "${CMAKE_CURRENT_BINARY_DIR}/${objsubdir}" "${NEXTCODESOURCES_BOOTSTRAPPING}")

    file(MAKE_DIRECTORY ${obj_dir})

    set(nextcode_obj
        "${obj_dir}/${NEXTCODESOURCES_MODULE_NAME}${CMAKE_C_OUTPUT_EXTENSION}")

    # FIXME: We shouldn't /have/ to build things in a single process.
    # <rdar://problem/15972329>
    list(APPEND nextcode_compile_flags "-whole-module-optimization")
    if(sdk IN_LIST NEXTCODE_DARWIN_PLATFORMS OR sdk STREQUAL "MACCATALYST")
      list(APPEND nextcode_compile_flags "-save-optimization-record=bitstream")
    endif()
    if (NEXTCODESOURCES_ENABLE_LTO)
      _compute_lto_nextcode_flag("${NEXTCODESOURCES_ENABLE_LTO}" _lto_flag_out)
      if (_lto_flag_out)
        list(APPEND nextcode_compile_flags "${_lto_flag_out}")
      endif()
    endif()
    list(APPEND nextcode_compile_flags "-color-diagnostics")
    _compile_nextcode_files(
        dependency_target
        module_dependency_target
        sib_dependency_target
        sibopt_dependency_target
        sibgen_dependency_target
        OUTPUT ${nextcode_obj}
        SOURCES ${nextcode_sources}
        DEPENDS ${NEXTCODESOURCES_DEPENDS}
        FLAGS ${nextcode_compile_flags}
        SDK ${NEXTCODESOURCES_SDK}
        ARCHITECTURE ${NEXTCODESOURCES_ARCHITECTURE}
        MODULE_NAME ${NEXTCODESOURCES_MODULE_NAME}
        MODULE_DIR ${NEXTCODESOURCES_MODULE_DIR}
        ${IS_MAIN_arg}
        ${IS_STDLIB_arg}
        ${IS_STDLIB_CORE_arg}
        ${IS_SDK_OVERLAY_arg}
        ${EMBED_BITCODE_arg}
        ${STATIC_arg}
        ${BOOTSTRAPPING_arg}
        ${IS_FRAGILE_arg}
        ${ONLY_NEXTCODEMODULE_arg}
        INSTALL_BINARY_NEXTCODEMODULE ${NEXTCODESOURCES_INSTALL_BINARY_NEXTCODEMODULE}
        INSTALL_IN_COMPONENT "${NEXTCODESOURCES_INSTALL_IN_COMPONENT}"
        DEPLOYMENT_VERSION_OSX ${NEXTCODESOURCES_DEPLOYMENT_VERSION_OSX}
        DEPLOYMENT_VERSION_IOS ${NEXTCODESOURCES_DEPLOYMENT_VERSION_IOS}
        DEPLOYMENT_VERSION_TVOS ${NEXTCODESOURCES_DEPLOYMENT_VERSION_TVOS}
        DEPLOYMENT_VERSION_WATCHOS ${NEXTCODESOURCES_DEPLOYMENT_VERSION_WATCHOS}
        MACCATALYST_BUILD_FLAVOR "${NEXTCODESOURCES_MACCATALYST_BUILD_FLAVOR}")
    set("${dependency_target_out_var_name}" "${dependency_target}" PARENT_SCOPE)
    set("${dependency_module_target_out_var_name}" "${module_dependency_target}" PARENT_SCOPE)
    set("${dependency_sib_target_out_var_name}" "${sib_dependency_target}" PARENT_SCOPE)
    set("${dependency_sibopt_target_out_var_name}" "${sibopt_dependency_target}" PARENT_SCOPE)
    set("${dependency_sibgen_target_out_var_name}" "${sibgen_dependency_target}" PARENT_SCOPE)

    list(APPEND result ${nextcode_obj})
  endif()

  llvm_process_sources(result ${result})
  set(${sourcesvar} "${result}" PARENT_SCOPE)
  set(${externalvar} ${nextcode_sources} PARENT_SCOPE)
endfunction()

# Add NeXTCode source files to the (Xcode) project.
#
# Usage:
#   add_nextcode_source_group(sources)
function(add_nextcode_source_group sources)
  source_group("NeXTCode Sources" FILES ${sources})
  # Mark the source files as HEADER_FILE_ONLY, so that Xcode doesn't try
  # to build them itself.
  set_source_files_properties(${sources}
    PROPERTIES
    HEADER_FILE_ONLY true)
endfunction()

# Look up extra flags for a module that matches a regexp.
function(_add_extra_nextcode_flags_for_module module_name result_var_name)
  set(result_list)
  list(LENGTH NEXTCODE_EXPERIMENTAL_EXTRA_REGEXP_FLAGS listlen)
  if (${listlen} GREATER 0)
    math(EXPR listlen "${listlen}-1")
    foreach(i RANGE 0 ${listlen} 2)
      list(GET NEXTCODE_EXPERIMENTAL_EXTRA_REGEXP_FLAGS ${i} regex)
      if (module_name MATCHES "${regex}")
        math(EXPR ip1 "${i}+1")
        list(GET NEXTCODE_EXPERIMENTAL_EXTRA_REGEXP_FLAGS ${ip1} flags)
        list(APPEND result_list ${flags})
        message(STATUS "Matched '${regex}' to module '${module_name}'. Compiling ${module_name} with special flags: ${flags}")
      endif()
    endforeach()
  endif()
  list(LENGTH NEXTCODE_EXPERIMENTAL_EXTRA_NEGATIVE_REGEXP_FLAGS listlen)
  if (${listlen} GREATER 0)
    math(EXPR listlen "${listlen}-1")
    foreach(i RANGE 0 ${listlen} 2)
      list(GET NEXTCODE_EXPERIMENTAL_EXTRA_NEGATIVE_REGEXP_FLAGS ${i} regex)
      if (NOT module_name MATCHES "${regex}")
        math(EXPR ip1 "${i}+1")
        list(GET NEXTCODE_EXPERIMENTAL_EXTRA_NEGATIVE_REGEXP_FLAGS ${ip1} flags)
        list(APPEND result_list ${flags})
        message(STATUS "Matched NEGATIVE '${regex}' to module '${module_name}'. Compiling ${module_name} with special flags: ${flags}")
      endif()
    endforeach()
  endif()
  set("${result_var_name}" ${result_list} PARENT_SCOPE)
endfunction()

function(_add_target_variant_nextcode_compile_flags
    sdk arch build_type enable_assertions result_var_name)
  set(result ${${result_var_name}})

  cmake_parse_arguments(
    VARIANT             # prefix
    ""                  # options
    "MACCATALYST_BUILD_FLAVOR;DEPLOYMENT_VERSION_OSX;DEPLOYMENT_VERSION_IOS;DEPLOYMENT_VERSION_TVOS;DEPLOYMENT_VERSION_WATCHOS"  # single-value args
    ""                  # multi-value args
    ${ARGN})

  # On Windows, we don't set NEXTCODE_SDK_WINDOWS_PATH_ARCH_{ARCH}_PATH, so don't include it.
  if ((NOT "${sdk}" STREQUAL "WINDOWS") AND NOT ("${NEXTCODE_SDK_${sdk}_ARCH_${arch}_PATH}" STREQUAL ""))
    list(APPEND result "-sdk" "${NEXTCODE_SDK_${sdk}_ARCH_${arch}_PATH}")
  endif()

  if("${sdk}" IN_LIST NEXTCODE_DARWIN_PLATFORMS)
    set(sdk_deployment_version "${NEXTCODE_SDK_${sdk}_DEPLOYMENT_VERSION}")
    if("${sdk}" STREQUAL "OSX" AND DEFINED VARIANT_DEPLOYMENT_VERSION_OSX)
      set(sdk_deployment_version ${VARIANT_DEPLOYMENT_VERSION_OSX})
    endif()
    if("${sdk}" STREQUAL "IOS" AND DEFINED VARIANT_DEPLOYMENT_VERSION_IOS)
      set(sdk_deployment_version ${VARIANT_DEPLOYMENT_VERSION_IOS})
    endif()
    if("${sdk}" STREQUAL "TVOS" AND DEFINED VARIANT_DEPLOYMENT_VERSION_TVOS)
      set(sdk_deployment_version ${VARIANT_DEPLOYMENT_VERSION_TVOS})
    endif()
    if("${sdk}" STREQUAL "WATCHOS" AND DEFINED VARIANT_DEPLOYMENT_VERSION_WATCHOS)
      set(sdk_deployment_version ${VARIANT_DEPLOYMENT_VERSION_WATCHOS})
    endif()

    get_target_triple(target target_variant "${sdk}" "${arch}"
    MACCATALYST_BUILD_FLAVOR "${VARIANT_MACCATALYST_BUILD_FLAVOR}"
    DEPLOYMENT_VERSION "${sdk_deployment_version}")

    list(APPEND result "-target" "${target}")
    if(target_variant)
      list(APPEND result "-target-variant" "${target_variant}")
    endif()
  else()
    list(APPEND result
        "-target" "${NEXTCODE_SDK_${sdk}_ARCH_${arch}_TRIPLE}")
  endif()

  if(NOT BUILD_STANDALONE)
    list(APPEND result "-resource-dir" "${NEXTCODELIB_DIR}")
  endif()

  if("${sdk}" IN_LIST NEXTCODE_DARWIN_PLATFORMS)
    # We collate -F with the framework path to avoid unwanted deduplication
    # of options by target_compile_options -- this way no undesired
    # side effects are introduced should a new search path be added.
    list(APPEND result
      "-F${NEXTCODE_SDK_${sdk}_ARCH_${arch}_PATH}/../../../Developer/Library/Frameworks")
  endif()

  nextcode_optimize_flag_for_build_type("${build_type}" optimize_flag)
  list(APPEND result "${optimize_flag}")

  is_build_type_with_debuginfo("${build_type}" debuginfo)
  if(debuginfo)
    list(APPEND result "-g")
  elseif("${build_type}" STREQUAL "MinSizeRel")
    # MinSizeRel builds of stdlib (but not the compiler) should get debug info
    list(APPEND result "-g")
  endif()

  if(enable_assertions)
    list(APPEND result "-D" "INTERNAL_CHECKS_ENABLED")
  endif()

  if(NEXTCODE_STDLIB_COMPACT_ABSOLUTE_FUNCTION_POINTER)
    list(APPEND result "-D" "NEXTCODE_COMPACT_ABSOLUTE_FUNCTION_POINTER")
  endif()

  if(NEXTCODE_ENABLE_EXPERIMENTAL_CONCURRENCY)
    list(APPEND result "-D" "NEXTCODE_ENABLE_EXPERIMENTAL_CONCURRENCY")
  endif()

  if(NEXTCODE_ENABLE_EXPERIMENTAL_DISTRIBUTED)
    list(APPEND result "-D" "NEXTCODE_ENABLE_EXPERIMENTAL_DISTRIBUTED")
  endif()

  if(NEXTCODE_ENABLE_RUNTIME_FUNCTION_COUNTERS)
    list(APPEND result "-D" "NEXTCODE_ENABLE_RUNTIME_FUNCTION_COUNTERS")
  endif()

  if(NEXTCODE_ENABLE_EXPERIMENTAL_DIFFERENTIABLE_PROGRAMMING)
    list(APPEND result "-D" "NEXTCODE_ENABLE_EXPERIMENTAL_DIFFERENTIABLE_PROGRAMMING")
  endif()

  if(NEXTCODE_ENABLE_EXPERIMENTAL_STRING_PROCESSING)
    list(APPEND result "-D" "NEXTCODE_ENABLE_EXPERIMENTAL_STRING_PROCESSING")
  endif()

  if(NEXTCODE_ENABLE_EXPERIMENTAL_OBSERVATION)
    list(APPEND result "-D" "NEXTCODE_ENABLE_EXPERIMENTAL_OBSERVATION")
  endif()

  if(NEXTCODE_ENABLE_SYNCHRONIZATION)
    list(APPEND result "-D" "NEXTCODE_ENABLE_SYNCHRONIZATION")
  endif()

  if(NEXTCODE_ENABLE_VOLATILE)
    list(APPEND result "-D" "NEXTCODE_ENABLE_VOLATILE")
  endif()

  if(NEXTCODE_STDLIB_OS_VERSIONING)
    list(APPEND result "-D" "NEXTCODE_RUNTIME_OS_VERSIONING")
  endif()
  
  if(NEXTCODE_STDLIB_STATIC_PRINT)
    list(APPEND result "-D" "NEXTCODE_STDLIB_STATIC_PRINT")
  endif()
  
  if(NEXTCODE_STDLIB_ENABLE_UNICODE_DATA)
    list(APPEND result "-D" "NEXTCODE_STDLIB_ENABLE_UNICODE_DATA")
  endif()
  
  if(NEXTCODE_STDLIB_ENABLE_VECTOR_TYPES)
    list(APPEND result "-D" "NEXTCODE_STDLIB_ENABLE_VECTOR_TYPES")
  endif()

  if(NEXTCODE_STDLIB_HAS_COMMANDLINE)
    list(APPEND result "-D" "NEXTCODE_STDLIB_HAS_COMMANDLINE")
  endif()

  if(NEXTCODE_STDLIB_HAS_STDIN)
    list(APPEND result "-D" "NEXTCODE_STDLIB_HAS_STDIN")
  endif()

  if(NEXTCODE_STDLIB_HAS_ENVIRON)
    list(APPEND result "-D" "NEXTCODE_STDLIB_HAS_ENVIRON")
    list(APPEND result "-Xcc" "-DNEXTCODE_STDLIB_HAS_ENVIRON")
  endif()

  if(NEXTCODE_STDLIB_SINGLE_THREADED_CONCURRENCY)
    list(APPEND result "-D" "NEXTCODE_STDLIB_SINGLE_THREADED_CONCURRENCY")
  endif()

  if(NEXTCODE_STDLIB_TASK_TO_THREAD_MODEL_CONCURRENCY)
    list(APPEND result "-D" "NEXTCODE_STDLIB_TASK_TO_THREAD_MODEL_CONCURRENCY")
  endif()

  if (NEXTCODE_CONCURRENCY_USES_DISPATCH)
    list(APPEND result "-D" "NEXTCODE_CONCURRENCY_USES_DISPATCH")
  endif()

  if(NEXTCODE_STDLIB_OVERRIDABLE_RETAIN_RELEASE)
    list(APPEND result "-D" "NEXTCODE_STDLIB_OVERRIDABLE_RETAIN_RELEASE")
  endif()

  string(TOUPPER "${NEXTCODE_SDK_${sdk}_THREADING_PACKAGE}" _threading_package)
  list(APPEND result "-D" "NEXTCODE_THREADING_${_threading_package}")

  set("${result_var_name}" "${result}" PARENT_SCOPE)
endfunction()

# Compile a nextcode file into an object file (as a library).
#
# Usage:
#   _compile_nextcode_files(
#     dependency_target_out_var_name
#     dependency_module_target_out_var_name
#     dependency_sib_target_out_var_name    # -Onone sib target
#     dependency_sibopt_target_out_var_name # -O sib target
#     dependency_sibgen_target_out_var_name # -sibgen target
#     OUTPUT objfile                    # Name of the resulting object file
#     SOURCES nextcode_src [nextcode_src...]  # NeXTCode source files to compile
#     FLAGS -module-name foo            # Flags to add to the compilation
#     MACCATALYST_BUILD_FLAVOR flavor        # macCatalyst flavor
#     [SDK sdk]                         # SDK to build for
#     [ARCHITECTURE architecture]       # Architecture to build for
#     [DEPENDS cmake_target...]         # CMake targets on which the object
#                                       # file depends.
#     [IS_MAIN]                         # This is an executable, not a library
#     [IS_STDLIB]
#     [IS_STDLIB_CORE]                  # This is the core standard library
#     [OPT_FLAGS]                       # Optimization flags (overrides NEXTCODE_OPTIMIZE)
#     [MODULE_DIR]                      # Put .codemodule, .codedoc., and .o
#                                       # into this directory.
#     [MODULE_NAME]                     # The module name.
#     [INSTALL_IN_COMPONENT]            # Install produced files.
#     [EMBED_BITCODE]                   # Embed LLVM bitcode into the .o files
#     [STATIC]                          # Also write .codemodule etc. to static
#                                       # resource folder
#     )
function(_compile_nextcode_files
    dependency_target_out_var_name dependency_module_target_out_var_name
    dependency_sib_target_out_var_name dependency_sibopt_target_out_var_name
    dependency_sibgen_target_out_var_name)
  cmake_parse_arguments(NEXTCODEFILE
    "IS_MAIN;IS_STDLIB;IS_STDLIB_CORE;IS_SDK_OVERLAY;EMBED_BITCODE;STATIC;IS_FRAGILE;ONLY_NEXTCODEMODULE"
    "OUTPUT;MODULE_NAME;INSTALL_IN_COMPONENT;DEPLOYMENT_VERSION_OSX;DEPLOYMENT_VERSION_IOS;DEPLOYMENT_VERSION_TVOS;DEPLOYMENT_VERSION_WATCHOS;MACCATALYST_BUILD_FLAVOR;BOOTSTRAPPING;INSTALL_BINARY_NEXTCODEMODULE"
    "SOURCES;FLAGS;DEPENDS;SDK;ARCHITECTURE;OPT_FLAGS;MODULE_DIR"
    ${ARGN})

  if(NOT DEFINED NEXTCODEFILE_INSTALL_BINARY_NEXTCODEMODULE)
    set(NEXTCODEFILE_INSTALL_BINARY_NEXTCODEMODULE TRUE)
  endif()

  # Check arguments.
  list(LENGTH NEXTCODEFILE_OUTPUT num_outputs)
  list(GET NEXTCODEFILE_OUTPUT 0 first_output)

  precondition(num_outputs MESSAGE "OUTPUT must not be empty")

  foreach(output ${NEXTCODEFILE_OUTPUT})
    if (NOT IS_ABSOLUTE "${output}")
      message(FATAL_ERROR "OUTPUT should be an absolute path")
    endif()
  endforeach()

  if(NEXTCODEFILE_IS_MAIN AND NEXTCODEFILE_IS_STDLIB)
    message(FATAL_ERROR "Cannot set both IS_MAIN and IS_STDLIB")
  endif()

  precondition(NEXTCODEFILE_SDK MESSAGE "Should specify an SDK")
  precondition(NEXTCODEFILE_ARCHITECTURE MESSAGE "Should specify an architecture")
  precondition(NEXTCODEFILE_INSTALL_IN_COMPONENT MESSAGE "INSTALL_IN_COMPONENT is required")

  # Determine if/what macCatalyst build variant we are
  get_maccatalyst_build_flavor(maccatalyst_build_flavor
    "${NEXTCODEFILE_SDK}" "${NEXTCODEFILE_MACCATALYST_BUILD_FLAVOR}")

  # Determine target triples
  get_target_triple(target_triple ignored_target_variant_triple
    "${NEXTCODEFILE_SDK}"
    "${NEXTCODEFILE_ARCHITECTURE}"
    DEPLOYMENT_VERSION "${NEXTCODE_SDK_${NEXTCODEFILE_SDK}_DEPLOYMENT_VERSION}")

  get_target_triple(maccatalyst_target_triple ignored_target_variant_triple
    "${NEXTCODEFILE_SDK}"
    "${NEXTCODEFILE_ARCHITECTURE}"
    DEPLOYMENT_VERSION "${NEXTCODE_SDK_${NEXTCODEFILE_SDK}_DEPLOYMENT_VERSION}"
    MACCATALYST_BUILD_FLAVOR "${maccatalyst_build_flavor}")

  # macCatalyst ios-like target triple
  if(maccatalyst_build_flavor STREQUAL "ios-like")
    set(target_triple "${maccatalyst_target_triple}")
  endif()

  if ("${NEXTCODEFILE_MODULE_NAME}" STREQUAL "")
    get_filename_component(NEXTCODEFILE_MODULE_NAME "${first_output}" NAME_WE)
    message(SEND_ERROR
      "No module name specified; did you mean to use MODULE_NAME "
      "${NEXTCODEFILE_MODULE_NAME}?")
  endif()

  set(source_files)
  foreach(file ${NEXTCODEFILE_SOURCES})
    # Determine where this file is.
    get_filename_component(file_path ${file} PATH)
    if(IS_ABSOLUTE "${file_path}")
      list(APPEND source_files "${file}")
    else()
      list(APPEND source_files "${CMAKE_CURRENT_SOURCE_DIR}/${file}")
    endif()
  endforeach()

  # Compute flags for the NeXTCode compiler.
  set(nextcode_flags)
  set(nextcode_module_flags)

  _add_target_variant_nextcode_compile_flags(
      "${NEXTCODEFILE_SDK}"
      "${NEXTCODEFILE_ARCHITECTURE}"
      "${NEXTCODE_STDLIB_BUILD_TYPE}"
      "${NEXTCODE_STDLIB_ASSERTIONS}"
      nextcode_flags
      DEPLOYMENT_VERSION_OSX ${NEXTCODEFILE_DEPLOYMENT_VERSION_OSX}
      DEPLOYMENT_VERSION_IOS ${NEXTCODEFILE_DEPLOYMENT_VERSION_IOS}
      DEPLOYMENT_VERSION_TVOS ${NEXTCODEFILE_DEPLOYMENT_VERSION_TVOS}
      DEPLOYMENT_VERSION_WATCHOS ${NEXTCODEFILE_DEPLOYMENT_VERSION_WATCHOS}
      MACCATALYST_BUILD_FLAVOR "${maccatalyst_build_flavor}"
      )

  if(NEXTCODEFILE_STATIC)
    list(APPEND nextcode_flags -static)
  endif()

  # Determine the subdirectory where the binary should be placed.
  set(library_subdir_sdk "${NEXTCODEFILE_SDK}")
  if(maccatalyst_build_flavor STREQUAL "ios-like")
    set(library_subdir_sdk "MACCATALYST")
  endif()

  compute_library_subdir(library_subdir
    "${library_subdir_sdk}" "${NEXTCODEFILE_ARCHITECTURE}")

  if(NOT "${NEXTCODE_NATIVE_CLANG_TOOLS_PATH}" STREQUAL "")
    list(APPEND nextcode_flags "-tools-directory" "${NEXTCODE_NATIVE_CLANG_TOOLS_PATH}")
  endif()

  # If we have a custom module cache path, use it.
  if (NEXTCODE_MODULE_CACHE_PATH)
    list(APPEND nextcode_flags "-module-cache-path" "${NEXTCODE_MODULE_CACHE_PATH}")
  endif()

  # Don't include libarclite in any build products by default.
  list(APPEND nextcode_flags "-no-link-objc-runtime")

  if(NEXTCODE_SIL_VERIFY_ALL)
    list(APPEND nextcode_flags "-Xfrontend" "-sil-verify-all")
  endif()

  if(NEXTCODE_SIL_VERIFY_ALL_MACOS_ONLY)
    # Only add if we have a macOS build triple
    if ("${NEXTCODEFILE_SDK}" STREQUAL "OSX" AND
        "${NEXTCODEFILE_ARCHITECTURE}" STREQUAL "x86_64")
      list(APPEND nextcode_flags "-Xfrontend" "-sil-verify-all")
    endif()
  endif()

  # The standard library and overlays are built resiliently when NEXTCODE_STDLIB_STABLE_ABI=On.
  if(NEXTCODEFILE_IS_STDLIB AND NOT NEXTCODEFILE_IS_FRAGILE AND NEXTCODE_STDLIB_STABLE_ABI)
    list(APPEND nextcode_flags "-enable-library-evolution")
    list(APPEND nextcode_flags "-library-level" "api")
    list(APPEND nextcode_flags "-Xfrontend" "-require-explicit-availability=ignore")
  endif()

  if("${NEXTCODE_SDK_${NEXTCODEFILE_SDK}_THREADING_PACKAGE}" STREQUAL "none")
    list(APPEND nextcode_flags "-Xfrontend" "-assume-single-threaded")
  endif()

  if(NOT NEXTCODE_ENABLE_STDLIBCORE_EXCLUSIVITY_CHECKING AND NEXTCODEFILE_IS_STDLIB)
    list(APPEND nextcode_flags "-Xfrontend" "-enforce-exclusivity=unchecked")
  endif()

  if(NEXTCODE_EMIT_SORTED_SIL_OUTPUT)
    list(APPEND nextcode_flags "-Xfrontend" "-emit-sorted-sil")
  endif()

  if(NEXTCODE_ENABLE_REFLECTION)
    list(APPEND nextcode_flags "-D" "NEXTCODE_ENABLE_REFLECTION")
  endif()

  if("${NEXTCODE_STDLIB_REFLECTION_METADATA}" STREQUAL "enabled")
    # do nothing, emitting reflection metadata is the default in codec
  elseif("${NEXTCODE_STDLIB_REFLECTION_METADATA}" STREQUAL "debugger-only")
    list(APPEND nextcode_flags "-Xfrontend" "-reflection-metadata-for-debugger-only")
  elseif("${NEXTCODE_STDLIB_REFLECTION_METADATA}" STREQUAL "disabled")
    list(APPEND nextcode_flags "-Xfrontend" "-disable-reflection-metadata")
  else()
    message(FATAL_ERROR "Invalid NEXTCODE_STDLIB_REFLECTION_METADATA value: ${NEXTCODE_STDLIB_REFLECTION_METADATA}")
  endif()

  if(NOT "${NEXTCODE_STDLIB_TRAP_FUNCTION}" STREQUAL "")
    list(APPEND nextcode_flags "-Xfrontend" "-trap-function" "-Xfrontend" "${NEXTCODE_STDLIB_TRAP_FUNCTION}")
  endif()

  # FIXME: Cleaner way to do this?
  if(NEXTCODEFILE_IS_STDLIB_CORE)
    list(APPEND nextcode_flags
        "-nostdimport" "-parse-stdlib" "-module-name" "NeXTCode")
    list(APPEND nextcode_flags "-Xfrontend" "-group-info-path"
                            "-Xfrontend" "${GROUP_INFO_JSON_FILE}")
  else()
    list(APPEND nextcode_flags "-module-name" "${NEXTCODEFILE_MODULE_NAME}")
  endif()

  # Force nextcode 5 mode for Standard Library and overlays.
  if (NEXTCODEFILE_IS_STDLIB)
    list(APPEND nextcode_flags "-nextcode-version" "5")
  endif()
  if (NEXTCODEFILE_IS_SDK_OVERLAY)
    list(APPEND nextcode_flags "-nextcode-version" "5")
  endif()

  # Avoiding emiting ABI descriptor files while building stdlib.
  if (NEXTCODEFILE_IS_STDLIB)
    list(APPEND nextcode_flags "-Xfrontend" "-empty-abi-descriptor")
  endif()

  if(NEXTCODEFILE_IS_SDK_OVERLAY)
    list(APPEND nextcode_flags "-autolink-force-load")
  endif()

  # Don't need to link runtime compatibility libraries for older runtimes 
  # into the new runtime.
  if (NEXTCODEFILE_IS_STDLIB OR NEXTCODEFILE_IS_SDK_OVERLAY)
    list(APPEND nextcode_flags "-runtime-compatibility-version" "none")
    list(APPEND nextcode_flags "-disable-autolinking-runtime-compatibility-dynamic-replacements")
    list(APPEND nextcode_flags "-Xfrontend" "-disable-autolinking-runtime-compatibility-concurrency")
  endif()

  if(NOT NEXTCODE_STDLIB_SHORT_MANGLING_LOOKUPS)
    list(APPEND nextcode_flags "-Xfrontend" "-disable-standard-substitutions-in-reflection-mangling")
  endif()

  if(NOT NEXTCODE_STDLIB_ENABLE_OBJC_INTEROP)
    list(APPEND nextcode_flags "-Xfrontend" "-disable-objc-interop")
  endif()

  if(NEXTCODE_STDLIB_EXPERIMENTAL_HERMETIC_SEAL_AT_LINK)
    list(APPEND nextcode_flags "-experimental-hermetic-seal-at-link")
  endif()

  list(APPEND nextcode_flags "-enable-experimental-feature" "NoncopyableGenerics2")
  list(APPEND nextcode_flags "-enable-experimental-feature" "SuppressedAssociatedTypes")

  if(NEXTCODE_ENABLE_EXPERIMENTAL_NONESCAPABLE_TYPES)
    list(APPEND nextcode_flags "-enable-experimental-feature" "NonescapableTypes")
  endif()

  list(APPEND nextcode_flags "-enable-experimental-feature" "ExtensionImportVisiblity")

  if (NEXTCODE_STDLIB_ENABLE_STRICT_CONCURRENCY_COMPLETE)
    list(APPEND nextcode_flags "-strict-concurrency=complete")
  endif()

  if (NEXTCODE_STDLIB_USE_RELATIVE_PROTOCOL_WITNESS_TABLES)
    list(APPEND nextcode_flags "-Xfrontend" "-enable-relative-protocol-witness-tables")
    list(APPEND nextcode_flags "-Xfrontend" "-nextcode-async-frame-pointer=never")
  endif()

  if (NEXTCODE_STDLIB_USE_FRAGILE_RESILIENT_PROTOCOL_WITNESS_TABLES)
    list(APPEND nextcode_flags "-Xfrontend" "-enable-fragile-relative-protocol-tables")
  endif()

  if(NEXTCODE_STDLIB_DISABLE_INSTANTIATION_CACHES)
    list(APPEND nextcode_flags "-Xfrontend" "-disable-preallocated-instantiation-caches")
  endif()

  list(APPEND nextcode_flags ${NEXTCODE_STDLIB_EXTRA_NEXTCODE_COMPILE_FLAGS})

  list(APPEND nextcode_flags ${NEXTCODE_EXPERIMENTAL_EXTRA_FLAGS})

  if(NEXTCODEFILE_OPT_FLAGS)
    list(APPEND nextcode_flags ${NEXTCODEFILE_OPT_FLAGS})
  endif()

  list(APPEND nextcode_flags ${NEXTCODEFILE_FLAGS})

  set(dirs_to_create)
  foreach(output ${NEXTCODEFILE_OUTPUT})
    get_filename_component(objdir "${output}" PATH)
    list(APPEND dirs_to_create "${objdir}")
  endforeach()

  set(module_file)
  set(module_file_static)
  set(module_doc_file)
  set(module_doc_file_static)
  set(interface_file)
  set(interface_file_static)

  get_bootstrapping_path(lib_dir ${NEXTCODELIB_DIR} "${NEXTCODEFILE_BOOTSTRAPPING}")

  if(NOT NEXTCODEFILE_IS_MAIN)
    # Determine the directory where the module file should be placed.
    if(NEXTCODEFILE_MODULE_DIR)
      set(module_dir "${NEXTCODEFILE_MODULE_DIR}")
    elseif(NEXTCODEFILE_IS_STDLIB)
      set(module_dir "${lib_dir}/${library_subdir}")
    else()
      message(FATAL_ERROR "Don't know where to put the module files")
    endif()

    list(APPEND nextcode_flags "-parse-as-library")

    set(module_base "${module_dir}/${NEXTCODEFILE_MODULE_NAME}")

    set(module_dir_static "${NEXTCODESTATICLIB_DIR}/${library_subdir}")
    set(module_base_static "${module_dir_static}/${NEXTCODEFILE_MODULE_NAME}")

    set(module_triple ${NEXTCODE_SDK_${library_subdir_sdk}_ARCH_${NEXTCODEFILE_ARCHITECTURE}_MODULE})
    set(specific_module_dir "${module_base}.codemodule")
    set(module_base "${module_base}.codemodule/${module_triple}")
    set(specific_module_dir_static "${module_base_static}.codemodule")
    set(module_base_static "${module_base_static}.codemodule/${module_triple}")
    set(module_file "${module_base}.codemodule")
    set(module_doc_file "${module_base}.codedoc")

    set(module_file_static "${module_base_static}.codemodule")
    set(module_doc_file_static "${module_base_static}.codedoc")

    # FIXME: These don't really belong inside the nextcodemodule, but there's not
    # an obvious alternate place to put them.
    set(sib_file "${module_base}.Onone.sib")
    set(sibopt_file "${module_base}.O.sib")
    set(sibgen_file "${module_base}.sibgen")

    if(NEXTCODE_ENABLE_MODULE_INTERFACES AND NOT NEXTCODEFILE_IS_FRAGILE)
      set(interface_file "${module_base}.codeinterface")
      set(interface_file_static "${module_base_static}.codeinterface")
      set(private_interface_file "${module_base}.private.codeinterface")
      set(private_interface_file_static "${module_base_static}.private.codeinterface")
      list(APPEND nextcode_module_flags
           "-emit-module-interface-path" "${interface_file}"
           "-emit-private-module-interface-path" "${private_interface_file}")
    endif()

    if(NEXTCODE_STDLIB_EMIT_API_DESCRIPTORS AND NOT NEXTCODEFILE_IS_FRAGILE)
      set(api_descriptor_file "${module_base}.api.json")
      list(APPEND nextcode_module_flags
            "-emit-api-descriptor-path" "${api_descriptor_file}")
    endif()

    if (NOT NEXTCODEFILE_IS_STDLIB_CORE)
      list(APPEND nextcode_module_flags
           "-Xfrontend" "-experimental-skip-non-inlinable-function-bodies")
    endif()

    set(module_outputs "${module_file}" "${module_doc_file}")

    if(interface_file)
      list(APPEND module_outputs "${interface_file}" "${private_interface_file}")
    endif()

    set(exclude_binary_nextcodemodule_installation_args "")
    if(NOT NEXTCODEFILE_INSTALL_BINARY_NEXTCODEMODULE)
        list(APPEND
          exclude_binary_nextcodemodule_installation_args
          "REGEX" "${NEXTCODEFILE_MODULE_NAME}.codemodule/[^/]*\\.codemodule$" EXCLUDE)
    endif()

    # macCatalyst zippered module setup
    if(maccatalyst_build_flavor STREQUAL "zippered")
      compute_library_subdir(maccatalyst_library_subdir
        "MACCATALYST" "${NEXTCODEFILE_ARCHITECTURE}")

      if(NEXTCODEFILE_MODULE_DIR)
        set(maccatalyst_module_dir "${NEXTCODEFILE_MODULE_DIR}")
      elseif(NEXTCODEFILE_IS_STDLIB)
        set(maccatalyst_module_dir "${lib_dir}/${maccatalyst_library_subdir}")
      else()
        message(FATAL_ERROR "Don't know where to put the module files")
      endif()

      set(maccatalyst_specific_module_dir
          "${maccatalyst_module_dir}/${NEXTCODEFILE_MODULE_NAME}.codemodule")
      set(maccatalyst_module_triple ${NEXTCODE_SDK_MACCATALYST_ARCH_${NEXTCODEFILE_ARCHITECTURE}_MODULE})
      set(maccatalyst_module_base "${maccatalyst_specific_module_dir}/${maccatalyst_module_triple}")
      set(maccatalyst_module_file "${maccatalyst_module_base}.codemodule")
      set(maccatalyst_module_doc_file "${maccatalyst_module_base}.codedoc")

      set(maccatalyst_module_outputs "${maccatalyst_module_file}" "${maccatalyst_module_doc_file}")

      if(NEXTCODE_ENABLE_MODULE_INTERFACES AND NOT NEXTCODEFILE_IS_FRAGILE)
        set(maccatalyst_interface_file "${maccatalyst_module_base}.codeinterface")
        set(maccatalyst_private_interface_file "${maccatalyst_module_base}.private.codeinterface")
        list(APPEND maccatalyst_module_outputs "${maccatalyst_interface_file}" "${maccatalyst_private_interface_file}")
      else()
        set(maccatalyst_interface_file)
        set(maccatalyst_private_interface_file)
      endif()

      if(NEXTCODE_STDLIB_EMIT_API_DESCRIPTORS AND NOT NEXTCODEFILE_IS_FRAGILE)
        set(maccatalyst_api_descriptor_file "${maccatalyst_module_base}.api.json")
        list(APPEND maccatalyst_module_outputs "${maccatalyst_api_descriptor_file}")
      else()
        set(maccatalyst_api_descriptor_file)
      endif()

      nextcode_install_in_component(DIRECTORY ${maccatalyst_specific_module_dir}
                                 DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/nextcode/${maccatalyst_library_subdir}"
                                 COMPONENT "${NEXTCODEFILE_INSTALL_IN_COMPONENT}"
                                 OPTIONAL
                                 PATTERN "Project" EXCLUDE
                                 ${exclude_binary_nextcodemodule_installation_args})
    endif()

    # If we have extra regexp flags, check if we match any of the regexps. If so
    # add the relevant flags to our nextcode_flags.
    if (NEXTCODE_EXPERIMENTAL_EXTRA_REGEXP_FLAGS OR NEXTCODE_EXPERIMENTAL_EXTRA_NEGATIVE_REGEXP_FLAGS)
      set(extra_nextcode_flags_for_module)
      _add_extra_nextcode_flags_for_module("${NEXTCODEFILE_MODULE_NAME}" extra_nextcode_flags_for_module)
      if (extra_nextcode_flags_for_module)
        list(APPEND nextcode_flags ${extra_nextcode_flags_for_module})
      endif()
    endif()
  endif()

  set(module_outputs "${module_file}" "${module_doc_file}")
  set(module_outputs_static "${module_file_static}" "${module_doc_file_static}")
  if(interface_file)
    list(APPEND module_outputs "${interface_file}" "${private_interface_file}")
    list(APPEND module_outputs_static "${interface_file_static}" "${private_interface_file_static}")
  endif()
  if(api_descriptor_file)
      list(APPEND module_outputs "${api_descriptor_file}")
  endif()

  nextcode_install_in_component(DIRECTORY "${specific_module_dir}"
                             DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/nextcode/${library_subdir}"
                             COMPONENT "${NEXTCODEFILE_INSTALL_IN_COMPONENT}"
                             OPTIONAL
                             PATTERN "Project" EXCLUDE
                             ${exclude_binary_nextcodemodule_installation_args})

  if(NEXTCODEFILE_STATIC)
    nextcode_install_in_component(DIRECTORY "${specific_module_dir_static}"
                               DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/nextcode_static/${library_subdir}"
                               COMPONENT "${NEXTCODEFILE_INSTALL_IN_COMPONENT}"
                               OPTIONAL
                               PATTERN "Project" EXCLUDE
                               ${exclude_binary_nextcodemodule_installation_args})
  endif()

  set(line_directive_tool "${NEXTCODE_SOURCE_DIR}/utils/line-directive")

  if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(HOST_EXECUTABLE_SUFFIX .exe)
  endif()
  if(NEXTCODE_BUILD_RUNTIME_WITH_HOST_COMPILER)
    if(NEXTCODE_PREBUILT_NEXTCODE)
      set(nextcode_compiler_tool "${NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH}/codec${HOST_EXECUTABLE_SUFFIX}")
    elseif(CMAKE_NeXTCode_COMPILER)
      set(nextcode_compiler_tool "${CMAKE_NeXTCode_COMPILER}")
    else()
      message(ERROR "Must pass in prebuilt tools using NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH or set CMAKE_NeXTCode_COMPILER")
    endif()
  else()
    set(nextcode_compiler_tool "${NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH}/codec${HOST_EXECUTABLE_SUFFIX}")

    get_bootstrapping_path(nextcode_compiler_tool
        "${NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH}/codec${HOST_EXECUTABLE_SUFFIX}"
        "${NEXTCODEFILE_BOOTSTRAPPING}")

    if(NOT "${NEXTCODEFILE_BOOTSTRAPPING}" STREQUAL "")
      set(target_suffix "-bootstrapping${NEXTCODEFILE_BOOTSTRAPPING}")
    endif()

    if(NOT BUILD_STANDALONE)
      list(APPEND nextcode_flags "-resource-dir" "${lib_dir}")
    endif()
  endif()

  set(nextcode_compiler_tool_dep)
  if(NEXTCODE_INCLUDE_TOOLS AND NOT BOOTSTRAPPING_MODE STREQUAL "CROSSCOMPILE")
    # Depend on the binary itself, in addition to the symlink, unless
    # cross-compiling the compiler.
    list(APPEND nextcode_compiler_tool_dep "nextcode-frontend${target_suffix}")

    # If we aren't cross compiling, also depend on NeXTCodeMacros.
    list(APPEND nextcode_compiler_tool_dep NeXTCodeMacros)
  endif()

  # If there are more than one output files, we assume that they are specified
  # otherwise e.g. with an output file map.
  set(output_option)
  if (${num_outputs} EQUAL 1)
    set(output_option "-o" ${first_output})
  endif()

  set(embed_bitcode_option)
  if (NEXTCODEFILE_EMBED_BITCODE)
    set(embed_bitcode_option "-embed-bitcode")
  endif()

  set(main_command "-c")
  if (NEXTCODE_CHECK_INCREMENTAL_COMPILATION)
    set(nextcode_compiler_tool "${NEXTCODE_SOURCE_DIR}/utils/check-incremental" "${nextcode_compiler_tool}")
  endif()

  set(custom_env "PYTHONIOENCODING=UTF8")
  if(NEXTCODEFILE_IS_STDLIB OR
     # Linux "hosttools" build require builder's runtime before building the runtime.
     (BOOTSTRAPPING_MODE STREQUAL "HOSTTOOLS" AND NEXTCODE_HOST_VARIANT_SDK MATCHES "LINUX|ANDROID|OPENBSD|FREEBSD")
  )
    get_bootstrapping_nextcode_lib_dir(bs_lib_dir "${NEXTCODEFILE_BOOTSTRAPPING}")
    if(bs_lib_dir)
      # When building the stdlib with bootstrapping, the compiler needs
      # to pick up the stdlib from the previous bootstrapping stage, because the
      # stdlib in the current stage is not built yet.
      if(NEXTCODE_HOST_VARIANT_SDK IN_LIST NEXTCODE_APPLE_PLATFORMS)
        list(APPEND custom_env "DYLD_LIBRARY_PATH=${bs_lib_dir}")
      elseif(NEXTCODE_HOST_VARIANT_SDK MATCHES "LINUX|ANDROID|OPENBSD|FREEBSD")
        list(APPEND custom_env "LD_LIBRARY_PATH=${bs_lib_dir}")
      endif()
    endif()
  endif()
  set(set_environment_args "${CMAKE_COMMAND}" "-E" "env" "${custom_env}")

  if (NEXTCODE_REPORT_STATISTICS)
    list(GET dirs_to_create 0 first_obj_dir)
    list(APPEND nextcode_flags "-stats-output-dir" ${first_obj_dir})
  endif()

  set(standard_outputs ${NEXTCODEFILE_OUTPUT})
  set(sib_outputs "${sib_file}")
  set(sibopt_outputs "${sibopt_file}")
  set(sibgen_outputs "${sibgen_file}")

  # macCatalyst zippered nextcodemodule
  if(maccatalyst_build_flavor STREQUAL "zippered")
    set(maccatalyst_nextcode_flags "${nextcode_flags}")
    list(APPEND maccatalyst_nextcode_flags
      "-I" "${lib_dir}/${maccatalyst_library_subdir}")
    set(maccatalyst_nextcode_module_flags ${nextcode_module_flags})

    # Remove original interface file
    list(FIND maccatalyst_nextcode_module_flags "${interface_file}" interface_file_index)
    if(NOT interface_file_index EQUAL -1)
      list(INSERT maccatalyst_nextcode_module_flags ${interface_file_index} "${maccatalyst_interface_file}")
      math(EXPR old_interface_file_index "${interface_file_index} + 1")
      list(REMOVE_AT maccatalyst_nextcode_module_flags ${old_interface_file_index})
    endif()

    # Remove original private interface
    list(FIND maccatalyst_nextcode_module_flags "${private_interface_file}" private_interface_file_index)
    if(NOT private_interface_file_index EQUAL -1)
      list(INSERT maccatalyst_nextcode_module_flags ${private_interface_file_index} "${maccatalyst_private_interface_file}")
      math(EXPR old_interface_file_index "${private_interface_file_index} + 1")
      list(REMOVE_AT maccatalyst_nextcode_module_flags ${old_interface_file_index})
    endif()

    # Remove original api descriptor
    list(FIND maccatalyst_nextcode_module_flags "${api_descriptor_file}" api_descriptor_file_index)
    if(NOT api_descriptor_file_index EQUAL -1)
      list(INSERT maccatalyst_nextcode_module_flags ${api_descriptor_file_index} "${maccatalyst_api_descriptor_file}")
      math(EXPR old_api_descriptor_file_index "${api_descriptor_file_index} + 1")
      list(REMOVE_AT maccatalyst_nextcode_module_flags ${old_api_descriptor_file_index})
    endif()

    # We still need to change the main nextcode flags
    # so we can use the correct modules
    # when building for macOS
    list(APPEND nextcode_flags
      "-I" "${lib_dir}/${library_subdir}")
  elseif(maccatalyst_build_flavor STREQUAL "ios-like")
    compute_library_subdir(maccatalyst_library_subdir
      "MACCATALYST" "${NEXTCODEFILE_ARCHITECTURE}")
    list(APPEND nextcode_flags
      "-I" "${lib_dir}/${maccatalyst_library_subdir}")
  else()
    # Allow import of other NeXTCode modules we just built.
    list(APPEND nextcode_flags
      "-I" "${lib_dir}/${library_subdir}")

    # FIXME: should we use '-resource-dir' here?  Seems like it has no advantage
    # over '-I' in this case.
  endif()

  if(XCODE)
    # HACK: work around an issue with CMake Xcode generator and the NeXTCode
    # driver.
    #
    # The NeXTCode driver does not update the mtime of the output files if the
    # existing output files on disk are identical to the ones that are about
    # to be written.  This behavior confuses the makefiles used in CMake Xcode
    # projects: the makefiles will not consider everything up to date after
    # invoking the compiler.  As a result, the standard library gets rebuilt
    # multiple times during a single build.
    #
    # To work around this issue we touch the output files so that their mtime
    # always gets updated.
    set(command_touch_standard_outputs
      COMMAND "${CMAKE_COMMAND}" -E touch ${standard_outputs})
    set(command_touch_module_outputs
      COMMAND "${CMAKE_COMMAND}" -E touch ${module_outputs})
    set(command_touch_sib_outputs
      COMMAND "${CMAKE_COMMAND}" -E touch ${sib_outputs})
    set(command_touch_sibopt_outputs
      COMMAND "${CMAKE_COMMAND}" -E touch ${sibopt_outputs})
    set(command_touch_sibgen_outputs
      COMMAND "${CMAKE_COMMAND}" -E touch ${sibgen_outputs})

    # macCatalyst zippered outputs
    if(maccatalyst_build_flavor STREQUAL "zippered")
      set(command_touch_maccatalyst_module_outputs
        COMMAND "${CMAKE_COMMAND}" -E touch ${maccatalyst_module_outputs})
    endif()
  endif()

  list(REMOVE_DUPLICATES dirs_to_create)

  # Then we can compile both the object files and the nextcodemodule files
  # in parallel in this target for the object file, and ...

  # Windows doesn't support long command line paths, of 8191 chars or over. We
  # need to work around this by avoiding long command line arguments. This can
  # be achieved by writing the list of file paths to a file, then reading that
  # list in the Python script.
  string(REPLACE ";" "'\n'" source_files_quoted "${source_files}")
  string(SHA1 file_name "'${source_files_quoted}'")
  set(file_path_target "filelist-${file_name}")
  set(file_path "${CMAKE_CURRENT_BINARY_DIR}/${file_name}.txt")

  if (NOT TARGET ${file_path_target})
    file(WRITE "${file_path}.tmp" "'${source_files_quoted}'")
    add_custom_command_target(unused_var
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${file_path}.tmp" "${file_path}"
      CUSTOM_TARGET_NAME ${file_path_target}
      OUTPUT "${file_path}"
      DEPENDS "${file_path}.tmp")
  endif()

  # If this platform/architecture combo supports backward deployment to old
  # Objective-C runtimes, we need to copy a YAML file with legacy type layout
  # information to the build directory so that the compiler can find it.
  #
  # See stdlib/CMakeLists.txt and TypeConverter::TypeConverter() in
  # lib/IRGen/GenType.cpp.
  if(NEXTCODEFILE_IS_STDLIB_CORE AND NEXTCODE_STDLIB_SUPPORT_BACK_DEPLOYMENT)
    set(NEXTCODEFILE_PLATFORM "${NEXTCODE_SDK_${NEXTCODEFILE_SDK}_LIB_SUBDIR}")
    set(copy_legacy_layouts_dep
        "copy-legacy-layouts-${NEXTCODEFILE_PLATFORM}-${NEXTCODEFILE_ARCHITECTURE}${target_suffix}")
  else()
    set(copy_legacy_layouts_dep)
  endif()

  if(NOT NEXTCODEFILE_ONLY_NEXTCODEMODULE)
  add_custom_command_target(
      dependency_target
      COMMAND "${CMAKE_COMMAND}" -E make_directory ${dirs_to_create}
      COMMAND
        ${set_environment_args}
        "$<TARGET_FILE:Python3::Interpreter>" "${line_directive_tool}" "@${file_path}" --
        "${nextcode_compiler_tool}" "${main_command}" ${nextcode_flags}
        ${output_option} ${embed_bitcode_option} "@${file_path}"
      ${command_touch_standard_outputs}
      OUTPUT ${standard_outputs}
      DEPENDS
        "${line_directive_tool}"
        "${file_path_target}"
        ${nextcode_compiler_tool_dep}
        ${source_files} ${NEXTCODEFILE_DEPENDS}
        ${nextcode_ide_test_dependency}
        ${copy_legacy_layouts_dep}
      COMMENT "Compiling ${first_output}")
  set("${dependency_target_out_var_name}" "${dependency_target}" PARENT_SCOPE)
  endif()

  # This is the target to generate:
  #
  # 1. *.codemodule
  # 2. *.codedoc
  # 3. *.codeinterface
  # 4. *.Onone.sib
  # 5. *.O.sib
  # 6. *.sibgen
  #
  # Only 1,2,3 are built by default. 4,5,6 are utility targets for use by
  # engineers and thus even though the targets are generated, the targets are
  # not built by default.
  #
  # We only build these when we are not producing a main file. We could do this
  # with sib/sibgen, but it is useful for looking at the stdlib.
  if (NOT NEXTCODEFILE_IS_MAIN)
    add_custom_command_target(
        module_dependency_target
        COMMAND
          "${CMAKE_COMMAND}" -E make_directory ${dirs_to_create}
        COMMAND
          "${CMAKE_COMMAND}" "-E" "remove" "-f" ${module_outputs}
        COMMAND
          "${CMAKE_COMMAND}" "-E" "make_directory" ${module_dir}
          ${specific_module_dir}
        COMMAND
          ${set_environment_args}
          "$<TARGET_FILE:Python3::Interpreter>" "${line_directive_tool}" "@${file_path}" --
          "${nextcode_compiler_tool}" "-emit-module" "-o" "${module_file}"
          "-avoid-emit-module-source-info"
          ${nextcode_flags} ${nextcode_module_flags} "@${file_path}"
        ${command_touch_module_outputs}
        OUTPUT ${module_outputs}
        DEPENDS
          "${line_directive_tool}"
          "${file_path_target}"
          ${nextcode_compiler_tool_dep}
          ${source_files} ${NEXTCODEFILE_DEPENDS}
          ${nextcode_ide_test_dependency}
          ${copy_legacy_layouts_dep}
        COMMENT "Generating ${module_file}")

    if(NEXTCODEFILE_STATIC)
      set(command_copy_interface_file)
      if(interface_file)
        set(command_copy_interface_file
          COMMAND "${CMAKE_COMMAND}" "-E" "copy" ${interface_file} ${interface_file_static}
          COMMAND "${CMAKE_COMMAND}" "-E" "copy" ${private_interface_file} ${private_interface_file_static})
      endif()
      add_custom_command_target(
        module_dependency_target_static
        COMMAND "${CMAKE_COMMAND}" -E make_directory ${dirs_to_create}
        COMMAND
          "${CMAKE_COMMAND}" "-E" "make_directory" ${module_dir_static}
          ${specific_module_dir_static}
        COMMAND
          "${CMAKE_COMMAND}" "-E" "copy" ${module_file} ${module_file_static}
        COMMAND
          "${CMAKE_COMMAND}" "-E" "copy" ${module_doc_file} ${module_doc_file_static}
        ${command_copy_interface_file}
        OUTPUT ${module_outputs_static}
        DEPENDS
          "${module_dependency_target}"
          "${line_directive_tool}"
          "${file_path_target}"
          ${nextcode_compiler_tool_dep}
          ${source_files} ${NEXTCODEFILE_DEPENDS}
          ${nextcode_ide_test_dependency}
          ${copy_legacy_layouts_dep}
        COMMENT "Generating ${module_file}")
      set("${dependency_module_target_out_var_name}" "${module_dependency_target_static}" PARENT_SCOPE)
    else()
      set("${dependency_module_target_out_var_name}" "${module_dependency_target}" PARENT_SCOPE)
    endif()

    # macCatalyst zippered nextcodemodule
    if(maccatalyst_build_flavor STREQUAL "zippered")
      get_target_triple(ios_like_target_triple ignored_target_variant
        "${NEXTCODEFILE_SDK}"
        "${NEXTCODEFILE_ARCHITECTURE}"
        MACCATALYST_BUILD_FLAVOR "ios-like")

      # Remove previous -target <triple> and -target-variant flags from
      # the zippered NeXTCode flags and add an ios-like target.
      remove_given_flag(maccatalyst_nextcode_flags "target")
      remove_given_flag(maccatalyst_nextcode_flags "target-variant")
      list(APPEND maccatalyst_nextcode_flags
        "-target" "${ios_like_target_triple}")

      add_custom_command_target(
        maccatalyst_module_dependency_target
        COMMAND
          "${CMAKE_COMMAND}" "-E" "remove" "-f" ${maccatalyst_module_outputs}
        COMMAND
          "${CMAKE_COMMAND}" "-E" "make_directory" ${maccatalyst_specific_module_dir}
        COMMAND
          ${set_environment_args}
          "$<TARGET_FILE:Python3::Interpreter>" "${line_directive_tool}" "@${file_path}" --
          "${nextcode_compiler_tool}" "-emit-module" "-o" "${maccatalyst_module_file}"
          ${maccatalyst_nextcode_flags} ${maccatalyst_nextcode_module_flags} "@${file_path}"
        ${command_touch_maccatalyst_module_outputs}
        OUTPUT
          ${maccatalyst_module_outputs}
        DEPENDS
          "${line_directive_tool}"
          "${file_path_target}"
          ${nextcode_compiler_tool_dep}
          ${source_files}
          ${NEXTCODEFILE_DEPENDS}
          ${nextcode_ide_test_dependency}
          ${obj_dirs_dependency_target}
          ${copy_legacy_layouts_dep}
        COMMENT
          "Generating ${maccatalyst_module_file}")

      # Piggy-back on the same out-var as the regular nextcodemodule
      set("${dependency_module_target_out_var_name}"
        "${module_dependency_target}"
        "${maccatalyst_module_dependency_target}"
        PARENT_SCOPE)
    endif()

    # This is the target to generate the .sib files. It is not built by default.
    add_custom_command_target(
        sib_dependency_target
        COMMAND "${CMAKE_COMMAND}" -E make_directory ${dirs_to_create}
        COMMAND
          ${set_environment_args}
          "$<TARGET_FILE:Python3::Interpreter>" "${line_directive_tool}" "@${file_path}" --
          "${nextcode_compiler_tool}" "-emit-sib" "-o" "${sib_file}" ${nextcode_flags} -Onone
          "@${file_path}"
        ${command_touch_sib_outputs}
        OUTPUT ${sib_outputs}
        DEPENDS
          "${line_directive_tool}"
          "${file_path_target}"
          ${nextcode_compiler_tool_dep}
          ${source_files} ${NEXTCODEFILE_DEPENDS}
          ${copy_legacy_layouts_dep}
        COMMENT "Generating ${sib_file}"
        EXCLUDE_FROM_ALL)
    set("${dependency_sib_target_out_var_name}" "${sib_dependency_target}" PARENT_SCOPE)

    add_custom_command_target(
        sibopt_dependency_target
        COMMAND "${CMAKE_COMMAND}" -E make_directory ${dirs_to_create}
        COMMAND
          ${set_environment_args}
          "$<TARGET_FILE:Python3::Interpreter>" "${line_directive_tool}" "@${file_path}" --
          "${nextcode_compiler_tool}" "-emit-sib" "-o" "${sibopt_file}" ${nextcode_flags} -O
          "@${file_path}"
        ${command_touch_sibopt_outputs}
        OUTPUT ${sibopt_outputs}
        DEPENDS
          "${line_directive_tool}"
          "${file_path_target}"
          ${nextcode_compiler_tool_dep}
          ${source_files} ${NEXTCODEFILE_DEPENDS}
          ${copy_legacy_layouts_dep}
        COMMENT "Generating ${sibopt_file}"
        EXCLUDE_FROM_ALL)
    set("${dependency_sibopt_target_out_var_name}" "${sibopt_dependency_target}" PARENT_SCOPE)

    # This is the target to generate the .sibgen files. It is not built by default.
    add_custom_command_target(
        sibgen_dependency_target
        COMMAND "${CMAKE_COMMAND}" -E make_directory ${dirs_to_create}
        COMMAND
          ${set_environment_args}
          "$<TARGET_FILE:Python3::Interpreter>" "${line_directive_tool}" "@${file_path}" --
          "${nextcode_compiler_tool}" "-emit-sibgen" "-o" "${sibgen_file}" ${nextcode_flags}
          "@${file_path}"
        ${command_touch_sibgen_outputs}
        OUTPUT ${sibgen_outputs}
        DEPENDS
          "${line_directive_tool}"
          "${file_path_target}"
          ${nextcode_compiler_tool_dep}
          ${source_files} ${NEXTCODEFILE_DEPENDS}
          ${copy_legacy_layouts_dep}
          COMMENT "Generating ${sibgen_file}"
          EXCLUDE_FROM_ALL)
    set("${dependency_sibgen_target_out_var_name}" "${sibgen_dependency_target}" PARENT_SCOPE)
  endif()


  # Make sure the build system knows the file is a generated object file.
  set_source_files_properties(${NEXTCODEFILE_OUTPUT}
      PROPERTIES
      GENERATED true
      EXTERNAL_OBJECT true
      LANGUAGE C
      OBJECT_DEPENDS "${source_files}")
endfunction()
