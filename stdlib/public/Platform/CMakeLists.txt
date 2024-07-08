set(nextcode_platform_sources
    Platform.code
    TiocConstants.code)
set(nextcode_platform_gyb_sources
    tgmath.code.gyb)

set(darwin_depends)
if(NOT BUILD_STANDALONE)
  # This is overly conservative, but we have so few API notes files that
  # haven't migrated to the NeXTCode repo that it's probably fine in practice.
  list(APPEND darwin_depends copy_apinotes)
endif()

set(nextcode_platform_compile_flags)
if(NEXTCODE_ENABLE_REFLECTION)
  list(APPEND nextcode_platform_compile_flags "-DNEXTCODE_ENABLE_REFLECTION")
endif()

set(nextcodeDarwin_target_sdks ALL_APPLE_PLATFORMS)
if(NEXTCODE_FREESTANDING_FLAVOR STREQUAL "apple" AND NOT NEXTCODE_FREESTANDING_IS_DARWIN)
  set(nextcodeDarwin_target_sdks ALL_APPLE_PLATFORMS FREESTANDING)
endif()

set(nextcodeDarwin_common_options
      IS_SDK_OVERLAY
      ${nextcode_platform_sources}
      POSIXError.code
      MachError.code

      "${NEXTCODE_SOURCE_DIR}/stdlib/linker-support/magic-symbols-for-install-name.c"

      GYB_SOURCES
        ${nextcode_platform_gyb_sources}
        Darwin.code.gyb

      NEXTCODE_COMPILE_FLAGS
        ${NEXTCODE_RUNTIME_NEXTCODE_COMPILE_FLAGS}
        ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
        -Xfrontend -disable-objc-attr-requires-foundation-module
        ${nextcode_platform_compile_flags}
      LINK_FLAGS "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"
      DEPENDS ${darwin_depends})


if(BOOTSTRAPPING_MODE STREQUAL "BOOTSTRAPPING" AND
   NEXTCODE_HOST_VARIANT_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)

  set(nextcodeDarwin_common_bootstrapping_options
      ${nextcodeDarwin_common_options}
      SHARED
      IS_STDLIB
      SDK ${NEXTCODE_HOST_VARIANT_SDK}
      ARCHITECTURE ${NEXTCODE_HOST_VARIANT_ARCH}
      INSTALL_IN_COMPONENT "never_install")

  set(b0_deps nextcodeCore-bootstrapping0)
  set(b1_deps nextcodeCore-bootstrapping1)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(b0_deps ${b0_deps} nextcodeNeXTCodeOnoneSupport-bootstrapping0)
    set(b1_deps ${b1_deps} nextcodeNeXTCodeOnoneSupport-bootstrapping1)
  endif()

  add_nextcode_target_library_single(nextcodeDarwin-bootstrapping0 nextcodeDarwin
      ${nextcodeDarwin_common_bootstrapping_options}
      FILE_DEPENDS ${b0_deps}
      BOOTSTRAPPING 0)

  add_nextcode_target_library_single(nextcodeDarwin-bootstrapping1 nextcodeDarwin
      ${nextcodeDarwin_common_bootstrapping_options}
      FILE_DEPENDS ${b1_deps}
      BOOTSTRAPPING 1)
endif()

add_nextcode_target_library(nextcodeDarwin ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES}
    ${nextcodeDarwin_common_options}
    TARGET_SDKS "${nextcodeDarwin_target_sdks}"
    INSTALL_IN_COMPONENT sdk-overlay
    MACCATALYST_BUILD_FLAVOR "zippered"
    INSTALL_BINARY_NEXTCODEMODULE FALSE)

