
set(NEXTCODE_HOST_VARIANT_SDK ANDROID CACHE STRING "")
set(NEXTCODE_HOST_VARIANT_ARCH armv7 CACHE STRING "")

# NOTE(compnerd) disable the tools, we are trying to build just the standard
# library.
set(NEXTCODE_INCLUDE_TOOLS NO CACHE BOOL "")

# NOTE(compnerd) cannot build tests since the tests require the toolchain
set(NEXTCODE_INCLUDE_TESTS NO CACHE BOOL "")

# NOTE(compnerd) cannot build docs since that requires perl
set(NEXTCODE_INCLUDE_DOCS NO CACHE BOOL "")

# NOTE(compnerd) these are part of the toolchain, not the runtime.
set(NEXTCODE_BUILD_SOURCEKIT NO CACHE BOOL "")

# NOTE(compnerd) build with the compiler specified, not a just built compiler.
set(NEXTCODE_BUILD_RUNTIME_WITH_HOST_COMPILER YES CACHE BOOL "")

set(NEXTCODE_SDK_ANDROID_ARCHITECTURES armv7 CACHE STRING "")

# NOTE(compnerd) this is lollipop, which seems to still have decent usage.
set(NEXTCODE_ANDROID_API_LEVEL 21 CACHE STRING "")
