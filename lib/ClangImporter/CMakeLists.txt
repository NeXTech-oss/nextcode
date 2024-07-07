set(NEXTCODE_GYB_FLAGS
    "-DCFDatabaseFile=${NEXTCODE_SOURCE_DIR}/lib/ClangImporter/CFDatabase.def")

add_gyb_target(generated_sorted_cf_database
    SortedCFDatabase.def.gyb
    DEPENDS "${NEXTCODE_SOURCE_DIR}/lib/ClangImporter/CFDatabase.def")

add_nextcode_host_library(nextcodeClangImporter STATIC
  CFTypeInfo.cpp
  ClangAdapter.cpp
  ClangClassTemplateNamePrinter.cpp
  ClangDerivedConformances.cpp
  ClangDiagnosticConsumer.cpp
  ClangImporter.cpp
  ClangImporterRequests.cpp
  ClangIncludePaths.cpp
  ClangModuleDependencyScanner.cpp
  ClangSourceBufferImporter.cpp
  NeXTCodeDeclSynthesizer.cpp
  DWARFImporter.cpp
  ImportDecl.cpp
  ImportEnumInfo.cpp
  ImportMacro.cpp
  ImportName.cpp
  ImportType.cpp
  Serializability.cpp
  NeXTCodeLookupTable.cpp
)
target_link_libraries(nextcodeClangImporter PRIVATE
  nextcodeAST
  nextcodeParse
  clangTooling
  LLVMBitstreamReader)

target_link_libraries(nextcodeClangImporter INTERFACE
  clangDependencyScanning)

# This property is only set by calls to clang_tablegen. It will not be set on
# standalone builds, so it can always be safely passed.
get_property(CLANG_TABLEGEN_TARGETS GLOBAL PROPERTY CLANG_TABLEGEN_TARGETS)
add_dependencies(nextcodeClangImporter
  generated_sorted_cf_database
  ${CLANG_TABLEGEN_TARGETS})

set_nextcode_llvm_is_available(nextcodeClangImporter)

if(NEXTCODE_ENABLE_CXX_INTEROP_NEXTCODE_BRIDGING_HEADER)
  add_subdirectory(NeXTCodeBridging)
endif()
