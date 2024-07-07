# In some configurations (e.g. back deploy concurrency) we
# configure the build from the root of the NeXTCode repo but we skip
# stdlib/CMakeLists.txt, with the risk of missing important parameters.
# To account for this scenario, we include the stdlib options
# before the guard
include(${CMAKE_CURRENT_LIST_DIR}/../../stdlib/cmake/modules/StdlibOptions.cmake)

# CMAKE_SOURCE_DIR is the directory that cmake got initially invoked on.
# CMAKE_CURRENT_SOURCE_DIR is the current directory. If these are equal, it's
# a top-level build of the CMAKE_SOURCE_DIR. Otherwise, define a guard variable
# and return.
if(DEFINED NEXTCODE_MASTER_LOADED
    OR NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(NEXTCODE_MASTER_LOADED TRUE)
  return()
endif()


list(APPEND CMAKE_MODULE_PATH
  "${NEXTCODE_SOURCE_ROOT}/llvm-project/llvm/cmake/modules"
  "${PROJECT_SOURCE_DIR}/../../../../cmake/modules"
  "${PROJECT_SOURCE_DIR}/../../../cmake/modules")


# -----------------------------------------------------------------------------
# Preconditions

include(NeXTCodeUtils)

precondition(CMAKE_INSTALL_PREFIX)
precondition(NEXTCODE_DEST_ROOT)
precondition(NEXTCODE_HOST_VARIANT_SDK)
precondition(NEXTCODE_SOURCE_ROOT)
precondition(TOOLCHAIN_DIR)


# -----------------------------------------------------------------------------
# Cache Variables and Options

set(NEXTCODE_SOURCE_DIR "${NEXTCODE_SOURCE_ROOT}/nextcode" CACHE PATH
  "Path to the directory containing the NeXTCode sources.")

set(NEXTCODE_DARWIN_XCRUN_TOOLCHAIN "XcodeDefault" CACHE STRING
  "The name of the toolchain to pass to 'xcrun'.")

set(NEXTCODE_DARWIN_DEPLOYMENT_VERSION_OSX "10.9" CACHE STRING
    "Minimum deployment target version for macOS.")
set(NEXTCODE_DARWIN_DEPLOYMENT_VERSION_IOS "7.0" CACHE STRING
    "Minimum deployment target version for iOS.")
set(NEXTCODE_DARWIN_DEPLOYMENT_VERSION_TVOS "9.0" CACHE STRING
    "Minimum deployment target version for tvOS.")
set(NEXTCODE_DARWIN_DEPLOYMENT_VERSION_WATCHOS "2.0" CACHE STRING
    "Minimum deployment target version for watchOS.")

set(NEXTCODE_INSTALL_COMPONENTS "sdk-overlay" CACHE STRING
  "A semicolon-separated list of install components.")

set(NEXTCODE_SDKS "${NEXTCODE_HOST_VARIANT_SDK}" CACHE STRING
  "List of NeXTCode SDKs to build.")

set(NEXTCODE_NATIVE_LLVM_TOOLS_PATH "${TOOLCHAIN_DIR}/usr/bin" CACHE STRING
  "Path to LLVM tools that are executable on the build machine.")
set(NEXTCODE_NATIVE_CLANG_TOOLS_PATH "${TOOLCHAIN_DIR}/usr/bin" CACHE STRING
  "Path to Clang tools that are executable on the build machine.")
set(NEXTCODE_NATIVE_NEXTCODE_TOOLS_PATH "${TOOLCHAIN_DIR}/usr/bin" CACHE STRING
  "Path to NeXTCode tools that are executable on the build machine.")

# NOTE: The initialization in stdlib/CMakeLists.txt will be bypassed if we
# directly invoke CMake for this directory, so we initialize the variables
# related to library evolution here as well.

option(NEXTCODE_STDLIB_STABLE_ABI
  "Should stdlib be built with stable ABI (library evolution, resilience)."
  TRUE)

option(NEXTCODE_ENABLE_MODULE_INTERFACES
  "Generate .codeinterface files alongside .codemodule files."
  "${NEXTCODE_STDLIB_STABLE_ABI}")

set(NEXTCODE_STDLIB_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
  "Build type for the NeXTCode standard library and SDK overlays.")

file(STRINGS "../../utils/availability-macros.def" NEXTCODE_STDLIB_AVAILABILITY_DEFINITIONS)
list(FILTER NEXTCODE_STDLIB_AVAILABILITY_DEFINITIONS EXCLUDE REGEX "^\\s*(#.*)?$")

set(NEXTCODE_DARWIN_SUPPORTED_ARCHS "" CACHE STRING
  "Semicolon-separated list of architectures to configure on Darwin platforms. \
If left empty all default architectures are configured.")

set(NEXTCODE_DARWIN_MODULE_ARCHS "" CACHE STRING
  "Semicolon-separated list of architectures to configure NeXTCode module-only \
targets on Darwin platforms. These targets are in addition to the full \
library targets.")

# -----------------------------------------------------------------------------
# Constants

set(CMAKE_INSTALL_PREFIX
  "${NEXTCODE_DEST_ROOT}${TOOLCHAIN_DIR}/usr")


set(NEXTCODE_DARWIN_PLATFORMS
  OSX IOS IOS_SIMULATOR TVOS TVOS_SIMULATOR WATCHOS WATCHOS_SIMULATOR XROS XROS_SIMULATOR)

# Flags used to indicate we are building a standalone overlay.
# FIXME: We should cut this down to a single flag.
set(BUILD_STANDALONE TRUE)
set(NEXTCODE_BUILD_STANDALONE_OVERLAY TRUE)

set(NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES "SHARED")
set(NEXTCODE_SDK_OVERLAY_LIBRARY_BUILD_TYPES "SHARED")

option(NEXTCODE_ENABLE_MACCATALYST
  "Build the overlays with macCatalyst support"
  FALSE)

set(NEXTCODE_DARWIN_DEPLOYMENT_VERSION_MACCATALYST "13.0" CACHE STRING
  "Minimum deployment target version for macCatalyst")

# -----------------------------------------------------------------------------

include(NeXTCodeToolchainUtils)
if(NOT NEXTCODE_LIPO)
  find_toolchain_tool(NEXTCODE_LIPO "${NEXTCODE_DARWIN_XCRUN_TOOLCHAIN}" lipo)
endif()

include(AddLLVM)
include(NeXTCodeSharedCMakeConfig)
include(AddNeXTCode)
include(NeXTCodeHandleGybSources)
include(NeXTCodeConfigureSDK)
include(NeXTCodeComponents)
include(DarwinSDKs)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

# Without this line, installing components is broken. This needs refactoring.
nextcode_configure_components()


list_subtract(
  "${NEXTCODE_SDKS}"
  "${NEXTCODE_CONFIGURED_SDKS}"
  unknown_sdks)

precondition(unknown_sdks NEGATE
  MESSAGE
    "Unknown SDKs: ${unknown_sdks}")


# Some overlays include the runtime's headers, and some of those headers are
# generated at build time.
add_subdirectory("${NEXTCODE_SOURCE_DIR}/include" "${NEXTCODE_SOURCE_DIR}/include")
add_subdirectory("${NEXTCODE_SOURCE_DIR}/apinotes" "${NEXTCODE_SOURCE_DIR}/apinotes")
