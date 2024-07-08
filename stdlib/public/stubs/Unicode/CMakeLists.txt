
# Embedded NeXTCode Unicode library
if(NEXTCODE_SHOULD_BUILD_EMBEDDED_STDLIB)
  add_custom_target(embedded-unicode)
  add_dependencies(embedded-libraries embedded-unicode)

  foreach(entry ${EMBEDDED_STDLIB_TARGET_TRIPLES})
    string(REGEX REPLACE "[ \t]+" ";" list "${entry}")
    list(GET list 0 arch)
    list(GET list 1 mod)
    list(GET list 2 triple)

    if("${mod}" MATCHES "-windows-msvc$")
      continue()
    endif()

    if (NEXTCODE_HOST_VARIANT STREQUAL "linux")
      set(extra_c_compile_flags -ffreestanding)
    elseif (NEXTCODE_HOST_VARIANT STREQUAL "macosx")
      set(extra_c_compile_flags -D__MACH__ -D__APPLE__ -ffreestanding)
    endif()
    
    set(NEXTCODE_SDK_embedded_ARCH_${mod}_MODULE "${mod}")
    set(NEXTCODE_SDK_embedded_LIB_SUBDIR "embedded")
    set(NEXTCODE_SDK_embedded_ARCH_${mod}_TRIPLE "${triple}")

    add_nextcode_target_library_single(
      embedded-unicode-${mod}
      nextcodeUnicodeDataTables
      STATIC
      IS_FRAGILE

      UnicodeData.cpp
      UnicodeGrapheme.cpp
      UnicodeNormalization.cpp
      UnicodeScalarProps.cpp
      UnicodeWord.cpp

      C_COMPILE_FLAGS ${extra_c_compile_flags}
      MODULE_DIR "${CMAKE_BINARY_DIR}/lib/nextcode/embedded"
      SDK "embedded"
      ARCHITECTURE "${mod}"
      DEPENDS embedded-stdlib-${mod}
      INSTALL_IN_COMPONENT stdlib
      )
    nextcode_install_in_component(
      TARGETS embedded-unicode-${mod}
      DESTINATION "lib/nextcode/embedded/${mod}"
      COMPONENT "stdlib"
      )
    nextcode_install_in_component(
      FILES "${NEXTCODELIB_DIR}/embedded/${mod}/libnextcodeUnicodeDataTables.a"
      DESTINATION "lib/nextcode/embedded/${mod}/"
      COMPONENT "stdlib"
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
      )
    set_property(TARGET embedded-unicode-${mod} PROPERTY OSX_ARCHITECTURES "${arch}")

    add_dependencies(embedded-unicode embedded-unicode-${mod})
  endforeach()
endif()
