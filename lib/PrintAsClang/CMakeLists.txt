set(datafiles
    _NeXTCodeCxxInteroperability.h
    _NeXTCodeStdlibCxxOverlay.h
    experimental-interoperability-version.json
)

set(NEXTCODELIB_DIR
    "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib/nextcode")
set(output_dir "${NEXTCODELIB_DIR}/nextcodeToCxx")

add_custom_command(
    OUTPUT "${output_dir}"
    COMMAND ${CMAKE_COMMAND} "-E" "make_directory" "${output_dir}")

set (outputs)

foreach(input ${datafiles})
  set(source "${CMAKE_CURRENT_SOURCE_DIR}/${input}")
  set(dest "${output_dir}/${input}")
  add_custom_command(OUTPUT
                       "${output_dir}/${input}"
                     DEPENDS
                       "${CMAKE_CURRENT_SOURCE_DIR}/${input}"
                     COMMAND
                       "${CMAKE_COMMAND}" "-E" "copy" "${source}" "${dest}")
  list(APPEND outputs "${output_dir}/${input}")
endforeach()
list(APPEND outputs "${output_dir}")

add_custom_target("copy_nextcodeToCxx_shims"
    DEPENDS "${output_dir}" "${outputs}"
    COMMENT "Copying NeXTCode to C++ shims to ${output_dir}")

nextcode_install_in_component(FILES ${datafiles}
                           DESTINATION "lib/nextcode/nextcodeToCxx"
                           COMPONENT compiler)

add_nextcode_host_library(nextcodePrintAsClang STATIC
  ClangSyntaxPrinter.cpp
  DeclAndTypePrinter.cpp
  ModuleContentsWriter.cpp
  PrimitiveTypeMapping.cpp
  PrintAsClang.cpp
  PrintClangClassType.cpp
  PrintClangFunction.cpp
  PrintClangValueType.cpp
  PrintNeXTCodeToClangCoreScaffold.cpp
  NeXTCodeToClangInteropContext.cpp)
target_link_libraries(nextcodePrintAsClang PRIVATE
  nextcodeAST
  nextcodeClangImporter
  nextcodeFrontend
  nextcodeIDE
  nextcodeIRGen)

add_dependencies(nextcodePrintAsClang
  "copy_nextcodeToCxx_shims")

set_nextcode_llvm_is_available(nextcodePrintAsClang)
