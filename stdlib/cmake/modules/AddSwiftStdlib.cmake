
include(AddNeXTCode)
include(NeXTCodeSource)
include(CompatibilityLibs)

function(add_dependencies_multiple_targets)
  cmake_parse_arguments(
      ADMT # prefix
      "" # options
      "" # single-value args
      "TARGETS;DEPENDS" # multi-value args
      ${ARGN})
  precondition(ADMT_UNPARSED_ARGUMENTS NEGATE MESSAGE "unrecognized arguments: ${ADMT_UNPARSED_ARGUMENTS}")

  if(NOT "${ADMT_DEPENDS}" STREQUAL "")
    foreach(target ${ADMT_TARGETS})
      add_dependencies("${target}" ${ADMT_DEPENDS})
    endforeach()
  endif()
endfunction()

# Usage:
# _add_target_variant_c_compile_link_flags(
#   SDK sdk
#   ARCH arch
#   BUILD_TYPE build_type
#   ENABLE_LTO enable_lto
#   ANALYZE_CODE_COVERAGE analyze_code_coverage
#   RESULT_VAR_NAME result_var_name
#   DEPLOYMENT_VERSION_OSX version # If provided, overrides the default value of the OSX deployment target set by the NeXTCode project for this compilation only.
#   DEPLOYMENT_VERSION_MACCATALYST version
#   DEPLOYMENT_VERSION_IOS version
#   DEPLOYMENT_VERSION_TVOS version
#   DEPLOYMENT_VERSION_WATCHOS version
#   DEPLOYMENT_VERSION_XROS version
#
# )
function(_add_target_variant_c_compile_link_flags)
  set(oneValueArgs SDK ARCH BUILD_TYPE RESULT_VAR_NAME ENABLE_LTO ANALYZE_CODE_COVERAGE
    DEPLOYMENT_VERSION_OSX DEPLOYMENT_VERSION_MACCATALYST DEPLOYMENT_VERSION_IOS DEPLOYMENT_VERSION_TVOS DEPLOYMENT_VERSION_WATCHOS
    DEPLOYMENT_VERSION_XROS
    MACCATALYST_BUILD_FLAVOR
  )
  cmake_parse_arguments(CFLAGS
    ""
    "${oneValueArgs}"
    ""
    ${ARGN})

  get_maccatalyst_build_flavor(maccatalyst_build_flavor
      "${CFLAGS_SDK}" "${CFLAGS_MACCATALYST_BUILD_FLAVOR}")

  set(result ${${CFLAGS_RESULT_VAR_NAME}})

  if("${CFLAGS_SDK}" IN_LIST NEXTCODE_DARWIN_PLATFORMS)
    # Check if there's a specific OS deployment version needed for this invocation
    if("${CFLAGS_SDK}" STREQUAL "OSX")
      if(DEFINED maccatalyst_build_flavor)
        set(DEPLOYMENT_VERSION ${CFLAGS_DEPLOYMENT_VERSION_MACCATALYST})
      else()
        set(DEPLOYMENT_VERSION ${CFLAGS_DEPLOYMENT_VERSION_OSX})
      endif()
    elseif("${CFLAGS_SDK}" STREQUAL "IOS" OR "${CFLAGS_SDK}" STREQUAL "IOS_SIMULATOR")
      set(DEPLOYMENT_VERSION ${CFLAGS_DEPLOYMENT_VERSION_IOS})
    elseif("${CFLAGS_SDK}" STREQUAL "TVOS" OR "${CFLAGS_SDK}" STREQUAL "TVOS_SIMULATOR")
      set(DEPLOYMENT_VERSION ${CFLAGS_DEPLOYMENT_VERSION_TVOS})
    elseif("${CFLAGS_SDK}" STREQUAL "WATCHOS" OR "${CFLAGS_SDK}" STREQUAL "WATCHOS_SIMULATOR")
      set(DEPLOYMENT_VERSION ${CFLAGS_DEPLOYMENT_VERSION_WATCHOS})
    elseif("${CFLAGS_SDK}" STREQUAL "XROS" OR "${CFLAGS_SDK}" STREQUAL "XROS_SIMULATOR")
      set(DEPLOYMENT_VERSION ${CFLAGS_DEPLOYMENT_VERSION_XROS})
    endif()

    if("${DEPLOYMENT_VERSION}" STREQUAL "")
      set(DEPLOYMENT_VERSION "${NEXTCODE_SDK_${CFLAGS_SDK}_DEPLOYMENT_VERSION}")
    endif()
  endif()

  if("${CFLAGS_SDK}" STREQUAL "ANDROID")
    set(DEPLOYMENT_VERSION ${NEXTCODE_ANDROID_API_LEVEL})
  endif()

  # MSVC, clang-cl, gcc don't understand -target.
  if(CMAKE_C_COMPILER_ID MATCHES "^Clang|AppleClang$" AND
      NOT NEXTCODE_COMPILER_IS_MSVC_LIKE)
    get_target_triple(target target_variant "${CFLAGS_SDK}" "${CFLAGS_ARCH}"
      MACCATALYST_BUILD_FLAVOR "${maccatalyst_build_flavor}"
      DEPLOYMENT_VERSION "${DEPLOYMENT_VERSION}")
    list(APPEND result "-target" "${target}")
    if(target_variant)
      # Check if the C compiler supports `-target-variant` flag
      # TODO (etcwilde): This is a massive hack to deal with the fact that we
      # are lying to cmake about what compiler is being used. Normally we could
      # use `check_compiler_flag(C ...)` here. Unfortunately, that uses a
      # different compiler since we swap out the C/CXX compiler part way through
      # the build.
      file(WRITE "${CMAKE_BINARY_DIR}/stdlib/empty" "")
      execute_process(
        COMMAND
          "${CMAKE_C_COMPILER}"
          -Wno-unused-command-line-argument
          -target-variant x86_64-apple-ios14.5-macabi -x c -c - -o /dev/null
        INPUT_FILE
          "${CMAKE_BINARY_DIR}/stdlib/empty"
        OUTPUT_QUIET ERROR_QUIET
        RESULT_VARIABLE
          SUPPORTS_TARGET_VARIANT)

      if(NOT SUPPORTS_TARGET_VARIANT)
        list(APPEND result "-target-variant" "${target_variant}")
      else()
        list(APPEND result "-darwin-target-variant" "${target_variant}")
      endif()
    endif()
  endif()

  set(_sysroot "${NEXTCODE_SDK_${CFLAGS_SDK}_ARCH_${CFLAGS_ARCH}_PATH}")
  if(NEXTCODE_SDK_${CFLAGS_SDK}_USE_ISYSROOT)
    list(APPEND result "-isysroot" "${_sysroot}")
  elseif(NOT NEXTCODE_COMPILER_IS_MSVC_LIKE AND NOT "${_sysroot}" STREQUAL "/")
    list(APPEND result "--sysroot=${_sysroot}")
  endif()

  if("${CFLAGS_SDK}" STREQUAL "LINUX_STATIC")
    list(APPEND result "-isystem" "${NEXTCODE_MUSL_PATH}/${CFLAGS_ARCH}/usr/include/c++/v1")
    list(APPEND result "-DNEXTCODE_LIBC_IS_MUSL")
  endif()


  if("${CFLAGS_SDK}" STREQUAL "ANDROID")
    # Make sure the Android NDK lld is used.
    nextcode_android_tools_path(${CFLAGS_ARCH} tools_path)
    list(APPEND result "-B" "${tools_path}")
  endif()

  if("${CFLAGS_SDK}" IN_LIST NEXTCODE_DARWIN_PLATFORMS)
    # We collate -F with the framework path to avoid unwanted deduplication
    # of options by target_compile_options -- this way no undesired
    # side effects are introduced should a new search path be added.
    list(APPEND result
      "-F${NEXTCODE_SDK_${CFLAGS_SDK}_PATH}/../../../Developer/Library/Frameworks")
  endif()

  if(CFLAGS_ANALYZE_CODE_COVERAGE)
    list(APPEND result "-fprofile-instr-generate"
                       "-fcoverage-mapping")
  endif()

  # Use frame pointers on Linux
  if("${CFLAGS_SDK}" STREQUAL "LINUX")
    list(APPEND result "-fno-omit-frame-pointer")
  endif()

  _compute_lto_flag("${CFLAGS_ENABLE_LTO}" _lto_flag_out)
  if (_lto_flag_out)
    list(APPEND result "${_lto_flag_out}")
    # Disable opaque pointers in lto mode.
    #list(APPEND result "-Xclang")
    #list(APPEND result "-no-opaque-pointers")
  endif()

  set("${CFLAGS_RESULT_VAR_NAME}" "${result}" PARENT_SCOPE)
endfunction()


function(_add_target_variant_c_compile_flags)
  set(oneValueArgs SDK ARCH BUILD_TYPE ENABLE_ASSERTIONS ANALYZE_CODE_COVERAGE
    DEPLOYMENT_VERSION_OSX DEPLOYMENT_VERSION_MACCATALYST DEPLOYMENT_VERSION_IOS DEPLOYMENT_VERSION_TVOS DEPLOYMENT_VERSION_WATCHOS
    DEPLOYMENT_VERSION_XROS
    RESULT_VAR_NAME ENABLE_LTO
    MACCATALYST_BUILD_FLAVOR)
  cmake_parse_arguments(CFLAGS
    ""
    "${oneValueArgs}"
    ""
    ${ARGN})

  set(result ${${CFLAGS_RESULT_VAR_NAME}})

  list(APPEND result
    "-DNEXTCODE_RUNTIME"
    "-DNEXTCODE_LIB_SUBDIR=\"${NEXTCODE_SDK_${CFLAGS_SDK}_LIB_SUBDIR}\""
    "-DNEXTCODE_ARCH=\"${CFLAGS_ARCH}\""
    )

  if ("${CFLAGS_ARCH}" STREQUAL "arm64" OR
      "${CFLAGS_ARCH}" STREQUAL "arm64_32")
    if (NEXTCODE_ENABLE_GLOBAL_ISEL_ARM64)
      list(APPEND result "-fglobal-isel")
    endif()
  endif()

  _add_target_variant_c_compile_link_flags(
    SDK "${CFLAGS_SDK}"
    ARCH "${CFLAGS_ARCH}"
    BUILD_TYPE "${CFLAGS_BUILD_TYPE}"
    ENABLE_ASSERTIONS "${CFLAGS_ENABLE_ASSERTIONS}"
    ENABLE_LTO "${CFLAGS_ENABLE_LTO}"
    ANALYZE_CODE_COVERAGE FALSE
    DEPLOYMENT_VERSION_OSX "${CFLAGS_DEPLOYMENT_VERSION_OSX}"
    DEPLOYMENT_VERSION_MACCATALYST "${CFLAGS_DEPLOYMENT_VERSION_MACCATALYST}"
    DEPLOYMENT_VERSION_IOS "${CFLAGS_DEPLOYMENT_VERSION_IOS}"
    DEPLOYMENT_VERSION_TVOS "${CFLAGS_DEPLOYMENT_VERSION_TVOS}"
    DEPLOYMENT_VERSION_WATCHOS "${CFLAGS_DEPLOYMENT_VERSION_WATCHOS}"
    DEPLOYMENT_VERSION_XROS "${CFLAGS_DEPLOYMENT_VERSION_XROS}"
    RESULT_VAR_NAME result
    MACCATALYST_BUILD_FLAVOR "${CFLAGS_MACCATALYST_BUILD_FLAVOR}")

  is_build_type_optimized("${CFLAGS_BUILD_TYPE}" optimized)
  if(optimized)
    if("${CFLAGS_BUILD_TYPE}" STREQUAL "MinSizeRel")
      list(APPEND result "-Os")
    else()
      list(APPEND result "-O2")
    endif()

    # Omit leaf frame pointers on x86 production builds (optimized, no debug
    # info, and no asserts).
    is_build_type_with_debuginfo("${CFLAGS_BUILD_TYPE}" debug)
    if(NOT debug AND NOT CFLAGS_ENABLE_ASSERTIONS)
      if("${CFLAGS_ARCH}" STREQUAL "i386" OR "${CFLAGS_ARCH}" STREQUAL "i686")
        if(NOT NEXTCODE_COMPILER_IS_MSVC_LIKE)
          list(APPEND result "-momit-leaf-frame-pointer")
        else()
          list(APPEND result "/Oy")
        endif()
      endif()
    endif()
  else()
    if(NOT NEXTCODE_COMPILER_IS_MSVC_LIKE)
      list(APPEND result "-O0")
    else()
      list(APPEND result "/Od")
    endif()
  endif()

  # CMake automatically adds the flags for debug info if we use MSVC/clang-cl.
  if(NOT NEXTCODE_COMPILER_IS_MSVC_LIKE)
    is_build_type_with_debuginfo("${CFLAGS_BUILD_TYPE}" debuginfo)
    if(debuginfo)
      _compute_lto_flag("${CFLAGS_ENABLE_LTO}" _lto_flag_out)
      if(_lto_flag_out)
        list(APPEND result "-gline-tables-only")
      else()
        list(APPEND result "-g")
      endif()
    elseif("${CFLAGS_BUILD_TYPE}" STREQUAL "MinSizeRel")
      # MinSizeRel builds of stdlib (but not the compiler) should get debug info
      list(APPEND result "-g")
    else()
      list(APPEND result "-g0")
    endif()
  endif()

  if("${CFLAGS_SDK}" STREQUAL "WINDOWS")
    # MSVC/clang-cl don't support -fno-pic or -fms-compatibility-version.
    if(NOT NEXTCODE_COMPILER_IS_MSVC_LIKE)
      list(APPEND result -fno-pic)
      list(APPEND result "-fms-compatibility-version=1900")
    endif()

    list(APPEND result "-DLLVM_ON_WIN32")
    list(APPEND result "-D_CRT_SECURE_NO_WARNINGS")
    list(APPEND result "-D_CRT_NONSTDC_NO_WARNINGS")
    list(APPEND result "-D_CRT_USE_BUILTIN_OFFSETOF")
    # TODO(compnerd) permit building for different families
    list(APPEND result "-D_CRT_USE_WINAPI_FAMILY_DESKTOP_APP")
    if("${CFLAGS_ARCH}" MATCHES arm)
      list(APPEND result "-D_ARM_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE")
    endif()
    list(APPEND result "-D_MT")
    # TODO(compnerd) handle /MT
    list(APPEND result "-D_DLL")
    # NOTE: We assume that we are using VS 2015 U2+
    list(APPEND result "-D_ENABLE_ATOMIC_ALIGNMENT_FIX")
    # NOTE: We use over-aligned values for the RefCount side-table
    # (see revision d913eefcc93f8c80d6d1a6de4ea898a2838d8b6f)
    # This is required to build with VS2017 15.8+
    list(APPEND result "-D_ENABLE_EXTENDED_ALIGNED_STORAGE=1")

    # msvcprt's std::function requires RTTI, but we do not want RTTI data.
    # Emulate /GR-.
    # TODO(compnerd) when moving up to VS 2017 15.3 and newer, we can disable
    # RTTI again
    if(NEXTCODE_COMPILER_IS_MSVC_LIKE)
      list(APPEND result /GR-)
    else()
      list(APPEND result -frtti)
      list(APPEND result -Xclang;-fno-rtti-data)
    endif()

    # NOTE: VS 2017 15.3 introduced this to disable the static components of
    # RTTI as well.  This requires a newer SDK though and we do not have
    # guarantees on the SDK version currently.
    list(APPEND result "-D_HAS_STATIC_RTTI=0")

    # NOTE(compnerd) workaround LLVM invoking `add_definitions(-D_DEBUG)` which
    # causes failures for the runtime library when cross-compiling due to
    # undefined symbols from the standard library.
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
      list(APPEND result "-U_DEBUG")
    endif()
  endif()

  # The concurrency library uses double-word atomics.  MSVC's std::atomic
  # uses a spin lock for this, so to get reasonable behavior we have to
  # implement it ourselves using _InterlockedCompareExchange128.
  # clang-cl requires us to enable the `cx16` feature to use this intrinsic.
  if(CFLAGS_ARCH STREQUAL "x86_64")
    if(NEXTCODE_COMPILER_IS_MSVC_LIKE)
      list(APPEND result /clang:-mcx16)
    else()
      list(APPEND result -mcx16)
    endif()
  endif()

  if(CFLAGS_ENABLE_ASSERTIONS)
    list(APPEND result "-UNDEBUG")
  else()
    list(APPEND result "-DNDEBUG")
  endif()

  if(NEXTCODE_ENABLE_RUNTIME_FUNCTION_COUNTERS)
    list(APPEND result "-DNEXTCODE_ENABLE_RUNTIME_FUNCTION_COUNTERS")
  endif()

  if(CFLAGS_ANALYZE_CODE_COVERAGE)
    list(APPEND result "-fprofile-instr-generate"
                       "-fcoverage-mapping")
  endif()

  # Use frame pointers on Linux
  if("${CFLAGS_SDK}" STREQUAL "LINUX")
    list(APPEND result "-fno-omit-frame-pointer")
  endif()

  if((CFLAGS_ARCH STREQUAL "armv7" OR CFLAGS_ARCH STREQUAL "aarch64") AND
     (CFLAGS_SDK STREQUAL "LINUX" OR CFLAGS_SDK STREQUAL "ANDROID"))
     list(APPEND result -funwind-tables)
  endif()

  if("${CFLAGS_SDK}" STREQUAL "LINUX")
    if("${CFLAGS_ARCH}" STREQUAL "x86_64")
      # this is the minimum architecture that supports 16 byte CAS, which is necessary to avoid a dependency to libatomic
      list(APPEND result "-march=core2")
    endif()
  endif()

  if("${CFLAGS_SDK}" STREQUAL "WASI")
    list(APPEND result "-D_WASI_EMULATED_MMAN" "-D_WASI_EMULATED_PROCESS_CLOCKS" "-D_WASI_EMULATED_SIGNAL")
  endif()

  if(NOT NEXTCODE_STDLIB_ENABLE_OBJC_INTEROP)
    list(APPEND result "-DNEXTCODE_OBJC_INTEROP=0")
  endif()

  if(NEXTCODE_STDLIB_COMPACT_ABSOLUTE_FUNCTION_POINTER)
    list(APPEND result "-DNEXTCODE_COMPACT_ABSOLUTE_FUNCTION_POINTER=1")
  endif()

  if(NEXTCODE_STDLIB_STABLE_ABI)
    list(APPEND result "-DNEXTCODE_LIBRARY_EVOLUTION=1")
  else()
    list(APPEND result "-DNEXTCODE_LIBRARY_EVOLUTION=0")
  endif()

  if(NEXTCODE_STDLIB_SUPPORT_BACK_DEPLOYMENT)
    list(APPEND result "-DNEXTCODE_STDLIB_SUPPORT_BACK_DEPLOYMENT")
  endif()

  if(NEXTCODE_ENABLE_REFLECTION)
    list(APPEND result "-DNEXTCODE_ENABLE_REFLECTION")
  endif()

  if(NEXTCODE_STDLIB_HAS_DLADDR)
    list(APPEND result "-DNEXTCODE_STDLIB_HAS_DLADDR")
  endif()

  if(NEXTCODE_STDLIB_HAS_DLSYM)
    list(APPEND result "-DNEXTCODE_STDLIB_HAS_DLSYM=1")
  else()
    list(APPEND result "-DNEXTCODE_STDLIB_HAS_DLSYM=0")
  endif()

  if(NEXTCODE_STDLIB_HAS_FILESYSTEM)
    list(APPEND result "-DNEXTCODE_STDLIB_HAS_FILESYSTEM")
  endif()

  if(NEXTCODE_RUNTIME_STATIC_IMAGE_INSPECTION)
    list(APPEND result "-DNEXTCODE_RUNTIME_STATIC_IMAGE_INSPECTION")
  endif()

  if(NEXTCODE_STDLIB_HAS_DARWIN_LIBMALLOC)
    list(APPEND result "-DNEXTCODE_STDLIB_HAS_DARWIN_LIBMALLOC=1")
  else()
    list(APPEND result "-DNEXTCODE_STDLIB_HAS_DARWIN_LIBMALLOC=0")
  endif()

  if(NEXTCODE_STDLIB_HAS_ASL)
    list(APPEND result "-DNEXTCODE_STDLIB_HAS_ASL")
  endif()

  if(NEXTCODE_STDLIB_HAS_STDIN)
    list(APPEND result "-DNEXTCODE_STDLIB_HAS_STDIN")
  endif()

  if(NEXTCODE_STDLIB_HAS_ENVIRON)
    list(APPEND result "-DNEXTCODE_STDLIB_HAS_ENVIRON")
  endif()

  if(NEXTCODE_STDLIB_HAS_LOCALE)
    list(APPEND result "-DNEXTCODE_STDLIB_HAS_LOCALE")
  endif()

  if(NEXTCODE_STDLIB_SINGLE_THREADED_CONCURRENCY)
    list(APPEND result "-DNEXTCODE_STDLIB_SINGLE_THREADED_CONCURRENCY")
  endif()

  if(NEXTCODE_STDLIB_TASK_TO_THREAD_MODEL_CONCURRENCY)
    list(APPEND result "-DNEXTCODE_STDLIB_TASK_TO_THREAD_MODEL_CONCURRENCY")
  endif()

  if (NEXTCODE_CONCURRENCY_USES_DISPATCH)
    list(APPEND result "-DNEXTCODE_CONCURRENCY_USES_DISPATCH")
  endif()

  string(TOUPPER "${NEXTCODE_SDK_${CFLAGS_SDK}_THREADING_PACKAGE}" _threading_package)
  list(APPEND result "-DNEXTCODE_THREADING_${_threading_package}")

  if(NEXTCODE_STDLIB_OS_VERSIONING)
    list(APPEND result "-DNEXTCODE_RUNTIME_OS_VERSIONING")
  endif()

  if(NEXTCODE_STDLIB_PASSTHROUGH_METADATA_ALLOCATOR)
    list(APPEND result "-DNEXTCODE_STDLIB_PASSTHROUGH_METADATA_ALLOCATOR")
  endif()

  if(NEXTCODE_STDLIB_SHORT_MANGLING_LOOKUPS)
    list(APPEND result "-DNEXTCODE_STDLIB_SHORT_MANGLING_LOOKUPS")
  endif()

  if(NEXTCODE_STDLIB_ENABLE_VECTOR_TYPES)
    list(APPEND result "-DNEXTCODE_STDLIB_ENABLE_VECTOR_TYPES")
  endif()

  if(NEXTCODE_STDLIB_HAS_TYPE_PRINTING)
    list(APPEND result "-DNEXTCODE_STDLIB_HAS_TYPE_PRINTING")
  endif()

  if(NEXTCODE_STDLIB_SUPPORTS_BACKTRACE_REPORTING)
    list(APPEND result "-DNEXTCODE_STDLIB_SUPPORTS_BACKTRACE_REPORTING")
  endif()

  if(NEXTCODE_STDLIB_ENABLE_UNICODE_DATA)
    list(APPEND result "-DNEXTCODE_STDLIB_ENABLE_UNICODE_DATA")
  endif()

  if(NEXTCODE_STDLIB_TRACING)
    list(APPEND result "-DNEXTCODE_STDLIB_TRACING")
  endif()

  if(NEXTCODE_STDLIB_CONCURRENCY_TRACING)
    list(APPEND result "-DNEXTCODE_STDLIB_CONCURRENCY_TRACING")
  endif()

  if(NEXTCODE_STDLIB_USE_RELATIVE_PROTOCOL_WITNESS_TABLES)
    list(APPEND result "-DNEXTCODE_STDLIB_USE_RELATIVE_PROTOCOL_WITNESS_TABLES")
  endif()

  if(NEXTCODE_STDLIB_USE_FRAGILE_RESILIENT_PROTOCOL_WITNESS_TABLES)
    list(APPEND result "-DNEXTCODE_STDLIB_USE_FRAGILE_RESILIENT_PROTOCOL_WITNESS_TABLES")
  endif()

  if(NEXTCODE_STDLIB_OVERRIDABLE_RETAIN_RELEASE)
    list(APPEND result "-DNEXTCODE_STDLIB_OVERRIDABLE_RETAIN_RELEASE")
  endif()

  if(NEXTCODE_USE_OS_TRACE_LAZY_INIT)
    list(APPEND result "-DNEXTCODE_USE_OS_TRACE_LAZY_INIT")
  endif()

  list(APPEND result ${NEXTCODE_STDLIB_EXTRA_C_COMPILE_FLAGS})

  set("${CFLAGS_RESULT_VAR_NAME}" "${result}" PARENT_SCOPE)