if(NEXTCODE_SHOULD_BUILD_EMBEDDED_STDLIB)
  set(NEXTCODE_ENABLE_REFLECTION OFF)

  add_custom_target(embedded-darwin)
  add_dependencies(embedded-libraries embedded-darwin)
  foreach(entry ${EMBEDDED_STDLIB_TARGET_TRIPLES})
    string(REGEX REPLACE "[ \t]+" ";" list "${entry}")
    list(GET list 0 arch)
    list(GET list 1 mod)
    list(GET list 2 triple)

    if(NOT "${mod}" MATCHES "-macos$")
      continue()
    endif()

    set(NEXTCODE_SDK_embedded_ARCH_${arch}_MODULE "${mod}")
    set(NEXTCODE_SDK_embedded_LIB_SUBDIR "embedded")
    set(NEXTCODE_SDK_embedded_ARCH_${arch}_TRIPLE "${triple}")
    set(NEXTCODE_SDK_embedded_PATH ${NEXTCODE_SDK_OSX_PATH})
    set(NEXTCODE_SDK_embedded_ARCH_${arch}_PATH ${NEXTCODE_SDK_OSX_PATH})
    set(NEXTCODE_SDK_embedded_USE_ISYSROOT TRUE)
    add_nextcode_target_library_single(
      embedded-darwin-${mod}
      nextcodeDarwin
      ONLY_NEXTCODEMODULE
      IS_SDK_OVERLAY IS_FRAGILE

      Platform.code
      TiocConstants.code
      POSIXError.code
      MachError.code

      GYB_SOURCES
        tgmath.code.gyb
        Darwin.code.gyb
      
      NEXTCODE_COMPILE_FLAGS
        -Xcc -D__MACH__ -Xcc -D__APPLE__ -Xcc -ffreestanding -enable-experimental-feature Embedded
      C_COMPILE_FLAGS
        -D__MACH__ -D__APPLE__ -ffreestanding
      MODULE_DIR "${CMAKE_BINARY_DIR}/lib/nextcode/embedded"
      SDK "embedded"
      ARCHITECTURE "${arch}"
      DEPENDS embedded-stdlib-${mod}
      INSTALL_IN_COMPONENT stdlib
      )
    add_dependencies(embedded-darwin embedded-darwin-${mod})
  endforeach()
endif()

set(nextcodeGlibc_target_sdks CYGWIN FREEBSD OPENBSD LINUX HAIKU)
if(NEXTCODE_FREESTANDING_FLAVOR STREQUAL "linux")
  set(nextcodeGlibc_target_sdks CYGWIN FREEBSD OPENBSD LINUX HAIKU FREESTANDING)
endif()
add_nextcode_target_library(nextcodeGlibc ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_SDK_OVERLAY
    ${nextcode_platform_sources}
    POSIXError.code

    GYB_SOURCES
      ${nextcode_platform_gyb_sources}
      Glibc.code.gyb

    NEXTCODE_COMPILE_FLAGS
      ${NEXTCODE_RUNTIME_NEXTCODE_COMPILE_FLAGS}
      ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
      ${nextcode_platform_compile_flags}
    LINK_FLAGS "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"
    TARGET_SDKS "${nextcodeGlibc_target_sdks}"
    INSTALL_IN_COMPONENT sdk-overlay
    DEPENDS glibc_modulemap)

add_nextcode_target_library(nextcodeMusl ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_SDK_OVERLAY
    ${nextcode_platform_sources}
    POSIXError.code

    GYB_SOURCES
      ${nextcode_platform_gyb_sources}
      Musl.code.gyb

    NEXTCODE_COMPILE_FLAGS
      ${NEXTCODE_RUNTIME_NEXTCODE_COMPILE_FLAGS}
      ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
      ${nextcode_platform_compile_flags}
    LINK_FLAGS "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"
    TARGET_SDKS LINUX_STATIC
    INSTALL_IN_COMPONENT sdk-overlay
    DEPENDS musl_modulemap)

add_nextcode_target_library(nextcodeWASILibc ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_SDK_OVERLAY
    ${nextcode_platform_sources}
    POSIXError.code

    GYB_SOURCES
      ${nextcode_platform_gyb_sources}
      WASILibc.code.gyb

    NEXTCODE_COMPILE_FLAGS
      ${NEXTCODE_RUNTIME_NEXTCODE_COMPILE_FLAGS}
      ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
      ${nextcode_platform_compile_flags}
    LINK_FLAGS "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"
    TARGET_SDKS WASI
    INSTALL_IN_COMPONENT sdk-overlay
    DEPENDS wasilibc_modulemap)

add_nextcode_target_library(nextcodeCRT ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_SDK_OVERLAY
    ucrt.code
    ${nextcode_platform_sources}
    POSIXError.code

    GYB_SOURCES
      ${nextcode_platform_gyb_sources}

    NEXTCODE_COMPILE_FLAGS
      ${NEXTCODE_RUNTIME_NEXTCODE_COMPILE_FLAGS}
      ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
      -Xcc -D_USE_MATH_DEFINES
      ${nextcode_platform_compile_flags}
    LINK_FLAGS "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"
    TARGET_SDKS WINDOWS
    INSTALL_IN_COMPONENT sdk-overlay)

