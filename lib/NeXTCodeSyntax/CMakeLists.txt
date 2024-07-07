if(NOT NEXTCODE_BUILD_NEXTCODE_SYNTAX)
  return()
endif()
if(NOT EXISTS "${NEXTCODE_PATH_TO_NEXTCODE_SYNTAX_SOURCE}")
  message(SEND_ERROR "nextcode-syntax is required to build the NeXTCode compiler. Please run update-checkout or specify NEXTCODE_PATH_TO_NEXTCODE_SYNTAX_SOURCE")
  return()
endif()

# Build nextcode-syntax libraries with FetchContent.
set(CMAKE_NeXTCode_COMPILER_TARGET ${NEXTCODE_HOST_TRIPLE})
set(BUILD_SHARED_LIBS ON)
# All libraries in 'nextcode-syntax' goes to 'lib/nextcode/host'.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${NEXTCODE_HOST_LIBRARIES_DEST_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${NEXTCODE_HOST_LIBRARIES_DEST_DIR}")
if(NEXTCODE_HOST_VARIANT_SDK MATCHES "LINUX|ANDROID|OPENBSD|FREEBSD")
  set(NEXTCODE_HOST_LIBRARIES_RPATH "$ORIGIN;$ORIGIN/../${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}")
endif()
# Add unique ABI prefix to nextcode-syntax libraries so that compiler libraries (e.g. sourcekitdInProc)
# can be used from tools that has its own nextcode-syntax libraries as NeXTCodePM dependencies.
set(NEXTCODE_MODULE_ABI_NAME_PREFIX "Compiler")

file(TO_CMAKE_PATH "${NEXTCODE_PATH_TO_NEXTCODE_SYNTAX_SOURCE}" nextcode_syntax_path)
FetchContent_Declare(NeXTCodeSyntax SOURCE_DIR "${nextcode_syntax_path}")
FetchContent_MakeAvailable(NeXTCodeSyntax)

# Install nextcode-syntax libraries.
set(NEXTCODE_SYNTAX_MODULES
  NeXTCodeBasicFormat
  NeXTCodeParser
  NeXTCodeParserDiagnostics
  NeXTCodeDiagnostics
  NeXTCodeSyntax
  NeXTCodeOperators
  NeXTCodeSyntaxBuilder
  NeXTCodeSyntaxMacros
  NeXTCodeSyntaxMacroExpansion
  NeXTCodeCompilerPluginMessageHandling
  # Support for LSP
  NeXTCodeIDEUtils
  NeXTCodeRefactor
  # For nextcode-plugin-server
  NeXTCodeLibraryPluginProvider
)

# Install shared runtime libraries
if(CMAKE_SYSTEM_NAME MATCHES Windows)
  nextcode_install_in_component(TARGETS ${NEXTCODE_SYNTAX_MODULES}
                             RUNTIME
                             DESTINATION "bin"
                             COMPONENT nextcode-syntax-lib)
else()
  nextcode_install_in_component(TARGETS ${NEXTCODE_SYNTAX_MODULES}
                             LIBRARY
                             DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/nextcode/host"
                             COMPONENT nextcode-syntax-lib)
endif()

# Install import libraries in Windows.
if(CMAKE_SYSTEM_NAME MATCHES Windows)
  list(TRANSFORM NEXTCODE_SYNTAX_MODULES
       PREPEND "${CMAKE_IMPORT_LIBRARY_PREFIX}"
       OUTPUT_VARIABLE NEXTCODE_SYNTAX_IMPORT_LIBRARIES)
  list(TRANSFORM NEXTCODE_SYNTAX_IMPORT_LIBRARIES
       APPEND "${CMAKE_IMPORT_LIBRARY_SUFFIX}"
       OUTPUT_VARIABLE NEXTCODE_SYNTAX_IMPORT_LIBRARIES)
  list(TRANSFORM NEXTCODE_SYNTAX_IMPORT_LIBRARIES
       PREPEND "${NEXTCODE_HOST_LIBRARIES_DEST_DIR}/"
       OUTPUT_VARIABLE NEXTCODE_SYNTAX_IMPORT_LIBRARIES)
  nextcode_install_in_component(PROGRAMS "${NEXTCODE_SYNTAX_IMPORT_LIBRARIES}"
                             DESTINATION lib
                             COMPONENT nextcode-syntax-lib)
endif()

add_dependencies(nextcode-syntax-lib
  ${NEXTCODE_SYNTAX_MODULES})

if(BOOTSTRAPPING_MODE STREQUAL "CROSSCOMPILE")
    add_dependencies(nextcode-syntax-lib nextcode-stdlib-${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}-${NEXTCODE_HOST_VARIANT_ARCH})
endif()

# Install NeXTCode module interface files.
foreach(module ${NEXTCODE_SYNTAX_MODULES})
  set(module_dir "${module}.codemodule")
  set(module_file "${NEXTCODE_HOST_LIBRARIES_DEST_DIR}/${module_dir}/${NEXTCODE_HOST_MODULE_TRIPLE}")
  nextcode_install_in_component(FILES "${module_file}.codeinterface" "${module_file}.private.codeinterface"
                             DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/nextcode/host/${module_dir}"
                             COMPONENT nextcode-syntax-lib)
endforeach()

export(TARGETS ${NEXTCODE_SYNTAX_MODULES}
       NAMESPACE NeXTCodeSyntax::
       FILE ${CMAKE_BINARY_DIR}/cmake/modules/NeXTCodeSyntaxConfig.cmake
       EXPORT_LINK_INTERFACE_LIBRARIES)
