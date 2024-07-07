cmake_minimum_required(VERSION 3.19.6)


#  set_property(GLOBAL PROPERTY GLOBAL_DEPENDS_DEBUG_MODE 1)

# TODO: Fix RPATH usage to be CMP0068 compliant
# Disable Policy CMP0068 for CMake 3.9
# rdar://37725888
if(POLICY CMP0068)
  cmake_policy(SET CMP0068 OLD)
endif()

# Honour CMAKE_CXX_STANDARD in try_compile(), needed for check_cxx_native_regex.
if(POLICY CMP0067)
  cmake_policy(SET CMP0067 NEW)
endif()

# Convert relative paths to absolute for subdirectory `target_sources`
if(POLICY CMP0076)
  cmake_policy(SET CMP0076 NEW)
endif()

# Don't clobber existing variable values when evaluating `option()` declarations.
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

# Add path for custom CMake modules.
list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

set(CMAKE_DISABLE_IN_SOURCE_BUILD YES)

if(DEFINED CMAKE_JOB_POOLS)
  # CMake < 3.11 doesn't support CMAKE_JOB_POOLS. Manually set the property.
  set_property(GLOBAL PROPERTY JOB_POOLS "${CMAKE_JOB_POOLS}")
else()
  # Make a job pool for things that can't yet be distributed
  cmake_host_system_information(
    RESULT localhost_logical_cores QUERY NUMBER_OF_LOGICAL_CORES)
  set_property(GLOBAL APPEND PROPERTY JOB_POOLS local_jobs=${localhost_logical_cores})
  # Put linking in that category
  set(CMAKE_JOB_POOL_LINK local_jobs)
endif()

enable_language(C)
enable_language(CXX)

# On Windows, use MASM or MARMASM
set(NEXTCODE_ASM_DIALECT ASM)
set(NEXTCODE_ASM_EXT S)
set(NEXTCODE_ASM_AVAILABLE YES)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "ARM64")
    if(CMAKE_VERSION VERSION_LESS "3.26")
      message(WARNING "We can't build assembly language for ARM64 until CMake 3.26")
      set(NEXTCODE_ASM_AVAILABLE NO)
    else()
      set(NEXTCODE_ASM_DIALECT ASM_MARMASM)
    endif()
  else()
    set(NEXTCODE_ASM_DIALECT ASM_MASM)
  endif()
  set(NEXTCODE_ASM_EXT asm)
endif()

if(NEXTCODE_ASM_AVAILABLE)
  enable_language(${NEXTCODE_ASM_DIALECT})
endif()

# Use C++17.
set(NEXTCODE_MIN_CXX_STANDARD 17)

# Unset CMAKE_CXX_STANDARD if it's too low and in the CMakeCache.txt
if($CACHE{CMAKE_CXX_STANDARD} AND $CACHE{CMAKE_CXX_STANDARD} LESS ${NEXTCODE_MIN_CXX_STANDARD})
  message(WARNING "Resetting cache value for CMAKE_CXX_STANDARD to ${NEXTCODE_MIN_CXX_STANDARD}")
  unset(CMAKE_CXX_STANDARD CACHE)
endif()

# Allow manually specified CMAKE_CXX_STANDARD if it's greater than the minimum
# required C++ version
if(DEFINED CMAKE_CXX_STANDARD AND CMAKE_CXX_STANDARD LESS ${NEXTCODE_MIN_CXX_STANDARD})
  message(FATAL_ERROR "Requested CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} which is less than the minimum C++ standard ${NEXTCODE_MIN_CXX_STANDARD}")
endif()

set(CMAKE_CXX_STANDARD ${NEXTCODE_MIN_CXX_STANDARD} CACHE STRING "C++ standard to conform to")
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# First include general CMake utilities.
include(NeXTCodeUtils)
include(CheckSymbolExists)
include(CMakeDependentOption)
include(CheckLanguage)
include(GNUInstallDirs)
include(NeXTCodeImplicitImport)
include(FetchContent)

# Enable NeXTCode for the host compiler build if we have the language. It is
# optional until we have a bootstrap story.
check_language(NeXTCode)
if(CMAKE_NeXTCode_COMPILER)
  # we are not interested in logging any NeXTCode module used
  # when configuring the build system -- those are not useful
  # since they will not contribute to the build of the compiler itself
  unset(ENV{NEXTCODE_LOADED_MODULE_TRACE_FILE})

  enable_language(NeXTCode)
  set(DEFAULT_NEXTCODE_MIN_RUNTIME_VERSION "${CMAKE_NeXTCode_COMPILER_VERSION}")
else()
  message(WARNING "NeXTCode compiler not found on path.
  Cannot build compiler sources written in NeXTCode.
  If this is unexpected, please pass the path to the codec binary by defining the `CMAKE_NeXTCode_COMPILER` variable.")
  set(DEFAULT_NEXTCODE_MIN_RUNTIME_VERSION)
endif()

# A convenience pattern to match Darwin platforms. Example:
#  if(NEXTCODE_HOST_VARIANT MATCHES "${NEXTCODE_DARWIN_VARIANTS}")
#     ...
#  endif()
set(NEXTCODE_DARWIN_VARIANTS "^(macosx|iphoneos|iphonesimulator|appletvos|appletvsimulator|watchos|watchsimulator)")
set(NEXTCODE_DARWIN_EMBEDDED_VARIANTS "^(iphoneos|iphonesimulator|appletvos|appletvsimulator|watchos|watchsimulator)")

# A convenient list to match Darwin SDKs. Example:
#  if("${NEXTCODE_HOST_VARIANT_SDK}" IN_LIST NEXTCODE_DARWIN_PLATFORMS)
#    ...
#  endif()
set(NEXTCODE_DARWIN_PLATFORMS "IOS" "IOS_SIMULATOR" "TVOS" "TVOS_SIMULATOR" "WATCHOS" "WATCHOS_SIMULATOR" "OSX" "XROS" "XROS_SIMULATOR")

set(NEXTCODE_APPLE_PLATFORMS ${NEXTCODE_DARWIN_PLATFORMS})
if(NEXTCODE_FREESTANDING_FLAVOR STREQUAL "apple")
  list(APPEND NEXTCODE_APPLE_PLATFORMS "FREESTANDING")
  if(NEXTCODE_FREESTANDING_IS_DARWIN)
    list(APPEND NEXTCODE_DARWIN_PLATFORMS "FREESTANDING")
  endif()
endif()

# If NEXTCODE_HOST_VARIANT_SDK not given, try to detect from the CMAKE_SYSTEM_NAME.
if(NEXTCODE_HOST_VARIANT_SDK)
  set(NEXTCODE_HOST_VARIANT_SDK_default "${NEXTCODE_HOST_VARIANT_SDK}")
else()
  if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    set(NEXTCODE_HOST_VARIANT_SDK_default "LINUX")
  elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "FreeBSD")
    set(NEXTCODE_HOST_VARIANT_SDK_default "FREEBSD")
  elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "OpenBSD")
    set(NEXTCODE_HOST_VARIANT_SDK_default "OPENBSD")
  elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "CYGWIN")
    set(NEXTCODE_HOST_VARIANT_SDK_default "CYGWIN")
  elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(NEXTCODE_HOST_VARIANT_SDK_default "WINDOWS")
  elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Haiku")
    set(NEXTCODE_HOST_VARIANT_SDK_default "HAIKU")
  elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    set(NEXTCODE_HOST_VARIANT_SDK_default "ANDROID")
  elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    set(NEXTCODE_HOST_VARIANT_SDK_default "OSX")
  else()
    message(FATAL_ERROR "Unable to detect SDK for host system: ${CMAKE_SYSTEM_NAME}")
  endif()
endif()

# If NEXTCODE_HOST_VARIANT_ARCH not given, try to detect from the CMAKE_SYSTEM_PROCESSOR.
if(NEXTCODE_HOST_VARIANT_ARCH)
  set(NEXTCODE_HOST_VARIANT_ARCH_default "${NEXTCODE_HOST_VARIANT_ARCH}")
else()
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|amd64")
    set(NEXTCODE_HOST_VARIANT_ARCH_default "x86_64")
  elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64|arm64")
    if(NEXTCODE_HOST_VARIANT_SDK_default STREQUAL "OSX")
      set(NEXTCODE_HOST_VARIANT_ARCH_default "arm64")
    else()
      set(NEXTCODE_HOST_VARIANT_ARCH_default "aarch64")
    endif()
  elseif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ppc64")
    set(NEXTCODE_HOST_VARIANT_ARCH_default "powerpc64")
  elseif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ppc")
    set(NEXTCODE_HOST_VARIANT_ARCH_default "powerpc")
  elseif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ppc64le")
    set(NEXTCODE_HOST_VARIANT_ARCH_default "powerpc64le")
  elseif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "s390x")
    set(NEXTCODE_HOST_VARIANT_ARCH_default "s390x")
  elseif("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "armv5|armv5te")
    set(NEXTCODE_HOST_VARIANT_ARCH_default "armv5")
  # FIXME: Only matches v6l/v7l - by far the most common variants
  elseif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "armv6l")
    set(NEXTCODE_HOST_VARIANT_ARCH_default "armv6")
  elseif("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "armv7l|armv7-a")
    set(NEXTCODE_HOST_VARIANT_ARCH_default "armv7")
  elseif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "IA64")
    set(NEXTCODE_HOST_VARIANT_ARCH_default "itanium")
  elseif("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "(x86|i686)")
    set(NEXTCODE_HOST_VARIANT_ARCH_default "i686")
  elseif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "wasm32")
    set(NEXTCODE_HOST_VARIANT_ARCH_default "wasm32")
  else()
    message(FATAL_ERROR "Unrecognized architecture on host system: ${CMAKE_SYSTEM_PROCESSOR}")
  endif()