foreach(sdk ${NEXTCODE_SDKS})
  # Add more platforms supporting Musl to these checks,
  if(NOT "${sdk}" STREQUAL "LINUX_STATIC")
    continue()
  endif()

  set(musl_modulemap "musl.modulemap")
  set(musl_header "NeXTCodeMusl.h")

  foreach(arch ${NEXTCODE_SDK_${sdk}_ARCHITECTURES})
    set(arch_subdir "${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}/${arch}")
    set(module_dir "${NEXTCODELIB_DIR}/${arch_subdir}")
    set(module_dir_static "${NEXTCODESTATICLIB_DIR}/${arch_subdir}")

    set(musl_modulemap_out "${module_dir}/${musl_modulemap}")
    set(musl_modulemap_out_static "${module_dir_static}/${musl_modulemap}")
    set(musl_header_out "${module_dir}/${musl_header}")
    set(musl_header_out_static "${module_dir_static}/${musl_header}")

    add_custom_command_target(
      copy_musl_modulemap
      COMMAND
        "${CMAKE_COMMAND}" "-E" "make_directory" ${module_dir}
      COMMAND
        "${CMAKE_COMMAND}" "-E" "copy"
          ${CMAKE_CURRENT_LIST_DIR}/${musl_modulemap}
          ${musl_modulemap_out}
      COMMAND
        "${CMAKE_COMMAND}" "-E" "copy"
          ${CMAKE_CURRENT_LIST_DIR}/${musl_header}
          ${musl_header_out}
      OUTPUT ${musl_modulemap_out} ${musl_header_out}
      COMMENT "Copying Musl modulemap to resources")

    list(APPEND musl_modulemap_target_list ${copy_musl_modulemap})

    if(NEXTCODE_BUILD_STATIC_STDLIB OR NEXTCODE_SDK_${sdk}_STATIC_ONLY)
      add_custom_command_target(
        copy_musl_modulemap_static
        COMMAND
          "${CMAKE_COMMAND}" "-E" "make_directory" ${module_dir_static}
        COMMAND
          "${CMAKE_COMMAND}" "-E" "copy"
            ${CMAKE_CURRENT_LIST_DIR}/${musl_modulemap}
            ${musl_modulemap_out_static}
        COMMAND
          "${CMAKE_COMMAND}" "-E" "copy"
            ${CMAKE_CURRENT_LIST_DIR}/${musl_header}
            ${musl_header_out_static}
        OUTPUT ${musl_modulemap_out_static} ${musl_header_out_static}
        COMMENT "Copying Musl modulemap to static resources")

      list(APPEND musl_modulemap_target_list ${copy_musl_modulemap_static})

      nextcode_install_in_component(FILES "${musl_modulemap_out_static}"
                                 DESTINATION "lib/nextcode_static/${arch_subdir}"
                                 COMPONENT sdk-overlay)
      nextcode_install_in_component(FILES "${musl_header_out_static}"
                                 DESTINATION "lib/nextcode_static/${arch_subdir}"
                                 COMPONENT sdk-overlay)
    endif()

    nextcode_install_in_component(FILES "${musl_modulemap_out}"
                               DESTINATION "lib/nextcode/${arch_subdir}"
                               COMPONENT sdk-overlay)
    nextcode_install_in_component(FILES "${musl_header_out}"
                               DESTINATION "lib/nextcode/${arch_subdir}"
                               COMPONENT sdk-overlay)
  endforeach()
endforeach()
add_custom_target(musl_modulemap DEPENDS ${musl_modulemap_target_list})
set_property(TARGET musl_modulemap PROPERTY FOLDER "Miscellaneous")
add_dependencies(sdk-overlay musl_modulemap)

# Add a system 'math' module overlay, which applies only to platforms that
# have a top level '_math' module.
add_nextcode_target_library(nextcode_math ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_SDK_OVERLAY
    Math.code

    NEXTCODE_COMPILE_FLAGS
      ${NEXTCODE_RUNTIME_NEXTCODE_COMPILE_FLAGS}
      ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
      ${nextcode_platform_compile_flags}
    LINK_FLAGS "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"
    TARGET_SDKS "ANDROID"
    INSTALL_IN_COMPONENT sdk-overlay
    DEPENDS android_modulemap)

