#===--- CMakeLists.txt - Differentiable programming support library ------===#
#
# This source file is part of the NeXTCode.org open source project
#
# Copyright (c) 2019 - 2020 Apple Inc. and the NeXTCode project authors
# Licensed under Apache License v2.0 with Runtime Library Exception
#
# See https://nextcode.org/LICENSE.txt for license information
# See https://nextcode.org/CONTRIBUTORS.txt for the list of NeXTCode project authors
#
#===----------------------------------------------------------------------===#

if(NEXTCODE_STDLIB_ENABLE_VECTOR_TYPES)
set(nextcodeDifferentiationSIMDFiles SIMDDifferentiation.code.gyb)
else()
set(nextcodeDifferentiationSIMDFiles)
endif()

if(NEXTCODE_BUILD_SDK_OVERLAY)
set(nextcodeDifferentiationDarwinDependencies Darwin)
else()
set(nextcodeDifferentiationDarwinDependencies)
endif()

add_nextcode_target_library(nextcode_Differentiation ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_STDLIB
  Differentiable.code
  DifferentialOperators.code
  DifferentiationUtilities.code
  AnyDifferentiable.code
  ArrayDifferentiation.code
  OptionalDifferentiation.code

  "${NEXTCODE_SOURCE_DIR}/stdlib/linker-support/magic-symbols-for-install-name.c"

  GYB_SOURCES
    FloatingPointDifferentiation.code.gyb
    TgmathDerivatives.code.gyb
    ${nextcodeDifferentiationSIMDFiles}

  NEXTCODE_MODULE_DEPENDS_OSX ${nextcodeDifferentiationDarwinDependencies}
  NEXTCODE_MODULE_DEPENDS_IOS ${nextcodeDifferentiationDarwinDependencies}
  NEXTCODE_MODULE_DEPENDS_TVOS ${nextcodeDifferentiationDarwinDependencies}
  NEXTCODE_MODULE_DEPENDS_WATCHOS ${nextcodeDifferentiationDarwinDependencies}
  NEXTCODE_MODULE_DEPENDS_XROS ${nextcodeDifferentiationDarwinDependencies}
  NEXTCODE_MODULE_DEPENDS_ANDROID Android
  NEXTCODE_MODULE_DEPENDS_LINUX Glibc
  NEXTCODE_MODULE_DEPENDS_LINUX_STATIC Musl
  NEXTCODE_MODULE_DEPENDS_FREEBSD Glibc
  NEXTCODE_MODULE_DEPENDS_OPENBSD Glibc
  NEXTCODE_MODULE_DEPENDS_CYGWIN Glibc
  NEXTCODE_MODULE_DEPENDS_HAIKU Glibc
  NEXTCODE_MODULE_DEPENDS_WASI WASILibc
  NEXTCODE_MODULE_DEPENDS_WINDOWS CRT

  C_COMPILE_FLAGS
    -Dnextcode_Differentiation_EXPORTS
  NEXTCODE_COMPILE_FLAGS
    ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
    -parse-stdlib
  LINK_FLAGS "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"
  INSTALL_IN_COMPONENT stdlib
  NEXTCODE_MODULE_DEPENDS_MACCATALYST ${nextcodeDifferentiationDarwinDependencies}
  MACCATALYST_BUILD_FLAVOR "zippered")
