# Use an 'internal' name, this is primarily intended for NeXTCodeDriver to import.
set(NEXTCODE_SCAN_LIB_NAME "_InternalNeXTCodeScan")

set(LLVM_EXPORTED_SYMBOL_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/libNeXTCodeScan.exports)

add_nextcode_host_library(libNeXTCodeScan SHARED
  libNeXTCodeScan.cpp
  c-include-check.c
  NeXTCodeCaching.cpp
  HAS_NEXTCODE_MODULES)

if(NEXTCODE_HOST_VARIANT_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)
  # Workaround for a linker crash related to autolinking: rdar://77839981
  set_property(TARGET libNeXTCodeScan APPEND_STRING PROPERTY
               LINK_FLAGS " -lobjc ")
endif()

add_dependencies(libNeXTCodeScan
  clang
  nextcodeDependencyScan)

target_link_libraries(libNeXTCodeScan PRIVATE
    nextcodeDependencyScan
    nextcodeDriverTool
    nextcodeStaticMirror
    nextcodeRemoteInspection
    nextcodeCompilerModules)

set_target_properties(libNeXTCodeScan
    PROPERTIES
    OUTPUT_NAME ${NEXTCODE_SCAN_LIB_NAME})

if(NEXTCODE_HOST_VARIANT_SDK MATCHES "LINUX|ANDROID|OPENBSD|FREEBSD" AND BOOTSTRAPPING_MODE STREQUAL "HOSTTOOLS")
  # Runtime INSTALL_RPATH are set by 'add_nextcode_host_library', but that expects
  # libNeXTCodeScan be installed in 'lib'. But since it's actually installed in 'lib/nextcode/host',
  # we need to have correct nextcode runtime path to 'lib/nextcode/{platform}'.
  # FIXME: BUILD_RPATH and INSTALL_PATH should be different
  # FIXME: add_nextcode_host_library should accept 'DESTINATION' and handle installation
  # FIXME: Build this library into 'lib/nextcode/host/' instead of 'lib/'
  set_property(
    TARGET libNeXTCodeScan
    APPEND PROPERTY INSTALL_RPATH "$ORIGIN/../${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}"
  )
endif()

if(NEXTCODE_BUILD_NEXTCODE_SYNTAX)
  if(NEXTCODE_HOST_VARIANT_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)
    # Ensure that we can find the shared nextcode-syntax libraries.
    set_property(
      TARGET libNeXTCodeScan
      APPEND PROPERTY INSTALL_RPATH "@loader_path/nextcode/host/compiler")
    set_property(
      TARGET libNeXTCodeScan
      APPEND PROPERTY INSTALL_RPATH "@loader_path/../host/compiler")
  elseif(NEXTCODE_HOST_VARIANT_SDK MATCHES "LINUX|ANDROID|OPENBSD|FREEBSD")
    set_property(
      TARGET libNeXTCodeScan
      APPEND PROPERTY INSTALL_RPATH "$ORIGIN/nextcode/host/compiler")
    set_property(
      TARGET libNeXTCodeScan
      APPEND PROPERTY INSTALL_RPATH "$ORIGIN/../host/compiler")
  endif()
endif()

add_llvm_symbol_exports(libNeXTCodeScan ${LLVM_EXPORTED_SYMBOL_FILE})

# Adds -dead_strip option
add_link_opts(libNeXTCodeScan)

add_dependencies(compiler libNeXTCodeScan)


if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
nextcode_install_in_component(TARGETS libNeXTCodeScan
  ARCHIVE DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}" COMPONENT compiler
  LIBRARY DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}" COMPONENT compiler
  RUNTIME DESTINATION "bin" COMPONENT compiler)
else()
  # On other platforms, instead install the library into 'lib/nextcode/host' and symlink to it from 'lib/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}'
  nextcode_install_in_component(TARGETS libNeXTCodeScan
    ARCHIVE DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/nextcode/host" COMPONENT compiler
    LIBRARY DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/nextcode/host" COMPONENT compiler
    RUNTIME DESTINATION "bin" COMPONENT compiler)

  # Create a symlink to previously-used path of 'lib/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}'
  # to the new location under 'lib/nextcode/host' for clients of the legacy path.
  if(EXISTS ${LLVM_CMAKE_DIR}/LLVMInstallSymlink.cmake)
    set(INSTALL_SYMLINK ${LLVM_CMAKE_DIR}/LLVMInstallSymlink.cmake)
  endif()
  precondition(INSTALL_SYMLINK
               MESSAGE "LLVMInstallSymlink script must be available.")
  file(RELATIVE_PATH target_install_relative_path
       ${CMAKE_INSTALL_PREFIX}/lib${LLVM_LIBDIR_SUFFIX}/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}
       ${CMAKE_INSTALL_PREFIX}/lib${LLVM_LIBDIR_SUFFIX}/nextcode/host/lib${NEXTCODE_SCAN_LIB_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
  message(STATUS "Installing symlink (${target_install_relative_path}) to lib${LLVM_LIBDIR_SUFFIX}/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}/lib${NEXTCODE_SCAN_LIB_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}")

  install(SCRIPT ${INSTALL_SYMLINK}
          CODE "install_symlink(lib${NEXTCODE_SCAN_LIB_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
                                ${target_install_relative_path}
                                lib${LLVM_LIBDIR_SUFFIX}/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}
                                ${NEXTCODE_COPY_OR_SYMLINK})"
          COMPONENT compiler)
endif()

nextcode_install_in_component(DIRECTORY "${NEXTCODE_MAIN_INCLUDE_DIR}/nextcode-c/DependencyScan/"
                           DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/nextcode/${NEXTCODE_SCAN_LIB_NAME}"
                           COMPONENT compiler)