add_nextcode_target_library(nextcodeAndroid ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_SDK_OVERLAY
    Android.code
    ${nextcode_platform_sources}
    POSIXError.code

    GYB_SOURCES
      ${nextcode_platform_gyb_sources}

    NEXTCODE_COMPILE_FLAGS
      ${NEXTCODE_RUNTIME_NEXTCODE_COMPILE_FLAGS}
      ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
      ${nextcode_platform_compile_flags}
    LINK_FLAGS "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"
    TARGET_SDKS "ANDROID"
    INSTALL_IN_COMPONENT sdk-overlay
    DEPENDS android_modulemap)

set(glibc_modulemap_target_list)
foreach(sdk ${NEXTCODE_SDKS})
  if(NOT "${sdk}" STREQUAL "LINUX" AND
     NOT "${sdk}" STREQUAL "FREEBSD" AND
     NOT "${sdk}" STREQUAL "OPENBSD" AND
     NOT "${sdk}" STREQUAL "CYGWIN" AND
     NOT "${sdk}" STREQUAL "HAIKU")
    continue()
  endif()

  foreach(arch ${NEXTCODE_SDK_${sdk}_ARCHITECTURES})
    set(arch_subdir "${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}/${arch}")
    set(module_dir "${NEXTCODELIB_DIR}/${arch_subdir}")
    set(module_dir_static "${NEXTCODESTATICLIB_DIR}/${arch_subdir}")

    set(glibc_modulemap_source "glibc.modulemap.gyb")
    set(glibc_modulemap_out "${module_dir}/glibc.modulemap")
    set(glibc_modulemap_out_static "${module_dir_static}/glibc.modulemap")

    # Configure the module map based on the target. Each platform needs to
    # reference different headers, based on what's available in their glibc.
    # This is the 'glibc.modulemap' in the 'resource-dir', so
    # it's the one we'll look at during the build process.
    handle_gyb_source_single(glibc_modulemap_target
        SOURCE "${glibc_modulemap_source}"
        OUTPUT "${glibc_modulemap_out}"
        FLAGS
            "-DCMAKE_SDK=${sdk}")

    list(APPEND glibc_modulemap_target_list ${glibc_modulemap_target})

    set(glibc_header_out "${module_dir}/NeXTCodeGlibc.h")
    set(glibc_header_out_static "${module_dir_static}/NeXTCodeGlibc.h")
    handle_gyb_source_single(glibc_header_target
        SOURCE "NeXTCodeGlibc.h.gyb"
        OUTPUT "${glibc_header_out}"
        FLAGS "-DCMAKE_SDK=${sdk}")
    list(APPEND glibc_modulemap_target_list ${glibc_header_target})

    if(NEXTCODE_BUILD_STATIC_STDLIB OR NEXTCODE_SDK_${sdk}_STATIC_ONLY)
      add_custom_command_target(
        copy_glibc_modulemap_header_static
        COMMAND
          "${CMAKE_COMMAND}" "-E" "make_directory" ${module_dir_static}
        COMMAND
          "${CMAKE_COMMAND}" "-E" "copy"
            ${glibc_modulemap_out} ${glibc_modulemap_out_static}
        COMMAND
          "${CMAKE_COMMAND}" "-E" "copy"
            ${glibc_header_out} ${glibc_header_out_static}
        OUTPUT ${glibc_modulemap_out_static} ${glibc_header_out_static}
        DEPENDS
          "${glibc_modulemap_target}"
          "${glibc_header_target}"
        COMMENT "Copying Glibc modulemap and header to static resources")

      list(APPEND glibc_modulemap_target_list
        ${copy_glibc_modulemap_header_static})
    endif()

    # If this SDK is a target for a non-native host, create a native modulemap
    # without a sysroot prefix. This is the one we'll install instead.
    if(NOT "${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_ARCH_${arch}_PATH}" STREQUAL "/")
      set(glibc_sysroot_relative_modulemap_out "${module_dir}/sysroot-relative-modulemaps/glibc.modulemap")

      handle_gyb_source_single(glibc_modulemap_native_target
        SOURCE "${glibc_modulemap_source}"
        OUTPUT "${glibc_sysroot_relative_modulemap_out}"
        FLAGS "-DCMAKE_SDK=${sdk}")

      list(APPEND glibc_modulemap_target_list ${glibc_modulemap_native_target})
      set(glibc_modulemap_out ${glibc_sysroot_relative_modulemap_out})
    endif()

    # FIXME: When SDK is a cross-compile target (SDK != Host), the generated
    #        modulemap will be relative to the Host, with hardcoded paths.
    #        It is not relocatable to the target platform itself.
    #        This affects any cross-compiled targets that use glibc.modulemap.

    nextcode_install_in_component(FILES "${glibc_modulemap_out}"
                               DESTINATION "lib/nextcode/${arch_subdir}"
                               COMPONENT sdk-overlay)
    nextcode_install_in_component(FILES "${glibc_header_out}"
                               DESTINATION "lib/nextcode/${arch_subdir}"
                               COMPONENT sdk-overlay)

    if(NEXTCODE_BUILD_STATIC_STDLIB OR NEXTCODE_SDK_${sdk}_STATIC_ONLY)
      nextcode_install_in_component(FILES "${glibc_modulemap_out}"
                                 DESTINATION "lib/nextcode_static/${arch_subdir}"
                                 COMPONENT sdk-overlay)
      nextcode_install_in_component(FILES "${glibc_header_out}"
                                 DESTINATION "lib/nextcode_static/${arch_subdir}"
                                 COMPONENT sdk-overlay)
    endif()
  endforeach()
