add_nextcode_host_library(nextcodeOption STATIC
  Options.cpp
  SanitizerOptions.cpp)
add_dependencies(nextcodeOption
  NeXTCodeOptions)
target_link_libraries(nextcodeOption PRIVATE
  nextcodeBasic)

set(features_merger "${NEXTCODE_SOURCE_DIR}/utils/merge-features.py")
set(features_file_nextcode_src "${CMAKE_CURRENT_SOURCE_DIR}/features.json")
set(features_file_clang_src "${LLVM_BINARY_DIR}/share/clang/features.json")
set(features_file_dest "${CMAKE_BINARY_DIR}/share/nextcode/features.json")

add_custom_command(
  OUTPUT
    ${features_file_dest}
  COMMAND
    $<TARGET_FILE:Python3::Interpreter> ${features_merger} -f ${features_file_nextcode_src} -p \"\" -f ${features_file_clang_src} -p clang- > ${features_file_dest}
  DEPENDS
    ${features_merger}
    ${features_file_nextcode_src}
    ${features_file_clang_src}
)

add_custom_target(nextcode-features-file DEPENDS ${features_file_dest})

add_dependencies(nextcodeOption nextcode-features-file)

nextcode_install_in_component(
  FILES
    ${features_file_dest}
  DESTINATION
    "share/nextcode"
  COMPONENT
    compiler
)

