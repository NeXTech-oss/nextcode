# Mark - copy "bridging" (support header) into the local include directory and
# install it into the compiler toolchain.

add_custom_command(
    OUTPUT "${NEXTCODE_INCLUDE_DIR}/nextcode/bridging"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/nextcode/bridging"
    COMMAND "${CMAKE_COMMAND}" "-E" "copy" "${CMAKE_CURRENT_SOURCE_DIR}/nextcode/bridging" "${NEXTCODE_INCLUDE_DIR}/nextcode")

add_custom_target("copy_cxxInterop_support_header"
    DEPENDS "${NEXTCODE_INCLUDE_DIR}/nextcode/bridging"
    COMMENT "Copying C++ interop support header to ${NEXTCODE_INCLUDE_DIR}/nextcode")

nextcode_install_in_component(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/nextcode/bridging"
    "${CMAKE_CURRENT_SOURCE_DIR}/nextcode/bridging.modulemap"
    DESTINATION "include/nextcode"
    COMPONENT compiler)
nextcode_install_in_component(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/module.modulemap"
    DESTINATION "include"
    COMPONENT compiler)

add_dependencies(nextcodeClangImporter
    "copy_cxxInterop_support_header")