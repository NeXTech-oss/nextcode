if(NOT DEFINED NEXTCODE_BUILD_CLANG_OVERLAYS_SKIP_BUILTIN_FLOAT OR NOT NEXTCODE_BUILD_CLANG_OVERLAYS_SKIP_BUILTIN_FLOAT)
  add_nextcode_target_library(nextcode_Builtin_float
      ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES}
      IS_SDK_OVERLAY

      GYB_SOURCES
        float.code.gyb

      NEXTCODE_COMPILE_FLAGS
        ${NEXTCODE_RUNTIME_NEXTCODE_COMPILE_FLAGS}
        ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
        -Xfrontend -previous-module-installname-map-file -Xfrontend "${NEXTCODE_SOURCE_DIR}/stdlib/linker-support/previous-module-installname.json"

      LINK_FLAGS
        ${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}

      INSTALL_IN_COMPONENT stdlib
      MACCATALYST_BUILD_FLAVOR zippered)
endif()