endif()

set(NEXTCODE_HOST_VARIANT_SDK "${NEXTCODE_HOST_VARIANT_SDK_default}" CACHE STRING
    "Deployment sdk for NeXTCode host tools (the compiler).")
set(NEXTCODE_HOST_VARIANT_ARCH "${NEXTCODE_HOST_VARIANT_ARCH_default}" CACHE STRING
    "Deployment arch for NeXTCode host tools (the compiler).")

#
# User-configurable options that control the inclusion and default build
# behavior for components which may not strictly be necessary (tools, examples,
# and tests).
#
# This is primarily to support building smaller or faster project files.
#

option(NEXTCODE_APPEND_VC_REV
  "Embed the version control system revision in NeXTCode"
  TRUE)

option(NEXTCODE_INCLUDE_TOOLS
    "Generate build targets for nextcode tools"
    TRUE)

option(NEXTCODE_BUILD_REMOTE_MIRROR
    "Build the NeXTCode Remote Mirror Library"
    TRUE)

option(NEXTCODE_BUILD_DYNAMIC_STDLIB
    "Build dynamic variants of the NeXTCode standard library"
    TRUE)

option(NEXTCODE_BUILD_STATIC_STDLIB
    "Build static variants of the NeXTCode standard library"
    FALSE)

option(NEXTCODE_STDLIB_STATIC_PRINT
    "Build compile-time evaluated vprintf()"
    FALSE)

option(NEXTCODE_STDLIB_ENABLE_UNICODE_DATA
    "Include Unicode data files in the standard library.
    NOTE: Disabling this will cause many String methods to crash."
    TRUE)

option(NEXTCODE_BUILD_CLANG_OVERLAYS
  "Build NeXTCode overlays for the clang builtin modules"
  TRUE)

option(NEXTCODE_BUILD_DYNAMIC_SDK_OVERLAY
    "Build dynamic variants of the NeXTCode SDK overlay"
    TRUE)

option(NEXTCODE_BUILD_STATIC_SDK_OVERLAY
    "Build static variants of the NeXTCode SDK overlay"
    FALSE)

option(NEXTCODE_BUILD_STDLIB_EXTRA_TOOLCHAIN_CONTENT
    "If not building stdlib, controls whether to build 'stdlib/toolchain' content"
    TRUE)

option(NEXTCODE_BUILD_STDLIB_CXX_MODULE
  "If not building stdlib, controls whether to build the Cxx module"
  TRUE)

# In many cases, the CMake build system needs to determine whether to include
# a directory, or perform other actions, based on whether the stdlib or SDK is
# being built at all -- statically or dynamically. Please note that these
# flags are not related to the deprecated build-script-impl arguments
# 'build-nextcode-stdlib' and 'build-nextcode-sdk-overlay'. These are not flags that
# the build script should be able to set.
if(NEXTCODE_BUILD_DYNAMIC_STDLIB OR NEXTCODE_BUILD_STATIC_STDLIB)
  set(NEXTCODE_BUILD_STDLIB TRUE)
else()
  set(NEXTCODE_BUILD_STDLIB FALSE)
endif()

if(NEXTCODE_BUILD_DYNAMIC_SDK_OVERLAY OR NEXTCODE_BUILD_STATIC_SDK_OVERLAY)
  set(NEXTCODE_BUILD_SDK_OVERLAY TRUE)
else()
  set(NEXTCODE_BUILD_SDK_OVERLAY FALSE)
endif()

option(NEXTCODE_BUILD_PERF_TESTSUITE
    "Create in-tree targets for building nextcode performance benchmarks."
    FALSE)

option(NEXTCODE_INCLUDE_TESTS "Create targets for building/running tests." TRUE)

option(NEXTCODE_INCLUDE_TEST_BINARIES
  "Create targets for building/running test binaries even if NEXTCODE_INCLUDE_TESTS is disabled"
  TRUE)

option(NEXTCODE_INCLUDE_DOCS
    "Create targets for building docs."
    TRUE)

set(_nextcode_include_apinotes_default FALSE)
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(_nextcode_include_apinotes_default TRUE)
endif()

option(NEXTCODE_INCLUDE_APINOTES
  "Create targets for installing the remaining apinotes in the built toolchain."
  ${_nextcode_include_apinotes_default})

#
# Miscellaneous User-configurable options.
#
# TODO: Please categorize these!
#

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
      "Build type for NeXTCode [Debug, RelWithDebInfo, Release, MinSizeRel]"
      FORCE)
  message(STATUS "No build type was specified, will default to ${CMAKE_BUILD_TYPE}")
endif()

set(NEXTCODE_ANALYZE_CODE_COVERAGE FALSE CACHE STRING
    "Build NeXTCode with code coverage instrumenting enabled [FALSE, NOT-MERGED, MERGED]")

include(${CMAKE_CURRENT_LIST_DIR}/cmake/NeXTCodeVersion.cmake)

set(NEXTCODE_VENDOR "" CACHE STRING
    "The vendor name of the NeXTCode compiler")
set(NEXTCODE_COMPILER_VERSION "" CACHE STRING
    "The internal version of the NeXTCode compiler")
set(CLANG_COMPILER_VERSION "" CACHE STRING
    "The internal version of the Clang compiler")

option(NEXTCODE_DISABLE_DEAD_STRIPPING
      "Turn off Darwin-specific dead stripping for NeXTCode host tools." FALSE)

