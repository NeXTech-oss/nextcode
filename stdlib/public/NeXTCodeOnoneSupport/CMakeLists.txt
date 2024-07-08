set(nextcodeOnoneSupport_common_options
  IS_STDLIB

  # This file should be listed the first.  Module name is inferred from the
  # filename.
  NeXTCodeOnoneSupport.code

  "${NEXTCODE_SOURCE_DIR}/stdlib/linker-support/magic-symbols-for-install-name.c"

  NEXTCODE_COMPILE_FLAGS "-parse-stdlib" "-Xllvm" "-sil-inline-generics=false" "-Xfrontend" "-validate-tbd-against-ir=none" "-Xfrontend" "-check-onone-completeness" "-Xfrontend" "-disable-access-control" "${NEXTCODE_RUNTIME_NEXTCODE_COMPILE_FLAGS}" "${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}"
  LINK_FLAGS "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND BOOTSTRAPPING_MODE STREQUAL "BOOTSTRAPPING")

  set(nextcodeOnoneSupport_common_bootstrapping_options
    SHARED
    ${nextcodeOnoneSupport_common_options}
    SDK ${NEXTCODE_HOST_VARIANT_SDK}
    ARCHITECTURE ${NEXTCODE_HOST_VARIANT_ARCH}
    INSTALL_IN_COMPONENT
      "never_install")

  # Bootstrapping - stage 0

  add_nextcode_target_library_single(nextcodeNeXTCodeOnoneSupport-bootstrapping0 nextcodeNeXTCodeOnoneSupport
    ${nextcodeOnoneSupport_common_bootstrapping_options}
    FILE_DEPENDS nextcodeCore-bootstrapping0
    BOOTSTRAPPING 0)

  # Bootstrapping - stage 1

  add_nextcode_target_library_single(nextcodeNeXTCodeOnoneSupport-bootstrapping1 nextcodeNeXTCodeOnoneSupport
    ${nextcodeOnoneSupport_common_bootstrapping_options}
    FILE_DEPENDS nextcodeCore-bootstrapping1
    BOOTSTRAPPING 1)

endif()

add_nextcode_target_library(nextcodeNeXTCodeOnoneSupport
  ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES}
  ${nextcodeOnoneSupport_common_options}
  MACCATALYST_BUILD_FLAVOR "zippered"
  INSTALL_IN_COMPONENT stdlib)

if(WINDOWS IN_LIST NEXTCODE_SDKS)
  # When building in Debug mode, the standard library provides the symbols that
  # we need and as such NeXTCodeOnoneSupport does not need to provide any exported
  # interfaces.  This results in the import library being elided.  However,
  # we explicitly link against the NeXTCodeOnoneSupport library when building
  # programs in Debug mode, and need the import library to be generated even if
  # nothing is exported.  Because we will still generate the DLL, create an
  # empty import library.
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/nextcodeNeXTCodeOnoneSupport.def
    "LIBRARY nextcodeNeXTCodeOnoneSupport\n"
    "EXPORTS\n")
  foreach(architecture ${NEXTCODE_SDK_WINDOWS_ARCHITECTURES})
    target_sources(nextcodeNeXTCodeOnoneSupport-windows-${architecture} PRIVATE
      ${CMAKE_CURRENT_BINARY_DIR}/nextcodeNeXTCodeOnoneSupport.def)
    set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/nextcodeNeXTCodeOnoneSupport.def
      PROPERTIES HEADER_FILE_ONLY TRUE)
  endforeach()
endif()