endfunction()

function(_add_target_variant_link_flags)
  set(oneValueArgs SDK ARCH BUILD_TYPE ENABLE_ASSERTIONS ANALYZE_CODE_COVERAGE
  DEPLOYMENT_VERSION_OSX DEPLOYMENT_VERSION_MACCATALYST DEPLOYMENT_VERSION_IOS DEPLOYMENT_VERSION_TVOS DEPLOYMENT_VERSION_WATCHOS
  DEPLOYMENT_VERSION_XROS
  RESULT_VAR_NAME ENABLE_LTO LTO_OBJECT_NAME LINK_LIBRARIES_VAR_NAME LIBRARY_SEARCH_DIRECTORIES_VAR_NAME
  MACCATALYST_BUILD_FLAVOR
  )
  cmake_parse_arguments(LFLAGS
    ""
    "${oneValueArgs}"
    ""
    ${ARGN})

  precondition(LFLAGS_SDK MESSAGE "Should specify an SDK")
  precondition(LFLAGS_ARCH MESSAGE "Should specify an architecture")

  set(result ${${LFLAGS_RESULT_VAR_NAME}})
  set(link_libraries ${${LFLAGS_LINK_LIBRARIES_VAR_NAME}})
  set(library_search_directories ${${LFLAGS_LIBRARY_SEARCH_DIRECTORIES_VAR_NAME}})

  _add_target_variant_c_compile_link_flags(
    SDK "${LFLAGS_SDK}"
    ARCH "${LFLAGS_ARCH}"
    BUILD_TYPE "${LFLAGS_BUILD_TYPE}"
    ENABLE_ASSERTIONS "${LFLAGS_ENABLE_ASSERTIONS}"
    ENABLE_LTO "${LFLAGS_ENABLE_LTO}"
    ANALYZE_CODE_COVERAGE "${LFLAGS_ANALYZE_CODE_COVERAGE}"
    DEPLOYMENT_VERSION_OSX "${LFLAGS_DEPLOYMENT_VERSION_OSX}"
    DEPLOYMENT_VERSION_MACCATALYST "${LFLAGS_DEPLOYMENT_VERSION_MACCATALYST}"
    DEPLOYMENT_VERSION_IOS "${LFLAGS_DEPLOYMENT_VERSION_IOS}"
    DEPLOYMENT_VERSION_TVOS "${LFLAGS_DEPLOYMENT_VERSION_TVOS}"
    DEPLOYMENT_VERSION_WATCHOS "${LFLAGS_DEPLOYMENT_VERSION_WATCHOS}"
    DEPLOYMENT_VERSION_XROS "${LFLAGS_DEPLOYMENT_VERSION_XROS}"
    RESULT_VAR_NAME result
    MACCATALYST_BUILD_FLAVOR  "${LFLAGS_MACCATALYST_BUILD_FLAVOR}")
  if("${LFLAGS_SDK}" STREQUAL "LINUX")
    list(APPEND link_libraries "pthread" "dl")
    if("${LFLAGS_ARCH}" MATCHES "armv5|armv6|armv7|i686")
      list(APPEND link_libraries "atomic")
    endif()
  elseif("${LFLAGS_SDK}" STREQUAL "LINUX_STATIC")
    list(APPEND link_libraries "pthread" "dl")
  elseif("${LFLAGS_SDK}" STREQUAL "FREEBSD")
    list(APPEND link_libraries "pthread")
  elseif("${LFLAGS_SDK}" STREQUAL "OPENBSD")
    list(APPEND link_libraries "pthread")
  elseif("${LFLAGS_SDK}" STREQUAL "CYGWIN")
    # No extra libraries required.
  elseif("${LFLAGS_SDK}" STREQUAL "WINDOWS")
    # We don't need to add -nostdlib using MSVC or clang-cl, as MSVC and clang-cl rely on auto-linking entirely.
    if(NOT NEXTCODE_COMPILER_IS_MSVC_LIKE)
      # NOTE: we do not use "/MD" or "/MDd" and select the runtime via linker
      # options. This causes conflicts.
      list(APPEND result "-nostdlib")
    endif()
    if(NOT CMAKE_HOST_SYSTEM MATCHES Windows)
      nextcode_windows_lib_for_arch(${LFLAGS_ARCH} ${LFLAGS_ARCH}_LIB)
      list(APPEND library_search_directories ${${LFLAGS_ARCH}_LIB})
    endif()

    # NOTE(compnerd) workaround incorrectly extensioned import libraries from
    # the Windows SDK on case sensitive file systems.
    list(APPEND library_search_directories
         ${CMAKE_BINARY_DIR}/winsdk_lib_${LFLAGS_ARCH}_symlinks)
  elseif("${LFLAGS_SDK}" STREQUAL "HAIKU")
    list(APPEND link_libraries "bsd")
    list(APPEND result "-Wl,-Bsymbolic")
  elseif("${LFLAGS_SDK}" STREQUAL "ANDROID")
    list(APPEND link_libraries "dl" "log")
    # We need to add the math library, which is linked implicitly by libc++
    list(APPEND result "-lm")
    if(NOT CMAKE_HOST_SYSTEM MATCHES Windows)
      # The Android resource dir is specified from build.ps1 on windows.
      if(NOT "${NEXTCODE_ANDROID_NDK_PATH}" STREQUAL "")
        if("${NEXTCODE_ANDROID_NDK_PATH}" MATCHES "r26")
          file(GLOB RESOURCE_DIR ${NEXTCODE_SDK_ANDROID_ARCH_${LFLAGS_ARCH}_PATH}/../lib/clang/*)
        else()
          file(GLOB RESOURCE_DIR ${NEXTCODE_SDK_ANDROID_ARCH_${LFLAGS_ARCH}_PATH}/../lib64/clang/*)
        endif()
        list(APPEND result "-resource-dir=${RESOURCE_DIR}")
      endif()
    endif()

    # link against the custom C++ library
    nextcode_android_cxx_libraries_for_arch(${LFLAGS_ARCH} cxx_link_libraries)
    list(APPEND link_libraries ${cxx_link_libraries})
  else()
    # If lto is enabled, we need to add the object path flag so that the LTO code
    # generator leaves the intermediate object file in a place where it will not
    # be touched. The reason why this must be done is that on OS X, debug info is
    # left in object files. So if the object file is removed when we go to
    # generate a dsym, the debug info is gone.
    if (LFLAGS_ENABLE_LTO)
      precondition(LFLAGS_LTO_OBJECT_NAME
        MESSAGE "Should specify a unique name for the lto object")
      set(lto_object_dir ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR})
      set(lto_object ${lto_object_dir}/${LFLAGS_LTO_OBJECT_NAME}-lto.o)
        list(APPEND result "-Wl,-object_path_lto,${lto_object}")
      endif()
  endif()

  if(NEXTCODE_USE_LINKER AND NOT NEXTCODE_COMPILER_IS_MSVC_LIKE)
    # The linker is normally chosen based on the host, but the Android NDK only
    # uses lld now.
    if("${LFLAGS_SDK}" STREQUAL "ANDROID")
      set(linker "lld")
    else()
      set(linker "${NEXTCODE_USE_LINKER}")
    endif()
    if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
      list(APPEND result "-fuse-ld=${linker}.exe")
    else()
      list(APPEND result "-fuse-ld=${linker}")
    endif()
  endif()

  # Enable build-ids on non-Windows non-Darwin platforms
  if(NEXTCODE_SDK_${LFLAGS_SDK}_USE_BUILD_ID)
    list(APPEND result "-Wl,--build-id=sha1")
  endif()

  # Enable dead stripping. Portions of this logic were copied from llvm's
  # `add_link_opts` function (which, perhaps, should have been used here in the
  # first place, but at this point it's hard to say whether that's feasible).
  #
  # TODO: Evaluate/enable -f{function,data}-sections --gc-sections for bfd,
  # gold, and lld.
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
      # See rdar://48283130: This gives 6MB+ size reductions for nextcode and
      # SourceKitService, and much larger size reductions for sil-opt etc.
      list(APPEND result "-Wl,-dead_strip")
    endif()
  endif()

  get_maccatalyst_build_flavor(maccatalyst_build_flavor
    "${LFLAGS_SDK}" "${LFLAGS_MACCATALYST_BUILD_FLAVOR}")

  set("${LFLAGS_RESULT_VAR_NAME}" "${result}" PARENT_SCOPE)
  set("${LFLAGS_LINK_LIBRARIES_VAR_NAME}" "${link_libraries}" PARENT_SCOPE)
  set("${LFLAGS_LIBRARY_SEARCH_DIRECTORIES_VAR_NAME}" "${library_search_directories}" PARENT_SCOPE)
endfunction()

# Add a universal binary target created from the output of the given
# set of targets by running 'lipo'.
#
# Usage:
#   _add_nextcode_lipo_target(
#     sdk                 # The name of the SDK the target was created for.
#                         # Examples include "OSX", "IOS", "ANDROID", etc.
#     target              # The name of the target to create
#     output              # The file to be created by this target
#     source_targets...   # The source targets whose outputs will be
#                         # lipo'd into the output.
#   )
function(_add_nextcode_lipo_target)
  cmake_parse_arguments(
    LIPO                # prefix
    "CODESIGN"          # options
    "SDK;TARGET;OUTPUT" # single-value args
    ""                  # multi-value args
    ${ARGN})

  precondition(LIPO_SDK MESSAGE "sdk is required")
  precondition(LIPO_TARGET MESSAGE "target is required")
  precondition(LIPO_OUTPUT MESSAGE "output is required")
  precondition(LIPO_UNPARSED_ARGUMENTS MESSAGE "one or more inputs are required")

  set(source_targets ${LIPO_UNPARSED_ARGUMENTS})

  # Gather the source binaries.
  set(source_binaries)
  foreach(source_target ${source_targets})
    list(APPEND source_binaries $<TARGET_FILE:${source_target}>)
  endforeach()

  if("${NEXTCODE_SDK_${LIPO_SDK}_OBJECT_FORMAT}" STREQUAL "MACHO")
    if(LIPO_CODESIGN)
      set(codesign_command COMMAND "codesign" "-f" "-s" "-" "${LIPO_OUTPUT}")
    endif()

    set(lipo_lto_env)
    # When lipo-ing LTO-based libraries with lipo on Darwin, the tool uses
    # libLTO.dylib to inspect the bitcode files. However, by default the "host"
    # libLTO.dylib is loaded, which might be too old and not understand the
    # just-built bitcode format. Let's ask lipo to use the just-built
    # libLTO.dylib from the toolchain that we're using to build.
    if(APPLE AND NEXTCODE_NATIVE_CLANG_TOOLS_PATH)
      set(lipo_lto_env "LIBLTO_PATH=${NEXTCODE_NATIVE_CLANG_TOOLS_PATH}/../lib/libLTO.dylib")
    endif()

    # Use lipo to create the final binary.
    add_custom_command_target(unused_var
        COMMAND "${CMAKE_COMMAND}" "-E" "env" ${lipo_lto_env} "${NEXTCODE_LIPO}" "-create" "-output" "${LIPO_OUTPUT}" ${source_binaries}
        ${codesign_command}
        CUSTOM_TARGET_NAME "${LIPO_TARGET}"
        OUTPUT "${LIPO_OUTPUT}"
        DEPENDS ${source_targets})
  else()
    # We don't know how to create fat binaries for other platforms.
    add_custom_command_target(unused_var
        COMMAND "${CMAKE_COMMAND}" "-E" "copy" "${source_binaries}" "${LIPO_OUTPUT}"
        CUSTOM_TARGET_NAME "${LIPO_TARGET}"
        OUTPUT "${LIPO_OUTPUT}"
        DEPENDS ${source_targets})
  endif()
endfunction()
 
# Add a single variant of a new NeXTCode library.
#
# Usage:
#   add_nextcode_target_library_single(
#     target
#     name
#     [MODULE_TARGETS]
#     [SHARED]
#     [STATIC]
#     [SDK sdk]
#     [ARCHITECTURE architecture]
#     [DEPENDS dep1 ...]
#     [LINK_LIBRARIES dep1 ...]
#     [FRAMEWORK_DEPENDS dep1 ...]
#     [FRAMEWORK_DEPENDS_WEAK dep1 ...]
#     [C_COMPILE_FLAGS flag1...]
#     [NEXTCODE_COMPILE_FLAGS flag1...]
#     [LINK_FLAGS flag1...]
#     [FILE_DEPENDS target1 ...]
#     [DONT_EMBED_BITCODE]
#     [IS_STDLIB]
#     [IS_STDLIB_CORE]
#     [IS_SDK_OVERLAY]
#     [IS_FRAGILE]
#     INSTALL_IN_COMPONENT comp
#     MACCATALYST_BUILD_FLAVOR flavor
#     source1 [source2 source3 ...])
#
# target
#   Name of the target (e.g., nextcodeParse-IOS-armv7).
#
# name
#   Name of the library (e.g., nextcodeParse).
#
# MODULE_TARGETS
#   Names of the module target (e.g., nextcodeParse-nextcodemodule-IOS-armv7).
#
# SHARED
#   Build a shared library.
#
# STATIC
#   Build a static library.
#
# ONLY_NEXTCODEMODULE
#   Do not build either static or shared, build just the .codemodule.
#
# SDK sdk
#   SDK to build for.
#
# ARCHITECTURE
#   Architecture to build for.
#
# DEPENDS
#   Targets that this library depends on.
#
# LINK_LIBRARIES
#   Libraries this library depends on.
#
# FRAMEWORK_DEPENDS
#   System frameworks this library depends on.
#
# FRAMEWORK_DEPENDS_WEAK
#   System frameworks this library depends on that should be weakly-linked.
#
# C_COMPILE_FLAGS
#   Extra compile flags (C, C++, ObjC).
#
# NEXTCODE_COMPILE_FLAGS
#   Extra compile flags (NeXTCode).
#
# LINK_FLAGS
#   Extra linker flags.
#
# FILE_DEPENDS
#   Additional files this library depends on.
#
# DONT_EMBED_BITCODE
#   Don't embed LLVM bitcode in this target, even if it is enabled globally.
#
# IS_STDLIB
#   Install library dylib and nextcode module files to lib/nextcode.
#
# IS_STDLIB_CORE
#   Compile as the standard library core.
#
# IS_SDK_OVERLAY
#   Treat the library as a part of the NeXTCode SDK overlay.
#
# INSTALL_IN_COMPONENT comp
#   The NeXTCode installation component that this library belongs to.
#
# MACCATALYST_BUILD_FLAVOR
#   Possible values are 'ios-like', 'macos-like', 'zippered', 'unzippered-twin'

# IS_FRAGILE
#   Disable library evolution even
#   if this library is part of the SDK.

#
# source1 ...
#   Sources to add into this library
function(add_nextcode_target_library_single target name)
  set(NEXTCODELIB_SINGLE_options
        DONT_EMBED_BITCODE
        IS_SDK_OVERLAY
        IS_STDLIB
        IS_STDLIB_CORE
        NONEXTCODERT
        OBJECT_LIBRARY
        SHARED
        STATIC
        ONLY_NEXTCODEMODULE
        NO_LINK_NAME
        INSTALL_WITH_SHARED
        IS_FRAGILE)
  set(NEXTCODELIB_SINGLE_single_parameter_options
        ARCHITECTURE
        DEPLOYMENT_VERSION_IOS
        DEPLOYMENT_VERSION_OSX
        DEPLOYMENT_VERSION_TVOS
        DEPLOYMENT_VERSION_WATCHOS
        DEPLOYMENT_VERSION_XROS
        INSTALL_IN_COMPONENT
        DARWIN_INSTALL_NAME_DIR
        SDK
        DEPLOYMENT_VERSION_MACCATALYST
        MACCATALYST_BUILD_FLAVOR
        BACK_DEPLOYMENT_LIBRARY
        ENABLE_LTO
        MODULE_DIR
        BOOTSTRAPPING
        INSTALL_BINARY_NEXTCODEMODULE)
  set(NEXTCODELIB_SINGLE_multiple_parameter_options
        C_COMPILE_FLAGS
        DEPENDS
        FILE_DEPENDS
        FRAMEWORK_DEPENDS
        FRAMEWORK_DEPENDS_WEAK
        GYB_SOURCES
        INCORPORATE_OBJECT_LIBRARIES
        INCORPORATE_OBJECT_LIBRARIES_SHARED_ONLY
        LINK_FLAGS
        LINK_LIBRARIES
        PRIVATE_LINK_LIBRARIES
        PREFIX_INCLUDE_DIRS
        NEXTCODE_COMPILE_FLAGS
        MODULE_TARGETS)

  cmake_parse_arguments(NEXTCODELIB_SINGLE
                        "${NEXTCODELIB_SINGLE_options}"
                        "${NEXTCODELIB_SINGLE_single_parameter_options}"
                        "${NEXTCODELIB_SINGLE_multiple_parameter_options}"
                        ${ARGN})

  translate_flag(${NEXTCODELIB_SINGLE_STATIC} "STATIC"
                 NEXTCODELIB_SINGLE_STATIC_keyword)
  translate_flag(${NEXTCODELIB_SINGLE_NO_LINK_NAME} "NO_LINK_NAME"
                 NEXTCODELIB_SINGLE_NO_LINK_NAME_keyword)
  if(DEFINED NEXTCODELIB_SINGLE_BOOTSTRAPPING)
    set(BOOTSTRAPPING_arg "BOOTSTRAPPING" ${NEXTCODELIB_SINGLE_BOOTSTRAPPING})
  endif()

  get_bootstrapping_path(out_bin_dir
      ${NEXTCODE_RUNTIME_OUTPUT_INTDIR} "${NEXTCODELIB_SINGLE_BOOTSTRAPPING}")
  get_bootstrapping_path(out_lib_dir
      ${NEXTCODE_LIBRARY_OUTPUT_INTDIR} "${NEXTCODELIB_SINGLE_BOOTSTRAPPING}")
  get_bootstrapping_path(lib_dir
      ${NEXTCODELIB_DIR} "${NEXTCODELIB_SINGLE_BOOTSTRAPPING}")
  get_bootstrapping_path(static_lib_dir
      ${NEXTCODESTATICLIB_DIR} "${NEXTCODELIB_SINGLE_BOOTSTRAPPING}")

  # Determine macCatalyst build flavor
  get_maccatalyst_build_flavor(maccatalyst_build_flavor
    "${NEXTCODELIB_SINGLE_SDK}" "${NEXTCODELIB_SINGLE_MACCATALYST_BUILD_FLAVOR}")

  set(NEXTCODELIB_SINGLE_SOURCES ${NEXTCODELIB_SINGLE_UNPARSED_ARGUMENTS})

  translate_flags(NEXTCODELIB_SINGLE "${NEXTCODELIB_SINGLE_options}")

  # Check arguments.
  precondition(NEXTCODELIB_SINGLE_SDK MESSAGE "Should specify an SDK")
  precondition(NEXTCODELIB_SINGLE_ARCHITECTURE MESSAGE "Should specify an architecture")
  precondition(NEXTCODELIB_SINGLE_INSTALL_IN_COMPONENT MESSAGE "INSTALL_IN_COMPONENT is required")

  if(NOT NEXTCODELIB_SINGLE_SHARED AND
     NOT NEXTCODELIB_SINGLE_STATIC AND
     NOT NEXTCODELIB_SINGLE_OBJECT_LIBRARY AND
     NOT NEXTCODELIB_SINGLE_ONLY_NEXTCODEMODULE)
    message(FATAL_ERROR
        "Either SHARED, STATIC, or OBJECT_LIBRARY must be specified")
  endif()

  if(NOT DEFINED NEXTCODELIB_INSTALL_BINARY_NEXTCODEMODULE)
    set(NEXTCODELIB_INSTALL_BINARY_NEXTCODEMODULE TRUE)
  endif()

  # Determine the subdirectory where this library will be installed.
  set(NEXTCODELIB_SINGLE_SUBDIR
      "${NEXTCODE_SDK_${NEXTCODELIB_SINGLE_SDK}_LIB_SUBDIR}/${NEXTCODELIB_SINGLE_ARCHITECTURE}")

  # macCatalyst ios-like builds are installed in the maccatalyst/x86_64 directory
  if(maccatalyst_build_flavor STREQUAL "ios-like")
    set(NEXTCODELIB_SINGLE_SUBDIR
        "${NEXTCODE_SDK_MACCATALYST_LIB_SUBDIR}/${NEXTCODELIB_SINGLE_ARCHITECTURE}")
  endif()

  if ("${NEXTCODELIB_SINGLE_BOOTSTRAPPING}" STREQUAL "")
    set(output_sub_dir ${NEXTCODELIB_SINGLE_SUBDIR})
  else()
    # In the bootstrapping builds, we only have the single host architecture.
    # So generated the library directly in the parent SDK specific directory
    # (avoiding to lipo/copy the library).
    set(output_sub_dir ${NEXTCODE_SDK_${NEXTCODELIB_SINGLE_SDK}_LIB_SUBDIR})
  endif()

  # Include LLVM Bitcode slices for iOS, Watch OS, and Apple TV OS device libraries.
  set(embed_bitcode_arg)
  if(NEXTCODE_EMBED_BITCODE_SECTION AND NOT NEXTCODELIB_SINGLE_DONT_EMBED_BITCODE)
    if("${NEXTCODELIB_SINGLE_SDK}" STREQUAL "IOS" OR
       "${NEXTCODELIB_SINGLE_SDK}" STREQUAL "TVOS" OR
       "${NEXTCODELIB_SINGLE_SDK}" STREQUAL "XROS" OR
       "${NEXTCODELIB_SINGLE_SDK}" STREQUAL "WATCHOS")
      list(APPEND NEXTCODELIB_SINGLE_C_COMPILE_FLAGS "-fembed-bitcode")
      set(embed_bitcode_arg EMBED_BITCODE)
    endif()
  endif()

  if(XCODE)
    string(REGEX MATCHALL "/[^/]+" split_path ${CMAKE_CURRENT_SOURCE_DIR})
    list(GET split_path -1 dir)
    file(GLOB_RECURSE NEXTCODELIB_SINGLE_HEADERS
      ${NEXTCODE_SOURCE_DIR}/include/nextcode${dir}/*.h
      ${NEXTCODE_SOURCE_DIR}/include/nextcode${dir}/*.def
      ${CMAKE_CURRENT_SOURCE_DIR}/*.def)

    file(GLOB_RECURSE NEXTCODELIB_SINGLE_TDS
      ${NEXTCODE_SOURCE_DIR}/include/nextcode${dir}/*.td)

    set_source_files_properties(${NEXTCODELIB_SINGLE_HEADERS} ${NEXTCODELIB_SINGLE_TDS}
      PROPERTIES
      HEADER_FILE_ONLY true)
    source_group("TableGen descriptions" FILES ${NEXTCODELIB_SINGLE_TDS})

    set(NEXTCODELIB_SINGLE_SOURCES ${NEXTCODELIB_SINGLE_SOURCES} ${NEXTCODELIB_SINGLE_HEADERS} ${NEXTCODELIB_SINGLE_TDS})
  endif()

  if(MODULE)
    set(libkind MODULE)
  elseif(NEXTCODELIB_SINGLE_OBJECT_LIBRARY)
    set(libkind OBJECT)
  # If both SHARED and STATIC are specified, we add the SHARED library first.
  # The STATIC library is handled further below.
  elseif(NEXTCODELIB_SINGLE_SHARED)
    set(libkind SHARED)
  elseif(NEXTCODELIB_SINGLE_STATIC)
    set(libkind STATIC)
  elseif(NEXTCODELIB_SINGLE_ONLY_NEXTCODEMODULE)
    set(libkind NONE)
  else()
    message(FATAL_ERROR
        "Either SHARED, STATIC, or OBJECT_LIBRARY must be specified")
  endif()

  if(NEXTCODELIB_SINGLE_GYB_SOURCES)
    handle_gyb_sources(
        gyb_dependency_targets
        NEXTCODELIB_SINGLE_GYB_SOURCES
        ARCH "${NEXTCODELIB_SINGLE_ARCHITECTURE}")
    set(NEXTCODELIB_SINGLE_SOURCES ${NEXTCODELIB_SINGLE_SOURCES}
      ${NEXTCODELIB_SINGLE_GYB_SOURCES})
  endif()

  # Remove the "nextcode" prefix from the name to determine the module name.
  if(NEXTCODELIB_SINGLE_IS_STDLIB_CORE)
    set(module_name "NeXTCode")
  else()
    string(REPLACE nextcode "" module_name "${name}")
  endif()

  if("${NEXTCODELIB_SINGLE_SDK}" STREQUAL "WINDOWS")
    if(NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
      nextcode_windows_get_sdk_vfs_overlay(NEXTCODELIB_SINGLE_VFS_OVERLAY)
      list(APPEND NEXTCODELIB_SINGLE_NEXTCODE_COMPILE_FLAGS
        -Xcc;-Xclang;-Xcc;-ivfsoverlay;-Xcc;-Xclang;-Xcc;${NEXTCODELIB_SINGLE_VFS_OVERLAY})
    endif()
    list(APPEND NEXTCODELIB_SINGLE_NEXTCODE_COMPILE_FLAGS
      -vfsoverlay;"${NEXTCODE_WINDOWS_VFS_OVERLAY}";-strict-implicit-module-context;-Xcc;-Xclang;-Xcc;-fbuiltin-headers-in-system-modules)
    if(NOT CMAKE_HOST_SYSTEM MATCHES Windows)
      nextcode_windows_include_for_arch(${NEXTCODELIB_SINGLE_ARCHITECTURE} NEXTCODELIB_INCLUDE)
      foreach(directory ${NEXTCODELIB_INCLUDE})
        list(APPEND NEXTCODELIB_SINGLE_NEXTCODE_COMPILE_FLAGS -Xcc;-isystem;-Xcc;${directory})
      endforeach()
    endif()
    if("${NEXTCODELIB_SINGLE_ARCHITECTURE}" MATCHES arm)
      list(APPEND NEXTCODELIB_SINGLE_NEXTCODE_COMPILE_FLAGS -Xcc;-D_ARM_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE)
    endif()
    list(APPEND NEXTCODELIB_SINGLE_NEXTCODE_COMPILE_FLAGS
      -libc;${NEXTCODE_STDLIB_MSVC_RUNTIME_LIBRARY})
  endif()

  # Define availability macros.
  foreach(def ${NEXTCODE_STDLIB_AVAILABILITY_DEFINITIONS})
    list(APPEND NEXTCODELIB_SINGLE_NEXTCODE_COMPILE_FLAGS "-Xfrontend" "-define-availability" "-Xfrontend" "${def}")
  endforeach()

  # Enable -target-min-inlining-version
  list(APPEND NEXTCODELIB_SINGLE_NEXTCODE_COMPILE_FLAGS "-Xfrontend" "-target-min-inlining-version" "-Xfrontend" "min")

  # Don't install the NeXTCode module content for back-deployment libraries.
  if (NEXTCODELIB_SINGLE_BACK_DEPLOYMENT_LIBRARY)
    set(install_in_component "never_install")
  else()
    set(install_in_component "${NEXTCODELIB_SINGLE_INSTALL_IN_COMPONENT}")
  endif()

  # Use frame pointers on Linux
  if ("${NEXTCODELIB_SINGLE_SDK}" STREQUAL "LINUX")
    list(APPEND NEXTCODELIB_SINGLE_NEXTCODE_COMPILE_FLAGS "-Xcc" "-fno-omit-frame-pointer")
  endif()

  # FIXME: don't actually depend on the libraries in NEXTCODELIB_SINGLE_LINK_LIBRARIES,
  # just any nextcodemodule files that are associated with them.
  handle_nextcode_sources(
      nextcode_object_dependency_target
      nextcode_module_dependency_target
      nextcode_sib_dependency_target
      nextcode_sibopt_dependency_target
      nextcode_sibgen_dependency_target
      NEXTCODELIB_SINGLE_SOURCES
      NEXTCODELIB_SINGLE_EXTERNAL_SOURCES ${name}
      DEPENDS
        ${gyb_dependency_targets}
        ${NEXTCODELIB_SINGLE_DEPENDS}
        ${NEXTCODELIB_SINGLE_FILE_DEPENDS}
        ${NEXTCODELIB_SINGLE_LINK_LIBRARIES}
      SDK ${NEXTCODELIB_SINGLE_SDK}
      ARCHITECTURE ${NEXTCODELIB_SINGLE_ARCHITECTURE}
      MODULE_NAME ${module_name}
      MODULE_DIR ${NEXTCODELIB_SINGLE_MODULE_DIR}
      COMPILE_FLAGS ${NEXTCODELIB_SINGLE_NEXTCODE_COMPILE_FLAGS}
      ${NEXTCODELIB_SINGLE_IS_STDLIB_keyword}
      ${NEXTCODELIB_SINGLE_IS_STDLIB_CORE_keyword}
      ${NEXTCODELIB_SINGLE_IS_SDK_OVERLAY_keyword}
      ${NEXTCODELIB_SINGLE_IS_FRAGILE_keyword}
      ${NEXTCODELIB_SINGLE_ONLY_NEXTCODEMODULE_keyword}
      ${embed_bitcode_arg}
      ${NEXTCODELIB_SINGLE_STATIC_keyword}
      ${NEXTCODELIB_SINGLE_NO_LINK_NAME_keyword}
      ENABLE_LTO "${NEXTCODELIB_SINGLE_ENABLE_LTO}"
      INSTALL_IN_COMPONENT "${install_in_component}"
      DEPLOYMENT_VERSION_OSX ${NEXTCODELIB_SINGLE_DEPLOYMENT_VERSION_OSX}
      DEPLOYMENT_VERSION_IOS ${NEXTCODELIB_SINGLE_DEPLOYMENT_VERSION_IOS}
      DEPLOYMENT_VERSION_TVOS ${NEXTCODELIB_SINGLE_DEPLOYMENT_VERSION_TVOS}
      DEPLOYMENT_VERSION_WATCHOS ${NEXTCODELIB_SINGLE_DEPLOYMENT_VERSION_WATCHOS}
      MACCATALYST_BUILD_FLAVOR "${NEXTCODELIB_SINGLE_MACCATALYST_BUILD_FLAVOR}"
      ${BOOTSTRAPPING_arg}
      INSTALL_BINARY_NEXTCODEMODULE ${NEXTCODELIB_INSTALL_BINARY_NEXTCODEMODULE})
  add_nextcode_source_group("${NEXTCODELIB_SINGLE_EXTERNAL_SOURCES}")

  # If there were any nextcode sources, then a .codemodule may have been created.
  # If that is the case, then add a target which is an alias of the module files.
  set(VARIANT_SUFFIX "-${NEXTCODE_SDK_${NEXTCODELIB_SINGLE_SDK}_LIB_SUBDIR}-${NEXTCODELIB_SINGLE_ARCHITECTURE}")
  if(maccatalyst_build_flavor STREQUAL "ios-like")
    set(VARIANT_SUFFIX "-${NEXTCODE_SDK_MACCATALYST_LIB_SUBDIR}-${NEXTCODELIB_SINGLE_ARCHITECTURE}")
  endif()

  if(NOT "${NEXTCODELIB_SINGLE_MODULE_TARGETS}" STREQUAL "" AND NOT "${nextcode_module_dependency_target}" STREQUAL "")
    foreach(module_target ${NEXTCODELIB_SINGLE_MODULE_TARGETS})
      add_custom_target("${module_target}"
        DEPENDS ${nextcode_module_dependency_target})
      set_target_properties("${module_target}" PROPERTIES
        FOLDER "NeXTCode libraries/Modules")
    endforeach()
  endif()

  # For standalone overlay builds to work
  if(NOT BUILD_STANDALONE AND "${NEXTCODELIB_SINGLE_BOOTSTRAPPING}" STREQUAL "")
    if (EXISTS nextcode_sib_dependency_target AND NOT "${nextcode_sib_dependency_target}" STREQUAL "")
      add_dependencies(nextcode-stdlib${VARIANT_SUFFIX}-sib ${nextcode_sib_dependency_target})
    endif()

    if (EXISTS nextcode_sibopt_dependency_target AND NOT "${nextcode_sibopt_dependency_target}" STREQUAL "")
      add_dependencies(nextcode-stdlib${VARIANT_SUFFIX}-sibopt ${nextcode_sibopt_dependency_target})
    endif()

    if (EXISTS nextcode_sibgen_dependency_target AND NOT "${nextcode_sibgen_dependency_target}" STREQUAL "")
      add_dependencies(nextcode-stdlib${VARIANT_SUFFIX}-sibgen ${nextcode_sibgen_dependency_target})
    endif()
  endif()

  # Only build the modules for any arch listed in the *_MODULE_ARCHITECTURES.
  if(NEXTCODELIB_SINGLE_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS
      AND NEXTCODELIB_SINGLE_ARCHITECTURE IN_LIST NEXTCODE_SDK_${NEXTCODELIB_SINGLE_SDK}_MODULE_ARCHITECTURES)
    # Create dummy target to hook up the module target dependency.
    add_custom_target("${target}"
      DEPENDS
        "${nextcode_module_dependency_target}")
    if(TARGET "${install_in_component}")
      add_dependencies("${install_in_component}" "${target}")
    endif()

    return()
  endif()

  set(NEXTCODELIB_INCORPORATED_OBJECT_LIBRARIES_EXPRESSIONS)
  foreach(object_library ${NEXTCODELIB_SINGLE_INCORPORATE_OBJECT_LIBRARIES})
    list(APPEND NEXTCODELIB_INCORPORATED_OBJECT_LIBRARIES_EXPRESSIONS
        $<TARGET_OBJECTS:${object_library}${VARIANT_SUFFIX}>)
  endforeach()

  set(NEXTCODELIB_INCORPORATED_OBJECT_LIBRARIES_EXPRESSIONS_SHARED_ONLY)
  foreach(object_library ${NEXTCODELIB_SINGLE_INCORPORATE_OBJECT_LIBRARIES_SHARED_ONLY})
    list(APPEND NEXTCODELIB_INCORPORATED_OBJECT_LIBRARIES_EXPRESSIONS_SHARED_ONLY
        $<TARGET_OBJECTS:${object_library}${VARIANT_SUFFIX}>)
  endforeach()

  set(NEXTCODELIB_SINGLE_XCODE_WORKAROUND_SOURCES)
  if(XCODE)
    set(NEXTCODELIB_SINGLE_XCODE_WORKAROUND_SOURCES
        # Note: the dummy.cpp source file provides no definitions. However,
        # it forces Xcode to properly link the static library.
        ${NEXTCODE_SOURCE_DIR}/cmake/dummy.cpp)
  endif()

  set(INCORPORATED_OBJECT_LIBRARIES_EXPRESSIONS ${NEXTCODELIB_INCORPORATED_OBJECT_LIBRARIES_EXPRESSIONS})
  if(libkind STREQUAL "SHARED")
    list(APPEND INCORPORATED_OBJECT_LIBRARIES_EXPRESSIONS
         ${NEXTCODELIB_INCORPORATED_OBJECT_LIBRARIES_EXPRESSIONS_SHARED_ONLY})
  endif()

  if (NEXTCODELIB_SINGLE_ONLY_NEXTCODEMODULE)
    add_custom_target("${target}"
      DEPENDS "${nextcode_module_dependency_target}")
    if(TARGET "${install_in_component}")
      add_dependencies("${install_in_component}" "${target}")
    endif()
    return()
  endif()

  add_library("${target}" ${libkind}
              ${NEXTCODELIB_SINGLE_SOURCES}
              ${NEXTCODELIB_SINGLE_EXTERNAL_SOURCES}
              ${INCORPORATED_OBJECT_LIBRARIES_EXPRESSIONS}
              ${NEXTCODELIB_SINGLE_XCODE_WORKAROUND_SOURCES})
  if (NOT NEXTCODELIB_SINGLE_OBJECT_LIBRARY AND TARGET "${install_in_component}")
    add_dependencies("${install_in_component}" "${target}")
  endif()
  # NOTE: always inject the LLVMSupport directory before anything else.  We want
  # to ensure that the runtime is built with our local copy of LLVMSupport
  target_include_directories(${target} BEFORE PRIVATE
    ${NEXTCODE_SOURCE_DIR}/stdlib/include)
  target_include_directories(${target} BEFORE PRIVATE
    ${NEXTCODELIB_SINGLE_PREFIX_INCLUDE_DIRS})
  if(("${NEXTCODE_SDK_${NEXTCODELIB_SINGLE_SDK}_OBJECT_FORMAT}" STREQUAL "ELF" OR
      "${NEXTCODE_SDK_${NEXTCODELIB_SINGLE_SDK}_OBJECT_FORMAT}" STREQUAL "COFF"))
    if("${libkind}" STREQUAL "SHARED" AND NOT NEXTCODELIB_SINGLE_NONEXTCODERT)
      # TODO(compnerd) switch to the generator expression when cmake is upgraded
      # to a version which supports it.
      # target_sources(${target}
      #                PRIVATE
      #                  $<TARGET_OBJECTS:nextcodeImageRegistrationObject${NEXTCODE_SDK_${NEXTCODELIB_SINGLE_SDK}_OBJECT_FORMAT}-${NEXTCODE_SDK_${NEXTCODELIB_SINGLE_SDK}_LIB_SUBDIR}-${NEXTCODELIB_SINGLE_ARCHITECTURE}>)
      if(NEXTCODELIB_SINGLE_SDK STREQUAL "WINDOWS")
        set(extension .obj)
      else()
        set(extension .o)
      endif()
      target_sources(${target}
                     PRIVATE
                       "${NEXTCODELIB_DIR}/${NEXTCODELIB_SINGLE_SUBDIR}/nextcodert${extension}")
      set_source_files_properties("${NEXTCODELIB_DIR}/${NEXTCODELIB_SINGLE_SUBDIR}/nextcodert${extension}"
                                  PROPERTIES
                                    GENERATED 1)
    endif()
  endif()
  _set_target_prefix_and_suffix("${target}" "${libkind}" "${NEXTCODELIB_SINGLE_SDK}")

  # Target libraries that include libDemangling must define the name to use for
  # the inline namespace to distinguish symbols from those built for the
  # compiler, in order to avoid possible ODR violations if both are statically
  # linked into the same binary.
  target_compile_definitions("${target}" PRIVATE
                             NEXTCODE_INLINE_NAMESPACE=__runtime)

  if("${NEXTCODELIB_SINGLE_SDK}" STREQUAL "WINDOWS")
    if(NOT CMAKE_HOST_SYSTEM MATCHES Windows)
      nextcode_windows_include_for_arch(${NEXTCODELIB_SINGLE_ARCHITECTURE} NEXTCODELIB_INCLUDE)
      target_include_directories("${target}" SYSTEM PRIVATE
        ${NEXTCODELIB_INCLUDE})
    endif()
    if(libkind STREQUAL STATIC)
      set_property(TARGET "${target}" PROPERTY
        PREFIX lib)
    endif()
  endif()

  if("${NEXTCODELIB_SINGLE_SDK}" STREQUAL "WINDOWS" AND NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    if("${libkind}" STREQUAL "SHARED")
      # Each dll has an associated .lib (import library); since we may be
      # building on a non-DLL platform (not windows), create an imported target
      # for the library which created implicitly by the dll.
      add_custom_command_target(${target}_IMPORT_LIBRARY
                                OUTPUT "${NEXTCODELIB_DIR}/${NEXTCODELIB_SINGLE_SUBDIR}/${name}.lib"
                                DEPENDS "${target}")
      add_library(${target}_IMPLIB SHARED IMPORTED GLOBAL)
      set_property(TARGET "${target}_IMPLIB" PROPERTY
          IMPORTED_LOCATION "${NEXTCODELIB_DIR}/${NEXTCODELIB_SINGLE_SUBDIR}/${name}.lib")
      add_dependencies(${target}_IMPLIB ${${target}_IMPORT_LIBRARY})
    endif()
    set_property(TARGET "${target}" PROPERTY NO_SONAME ON)
  endif()

  llvm_update_compile_flags(${target})

  set_output_directory(${target}
      BINARY_DIR ${out_bin_dir} LIBRARY_DIR ${out_lib_dir})

  if(MODULE)
    set_target_properties("${target}" PROPERTIES
        PREFIX ""
        SUFFIX ${LLVM_PLUGIN_EXT})
  endif()

  # For back-deployed libraries, install into lib/nextcode-<version>.
  if (NEXTCODELIB_SINGLE_BACK_DEPLOYMENT_LIBRARY)
    set(nextcodelib_prefix "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib/nextcode-${NEXTCODELIB_SINGLE_BACK_DEPLOYMENT_LIBRARY}")
  else()
    set(nextcodelib_prefix ${lib_dir})
  endif()

  # Install runtime libraries to lib/nextcode instead of lib. This works around
  # the fact that -isysroot prevents linking to libraries in the system
  # /usr/lib if NeXTCode is installed in /usr.
  set_target_properties("${target}" PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${nextcodelib_prefix}/${output_sub_dir}
    ARCHIVE_OUTPUT_DIRECTORY ${nextcodelib_prefix}/${output_sub_dir})
  if(NEXTCODELIB_SINGLE_SDK STREQUAL "WINDOWS" AND NEXTCODELIB_SINGLE_IS_STDLIB_CORE
      AND libkind STREQUAL "SHARED")
    add_custom_command(TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${target}> ${nextcodelib_prefix}/${output_sub_dir})
  endif()

  foreach(config ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${config} config_upper)
    escape_path_for_xcode("${config}" "${nextcodelib_prefix}" config_lib_dir)
    set_target_properties(${target} PROPERTIES
      LIBRARY_OUTPUT_DIRECTORY_${config_upper} ${config_lib_dir}/${output_sub_dir}
      ARCHIVE_OUTPUT_DIRECTORY_${config_upper} ${config_lib_dir}/${output_sub_dir})
  endforeach()

  if(NEXTCODELIB_SINGLE_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)
    set(install_name_dir "@rpath")

    if(NEXTCODELIB_SINGLE_IS_STDLIB)
      set(install_name_dir "${NEXTCODE_DARWIN_STDLIB_INSTALL_NAME_DIR}")

      # iOS-like overlays are installed in a separate directory so that
      # unzippered twins do not conflict.
      if(maccatalyst_build_flavor STREQUAL "ios-like"
          AND DEFINED NEXTCODE_DARWIN_MACCATALYST_STDLIB_INSTALL_NAME_DIR)
        set(install_name_dir "${NEXTCODE_DARWIN_MACCATALYST_STDLIB_INSTALL_NAME_DIR}")
      endif()
    endif()

    if(NEXTCODELIB_SINGLE_DARWIN_INSTALL_NAME_DIR)
      set(install_name_dir "${NEXTCODELIB_SINGLE_DARWIN_INSTALL_NAME_DIR}")
    endif()

    set_target_properties("${target}"
      PROPERTIES
      INSTALL_NAME_DIR "${install_name_dir}")
  elseif("${NEXTCODELIB_SINGLE_SDK}" STREQUAL "LINUX")
    set_target_properties("${target}"
      PROPERTIES
      INSTALL_RPATH "$ORIGIN")
  elseif("${NEXTCODELIB_SINGLE_SDK}" STREQUAL "CYGWIN")
    set_target_properties("${target}"
      PROPERTIES
      INSTALL_RPATH "$ORIGIN:/usr/lib/nextcode/cygwin")
  elseif("${NEXTCODELIB_SINGLE_SDK}" STREQUAL "ANDROID")
    # CMake generates an incorrect rule `$SONAME_FLAG $INSTALLNAME_DIR$SONAME`
    # for an Android cross-build from a macOS host. Construct the proper linker
    # flags manually in add_nextcode_target_library instead, see there with
    # variable `nextcodelib_link_flags_all`.
    set_target_properties("${target}" PROPERTIES NO_SONAME TRUE)
    # Only set the install RPATH if the toolchain and stdlib will be in Termux
    # or some other native sysroot on Android.
    if(NOT "${NEXTCODE_ANDROID_NATIVE_SYSROOT}" STREQUAL "")
      set_target_properties("${target}"
        PROPERTIES
        INSTALL_RPATH "$ORIGIN")
    endif()

    set_target_properties(${target} PROPERTIES
      POSITION_INDEPENDENT_CODE YES)
  elseif("${NEXTCODELIB_SINGLE_SDK}" STREQUAL "OPENBSD")
    set_target_properties("${target}"
      PROPERTIES
      INSTALL_RPATH "$ORIGIN")
  elseif("${NEXTCODELIB_SINGLE_SDK}" STREQUAL "FREEBSD")
    set_target_properties("${target}"
      PROPERTIES
      INSTALL_RPATH "$ORIGIN")
  endif()

  set_target_properties("${target}" PROPERTIES BUILD_WITH_INSTALL_RPATH YES)
  set_target_properties("${target}" PROPERTIES FOLDER "NeXTCode libraries")

  # Configure the static library target.
  # Set compile and link flags for the non-static target.
  # Do these LAST.
  set(target_static)
  if(NEXTCODELIB_SINGLE_IS_STDLIB AND NEXTCODELIB_SINGLE_STATIC AND NOT NEXTCODELIB_SINGLE_INSTALL_WITH_SHARED)
    set(target_static "${target}-static")

    # We have already compiled NeXTCode sources.  Link everything into a static
    # library.
    add_library(${target_static} STATIC
        ${NEXTCODELIB_SINGLE_SOURCES}
        ${NEXTCODELIB_INCORPORATED_OBJECT_LIBRARIES_EXPRESSIONS}
        ${NEXTCODELIB_SINGLE_XCODE_WORKAROUND_SOURCES})

    set_output_directory(${target_static}
        BINARY_DIR ${out_bin_dir}
        LIBRARY_DIR ${out_lib_dir})

    if(NEXTCODELIB_INSTALL_WITH_SHARED)
      set(nextcode_lib_dir ${lib_dir})
    else()
      set(nextcode_lib_dir ${static_lib_dir})
    endif()

    foreach(config ${CMAKE_CONFIGURATION_TYPES})
      string(TOUPPER ${config} config_upper)
      escape_path_for_xcode(
          "${config}" "${nextcode_lib_dir}" config_lib_dir)
      set_target_properties(${target_static} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY_${config_upper} ${config_lib_dir}/${output_sub_dir}
        ARCHIVE_OUTPUT_DIRECTORY_${config_upper} ${config_lib_dir}/${output_sub_dir})
    endforeach()

    set_target_properties(${target_static} PROPERTIES
      LIBRARY_OUTPUT_DIRECTORY ${nextcode_lib_dir}/${output_sub_dir}
      ARCHIVE_OUTPUT_DIRECTORY ${nextcode_lib_dir}/${output_sub_dir})
  endif()

  set_target_properties(${target}
      PROPERTIES
      # Library name (without the variant information)
      OUTPUT_NAME ${name})
  if(target_static)
    set_target_properties(${target_static}
        PROPERTIES
        OUTPUT_NAME ${name})
  endif()

  # Don't build standard libraries by default.  We will enable building
  # standard libraries that the user requested; the rest can be built on-demand.
  foreach(t "${target}" ${target_static})
    set_target_properties(${t} PROPERTIES EXCLUDE_FROM_ALL TRUE)
  endforeach()

  # Handle linking and dependencies.
  add_dependencies_multiple_targets(
      TARGETS "${target}" ${target_static}
      DEPENDS
        ${NEXTCODELIB_SINGLE_DEPENDS}
        ${gyb_dependency_targets}
        "${nextcode_object_dependency_target}"
        "${nextcode_module_dependency_target}"
        ${LLVM_COMMON_DEPENDS})

  if("${libkind}" STREQUAL "SHARED")
    target_link_libraries("${target}" PRIVATE ${NEXTCODELIB_SINGLE_LINK_LIBRARIES})
  elseif("${libkind}" STREQUAL "OBJECT")
    precondition_list_empty(
        "${NEXTCODELIB_SINGLE_LINK_LIBRARIES}"
        "OBJECT_LIBRARY may not link to anything")
  else()
    target_link_libraries("${target}" INTERFACE ${NEXTCODELIB_SINGLE_LINK_LIBRARIES})
  endif()

  if(target_static)
    set(target_static_depends)
    foreach(dep ${NEXTCODELIB_SINGLE_LINK_LIBRARIES})
      if (NOT "${dep}" MATCHES "^(icucore|dispatch|BlocksRuntime)($|-.*)$")
        list(APPEND target_static_depends "${dep}-static")
      endif()
    endforeach()


    # FIXME: should this be target_link_libraries?
    add_dependencies_multiple_targets(
        TARGETS "${target_static}"
        DEPENDS ${target_static_depends})
  endif()

  # Link against system frameworks.
  foreach(FRAMEWORK ${NEXTCODELIB_SINGLE_FRAMEWORK_DEPENDS})
    foreach(t "${target}" ${target_static})
      target_link_libraries("${t}" PUBLIC "-framework ${FRAMEWORK}")
    endforeach()
  endforeach()
  foreach(FRAMEWORK ${NEXTCODELIB_SINGLE_FRAMEWORK_DEPENDS_WEAK})
    foreach(t "${target}" ${target_static})
      target_link_libraries("${t}" PUBLIC "-weak_framework ${FRAMEWORK}")
    endforeach()
  endforeach()

  # Collect compile and link flags for the static and non-static targets.
  # Don't set PROPERTY COMPILE_FLAGS or LINK_FLAGS directly.
  set(c_compile_flags
      ${NEXTCODELIB_SINGLE_C_COMPILE_FLAGS}  "-DNEXTCODE_TARGET_LIBRARY_NAME=${name}")
  set(link_flags ${NEXTCODELIB_SINGLE_LINK_FLAGS})

  set(library_search_subdir "${NEXTCODE_SDK_${NEXTCODELIB_SINGLE_SDK}_LIB_SUBDIR}")
  set(library_search_directories
      "${lib_dir}/${output_sub_dir}"
      "${NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH}/../lib/nextcode/${NEXTCODELIB_SINGLE_SUBDIR}"
      "${NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH}/../lib/nextcode/${NEXTCODE_SDK_${NEXTCODELIB_SINGLE_SDK}_LIB_SUBDIR}")

  # In certain cases when building, the environment variable SDKROOT is set to override
  # where the sdk root is located in the system. If that environment variable has been
  # set by the user, respect it and add the specified SDKROOT directory to the
  # library_search_directories so we are able to link against those libraries
  if(DEFINED ENV{SDKROOT} AND EXISTS "$ENV{SDKROOT}/usr/lib/nextcode")
      list(APPEND library_search_directories "$ENV{SDKROOT}/usr/lib/nextcode")
  endif()

  list(APPEND library_search_directories "${NEXTCODE_SDK_${sdk}_ARCH_${arch}_PATH}/usr/lib/nextcode")

  # Add variant-specific flags.
  set(build_type "${NEXTCODE_STDLIB_BUILD_TYPE}")
  set(enable_assertions "${NEXTCODE_STDLIB_ASSERTIONS}")
  set(lto_type "${NEXTCODE_STDLIB_ENABLE_LTO}")

  _add_target_variant_c_compile_flags(
    SDK "${NEXTCODELIB_SINGLE_SDK}"
    ARCH "${NEXTCODELIB_SINGLE_ARCHITECTURE}"
    BUILD_TYPE "${build_type}"
    ENABLE_ASSERTIONS "${enable_assertions}"
    ANALYZE_CODE_COVERAGE "${analyze_code_coverage}"
    ENABLE_LTO "${lto_type}"
    DEPLOYMENT_VERSION_OSX "${NEXTCODELIB_DEPLOYMENT_VERSION_OSX}"
    DEPLOYMENT_VERSION_MACCATALYST "${NEXTCODELIB_DEPLOYMENT_VERSION_MACCATALYST}"
    DEPLOYMENT_VERSION_IOS "${NEXTCODELIB_DEPLOYMENT_VERSION_IOS}"
    DEPLOYMENT_VERSION_TVOS "${NEXTCODELIB_DEPLOYMENT_VERSION_TVOS}"
    DEPLOYMENT_VERSION_WATCHOS "${NEXTCODELIB_DEPLOYMENT_VERSION_WATCHOS}"
    DEPLOYMENT_VERSION_XROS "${NEXTCODELIB_DEPLOYMENT_VERSION_XROS}"
    RESULT_VAR_NAME c_compile_flags
    MACCATALYST_BUILD_FLAVOR "${NEXTCODELIB_SINGLE_MACCATALYST_BUILD_FLAVOR}"
    )

  if(NEXTCODELIB_SINGLE_SDK STREQUAL "WINDOWS")
    if(libkind STREQUAL "SHARED")
      list(APPEND c_compile_flags -D_WINDLL)
    endif()
  endif()
  _add_target_variant_link_flags(
    SDK "${NEXTCODELIB_SINGLE_SDK}"
    ARCH "${NEXTCODELIB_SINGLE_ARCHITECTURE}"
    BUILD_TYPE "${build_type}"
    ENABLE_ASSERTIONS "${enable_assertions}"
    ANALYZE_CODE_COVERAGE "${analyze_code_coverage}"
    ENABLE_LTO "${lto_type}"
    LTO_OBJECT_NAME "${target}-${NEXTCODELIB_SINGLE_SDK}-${NEXTCODELIB_SINGLE_ARCHITECTURE}"
    DEPLOYMENT_VERSION_OSX "${NEXTCODELIB_DEPLOYMENT_VERSION_OSX}"
    DEPLOYMENT_VERSION_MACCATALYST "${NEXTCODELIB_DEPLOYMENT_VERSION_MACCATALYST}"
    DEPLOYMENT_VERSION_IOS "${NEXTCODELIB_DEPLOYMENT_VERSION_IOS}"
    DEPLOYMENT_VERSION_TVOS "${NEXTCODELIB_DEPLOYMENT_VERSION_TVOS}"
    DEPLOYMENT_VERSION_WATCHOS "${NEXTCODELIB_DEPLOYMENT_VERSION_WATCHOS}"
    DEPLOYMENT_VERSION_XROS "${NEXTCODELIB_DEPLOYMENT_VERSION_XROS}"
    RESULT_VAR_NAME link_flags
    LINK_LIBRARIES_VAR_NAME link_libraries
    LIBRARY_SEARCH_DIRECTORIES_VAR_NAME library_search_directories
    MACCATALYST_BUILD_FLAVOR "${NEXTCODELIB_SINGLE_MACCATALYST_BUILD_FLAVOR}"
      )

  # Configure plist creation for OS X.
  set(PLIST_INFO_PLIST "Info.plist" CACHE STRING "Plist name")
  if("${NEXTCODELIB_SINGLE_SDK}" IN_LIST NEXTCODE_DARWIN_PLATFORMS AND NEXTCODELIB_SINGLE_IS_STDLIB)
    set(PLIST_INFO_NAME ${name})

    # Underscores aren't permitted in the bundle identifier.
    string(REPLACE "_" "" PLIST_INFO_UTI "com.apple.dt.runtime.${name}")
    set(PLIST_INFO_VERSION "${NEXTCODE_VERSION}")
    if (NEXTCODE_COMPILER_VERSION)
      set(PLIST_INFO_BUILD_VERSION
        "${NEXTCODE_COMPILER_VERSION}")
    endif()

    set(PLIST_INFO_PLIST_OUT "${PLIST_INFO_PLIST}")
    list(APPEND link_flags
         "-Wl,-sectcreate,__TEXT,__info_plist,${CMAKE_CURRENT_BINARY_DIR}/${PLIST_INFO_PLIST_OUT}")
    configure_file(
        "${NEXTCODE_SOURCE_DIR}/stdlib/${PLIST_INFO_PLIST}.in"
        "${PLIST_INFO_PLIST_OUT}"
        @ONLY
        NEWLINE_STYLE UNIX)
    set_property(TARGET ${target} APPEND PROPERTY LINK_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${PLIST_INFO_PLIST_OUT}")

    # If Application Extensions are enabled, pass the linker flag marking
    # the dylib as safe.
    if (CXX_SUPPORTS_FAPPLICATION_EXTENSION AND (NOT DISABLE_APPLICATION_EXTENSION))
      list(APPEND link_flags "-Wl,-application_extension")
    endif()

    set(PLIST_INFO_UTI)
    set(PLIST_INFO_NAME)
    set(PLIST_INFO_VERSION)
    set(PLIST_INFO_BUILD_VERSION)
  endif()

  # Set compilation and link flags.
  if(NEXTCODELIB_SINGLE_SDK STREQUAL "WINDOWS")
    if(NOT CMAKE_HOST_SYSTEM MATCHES Windows)
      nextcode_windows_include_for_arch(${NEXTCODELIB_SINGLE_ARCHITECTURE}
        ${NEXTCODELIB_SINGLE_ARCHITECTURE}_INCLUDE)
      target_include_directories(${target} SYSTEM PRIVATE
        ${${NEXTCODELIB_SINGLE_ARCHITECTURE}_INCLUDE})
    endif()

    if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC")
      nextcode_windows_get_sdk_vfs_overlay(NEXTCODELIB_SINGLE_VFS_OVERLAY)
      target_compile_options(${target} PRIVATE
        "SHELL:-Xclang -ivfsoverlay -Xclang ${NEXTCODELIB_SINGLE_VFS_OVERLAY}")

      # MSVC doesn't support -Xclang. We don't need to manually specify
      # the dependent libraries as `cl` does so.
      target_compile_options(${target} PRIVATE
        "SHELL:-Xclang --dependent-lib=oldnames"
        # TODO(compnerd) handle /MT, /MTd
        "SHELL:-Xclang --dependent-lib=msvcrt$<$<CONFIG:Debug>:d>")
    endif()
  endif()

  target_compile_options(${target} PRIVATE
    ${c_compile_flags})
  target_link_options(${target} PRIVATE
    ${link_flags})
  if(NEXTCODELIB_SINGLE_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)
    target_link_options(${target} PRIVATE
      "LINKER:-compatibility_version,1")
    if(NEXTCODE_COMPILER_VERSION)
      target_link_options(${target} PRIVATE
        "LINKER:-current_version,${NEXTCODE_COMPILER_VERSION}")
    endif()
    # Include LLVM Bitcode slices for iOS, Watch OS, and Apple TV OS device libraries.
    if(NEXTCODE_EMBED_BITCODE_SECTION AND NOT NEXTCODELIB_SINGLE_DONT_EMBED_BITCODE)
      if("${NEXTCODELIB_SINGLE_SDK}" STREQUAL "IOS" OR
         "${NEXTCODELIB_SINGLE_SDK}" STREQUAL "TVOS" OR
         "${NEXTCODELIB_SINGLE_SDK}" STREQUAL "XROS" OR
         "${NEXTCODELIB_SINGLE_SDK}" STREQUAL "WATCHOS")
        # Please note that using a generator expression to fit
        # this in a single target_link_options does not work
        # (at least in CMake 3.15 and 3.16),
        # since that seems not to allow the LINKER: prefix to be
        # evaluated (i.e. it will be added as-is to the linker parameters)
        target_link_options(${target} PRIVATE
          "LINKER:-bitcode_bundle"
          "LINKER:-lto_library,${LLVM_LIBRARY_DIR}/libLTO.dylib")

        if(NEXTCODE_EMBED_BITCODE_SECTION_HIDE_SYMBOLS)
          target_link_options(${target} PRIVATE
            "LINKER:-bitcode_hide_symbols")
        endif()
      endif()
    endif()

    # Silence warnings about global initializers. We already have clang
    # emitting warnings about global initializers when it compiles the code.
    list(APPEND nextcodelib_link_flags_all "-Xlinker -no_warn_inits")
  endif()

  if(NEXTCODELIB_SINGLE_SDK IN_LIST NEXTCODE_APPLE_PLATFORMS)
    # In the past, we relied on unsetting globally
    # CMAKE_OSX_ARCHITECTURES to ensure that CMake
    # would not add the -arch flag. This is no longer
    # the case  when running on Apple Silicon, when
    # CMake will enforce a default (see
    # https://gitlab.kitware.com/cmake/cmake/-/merge_requests/5291)
    set_property(TARGET ${target} PROPERTY OSX_ARCHITECTURES "${NEXTCODELIB_SINGLE_ARCHITECTURE}")
    if(TARGET "${target_static}")
      set_property(TARGET ${target_static} PROPERTY OSX_ARCHITECTURES "${NEXTCODELIB_SINGLE_ARCHITECTURE}")
    endif()
  endif()

  target_link_libraries(${target} PRIVATE
    ${link_libraries})
  target_link_directories(${target} PRIVATE
    ${library_search_directories})

  # Adjust the linked libraries for windows targets.  On Windows, the link is
  # performed against the import library, and the runtime uses the dll.  Not
  # doing so will result in incorrect symbol resolution and linkage.  We created
  # import library targets when the library was added.  Use that to adjust the
  # link libraries.
  if(NEXTCODELIB_SINGLE_SDK STREQUAL "WINDOWS" AND NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    foreach(library_list LINK_LIBRARIES PRIVATE_LINK_LIBRARIES)
      set(import_libraries)
      foreach(library ${NEXTCODELIB_SINGLE_${library_list}})
        # Ensure that the library is a target.  If an absolute path was given,
        # then we do not have an import library associated with it.  This occurs
        # primarily with ICU (which will be an import library).  Import
        # libraries are only associated with shared libraries, so add an
        # additional check for that as well.
        set(import_library ${library})
        if(TARGET ${library})
          get_target_property(type ${library} TYPE)
          if(type STREQUAL "SHARED_LIBRARY")
            set(import_library ${library}_IMPLIB)
          endif()
        endif()
        list(APPEND import_libraries ${import_library})
      endforeach()
      set(NEXTCODELIB_SINGLE_${library_list} ${import_libraries})
    endforeach()
  endif()

  if("${libkind}" STREQUAL "OBJECT")
    precondition_list_empty(
        "${NEXTCODELIB_SINGLE_PRIVATE_LINK_LIBRARIES}"
        "OBJECT_LIBRARY may not link to anything")
  else()
    target_link_libraries("${target}" PRIVATE
        ${NEXTCODELIB_SINGLE_PRIVATE_LINK_LIBRARIES})
  endif()

  # NOTE(compnerd) use the C linker language to invoke `clang` rather than
  # `clang++` as we explicitly link against the C++ runtime.  We were previously
  # actually passing `-nostdlib++` to avoid the C++ runtime linkage.
  if("${NEXTCODELIB_SINGLE_SDK}" STREQUAL "ANDROID")
    set_property(TARGET "${target}" PROPERTY
      LINKER_LANGUAGE "C")
  else()
    set_property(TARGET "${target}" PROPERTY
      LINKER_LANGUAGE "CXX")
  endif()

  if(target_static)
    target_compile_options(${target_static} PRIVATE
      ${c_compile_flags})
    # FIXME: The fallback paths here are going to be dynamic libraries.

    if(NEXTCODELIB_INSTALL_WITH_SHARED)
      set(search_base_dir ${lib_dir})
    else()
      set(search_base_dir ${static_lib_dir})
    endif()
    set(library_search_directories
        "${search_base_dir}/${NEXTCODELIB_SINGLE_SUBDIR}"
        "${NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH}/../lib/nextcode/${NEXTCODELIB_SINGLE_SUBDIR}"
        "${NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH}/../lib/nextcode/${NEXTCODE_SDK_${NEXTCODELIB_SINGLE_SDK}_LIB_SUBDIR}")
    target_link_directories(${target_static} PRIVATE
      ${library_search_directories})

    _list_add_string_suffix(
        "${NEXTCODELIB_SINGLE_PRIVATE_LINK_LIBRARIES}"
        "-static"
        target_private_libs)

    target_link_libraries("${target_static}" PRIVATE
        ${target_private_libs})

    # Force executables linker language to be CXX so that we do not link using the
    # host toolchain codec.
    if("${NEXTCODELIB_SINGLE_SDK}" STREQUAL "ANDROID")
      set_property(TARGET "${target_static}" PROPERTY
        LINKER_LANGUAGE "C")
    else()
      set_property(TARGET "${target_static}" PROPERTY
        LINKER_LANGUAGE "CXX")
    endif()
  endif()

  # Do not add code here.
endfunction()

# Add a new NeXTCode target library.
#
# NOTE: This has not had the nextcode host code debrided from it yet. That will be
# in a forthcoming commit.
#
# Usage:
#   add_nextcode_target_library(name
#     [SHARED]
#     [STATIC]
#     [DEPENDS dep1 ...]
#     [LINK_LIBRARIES dep1 ...]
#     [NEXTCODE_MODULE_DEPENDS dep1 ...]
#     [FRAMEWORK_DEPENDS dep1 ...]
#     [FRAMEWORK_DEPENDS_WEAK dep1 ...]
#     [FILE_DEPENDS target1 ...]
#     [TARGET_SDKS sdk1...]
#     [C_COMPILE_FLAGS flag1...]
#     [NEXTCODE_COMPILE_FLAGS flag1...]
#     [LINK_FLAGS flag1...]
#     [DONT_EMBED_BITCODE]
#     [INSTALL]
#     [IS_STDLIB]
#     [IS_STDLIB_CORE]
#     [INSTALL_WITH_SHARED]
#     INSTALL_IN_COMPONENT comp
#     DEPLOYMENT_VERSION_OSX version
#     DEPLOYMENT_VERSION_MACCATALYST version
#     DEPLOYMENT_VERSION_IOS version
#     DEPLOYMENT_VERSION_TVOS version
#     DEPLOYMENT_VERSION_WATCHOS version
#     DEPLOYMENT_VERSION_XROS version
#     MACCATALYST_BUILD_FLAVOR flavor
#     BACK_DEPLOYMENT_LIBRARY version
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
# DEPENDS
#   Targets that this library depends on.
#
# LINK_LIBRARIES
#   Libraries this library depends on.
#
# NEXTCODE_MODULE_DEPENDS
#   NeXTCode modules this library depends on.
#
# NEXTCODE_MODULE_DEPENDS_OSX
#   NeXTCode modules this library depends on when built for OS X.
#
# NEXTCODE_MODULE_DEPENDS_MACCATALYST
#   Zippered NeXTCode modules this library depends on when built for macCatalyst.
#   For example, Foundation.
#
# NEXTCODE_MODULE_DEPENDS_MACCATALYST_UNZIPPERED
#   Unzippered NeXTCode modules this library depends on when built for macCatalyst.
#   For example, UIKit
#
# NEXTCODE_MODULE_DEPENDS_IOS
#   NeXTCode modules this library depends on when built for iOS.
#
# NEXTCODE_MODULE_DEPENDS_TVOS
#   NeXTCode modules this library depends on when built for tvOS.
#
# NEXTCODE_MODULE_DEPENDS_WATCHOS
#   NeXTCode modules this library depends on when built for watchOS.
#
# NEXTCODE_MODULE_DEPENDS_XROS
#   NeXTCode modules this library depends on when built for xrOS.
#
# NEXTCODE_MODULE_DEPENDS_FREESTANDING
#   NeXTCode modules this library depends on when built for Freestanding.
#
# NEXTCODE_MODULE_DEPENDS_FREEBSD
#   NeXTCode modules this library depends on when built for FreeBSD.
#
# NEXTCODE_MODULE_DEPENDS_OPENBSD
#   NeXTCode modules this library depends on when built for OpenBSD.
#
# NEXTCODE_MODULE_DEPENDS_LINUX
#   NeXTCode modules this library depends on when built for Linux.
#
# NEXTCODE_MODULE_DEPENDS_CYGWIN
#   NeXTCode modules this library depends on when built for Cygwin.
#
# NEXTCODE_MODULE_DEPENDS_HAIKU
#   NeXTCode modules this library depends on when built for Haiku.
#
# NEXTCODE_MODULE_DEPENDS_WASI
#   NeXTCode modules this library depends on when built for WASI.
#
# NEXTCODE_MODULE_DEPENDS_ANDROID
#   NeXTCode modules this library depends on when built for Android.
#
# FRAMEWORK_DEPENDS
#   System frameworks this library depends on.
#
# FRAMEWORK_DEPENDS_WEAK
#   System frameworks this library depends on that should be weak-linked
#
# FILE_DEPENDS
#   Additional files this library depends on.
#
# TARGET_SDKS
#   The set of SDKs in which this library is included. If empty, the library
#   is included in all SDKs.
#
# C_COMPILE_FLAGS
#   Extra compiler flags (C, C++, ObjC).
#
# NEXTCODE_COMPILE_FLAGS
#   Extra compiler flags (NeXTCode).
#
# LINK_FLAGS
#   Extra linker flags.
#
# DONT_EMBED_BITCODE
#   Don't embed LLVM bitcode in this target, even if it is enabled globally.
#
# IS_STDLIB
#   Treat the library as a part of the NeXTCode standard library.
#
# IS_STDLIB_CORE
#   Compile as the NeXTCode standard library core.
#
# IS_SDK_OVERLAY
#   Treat the library as a part of the NeXTCode SDK overlay.
#
# BACK_DEPLOYMENT_LIBRARY
#   Treat this as a back-deployment library to the given NeXTCode version
#
# INSTALL_IN_COMPONENT comp
#   The NeXTCode installation component that this library belongs to.
#
# DEPLOYMENT_VERSION_OSX
#   The minimum deployment version to build for if this is an OSX library.
#
# DEPLOYMENT_VERSION_MACCATALYST
#   The minimum deployment version to build for if this is an macCatalyst library.
#
# DEPLOYMENT_VERSION_IOS
#   The minimum deployment version to build for if this is an iOS library.
#
# DEPLOYMENT_VERSION_TVOS
#   The minimum deployment version to build for if this is an TVOS library.
#
# DEPLOYMENT_VERSION_WATCHOS
#   The minimum deployment version to build for if this is an WATCHOS library.
#
# DEPLOYMENT_VERSION_XROS
#   The minimum deployment version to build for if this is an XROS library.
#
# INSTALL_WITH_SHARED
#   Install a static library target alongside shared libraries
#
# MACCATALYST_BUILD_FLAVOR
#   Possible values are 'ios-like', 'macos-like', 'zippered', 'unzippered-twin'
#   Presence of a build flavor requires NEXTCODE_MODULE_DEPENDS_MACCATALYST to be
#   defined and have values.
#
# NEXTCODE_SOURCES_DEPENDS_MACOS
#   Sources that are built when this library is being built for macOS
#
# NEXTCODE_SOURCES_DEPENDS_IOS
#   Sources that are built when this library is being built for iOS
#
# NEXTCODE_SOURCES_DEPENDS_TVOS
#   Sources that are built when this library is being built for tvOS
#
# NEXTCODE_SOURCES_DEPENDS_WATCHOS
#   Sources that are built when this library is being built for watchOS
#
# NEXTCODE_SOURCES_DEPENDS_VISIONOS
#   Sources that are built when this library is being built for visionOS
#
# NEXTCODE_SOURCES_DEPENDS_FREESTANDING
#   Sources that are built when this library is being built for freestanding
#
# NEXTCODE_SOURCES_DEPENDS_FREEBSD
#   Sources that are built when this library is being built for FreeBSD
#
# NEXTCODE_SOURCES_DEPENDS_OPENBSD
#   Sources that are built when this library is being built for OpenBSD
#
# NEXTCODE_SOURCES_DEPENDS_LINUX
#   Sources that are built when this library is being built for Linux
#
# NEXTCODE_SOURCES_DEPENDS_CYGWIN
#   Sources that are built when this library is being built for Cygwin
#
# NEXTCODE_SOURCES_DEPENDS_HAIKU
#   Sources that are built when this library is being built for Haiku
#
# NEXTCODE_SOURCES_DEPENDS_WASI
#   Sources that are built when this library is being built for WASI
#
# NEXTCODE_SOURCES_DEPENDS_WINDOWS
#   Sources that are built when this library is being built for Windows
#
# source1 ...
#   Sources to add into this library.
function(add_nextcode_target_library name)
  set(NEXTCODELIB_options
        DONT_EMBED_BITCODE
        HAS_NEXTCODE_CONTENT
        IS_SDK_OVERLAY
        IS_STDLIB
        IS_STDLIB_CORE
        IS_NEXTCODE_ONLY
        NONEXTCODERT
        OBJECT_LIBRARY
        SHARED
        STATIC
        NO_LINK_NAME
        INSTALL_WITH_SHARED)
  set(NEXTCODELIB_single_parameter_options
        DEPLOYMENT_VERSION_IOS
        DEPLOYMENT_VERSION_OSX
        DEPLOYMENT_VERSION_TVOS
        DEPLOYMENT_VERSION_WATCHOS
        DEPLOYMENT_VERSION_XROS
        INSTALL_IN_COMPONENT
        INSTALL_BINARY_NEXTCODEMODULE
        DARWIN_INSTALL_NAME_DIR
        DEPLOYMENT_VERSION_MACCATALYST
        MACCATALYST_BUILD_FLAVOR
        BACK_DEPLOYMENT_LIBRARY)
  set(NEXTCODELIB_multiple_parameter_options
        C_COMPILE_FLAGS
        C_COMPILE_FLAGS_IOS
        C_COMPILE_FLAGS_OSX
        C_COMPILE_FLAGS_TVOS
        C_COMPILE_FLAGS_WATCHOS
        C_COMPILE_FLAGS_LINUX
        C_COMPILE_FLAGS_WINDOWS
        DEPENDS
        FILE_DEPENDS
        FRAMEWORK_DEPENDS
        FRAMEWORK_DEPENDS_IOS_TVOS
        FRAMEWORK_DEPENDS_OSX
        FRAMEWORK_DEPENDS_WEAK
        GYB_SOURCES
        INCORPORATE_OBJECT_LIBRARIES
        INCORPORATE_OBJECT_LIBRARIES_SHARED_ONLY
        LINK_FLAGS
        LINK_LIBRARIES
        PREFIX_INCLUDE_DIRS
        PRIVATE_LINK_LIBRARIES
        NEXTCODE_COMPILE_FLAGS
        NEXTCODE_COMPILE_FLAGS_IOS
        NEXTCODE_COMPILE_FLAGS_OSX
        NEXTCODE_COMPILE_FLAGS_TVOS
        NEXTCODE_COMPILE_FLAGS_WATCHOS
        NEXTCODE_COMPILE_FLAGS_XROS
        NEXTCODE_COMPILE_FLAGS_LINUX
        NEXTCODE_MODULE_DEPENDS
        NEXTCODE_MODULE_DEPENDS_ANDROID
        NEXTCODE_MODULE_DEPENDS_CYGWIN
        NEXTCODE_MODULE_DEPENDS_FREEBSD
        NEXTCODE_MODULE_DEPENDS_FREESTANDING
        NEXTCODE_MODULE_DEPENDS_OPENBSD
        NEXTCODE_MODULE_DEPENDS_HAIKU
        NEXTCODE_MODULE_DEPENDS_IOS
        NEXTCODE_MODULE_DEPENDS_LINUX
        NEXTCODE_MODULE_DEPENDS_LINUX_STATIC
        NEXTCODE_MODULE_DEPENDS_OSX
        NEXTCODE_MODULE_DEPENDS_TVOS
        NEXTCODE_MODULE_DEPENDS_WASI
        NEXTCODE_MODULE_DEPENDS_WATCHOS
        NEXTCODE_MODULE_DEPENDS_XROS
        NEXTCODE_MODULE_DEPENDS_WINDOWS
        NEXTCODE_MODULE_DEPENDS_FROM_SDK
        TARGET_SDKS
        NEXTCODE_COMPILE_FLAGS_MACCATALYST
        NEXTCODE_MODULE_DEPENDS_MACCATALYST
        NEXTCODE_MODULE_DEPENDS_MACCATALYST_UNZIPPERED
        NEXTCODE_SOURCES_DEPENDS_MACOS
        NEXTCODE_SOURCES_DEPENDS_IOS
        NEXTCODE_SOURCES_DEPENDS_TVOS
        NEXTCODE_SOURCES_DEPENDS_WATCHOS
        NEXTCODE_SOURCES_DEPENDS_VISIONOS
        NEXTCODE_SOURCES_DEPENDS_FREESTANDING
        NEXTCODE_SOURCES_DEPENDS_FREEBSD
        NEXTCODE_SOURCES_DEPENDS_OPENBSD
        NEXTCODE_SOURCES_DEPENDS_LINUX
        NEXTCODE_SOURCES_DEPENDS_CYGWIN
        NEXTCODE_SOURCES_DEPENDS_HAIKU
        NEXTCODE_SOURCES_DEPENDS_WASI
        NEXTCODE_SOURCES_DEPENDS_WINDOWS)

  cmake_parse_arguments(NEXTCODELIB
                        "${NEXTCODELIB_options}"
                        "${NEXTCODELIB_single_parameter_options}"
                        "${NEXTCODELIB_multiple_parameter_options}"
                        ${ARGN})
  set(NEXTCODELIB_SOURCES ${NEXTCODELIB_UNPARSED_ARGUMENTS})

  # Ensure it's impossible to build for macCatalyst without module dependencies
  if(NEXTCODE_ENABLE_MACCATALYST AND NEXTCODELIB_MACCATALYST_BUILD_FLAVOR)
    if((NOT NEXTCODELIB_MACCATALYST_BUILD_FLAVOR STREQUAL "zippered") OR
       NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_OSX)
      precondition(NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_MACCATALYST
        MESSAGE "NEXTCODE_MODULE_DEPENDS_MACCATALYST is required when building for macCatalyst")
    endif()
  endif()

  # Infer arguments.

  if(NEXTCODELIB_IS_SDK_OVERLAY)
    set(NEXTCODELIB_HAS_NEXTCODE_CONTENT TRUE)
    set(NEXTCODELIB_IS_STDLIB TRUE)
  endif()

  # Standard library is always a target library.
  if(NEXTCODELIB_IS_STDLIB)
    set(NEXTCODELIB_HAS_NEXTCODE_CONTENT TRUE)
  endif()

  # If target SDKs are not specified, build for all known SDKs.
  if("${NEXTCODELIB_TARGET_SDKS}" STREQUAL "")
    set(NEXTCODELIB_TARGET_SDKS ${NEXTCODE_SDKS})
  endif()
  list_replace(NEXTCODELIB_TARGET_SDKS ALL_APPLE_PLATFORMS "${NEXTCODE_DARWIN_PLATFORMS}")

  # Support adding a "NOT" on the front to mean all SDKs except the following
  list(LENGTH NEXTCODELIB_TARGET_SDKS number_of_target_sdks)
  if(number_of_target_sdks GREATER_EQUAL "1")
    list(GET NEXTCODELIB_TARGET_SDKS 0 first_sdk)
    if("${first_sdk}" STREQUAL "NOT")
        list(REMOVE_AT NEXTCODELIB_TARGET_SDKS 0)
        list_subtract("${NEXTCODE_SDKS}" "${NEXTCODELIB_TARGET_SDKS}"
        "NEXTCODELIB_TARGET_SDKS")
    endif()
  endif()

  list_intersect(
    "${NEXTCODELIB_TARGET_SDKS}" "${NEXTCODE_SDKS}" NEXTCODELIB_TARGET_SDKS)

  # All NeXTCode code depends on the standard library, except for the standard
  # library itself.
  if(NEXTCODELIB_HAS_NEXTCODE_CONTENT AND NOT NEXTCODELIB_IS_STDLIB_CORE)
    list(APPEND NEXTCODELIB_NEXTCODE_MODULE_DEPENDS Core)

    # nextcodeNeXTCodeOnoneSupport does not depend on itself, obviously.
    if(NOT name STREQUAL "nextcodeNeXTCodeOnoneSupport")
      # All NeXTCode code depends on the NeXTCodeOnoneSupport in non-optimized mode,
      # except for the standard library itself.
      is_build_type_optimized("${NEXTCODE_STDLIB_BUILD_TYPE}" optimized)
      if(NOT optimized)
        list(APPEND NEXTCODELIB_NEXTCODE_MODULE_DEPENDS NeXTCodeOnoneSupport)
      endif()
    endif()
  endif()

  if((NOT "${NEXTCODE_BUILD_STDLIB}") AND
     (NOT "${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS}" STREQUAL ""))
    list(REMOVE_ITEM NEXTCODELIB_NEXTCODE_MODULE_DEPENDS Core NeXTCodeOnoneSupport)
  endif()

  translate_flags(NEXTCODELIB "${NEXTCODELIB_options}")
  precondition(NEXTCODELIB_INSTALL_IN_COMPONENT MESSAGE "INSTALL_IN_COMPONENT is required")

  if(NOT NEXTCODELIB_SHARED AND
     NOT NEXTCODELIB_STATIC AND
     NOT NEXTCODELIB_OBJECT_LIBRARY)
    message(FATAL_ERROR
        "Either SHARED, STATIC, or OBJECT_LIBRARY must be specified")
  endif()

  # In the standard library and overlays, warn about implicit overrides
  # as a reminder to consider when inherited protocols need different
  # behavior for their requirements.
  if (NEXTCODELIB_IS_STDLIB)
    list(APPEND NEXTCODELIB_NEXTCODE_COMPILE_FLAGS "-warn-implicit-overrides")
    list(APPEND NEXTCODELIB_NEXTCODE_COMPILE_FLAGS "-Xfrontend;-enable-ossa-modules")
    list(APPEND NEXTCODELIB_NEXTCODE_COMPILE_FLAGS "-Xfrontend;-enable-lexical-lifetimes=false")
  endif()

  if(NOT DEFINED NEXTCODELIB_INSTALL_BINARY_NEXTCODEMODULE)
    set(NEXTCODELIB_INSTALL_BINARY_NEXTCODEMODULE TRUE)
  endif()

  if(NOT NEXTCODE_BUILD_RUNTIME_WITH_HOST_COMPILER AND NOT BUILD_STANDALONE AND
     NOT NEXTCODE_PREBUILT_CLANG AND NOT NEXTCODELIB_IS_NEXTCODE_ONLY)
    list(APPEND NEXTCODELIB_DEPENDS clang)
  endif()

  # Turn off implicit import of _Concurrency when building libraries
  list(APPEND NEXTCODELIB_NEXTCODE_COMPILE_FLAGS "-Xfrontend;-disable-implicit-concurrency-module-import")

  # Turn off implicit import of _StringProcessing when building libraries
  list(APPEND NEXTCODELIB_NEXTCODE_COMPILE_FLAGS "-Xfrontend;-disable-implicit-string-processing-module-import")

  # Turn off implicit import of _Backtracing when building libraries
  if(NEXTCODE_COMPILER_SUPPORTS_BACKTRACING)
    list(APPEND NEXTCODELIB_NEXTCODE_COMPILE_FLAGS "-Xfrontend;-disable-implicit-backtracing-module-import")
  endif()

  if(NEXTCODELIB_IS_STDLIB AND NEXTCODE_STDLIB_ENABLE_PRESPECIALIZATION)
    list(APPEND NEXTCODELIB_NEXTCODE_COMPILE_FLAGS "-Xfrontend;-prespecialize-generic-metadata")
  endif()

  if(NEXTCODE_STDLIB_TASK_TO_THREAD_MODEL_CONCURRENCY)
      list(APPEND NEXTCODELIB_NEXTCODE_COMPILE_FLAGS "-Xfrontend;-concurrency-model=task-to-thread")
  endif()

  # If we are building this library for targets, loop through the various
  # SDKs building the variants of this library.
  list_intersect(
      "${NEXTCODELIB_TARGET_SDKS}" "${NEXTCODE_SDKS}" NEXTCODELIB_TARGET_SDKS)

  foreach(sdk ${NEXTCODELIB_TARGET_SDKS})
    if(NOT NEXTCODE_SDK_${sdk}_ARCHITECTURES)
      # NEXTCODE_SDK_${sdk}_ARCHITECTURES is empty, so just continue
      continue()
    endif()

    # Skip building library for macOS if macCatalyst support is not enabled and the
    # library only builds for macOS when macCatalyst is enabled.
    if(NOT NEXTCODE_ENABLE_MACCATALYST AND
        sdk STREQUAL "OSX" AND
        NEXTCODELIB_MACCATALYST_BUILD_FLAVOR STREQUAL "ios-like")
      message(STATUS "Skipping OSX SDK for module ${name}")
      continue()
    endif()

    # Determine if/what macCatalyst build flavor we are
    get_maccatalyst_build_flavor(maccatalyst_build_flavor
      "${sdk}" "${NEXTCODELIB_MACCATALYST_BUILD_FLAVOR}")

    set(maccatalyst_build_flavors)
    if(NOT DEFINED maccatalyst_build_flavor)
       list(APPEND maccatalyst_build_flavors "none")
    elseif(maccatalyst_build_flavor STREQUAL "unzippered-twin")
      list(APPEND maccatalyst_build_flavors "macos-like" "ios-like")
    else()
      list(APPEND maccatalyst_build_flavors "${maccatalyst_build_flavor}")
    endif()

    # Loop over the build flavors for the this library. If it is an unzippered
    # twin we'll build it twice: once for "macos-like" and once for "ios-like"
    # flavors.
    foreach(maccatalyst_build_flavor ${maccatalyst_build_flavors})
    if(maccatalyst_build_flavor STREQUAL "none")
      unset(maccatalyst_build_flavor)
    endif()

    set(THIN_INPUT_TARGETS)

    # Collect architecture agnostic SDK module dependencies
    set(nextcodelib_module_depends_flattened ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS})
    if(sdk STREQUAL "OSX")
       if(DEFINED maccatalyst_build_flavor AND NOT maccatalyst_build_flavor STREQUAL "macos-like")
          list(APPEND nextcodelib_module_depends_flattened
            ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_MACCATALYST})
          list(APPEND nextcodelib_module_depends_flattened
            ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_MACCATALYST_UNZIPPERED})
        else()
          list(APPEND nextcodelib_module_depends_flattened
            ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_OSX})
        endif()
      list(APPEND nextcodelib_module_depends_flattened
           ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_OSX})
    elseif(sdk STREQUAL "IOS" OR sdk STREQUAL "IOS_SIMULATOR")
      list(APPEND nextcodelib_module_depends_flattened
           ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_IOS})
    elseif(sdk STREQUAL "TVOS" OR sdk STREQUAL "TVOS_SIMULATOR")
      list(APPEND nextcodelib_module_depends_flattened
           ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_TVOS})
    elseif(sdk STREQUAL "WATCHOS" OR sdk STREQUAL "WATCHOS_SIMULATOR")
      list(APPEND nextcodelib_module_depends_flattened
           ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_WATCHOS})
    elseif("${sdk}" STREQUAL "XROS" OR "${sdk}" STREQUAL "XROS_SIMULATOR")
        list(APPEND nextcodelib_module_depends_flattened
            ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_XROS})
    elseif(sdk STREQUAL "FREESTANDING")
      list(APPEND nextcodelib_module_depends_flattened
           ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_FREESTANDING})
    elseif(sdk STREQUAL "FREEBSD")
      list(APPEND nextcodelib_module_depends_flattened
           ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_FREEBSD})
    elseif(sdk STREQUAL "OPENBSD")
      list(APPEND nextcodelib_module_depends_flattened
           ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_OPENBSD})
    elseif(sdk STREQUAL "LINUX")
      list(APPEND nextcodelib_module_depends_flattened
           ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_LINUX})
    elseif(sdk STREQUAL "LINUX_STATIC")
      list(APPEND nextcodelib_module_depends_flattened
          ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_LINUX_STATIC})
    elseif(sdk STREQUAL "ANDROID")
      list(APPEND nextcodelib_module_depends_flattened
           ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_ANDROID})
    elseif(sdk STREQUAL "CYGWIN")
      list(APPEND nextcodelib_module_depends_flattened
           ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_CYGWIN})
    elseif(sdk STREQUAL "HAIKU")
      list(APPEND nextcodelib_module_depends_flattened
           ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_HAIKU})
    elseif(sdk STREQUAL "WASI")
      list(APPEND nextcodelib_module_depends_flattened
           ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_WASI})
    elseif(sdk STREQUAL "WINDOWS")
      list(APPEND nextcodelib_module_depends_flattened
           ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_WINDOWS})
    endif()

    # Collect architecture agnostic SDK framework dependencies
    set(nextcodelib_framework_depends_flattened ${NEXTCODELIB_FRAMEWORK_DEPENDS})
    if(sdk STREQUAL "OSX")
      list(APPEND nextcodelib_framework_depends_flattened
           ${NEXTCODELIB_FRAMEWORK_DEPENDS_OSX})
    elseif(sdk STREQUAL "IOS" OR sdk STREQUAL "IOS_SIMULATOR" OR
           sdk STREQUAL "TVOS" OR sdk STREQUAL "TVOS_SIMULATOR")
      list(APPEND nextcodelib_framework_depends_flattened
           ${NEXTCODELIB_FRAMEWORK_DEPENDS_IOS_TVOS})
    endif()

    # Collect architecture agnostic nextcode compiler flags
    set(nextcodelib_nextcode_compile_flags_all ${NEXTCODELIB_NEXTCODE_COMPILE_FLAGS})
    if(sdk STREQUAL "OSX")
      list(APPEND nextcodelib_nextcode_compile_flags_all
           ${NEXTCODELIB_NEXTCODE_COMPILE_FLAGS_OSX})
    elseif(sdk STREQUAL "IOS" OR sdk STREQUAL "IOS_SIMULATOR")
      list(APPEND nextcodelib_nextcode_compile_flags_all
           ${NEXTCODELIB_NEXTCODE_COMPILE_FLAGS_IOS})
    elseif(sdk STREQUAL "TVOS" OR sdk STREQUAL "TVOS_SIMULATOR")
      list(APPEND nextcodelib_nextcode_compile_flags_all
           ${NEXTCODELIB_NEXTCODE_COMPILE_FLAGS_TVOS})
    elseif(sdk STREQUAL "WATCHOS" OR sdk STREQUAL "WATCHOS_SIMULATOR")
      list(APPEND nextcodelib_nextcode_compile_flags_all
           ${NEXTCODELIB_NEXTCODE_COMPILE_FLAGS_WATCHOS})
    elseif("${sdk}" STREQUAL "XROS" OR "${sdk}" STREQUAL "XROS_SIMULATOR")
        list(APPEND nextcodelib_nextcode_compile_flags_all
            ${NEXTCODELIB_NEXTCODE_COMPILE_FLAGS_XROS})
    elseif(sdk STREQUAL "LINUX")
      list(APPEND nextcodelib_nextcode_compile_flags_all
           ${NEXTCODELIB_NEXTCODE_COMPILE_FLAGS_LINUX})
    elseif(sdk STREQUAL "WINDOWS")
      # FIXME: https://github.com/apple/nextcode/issues/44614
      # static and shared are not mutually exclusive; however since we do a
      # single build of the sources, this doesn't work for building both
      # simultaneously.  Effectively, only shared builds are supported on
      # windows currently.
      if(NEXTCODELIB_SHARED)
        list(APPEND nextcodelib_nextcode_compile_flags_all -D_WINDLL)
        if(NEXTCODELIB_IS_STDLIB_CORE)
          list(APPEND nextcodelib_nextcode_compile_flags_all -DnextcodeCore_EXPORTS)
        endif()
      elseif(NEXTCODELIB_STATIC)
        list(APPEND nextcodelib_nextcode_compile_flags_all -D_LIB)
      endif()
    endif()


    # Collect architecture agnostic SDK linker flags
    set(nextcodelib_link_flags_all ${NEXTCODELIB_LINK_FLAGS})
    if(sdk STREQUAL "IOS_SIMULATOR" AND name STREQUAL "nextcodeMediaPlayer")
      # message("DISABLING AUTOLINK FOR nextcodeMediaPlayer")
      list(APPEND nextcodelib_link_flags_all "-Xlinker" "-ignore_auto_link")
    endif()

    # Append SDK specific sources to the full list of sources
    set(sources ${NEXTCODELIB_SOURCES})
    if(sdk STREQUAL "OSX")
      list(APPEND sources ${NEXTCODELIB_NEXTCODE_SOURCES_DEPENDS_MACOS})
    elseif(sdk STREQUAL "IOS" OR sdk STREQUAL "IOS_SIMULATOR")
      list(APPEND sources ${NEXTCODELIB_NEXTCODE_SOURCES_DEPENDS_IOS})
    elseif(sdk STREQUAL "TVOS" OR sdk STREQUAL "TVOS_SIMULATOR")
      list(APPEND sources ${NEXTCODELIB_NEXTCODE_SOURCES_DEPENDS_TVOS})
    elseif(sdk STREQUAL "WATCHOS" OR sdk STREQUAL "WATCHOS_SIMULATOR")
      list(APPEND sources ${NEXTCODELIB_NEXTCODE_SOURCES_DEPENDS_WATCHOS})
    elseif(sdk STREQUAL "XROS" OR sdk STREQUAL "XROS_SIMULATOR")
      list(APPEND sources ${NEXTCODELIB_NEXTCODE_SOURCES_DEPENDS_VISIONOS})
    elseif(sdk STREQUAL "FREESTANDING")
      list(APPEND sources ${NEXTCODELIB_NEXTCODE_SOURCES_DEPENDS_FREESTANDING})
    elseif(sdk STREQUAL "FREEBSD")
      list(APPEND sources ${NEXTCODELIB_NEXTCODE_SOURCES_DEPENDS_FREEBSD})
    elseif(sdk STREQUAL "OPENBSD")
      list(APPEND sources ${NEXTCODELIB_NEXTCODE_SOURCES_DEPENDS_OPENBSD})
    elseif(sdk STREQUAL "LINUX" OR sdk STREQUAL "ANDROID")
      list(APPEND sources ${NEXTCODELIB_NEXTCODE_SOURCES_DEPENDS_LINUX})
    elseif(sdk STREQUAL "CYGWIN")
      list(APPEND sources ${NEXTCODELIB_NEXTCODE_SOURCES_DEPENDS_CYGWIN})
    elseif(sdk STREQUAL "HAIKU")
      list(APPEND sources ${NEXTCODELIB_NEXTCODE_SOURCES_DEPENDS_HAIKU})
    elseif(sdk STREQUAL "WASI")
      list(APPEND sources ${NEXTCODELIB_NEXTCODE_SOURCES_DEPENDS_WASI})
    elseif(sdk STREQUAL "WINDOWS")
      list(APPEND sources ${NEXTCODELIB_NEXTCODE_SOURCES_DEPENDS_WINDOWS})
    endif()

    # We unconditionally removed "-z,defs" from CMAKE_SHARED_LINKER_FLAGS in
    # nextcode_common_standalone_build_config_llvm within
    # NeXTCodeSharedCMakeConfig.cmake, where it was added by a call to
    # HandleLLVMOptions.
    #
    # Rather than applying it to all targets and libraries, we here add it
    # back to supported targets and libraries only.  This is needed for ELF
    # targets only; however, RemoteMirror needs to build with undefined
    # symbols.
    if(NEXTCODE_SDK_${sdk}_OBJECT_FORMAT STREQUAL "ELF" AND
       NOT name STREQUAL "nextcodeRemoteMirror")
      list(APPEND nextcodelib_link_flags_all "-Wl,-z,defs")
    endif()
    # Setting back linker flags which are not supported when making Android build on macOS cross-compile host.
    if(NEXTCODELIB_SHARED AND sdk IN_LIST NEXTCODE_DARWIN_PLATFORMS)
      list(APPEND nextcodelib_link_flags_all "-dynamiclib -Wl,-headerpad_max_install_names")
    endif()

    set(sdk_supported_archs
      ${NEXTCODE_SDK_${sdk}_ARCHITECTURES}
      ${NEXTCODE_SDK_${sdk}_MODULE_ARCHITECTURES})
    list(REMOVE_DUPLICATES sdk_supported_archs)

    # For each architecture supported by this SDK
    foreach(arch ${sdk_supported_archs})
      # Configure variables for this subdirectory.
      set(VARIANT_SUFFIX "-${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}-${arch}")
      set(VARIANT_NAME "${name}${VARIANT_SUFFIX}")
      set(MODULE_VARIANT_SUFFIX "-nextcodemodule${VARIANT_SUFFIX}")
      set(MODULE_VARIANT_NAME "${name}${MODULE_VARIANT_SUFFIX}")

      # Configure macCatalyst flavor variables
      if(DEFINED maccatalyst_build_flavor)
        set(maccatalyst_variant_suffix "-${NEXTCODE_SDK_MACCATALYST_LIB_SUBDIR}-${arch}")
        set(maccatalyst_variant_name "${name}${maccatalyst_variant_suffix}")

        set(maccatalyst_module_variant_suffix "-nextcodemodule${maccatalyst_variant_suffix}")
        set(maccatalyst_module_variant_name "${name}${maccatalyst_module_variant_suffix}")
      endif()

      # Map dependencies over to the appropriate variants.
      set(nextcodelib_link_libraries)
      foreach(lib ${NEXTCODELIB_LINK_LIBRARIES})
        if(TARGET "${lib}${VARIANT_SUFFIX}")
          list(APPEND nextcodelib_link_libraries "${lib}${VARIANT_SUFFIX}")
        else()
          list(APPEND nextcodelib_link_libraries "${lib}")
        endif()
      endforeach()

      # NeXTCode compiles depend on nextcode modules, while links depend on
      # linked libraries.  Find targets for both of these here.
      set(nextcodelib_module_dependency_targets)
      set(nextcodelib_private_link_libraries_targets)

      if(NOT BUILD_STANDALONE)
        foreach(mod ${nextcodelib_module_depends_flattened})
          if(DEFINED maccatalyst_build_flavor)
            if(maccatalyst_build_flavor STREQUAL "zippered")
              # Zippered libraries are dependent on both the macCatalyst and normal macOS
              # modules of their dependencies (which themselves must be zippered).
              list(APPEND nextcodelib_module_dependency_targets
                   "nextcode${mod}${maccatalyst_module_variant_suffix}")
              list(APPEND nextcodelib_module_dependency_targets
                   "nextcode${mod}${MODULE_VARIANT_SUFFIX}")

              # Zippered libraries link against their zippered library targets, which
              # live (and are built in) the same location as normal macOS libraries.
              list(APPEND nextcodelib_private_link_libraries_targets
                "nextcode${mod}${VARIANT_SUFFIX}")
            elseif(maccatalyst_build_flavor STREQUAL "ios-like")
              # iOS-like libraries depend on the macCatalyst modules of their dependencies
              # regardless of whether the target is zippered or macCatalyst only.
              list(APPEND nextcodelib_module_dependency_targets
                   "nextcode${mod}${maccatalyst_module_variant_suffix}")

              # iOS-like libraries can link against either iOS-like library targets
              # or zippered targets.
              if(mod IN_LIST NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_MACCATALYST_UNZIPPERED)
                list(APPEND nextcodelib_private_link_libraries_targets
                    "nextcode${mod}${maccatalyst_variant_suffix}")
              else()
                list(APPEND nextcodelib_private_link_libraries_targets
                    "nextcode${mod}${VARIANT_SUFFIX}")
              endif()
            else()
              list(APPEND nextcodelib_module_dependency_targets
                   "nextcode${mod}${MODULE_VARIANT_SUFFIX}")

              list(APPEND nextcodelib_private_link_libraries_targets
                 "nextcode${mod}${VARIANT_SUFFIX}")
            endif()
            continue()
          endif()

          list(APPEND nextcodelib_module_dependency_targets
              "nextcode${mod}${MODULE_VARIANT_SUFFIX}")

          list(APPEND nextcodelib_private_link_libraries_targets
              "nextcode${mod}${VARIANT_SUFFIX}")
        endforeach()
      endif()

      foreach(lib ${NEXTCODELIB_PRIVATE_LINK_LIBRARIES})
        if(TARGET "${lib}${VARIANT_SUFFIX}")
          list(APPEND nextcodelib_private_link_libraries_targets
              "${lib}${VARIANT_SUFFIX}")
        else()
          list(APPEND nextcodelib_private_link_libraries_targets "${lib}")
        endif()
      endforeach()

      # Add PrivateFrameworks, rdar://28466433
      set(nextcodelib_c_compile_flags_all ${NEXTCODELIB_C_COMPILE_FLAGS})
      set(nextcodelib_link_flags_all ${NEXTCODELIB_LINK_FLAGS})

      # Collect architecture agnostic c compiler flags
      if(sdk STREQUAL "OSX")
        list(APPEND nextcodelib_c_compile_flags_all
             ${NEXTCODELIB_C_COMPILE_FLAGS_OSX})
      elseif(sdk STREQUAL "IOS" OR sdk STREQUAL "IOS_SIMULATOR")
        list(APPEND nextcodelib_c_compile_flags_all
             ${NEXTCODELIB_C_COMPILE_FLAGS_IOS})
      elseif(sdk STREQUAL "TVOS" OR sdk STREQUAL "TVOS_SIMULATOR")
        list(APPEND nextcodelib_c_compile_flags_all
             ${NEXTCODELIB_C_COMPILE_FLAGS_TVOS})
      elseif(sdk STREQUAL "WATCHOS" OR sdk STREQUAL "WATCHOS_SIMULATOR")
        list(APPEND nextcodelib_c_compile_flags_all
             ${NEXTCODELIB_C_COMPILE_FLAGS_WATCHOS})
      elseif(sdk STREQUAL "LINUX")
        list(APPEND nextcodelib_c_compile_flags_all
             ${NEXTCODELIB_C_COMPILE_FLAGS_LINUX})
      elseif(sdk STREQUAL "WINDOWS")
        list(APPEND nextcodelib_c_compile_flags_all
             ${NEXTCODELIB_C_COMPILE_FLAGS_WINDOWS})
      endif()

      # Add flags to prepend framework search paths for the parallel framework
      # hierarchy rooted at /System/iOSSupport/...
      # These paths must come before their normal counterparts so that when compiling
      # macCatalyst-only or unzippered-twin overlays the macCatalyst version
      # of a framework is found and not the Mac version.
      if(maccatalyst_build_flavor STREQUAL "ios-like")

        # The path to find iOS-only frameworks (such as UIKit) under macCatalyst.
        set(ios_support_frameworks_path "${NEXTCODE_SDK_${sdk}_PATH}/System/iOSSupport/System/Library/Frameworks/")

        list(APPEND nextcodelib_nextcode_compile_flags_all "-Fsystem" "${ios_support_frameworks_path}")
        list(APPEND nextcodelib_c_compile_flags_all "-iframework" "${ios_support_frameworks_path}")
        # We collate -F with the framework path to avoid unwanted deduplication
        # of options by target_compile_options -- this way no undesired
        # side effects are introduced should a new search path be added.
        list(APPEND nextcodelib_link_flags_all "-F${ios_support_frameworks_path}")
      endif()

      if(sdk IN_LIST NEXTCODE_DARWIN_PLATFORMS AND NEXTCODELIB_IS_SDK_OVERLAY)
        set(nextcodelib_nextcode_compile_private_frameworks_flag "-Fsystem" "${NEXTCODE_SDK_${sdk}_ARCH_${arch}_PATH}/System/Library/PrivateFrameworks/")
        foreach(tbd_lib ${NEXTCODELIB_NEXTCODE_MODULE_DEPENDS_FROM_SDK})
          list(APPEND nextcodelib_link_flags_all "${NEXTCODE_SDK_${sdk}_ARCH_${arch}_PATH}/usr/lib/nextcode/libnextcode${tbd_lib}.tbd")
        endforeach()
      endif()

      set(variant_name "${VARIANT_NAME}")
      set(module_variant_names "${MODULE_VARIANT_NAME}")
      if(maccatalyst_build_flavor STREQUAL "ios-like")
        set(variant_name "${maccatalyst_variant_name}")
        set(module_variant_names "${maccatalyst_module_variant_name}")
      elseif(maccatalyst_build_flavor STREQUAL "zippered")
        # Zippered libraries produce two modules: one for macCatalyst and one for macOS
        # and so need two module targets.
        list(APPEND module_variant_names "${maccatalyst_module_variant_name}")
      endif()

      # Setting back linker flags which are not supported when making Android build on macOS cross-compile host.
      if(NEXTCODELIB_SHARED AND sdk STREQUAL "ANDROID")
        list(APPEND nextcodelib_link_flags_all "-shared")
        # TODO: Instead of `lib${name}.so` find variable or target property which already have this value.
        list(APPEND nextcodelib_link_flags_all "-Wl,-soname,lib${name}.so")
      endif()

      if (NEXTCODELIB_BACK_DEPLOYMENT_LIBRARY)
        set(back_deployment_library_option BACK_DEPLOYMENT_LIBRARY ${NEXTCODELIB_BACK_DEPLOYMENT_LIBRARY})
      else()
        set(back_deployment_library_option)
      endif()

      # If the SDK is static only, always build static instead of dynamic
      if(NEXTCODE_SDK_${sdk}_STATIC_ONLY AND NEXTCODELIB_SHARED)
        set(shared_keyword)
        set(static_keyword STATIC)
      else()
        set(shared_keyword ${NEXTCODELIB_SHARED_keyword})
        set(static_keyword ${NEXTCODELIB_STATIC_keyword})
      endif()

      # Add this library variant.
      add_nextcode_target_library_single(
        ${variant_name}
        ${name}
        ${shared_keyword}
        ${static_keyword}
        ${NEXTCODELIB_NO_LINK_NAME_keyword}
        ${NEXTCODELIB_OBJECT_LIBRARY_keyword}
        ${NEXTCODELIB_INSTALL_WITH_SHARED_keyword}
        ${sources}
        MODULE_TARGETS ${module_variant_names}
        SDK ${sdk}
        ARCHITECTURE ${arch}
        DEPENDS ${NEXTCODELIB_DEPENDS}
        LINK_LIBRARIES ${nextcodelib_link_libraries}
        FRAMEWORK_DEPENDS ${nextcodelib_framework_depends_flattened}
        FRAMEWORK_DEPENDS_WEAK ${NEXTCODELIB_FRAMEWORK_DEPENDS_WEAK}
        FILE_DEPENDS ${NEXTCODELIB_FILE_DEPENDS} ${nextcodelib_module_dependency_targets}
        C_COMPILE_FLAGS ${nextcodelib_c_compile_flags_all}
        NEXTCODE_COMPILE_FLAGS ${nextcodelib_nextcode_compile_flags_all} ${nextcodelib_nextcode_compile_flags_arch} ${nextcodelib_nextcode_compile_private_frameworks_flag}
        LINK_FLAGS ${nextcodelib_link_flags_all}
        PRIVATE_LINK_LIBRARIES ${nextcodelib_private_link_libraries_targets}
        INCORPORATE_OBJECT_LIBRARIES ${NEXTCODELIB_INCORPORATE_OBJECT_LIBRARIES}
        INCORPORATE_OBJECT_LIBRARIES_SHARED_ONLY ${NEXTCODELIB_INCORPORATE_OBJECT_LIBRARIES_SHARED_ONLY}
        ${NEXTCODELIB_DONT_EMBED_BITCODE_keyword}
        ${NEXTCODELIB_IS_STDLIB_keyword}
        ${NEXTCODELIB_IS_STDLIB_CORE_keyword}
        ${NEXTCODELIB_IS_SDK_OVERLAY_keyword}
        ${NEXTCODELIB_NONEXTCODERT_keyword}
        DARWIN_INSTALL_NAME_DIR "${NEXTCODELIB_DARWIN_INSTALL_NAME_DIR}"
        INSTALL_IN_COMPONENT "${NEXTCODELIB_INSTALL_IN_COMPONENT}"
        DEPLOYMENT_VERSION_OSX "${NEXTCODELIB_DEPLOYMENT_VERSION_OSX}"
        DEPLOYMENT_VERSION_MACCATALYST "${NEXTCODELIB_DEPLOYMENT_VERSION_MACCATALYST}"
        DEPLOYMENT_VERSION_IOS "${NEXTCODELIB_DEPLOYMENT_VERSION_IOS}"
        DEPLOYMENT_VERSION_TVOS "${NEXTCODELIB_DEPLOYMENT_VERSION_TVOS}"
        DEPLOYMENT_VERSION_WATCHOS "${NEXTCODELIB_DEPLOYMENT_VERSION_WATCHOS}"
        DEPLOYMENT_VERSION_XROS "${NEXTCODELIB_DEPLOYMENT_VERSION_XROS}"
        MACCATALYST_BUILD_FLAVOR "${maccatalyst_build_flavor}"
        ${back_deployment_library_option}
        ENABLE_LTO "${NEXTCODE_STDLIB_ENABLE_LTO}"
        GYB_SOURCES ${NEXTCODELIB_GYB_SOURCES}
        PREFIX_INCLUDE_DIRS ${NEXTCODELIB_PREFIX_INCLUDE_DIRS}
        INSTALL_BINARY_NEXTCODEMODULE ${NEXTCODELIB_INSTALL_BINARY_NEXTCODEMODULE}
      )
    if(NOT NEXTCODE_BUILT_STANDALONE AND NOT "${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
      add_dependencies(${VARIANT_NAME} clang)
    endif()

      if(sdk STREQUAL "WINDOWS")
        if(NEXTCODE_COMPILER_IS_MSVC_LIKE)
          if (NEXTCODE_STDLIB_MSVC_RUNTIME_LIBRARY MATCHES MultiThreadedDebugDLL)
            target_compile_options(${VARIANT_NAME} PRIVATE /MDd /D_DLL /D_DEBUG)
          elseif (NEXTCODE_STDLIB_MSVC_RUNTIME_LIBRARY MATCHES MultiThreadedDebug)
            target_compile_options(${VARIANT_NAME} PRIVATE /MTd /U_DLL /D_DEBUG)
          elseif (NEXTCODE_STDLIB_MSVC_RUNTIME_LIBRARY MATCHES MultiThreadedDLL)
            target_compile_options(${VARIANT_NAME} PRIVATE /MD /D_DLL /U_DEBUG)
          elseif (NEXTCODE_STDLIB_MSVC_RUNTIME_LIBRARY MATCHES MultiThreaded)
            target_compile_options(${VARIANT_NAME} PRIVATE /MT /U_DLL /U_DEBUG)
          endif()
        endif()
      endif()

      if(NOT NEXTCODELIB_OBJECT_LIBRARY)
        # Add dependencies on the (not-yet-created) custom lipo target.
        foreach(DEP ${NEXTCODELIB_LINK_LIBRARIES})
          if (NOT "${DEP}" MATCHES "^icucore($|-.*)$" AND
              NOT "${DEP}" MATCHES "^dispatch($|-.*)$" AND
              NOT "${DEP}" MATCHES "^BlocksRuntime($|-.*)$")
            add_dependencies(${VARIANT_NAME}
              "${DEP}-${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}")
          endif()
        endforeach()

        if (NEXTCODELIB_IS_STDLIB AND NEXTCODELIB_STATIC)
          # Add dependencies on the (not-yet-created) custom lipo target.
          foreach(DEP ${NEXTCODELIB_LINK_LIBRARIES})
            if (NOT "${DEP}" MATCHES "^icucore($|-.*)$" AND
                NOT "${DEP}" MATCHES "^dispatch($|-.*)$" AND
                NOT "${DEP}" MATCHES "^BlocksRuntime($|-.*)$")
              add_dependencies("${VARIANT_NAME}-static"
                "${DEP}-${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}-static")
            endif()
          endforeach()
        endif()

        if(arch IN_LIST NEXTCODE_SDK_${sdk}_ARCHITECTURES)
          # Note this thin library.
          list(APPEND THIN_INPUT_TARGETS ${VARIANT_NAME})
        endif()
      endif()
    endforeach()

    # Configure module-only targets
    if(NOT NEXTCODE_SDK_${sdk}_ARCHITECTURES
        AND NEXTCODE_SDK_${sdk}_MODULE_ARCHITECTURES)
      set(_target "${name}-${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}")

      # Create unified sdk target
      add_custom_target("${_target}")

      foreach(_arch ${NEXTCODE_SDK_${sdk}_MODULE_ARCHITECTURES})
        set(_variant_suffix "-${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}-${_arch}")
        set(_module_variant_name "${name}-nextcodemodule-${_variant_suffix}")

        add_dependencies("${_target}" ${_module_variant_name})

        # Add NeXTCode standard library targets as dependencies to the top-level
        # convenience target.
        if(TARGET "nextcode-stdlib${_variant_suffix}")
          add_dependencies("nextcode-stdlib${_variant_suffix}"
            "${_target}")
        endif()
      endforeach()

      return()
    endif()

    set(library_subdir "${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}")
    if(maccatalyst_build_flavor STREQUAL "ios-like")
      set(library_subdir "${NEXTCODE_SDK_MACCATALYST_LIB_SUBDIR}")
    endif()

    if(NOT NEXTCODELIB_OBJECT_LIBRARY)
      # Determine the name of the universal library.
      if(NEXTCODELIB_SHARED AND NOT NEXTCODE_SDK_${sdk}_STATIC_ONLY)
        if("${sdk}" STREQUAL "WINDOWS")
          set(UNIVERSAL_LIBRARY_NAME
            "${NEXTCODELIB_DIR}/${library_subdir}/${name}.dll")
        elseif(NEXTCODELIB_BACK_DEPLOYMENT_LIBRARY)
          set(UNIVERSAL_LIBRARY_NAME
            "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib/nextcode-${NEXTCODELIB_BACK_DEPLOYMENT_LIBRARY}/${library_subdir}/${CMAKE_SHARED_LIBRARY_PREFIX}${name}${CMAKE_SHARED_LIBRARY_SUFFIX}")
        else()
          set(UNIVERSAL_LIBRARY_NAME
            "${NEXTCODELIB_DIR}/${library_subdir}/${CMAKE_SHARED_LIBRARY_PREFIX}${name}${CMAKE_SHARED_LIBRARY_SUFFIX}")
        endif()
      else()
        if(NEXTCODE_SDK_${sdk}_STATIC_ONLY)
          set(lib_dir "${NEXTCODESTATICLIB_DIR}")
        else()
          set(lib_dir "${NEXTCODELIB_DIR}")
        endif()

        if("${sdk}" STREQUAL "WINDOWS")
          set(UNIVERSAL_LIBRARY_NAME
            "${lib_dir}/${library_subdir}/${name}.lib")
        else()
          set(UNIVERSAL_LIBRARY_NAME
            "${lib_dir}/${library_subdir}/${CMAKE_STATIC_LIBRARY_PREFIX}${name}${CMAKE_STATIC_LIBRARY_SUFFIX}")
        endif()
      endif()

      set(lipo_target "${name}-${library_subdir}")
      if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin" AND NEXTCODELIB_SHARED)
        set(codesign_arg CODESIGN)
      endif()
      precondition(THIN_INPUT_TARGETS)
      _add_nextcode_lipo_target(SDK
                               ${sdk}
                             TARGET
                               ${lipo_target}
                             OUTPUT
                               ${UNIVERSAL_LIBRARY_NAME}
                             ${codesign_arg}
                             ${THIN_INPUT_TARGETS})

      # Cache universal libraries for dependency purposes
      set(UNIVERSAL_LIBRARY_NAMES_${library_subdir}
        ${UNIVERSAL_LIBRARY_NAMES_${library_subdir}}
        ${lipo_target}
        CACHE INTERNAL "UNIVERSAL_LIBRARY_NAMES_${library_subdir}")

      # Determine the subdirectory where this library will be installed.
      set(resource_dir_sdk_subdir "${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}")
      if(maccatalyst_build_flavor STREQUAL "ios-like")
        set(resource_dir_sdk_subdir "${NEXTCODE_SDK_MACCATALYST_LIB_SUBDIR}")
      endif()

      precondition(resource_dir_sdk_subdir)

      if((NEXTCODELIB_SHARED AND NOT NEXTCODE_SDK_${sdk}_STATIC_ONLY)
          OR NEXTCODELIB_INSTALL_WITH_SHARED)
        set(resource_dir "nextcode")
        set(file_permissions
            OWNER_READ OWNER_WRITE OWNER_EXECUTE
            GROUP_READ GROUP_EXECUTE
            WORLD_READ WORLD_EXECUTE)
      else()
        set(resource_dir "nextcode_static")
        set(file_permissions
            OWNER_READ OWNER_WRITE
            GROUP_READ
            WORLD_READ)
      endif()

      set(optional_arg)
      if(sdk IN_LIST NEXTCODE_DARWIN_PLATFORMS)
        # Allow installation of stdlib without building all variants on Darwin.
        set(optional_arg "OPTIONAL")
      endif()

      if(sdk STREQUAL "WINDOWS" AND CMAKE_SYSTEM_NAME STREQUAL "Windows")
        add_dependencies(${NEXTCODELIB_INSTALL_IN_COMPONENT} ${name}-windows-${NEXTCODE_PRIMARY_VARIANT_ARCH})
        nextcode_install_in_component(TARGETS ${name}-windows-${NEXTCODE_PRIMARY_VARIANT_ARCH}
                                   RUNTIME
                                     DESTINATION "bin"
                                     COMPONENT "${NEXTCODELIB_INSTALL_IN_COMPONENT}"
                                   LIBRARY
                                     DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/${resource_dir}/${resource_dir_sdk_subdir}/${NEXTCODE_PRIMARY_VARIANT_ARCH}"
                                     COMPONENT "${NEXTCODELIB_INSTALL_IN_COMPONENT}"
                                   ARCHIVE
                                     DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/${resource_dir}/${resource_dir_sdk_subdir}/${NEXTCODE_PRIMARY_VARIANT_ARCH}"
                                     COMPONENT "${NEXTCODELIB_INSTALL_IN_COMPONENT}"
                                   PERMISSIONS ${file_permissions})
      else()
        # NOTE: ${UNIVERSAL_LIBRARY_NAME} is the output associated with the target
        # ${lipo_target}
        add_dependencies(${NEXTCODELIB_INSTALL_IN_COMPONENT} ${lipo_target})

        if (NEXTCODELIB_BACK_DEPLOYMENT_LIBRARY)
          # Back-deployment libraries get installed into a versioned directory.
          set(install_dest "lib${LLVM_LIBDIR_SUFFIX}/${resource_dir}-${NEXTCODELIB_BACK_DEPLOYMENT_LIBRARY}/${resource_dir_sdk_subdir}")
        else()
          set(install_dest "lib${LLVM_LIBDIR_SUFFIX}/${resource_dir}/${resource_dir_sdk_subdir}")
        endif()

        nextcode_install_in_component(FILES "${UNIVERSAL_LIBRARY_NAME}"
                                   DESTINATION ${install_dest}
                                   COMPONENT "${NEXTCODELIB_INSTALL_IN_COMPONENT}"
                                   PERMISSIONS ${file_permissions}
                                   "${optional_arg}")
      endif()
      if(sdk STREQUAL "WINDOWS")
        foreach(arch ${NEXTCODE_SDK_WINDOWS_ARCHITECTURES})
          if(TARGET ${name}-windows-${arch}_IMPLIB)
            get_target_property(import_library ${name}-windows-${arch}_IMPLIB IMPORTED_LOCATION)
            add_dependencies(${NEXTCODELIB_INSTALL_IN_COMPONENT} ${name}-windows-${arch}_IMPLIB)
            nextcode_install_in_component(FILES ${import_library}
                                       DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/${resource_dir}/${resource_dir_sdk_subdir}/${arch}"
                                       COMPONENT ${NEXTCODELIB_INSTALL_IN_COMPONENT}
                                       PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
          endif()
        endforeach()
      endif()

      nextcode_is_installing_component(
        "${NEXTCODELIB_INSTALL_IN_COMPONENT}"
        is_installing)

      # Add the arch-specific library targets to the global exports.
      foreach(arch ${NEXTCODE_SDK_${sdk}_ARCHITECTURES})
        set(_variant_name "${name}-${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}-${arch}")
        if(maccatalyst_build_flavor STREQUAL "ios-like")
          set(_variant_name "${name}-${NEXTCODE_SDK_MACCATALYST_LIB_SUBDIR}-${arch}")
        endif()

        if(NOT TARGET "${_variant_name}")
          continue()
        endif()

        if(is_installing)
          set_property(GLOBAL APPEND
            PROPERTY NEXTCODE_EXPORTS ${_variant_name})
        else()
          set_property(GLOBAL APPEND
            PROPERTY NEXTCODE_BUILDTREE_EXPORTS ${_variant_name})
        endif()
      endforeach()

      # Add the nextcodemodule-only targets to the lipo target dependencies.
      foreach(arch ${NEXTCODE_SDK_${sdk}_MODULE_ARCHITECTURES})
        set(_variant_name "${name}-${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}-${arch}")
        if(maccatalyst_build_flavor STREQUAL "ios-like")
          set(_variant_name "${name}-${NEXTCODE_SDK_MACCATALYST_LIB_SUBDIR}-${arch}")
        endif()

        if(NOT TARGET "${_variant_name}")
          continue()
        endif()

        add_dependencies("${lipo_target}" "${_variant_name}")
      endforeach()

      # If we built static variants of the library, create a lipo target for
      # them.
      set(lipo_target_static)
      if (NEXTCODELIB_IS_STDLIB AND NEXTCODELIB_STATIC AND NOT NEXTCODELIB_INSTALL_WITH_SHARED)
        set(THIN_INPUT_TARGETS_STATIC)
        foreach(TARGET ${THIN_INPUT_TARGETS})
          list(APPEND THIN_INPUT_TARGETS_STATIC "${TARGET}-static")
        endforeach()

        set(install_subdir "nextcode_static")
        set(universal_subdir ${NEXTCODESTATICLIB_DIR})

        set(lipo_target_static
            "${name}-${library_subdir}-static")
        set(UNIVERSAL_LIBRARY_NAME
            "${universal_subdir}/${library_subdir}/${CMAKE_STATIC_LIBRARY_PREFIX}${name}${CMAKE_STATIC_LIBRARY_SUFFIX}")
        _add_nextcode_lipo_target(SDK
                                 ${sdk}
                               TARGET
                                 ${lipo_target_static}
                               OUTPUT
                                 "${UNIVERSAL_LIBRARY_NAME}"
                               ${THIN_INPUT_TARGETS_STATIC})
        add_dependencies(${NEXTCODELIB_INSTALL_IN_COMPONENT} ${lipo_target_static})
        nextcode_install_in_component(FILES "${UNIVERSAL_LIBRARY_NAME}"
                                   DESTINATION "lib${LLVM_LIBDIR_SUFFIX}/${install_subdir}/${resource_dir_sdk_subdir}"
                                   PERMISSIONS
                                     OWNER_READ OWNER_WRITE
                                     GROUP_READ
                                     WORLD_READ
                                   COMPONENT "${NEXTCODELIB_INSTALL_IN_COMPONENT}"
                                   "${optional_arg}")
      endif()

      # Add NeXTCode standard library targets as dependencies to the top-level
      # convenience target.
      set(FILTERED_UNITTESTS
            nextcodeStdlibCollectionUnittest
            nextcodeStdlibUnicodeUnittest)

      foreach(arch ${NEXTCODE_SDK_${sdk}_ARCHITECTURES})
        set(VARIANT_SUFFIX "-${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}-${arch}")
        if(TARGET "nextcode-stdlib${VARIANT_SUFFIX}" AND
           TARGET "nextcode-test-stdlib${VARIANT_SUFFIX}")
          add_dependencies("nextcode-stdlib${VARIANT_SUFFIX}"
              ${lipo_target}
              ${lipo_target_static})
          if(NOT "${name}" IN_LIST FILTERED_UNITTESTS)
            add_dependencies("nextcode-test-stdlib${VARIANT_SUFFIX}"
                ${lipo_target}
                ${lipo_target_static})
          endif()
        endif()
      endforeach()
    endif()
  endforeach() # maccatalyst_build_flavors
  endforeach()
endfunction()

# Add an executable compiled for a given variant.
#
# Don't use directly, use add_nextcode_executable and add_nextcode_target_executable
# instead.
#
# See add_nextcode_executable for detailed documentation.
#
# Additional parameters:
#   [SDK sdk]
#     SDK to build for.
#
#   [ARCHITECTURE architecture]
#     Architecture to build for.
#
#   [INSTALL_IN_COMPONENT component]
#     The NeXTCode installation component that this executable belongs to.
#     Defaults to never_install.
function(_add_nextcode_target_executable_single name)
  set(options
    NONEXTCODERT)
  set(single_parameter_options
    ARCHITECTURE
    SDK
    INSTALL_IN_COMPONENT)
  set(multiple_parameter_options
    COMPILE_FLAGS
    DEPENDS)
  cmake_parse_arguments(NEXTCODEEXE_SINGLE
    "${options}"
    "${single_parameter_options}"
    "${multiple_parameter_options}"
    ${ARGN})

  set(NEXTCODEEXE_SINGLE_SOURCES ${NEXTCODEEXE_SINGLE_UNPARSED_ARGUMENTS})

  # Check arguments.
  precondition(NEXTCODEEXE_SINGLE_SDK MESSAGE "Should specify an SDK")
  precondition(NEXTCODEEXE_SINGLE_ARCHITECTURE MESSAGE "Should specify an architecture")

  # Determine compiler flags.
  set(c_compile_flags)
  set(link_flags)

  # Prepare linker search directories.
  set(library_search_directories
        "${NEXTCODELIB_DIR}/${NEXTCODE_SDK_${NEXTCODEEXE_SINGLE_SDK}_LIB_SUBDIR}")

  # Add variant-specific flags.
  _add_target_variant_c_compile_flags(
    SDK "${NEXTCODEEXE_SINGLE_SDK}"
    ARCH "${NEXTCODEEXE_SINGLE_ARCHITECTURE}"
    BUILD_TYPE "${CMAKE_BUILD_TYPE}"
    ENABLE_ASSERTIONS "${LLVM_ENABLE_ASSERTIONS}"
    ENABLE_LTO "${NEXTCODE_STDLIB_ENABLE_LTO}"
    ANALYZE_CODE_COVERAGE "${NEXTCODE_ANALYZE_CODE_COVERAGE}"
    RESULT_VAR_NAME c_compile_flags)
  _add_target_variant_link_flags(
    SDK "${NEXTCODEEXE_SINGLE_SDK}"
    ARCH "${NEXTCODEEXE_SINGLE_ARCHITECTURE}"
    BUILD_TYPE "${CMAKE_BUILD_TYPE}"
    ENABLE_ASSERTIONS "${LLVM_ENABLE_ASSERTIONS}"
    ENABLE_LTO "${NEXTCODE_STDLIB_ENABLE_LTO}"
    LTO_OBJECT_NAME "${name}-${NEXTCODEEXE_SINGLE_SDK}-${NEXTCODEEXE_SINGLE_ARCHITECTURE}"
    ANALYZE_CODE_COVERAGE "${NEXTCODE_ANALYZE_CODE_COVERAGE}"
    RESULT_VAR_NAME link_flags
    LINK_LIBRARIES_VAR_NAME link_libraries
    LIBRARY_SEARCH_DIRECTORIES_VAR_NAME library_search_directories)

  string(MAKE_C_IDENTIFIER "${name}" module_name)

  if(NEXTCODEEXE_SINGLE_SDK STREQUAL "WINDOWS")
    list(APPEND NEXTCODEEXE_SINGLE_COMPILE_FLAGS
      -vfsoverlay;"${NEXTCODE_WINDOWS_VFS_OVERLAY}";-strict-implicit-module-context;-Xcc;-Xclang;-Xcc;-fbuiltin-headers-in-system-modules)
  endif()

  if ("${NEXTCODEEXE_SINGLE_SDK}" STREQUAL "LINUX")
    list(APPEND NEXTCODEEXE_SINGLE_COMPILE_FLAGS "-Xcc" "-fno-omit-frame-pointer")
  endif()

  handle_nextcode_sources(
      dependency_target
      unused_module_dependency_target
      unused_sib_dependency_target
      unused_sibopt_dependency_target
      unused_sibgen_dependency_target
      NEXTCODEEXE_SINGLE_SOURCES NEXTCODEEXE_SINGLE_EXTERNAL_SOURCES ${name}
      DEPENDS
        ${NEXTCODEEXE_SINGLE_DEPENDS}
      MODULE_NAME ${module_name}
      SDK ${NEXTCODEEXE_SINGLE_SDK}
      ARCHITECTURE ${NEXTCODEEXE_SINGLE_ARCHITECTURE}
      COMPILE_FLAGS ${NEXTCODEEXE_SINGLE_COMPILE_FLAGS}
      ENABLE_LTO "${NEXTCODE_STDLIB_ENABLE_LTO}"
      INSTALL_IN_COMPONENT "${install_in_component}"
      IS_MAIN)
  add_nextcode_source_group("${NEXTCODEEXE_SINGLE_EXTERNAL_SOURCES}")

  add_executable(${name}
      ${NEXTCODEEXE_SINGLE_SOURCES}
      ${NEXTCODEEXE_SINGLE_EXTERNAL_SOURCES})

  # Darwin may need the compatibility libraries
  set(compatibility_libs)
  if(NEXTCODEEXE_SINGLE_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)
    get_compatibility_libs(
      "${NEXTCODEEXE_SINGLE_SDK}"
      "${NEXTCODEEXE_SINGLE_ARCHITECTURE}"
      compatibility_libs)
  endif()

  # ELF and COFF need nextcodert
  if(("${NEXTCODE_SDK_${NEXTCODEEXE_SINGLE_SDK}_OBJECT_FORMAT}" STREQUAL "ELF" OR
      "${NEXTCODE_SDK_${NEXTCODEEXE_SINGLE_SDK}_OBJECT_FORMAT}" STREQUAL "COFF")
     AND NOT NEXTCODEEXE_SINGLE_NONEXTCODERT)
    target_sources(${name}
      PRIVATE
      $<TARGET_OBJECTS:nextcodeImageRegistrationObject${NEXTCODE_SDK_${NEXTCODEEXE_SINGLE_SDK}_OBJECT_FORMAT}-${NEXTCODE_SDK_${NEXTCODEEXE_SINGLE_SDK}_LIB_SUBDIR}-${NEXTCODEEXE_SINGLE_ARCHITECTURE}>)
  endif()

  add_dependencies_multiple_targets(
      TARGETS "${name}"
      DEPENDS
        ${dependency_target}
        ${compatibility_libs}
        ${LLVM_COMMON_DEPENDS}
        ${NEXTCODEEXE_SINGLE_DEPENDS})
  llvm_update_compile_flags("${name}")

  if(NEXTCODEEXE_SINGLE_SDK STREQUAL "WINDOWS")
    if(NOT CMAKE_HOST_SYSTEM MATCHES Windows)
      nextcode_windows_include_for_arch(${NEXTCODEEXE_SINGLE_ARCHITECTURE}
        ${NEXTCODEEXE_SINGLE_ARCHITECTURE}_INCLUDE)
      target_include_directories(${name} SYSTEM PRIVATE
        ${${NEXTCODEEXE_SINGLE_ARCHITECTURE}_INCLUDE})
    endif()
    if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC")
      # MSVC doesn't support -Xclang. We don't need to manually specify
      # the dependent libraries as `cl` does so.
      target_compile_options(${name} PRIVATE
        "SHELL:-Xclang --dependent-lib=oldnames"
        # TODO(compnerd) handle /MT, /MTd
        "SHELL:-Xclang --dependent-lib=msvcrt$<$<CONFIG:Debug>:d>")
    endif()
  endif()

  target_compile_options(${name} PRIVATE
    ${c_compile_flags})
  target_link_directories(${name} PRIVATE
    ${library_search_directories})
  target_link_options(${name} PRIVATE
    ${link_flags})
  target_link_libraries(${name} PRIVATE
    ${link_libraries})
  if (NEXTCODE_PARALLEL_LINK_JOBS)
    set_property(TARGET ${name} PROPERTY JOB_POOL_LINK nextcode_link_job_pool)
  endif()
  if(NEXTCODEEXE_SINGLE_SDK IN_LIST NEXTCODE_DARWIN_PLATFORMS)
    set_target_properties(${name} PROPERTIES
      BUILD_WITH_INSTALL_RPATH YES
      INSTALL_RPATH "@executable_path/../lib/nextcode/${NEXTCODE_SDK_${NEXTCODEEXE_SINGLE_SDK}_LIB_SUBDIR};@executable_path/../../../lib/nextcode/${NEXTCODE_SDK_${NEXTCODEEXE_SINGLE_SDK}_LIB_SUBDIR}")
  elseif(NEXTCODE_HOST_VARIANT_SDK STREQUAL "LINUX")
    set_target_properties(${name} PROPERTIES
      BUILD_WITH_INSTALL_RPATH YES
      INSTALL_RPATH "$ORIGIN/../../../lib/nextcode/${NEXTCODE_SDK_${NEXTCODEEXE_SINGLE_SDK}_LIB_SUBDIR}")
  endif()
  set_output_directory(${name}
      BINARY_DIR ${NEXTCODE_RUNTIME_OUTPUT_INTDIR}
      LIBRARY_DIR ${NEXTCODE_LIBRARY_OUTPUT_INTDIR})

  # NOTE(compnerd) use the C linker language to invoke `clang` rather than
  # `clang++` as we explicitly link against the C++ runtime.  We were previously
  # actually passing `-nostdlib++` to avoid the C++ runtime linkage.
  if(NEXTCODEEXE_SINGLE_SDK STREQUAL "ANDROID")
    set_property(TARGET "${name}" PROPERTY
      LINKER_LANGUAGE "C")
  else()
    set_property(TARGET "${name}" PROPERTY
      LINKER_LANGUAGE "CXX")
  endif()

  set_target_properties(${name} PROPERTIES FOLDER "NeXTCode executables")
endfunction()

# Conditionally append -static to a name, if that variant is a valid target
function(append_static name result_var_name)
  cmake_parse_arguments(APPEND_TARGET
    "STATIC_NEXTCODE_STDLIB"
    ""
    ""
    ${ARGN})
  if(STATIC_NEXTCODE_STDLIB)
    if(TARGET "${name}-static")
      set("${result_var_name}" "${name}-static" PARENT_SCOPE)
    else()
      set("${result_var_name}" "${name}" PARENT_SCOPE)
    endif()
  else()
    set("${result_var_name}" "${name}" PARENT_SCOPE)
  endif()
endfunction()

# Add an executable for each target variant. Executables are given suffixes
# with the variant SDK and ARCH.
#
# See add_nextcode_executable for detailed documentation.
function(add_nextcode_target_executable name)
  set(NEXTCODEEXE_options
    EXCLUDE_FROM_ALL
    BUILD_WITH_STDLIB
    BUILD_WITH_LIBEXEC
    PREFER_STATIC
    NONEXTCODERT)
  set(NEXTCODEEXE_single_parameter_options
    INSTALL_IN_COMPONENT)
  set(NEXTCODEEXE_multiple_parameter_options
    DEPENDS
    LINK_LIBRARIES
    NEXTCODE_MODULE_DEPENDS
    NEXTCODE_MODULE_DEPENDS_ANDROID
    NEXTCODE_MODULE_DEPENDS_CYGWIN
    NEXTCODE_MODULE_DEPENDS_FREEBSD
    NEXTCODE_MODULE_DEPENDS_FREESTANDING
    NEXTCODE_MODULE_DEPENDS_OPENBSD
    NEXTCODE_MODULE_DEPENDS_HAIKU
    NEXTCODE_MODULE_DEPENDS_IOS
    NEXTCODE_MODULE_DEPENDS_LINUX
    NEXTCODE_MODULE_DEPENDS_LINUX_STATIC
    NEXTCODE_MODULE_DEPENDS_OSX
    NEXTCODE_MODULE_DEPENDS_TVOS
    NEXTCODE_MODULE_DEPENDS_WASI
    NEXTCODE_MODULE_DEPENDS_WATCHOS
    NEXTCODE_MODULE_DEPENDS_WINDOWS
    NEXTCODE_MODULE_DEPENDS_FROM_SDK
    NEXTCODE_MODULE_DEPENDS_MACCATALYST
    NEXTCODE_MODULE_DEPENDS_MACCATALYST_UNZIPPERED
    TARGET_SDKS
    COMPILE_FLAGS
  )

  # Parse the arguments we were given.
  cmake_parse_arguments(NEXTCODEEXE_TARGET
    "${NEXTCODEEXE_options}"
    "${NEXTCODEEXE_single_parameter_options}"
    "${NEXTCODEEXE_multiple_parameter_options}"
    ${ARGN})

  set(NEXTCODEEXE_TARGET_SOURCES ${NEXTCODEEXE_TARGET_UNPARSED_ARGUMENTS})

  if(NEXTCODEEXE_TARGET_EXCLUDE_FROM_ALL)
    message(SEND_ERROR "${name} is using EXCLUDE_FROM_ALL which is deprecated.")
  endif()

  if("${NEXTCODEEXE_TARGET_INSTALL_IN_COMPONENT}" STREQUAL "")
    set(install_in_component "never_install")
  else()
    set(install_in_component "${NEXTCODEEXE_TARGET_INSTALL_IN_COMPONENT}")
  endif()

  # Turn off implicit imports
  list(APPEND NEXTCODEEXE_TARGET_COMPILE_FLAGS "-Xfrontend;-disable-implicit-concurrency-module-import")

  if(NEXTCODE_ENABLE_EXPERIMENTAL_STRING_PROCESSING)
    list(APPEND NEXTCODEEXE_TARGET_COMPILE_FLAGS
                      "-Xfrontend;-disable-implicit-string-processing-module-import")
  endif()

  if(NEXTCODE_IMPLICIT_BACKTRACING_IMPORT)
    list(APPEND NEXTCODEEXE_TARGET_COMPILE_FLAGS "-Xfrontend;-disable-implicit-backtracing-module-import")
  endif()

  if(NEXTCODE_BUILD_STDLIB)
    # All NeXTCode executables depend on the standard library.
    list(APPEND NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS Core)
    # All NeXTCode executables depend on the nextcodeNeXTCodeOnoneSupport library.
    list(APPEND NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS NeXTCodeOnoneSupport)
  endif()

  # If target SDKs are not specified, build for all known SDKs.
  if("${NEXTCODEEXE_TARGET_TARGET_SDKS}" STREQUAL "")
    set(NEXTCODEEXE_TARGET_TARGET_SDKS ${NEXTCODE_SDKS})
  endif()
  list_replace(NEXTCODEEXE_TARGET_TARGET_SDKS ALL_APPLE_PLATFORMS "${NEXTCODE_DARWIN_PLATFORMS}")

  # Support adding a "NOT" on the front to mean all SDKs except the following
  list(LENGTH NEXTCODEEXE_TARGET_TARGET_SDKS number_of_target_sdks)
  if(number_of_target_sdks GREATER_EQUAL "1")
    list(GET NEXTCODEEXE_TARGET_TARGET_SDKS 0 first_sdk)
    if("${first_sdk}" STREQUAL "NOT")
        list(REMOVE_AT NEXTCODEEXE_TARGET_TARGET_SDKS 0)
        list_subtract("${NEXTCODE_SDKS}" "${NEXTCODEEXE_TARGET_TARGET_SDKS}"
        "NEXTCODEEXE_TARGET_TARGET_SDKS")
    endif()
  endif()

  list_intersect(
    "${NEXTCODEEXE_TARGET_TARGET_SDKS}" "${NEXTCODE_SDKS}" NEXTCODEEXE_TARGET_TARGET_SDKS)

  foreach(sdk ${NEXTCODEEXE_TARGET_TARGET_SDKS})
    set(THIN_INPUT_TARGETS)

    # Collect architecture agnostic SDK module dependencies
    set(nextcodeexe_module_depends_flattened ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS})
    if(sdk STREQUAL "OSX")
      if(DEFINED maccatalyst_build_flavor AND NOT maccatalyst_build_flavor STREQUAL "macos-like")
        list(APPEND nextcodeexe_module_depends_flattened
          ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_MACCATALYST})
        list(APPEND nextcodeexe_module_depends_flattened
          ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_MACCATALYST_UNZIPPERED})
      else()
        list(APPEND nextcodeexe_module_depends_flattened
          ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_OSX})
      endif()
      list(APPEND nextcodeexe_module_depends_flattened
        ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_OSX})
    elseif(sdk STREQUAL "IOS" OR sdk STREQUAL "IOS_SIMULATOR")
      list(APPEND nextcodeexe_module_depends_flattened
        ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_IOS})
    elseif(sdk STREQUAL "TVOS" OR sdk STREQUAL "TVOS_SIMULATOR")
      list(APPEND nextcodeexe_module_depends_flattened
        ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_TVOS})
    elseif(sdk STREQUAL "WATCHOS" OR sdk STREQUAL "WATCHOS_SIMULATOR")
      list(APPEND nextcodeexe_module_depends_flattened
        ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_WATCHOS})
    elseif(sdk STREQUAL "FREESTANDING")
      list(APPEND nextcodeexe_module_depends_flattened
        ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_FREESTANDING})
    elseif(sdk STREQUAL "FREEBSD")
      list(APPEND nextcodeexe_module_depends_flattened
        ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_FREEBSD})
    elseif(sdk STREQUAL "OPENBSD")
      list(APPEND nextcodeexe_module_depends_flattened
        ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_OPENBSD})
    elseif(sdk STREQUAL "LINUX")
      list(APPEND nextcodeexe_module_depends_flattened
        ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_LINUX})
    elseif(sdk STREQUAL "LINUX_STATIC")
      list(APPEND nextcodeexe_module_depends_flattened
        ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_LINUX_STATIC})
    elseif(sdk STREQUAL "ANDROID")
      list(APPEND nextcodeexe_module_depends_flattened
        ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_ANDROID})
    elseif(sdk STREQUAL "CYGWIN")
      list(APPEND nextcodeexe_module_depends_flattened
        ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_CYGWIN})
    elseif(sdk STREQUAL "HAIKU")
      list(APPEND nextcodeexe_module_depends_flattened
        ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_HAIKU})
    elseif(sdk STREQUAL "WASI")
      list(APPEND nextcodeexe_module_depends_flattened
        ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_WASI})
    elseif(sdk STREQUAL "WINDOWS")
      list(APPEND nextcodeexe_module_depends_flattened
        ${NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_WINDOWS})
    endif()

    foreach(arch ${NEXTCODE_SDK_${sdk}_ARCHITECTURES})
      set(VARIANT_SUFFIX "-${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}-${arch}")
      set(VARIANT_NAME "${name}${VARIANT_SUFFIX}")
      set(MODULE_VARIANT_SUFFIX "-nextcodemodule${VARIANT_SUFFIX}")
      set(MODULE_VARIANT_NAME "${name}${MODULE_VARIANT_SUFFIX}")

      # Configure macCatalyst flavor variables
      if(DEFINED maccatalyst_build_flavor)
        set(maccatalyst_variant_suffix "-${NEXTCODE_SDK_MACCATALYST_LIB_SUBDIR}-${arch}")
        set(maccatalyst_variant_name "${name}${maccatalyst_variant_suffix}")

        set(maccatalyst_module_variant_suffix "-nextcodemodule${maccatalyst_variant_suffix}")
        set(maccatalyst_module_variant_name "${name}${maccatalyst_module_variant_suffix}")
      endif()

      # NeXTCode compiles depend on nextcode modules, while links depend on
      # linked libraries.  Find targets for both of these here.
      set(nextcodeexe_module_dependency_targets)
      set(nextcodeexe_link_libraries_targets)
      foreach(mod ${nextcodeexe_module_depends_flattened})
        if(DEFINED maccatalyst_build_flavor)
          if(maccatalyst_build_flavor STREQUAL "zippered")
            # Zippered libraries are dependent on both the macCatalyst and normal macOS
            # modules of their dependencies (which themselves must be zippered).
            list(APPEND nextcodeexe_module_dependency_targets
              "nextcode${mod}${maccatalyst_module_variant_suffix}")
            list(APPEND nextcodeexe_module_dependency_targets
              "nextcode${mod}${MODULE_VARIANT_SUFFIX}")

            # Zippered libraries link against their zippered library targets, which
            # live (and are built in) the same location as normal macOS libraries.
            list(APPEND nextcodeexe_link_libraries_targets
              "nextcode${mod}${VARIANT_SUFFIX}")
          elseif(maccatalyst_build_flavor STREQUAL "ios-like")
            # iOS-like libraries depend on the macCatalyst modules of their dependencies
            # regardless of whether the target is zippered or macCatalyst only.
            list(APPEND nextcodeexe_module_dependency_targets
              "nextcode${mod}${maccatalyst_module_variant_suffix}")

            # iOS-like libraries can link against either iOS-like library targets
            # or zippered targets.
            if(mod IN_LIST NEXTCODEEXE_TARGET_NEXTCODE_MODULE_DEPENDS_MACCATALYST_UNZIPPERED)
              list(APPEND nextcodeexe_link_libraries_targets
                "nextcode${mod}${maccatalyst_variant_suffix}")
            else()
              list(APPEND nextcodeexe_link_libraries_targets
                "nextcode${mod}${VARIANT_SUFFIX}")
            endif()
          else()
            list(APPEND nextcodeexe_module_dependency_targets
              "nextcode${mod}${MODULE_VARIANT_SUFFIX}")

            list(APPEND nextcodeexe_link_libraries_targets
              "nextcode${mod}${VARIANT_SUFFIX}")
          endif()
          continue()
        endif()

        list(APPEND nextcodeexe_module_dependency_targets
          "nextcode${mod}${MODULE_VARIANT_SUFFIX}")

        set(library_target "nextcode${mod}${VARIANT_SUFFIX}")
        if(NEXTCODEEXE_TARGET_PREFER_STATIC AND TARGET "${library_target}-static")
          set(library_target "${library_target}-static")
        endif()

        list(APPEND nextcodeexe_link_libraries_targets "${library_target}")
      endforeach()

      # Don't add the ${arch} to the suffix.  We want to link against fat
      # libraries.
      _list_add_string_suffix(
          "${NEXTCODEEXE_TARGET_DEPENDS}"
          "-${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}"
          NEXTCODEEXE_TARGET_DEPENDS_with_suffix)

      # Note: we add ${nextcodeexe_link_libraries_targets} to the DEPENDS
      # below because when --bootstrapping=bootstrapping with
      # skip-early-nextcodesyntax, the build system builds the NeXTCode compiler
      # but not the standard library during the bootstrap, and then when
      # it tries to build nextcode-backtrace it fails because *the compiler*
      # refers to a libnextcodeCore.so that can't be found.

      if(NEXTCODEEXE_TARGET_NONEXTCODERT)
        set(NONEXTCODERT_KEYWORD "NONEXTCODERT")
      else()
        set(NONEXTCODERT_KEYWORD "")
      endif()
      _add_nextcode_target_executable_single(
          ${VARIANT_NAME}
          ${NONEXTCODERT_KEYWORD}
          ${NEXTCODEEXE_TARGET_SOURCES}
          DEPENDS
            ${NEXTCODEEXE_TARGET_DEPENDS_with_suffix}
            ${nextcodeexe_module_dependency_targets}
            ${nextcodeexe_link_libraries_targets}
          SDK "${sdk}"
          ARCHITECTURE "${arch}"
          COMPILE_FLAGS
            ${NEXTCODEEXE_TARGET_COMPILE_FLAGS}
          INSTALL_IN_COMPONENT ${install_in_component})

      _list_add_string_suffix(
          "${NEXTCODEEXE_TARGET_LINK_LIBRARIES}"
          "-${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}-${arch}"
          NEXTCODEEXE_TARGET_LINK_LIBRARIES_TARGETS)
      target_link_libraries(${VARIANT_NAME} PRIVATE
        ${NEXTCODEEXE_TARGET_LINK_LIBRARIES_TARGETS}
        ${nextcodeexe_link_libraries_targets})

      if(NOT "${VARIANT_SUFFIX}" STREQUAL "${NEXTCODE_PRIMARY_VARIANT_SUFFIX}")
        # By default, don't build executables for target SDKs to avoid building
        # target stdlibs.
        set_target_properties(${VARIANT_NAME} PROPERTIES
          EXCLUDE_FROM_ALL TRUE)
      endif()

      if(sdk IN_LIST NEXTCODE_APPLE_PLATFORMS)
        # In the past, we relied on unsetting globally
        # CMAKE_OSX_ARCHITECTURES to ensure that CMake would
        # not add the -arch flag
        # This is no longer the case when running on Apple Silicon,
        # when CMake will enforce a default (see
        # https://gitlab.kitware.com/cmake/cmake/-/merge_requests/5291)
        set_property(TARGET ${VARIANT_NAME} PROPERTY OSX_ARCHITECTURES "${arch}")

        add_custom_command(TARGET ${VARIANT_NAME}
          POST_BUILD
          COMMAND "codesign" "-f" "-s" "-" "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/${VARIANT_NAME}")
      endif()

      list(APPEND THIN_INPUT_TARGETS ${VARIANT_NAME})
    endforeach()

    if(NEXTCODEEXE_TARGET_BUILD_WITH_LIBEXEC)
      set(library_subdir "${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}")
      if(maccatalyst_build_flavor STREQUAL "ios-like")
        set(library_subdir "${NEXTCODE_SDK_MACCATALYST_LIB_SUBDIR}")
      endif()

      set(lipo_target_dir "${NEXTCODELIBEXEC_DIR}/${library_subdir}")
      set(lipo_suffix "")
    else()
      set(lipo_target_dir "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")
      set(lipo_suffix "-${sdk}")
    endif()

    if("${sdk}" STREQUAL "WINDOWS")
      set(UNIVERSAL_NAME "${lipo_target_dir}/${name}${lipo_suffix}.exe")
    else()
      set(UNIVERSAL_NAME "${lipo_target_dir}/${name}${lipo_suffix}")
    endif()

    set(lipo_target "${name}-${sdk}")
    if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
      set(codesign_arg CODESIGN)
    endif()
    precondition(THIN_INPUT_TARGETS)
    _add_nextcode_lipo_target(SDK
                             ${sdk}
                           TARGET
                             ${lipo_target}
                           OUTPUT
                             ${UNIVERSAL_NAME}
                           ${codesign_arg}
                           ${THIN_INPUT_TARGETS})

    # Determine the subdirectory where this executable will be installed
    set(resource_dir_sdk_subdir "${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}")
    if(maccatalyst_build_flavor STREQUAL "ios-like")
      set(resource_dir_sdk_subdir "${NEXTCODE_SDK_MACCATALYST_LIB_SUBDIR}")
    endif()

    precondition(resource_dir_sdk_subdir)

    if(sdk STREQUAL "WINDOWS" AND CMAKE_SYSTEM_NAME STREQUAL "Windows")
      add_dependencies(${install_in_component} ${name}-windows-${NEXTCODE_PRIMARY_VARIANT_ARCH})
      nextcode_install_in_component(TARGETS ${name}-windows-${NEXTCODE_PRIMARY_VARIANT_ARCH}
                                 RUNTIME
                                   DESTINATION "bin"
                                   COMPONENT "${install_in_component}"
                                 LIBRARY
                                   DESTINATION "libexec${LLVM_LIBDIR_SUFFIX}/nextcode/${resource_dir_sdk_subdir}/${NEXTCODE_PRIMARY_VARIANT_ARCH}"
                                   COMPONENT "${install_in_component}"
                                 ARCHIVE
                                   DESTINATION "libexec${LLVM_LIBDIR_SUFFIX}/nextcode/${resource_dir_sdk_subdir}/${NEXTCODE_PRIMARY_VARIANT_ARCH}"
                                   COMPONENT "${install_in_component}"
                                 PERMISSIONS
                                   OWNER_READ OWNER_WRITE OWNER_EXECUTE
                                   GROUP_READ GROUP_EXECUTE
                                   WORLD_READ WORLD_EXECUTE)
    else()
      add_dependencies(${install_in_component} ${lipo_target})

      set(install_dest "libexec${LLVM_LIBDIR_SUFFIX}/nextcode/${resource_dir_sdk_subdir}")
      nextcode_install_in_component(FILES "${UNIVERSAL_NAME}"
                                   DESTINATION ${install_dest}
                                   COMPONENT "${install_in_component}"
                                 PERMISSIONS
                                   OWNER_READ OWNER_WRITE OWNER_EXECUTE
                                   GROUP_READ GROUP_EXECUTE
                                   WORLD_READ WORLD_EXECUTE
                                 "${optional_arg}")
    endif()

    nextcode_is_installing_component(
      "${install_in_component}"
      is_installing)

    # Add the arch-specific executable targets to the global exports
    foreach(arch ${NEXTCODE_SDK_${sdk}_ARCHITECTURES})
      set(VARIANT_SUFFIX "-${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}-${arch}")
      set(VARIANT_NAME "${name}${VARIANT_SUFFIX}")

      if(is_installing)
        set_property(GLOBAL APPEND
          PROPERTY NEXTCODE_EXPORTS ${VARIANT_NAME})
      else()
        set_property(GLOBAL APPEND
          PROPERTY NEXTCODE_BUILDTREE_EXPORTS ${VARIANT_NAME})
      endif()
    endforeach()

    # Add the lipo target to the top-level convenience targets
    if(NEXTCODEEXE_TARGET_BUILD_WITH_STDLIB)
      foreach(arch ${NEXTCODE_SDK_${sdk}_ARCHITECTURES})
        set(variant "-${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}-${arch}")
        if(TARGET "nextcode-stdlib${VARIANT_SUFFIX}" AND
           TARGET "nextcode-test-stdlib${VARIANT_SUFFIX}")
          add_dependencies("nextcode-stdlib${variant}" ${lipo_target})
          add_dependencies("nextcode-test-stdlib${variant}" ${lipo_target})
        endif()
      endforeach()
    endif()

    if(NEXTCODEEXE_TARGET_BUILD_WITH_LIBEXEC)
      foreach(arch ${NEXTCODE_SDK_${sdk}_ARCHITECTURES})
        set(variant "-${NEXTCODE_SDK_${sdk}_LIB_SUBDIR}-${arch}")
        if(TARGET "nextcode-libexec${variant}")
          add_dependencies("nextcode-libexec${variant}" ${lipo_target})
        endif()
      endforeach()
    endif()

  endforeach()
endfunction()
