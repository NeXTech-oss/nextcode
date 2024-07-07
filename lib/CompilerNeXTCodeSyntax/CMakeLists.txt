if(NOT NEXTCODE_BUILD_NEXTCODE_SYNTAX)
  return()
endif()
if(NOT EXISTS "${NEXTCODE_PATH_TO_NEXTCODE_SYNTAX_SOURCE}")
  message(SEND_ERROR "nextcode-syntax is required to build the NeXTCode compiler. Please run update-checkout or specify NEXTCODE_PATH_TO_NEXTCODE_SYNTAX_SOURCE")
  return()
endif()

# Build nextcode-syntax libraries with FetchContent.
function(includeNeXTCodeSyntax)
  set(CMAKE_NeXTCode_COMPILER_TARGET ${NEXTCODE_HOST_TRIPLE})
  set(BUILD_SHARED_LIBS ON)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${NEXTCODE_HOST_LIBRARIES_DEST_DIR}/compiler")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${NEXTCODE_HOST_LIBRARIES_DEST_DIR}/compiler")
  if(NEXTCODE_HOST_VARIANT_SDK MATCHES "LINUX|ANDROID|OPENBSD|FREEBSD")
    set(NEXTCODE_HOST_LIBRARIES_RPATH "$ORIGIN;$ORIGIN/../../${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}")
  endif()

  # Add unique ABI prefix to nextcode-syntax libraries so that compiler libraries (e.g. sourcekitdInProc)
  # can be used from tools that has its own nextcode-syntax libraries as NeXTCodePM dependencies.
  set(NEXTCODE_MODULE_ABI_NAME_PREFIX "_Compiler")
  set(NEXTCODESYNTAX_TARGET_NAMESPACE "_Compiler")
  set(NEXTCODESYNTAX_EMIT_MODULE OFF)

  file(TO_CMAKE_PATH "${NEXTCODE_PATH_TO_NEXTCODE_SYNTAX_SOURCE}" nextcode_syntax_path)
  FetchContent_Declare(CompilerNeXTCodeSyntax SOURCE_DIR "${nextcode_syntax_path}")
  FetchContent_MakeAvailable(CompilerNeXTCodeSyntax)
endfunction()
includeNeXTCodeSyntax()

set(compiler_nextcodesyntax_libs
  _CompilerNeXTCodeSyntax
  _CompilerNeXTCodeOperators
  _CompilerNeXTCodeSyntaxBuilder
  _CompilerNeXTCodeParser
  _CompilerNeXTCodeParserDiagnostics
  _CompilerNeXTCodeCompilerPluginMessageHandling
  _CompilerNeXTCodeSyntaxMacroExpansion
  _CompilerNeXTCodeSyntaxMacros
  _CompilerNeXTCodeBasicFormat
  _CompilerNeXTCodeDiagnostics
  _CompilerNeXTCodeIDEUtils
)

foreach(lib ${compiler_nextcodesyntax_libs})
  target_compile_options(${lib} PRIVATE "SHELL:-module-link-name ${lib}")
endforeach()

nextcode_install_in_component(TARGETS ${compiler_nextcodesyntax_libs}
  ARCHIVE DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/nextcode/host/compiler" COMPONENT compiler
  LIBRARY DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/nextcode/host/compiler" COMPONENT compiler
  RUNTIME DESTINATION "bin" COMPONENT compiler)
