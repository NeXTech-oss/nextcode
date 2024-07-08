#===--- CMakeLists.txt - Backtracing support library -----------------------===#
#
#
# Copyright (c) 2024, NeXTech Corporation. All rights reserved.
# DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#
# This code is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# version 2 for more details (a copy is included in the LICENSE file that
# accompanied this code).
#
# Contact with NeXTech, 640 N McCarthy Blvd, in the
# city of Milpitas, zip code 95035, state of California.
# or visit www.it-gss.com if you need additional information or have any
# questions.
#
# Author(-s): Mohammed Samy El-Melegy (mohammedsamy@it-gss.com)
#
#
#===------------------------------------------------------------------------===#

set(nextcode_backtracing_link_libraries
  nextcodeCore
  nextcode_Concurrency
)

set(concurrency)
if(NEXTCODE_BUILD_STDLIB AND NEXTCODE_ENABLE_EXPERIMENTAL_CONCURRENCY)
  set(concurrency _Concurrency)
endif()

set(BACKTRACING_SOURCES
  ArrayImageSource.code
  Backtrace.code
  BacktraceFormatter.code
  ByteSwapping.code
  CachingMemoryReader.code
  Context.code
  Compression.code
  CoreSymbolication.code
  Dwarf.code
  Elf.code
  FileImageSource.code
  FramePointerUnwinder.code
  Image.code
  ImageSource.code
  MemoryImageSource.code
  MemoryReader.code
  ProcMapsScanner.code
  Registers.code
  SymbolicatedBacktrace.code
  Utils.code
  Win32Extras.cpp

  get-cpu-context.${NEXTCODE_ASM_EXT}
)

set(BACKTRACING_COMPILE_FLAGS
  "-Xfrontend;-experimental-spi-only-imports"
  "-Xcc;-I${NEXTCODE_SOURCE_DIR}/include"
  "-Xcc;-I${CMAKE_BINARY_DIR}/include"
  "-Xcc;-fno-implicit-module-maps"
  "-Xcc;-fbuiltin-module-map"
  "-Xcc;-fmodule-map-file=${NEXTCODE_STDLIB_SOURCE_DIR}/public/NeXTCodeShims/nextcode/shims/module.modulemap"
  "-Xcc;-fmodule-map-file=${NEXTCODE_STDLIB_SOURCE_DIR}/public/Backtracing/modules/module.modulemap")

###TODO: Add these when we add static linking support
#
#list(APPEND BACKTRACING_COMPILE_FLAGS
#  "-Xcc;-I${NEXTCODE_PATH_TO_ZLIB_SOURCE}"
#  "-Xcc;-I${NEXTCODE_PATH_TO_ZSTD_SOURCE}/lib"
#  "-Xcc;-I${NEXTCODE_PATH_TO_LIBLZMA_SOURCE}/src/liblzma/api")

if(NEXTCODE_ASM_AVAILABLE)
  list(APPEND BACKTRACING_SOURCES get-cpu-context.${NEXTCODE_ASM_EXT})
  list(APPEND BACKTRACING_COMPILE_FLAGS "-DNEXTCODE_ASM_AVAILABLE")
else()
  message(warning "Assembly language not available on this platform; backtracing will fail.")
endif()

set(LLVM_OPTIONAL_SOURCES
  get-cpu-context.S
  get-cpu-context.asm
)

add_nextcode_target_library(nextcode_Backtracing ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_STDLIB
  ${BACKTRACING_SOURCES}

  NEXTCODE_MODULE_DEPENDS ${concurrency}

  PRIVATE_LINK_LIBRARIES ${nextcode_backtracing_link_libraries}

  NEXTCODE_COMPILE_FLAGS
    ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
    ${BACKTRACING_COMPILE_FLAGS}
    -parse-stdlib

  LINK_FLAGS
    ${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}

  INSTALL_IN_COMPONENT stdlib
  MACCATALYST_BUILD_FLAVOR "zippered"

  TARGET_SDKS OSX LINUX LINUX_STATIC
)