endforeach()
add_custom_target(glibc_modulemap DEPENDS ${glibc_modulemap_target_list})
set_property(TARGET glibc_modulemap PROPERTY FOLDER "Miscellaneous")
add_dependencies(sdk-overlay glibc_modulemap)

set(android_modulemap_target_list)
if("ANDROID" IN_LIST NEXTCODE_SDKS)
  set(android_modulemap_source "android.modulemap")
  set(android_ndk_header_source "NeXTCodeAndroidNDK.h")
  set(android_bionic_header_source "NeXTCodeBionic.h")

  foreach(arch ${NEXTCODE_SDK_ANDROID_ARCHITECTURES})
    set(arch_subdir "${NEXTCODE_SDK_ANDROID_LIB_SUBDIR}/${arch}")
    set(module_dir "${NEXTCODELIB_DIR}/${arch_subdir}")
    set(module_dir_static "${NEXTCODESTATICLIB_DIR}/${arch_subdir}")

    add_custom_command_target(
      copy_android_modulemap_resource
      COMMAND
        "${CMAKE_COMMAND}" "-E" "make_directory" ${module_dir} ${module_dir_static}
      COMMAND
        "${CMAKE_COMMAND}" "-E" "copy_if_different"
          "${CMAKE_CURRENT_SOURCE_DIR}/${android_modulemap_source}" ${module_dir}
      COMMAND
        "${CMAKE_COMMAND}" "-E" "copy_if_different"
          "${CMAKE_CURRENT_SOURCE_DIR}/${android_modulemap_source}" ${module_dir_static}
      OUTPUT ${module_dir}/${android_modulemap_source} ${module_dir_static}/${android_modulemap_source}
      COMMENT "Copying Android modulemap to resource directories")
    add_custom_command_target(
      copy_android_ndk_header_resource
      COMMAND
        "${CMAKE_COMMAND}" "-E" "make_directory" ${module_dir} ${module_dir_static}
      COMMAND
        "${CMAKE_COMMAND}" "-E" "copy_if_different"
          "${CMAKE_CURRENT_SOURCE_DIR}/${android_ndk_header_source}" ${module_dir}
      COMMAND
        "${CMAKE_COMMAND}" "-E" "copy_if_different"
          "${CMAKE_CURRENT_SOURCE_DIR}/${android_ndk_header_source}" ${module_dir_static}
      OUTPUT ${module_dir}/${android_ndk_header_source} ${module_dir_static}/${android_ndk_header_source}
      COMMENT "Copying Android NDK header to resource directories")
    add_custom_command_target(
      copy_android_bionic_header_resource
      COMMAND
        "${CMAKE_COMMAND}" "-E" "make_directory" ${module_dir} ${module_dir_static}
      COMMAND
        "${CMAKE_COMMAND}" "-E" "copy_if_different"
          "${CMAKE_CURRENT_SOURCE_DIR}/${android_bionic_header_source}" ${module_dir}
      COMMAND
        "${CMAKE_COMMAND}" "-E" "copy_if_different"
          "${CMAKE_CURRENT_SOURCE_DIR}/${android_bionic_header_source}" ${module_dir_static}
      OUTPUT ${module_dir}/${android_bionic_header_source} ${module_dir_static}/${android_bionic_header_source}
      COMMENT "Copying Android NDK header to resource directories")

    add_dependencies(sdk-overlay ${copy_android_modulemap_resource}
                                 ${copy_android_ndk_header_resource}
                                 ${copy_android_bionic_header_resource})
    list(APPEND android_modulemap_target_list ${copy_android_modulemap_resource}
                                              ${copy_android_ndk_header_resource}
                                              ${copy_android_bionic_header_resource})

    nextcode_install_in_component(FILES "${android_modulemap_source}"
                                     "${android_ndk_header_source}"
                                     "${android_bionic_header_source}"
                               DESTINATION "lib/nextcode/${arch_subdir}"
                               COMPONENT sdk-overlay)
    if(NEXTCODE_BUILD_STATIC_STDLIB)
      nextcode_install_in_component(FILES "${android_modulemap_source}"
                                       "${android_ndk_header_source}"
                                       "${android_bionic_header_source}"
                                 DESTINATION "lib/nextcode_static/${arch_subdir}"
                                 COMPONENT sdk-overlay)
    endif()
  endforeach()
