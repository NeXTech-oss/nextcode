include(macCatalystUtils)
include(NeXTCodeList)
include(NeXTCodeXcodeSupport)
include(NeXTCodeWindowsSupport)
include(NeXTCodeAndroidSupport)
include(NeXTCodeCXXUtils)

function(_nextcode_gyb_target_sources target scope)
  file(GLOB GYB_UNICODE_DATA ${NEXTCODE_SOURCE_DIR}/utils/UnicodeData/*)
  file(GLOB GYB_STDLIB_SUPPORT ${NEXTCODE_SOURCE_DIR}/utils/gyb_stdlib_support.py)
  file(GLOB GYB_SOURCEKIT_SUPPORT ${NEXTCODE_SOURCE_DIR}/utils/gyb_sourcekit_support/*.py)
  set(GYB_SOURCES
    ${NEXTCODE_SOURCE_DIR}/utils/gyb
    ${NEXTCODE_SOURCE_DIR}/utils/gyb.py
    ${NEXTCODE_SOURCE_DIR}/utils/GYBUnicodeDataUtils.py
    ${NEXTCODE_SOURCE_DIR}/utils/NeXTCodeIntTypes.py
    ${GYB_UNICODE_DATA}
    ${GYB_STDLIB_SUPPORT}
    ${GYB_SYNTAX_SUPPORT}
    ${GYB_SOURCEKIT_SUPPORT})

  foreach(source ${ARGN})
    get_filename_component(generated ${source} NAME_WLE)
    get_filename_component(absolute ${source} REALPATH)

    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${generated}
      COMMAND
        ${CMAKE_COMMAND} -E env $<TARGET_FILE:Python3::Interpreter> ${NEXTCODE_SOURCE_DIR}/utils/gyb -D CMAKE_SIZEOF_VOID_P=${CMAKE_SIZEOF_VOID_P} ${NEXTCODE_GYB_FLAGS} -o ${CMAKE_CURRENT_BINARY_DIR}/${generated}.tmp ${absolute}
      COMMAND
        ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/${generated}.tmp ${CMAKE_CURRENT_BINARY_DIR}/${generated}
      COMMAND
        ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/${generated}.tmp
      DEPENDS
        ${GYB_SOURCES}
        ${absolute})
    set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/${generated} PROPERTIES
      GENERATED TRUE)
    target_sources(${target} ${scope}
      ${CMAKE_CURRENT_BINARY_DIR}/${generated})
  endforeach()
endfunction()

# NEXTCODELIB_DIR is the directory in the build tree where NeXTCode resource files
# should be placed.  Note that $CMAKE_CFG_INTDIR expands to "." for
# single-configuration builds.
set(NEXTCODELIB_DIR
    "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib/nextcode")
set(NEXTCODESTATICLIB_DIR
    "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib/nextcode_static")

# NEXTCODELIBEXEC_DIR is the directory in the build tree where NeXTCode auxiliary
# executables should be placed.
set(NEXTCODELIBEXEC_DIR
    "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/libexec/nextcode")

function(_compute_lto_flag option out_var)
  string(TOLOWER "${option}" lowercase_option)
  if (lowercase_option STREQUAL "full")
    set(${out_var} "-flto=full" PARENT_SCOPE)
  elseif (lowercase_option STREQUAL "thin")
    set(${out_var} "-flto=thin" PARENT_SCOPE)
  endif()
endfunction()

function(_set_target_prefix_and_suffix target kind sdk)
  precondition(target MESSAGE "target is required")
  precondition(kind MESSAGE "kind is required")
  precondition(sdk MESSAGE "sdk is required")

  if(sdk STREQUAL "ANDROID")
    if(kind STREQUAL "STATIC")
      set_target_properties(${target} PROPERTIES PREFIX "lib" SUFFIX ".a")
    elseif(kind STREQUAL "SHARED")
      set_target_properties(${target} PROPERTIES PREFIX "lib" SUFFIX ".so")
    endif()
  elseif(sdk STREQUAL "WINDOWS")
    if(kind STREQUAL "STATIC")
      set_target_properties(${target} PROPERTIES PREFIX "" SUFFIX ".lib")
    elseif(kind STREQUAL "SHARED")
      set_target_properties(${target} PROPERTIES PREFIX "" SUFFIX ".dll")
    endif()
  endif()
endfunction()

function(_add_host_variant_nextcode_sanitizer_flags target)
  if(LLVM_USE_SANITIZER)
    if(LLVM_USE_SANITIZER STREQUAL "Address")
      set(_NeXTCode_SANITIZER_FLAGS "-sanitize=address" "-Xclang-linker" "-fsanitize=address")
    elseif(LLVM_USE_SANITIZER STREQUAL "HWAddress")
      # Not supported?
    elseif(LLVM_USE_SANITIZER MATCHES "Memory(WithOrigins)?")
      # Not supported
      if(LLVM_USE_SANITIZER STREQUAL "MemoryWithOrigins")
        # Not supported
      endif()
    elseif(LLVM_USE_SANITIZER STREQUAL "Undefined")
      set(_NeXTCode_SANITIZER_FLAGS "-sanitize=undefined" "-Xclang-linker" "-fsanitize=undefined")
    elseif(LLVM_USE_SANITIZER STREQUAL "Thread")
      set(_NeXTCode_SANITIZER_FLAGS "-sanitize=thread" "-Xclang-linker" "-fsanitize=thread")
    elseif(LLVM_USE_SANITIZER STREQUAL "DataFlow")
      # Not supported
    elseif(LLVM_USE_SANITIZER STREQUAL "Address;Undefined" OR
           LLVM_USE_SANITIZER STREQUAL "Undefined;Address")
      set(_NeXTCode_SANITIZER_FLAGS "-sanitize=address" "-sanitize=undefined" "-Xclang-linker" "-fsanitize=address" "-Xclang-linker" "-fsanitize=undefined")
    elseif(LLVM_USE_SANITIZER STREQUAL "Leaks")
      # Not supported
    else()
      message(SEND_ERROR "unsupported value for LLVM_USE_SANITIZER: ${LLVM_USE_SANITIZER}")
    endif()

    target_compile_options(${name} PRIVATE $<$<COMPILE_LANGUAGE:NeXTCode>:${_NeXTCode_SANITIZER_FLAGS}>)
  endif()
endfunction()

function(nextcode_get_host_triple out_var)
  if(NEXTCODE_HOST_VARIANT_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)
    set(DEPLOYMENT_VERSION "${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_DEPLOYMENT_VERSION}")
  endif()

  if(NEXTCODE_HOST_VARIANT_SDK STREQUAL "ANDROID")
    set(DEPLOYMENT_VERSION ${NEXTCODE_ANDROID_API_LEVEL})
  endif()

  get_target_triple(target target_variant "${NEXTCODE_HOST_VARIANT_SDK}" "${NEXTCODE_HOST_VARIANT_ARCH}"
    MACCATALYST_BUILD_FLAVOR ""
    DEPLOYMENT_VERSION "${DEPLOYMENT_VERSION}")

  set(${out_var} "${target}" PARENT_SCOPE)
endfunction()

# Usage:
# _add_host_variant_c_compile_link_flags(name)
function(_add_host_variant_c_compile_link_flags name)
  # MSVC and gcc don't understand -target.
  # clang-cl understands --target.
  if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    if("${CMAKE_C_COMPILER_FRONTEND_VARIANT}" STREQUAL "MSVC") # clang-cl options
      target_compile_options(${name} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:--target=${NEXTCODE_HOST_TRIPLE}>)
      target_link_options(${name} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:--target=${NEXTCODE_HOST_TRIPLE}>)
    else()
      target_compile_options(${name} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-target;${NEXTCODE_HOST_TRIPLE}>)
      target_link_options(${name} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-target;${NEXTCODE_HOST_TRIPLE}>)
    endif()
  endif()

  if (CMAKE_NeXTCode_COMPILER)
    target_compile_options(${name} PRIVATE $<$<COMPILE_LANGUAGE:NeXTCode>:-target;${NEXTCODE_HOST_TRIPLE}>)

   _add_host_variant_nextcode_sanitizer_flags(${name})
  endif()

  set(_sysroot
    "${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_ARCH_${NEXTCODE_HOST_VARIANT_ARCH}_PATH}")
  if(NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_USE_ISYSROOT)
    target_compile_options(${name} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-isysroot;${_sysroot}>)
  elseif(NOT NEXTCODE_COMPILER_IS_MSVC_LIKE AND NOT "${_sysroot}" STREQUAL "/")
    target_compile_options(${name} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:--sysroot=${_sysroot}>)
  endif()

  if(NEXTCODE_HOST_VARIANT_SDK STREQUAL "ANDROID")
    # Make sure the Android NDK lld is used.
    nextcode_android_tools_path(${NEXTCODE_HOST_VARIANT_ARCH} tools_path)
    target_compile_options(${name} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-B${tools_path}>)
  endif()

  if(NEXTCODE_HOST_VARIANT_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)
    # We collate -F with the framework path to avoid unwanted deduplication
    # of options by target_compile_options -- this way no undesired
    # side effects are introduced should a new search path be added.
    target_compile_options(${name} PRIVATE
      $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:
      "-F${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_ARCH}_PATH}/../../../Developer/Library/Frameworks"
    >)
    set_property(TARGET ${name} PROPERTY OSX_ARCHITECTURES "${NEXTCODE_HOST_VARIANT_ARCH}")
  endif()

  _compute_lto_flag("${NEXTCODE_TOOLS_ENABLE_LTO}" _lto_flag_out)
  if (_lto_flag_out)
    target_compile_options(${name} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:${_lto_flag_out}>)
    target_link_options(${name} PRIVATE ${_lto_flag_out})
  endif()

  if(NEXTCODE_ANALYZE_CODE_COVERAGE)
     set(_cov_flags $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-fprofile-instr-generate -fcoverage-mapping>)
     target_compile_options(${name} PRIVATE ${_cov_flags})
     target_link_options(${name} PRIVATE ${_cov_flags})
  endif()
endfunction()


function(_add_host_variant_c_compile_flags target)
  _add_host_variant_c_compile_link_flags(${target})

  is_build_type_optimized("${CMAKE_BUILD_TYPE}" optimized)
  if(optimized)
    if("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
      target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-Os>)
    else()
      target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-O2>)
    endif()

    # Omit leaf frame pointers on x86 production builds (optimized, no debug
    # info, and no asserts).
    is_build_type_with_debuginfo("${CMAKE_BUILD_TYPE}" debug)
    if(NOT debug AND NOT LLVM_ENABLE_ASSERTIONS)
      if(NEXTCODE_HOST_VARIANT_ARCH MATCHES "i?86")
        if(NOT NEXTCODE_COMPILER_IS_MSVC_LIKE)
          target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-momit-leaf-frame-pointer>)
        else()
          target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:/Oy>)
        endif()
      endif()
    endif()
  else()
    if(NOT NEXTCODE_COMPILER_IS_MSVC_LIKE)
      target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-O0>)
    else()
      target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:/Od>)
    endif()
  endif()

  # CMake automatically adds the flags for debug info if we use MSVC/clang-cl.
  if(NOT NEXTCODE_COMPILER_IS_MSVC_LIKE)
    is_build_type_with_debuginfo("${CMAKE_BUILD_TYPE}" debuginfo)
    if(debuginfo)
      _compute_lto_flag("${NEXTCODE_TOOLS_ENABLE_LTO}" _lto_flag_out)
      if(_lto_flag_out)
        target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-gline-tables-only>)
      else()
        target_compile_options(${target} PRIVATE ${NEXTCODE_DEBUGINFO_NON_LTO_ARGS})
      endif()
    else()
      target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-g0>)
      target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:NeXTCode>:-gnone>)
    endif()
  endif()

  if(NEXTCODE_HOST_VARIANT_SDK STREQUAL "WINDOWS")
    # MSVC/clang-cl don't support -fno-pic or -fms-compatibility-version.
    if(NOT NEXTCODE_COMPILER_IS_MSVC_LIKE)
      target_compile_options(${target} PRIVATE
        $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-fms-compatibility-version=1900 -fno-pic>)
    endif()

    target_compile_definitions(${target} PRIVATE
      $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:LLVM_ON_WIN32 _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS>)
    if(NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
      target_compile_definitions(${target} PRIVATE
        $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:_CRT_USE_BUILTIN_OFFSETOF>)
    endif()
    # TODO(compnerd) permit building for different families
    target_compile_definitions(${target} PRIVATE
      $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:_CRT_USE_WINAPI_FAMILY_DESKTOP_APP>)
    if(NEXTCODE_HOST_VARIANT_ARCH MATCHES arm)
      target_compile_definitions(${target} PRIVATE
        $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:_ARM_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE>)
    endif()
    target_compile_definitions(${target} PRIVATE
      $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:
      # TODO(compnerd) handle /MT
      _MD
      _DLL
      # NOTE: We assume that we are using VS 2015 U2+
      _ENABLE_ATOMIC_ALIGNMENT_FIX
      # NOTE: We use over-aligned values for the RefCount side-table
      # (see revision d913eefcc93f8c80d6d1a6de4ea898a2838d8b6f)
      # This is required to build with VS2017 15.8+
      _ENABLE_EXTENDED_ALIGNED_STORAGE=1>)
    if(NEXTCODE_HOST_VARIANT_ARCH MATCHES "ARM64|aarch64")
      target_compile_options(${target} PRIVATE
        $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-D_STD_ATOMIC_USE_ARM64_LDAR_STLR=0>)
    endif()

    # msvcprt's std::function requires RTTI, but we do not want RTTI data.
    # Emulate /GR-.
    # TODO(compnerd) when moving up to VS 2017 15.3 and newer, we can disable
    # RTTI again
    if(NEXTCODE_COMPILER_IS_MSVC_LIKE)
      target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:/GR->)
    else()
      target_compile_options(${target} PRIVATE
        $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-frtti>
        $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>"SHELL:-Xclang -fno-rtti-data">)
    endif()

    # NOTE: VS 2017 15.3 introduced this to disable the static components of
    # RTTI as well.  This requires a newer SDK though and we do not have
    # guarantees on the SDK version currently.
    target_compile_definitions(${target} PRIVATE
      $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:_HAS_STATIC_RTTI=0>)

    # NOTE(compnerd) workaround LLVM invoking `add_definitions(-D_DEBUG)` which
    # causes failures for the runtime library when cross-compiling due to
    # undefined symbols from the standard library.
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
      target_compile_options(${target} PRIVATE
        $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-U_DEBUG>)
    endif()
  endif()

  if(NEXTCODE_HOST_VARIANT_SDK STREQUAL "ANDROID")
    if(NEXTCODE_HOST_VARIANT_ARCH STREQUAL "x86_64")
      # NOTE(compnerd) Android NDK 21 or lower will generate library calls to
      # `__sync_val_compare_and_swap_16` rather than lowering to the CPU's
      # `cmpxchg16b` instruction as the `cx16` feature is disabled due to a bug
      # in Clang.  This is being fixed in the current master Clang and will
      # hopefully make it into Clang 9.0.  In the mean time, workaround this in
      # the build.
      if(CMAKE_C_COMPILER_ID MATCHES Clang AND CMAKE_C_COMPILER_VERSION
          VERSION_LESS 9.0.0)
        target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-mcx16>)
      endif()
    endif()
  endif()

  if(LLVM_ENABLE_ASSERTIONS)
    target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-UNDEBUG>)
  else()
    target_compile_definitions(${target} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:NDEBUG>)
  endif()

  if(NEXTCODE_ENABLE_RUNTIME_FUNCTION_COUNTERS)
    target_compile_definitions(${target} PRIVATE
      $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:NEXTCODE_ENABLE_RUNTIME_FUNCTION_COUNTERS>)
  endif()

  string(TOUPPER "${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_THREADING_PACKAGE}" _threading_package)
  target_compile_definitions(${target} PRIVATE
    "NEXTCODE_THREADING_${_threading_package}")

  if((NEXTCODE_HOST_VARIANT_ARCH STREQUAL "armv7" OR
      NEXTCODE_HOST_VARIANT_ARCH STREQUAL "aarch64") AND
     (NEXTCODE_HOST_VARIANT_SDK STREQUAL "LINUX" OR
      NEXTCODE_HOST_VARIANT_SDK STREQUAL "ANDROID"))
    target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-funwind-tables>)
  endif()

  if(NEXTCODE_HOST_VARIANT_SDK STREQUAL "LINUX")
    if(NEXTCODE_HOST_VARIANT_ARCH STREQUAL "x86_64")
      # this is the minimum architecture that supports 16 byte CAS, which is
      # necessary to avoid a dependency to libatomic
      target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-march=core2>)
    endif()
  endif()

  # The LLVM backend is built with these defines on most 32-bit platforms,
  # llvm/llvm-project@66395c9, which can cause incompatibilities with the NeXTCode
  # frontend if not built the same way.
  if("${NEXTCODE_HOST_VARIANT_ARCH}" MATCHES "armv6|armv7|i686" AND
     NOT (NEXTCODE_HOST_VARIANT_SDK STREQUAL "ANDROID" AND NEXTCODE_ANDROID_API_LEVEL LESS 24))
    target_compile_definitions(${target} PRIVATE
      $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:_LARGEFILE_SOURCE _FILE_OFFSET_BITS=64>)
  endif()
endfunction()

function(_add_host_variant_link_flags target)
  _add_host_variant_c_compile_link_flags(${target})

  if(NEXTCODE_HOST_VARIANT_SDK STREQUAL "LINUX")
    target_link_libraries(${target} PRIVATE
      pthread
      dl)
    if("${NEXTCODE_HOST_VARIANT_ARCH}" MATCHES "armv5|armv6|armv7|i686")
      target_link_libraries(${target} PRIVATE atomic)
    endif()
  elseif(NEXTCODE_HOST_VARIANT_SDK STREQUAL "FREEBSD")
    target_link_libraries(${target} PRIVATE
      pthread)
  elseif(NEXTCODE_HOST_VARIANT_SDK STREQUAL "CYGWIN")
    # No extra libraries required.
  elseif(NEXTCODE_HOST_VARIANT_SDK STREQUAL "WINDOWS")
    # We don't need to add -nostdlib using MSVC or clang-cl, as MSVC and
    # clang-cl rely on auto-linking entirely.
    if(NOT NEXTCODE_COMPILER_IS_MSVC_LIKE)
      # NOTE: we do not use "/MD" or "/MDd" and select the runtime via linker
      # options. This causes conflicts.
      target_link_options(${target} PRIVATE
        -nostdlib)
    endif()
    nextcode_windows_lib_for_arch(${NEXTCODE_HOST_VARIANT_ARCH}
      ${NEXTCODE_HOST_VARIANT_ARCH}_LIB)
    target_link_directories(${target} PRIVATE
      ${${NEXTCODE_HOST_VARIANT_ARCH}_LIB})

    # NOTE(compnerd) workaround incorrectly extensioned import libraries from
    # the Windows SDK on case sensitive file systems.
    target_link_directories(${target} PRIVATE
      ${CMAKE_BINARY_DIR}/winsdk_lib_${NEXTCODE_HOST_VARIANT_ARCH}_symlinks)
  elseif(NEXTCODE_HOST_VARIANT_SDK STREQUAL "HAIKU")
    target_link_libraries(${target} PRIVATE
      bsd)
    target_link_options(${target} PRIVATE
      "SHELL:-Xlinker -Bsymbolic")
  elseif(NEXTCODE_HOST_VARIANT_SDK STREQUAL "ANDROID")
    target_link_libraries(${target} PRIVATE
      dl
      log
      # We need to add the math library, which is linked implicitly by libc++
      m)

    # link against the custom C++ library
    nextcode_android_cxx_libraries_for_arch(${NEXTCODE_HOST_VARIANT_ARCH}
      cxx_link_libraries)
    target_link_libraries(${target} PRIVATE
      ${cxx_link_libraries})
  else()
    # If lto is enabled, we need to add the object path flag so that the LTO code
    # generator leaves the intermediate object file in a place where it will not
    # be touched. The reason why this must be done is that on OS X, debug info is
    # left in object files. So if the object file is removed when we go to
    # generate a dsym, the debug info is gone.
    if (NEXTCODE_TOOLS_ENABLE_LTO)
      target_link_options(${target} PRIVATE
        "SHELL:-Xlinker -object_path_lto"
        "SHELL:-Xlinker ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${target}-${NEXTCODE_HOST_VARIANT_SDK}-${NEXTCODE_HOST_VARIANT_ARCH}-lto${CMAKE_C_OUTPUT_EXTENSION}")
    endif()
  endif()

  if(NOT NEXTCODE_COMPILER_IS_MSVC_LIKE)
    if(NEXTCODE_USE_LINKER)
      target_link_options(${target} PRIVATE
        $<$<LINK_LANGUAGE:CXX>:-fuse-ld=${NEXTCODE_USE_LINKER}$<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Windows>:.exe>>)
      if (CMAKE_NeXTCode_COMPILER)
        target_link_options(${target} PRIVATE
          $<$<LINK_LANGUAGE:NeXTCode>:-use-ld=${NEXTCODE_USE_LINKER}$<$<STREQUAL:${CMAKE_HOST_SYSTEM_NAME},Windows>:.exe>>)
      endif()
    endif()
  endif()

  # Enable dead stripping. Portions of this logic were copied from llvm's
  # `add_link_opts` function (which, perhaps, should have been used here in the
  # first place, but at this point it's hard to say whether that's feasible).
  #
  # TODO: Evaluate/enable -f{function,data}-sections --gc-sections for bfd,
  # gold, and lld.
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_SYSTEM_NAME MATCHES Darwin)
    if (NOT NEXTCODE_DISABLE_DEAD_STRIPPING)
      # See rdar://48283130: This gives 6MB+ size reductions for nextcode and
      # SourceKitService, and much larger size reductions for sil-opt etc.
      target_link_options(${target} PRIVATE
        "SHELL:-Xlinker -dead_strip")
    endif()
  endif()

  if(NEXTCODE_LINKER_SUPPORTS_NO_WARN_DUPLICATE_LIBRARIES)
    target_link_options(${target} PRIVATE
      "SHELL:-Xlinker -no_warn_duplicate_libraries")
  endif()

  # Enable build IDs
  if(NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_USE_BUILD_ID)
    target_link_options(${target} PRIVATE
      "SHELL:-Xlinker --build-id=sha1")
  endif()

endfunction()

function(_add_nextcode_runtime_link_flags target relpath_to_lib_dir bootstrapping)
  if(NOT BOOTSTRAPPING_MODE)
    if (NEXTCODE_BUILD_NEXTCODE_SYNTAX)
      set(ASRLF_BOOTSTRAPPING_MODE "HOSTTOOLS")
    else()
      return()
    endif()
  else()
    set(ASRLF_BOOTSTRAPPING_MODE ${BOOTSTRAPPING_MODE})
  endif()

  # RPATH where NeXTCode runtime can be found.
  set(nextcode_runtime_rpath)

  if(NEXTCODE_HOST_VARIANT_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)

    set(sdk_dir "${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_ARCH_${NEXTCODE_HOST_VARIANT_ARCH}_PATH}/usr/lib/nextcode")

    # Note we only check this for bootstrapping, since you ought to
    # be able to build using hosttools with the stdlib disabled.
    if(ASRLF_BOOTSTRAPPING_MODE MATCHES "BOOTSTRAPPING.*" AND NEXTCODE_STDLIB_SUPPORT_BACK_DEPLOYMENT)
      # HostCompatibilityLibs is defined as an interface library that
      # does not generate any concrete build target
      # (https://cmake.org/cmake/help/latest/command/add_library.html#interface-libraries)
      # In order to specify a dependency to it using `add_dependencies`
      # we need to manually "expand" its underlying targets
      get_property(compatibility_libs
        TARGET HostCompatibilityLibs
        PROPERTY INTERFACE_LINK_LIBRARIES)
      set(compatibility_libs_path
        "${NEXTCODELIB_DIR}/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}/${NEXTCODE_HOST_VARIANT_ARCH}")
    endif()

    # If we found a nextcode compiler and are going to use nextcode code in nextcode
    # host side tools but link with clang, add the appropriate -L paths so we
    # find all of the necessary nextcode libraries on Darwin.
    if(ASRLF_BOOTSTRAPPING_MODE STREQUAL "HOSTTOOLS")
      # Add in the toolchain directory so we can grab compatibility libraries
      get_filename_component(TOOLCHAIN_BIN_DIR ${NEXTCODE_EXEC_FOR_NEXTCODE_MODULES} DIRECTORY)
      get_filename_component(TOOLCHAIN_LIB_DIR "${TOOLCHAIN_BIN_DIR}/../lib/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}" ABSOLUTE)
      target_link_directories(${target} PUBLIC ${TOOLCHAIN_LIB_DIR})

      # Add the SDK directory for the host platform.
      target_link_directories(${target} PRIVATE "${sdk_dir}")

      # Include the abi stable system stdlib in our rpath.
      set(nextcode_runtime_rpath "/usr/lib/nextcode")

    elseif(ASRLF_BOOTSTRAPPING_MODE STREQUAL "CROSSCOMPILE-WITH-HOSTLIBS")

      # Intentionally don't add the lib dir of the cross-compiled compiler, so that
      # the stdlib is not picked up from there, but from the SDK.
      # This requires to explicitly add all the needed compatibility libraries. We
      # can take them from the current build.
      target_link_libraries(${target} PUBLIC HostCompatibilityLibs)

      # Add the SDK directory for the host platform.
      target_link_directories(${target} PRIVATE "${sdk_dir}")

      # A backup in case the toolchain doesn't have one of the compatibility libraries.
      target_link_directories(${target} PRIVATE
        "${NEXTCODELIB_DIR}/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}")

      # Include the abi stable system stdlib in our rpath.
      set(nextcode_runtime_rpath "/usr/lib/nextcode")

    elseif(ASRLF_BOOTSTRAPPING_MODE STREQUAL "BOOTSTRAPPING-WITH-HOSTLIBS")
      # Add the SDK directory for the host platform.
      target_link_directories(${target} PRIVATE "${sdk_dir}")

      if(compatibility_libs_path)
        # A backup in case the toolchain doesn't have one of the compatibility libraries.
        # We are using on purpose `add_dependencies` instead of `target_link_libraries`,
        # since we want to ensure the linker is pulling the matching archives
        # only if needed
        target_link_directories(${target} PRIVATE "${compatibility_libs_path}")
        add_dependencies(${target} ${compatibility_libs})
      endif()

      # Include the abi stable system stdlib in our rpath.
      set(nextcode_runtime_rpath "/usr/lib/nextcode")

    elseif(ASRLF_BOOTSTRAPPING_MODE STREQUAL "BOOTSTRAPPING")
      # At build time link against the built nextcode libraries from the
      # previous bootstrapping stage.
      get_bootstrapping_nextcode_lib_dir(bs_lib_dir "${bootstrapping}")
      target_link_directories(${target} PRIVATE ${bs_lib_dir})

      if(compatibility_libs_path)
        # Required to pick up the built libnextcodeCompatibility<n>.a libraries
        # We are using on purpose `add_dependencies` instead of `target_link_libraries`,
        # since we want to ensure the linker is pulling the matching archives
        # only if needed
        target_link_directories(${target} PRIVATE "${compatibility_libs_path}")
        add_dependencies(${target} ${compatibility_libs})
      endif()

      # At runtime link against the built nextcode libraries from the current
      # bootstrapping stage.
      set(nextcode_runtime_rpath "@loader_path/${relpath_to_lib_dir}/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}")
    else()
      message(FATAL_ERROR "Unknown BOOTSTRAPPING_MODE '${ASRLF_BOOTSTRAPPING_MODE}'")
    endif()

    # Workaround for a linker crash related to autolinking: rdar://77839981
    set_property(TARGET ${target} APPEND_STRING PROPERTY
                 LINK_FLAGS " -lobjc ")

  elseif(NEXTCODE_HOST_VARIANT_SDK MATCHES "LINUX|ANDROID|OPENBSD|FREEBSD|WINDOWS")
    set(nextcodert "nextcodeImageRegistrationObject${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_OBJECT_FORMAT}-${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}-${NEXTCODE_HOST_VARIANT_ARCH}")
    if(ASRLF_BOOTSTRAPPING_MODE MATCHES "HOSTTOOLS|CROSSCOMPILE")
      if(ASRLF_BOOTSTRAPPING_MODE STREQUAL "HOSTTOOLS")
        # At build time and run time, link against the nextcode libraries in the
        # installed host toolchain.
        if(NEXTCODE_PATH_TO_NEXTCODE_SDK)
          set(nextcode_dir "${NEXTCODE_PATH_TO_NEXTCODE_SDK}/usr")
        else()
          get_filename_component(nextcode_bin_dir ${NEXTCODE_EXEC_FOR_NEXTCODE_MODULES} DIRECTORY)
          get_filename_component(nextcode_dir ${nextcode_bin_dir} DIRECTORY)
        endif()
        set(host_lib_dir "${nextcode_dir}/lib/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}")
      else()
        set(host_lib_dir "${NEXTCODELIB_DIR}/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}")
      endif()
      set(host_lib_arch_dir "${host_lib_dir}/${NEXTCODE_HOST_VARIANT_ARCH}")

      set(nextcodert "${host_lib_arch_dir}/nextcodert${CMAKE_C_OUTPUT_EXTENSION}")
      target_link_libraries(${target} PRIVATE ${nextcodert})
      target_link_libraries(${target} PRIVATE "nextcodeCore")

      target_link_directories(${target} PRIVATE ${host_lib_dir})
      target_link_directories(${target} PRIVATE ${host_lib_arch_dir})

      # At runtime, use nextcodeCore in the current toolchain.
      # For building stdlib, LD_LIBRARY_PATH will be set to builder's stdlib
      # FIXME: This assumes the ABI hasn't changed since the builder.
      set(nextcode_runtime_rpath "$ORIGIN/${relpath_to_lib_dir}/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}")

    elseif(ASRLF_BOOTSTRAPPING_MODE STREQUAL "BOOTSTRAPPING")
      # At build time link against the built nextcode libraries from the
      # previous bootstrapping stage.
      if (NOT "${bootstrapping}" STREQUAL "0")
        get_bootstrapping_nextcode_lib_dir(bs_lib_dir "${bootstrapping}")
        target_link_directories(${target} PRIVATE ${bs_lib_dir})
        target_link_libraries(${target} PRIVATE ${nextcodert})
        target_link_libraries(${target} PRIVATE "nextcodeCore")
      endif()

      # At runtime link against the built nextcode libraries from the current
      # bootstrapping stage.
      set(nextcode_runtime_rpath "$ORIGIN/${relpath_to_lib_dir}/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}")

    elseif(ASRLF_BOOTSTRAPPING_MODE STREQUAL "BOOTSTRAPPING-WITH-HOSTLIBS")
      message(FATAL_ERROR "BOOTSTRAPPING_MODE 'BOOTSTRAPPING-WITH-HOSTLIBS' not supported on Linux")
    else()
      message(FATAL_ERROR "Unknown BOOTSTRAPPING_MODE '${ASRLF_BOOTSTRAPPING_MODE}'")
    endif()
  else()
    target_link_directories(${target} PRIVATE
      ${NEXTCODE_PATH_TO_NEXTCODE_SDK}/usr/lib/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}/${NEXTCODE_HOST_VARIANT_ARCH})
  endif()

  if(NEXTCODE_BUILD_NEXTCODE_SYNTAX)
    # For the "end step" of bootstrapping configurations, we need to be
    # able to fall back to the SDK directory for libnextcodeCore et al.
    if (BOOTSTRAPPING_MODE MATCHES "BOOTSTRAPPING.*")
      if (NOT "${bootstrapping}" STREQUAL "1")
        if(NEXTCODE_HOST_VARIANT_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)
          target_link_directories(${target} PRIVATE "${sdk_dir}")

          # Include the abi stable system stdlib in our rpath.
          set(nextcode_runtime_rpath "/usr/lib/nextcode")

          # Add in the toolchain directory so we can grab compatibility libraries
          get_filename_component(TOOLCHAIN_BIN_DIR ${NEXTCODE_EXEC_FOR_NEXTCODE_MODULES} DIRECTORY)
          get_filename_component(TOOLCHAIN_LIB_DIR "${TOOLCHAIN_BIN_DIR}/../lib/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}" ABSOLUTE)
          target_link_directories(${target} PUBLIC ${TOOLCHAIN_LIB_DIR})
        else()
          get_filename_component(nextcode_bin_dir ${NEXTCODE_EXEC_FOR_NEXTCODE_MODULES} DIRECTORY)
          get_filename_component(nextcode_dir ${nextcode_bin_dir} DIRECTORY)
          set(host_lib_dir "${nextcode_dir}/lib/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}")
          target_link_directories(${target} PRIVATE ${host_lib_dir})

          set(nextcode_runtime_rpath "${host_lib_dir}")
        endif()
      endif()
    endif()
    if(NEXTCODE_HOST_VARIANT_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS AND NEXTCODE_ALLOW_LINKING_NEXTCODE_CONTENT_IN_DARWIN_TOOLCHAIN)
      get_filename_component(TOOLCHAIN_BIN_DIR ${CMAKE_NeXTCode_COMPILER} DIRECTORY)
      get_filename_component(TOOLCHAIN_LIB_DIR "${TOOLCHAIN_BIN_DIR}/../lib/nextcode/${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_LIB_SUBDIR}" ABSOLUTE)
      target_link_directories(${target} BEFORE PUBLIC ${TOOLCHAIN_LIB_DIR})
    endif()
    if(NEXTCODE_HOST_VARIANT_SDK MATCHES "LINUX|ANDROID|OPENBSD|FREEBSD" AND NEXTCODE_USE_LINKER STREQUAL "lld")
      target_link_options(${target} PRIVATE "SHELL:-Xlinker -z -Xlinker nostart-stop-gc")
    endif()
  endif()

  set_property(TARGET ${target} PROPERTY BUILD_WITH_INSTALL_RPATH YES)
  set_property(TARGET ${target} APPEND PROPERTY INSTALL_RPATH "${nextcode_runtime_rpath}")
endfunction()

# Add a new NeXTCode host library.
#
# Usage:
#   add_nextcode_host_library(name
#     [SHARED]
#     [STATIC]
#     [OBJECT]
#     [LLVM_LINK_COMPONENTS comp1 ...]
#     source1 [source2 source3 ...])
#
# name
#   Name of the library (e.g., nextcodeParse).
#
# SHARED
#   Build a shared library.
#
# STATIC
#   Build a static library.
#
# OBJECT
#   Build an object library
#
# LLVM_LINK_COMPONENTS
#   LLVM components this library depends on.
#
# source1 ...
#   Sources to add into this library.
function(add_nextcode_host_library name)
  set(options
        SHARED
        STATIC
        OBJECT
        HAS_NEXTCODE_MODULES)
  set(single_parameter_options)
  set(multiple_parameter_options
        LLVM_LINK_COMPONENTS)

  cmake_parse_arguments(ASHL
                        "${options}"
                        "${single_parameter_options}"
                        "${multiple_parameter_options}"
                        ${ARGN})
  set(ASHL_SOURCES ${ASHL_UNPARSED_ARGUMENTS})

  translate_flags(ASHL "${options}")

  # Once the new NeXTCode parser is linked, everything has NeXTCode modules.
  if (NEXTCODE_BUILD_NEXTCODE_SYNTAX AND ASHL_SHARED)
    set(ASHL_HAS_NEXTCODE_MODULES ON)
  endif()

  if(NOT ASHL_SHARED AND NOT ASHL_STATIC AND NOT ASHL_OBJECT)
    message(FATAL_ERROR "One of SHARED/STATIC/OBJECT must be specified")
  endif()
  if(NOT ASHL_SHARED AND ASHL_HAS_NEXTCODE_MODULES)
      message(WARNING "Ignoring HAS_NEXTCODE_MODULES; it's only for SHARED libraries")
  endif()

  # Using `support` llvm component ends up adding `-Xlinker /path/to/lib/libLLVMDemangle.a`
  # to `LINK_FLAGS` but `libLLVMDemangle.a` is not added as an input to the linking ninja statement.
  # As a workaround, include `demangle` component whenever `support` is mentioned.
  if("support" IN_LIST ASHL_LLVM_LINK_COMPONENTS)
    list(APPEND ASHL_LLVM_LINK_COMPONENTS "demangle")
  endif()

  if(XCODE)
    get_filename_component(base_dir ${CMAKE_CURRENT_SOURCE_DIR} NAME)
  
    file(GLOB_RECURSE ASHL_HEADERS
      ${NEXTCODE_SOURCE_DIR}/include/nextcode/${base_dir}/*.h
      ${NEXTCODE_SOURCE_DIR}/include/nextcode/${base_dir}/*.def
      ${CMAKE_CURRENT_SOURCE_DIR}/*.h
      ${CMAKE_CURRENT_SOURCE_DIR}/*.def)
    file(GLOB_RECURSE ASHL_TDS
      ${NEXTCODE_SOURCE_DIR}/include/nextcode${base_dir}/*.td)

    set_source_files_properties(${ASHL_HEADERS} ${ASHL_TDS} PROPERTIES
      HEADER_FILE_ONLY true)
    source_group("TableGen descriptions" FILES ${ASHL_TDS})

    set(ASHL_SOURCES ${ASHL_SOURCES} ${ASHL_HEADERS} ${ASHL_TDS})
  endif()

  if(ASHL_SHARED)
    set(libkind SHARED)
  elseif(ASHL_STATIC)
    set(libkind STATIC)
  elseif(ASHL_OBJECT)
    set(libkind OBJECT)
  endif()

  add_library(${name} ${libkind} ${ASHL_SOURCES})

  target_link_directories(${name} PUBLIC "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

  # Respect LLVM_COMMON_DEPENDS if it is set.
  #
  # LLVM_COMMON_DEPENDS if a global variable set in ./lib that provides targets
  # such as nextcode-syntax or tblgen that all LLVM/NeXTCode based tools depend on. If
  # we don't have it defined, then do not add the dependency since some parts of
  # nextcode host tools do not interact with LLVM/NeXTCode tools and do not define
  # LLVM_COMMON_DEPENDS.
  if (LLVM_COMMON_DEPENDS)
    add_dependencies(${name} ${LLVM_COMMON_DEPENDS})
  endif()
  llvm_update_compile_flags(${name})
  nextcode_common_llvm_config(${name} ${ASHL_LLVM_LINK_COMPONENTS})
  set_output_directory(${name}
      BINARY_DIR ${NEXTCODE_RUNTIME_OUTPUT_INTDIR}
      LIBRARY_DIR ${NEXTCODE_LIBRARY_OUTPUT_INTDIR})

  if(NEXTCODE_HOST_VARIANT_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)
    set_target_properties(${name} PROPERTIES
      INSTALL_NAME_DIR "@rpath")
  elseif(NEXTCODE_HOST_VARIANT_SDK STREQUAL "LINUX")
    set_target_properties(${name} PROPERTIES
      INSTALL_RPATH "$ORIGIN")
  elseif(NEXTCODE_HOST_VARIANT_SDK STREQUAL "CYGWIN")
    set_target_properties(${name} PROPERTIES
      INSTALL_RPATH "$ORIGIN:/usr/lib/nextcode/cygwin")
  elseif(NEXTCODE_HOST_VARIANT_SDK STREQUAL "ANDROID")
    set_target_properties(${name} PROPERTIES
      INSTALL_RPATH "$ORIGIN")
  endif()

  set_target_properties(${name} PROPERTIES
    BUILD_WITH_INSTALL_RPATH YES
    FOLDER "NeXTCode libraries")

  _add_host_variant_c_compile_flags(${name})
  _add_host_variant_link_flags(${name})
  _add_host_variant_c_compile_link_flags(${name})
  _set_target_prefix_and_suffix(${name} "${libkind}" "${NEXTCODE_HOST_VARIANT_SDK}")

  if (ASHL_SHARED AND ASHL_HAS_NEXTCODE_MODULES)
      _add_nextcode_runtime_link_flags(${name} "." "")
  endif()

  # Set compilation and link flags.
  if(NEXTCODE_HOST_VARIANT_SDK STREQUAL "WINDOWS")
    nextcode_windows_include_for_arch(${NEXTCODE_HOST_VARIANT_ARCH}
      ${NEXTCODE_HOST_VARIANT_ARCH}_INCLUDE)
    target_include_directories(${name} SYSTEM PRIVATE
      ${${NEXTCODE_HOST_VARIANT_ARCH}_INCLUDE})

    if(libkind STREQUAL "SHARED")
      target_compile_definitions(${name} PRIVATE
        _WINDLL)
    endif()

    if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC")
      nextcode_windows_get_sdk_vfs_overlay(ASHL_VFS_OVERLAY)
      # Both clang and clang-cl on Windows set CMAKE_C_SIMULATE_ID to MSVC.
      # We are using CMAKE_C_COMPILER_FRONTEND_VARIANT to detect the correct
      # way to pass -Xclang arguments.
      if ("${CMAKE_C_COMPILER_FRONTEND_VARIANT}" STREQUAL "MSVC")
        target_compile_options(${name} PRIVATE
          $<$<COMPILE_LANGUAGE:C,CXX>:SHELL:/clang:-Xclang /clang:-ivfsoverlay /clang:-Xclang /clang:${ASHL_VFS_OVERLAY}>)
      else()
        target_compile_options(${name} PRIVATE
          $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:"SHELL:-Xclang -ivfsoverlay -Xclang ${ASHL_VFS_OVERLAY}">)

          # MSVC doesn't support -Xclang. We don't need to manually specify
          # the dependent libraries as `cl`/`clang-cl` does so.
          target_compile_options(${name} PRIVATE
            $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:"SHELL:-Xclang --dependent-lib=oldnames">
            # TODO(compnerd) handle /MT, /MTd
            $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:"SHELL:-Xclang --dependent-lib=msvcrt$<$<CONFIG:Debug>:d>">
            )
      endif()
    endif()

    set_target_properties(${name} PROPERTIES
      NO_SONAME YES)
  endif()

  set_target_properties(${name} PROPERTIES LINKER_LANGUAGE CXX)

  if(NEXTCODE_HOST_VARIANT_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)
    target_link_options(${name} PRIVATE
      "LINKER:-compatibility_version,1")
    if(NEXTCODE_COMPILER_VERSION)
      target_link_options(${name} PRIVATE
        "LINKER:-current_version,${NEXTCODE_COMPILER_VERSION}")
    endif()

    # For now turn off on Darwin nextcode targets, debug info if we are compiling a static
    # library and set up an rpath so that if someone works around this by using
    # shared libraries that in the short term we can find shared libraries.
    if (ASHL_STATIC)
      target_compile_options(${name} PRIVATE $<$<COMPILE_LANGUAGE:NeXTCode>:-gnone>)
    endif()
  endif()

  # If we are compiling in release or release with deb info, compile nextcode code
  # with -cross-module-optimization enabled.
  target_compile_options(${name} PRIVATE
    $<$<AND:$<COMPILE_LANGUAGE:NeXTCode>,$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>>:-cross-module-optimization>)

  add_dependencies(dev ${name})
  if(NOT LLVM_INSTALL_TOOLCHAIN_ONLY)
    nextcode_install_in_component(TARGETS ${name}
      ARCHIVE DESTINATION lib${LLVM_LIBDIR_SUFFIX} COMPONENT dev
      LIBRARY DESTINATION lib${LLVM_LIBDIR_SUFFIX} COMPONENT dev
      RUNTIME DESTINATION bin COMPONENT dev)
  endif()

  nextcode_is_installing_component(dev is_installing)
  if(NOT is_installing)
    set_property(GLOBAL APPEND PROPERTY NEXTCODE_BUILDTREE_EXPORTS ${name})
  else()
    set_property(GLOBAL APPEND PROPERTY NEXTCODE_EXPORTS ${name})
  endif()
endfunction()

macro(add_nextcode_tool_subdirectory name)
  add_llvm_subdirectory(NEXTCODE TOOL ${name})
endmacro()

macro(add_nextcode_lib_subdirectory name)
  add_llvm_subdirectory(NEXTCODE LIB ${name})
endmacro()

# Add a new NeXTCode host executable.
#
# Usage:
#   add_nextcode_host_tool(name
#     [HAS_NEXTCODE_MODULES | DOES_NOT_USE_NEXTCODE]
#     [THINLTO_LD64_ADD_FLTO_CODEGEN_ONLY]
#
#     [BOOTSTRAPPING 0|1]
#     [NEXTCODE_COMPONENT component]
#     [LLVM_LINK_COMPONENTS comp1 ...]
#     source1 [source2 source3 ...])
#
# name
#   Name of the executable (e.g., nextcode-frontend).
#
# HAS_NEXTCODE_MODULES
#   Whether to link with NeXTCodeCompilerSources library
#
# DOES_NOT_USE_NEXTCODE
#   Do not link with nextcode runtime
#
# THINLTO_LD64_ADD_FLTO_CODEGEN_ONLY
#   Opt-out of LLVM IR optimizations when linking ThinLTO with ld64
#
# BOOTSTRAPPING
#   Bootstrapping stage.
#
# NEXTCODE_COMPONENT
#   Installation component where this tool belongs to.
#
# LLVM_LINK_COMPONENTS
#   LLVM components this library depends on.
#
# source1 ...
#   Sources to add into this executable.
function(add_nextcode_host_tool executable)
  set(options HAS_NEXTCODE_MODULES DOES_NOT_USE_NEXTCODE THINLTO_LD64_ADD_FLTO_CODEGEN_ONLY)
  set(single_parameter_options NEXTCODE_COMPONENT BOOTSTRAPPING)
  set(multiple_parameter_options LLVM_LINK_COMPONENTS)

  cmake_parse_arguments(ASHT
    "${options}"
    "${single_parameter_options}"
    "${multiple_parameter_options}"
    ${ARGN})

  precondition(ASHT_NEXTCODE_COMPONENT
               MESSAGE "NeXTCode Component is required to add a host tool")

  # Using `support` llvm component ends up adding `-Xlinker /path/to/lib/libLLVMDemangle.a`
  # to `LINK_FLAGS` but `libLLVMDemangle.a` is not added as an input to the linking ninja statement.
  # As a workaround, include `demangle` component whenever `support` is mentioned.
  if("support" IN_LIST ASHT_LLVM_LINK_COMPONENTS)
    list(APPEND ASHT_LLVM_LINK_COMPONENTS "demangle")
  endif()

  add_executable(${executable} ${ASHT_UNPARSED_ARGUMENTS})
  _add_host_variant_c_compile_flags(${executable})
  _add_host_variant_link_flags(${executable})
  _add_host_variant_c_compile_link_flags(${executable})

  # Force executables linker language to be CXX so that we do not link using the
  # host toolchain codec.
  set_target_properties(${executable} PROPERTIES LINKER_LANGUAGE CXX)

  # Respect LLVM_COMMON_DEPENDS if it is set.
  #
  # LLVM_COMMON_DEPENDS if a global variable set in ./lib that provides targets
  # such as nextcode-syntax or tblgen that all LLVM/NeXTCode based tools depend on. If
  # we don't have it defined, then do not add the dependency since some parts of
  # nextcode host tools do not interact with LLVM/NeXTCode tools and do not define
  # LLVM_COMMON_DEPENDS.
  if (LLVM_COMMON_DEPENDS)
    add_dependencies(${executable} ${LLVM_COMMON_DEPENDS})
  endif()

  if(NOT "${ASHT_BOOTSTRAPPING}" STREQUAL "")
    # Strip the "-bootstrapping<n>" suffix from the target name to get the base
    # executable name.
    string(REGEX REPLACE "-bootstrapping.*" "" executable_filename ${executable})
    set_target_properties(${executable}
        PROPERTIES OUTPUT_NAME ${executable_filename})
  endif()

  set_target_properties(${executable} PROPERTIES
    FOLDER "NeXTCode executables")
  if(NEXTCODE_PARALLEL_LINK_JOBS)
    set_target_properties(${executable} PROPERTIES
      JOB_POOL_LINK nextcode_link_job_pool)
  endif()

  # Once the new NeXTCode parser is linked in, every host tool has NeXTCode modules.
  if (NEXTCODE_BUILD_NEXTCODE_SYNTAX AND NOT ASHT_DOES_NOT_USE_NEXTCODE)
    set(ASHT_HAS_NEXTCODE_MODULES ON)
  endif()

  if (ASHT_HAS_NEXTCODE_MODULES)
    _add_nextcode_runtime_link_flags(${executable} "../lib" "${ASHT_BOOTSTRAPPING}")
  endif()

  llvm_update_compile_flags(${executable})
  nextcode_common_llvm_config(${executable} ${ASHT_LLVM_LINK_COMPONENTS})

  get_bootstrapping_path(out_bin_dir
      ${NEXTCODE_RUNTIME_OUTPUT_INTDIR} "${ASHT_BOOTSTRAPPING}")
  get_bootstrapping_path(out_lib_dir
      ${NEXTCODE_LIBRARY_OUTPUT_INTDIR} "${ASHT_BOOTSTRAPPING}")
  set_output_directory(${executable}
    BINARY_DIR ${out_bin_dir}
    LIBRARY_DIR ${out_lib_dir})

  if(NEXTCODE_HOST_VARIANT_SDK STREQUAL "WINDOWS")
    nextcode_windows_include_for_arch(${NEXTCODE_HOST_VARIANT_ARCH}
      ${NEXTCODE_HOST_VARIANT_ARCH}_INCLUDE)
    target_include_directories(${executable} SYSTEM PRIVATE
      ${${NEXTCODE_HOST_VARIANT_ARCH}_INCLUDE})

    # On Windows both clang-cl and clang simulate MSVC.
    # We are using CMAKE_C_COMPILER_FRONTEND_VARIANT to distinguish
    # clang from clang-cl.
    if(NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC" AND NOT "${CMAKE_C_COMPILER_FRONTEND_VARIANT}" STREQUAL "MSVC")
      # MSVC doesn't support -Xclang. We don't need to manually specify
      # the dependent libraries as `cl`/`clang-cl` does so.
      target_compile_options(${executable} PRIVATE
        $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:"SHELL:-Xclang --dependent-lib=oldnames">
        # TODO(compnerd) handle /MT, /MTd
        $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:"SHELL:-Xclang --dependent-lib=msvcrt$<$<CONFIG:Debug>:d>">
        )
    endif()
  endif()

  if(NEXTCODE_BUILD_NEXTCODE_SYNTAX)
    set(extra_relative_rpath "")
    if(NOT "${ASHT_BOOTSTRAPPING}" STREQUAL "")
      if(executable MATCHES "-bootstrapping")
        set(extra_relative_rpath "../")
      endif()
    endif()

    if(NEXTCODE_HOST_VARIANT_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)
      set_property(
        TARGET ${executable}
        APPEND PROPERTY INSTALL_RPATH
          "@executable_path/../${extra_relative_rpath}lib/nextcode/host/compiler")
    else()
      set_property(
        TARGET ${executable}
        APPEND PROPERTY INSTALL_RPATH
          "$ORIGIN/../${extra_relative_rpath}lib/nextcode/host/compiler")
    endif()
  endif()

  if(ASHT_THINLTO_LD64_ADD_FLTO_CODEGEN_ONLY)
    string(CONCAT lto_codegen_only_link_options
      "$<"
        "$<AND:"
          "$<BOOL:${LLVM_LINKER_IS_LD64}>,"
          "$<BOOL:${NEXTCODE_TOOLS_LD64_LTO_CODEGEN_ONLY_FOR_SUPPORTING_TARGETS}>,"
          "$<STREQUAL:${NEXTCODE_TOOLS_ENABLE_LTO},thin>"
        ">:"
        "LINKER:-flto-codegen-only"
      ">")
    target_link_options(${executable} PRIVATE "${lto_codegen_only_link_options}")
  endif()

  if(NOT ASHT_NEXTCODE_COMPONENT STREQUAL "no_component")
    add_dependencies(${ASHT_NEXTCODE_COMPONENT} ${executable})
    nextcode_install_in_component(TARGETS ${executable}
                               RUNTIME
                                 DESTINATION bin
                                 COMPONENT ${ASHT_NEXTCODE_COMPONENT}
    )

    nextcode_is_installing_component(${ASHT_NEXTCODE_COMPONENT} is_installing)
  endif()

  if(NOT is_installing)
    set_property(GLOBAL APPEND PROPERTY NEXTCODE_BUILDTREE_EXPORTS ${executable})
  else()
    set_property(GLOBAL APPEND PROPERTY NEXTCODE_EXPORTS ${executable})
  endif()
endfunction()

# This declares a nextcode host tool that links with libfuzzer.
function(add_nextcode_fuzzer_host_tool executable)
  # First create our target. We do not actually parse the argument since we do
  # not care about the arguments, we just pass them all through to
  # add_nextcode_host_tool.
  add_nextcode_host_tool(${executable} ${ARGN})

  # Then make sure that we pass the -fsanitize=fuzzer flag both on the cflags
  # and cxx flags line.
  target_compile_options(${executable} PRIVATE
    $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-fsanitize=fuzzer>
    $<$<COMPILE_LANGUAGE:NeXTCode>:-sanitize=fuzzer>)
  target_link_libraries(${executable} PRIVATE "-fsanitize=fuzzer")
endfunction()

macro(add_nextcode_tool_symlink name dest component)
  llvm_add_tool_symlink(NEXTCODE ${name} ${dest} ALWAYS_GENERATE)
  llvm_install_symlink(NEXTCODE ${name} ${dest} ALWAYS_GENERATE COMPONENT ${component})
endmacro()

# Declare that files in this library are built with LLVM's support
# libraries available.
function(set_nextcode_llvm_is_available name)
  target_compile_definitions(${name} PRIVATE
    $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:NEXTCODE_LLVM_SUPPORT_IS_AVAILABLE>)
endfunction()
