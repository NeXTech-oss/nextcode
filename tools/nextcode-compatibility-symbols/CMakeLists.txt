add_nextcode_host_tool(nextcode-compatibility-symbols
  nextcode-compatibility-symbols.cpp
  LLVM_LINK_COMPONENTS support
  NEXTCODE_COMPONENT tools
  DOES_NOT_USE_NEXTCODE
)

set(syms_file "${CMAKE_BINARY_DIR}/share/nextcode/compatibility-symbols")

add_custom_command_target(copy_compat_target
  COMMAND
    "${NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH}/nextcode-compatibility-symbols"
    --output-filename ${syms_file}
  OUTPUT
    ${syms_file}
  DEPENDS
    nextcode-compatibility-symbols
)

add_dependencies(nextcode-frontend "${copy_compat_target}")

nextcode_install_in_component(
  FILES
    ${syms_file}
  DESTINATION
    "share/nextcode"
  COMPONENT
    compiler
)
