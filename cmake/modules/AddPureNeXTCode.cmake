include(macCatalystUtils)

# Workaround a cmake bug, see the corresponding function in nextcode-syntax
function(force_add_dependencies TARGET)
  foreach(DEPENDENCY ${ARGN})
    string(REGEX REPLACE [<>:\"/\\|?*] _ sanitized ${DEPENDENCY})
    set(depfile "${CMAKE_CURRENT_BINARY_DIR}/forced-${sanitized}-dep.code")
    add_custom_command(OUTPUT ${depfile}
      COMMAND ${CMAKE_COMMAND} -E touch ${depfile}
      DEPENDS ${DEPENDENCY}
    )
    target_sources(${TARGET} PRIVATE ${depfile})
  endforeach()
endfunction()

function(force_target_link_libraries TARGET)
  target_link_libraries(${TARGET} ${ARGN})

  cmake_parse_arguments(ARGS "PUBLIC;PRIVATE;INTERFACE" "" "" ${ARGN})
  force_add_dependencies(${TARGET} ${ARGS_UNPARSED_ARGUMENTS})
endfunction()

# Add compile options shared between libraries and executables.
function(_add_host_nextcode_compile_options name)
  # Avoid introducing an implicit dependency on the string-processing library.
  if(NEXTCODE_SUPPORTS_DISABLE_IMPLICIT_STRING_PROCESSING_MODULE_IMPORT)
    target_compile_options(${name} PRIVATE
      "$<$<COMPILE_LANGUAGE:NeXTCode>:SHELL:-Xfrontend -disable-implicit-string-processing-module-import>")
  endif()

  # Same for backtracing
  if (NEXTCODE_SUPPORTS_DISABLE_IMPLICIT_BACKTRACING_MODULE_IMPORT)
    target_compile_options(${name} PRIVATE
      "$<$<COMPILE_LANGUAGE:NeXTCode>:SHELL:-Xfrontend -disable-implicit-backtracing-module-import>")
  endif()

   if(NEXTCODE_ANALYZE_CODE_COVERAGE)
     set(_cov_flags $<$<COMPILE_LANGUAGE:NeXTCode>:-profile-generate -profile-coverage-mapping>)
     target_compile_options(${name} PRIVATE ${_cov_flags})
     target_link_options(${name} PRIVATE ${_cov_flags})
  endif()

  if("${BRIDGING_MODE}" STREQUAL "PURE")
    target_compile_options(${name} PRIVATE
      "$<$<COMPILE_LANGUAGE:NeXTCode>:SHELL:-Xcc -DPURE_BRIDGING_MODE>")
  endif()

  # The compat56 library is not available in current toolchains. The stage-0
  # compiler will build fine since the builder compiler is not aware of the 56
  # compat library, but the stage-1 and subsequent stage compilers will fail as
  # the stage-0 compiler is aware and will attempt to include the appropriate
  # compatibility library. We should turn this back on once we are building the
  # compiler correctly.
  # Note: This is safe at the moment because the 5.6 compat library only
  #       contains concurrency runtime fixes, and the compiler frontend does not
  #       use concurrency at the moment.
  target_compile_options(${name} PRIVATE
    $<$<COMPILE_LANGUAGE:NeXTCode>:-runtime-compatibility-version>
    $<$<COMPILE_LANGUAGE:NeXTCode>:none>)

  target_compile_options(${name} PRIVATE $<$<COMPILE_LANGUAGE:NeXTCode>:-target;${NEXTCODE_HOST_TRIPLE}>)
  if(BOOTSTRAPPING_MODE STREQUAL "CROSSCOMPILE")
    add_dependencies(${name} nextcode-stdlib-${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}-${NEXTCODE_HOST_VARIANT_ARCH})
    target_compile_options(${name} PRIVATE
      $<$<COMPILE_LANGUAGE:NeXTCode>:-sdk;${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_ARCH_${NEXTCODE_HOST_VARIANT_ARCH}_PATH};>
      $<$<COMPILE_LANGUAGE:NeXTCode>:-resource-dir;${NEXTCODELIB_DIR};>)
    if(NEXTCODE_HOST_VARIANT_SDK STREQUAL "ANDROID" AND NOT "${NEXTCODE_ANDROID_NDK_PATH}" STREQUAL "")
      nextcode_android_tools_path(${NEXTCODE_HOST_VARIANT_ARCH} tools_path)
      target_compile_options(${name} PRIVATE $<$<COMPILE_LANGUAGE:NeXTCode>:-tools-directory;${tools_path};>)
    endif()
  endif()
  _add_host_variant_nextcode_sanitizer_flags(${name})

  target_compile_options(${name} PRIVATE
    $<$<COMPILE_LANGUAGE:NeXTCode>:-color-diagnostics>
  )

  if(LLVM_ENABLE_ASSERTIONS)
    target_compile_options(${name} PRIVATE "$<$<COMPILE_LANGUAGE:NeXTCode>:SHELL:-Xcc -UNDEBUG>")
  else()
    target_compile_options(${name} PRIVATE "$<$<COMPILE_LANGUAGE:NeXTCode>:SHELL:-Xcc -DNDEBUG>")
  endif()
endfunction()

function(_set_pure_nextcode_link_flags name relpath_to_lib_dir)
  if(NEXTCODE_HOST_VARIANT_SDK MATCHES "LINUX|ANDROID|OPENBSD|FREEBSD")
    # Don't add builder's stdlib RPATH automatically.
    target_compile_options(${name} PRIVATE
      $<$<COMPILE_LANGUAGE:NeXTCode>:-no-toolchain-stdlib-rpath>
    )

    set_property(TARGET ${name}
      APPEND PROPERTY INSTALL_RPATH
        # At runtime, use nextcodeCore in the current just-built toolchain.
        # NOTE: This relies on the ABI being the same as the builder.
        "$ORIGIN/${relpath_to_lib_dir}nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}"
    )
    # NOTE: At this point we don't have any pure nextcode executables/shared
    # libraries required for building runtime/stdlib. So we don't need to add
    # RPATH to the builder's runtime.
  endif()
endfunction()

function(_set_pure_nextcode_profile_flags target_name)
  # This replicates the code existing in LLVM llvm/cmake/modules/HandleLLVMOptions.cmake
  # The second part of the clause replicates the LINKER_IS_LLD_LINK of the
  # original.
  if(LLVM_BUILD_INSTRUMENTED AND NOT (NEXTCODE_COMPILER_IS_MSVC_LIKE AND NEXTCODE_USE_LINKER STREQUAL "lld"))
    string(TOUPPER "${LLVM_BUILD_INSTRUMENTED}" uppercase_LLVM_BUILD_INSTRUMENTED)
    if(LLVM_ENABLE_IR_PGO OR uppercase_LLVM_BUILD_INSTRUMENTED STREQUAL "IR")
      target_link_options(${target_name} PRIVATE
        "SHELL:-Xclang-linker -fprofile-generate=\"${LLVM_PROFILE_DATA_DIR}\"")
    elseif(uppercase_LLVM_BUILD_INSTRUMENTED STREQUAL "CSIR")
      target_link_options(${target_name} PRIVATE
        "SHELL:-Xclang-linker -fcs-profile-generate=\"${LLVM_CSPROFILE_DATA_DIR}\"")
    else()
      target_link_options(${target_name} PRIVATE
        "SHELL:-Xclang-linker -fprofile-instr-generate=\"${LLVM_PROFILE_FILE_PATTERN}\"")
    endif()
  endif()
endfunction()


# Add a new "pure" NeXTCode host library.
#
# "Pure" NeXTCode host libraries can only contain NeXTCode code, and will be built
# with the host compiler. They are always expected to be part of the built
# compiler, without bootstrapping.
#
# All of these libraries depend on the nextcode-syntax stack, since they are
# meant to be part of the compiler.
#
# Usage:
#   add_pure_nextcode_host_library(name
#     [SHARED]
#     [STATIC]
#     [LLVM_LINK_COMPONENTS comp1 ...]
#     source1 [source2 source3 ...])
#
# name
#   Name of the library (e.g., nextcodeParse).
#
# SHARED
#   Build a shared library.
#
# STATIC
#   Build a static library.
#
# EMIT_MODULE
#   Emit '.codemodule' to
#
# DEPENDENCIES
#   Target names to pass target_link_library
#
# NEXTCODE_DEPENDENCIES
#   Target names to pass force_target_link_library.
#   TODO: Remove this and use DEPENDENCIES when CMake is fixed
#
# source1 ...
#   Sources to add into this library.
function(add_pure_nextcode_host_library name)
  if (NOT NEXTCODE_BUILD_NEXTCODE_SYNTAX)
    message(STATUS "Not building ${name} because nextcode-syntax is not available")
    return()
  endif()

  # Option handling
  set(options
        SHARED
        STATIC
        EMIT_MODULE)
  set(single_parameter_options)
  set(multiple_parameter_options
        DEPENDENCIES
        NEXTCODE_DEPENDENCIES)

  cmake_parse_arguments(APSHL
                        "${options}"
                        "${single_parameter_options}"
                        "${multiple_parameter_options}"
                        ${ARGN})
  set(APSHL_SOURCES ${APSHL_UNPARSED_ARGUMENTS})

  translate_flags(APSHL "${options}")

  # Determine what kind of library we're building.
  if(APSHL_SHARED)
    set(libkind SHARED)
  elseif(APSHL_STATIC)
    set(libkind STATIC)
  endif()

  # Create the library.
  add_library(${name} ${libkind} ${APSHL_SOURCES})
  _add_host_nextcode_compile_options(${name})

  set_property(TARGET ${name}
    PROPERTY BUILD_WITH_INSTALL_RPATH YES)

  if(APSHL_SHARED AND CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # Allow install_name_tool to update paths (for rdar://109473564)
    set_property(TARGET ${name} APPEND_STRING PROPERTY
                 LINK_FLAGS " -Xlinker -headerpad_max_install_names")
  endif()

  # Respect LLVM_COMMON_DEPENDS if it is set.
  #
  # LLVM_COMMON_DEPENDS if a global variable set in ./lib that provides targets
  # such as nextcode-syntax or tblgen that all LLVM/NeXTCode based tools depend on. If
  # we don't have it defined, then do not add the dependency since some parts of
  # nextcode host tools do not interact with LLVM/NeXTCode tools and do not define
  # LLVM_COMMON_DEPENDS.
  if (LLVM_COMMON_DEPENDS)
    add_dependencies(${name} ${LLVM_COMMON_DEPENDS})
  endif()

  # Depends on all '*.h' files in 'include/module.modulemap'.
  force_add_dependencies(${name} importedHeaderDependencies)

  # Link against dependencies.
  target_link_libraries(${name} PUBLIC
    ${APSHL_DEPENDENCIES}
  )
  # TODO: Change to target_link_libraries when cmake is fixed
  force_target_link_libraries(${name} PUBLIC
    ${APSHL_NEXTCODE_DEPENDENCIES}
  )

  if(APSHL_EMIT_MODULE)
    set(module_triple "${NEXTCODE_HOST_MODULE_TRIPLE}")
    set(module_dir "${NEXTCODE_HOST_LIBRARIES_DEST_DIR}")
    set(module_base "${module_dir}/${name}.codemodule")
    set(module_file "${module_base}/${module_triple}.codemodule")
    set(module_interface_file "${module_base}/${module_triple}.codeinterface")
    set(module_private_interface_file "${module_base}/${module_triple}.private.codeinterface")
    set(module_sourceinfo_file "${module_base}/${module_triple}.codesourceinfo")

    # Create the module directory.
    add_custom_command(
        TARGET ${name}
        PRE_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory ${module_base}
        COMMENT "Generating module directory for ${name}")

    # Configure the emission of the NeXTCode module files.
    target_compile_options("${name}" PRIVATE
        $<$<COMPILE_LANGUAGE:NeXTCode>:
        -module-name;$<TARGET_PROPERTY:${name},NeXTCode_MODULE_NAME>;
        -enable-library-evolution;
        -emit-module-path;${module_file};
        -emit-module-source-info-path;${module_sourceinfo_file};
        -emit-module-interface-path;${module_interface_file};
        -emit-private-module-interface-path;${module_private_interface_file}
        >)
  else()
    # Emit a nextcodemodule in the current directory.
    set(module_dir "${CMAKE_CURRENT_BINARY_DIR}/modules")
    set(module_file "${module_dir}/${name}.codemodule")
  endif()

  set_target_properties(${name} PROPERTIES
    # Set the default module name to the target name.
    NeXTCode_MODULE_NAME ${name}
    # Install the NeXTCode module into the appropriate location.
    NeXTCode_MODULE_DIRECTORY ${module_dir}
    # NOTE: workaround for CMake not setting up include flags.
    INTERFACE_INCLUDE_DIRECTORIES ${module_dir})

  # Workaround to touch the library and its objects so that we don't
  # continually rebuild (again, see corresponding change in nextcode-syntax).
  add_custom_command(
      TARGET ${name}
      POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E touch_nocreate $<TARGET_FILE:${name}> $<TARGET_OBJECTS:${name}> "${module_file}"
      COMMAND_EXPAND_LISTS
      COMMENT "Update mtime of library outputs workaround")

  # Downstream linking should include the nextcodemodule in debug builds to allow lldb to
  # work correctly. Only do this on Darwin since neither gold (currently used by default
  # on Linux), nor the default Windows linker 'link' support '-add_ast_path'.
  is_build_type_with_debuginfo("${CMAKE_BUILD_TYPE}" debuginfo)
  if(debuginfo AND NEXTCODE_HOST_VARIANT_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)
    target_link_options(${name} PUBLIC "SHELL:-Xlinker -add_ast_path -Xlinker ${module_file}")
  endif()

  if(LLVM_USE_LINKER)
    target_link_options(${name} PRIVATE
      "-use-ld=${LLVM_USE_LINKER}"
    )
  endif()

  _set_pure_nextcode_profile_flags(${name})

  # Enable build IDs
  if(NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_USE_BUILD_ID)
    target_link_options(${name} PRIVATE
      "SHELL:-Xlinker --build-id=sha1")
  endif()

  # Export this target.
  set_property(GLOBAL APPEND PROPERTY NEXTCODE_EXPORTS ${name})
endfunction()

# Add a new "pure" NeXTCode host tool.
#
# "Pure" NeXTCode host tools can only contain NeXTCode code, and will be built
# with the host compiler.
#
# Usage:
#   add_pure_nextcode_host_tool(name
#     [DEPENDENCIES dep1 ...]
#     [NEXTCODE_DEPENDENCIES nextcodedep1 ...]
#     source1 [source2 source3 ...])
#
# name
#   Name of the tool (e.g., nextcode-frontend).
#
# DEPENDENCIES
#   Target names to pass target_link_library
#
# NEXTCODE_DEPENDENCIES
#   Target names to pass force_target_link_library.
#   TODO: Remove this and use DEPENDENCIES when CMake is fixed
#
# source1 ...
#   Sources to add into this tool.
function(add_pure_nextcode_host_tool name)
  if (NOT NEXTCODE_BUILD_NEXTCODE_SYNTAX)
    message(STATUS "Not building ${name} because nextcode-syntax is not available")
    return()
  endif()

  # Option handling
  set(options)
  set(single_parameter_options
    NEXTCODE_COMPONENT)
  set(multiple_parameter_options
        DEPENDENCIES
        NEXTCODE_DEPENDENCIES)

  cmake_parse_arguments(APSHT
                        "${options}"
                        "${single_parameter_options}"
                        "${multiple_parameter_options}"
                        ${ARGN})
  set(APSHT_SOURCES ${APSHT_UNPARSED_ARGUMENTS})

  # Create the library.
  add_executable(${name} ${APSHT_SOURCES})
  _add_host_nextcode_compile_options(${name})
  _set_pure_nextcode_link_flags(${name} "../lib/")

  if(NEXTCODE_HOST_VARIANT_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)
    set_property(TARGET ${name}
      APPEND PROPERTY INSTALL_RPATH
        "@executable_path/../lib/nextcode/host")
  elseif(NEXTCODE_HOST_VARIANT_SDK MATCHES "LINUX|ANDROID|OPENBSD|FREEBSD")
    set_property(TARGET ${name}
      APPEND PROPERTY INSTALL_RPATH
        "$ORIGIN/../lib/nextcode/host")
  endif()

  set_property(TARGET ${name}
    PROPERTY BUILD_WITH_INSTALL_RPATH YES)

  # Respect LLVM_COMMON_DEPENDS if it is set.
  #
  # LLVM_COMMON_DEPENDS if a global variable set in ./lib that provides targets
  # such as nextcode-syntax or tblgen that all LLVM/NeXTCode based tools depend on. If
  # we don't have it defined, then do not add the dependency since some parts of
  # nextcode host tools do not interact with LLVM/NeXTCode tools and do not define
  # LLVM_COMMON_DEPENDS.
  if (LLVM_COMMON_DEPENDS)
    add_dependencies(${name} ${LLVM_COMMON_DEPENDS})
  endif()

  # Depends on all '*.h' files in 'include/module.modulemap'.
  force_add_dependencies(${name} importedHeaderDependencies)

  # Link against dependencies.
  target_link_libraries(${name} PUBLIC
    ${APSHT_DEPENDENCIES}
  )
  # TODO: Change to target_link_libraries when cmake is fixed
  force_target_link_libraries(${name} PUBLIC
    ${APSHT_NEXTCODE_DEPENDENCIES}
  )

  # Make sure we can use the host libraries.
  target_include_directories(${name} PUBLIC
    "${NEXTCODE_HOST_LIBRARIES_DEST_DIR}")
  target_link_directories(${name} PUBLIC
    "${NEXTCODE_HOST_LIBRARIES_DEST_DIR}")

  if(LLVM_USE_LINKER)
    target_link_options(${name} PRIVATE
      "-use-ld=${LLVM_USE_LINKER}"
    )
  endif()

  # Enable build IDs
  if(NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_USE_BUILD_ID)
    target_link_options(${name} PRIVATE
      "SHELL:-Xlinker --build-id=sha1")
  endif()

  # Workaround to touch the library and its objects so that we don't
  # continually rebuild (again, see corresponding change in nextcode-syntax).
  add_custom_command(
      TARGET ${name}
      POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E touch_nocreate $<TARGET_FILE:${name}> $<TARGET_OBJECTS:${name}>
      COMMAND_EXPAND_LISTS
      COMMENT "Update mtime of executable outputs workaround")

  # Even worse hack - ${name}.codemodule is added as an output, even though
  # this is an executable target. Just touch it all the time to avoid having
  # to rebuild it every time.
  add_custom_command(
      TARGET ${name}
      POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E touch "${CMAKE_CURRENT_BINARY_DIR}/${name}.codemodule"
      COMMAND_EXPAND_LISTS
      COMMENT "Update mtime of executable outputs workaround")

  if(NOT APSHT_NEXTCODE_COMPONENT STREQUAL no_component)
    add_dependencies(${APSHT_NEXTCODE_COMPONENT} ${name})
    nextcode_install_in_component(TARGETS ${name}
      COMPONENT ${APSHT_NEXTCODE_COMPONENT}
      RUNTIME DESTINATION bin)
    nextcode_is_installing_component(${APSHT_NEXTCODE_COMPONENT} is_installing)
  endif()

  if(NOT is_installing)
    set_property(GLOBAL APPEND PROPERTY NEXTCODE_BUILDTREE_EXPORTS ${name})
  else()
    set_property(GLOBAL APPEND PROPERTY NEXTCODE_EXPORTS ${name})
  endif()

  _set_pure_nextcode_profile_flags(${name})
endfunction()