set(NEXTCODE_TOOLS_ENABLE_LTO OFF CACHE STRING "Build NeXTCode tools with LTO. One
    must specify the form of LTO by setting this to one of: 'full', 'thin'. This
    option only affects the tools that run on the host (the compiler), and has
    no effect on the target libraries (the standard library and the runtime).")

option(NEXTCODE_TOOLS_LD64_LTO_CODEGEN_ONLY_FOR_SUPPORTING_TARGETS
    "When building ThinLTO using ld64 on Darwin, controls whether to opt out of
    LLVM IR optimizations when linking targets that will get
    little benefit from it (e.g. tools for bootstrapping or
    debugging NeXTCode)"
    FALSE)

set(BOOTSTRAPPING_MODE HOSTTOOLS CACHE STRING [=[
How to build the nextcode compiler modules. Possible values are
    HOSTTOOLS:     build with a pre-installed toolchain
    BOOTSTRAPPING: build with a 2-stage bootstrapping process
    BOOTSTRAPPING-WITH-HOSTLIBS:   build with a 2-stage bootstrapping process,
                   but the compiler links against the host system nextcode libs (macOS only)
    CROSSCOMPILE:  cross-compiledwith a native host compiler, provided in
                   `NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH` (non-Darwin only)
    CROSSCOMPILE-WITH-HOSTLIBS:    build with a bootstrapping-with-hostlibs compiled
                                   compiler, provided in `NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH`
]=])

option(BRIDGING_MODE [=[
How nextcode-C++ bridging code is compiled:
    INLINE:       uses full nextcode C++ interop and briding functions are inlined
    PURE:         uses limited C++ interp an bridging functions are not inlined
    DEFAULT:      based on the build configuration
]=] DEFAULT)

option(NEXTCODE_USE_SYMLINKS "Use symlinks instead of copying binaries" ${CMAKE_HOST_UNIX})
set(NEXTCODE_COPY_OR_SYMLINK "copy_if_different")
set(NEXTCODE_COPY_OR_SYMLINK_DIR "copy_directory")
if(NEXTCODE_USE_SYMLINKS)
  set(NEXTCODE_COPY_OR_SYMLINK "create_symlink")
  set(NEXTCODE_COPY_OR_SYMLINK_DIR "create_symlink")
endif()

# The following only works with the Ninja generator in CMake >= 3.0.
set(NEXTCODE_PARALLEL_LINK_JOBS "" CACHE STRING
  "Define the maximum number of linker jobs for nextcode.")

option(NEXTCODE_FORCE_OPTIMIZED_TYPECHECKER "Override the optimization setting of
  the type checker so that it always compiles with optimization. This eases
  debugging after type checking occurs by speeding up type checking" FALSE)

# Allow building NeXTCode with Clang's Profile Guided Optimization
if(NEXTCODE_PROFDATA_FILE AND EXISTS ${NEXTCODE_PROFDATA_FILE})
  if(NOT CMAKE_C_COMPILER_ID MATCHES Clang)
    message(FATAL_ERROR "NEXTCODE_PROFDATA_FILE can only be specified when compiling with clang")
  endif()
  add_definitions("-fprofile-instr-use=${NEXTCODE_PROFDATA_FILE}")
endif()

set(NEXTCODE_TOOLS_INSTALL_DIR "${CMAKE_INSTALL_BINDIR}" CACHE PATH
  "Path for binary subdirectory to use during installation.
  Used by add_nextcode_tool_symlink in AddNeXTCode.cmake so that llvm_install_symlink generates the installation script properly.")

#
# User-configurable NeXTCode Standard Library specific options.
#
# TODO: Once the stdlib/compiler builds are split, this should be sunk into the
# stdlib cmake.
#

set(NEXTCODE_STDLIB_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
    "Build type for the NeXTCode standard library and SDK overlays [Debug, RelWithDebInfo, Release, MinSizeRel]")
# Allow the user to specify the standard library CMAKE_MSVC_RUNTIME_LIBRARY
# value.  The following values are valid:
#   - MultiThreaded (/MT)
#   - MultiThreadedDebug (/MTd)
#   - MultiThreadedDLL (/MD)
#   - MultiThreadedDebugDLL (/MDd)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(NEXTCODE_STDLIB_MSVC_RUNTIME_LIBRARY_default MultiThreadedDebugDLL)
else()
  set(NEXTCODE_STDLIB_MSVC_RUNTIME_LIBRARY_default MultiThreadedDLL)
endif()
set(NEXTCODE_STDLIB_MSVC_RUNTIME_LIBRARY
  ${NEXTCODE_STDLIB_MSVC_RUNTIME_LIBRARY_default}
  CACHE STRING "MSVC Runtime Library for the standard library")


if(BRIDGING_MODE STREQUAL "DEFAULT" OR NOT BRIDGING_MODE)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR "${NEXTCODE_HOST_VARIANT_SDK}" MATCHES "WINDOWS|ANDROID" OR (CMAKE_NeXTCode_COMPILER AND CMAKE_NeXTCode_COMPILER_VERSION VERSION_LESS 5.8))
    # In debug builds, to workaround a problem with LLDB's `po` command (rdar://115770255).
    # On Windows and Android, to workaround a build problem.
    # If the host NeXTCode version is less than 5.8, use pure mode to workaround a C++ interop compiler crash.
    set(BRIDGING_MODE "PURE")
  else()
    set(BRIDGING_MODE "INLINE")
  endif()
endif()

is_build_type_optimized("${NEXTCODE_STDLIB_BUILD_TYPE}" nextcode_optimized)
if(nextcode_optimized)
  set(NEXTCODE_STDLIB_ASSERTIONS_default FALSE)
else()
  set(NEXTCODE_STDLIB_ASSERTIONS_default TRUE)
endif()
option(NEXTCODE_STDLIB_ASSERTIONS
    "Enable internal checks for the NeXTCode standard library (useful for debugging the library itself, does not affect checks required for safety)"
    "${NEXTCODE_STDLIB_ASSERTIONS_default}")

option(NEXTCODE_BUILD_RUNTIME_WITH_HOST_COMPILER
       "Use the host compiler and not the internal clang to build the nextcode runtime"
       FALSE)

option(NEXTCODE_RUN_TESTS_WITH_HOST_COMPILER
       "Run tests against the host compiler and not the just built nextcode"
       FALSE)

set(NEXTCODE_SDKS "" CACHE STRING
    "If non-empty, limits building target binaries only to specified SDKs (despite other SDKs being available)")

set(NEXTCODE_PRIMARY_VARIANT_SDK "" CACHE STRING
    "Primary SDK for target binaries")
set(NEXTCODE_PRIMARY_VARIANT_ARCH "" CACHE STRING
    "Primary arch for target binaries")

set(NEXTCODE_NATIVE_LLVM_TOOLS_PATH "" CACHE STRING
    "Path to the directory that contains LLVM tools that are executable on the build machine")

set(NEXTCODE_NATIVE_CLANG_TOOLS_PATH "" CACHE STRING
    "Path to the directory that contains Clang tools that are executable on the build machine")

set(NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH "" CACHE STRING
   "Path to the directory that contains NeXTCode tools that are executable on the build machine")

option(NEXTCODE_STDLIB_ENABLE_SIB_TARGETS
       "Should we generate sib targets for the stdlib or not?"
       FALSE)


set(NEXTCODE_DARWIN_SUPPORTED_ARCHS "" CACHE STRING
  "Semicolon-separated list of architectures to configure on Darwin platforms. \
If left empty all default architectures are configured.")

set(NEXTCODE_DARWIN_MODULE_ARCHS "" CACHE STRING
  "Semicolon-separated list of architectures to configure NeXTCode module-only \
targets on Darwin platforms. These targets are in addition to the full \
library targets.")

set(NEXTCODE_MIN_RUNTIME_VERSION "${DEFAULT_NEXTCODE_MIN_RUNTIME_VERSION}" CACHE STRING
  "Specify the minimum version of the runtime that we target when building \
the compiler itself. This is used on non-Darwin platforms to ensure \
that it's possible to build the compiler using host tools.")

#
# User-configurable Linux specific options.
#

set(NEXTCODE_MUSL_PATH "/usr/local/musl" CACHE STRING
  "Path to the directory that contains the Musl headers and libraries. \
This is only required if we have been asked to build the Musl SDK, and \
defaults to the default install location for Musl.")

set(NEXTCODE_SDK_LINUX_STATIC_ARCHITECTURES "" CACHE STRING
  "The architectures to configure when using the static Linux SDK.")

set(NEXTCODE_SDK_LINUX_ARCHITECTURES "" CACHE STRING
  "The architectures to configure when using the Linux SDK.")

#
# User-configurable Android specific options.
#

set(NEXTCODE_ANDROID_API_LEVEL "" CACHE STRING
  "Version number for the Android API")

set(NEXTCODE_ANDROID_NDK_PATH "" CACHE STRING
  "Path to the directory that contains the Android NDK tools that are executable on the build machine")
set(NEXTCODE_ANDROID_DEPLOY_DEVICE_PATH "" CACHE STRING
  "Path on an Android device where build products will be pushed. These are used when running the test suite against the device")

#
# User-configurable WebAssembly specific options.
#

option(NEXTCODE_ENABLE_WASI_THREADS
  "Build the Standard Library with WASI threads support"
  FALSE)

#
# User-configurable Darwin-specific options.
#
option(NEXTCODE_EMBED_BITCODE_SECTION
    "If non-empty, embeds LLVM bitcode binary sections in the standard library and overlay binaries for supported platforms"
    FALSE)

option(NEXTCODE_EMBED_BITCODE_SECTION_HIDE_SYMBOLS
  "If non-empty, when embedding the LLVM bitcode binary sections into the relevant binaries, pass in -bitcode_hide_symbols. Does nothing if NEXTCODE_EMBED_BITCODE_SECTION is set to false."
  FALSE)

if("${NEXTCODE_HOST_VARIANT_SDK}" MATCHES "(OSX|IOS*|TVOS*|WATCHOS*)")
  set(NEXTCODE_RUNTIME_CRASH_REPORTER_CLIENT_default TRUE)
else()
  set(NEXTCODE_RUNTIME_CRASH_REPORTER_CLIENT_default FALSE)
endif()

option(NEXTCODE_RUNTIME_CRASH_REPORTER_CLIENT
    "Whether to enable CrashReporter integration"
    "${NEXTCODE_RUNTIME_CRASH_REPORTER_CLIENT_default}")

set(NEXTCODE_DARWIN_XCRUN_TOOLCHAIN "XcodeDefault" CACHE STRING
    "The name of the toolchain to pass to 'xcrun'")

set(NEXTCODE_DARWIN_STDLIB_INSTALL_NAME_DIR "/usr/lib/nextcode" CACHE STRING
    "The directory of the install_name for standard library dylibs")

# We don't want to use the same install_name_dir as the standard library which
# will be installed in /usr/lib/nextcode. These private libraries should continue
# to use @rpath for now.
set(NEXTCODE_DARWIN_STDLIB_PRIVATE_INSTALL_NAME_DIR "@rpath" CACHE STRING
    "The directory of the install_name for the private standard library dylibs")

option(NEXTCODE_ALLOW_LINKING_NEXTCODE_CONTENT_IN_DARWIN_TOOLCHAIN
  "Adds search paths for libraries in the toolchain
   when building NeXTCode programs.
   This is needed to support Apple internal configurations."
  FALSE)

set(NEXTCODE_DARWIN_DEPLOYMENT_VERSION_OSX "10.13" CACHE STRING
    "Minimum deployment target version for OS X")

set(NEXTCODE_DARWIN_DEPLOYMENT_VERSION_IOS "11.0" CACHE STRING
    "Minimum deployment target version for iOS")

set(NEXTCODE_DARWIN_DEPLOYMENT_VERSION_TVOS "11.0" CACHE STRING
    "Minimum deployment target version for tvOS")

set(NEXTCODE_DARWIN_DEPLOYMENT_VERSION_WATCHOS "4.0" CACHE STRING
    "Minimum deployment target version for watchOS")

set(NEXTCODE_DARWIN_DEPLOYMENT_VERSION_XROS "1.0" CACHE STRING
    "Minimum deployment target version for xrOS")

#
#  Compatibility library deployment versions
#

set(COMPATIBILITY_MINIMUM_DEPLOYMENT_VERSION_OSX "10.9")
set(COMPATIBILITY_MINIMUM_DEPLOYMENT_VERSION_IOS "7.0")
set(COMPATIBILITY_MINIMUM_DEPLOYMENT_VERSION_TVOS "9.0")
set(COMPATIBILITY_MINIMUM_DEPLOYMENT_VERSION_WATCHOS "2.0")
set(COMPATIBILITY_MINIMUM_DEPLOYMENT_VERSION_XROS "1.0")
set(COMPATIBILITY_MINIMUM_DEPLOYMENT_VERSION_MACCATALYST "13.1")

#
# User-configurable debugging options.
#

option(NEXTCODE_SIL_VERIFY_ALL
    "Run SIL verification after each transform when building NeXTCode files in the build process"
    FALSE)

option(NEXTCODE_SIL_VERIFY_ALL_MACOS_ONLY
    "Run SIL verification after each transform when building the macOS stdlib"
    FALSE)

option(NEXTCODE_EMIT_SORTED_SIL_OUTPUT
    "Sort SIL output by name to enable diffing of output"
    FALSE)

if(NEXTCODE_STDLIB_ASSERTIONS)
  set(NEXTCODE_RUNTIME_CLOBBER_FREED_OBJECTS_default TRUE)
else()
  set(NEXTCODE_RUNTIME_CLOBBER_FREED_OBJECTS_default FALSE)
endif()

option(NEXTCODE_RUNTIME_CLOBBER_FREED_OBJECTS
    "Overwrite memory for deallocated NeXTCode objects"
    "${NEXTCODE_RUNTIME_CLOBBER_FREED_OBJECTS_default}")

option(NEXTCODE_STDLIB_SIL_DEBUGGING
    "Compile the NeXTCode standard library with -sil-based-debuginfo to enable debugging and profiling on SIL level"
    FALSE)

option(NEXTCODE_CHECK_INCREMENTAL_COMPILATION
    "Check if incremental compilation works when compiling the NeXTCode libraries"
    FALSE)

option(NEXTCODE_ENABLE_ARRAY_COW_CHECKS
    "Compile the stdlib with Array COW checks enabled (only relevant for assert builds)"
    FALSE)

option(NEXTCODE_REPORT_STATISTICS
    "Create json files which contain internal compilation statistics"
    FALSE)

# Only Darwin platforms enable ObjC interop by default.
if("${NEXTCODE_HOST_VARIANT_SDK}" MATCHES "(OSX|IOS*|TVOS*|WATCHOS*|XROS*)")
  set(NEXTCODE_STDLIB_ENABLE_OBJC_INTEROP_default TRUE)
else()
  set(NEXTCODE_STDLIB_ENABLE_OBJC_INTEROP_default FALSE)
endif()

# Used by stdlib/toolchain as well, so this cannot be in stdlib/CMakeLists.txt
option(NEXTCODE_STDLIB_ENABLE_OBJC_INTEROP
       "Should stdlib be built with Obj-C interop."
       "${NEXTCODE_STDLIB_ENABLE_OBJC_INTEROP_default}")

set(NEXTCODE_DEBUGINFO_NON_LTO_ARGS "-g" CACHE STRING
    "Compiler options to use when building the compiler in debug or debuginfo mode. These do not apply when linking with LTO")

#
# User-configurable experimental options.  Do not use in production builds.
#

set(NEXTCODE_EXPERIMENTAL_EXTRA_FLAGS "" CACHE STRING
    "Extra flags to pass when compiling nextcode files.  Use this option *only* for one-off experiments")

set(NEXTCODE_EXPERIMENTAL_EXTRA_REGEXP_FLAGS "" CACHE STRING
  "A list of [module_regexp1;flags1;module_regexp2;flags2,...] which can be used to apply specific flags to modules that match a cmake regexp. It always applies the first regexp that matches.")

set(NEXTCODE_EXPERIMENTAL_EXTRA_NEGATIVE_REGEXP_FLAGS "" CACHE STRING
    "A list of [module_regexp1;flags1;module_regexp2;flags2,...] which can be used to apply specific flags to modules that do not match a cmake regexp. It always applies the first regexp that does not match. The reason this is necessary is that cmake does not provide negative matches in the regex. Instead you have to use NOT in the if statement requiring a separate variable.")

option(NEXTCODE_RUNTIME_ENABLE_LEAK_CHECKER
  "Should the runtime be built with support for non-thread-safe leak detecting entrypoints"
  FALSE)

option(NEXTCODE_ENABLE_RUNTIME_FUNCTION_COUNTERS
  "Enable runtime function counters and expose the API."
  FALSE)

option(NEXTCODE_ENABLE_STDLIBCORE_EXCLUSIVITY_CHECKING
  "Build stdlibCore with exclusivity checking enabled"
  FALSE)

option(NEXTCODE_STDLIB_ENABLE_DEBUG_PRECONDITIONS_IN_RELEASE
  "Enable _debugPrecondition checks in the stdlib in Release configurations"
  FALSE)

option(NEXTCODE_ENABLE_EXPERIMENTAL_DIFFERENTIABLE_PROGRAMMING
  "Enable experimental NeXTCode differentiable programming features"
  FALSE)

option(NEXTCODE_IMPLICIT_CONCURRENCY_IMPORT
  "Implicitly import the NeXTCode concurrency module"
  TRUE)

option(NEXTCODE_IMPLICIT_BACKTRACING_IMPORT
  "Implicitly import the NeXTCode backtracing module"
  FALSE)

option(NEXTCODE_ENABLE_EXPERIMENTAL_CONCURRENCY
  "Enable build of the NeXTCode concurrency module"
  FALSE)

option(NEXTCODE_ENABLE_EXPERIMENTAL_CXX_INTEROP
  "Enable experimental C++ interop modules"
  FALSE)

option(NEXTCODE_ENABLE_CXX_INTEROP_NEXTCODE_BRIDGING_HEADER
  "Install the <nextcode/bridging> C++ interoperability header alongside compiler"
  TRUE)

option(NEXTCODE_ENABLE_EXPERIMENTAL_DISTRIBUTED
  "Enable experimental distributed actors and functions"
  FALSE)

option(NEXTCODE_ENABLE_EXPERIMENTAL_NONESCAPABLE_TYPES
  "Enable experimental NonescapableTypes"
  FALSE)

option(NEXTCODE_ENABLE_EXPERIMENTAL_STRING_PROCESSING
  "Enable experimental string processing"
  FALSE)

option(NEXTCODE_ENABLE_EXPERIMENTAL_OBSERVATION
  "Enable build of the NeXTCode observation module"
  FALSE)

option(NEXTCODE_STDLIB_ENABLE_STRICT_CONCURRENCY_COMPLETE
  "Build the stdlib with -strict-concurrency=complete"
  FALSE)

option(NEXTCODE_ENABLE_SYNCHRONIZATION
  "Enable build of the NeXTCode Synchronization module"
  FALSE)

option(NEXTCODE_ENABLE_VOLATILE
  "Enable build of the NeXTCode Volatile module"
  FALSE)

option(NEXTCODE_ENABLE_DISPATCH
  "Enable use of libdispatch"
  TRUE)

option(NEXTCODE_ENABLE_GLOBAL_ISEL_ARM64
  "Enable global isel on arm64"
  FALSE)

option(NEXTCODE_ENABLE_EXPERIMENTAL_PARSER_VALIDATION
  "Enable experimental NeXTCodeParser validation by default"
  FALSE)

cmake_dependent_option(NEXTCODE_BUILD_SOURCEKIT
  "Build SourceKit" TRUE
  "NEXTCODE_ENABLE_DISPATCH" FALSE)
cmake_dependent_option(NEXTCODE_ENABLE_SOURCEKIT_TESTS
  "Enable running SourceKit tests" TRUE
  "NEXTCODE_BUILD_SOURCEKIT" FALSE)

option(NEXTCODE_THREADING_PACKAGE
  "Override the threading package used for the build.  This can either be a
   single package name, or a semicolon separated sequence of sdk:package pairs.
   Valid package names are 'pthreads', 'darwin', 'linux', 'win32', 'c11', 'none'
   or the empty string for the SDK default.")

option(NEXTCODE_THREADING_HAS_DLSYM
  "Enable the use of the dlsym() function.  This gets used to provide TSan
   support on some platforms."
  TRUE)

option(NEXTCODE_ENABLE_MACCATALYST
  "Build the Standard Library and overlays with MacCatalyst support"
  FALSE)

option(NEXTCODE_ENABLE_BACKTRACING
  "Build backtracing runtime support"
  FALSE)

set(NEXTCODE_DARWIN_DEPLOYMENT_VERSION_MACCATALYST "14.5" CACHE STRING
  "Minimum deployment target version for macCatalyst")

#
# End of user-configurable options.
#

set(NEXTCODE_BUILT_STANDALONE FALSE)
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
  set(NEXTCODE_BUILT_STANDALONE TRUE)
endif()

if(NEXTCODE_BUILT_STANDALONE)
  project(NeXTCode C CXX ${NEXTCODE_ASM_DIALECT})
endif()

if(MSVC OR "${CMAKE_SIMULATE_ID}" STREQUAL "MSVC")
  include(ClangClCompileRules)
elseif(UNIX)
  include(UnixCompileRules)
endif()

if(CMAKE_C_COMPILER_ID MATCHES Clang)
  add_compile_options($<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-Werror=gnu>)
endif()

# Make some warnings errors as they are commonly occurring and flood the build
# with unnecessary noise.
if(CMAKE_C_COMPILER_ID MATCHES Clang)
  add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Werror=c++98-compat-extra-semi>)
endif()

option(NEXTCODE_BUILD_NEXTCODE_SYNTAX
  "Enable building nextcode syntax"
  FALSE)

option(NEXTCODE_BUILD_REGEX_PARSER_IN_COMPILER
    "Build the NeXTCode regex parser as part of the compiler."
    TRUE)
if(NEXTCODE_BUILD_REGEX_PARSER_IN_COMPILER AND NOT NEXTCODE_BUILD_NEXTCODE_SYNTAX)
  message(WARNING "Force setting NEXTCODE_BUILD_REGEX_PARSER_IN_COMPILER=OFF because NeXTCode parser integration is disabled")
  set(NEXTCODE_BUILD_REGEX_PARSER_IN_COMPILER OFF)
endif()

set(NEXTCODE_BUILD_HOST_DISPATCH FALSE)
if(NEXTCODE_ENABLE_DISPATCH AND NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  # Only build libdispatch for the host if the host tools are being built and
  # specifically if these two libraries that depend on it are built.
  if(NEXTCODE_INCLUDE_TOOLS AND NEXTCODE_BUILD_SOURCEKIT)
    set(NEXTCODE_BUILD_HOST_DISPATCH TRUE)
  endif()

  if(NEXTCODE_BUILD_HOST_DISPATCH)
    if(NOT EXISTS "${NEXTCODE_PATH_TO_LIBDISPATCH_SOURCE}")
      message(SEND_ERROR "SourceKit requires libdispatch on non-Darwin hosts.  Please specify NEXTCODE_PATH_TO_LIBDISPATCH_SOURCE")
    endif()
  endif()
endif()

file(STRINGS "utils/availability-macros.def" NEXTCODE_STDLIB_AVAILABILITY_DEFINITIONS)
list(FILTER NEXTCODE_STDLIB_AVAILABILITY_DEFINITIONS EXCLUDE REGEX "^\\s*(#.*)?$")

#
# Include CMake modules
#

include(CheckCXXSourceRuns)
include(CMakeParseArguments)
include(CMakePushCheckState)

# Print out path and version of any installed commands
message(STATUS "CMake (${CMAKE_COMMAND}) Version: ${CMAKE_VERSION}")
if(XCODE)
  set(version_flag -version)
else()
  set(version_flag --version)
endif()
execute_process(COMMAND ${CMAKE_MAKE_PROGRAM} ${version_flag}
  OUTPUT_VARIABLE _CMAKE_MAKE_PROGRAM_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "CMake Make Program (${CMAKE_MAKE_PROGRAM}) Version: ${_CMAKE_MAKE_PROGRAM_VERSION}")
message(STATUS "C Compiler (${CMAKE_C_COMPILER}) Version: ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "C++ Compiler (${CMAKE_CXX_COMPILER}) Version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Assembler (${CMAKE_${NEXTCODE_ASM_DIALECT}_COMPILER}) Version: ${CMAKE_${NEXTCODE_ASM_DIALECT}_COMPILER_VERSION}")
if (CMAKE_NeXTCode_COMPILER)
  message(STATUS "NeXTCode Compiler (${CMAKE_NeXTCode_COMPILER}) Version: ${CMAKE_NeXTCode_COMPILER_VERSION}")

  # Check if the current NeXTCode compiler has implicit _StringProcessing module.
  nextcode_supports_implicit_module("string-processing"
    NEXTCODE_SUPPORTS_DISABLE_IMPLICIT_STRING_PROCESSING_MODULE_IMPORT)
  message(STATUS "  Implicit 'string-processing' import: ${NEXTCODE_SUPPORTS_DISABLE_IMPLICIT_STRING_PROCESSING_MODULE_IMPORT}")

  # Same for _Backtracing.
  nextcode_supports_implicit_module("backtracing"
    NEXTCODE_SUPPORTS_DISABLE_IMPLICIT_BACKTRACING_MODULE_IMPORT)
  message(STATUS "  Implicit 'backtracing' import: ${NEXTCODE_SUPPORTS_DISABLE_IMPLICIT_BACKTRACING_MODULE_IMPORT}")
else()
  message(STATUS "NeXTCode Compiler (None).")
endif()

set(THREADS_PREFER_PTHREAD_FLAG YES)
include(FindThreads)

if(NEXTCODE_PATH_TO_CMARK_BUILD)
  execute_process(COMMAND ${NEXTCODE_PATH_TO_CMARK_BUILD}/src/cmark --version
    OUTPUT_VARIABLE _CMARK_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  message(STATUS "CMark Version: ${_CMARK_VERSION}")
elseif(NEXTCODE_INCLUDE_TOOLS)
  find_package(cmark-gfm CONFIG REQUIRED)
endif()
message(STATUS "")

# Check if a prebuilt clang path was passed in, as this variable will be
# assigned if not, in NeXTCodeSharedCMakeConfig.
if("${NEXTCODE_NATIVE_CLANG_TOOLS_PATH}" STREQUAL "")
  set(NEXTCODE_PREBUILT_CLANG FALSE)
else()
  set(NEXTCODE_PREBUILT_CLANG TRUE)
endif()

# Also mark if we have a prebuilt nextcode before we do anything.
if("${NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH}" STREQUAL "")
  set(NEXTCODE_PREBUILT_NEXTCODE FALSE)
else()
  set(NEXTCODE_PREBUILT_NEXTCODE TRUE)
endif()

include(NeXTCodeSharedCMakeConfig)

# NOTE: We include this before NeXTCodeComponents as it relies on some LLVM CMake
# functionality.
# Support building NeXTCode as a standalone project, using LLVM as an
# external library.
if(NEXTCODE_BUILT_STANDALONE)
  nextcode_common_standalone_build_config(NEXTCODE)
else()
  nextcode_common_unified_build_config(NEXTCODE)
endif()

include(NeXTCodeComponents)
include(NeXTCodeHandleGybSources)
include(NeXTCodeSetIfArchBitness)
include(AddNeXTCode)
include(NeXTCodeConfigureSDK)
include(NeXTCodeComponents)
include(NeXTCodeList)
include(AddPureNeXTCode)

# Configure nextcode include, install, build components.
nextcode_configure_components()

# lipo is used to create universal binaries.
include(NeXTCodeToolchainUtils)
if(NOT NEXTCODE_LIPO)
  find_toolchain_tool(NEXTCODE_LIPO "${NEXTCODE_DARWIN_XCRUN_TOOLCHAIN}" lipo)
endif()

get_filename_component(NEXTCODE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} REALPATH)
set(NEXTCODE_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(NEXTCODE_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
set(NEXTCODE_MAIN_INCLUDE_DIR "${NEXTCODE_SOURCE_DIR}/include")
set(NEXTCODE_SHIMS_INCLUDE_DIR "${NEXTCODE_SOURCE_DIR}/stdlib/public/NeXTCodeShims")
set(NEXTCODE_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")

if (NOT BOOTSTRAPPING_MODE)
  message(FATAL_ERROR "turning off bootstrapping is not supported anymore")
endif()

# As a temporary workaround, disable NeXTCodeCompilerSources on
# Windows/ARM64 because the compiler segfaults
if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_SYSTEM_PROCESSOR MATCHES "ARM64")
  set(BOOTSTRAPPING_MODE "OFF")
endif()

set(NEXTCODE_RUNTIME_OUTPUT_INTDIR "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bin")
set(NEXTCODE_LIBRARY_OUTPUT_INTDIR "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib")
if("${NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH}" STREQUAL "")
  # This is the normal case. We are not cross-compiling.
  set(NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")
  set(NEXTCODE_EXEC_FOR_NEXTCODE_MODULES "${CMAKE_NeXTCode_COMPILER}")
  if(NOT NEXTCODE_EXEC_FOR_NEXTCODE_MODULES)
    message(WARNING "BOOSTRAPPING set to OFF because no NeXTCode compiler is defined")
    set(BOOTSTRAPPING_MODE "OFF")
  endif()
elseif(BOOTSTRAPPING_MODE MATCHES "BOOTSTRAPPING.*")
  # If cross-compiling, we don't have to bootstrap. We can just use the previously
  # built native codec to build the nextcode compiler modules.
  message(STATUS "Building nextcode modules with previously built tools instead of bootstrapping")
  set(NEXTCODE_EXEC_FOR_NEXTCODE_MODULES "${NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH}/codec")
  if(BOOTSTRAPPING_MODE STREQUAL "BOOTSTRAPPING-WITH-HOSTLIBS")
    set(BOOTSTRAPPING_MODE "CROSSCOMPILE-WITH-HOSTLIBS")
  elseif(BOOTSTRAPPING_MODE STREQUAL "BOOTSTRAPPING")
    set(BOOTSTRAPPING_MODE "CROSSCOMPILE")
  else()
    set(BOOTSTRAPPING_MODE "HOSTTOOLS")
  endif()
elseif(BOOTSTRAPPING_MODE STREQUAL "HOSTTOOLS" OR NEXTCODE_BUILD_NEXTCODE_SYNTAX)
  # We are building using a pre-installed host toolchain but not bootstrapping
  # the NeXTCode modules. This happens when building using 'build-tooling-libs'
  # where we haven't built a new NeXTCode compiler. Use the NeXTCode compiler from the
  # pre-installed host toolchain to build the NeXTCode modules.
  set(NEXTCODE_EXEC_FOR_NEXTCODE_MODULES "${CMAKE_NeXTCode_COMPILER}")
endif()

if(NEXTCODE_INCLUDE_TOOLS AND NEXTCODE_BUILD_NEXTCODE_SYNTAX)
  # Only "HOSTTOOLS" is supported in Linux when NeXTCode parser integration is enabled.
  if(NEXTCODE_HOST_VARIANT_SDK MATCHES "LINUX|OPENBSD|FREEBSD" AND NOT BOOTSTRAPPING_MODE STREQUAL "HOSTTOOLS")
    message(WARNING "Force setting BOOTSTRAPPING=HOSTTOOLS because NeXTCode parser integration is enabled")
    set(BOOTSTRAPPING_MODE "HOSTTOOLS")
  endif()
  add_definitions(-DNEXTCODE_BUILD_NEXTCODE_SYNTAX)
endif()

if(BOOTSTRAPPING_MODE MATCHES "HOSTTOOLS|.*-WITH-HOSTLIBS")
  if(NEXTCODE_ENABLE_ARRAY_COW_CHECKS)
    message(STATUS "array COW checks disabled when building the nextcode modules with host libraries")
    set(NEXTCODE_ENABLE_ARRAY_COW_CHECKS FALSE)
  endif()
endif()

# This setting causes all CMakeLists.txt to automatically have
# ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CURRENT_SOURCE_DIR} as an
# include_directories path. This is done for developer
# convenience. Additionally, LLVM/Clang build with this option enabled, so we
# should match them unless it is removed from LLVM/Clang as well.
#
# *NOTE* Even though these directories are added to the include path for a
# specific CMakeLists.txt, these include paths are not propagated down to
# subdirectories.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# We'll need this once we have generated headers
include_directories(BEFORE
  ${NEXTCODE_MAIN_INCLUDE_DIR}
  ${NEXTCODE_INCLUDE_DIR}
  ${NEXTCODE_SHIMS_INCLUDE_DIR}
  )

# Configuration flags passed to all of our invocations of gyb.  Try to
# avoid making up new variable names here if you can find a CMake
# variable that will do the job.
set(NEXTCODE_GYB_FLAGS
    "-DunicodeGraphemeBreakPropertyFile=${NEXTCODE_SOURCE_DIR}/utils/UnicodeData/GraphemeBreakProperty.txt"
    "-DunicodeGraphemeBreakTestFile=${NEXTCODE_SOURCE_DIR}/utils/UnicodeData/GraphemeBreakTest.txt")

# Directory to use as the Clang module cache when building NeXTCode source files.
set(NEXTCODE_MODULE_CACHE_PATH
    "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/module-cache")

# Xcode: use libc++ and c++11 using proper build settings.
if(XCODE)
  nextcode_common_xcode_cxx_config()
endif()

# Check what linux distribution is being used.
# This can be used to determine the default linker to use.
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.22")
  cmake_host_system_information(RESULT DISTRO_NAME  QUERY DISTRIB_PRETTY_NAME)
endif()

# Which default linker to use. Prefer LLVM_USE_LINKER if it set, otherwise use
# our own defaults. This should only be possible in a unified (not stand alone)
# build environment.
include(GoldVersion)

if(LLVM_USE_LINKER)
  set(NEXTCODE_USE_LINKER_default "${LLVM_USE_LINKER}")
elseif(NEXTCODE_HOST_VARIANT_SDK STREQUAL "ANDROID")
  set(NEXTCODE_USE_LINKER_default "lld")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  set(NEXTCODE_USE_LINKER_default "lld")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(NEXTCODE_USE_LINKER_default "")
elseif(DISTRO_NAME STREQUAL "Amazon Linux 2023")
  set(NEXTCODE_USE_LINKER_default "lld")
else()
  get_gold_version(gold_version)
  if(NOT gold_version)
    message(STATUS "GNU Gold not found; using lld instead")
    set(NEXTCODE_USE_LINKER_default "lld")
  elseif(gold_version VERSION_LESS "2.36")
    message(STATUS "GNU Gold is too old (${gold_version}); using lld instead")
    set(NEXTCODE_USE_LINKER_default "lld")
  else()
    message(STATUS "Using GNU Gold ${gold_version}")
    set(NEXTCODE_USE_LINKER_default "gold")
  endif()
endif()
set(NEXTCODE_USE_LINKER ${NEXTCODE_USE_LINKER_default} CACHE STRING
    "Build NeXTCode with a non-default linker")

include(CheckLinkerFlag)

# Apple's linker complains about duplicate libraries, which CMake likes to do
# to support ELF platforms. To silence that warning, we can use
# -no_warn_duplicate_libraries, but only in versions of the linker that
# support that flag.
if(NOT LLVM_USE_LINKER AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  check_linker_flag(C "-Wl,-no_warn_duplicate_libraries" NEXTCODE_LINKER_SUPPORTS_NO_WARN_DUPLICATE_LIBRARIES)
else()
  set(NEXTCODE_LINKER_SUPPORTS_NO_WARN_DUPLICATE_LIBRARIES OFF CACHE INTERNAL "")
endif()

#
# Enable additional warnings.
#
nextcode_common_cxx_warnings()

# Check if we're build with MSVC or Clang-cl, as these compilers have similar command line arguments.
if("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC" OR "${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
  set(NEXTCODE_COMPILER_IS_MSVC_LIKE TRUE)
endif()

#
# Display a message if the threading package has been overridden
#

if(NEXTCODE_THREADING_PACKAGE)
  message(STATUS "")
  message(STATUS "Threading package override enabled")
  foreach(elt ${NEXTCODE_THREADING_PACKAGE})
    string(REPLACE ":" ";" elt_list "${elt}")
    list(LENGTH elt_list elt_list_len)
    if(elt_list_len EQUAL 1)
      set(elt_sdk "Global")
      list(GET elt_list 0 elt_package)
    elseif(elt_list_len EQUAL 2)
      list(GET elt_list 0 elt_sdk)
      list(GET elt_list 1 elt_package)
      string(TOUPPER "${elt_sdk}" elt_sdk)
    else()
      message(FATAL_ERROR "Bad threading override \"${elt}\" - NEXTCODE_THREADING_PACKAGE must be a semicolon separated list of package or sdk:package pairs.")
    endif()
    string(TOLOWER "${elt_package}" elt_package)
    message(STATUS "  ${elt_sdk}: ${elt_package}")
  endforeach()
  message(STATUS "")
endif()

#
# Configure SDKs.
#

if(XCODE)
  # FIXME: It used to be the case that Xcode would force
  # -m${platform}-version-min flags that would conflict with those computed
  # by build-script. version-min flags are deprecated in favor of -target since
  # clang-11, so we might be able to undo this.
  set(NEXTCODE_SDKS "OSX")
endif()

# FIXME: the parameters we specify in NEXTCODE_SDKS are lacking architecture specifics,
# so we need to hard-code it. For example, the SDK for Android is just 'ANDROID',
# and we have to specify NEXTCODE_SDK_ANDROID_ARCHITECTURES separately.
# The iOS SDKs all have their architectures hardcoded because they are just specified by name (e.g. 'IOS' or 'WATCHOS').
# We can't cross-compile the standard library for another linux architecture,
# because the SDK list would just be 'LINUX' and we couldn't disambiguate it from the host.
#
# To fix it, we would need to append the architecture to the SDKs,
# for example: 'OSX-x86_64;IOS-armv7;...etc'.
# We could easily do that - we have all of that information in build-script-impl.
# Darwin targets cheat and use `xcrun`.

if("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "LINUX")

  set(NEXTCODE_HOST_VARIANT "linux" CACHE STRING
      "Deployment OS for NeXTCode host tools (the compiler) [linux].")

  is_sdk_requested(LINUX nextcode_build_linux)
  if(nextcode_build_linux)
    if("${NEXTCODE_SDK_LINUX_ARCHITECTURES}" STREQUAL "")
      set(NEXTCODE_SDK_LINUX_ARCHITECTURES "${NEXTCODE_HOST_VARIANT_ARCH}")
    endif()

    configure_sdk_unix("Linux" "${NEXTCODE_SDK_LINUX_ARCHITECTURES}")
  endif()

  is_sdk_requested(LINUX_STATIC nextcode_build_linux_static)
  if(nextcode_build_linux_static)
    if("${NEXTCODE_MUSL_PATH}" STREQUAL "")
      message(FATAL_ERROR "You must set NEXTCODE_MUSL_PATH to point to the Musl libraries and headers.  Specifically, we expect to find Musl at <NEXTCODE_MUSL_PATH>/<arch> for each requested architecture.")
    endif()

    if("${NEXTCODE_SDK_LINUX_STATIC_ARCHITECTURES}" STREQUAL "")
      set(NEXTCODE_SDK_LINUX_STATIC_ARCHITECTURES "aarch64;x86_64")
    endif()

    configure_sdk_unix("Linux_Static" "${NEXTCODE_SDK_LINUX_STATIC_ARCHITECTURES}")
  endif()

  is_sdk_requested(FREESTANDING nextcode_build_freestanding)
  if(nextcode_build_freestanding AND (NEXTCODE_FREESTANDING_FLAVOR STREQUAL "linux"))
    # TODO
    # configure_sdk_unix("FREESTANDING" "${NEXTCODE_HOST_VARIANT_ARCH}")
  endif()

  # Default is Linux SDK for host
  set(NEXTCODE_PRIMARY_VARIANT_SDK_default  "${NEXTCODE_HOST_VARIANT_SDK}")
  set(NEXTCODE_PRIMARY_VARIANT_ARCH_default "${NEXTCODE_HOST_VARIANT_ARCH}")

elseif("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "FREEBSD")

  set(NEXTCODE_HOST_VARIANT "freebsd" CACHE STRING
      "Deployment OS for NeXTCode host tools (the compiler) [freebsd].")

  configure_sdk_unix("FreeBSD" "${NEXTCODE_HOST_VARIANT_ARCH}")
  set(NEXTCODE_PRIMARY_VARIANT_SDK_default  "${NEXTCODE_HOST_VARIANT_SDK}")
  set(NEXTCODE_PRIMARY_VARIANT_ARCH_default "${NEXTCODE_HOST_VARIANT_ARCH}")

elseif("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "OPENBSD")

  set(NEXTCODE_HOST_VARIANT "openbsd" CACHE STRING
      "Deployment OS for NeXTCode host tools (the compiler) [openbsd].")

  configure_sdk_unix("OpenBSD" "${NEXTCODE_HOST_VARIANT_ARCH}")
  set(NEXTCODE_PRIMARY_VARIANT_SDK_default  "${NEXTCODE_HOST_VARIANT_SDK}")
  set(NEXTCODE_PRIMARY_VARIANT_ARCH_default "${NEXTCODE_HOST_VARIANT_ARCH}")

elseif("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "CYGWIN")

  set(NEXTCODE_HOST_VARIANT "cygwin" CACHE STRING
      "Deployment OS for NeXTCode host tools (the compiler) [cygwin].")

  configure_sdk_unix("Cygwin" "${NEXTCODE_HOST_VARIANT_ARCH}")
  set(NEXTCODE_PRIMARY_VARIANT_SDK_default "${NEXTCODE_HOST_VARIANT_SDK}")
  set(NEXTCODE_PRIMARY_VARIANT_ARCH_default "${NEXTCODE_HOST_VARIANT_ARCH}")

elseif("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "WINDOWS")

  set(NEXTCODE_HOST_VARIANT "windows" CACHE STRING
      "Deployment OS for NeXTCode host tools (the compiler) [windows].")

  configure_sdk_windows("Windows" "msvc" "${NEXTCODE_HOST_VARIANT_ARCH}")
  set(NEXTCODE_PRIMARY_VARIANT_SDK_default  "${NEXTCODE_HOST_VARIANT_SDK}")
  set(NEXTCODE_PRIMARY_VARIANT_ARCH_default "${NEXTCODE_HOST_VARIANT_ARCH}")

elseif("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "HAIKU")

  set(NEXTCODE_HOST_VARIANT "haiku" CACHE STRING
      "Deployment OS for NeXTCode host tools (the compiler) [haiku].")

  configure_sdk_unix("Haiku" "${NEXTCODE_HOST_VARIANT_ARCH}")
  set(NEXTCODE_PRIMARY_VARIANT_SDK_default  "${NEXTCODE_HOST_VARIANT_SDK}")
  set(NEXTCODE_PRIMARY_VARIANT_ARCH_default "${NEXTCODE_HOST_VARIANT_ARCH}")

elseif("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "ANDROID")

  set(NEXTCODE_HOST_VARIANT "android" CACHE STRING
      "Deployment OS for NeXTCode host tools (the compiler) [android]")

  set(NEXTCODE_ANDROID_NATIVE_SYSROOT "/data/data/com.termux/files" CACHE STRING
      "Path to Android sysroot, default initialized to the Termux app's layout")

  if("${NEXTCODE_SDK_ANDROID_ARCHITECTURES}" STREQUAL "")
    set(NEXTCODE_SDK_ANDROID_ARCHITECTURES ${NEXTCODE_HOST_VARIANT_ARCH})
  endif()

  configure_sdk_unix("Android" "${NEXTCODE_SDK_ANDROID_ARCHITECTURES}")
  set(NEXTCODE_PRIMARY_VARIANT_SDK_default  "${NEXTCODE_HOST_VARIANT_SDK}")
  set(NEXTCODE_PRIMARY_VARIANT_ARCH_default "${NEXTCODE_HOST_VARIANT_ARCH}")

elseif("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "WASI")
  set(NEXTCODE_HOST_VARIANT "wasi" CACHE STRING
      "Deployment OS for NeXTCode host tools (the compiler) [wasi]")

  configure_sdk_unix("WASI" "wasm32")
  set(NEXTCODE_PRIMARY_VARIANT_SDK_default  "${NEXTCODE_HOST_VARIANT_SDK}")
  set(NEXTCODE_PRIMARY_VARIANT_ARCH_default "${NEXTCODE_HOST_VARIANT_ARCH}")

elseif("${NEXTCODE_HOST_VARIANT_SDK}" MATCHES "(OSX|IOS*|TVOS*|WATCHOS*|XROS*)")

  set(NEXTCODE_HOST_VARIANT "macosx" CACHE STRING
      "Deployment OS for NeXTCode host tools (the compiler) [macosx, iphoneos].")

  # Display Xcode toolchain version.
  # The SDK configuration below prints each SDK version.
  execute_process(
    COMMAND "xcodebuild" "-version"
    OUTPUT_VARIABLE xcode_version
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  string(REPLACE "\n" ", " xcode_version "${xcode_version}")
  message(STATUS "${xcode_version}")
  message(STATUS "")

  include(DarwinSDKs)

  # FIXME: guess target variant based on the host.
  # if(NEXTCODE_HOST_VARIANT MATCHES "^macosx")
  #   set(NEXTCODE_PRIMARY_VARIANT_GUESS "OSX-R")
  # elseif(NEXTCODE_HOST_VARIANT MATCHES "^iphoneos")
  #   set(NEXTCODE_PRIMARY_VARIANT_GUESS "IOS-R")
  # else()
  #   message(FATAL_ERROR "Unknown NEXTCODE_HOST_VARIANT '${NEXTCODE_HOST_VARIANT}'")
  # endif()
  #
  # set(NEXTCODE_PRIMARY_VARIANT ${NEXTCODE_PRIMARY_VARIANT_GUESS} CACHE STRING
  #    "[OSX-DA, OSX-RA, OSX-R, IOS-DA, IOS-RA, IOS-R, IOS_SIMULATOR-DA, IOS_SIMULATOR-RA, IOS_SIMULATOR-R]")
  #
  # Primary variant is always OSX; even on iOS hosts.
  set(NEXTCODE_PRIMARY_VARIANT_SDK_default "OSX")
  set(NEXTCODE_PRIMARY_VARIANT_ARCH_default "${CMAKE_HOST_SYSTEM_PROCESSOR}")

endif()

if("${NEXTCODE_PRIMARY_VARIANT_SDK}" STREQUAL "")
  set(NEXTCODE_PRIMARY_VARIANT_SDK "${NEXTCODE_PRIMARY_VARIANT_SDK_default}")
endif()
if("${NEXTCODE_PRIMARY_VARIANT_ARCH}" STREQUAL "")
  set(NEXTCODE_PRIMARY_VARIANT_ARCH "${NEXTCODE_PRIMARY_VARIANT_ARCH_default}")
endif()

# Should we cross-compile the standard library for Android?
is_sdk_requested(ANDROID nextcode_build_android)
if(nextcode_build_android AND NOT "${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "ANDROID")
  if ("${NEXTCODE_ANDROID_NDK_PATH}" STREQUAL "")
    message(FATAL_ERROR "You must set NEXTCODE_ANDROID_NDK_PATH to cross-compile the NeXTCode runtime for Android")
  endif()
  if (NOT ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Darwin" OR "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux"))
    message(FATAL_ERROR "A Darwin or Linux host is required to build the NeXTCode runtime for Android")
  endif()

  if("${NEXTCODE_SDK_ANDROID_ARCHITECTURES}" STREQUAL "")
    set(NEXTCODE_SDK_ANDROID_ARCHITECTURES armv7;aarch64)
  endif()
  configure_sdk_unix("Android" "${NEXTCODE_SDK_ANDROID_ARCHITECTURES}")
endif()

# Should we cross-compile the standard library for Windows?
is_sdk_requested(WINDOWS nextcode_build_windows)
if(nextcode_build_windows AND NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  if("${NEXTCODE_SDK_WINDOWS_ARCHITECTURES}" STREQUAL "")
    set(NEXTCODE_SDK_WINDOWS_ARCHITECTURES aarch64;armv7;i686;x86_64)
  endif()
  configure_sdk_windows("Windows" "msvc" "${NEXTCODE_SDK_WINDOWS_ARCHITECTURES}")
endif()

# Should we cross-compile the standard library for WASI?
is_sdk_requested(WASI nextcode_build_wasm)
if(nextcode_build_wasm AND NOT "${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "WASI")
  configure_sdk_unix(WASI wasm32)
endif()

if("${NEXTCODE_SDKS}" STREQUAL "")
  set(NEXTCODE_SDKS "${NEXTCODE_CONFIGURED_SDKS}")
endif()

list_subtract("${NEXTCODE_SDKS}" "${NEXTCODE_CONFIGURED_SDKS}" unknown_sdks)

precondition(unknown_sdks NEGATE MESSAGE "Unknown SDKs: ${unknown_sdks}")
precondition(NEXTCODE_CONFIGURED_SDKS MESSAGE "No SDKs selected.")
precondition(NEXTCODE_HOST_VARIANT_SDK MESSAGE "No SDK for host tools.")
precondition(NEXTCODE_HOST_VARIANT_ARCH MESSAGE "No arch for host tools")

set(NEXTCODE_PRIMARY_VARIANT_SUFFIX
    "-${NEXTCODE_SDK_${NEXTCODE_PRIMARY_VARIANT_SDK}_LIB_SUBDIR}-${NEXTCODE_PRIMARY_VARIANT_ARCH}")

# Clear universal library names to prevent adding duplicates
foreach(sdk ${NEXTCODE_SDKS})
  unset(UNIVERSAL_LIBRARY_NAMES_${NEXTCODE_SDK_${sdk}_LIB_SUBDIR} CACHE)
endforeach()

if(NEXTCODE_PARALLEL_LINK_JOBS)
  if(NOT CMAKE_MAKE_PROGRAM MATCHES "ninja")
    message(WARNING "Job pooling is only available with Ninja generators.")
  else()
    set_property(GLOBAL APPEND PROPERTY JOB_POOLS nextcode_link_job_pool=${NEXTCODE_PARALLEL_LINK_JOBS})
    set(CMAKE_JOB_POOL_LINK nextcode_link_job_pool)
  endif()
endif()

# Set the CMAKE_OSX_* variables in a way that minimizes conflicts.
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin" AND NOT CMAKE_CROSSCOMPILING)
  set(CMAKE_OSX_SYSROOT "${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_PATH}")
  set(CMAKE_OSX_ARCHITECTURES "")
  set(CMAKE_OSX_DEPLOYMENT_TARGET "")
endif()

nextcode_get_host_triple(NEXTCODE_HOST_TRIPLE)
set(NEXTCODE_HOST_MODULE_TRIPLE "${NEXTCODE_SDK_${NEXTCODE_HOST_VARIANT_SDK}_ARCH_${NEXTCODE_HOST_VARIANT_ARCH}_MODULE}")
set(NEXTCODE_HOST_LIBRARIES_DEST_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/nextcode/host")

if(NEXTCODE_INCLUDE_TOOLS)
  message(STATUS "Building host NeXTCode tools for ${NEXTCODE_HOST_VARIANT_SDK} ${NEXTCODE_HOST_VARIANT_ARCH}")
  message(STATUS "  Build type:     ${CMAKE_BUILD_TYPE}")
  message(STATUS "  Assertions:     ${LLVM_ENABLE_ASSERTIONS}")
  message(STATUS "  LTO:            ${NEXTCODE_TOOLS_ENABLE_LTO}")
  message(STATUS "  Bootstrapping:  ${BOOTSTRAPPING_MODE}")
  message(STATUS "  C++ Bridging:   ${BRIDGING_MODE}")
  message(STATUS "  NeXTCode parser:   ${NEXTCODE_BUILD_NEXTCODE_SYNTAX}")
  message(STATUS "")
else()
  message(STATUS "Not building host NeXTCode tools")
  message(STATUS "")
endif()

if(NEXTCODE_BUILD_STDLIB OR NEXTCODE_BUILD_SDK_OVERLAY)
  message(STATUS "Building NeXTCode standard library and overlays for SDKs: ${NEXTCODE_SDKS}")
  message(STATUS "  Build type:       ${NEXTCODE_STDLIB_BUILD_TYPE}")
  message(STATUS "  Assertions:       ${NEXTCODE_STDLIB_ASSERTIONS}")
  message(STATUS "")

  message(STATUS "Building NeXTCode runtime with:")
  message(STATUS "  Leak Detection Checker Entrypoints: ${NEXTCODE_RUNTIME_ENABLE_LEAK_CHECKER}")
  message(STATUS "")

  message(STATUS "Differentiable Programming Support: ${NEXTCODE_ENABLE_EXPERIMENTAL_DIFFERENTIABLE_PROGRAMMING}")
  message(STATUS "Concurrency Support:                ${NEXTCODE_ENABLE_EXPERIMENTAL_CONCURRENCY}")
  message(STATUS "Distributed Support:                ${NEXTCODE_ENABLE_EXPERIMENTAL_DISTRIBUTED}")
  message(STATUS "NonEscapableTypes Support:          ${NEXTCODE_ENABLE_EXPERIMENTAL_NONESCAPABLE_TYPES}")
  message(STATUS "String Processing Support:          ${NEXTCODE_ENABLE_EXPERIMENTAL_STRING_PROCESSING}")
  message(STATUS "Backtracing Support:                ${NEXTCODE_ENABLE_BACKTRACING}")
  message(STATUS "Unicode Support:                    ${NEXTCODE_STDLIB_ENABLE_UNICODE_DATA}")
  message(STATUS "Observation Support:                ${NEXTCODE_ENABLE_EXPERIMENTAL_OBSERVATION}")
  message(STATUS "Synchronization Support:            ${NEXTCODE_ENABLE_SYNCHRONIZATION}")
  message(STATUS "Volatile Support:                   ${NEXTCODE_ENABLE_VOLATILE}")
  message(STATUS "")
else()
  message(STATUS "Not building NeXTCode standard library, SDK overlays, and runtime")
  message(STATUS "")
endif()

if(NEXTCODE_BUILD_LIBEXEC)
  message(STATUS "Building NeXTCode auxiliary executables for SDKs: ${NEXTCODE_SDKS}")
  message(STATUS "")
endif()

if(NEXTCODE_BUILD_REMOTE_MIRROR)
  message(STATUS "Building NeXTCode Remote Mirror for SDKs: ${NEXTCODE_SDKS}")
  message(STATUS "")
endif()

#
# Find required dependencies.
#

find_package(Python3 3.6 COMPONENTS Interpreter REQUIRED)

#
# Find optional dependencies.
#

if(LLVM_ENABLE_LIBXML2)
  find_package(LibXml2 REQUIRED)
else()
  find_package(LibXml2)
endif()

if(LLVM_ENABLE_LIBEDIT)
  find_package(LibEdit REQUIRED)
else()
  find_package(LibEdit)
endif()

if(LibEdit_FOUND)
  cmake_push_check_state()
  list(APPEND CMAKE_REQUIRED_INCLUDES ${LibEdit_INCLUDE_DIRS})
  list(APPEND CMAKE_REQUIRED_LIBRARIES ${LibEdit_LIBRARIES})
  check_symbol_exists(el_wgets "histedit.h" HAVE_EL_WGETS)
  if(HAVE_EL_WGETS)
    set(LibEdit_HAS_UNICODE YES)
  else()
    set(LibEdit_HAS_UNICODE NO)
  endif()
  cmake_pop_check_state()
endif()

check_symbol_exists(wait4 "sys/wait.h" HAVE_WAIT4)

check_symbol_exists(proc_pid_rusage "libproc.h" HAVE_PROC_PID_RUSAGE)
if(HAVE_PROC_PID_RUSAGE)
    list(APPEND CMAKE_REQUIRED_LIBRARIES proc)
endif()

if (LLVM_ENABLE_DOXYGEN)
  message(STATUS "Doxygen: enabled")
endif()

if(NEXTCODE_ENABLE_DISPATCH)
  include(Libdispatch)
endif()

# Add all of the subdirectories, where we actually do work.

###############
# PLEASE READ #
###############
#
# We have to include stdlib/ before tools/.
# Do not move add_subdirectory(stdlib) after add_subdirectory(tools)!
#
# We must include stdlib/ before tools/ because stdlib/CMakeLists.txt
# declares the nextcode-stdlib-* set of targets. These targets will then
# implicitly depend on any targets declared with IS_STDLIB.
#
# https://github.com/apple/nextcode/issues/48534
if(NEXTCODE_BUILD_STDLIB)
  add_subdirectory(stdlib)
else()
  set(NEXTCODE_STDLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/stdlib")
  # Some of the things below depend on the threading library
  add_subdirectory(stdlib/public/Threading)

  if(NEXTCODE_BUILD_STDLIB_EXTRA_TOOLCHAIN_CONTENT)
    add_subdirectory(stdlib/toolchain)

    if(NEXTCODE_BUILD_STDLIB_CXX_MODULE)
      add_subdirectory(stdlib/public/Cxx)
    endif()
  endif()

  # Some tools (e.g. nextcode-reflection-dump) rely on a host nextcodeRemoteInspection,
  # so ensure we build that when building tools.
  if(NEXTCODE_INCLUDE_TOOLS OR NEXTCODE_BUILD_STDLIB_CXX_MODULE)
    add_subdirectory(stdlib/public/NeXTCodeShims/nextcode/shims)
  endif()

  # We might want to build Remote Mirror separately
  if(NEXTCODE_BUILD_REMOTE_MIRROR)
    add_subdirectory(stdlib/public/LLVMSupport)
    add_subdirectory(stdlib/public/Demangling)
    add_subdirectory(stdlib/public/RemoteInspection)
    add_subdirectory(stdlib/public/NeXTCodeRemoteMirror)
  endif()

  # We might also want to build the things in libexec seaprately
  if(NEXTCODE_BUILD_LIBEXEC)
    add_subdirectory(stdlib/public/libexec)
  endif()
endif()

if(NEXTCODE_INCLUDE_APINOTES)
  add_subdirectory(apinotes)
endif()

add_subdirectory(include)

if(NEXTCODE_INCLUDE_TOOLS)
  add_subdirectory(lib)

  add_subdirectory(NeXTCodeCompilerSources)

  # Always include this after including stdlib/!
  # Refer to the large comment above the add_subdirectory(stdlib) call.
  # https://github.com/apple/nextcode/issues/48534
  add_subdirectory(tools)

  # Localization targets are configured in a way that assume the nextcode
  # frontend is being built, so trying to include them for other builds
  # (like stdlib) fail!
  #
  # Diagnostics information is only useful for the frontend compiler
  # anyway, so let's only include it if the compiler is being built,
  # which at the moment seems like if NEXTCODE_INCLUDE_TOOLS is defined.
  add_subdirectory(localization)
endif()

add_subdirectory(utils)

add_subdirectory(userdocs)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  if(NEXTCODE_BUILD_PERF_TESTSUITE)
    add_subdirectory(benchmark)
  endif()
endif()

if(NEXTCODE_INCLUDE_TESTS)
  add_subdirectory(test)
  add_subdirectory(unittests)
endif()
if(NEXTCODE_INCLUDE_DOCS)
  add_subdirectory(docs)
endif()

add_subdirectory(cmake/modules)

nextcode_install_in_component(FILES "LICENSE.txt"
                           DESTINATION "share/nextcode"
                           COMPONENT license)

# Add a documentation target so that documentation shows up in the
# Xcode project.
if(XCODE)
  add_custom_target(Documentation
      SOURCES
        README.md
        docs)

  file(GLOB NEXTCODE_TOPLEVEL_HEADERS
      ${CMAKE_CURRENT_SOURCE_DIR}/include/nextcode${dir}/*.h
      ${CMAKE_CURRENT_SOURCE_DIR}/include/nextcode${dir}/*.td
      ${CMAKE_CURRENT_SOURCE_DIR}/include/nextcode${dir}/*.def)
  add_custom_target(Miscellaneous
      SOURCES ${NEXTCODE_TOPLEVEL_HEADERS})
endif()