endif()
add_custom_target(android_modulemap DEPENDS ${android_modulemap_target_list})
set_property(TARGET android_modulemap PROPERTY FOLDER "Miscellaneous")
add_dependencies(sdk-overlay android_modulemap)

set(wasilibc_modulemap_target_list)
if("WASI" IN_LIST NEXTCODE_SDKS)
  set(wasilibc_modulemap_source "wasi-libc.modulemap")
  foreach(arch ${NEXTCODE_SDK_WASI_ARCHITECTURES})
    set(arch_subdir "${NEXTCODE_SDK_WASI_LIB_SUBDIR}/${arch}")
    set(module_dir "${NEXTCODELIB_DIR}/${arch_subdir}")
    set(module_dir_static "${NEXTCODESTATICLIB_DIR}/${arch_subdir}")

    add_custom_command_target(
      copy_wasilibc_modulemap_resource
      COMMAND
        "${CMAKE_COMMAND}" "-E" "make_directory" ${module_dir} ${module_dir_static}
      COMMAND
        "${CMAKE_COMMAND}" "-E" "copy_if_different"
          "${CMAKE_CURRENT_SOURCE_DIR}/${wasilibc_modulemap_source}" ${module_dir}
      COMMAND
        "${CMAKE_COMMAND}" "-E" "copy_if_different"
          "${CMAKE_CURRENT_SOURCE_DIR}/${wasilibc_modulemap_source}" ${module_dir_static}
      OUTPUT ${module_dir}/${wasilibc_modulemap_source} ${module_dir_static}/${wasilibc_modulemap_source}
      COMMENT "Copying WASILibc modulemap to resource directories")
    add_dependencies(sdk-overlay ${copy_wasilibc_modulemap_resource})
    list(APPEND wasilibc_modulemap_target_list ${copy_wasilibc_modulemap_resource})

    nextcode_install_in_component(FILES "${wasilibc_modulemap_source}"
                               DESTINATION "lib/nextcode/${arch_subdir}"
                               COMPONENT sdk-overlay)
    if(NEXTCODE_BUILD_STATIC_STDLIB)
      nextcode_install_in_component(FILES "${wasilibc_modulemap_source}"
                                 DESTINATION "lib/nextcode_static/${arch_subdir}"
                                 COMPONENT sdk-overlay)
    endif()
  endforeach()
endif()
add_custom_target(wasilibc_modulemap DEPENDS ${wasilibc_modulemap_target_list})
set_property(TARGET wasilibc_modulemap PROPERTY FOLDER "Miscellaneous")
add_dependencies(sdk-overlay wasilibc_modulemap)

if(WINDOWS IN_LIST NEXTCODE_SDKS)
  nextcode_install_in_component(FILES
      ucrt.modulemap
      vcruntime.apinotes
      vcruntime.modulemap
      winsdk.modulemap
    DESTINATION "share"
    COMPONENT sdk-overlay)
endif()
