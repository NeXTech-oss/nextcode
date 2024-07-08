# Use an 'internal' name, for now
set(NEXTCODE_SCAN_LIB_NAME "_InternalNeXTCodeStaticMirror")

set(LLVM_EXPORTED_SYMBOL_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/libStaticMirror.exports)

add_nextcode_host_library(libStaticMirror SHARED
  libStaticMirror.cpp
  c-include-check.c
  LLVM_LINK_COMPONENTS object support
)
if(NOT NEXTCODE_BUILT_STANDALONE AND NOT CMAKE_C_COMPILER_ID MATCHES Clang)
  add_dependencies(libStaticMirror clang)
endif()

add_dependencies(libStaticMirror
    nextcodeStaticMirror)

target_link_libraries(libStaticMirror PRIVATE
    nextcodeStaticMirror
    nextcodeRemoteInspection)

set_target_properties(libStaticMirror
    PROPERTIES
    OUTPUT_NAME ${NEXTCODE_SCAN_LIB_NAME})

add_llvm_symbol_exports(libStaticMirror ${LLVM_EXPORTED_SYMBOL_FILE})

# Adds -dead_strip option
add_link_opts(libStaticMirror)

add_dependencies(static-mirror-lib libStaticMirror)
nextcode_install_in_component(TARGETS libStaticMirror
  ARCHIVE DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}" COMPONENT static-mirror-lib
  LIBRARY DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}" COMPONENT static-mirror-lib
  RUNTIME DESTINATION "bin" COMPONENT static-mirror-lib)
nextcode_install_in_component(DIRECTORY "${NEXTCODE_MAIN_INCLUDE_DIR}/nextcode-c/StaticMirror/"
                           DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/nextcode/${NEXTCODE_SCAN_LIB_NAME}"
                           COMPONENT static-mirror-lib)
