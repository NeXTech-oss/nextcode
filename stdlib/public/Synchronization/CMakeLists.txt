#===--- CMakeLists.txt - Synchronization library ---------------------------===#
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
#===----------------------------------------------------------------------===#

set(NEXTCODE_SYNCHRONIZATION_ATOMIC_SOURCES
  Atomics/Atomic.code
  Atomics/AtomicBool.code
  Atomics/AtomicFloats.code
  Atomics/AtomicLazyReference.code
  Atomics/AtomicMemoryOrderings.code
  Atomics/AtomicOptional.code
  Atomics/AtomicPointers.code
  Atomics/AtomicRepresentable.code
  Atomics/WordPair.code
)

set(NEXTCODE_SYNCHRONIZATION_SOURCES
  ${NEXTCODE_SYNCHRONIZATION_ATOMIC_SOURCES}

  Cell.code
)

set(NEXTCODE_SYNCHRONIZATION_GYB_SOURCES
  Atomics/AtomicIntegers.code.gyb
  Atomics/AtomicStorage.code.gyb
)

# Darwin dependencies and sources

set(NEXTCODE_SYNCHRONIZATION_DARWIN_DEPENDENCIES)

if(NEXTCODE_BUILD_SDK_OVERLAY)
  set(NEXTCODE_SYNCHRONIZATION_DARWIN_DEPENDENCIES Darwin)
endif()

set(NEXTCODE_SYNCHRONIZATION_DARWIN_SOURCES
  Mutex/DarwinImpl.code
  Mutex/Mutex.code
)

# Linux and Android sources

set(NEXTCODE_SYNCHRONIZATION_LINUX_SOURCES
  Mutex/LinuxImpl.code
  Mutex/Mutex.code
  Mutex/SpinLoopHint.code
)

# Wasm sources

set(NEXTCODE_SYNCHRONIZATION_WASM_SOURCES
  Mutex/Mutex.code
  Mutex/WasmImpl.code
)

# Windows sources

set(NEXTCODE_SYNCHRONIZATION_WINDOWS_SOURCES
  Mutex/Mutex.code
  Mutex/WindowsImpl.code
)

set(NEXTCODE_SYNCHRNOIZATION_NEXTCODE_FLAGS
  ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
  "-enable-builtin-module"
  "-enable-experimental-feature" "RawLayout"
  "-enable-experimental-feature" "StaticExclusiveOnly"
  "-enable-experimental-feature" "Extern"
)

add_nextcode_target_library(nextcodeSynchronization ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_STDLIB
  ${NEXTCODE_SYNCHRONIZATION_SOURCES}

  GYB_SOURCES
    ${NEXTCODE_SYNCHRONIZATION_GYB_SOURCES}

  NEXTCODE_SOURCES_DEPENDS_MACOS
    ${NEXTCODE_SYNCHRONIZATION_DARWIN_SOURCES}
  NEXTCODE_SOURCES_DEPENDS_IOS
    ${NEXTCODE_SYNCHRONIZATION_DARWIN_SOURCES}
  NEXTCODE_SOURCES_DEPENDS_TVOS
    ${NEXTCODE_SYNCHRONIZATION_DARWIN_SOURCES}
  NEXTCODE_SOURCES_DEPENDS_WATCHOS
    ${NEXTCODE_SYNCHRONIZATION_DARWIN_SOURCES}
  NEXTCODE_SOURCES_DEPENDS_VISIONOS
    ${NEXTCODE_SYNCHRONIZATION_DARWIN_SOURCES}
  NEXTCODE_SOURCES_DEPENDS_LINUX
    ${NEXTCODE_SYNCHRONIZATION_LINUX_SOURCES}
  NEXTCODE_SOURCES_DEPENDS_WASI
    ${NEXTCODE_SYNCHRONIZATION_WASM_SOURCES}
  NEXTCODE_SOURCES_DEPENDS_WINDOWS
    ${NEXTCODE_SYNCHRONIZATION_WINDOWS_SOURCES}
  NEXTCODE_SOURCES_DEPENDS_FREESTANDING
    Mutex/MutexUnavailable.code

  NEXTCODE_MODULE_DEPENDS_OSX
    ${NEXTCODE_SYNCHRONIZATION_DARWIN_DEPENDENCIES}
  NEXTCODE_MODULE_DEPENDS_IOS
    ${NEXTCODE_SYNCHRONIZATION_DARWIN_DEPENDENCIES}
  NEXTCODE_MODULE_DEPENDS_TVOS
    ${NEXTCODE_SYNCHRONIZATION_DARWIN_DEPENDENCIES}
  NEXTCODE_MODULE_DEPENDS_WATCHOS
    ${NEXTCODE_SYNCHRONIZATION_DARWIN_DEPENDENCIES}
  NEXTCODE_MODULE_DEPENDS_XROS
    ${NEXTCODE_SYNCHRONIZATION_DARWIN_DEPENDENCIES}
  NEXTCODE_MODULE_DEPENDS_MACCATALYST
    ${NEXTCODE_SYNCHRONIZATION_DARWIN_DEPENDENCIES}
  NEXTCODE_MODULE_DEPENDS_LINUX
    Glibc
  NEXTCODE_MODULE_DEPENDS_ANDROID
    Android
  NEXTCODE_MODULE_DEPENDS_WINDOWS
    WinSDK

  NEXTCODE_COMPILE_FLAGS
    ${NEXTCODE_SYNCHRNOIZATION_NEXTCODE_FLAGS}
  LINK_FLAGS
    "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"
  INSTALL_IN_COMPONENT
    stdlib
  MACCATALYST_BUILD_FLAVOR
    "zippered"
)

# Embedded Synchronization - embedded libraries are built as .codemodule only,
# i.e. there is no .o or .a file produced (no binary code is actually produced)
# and only users of a library are going to actually compile any needed code.
if(NEXTCODE_SHOULD_BUILD_EMBEDDED_STDLIB)
  add_custom_target(embedded-synchronization)
  add_dependencies(embedded-libraries embedded-synchronization)

  foreach(entry ${EMBEDDED_STDLIB_TARGET_TRIPLES})
    string(REGEX REPLACE "[ \t]+" ";" list "${entry}")
    list(GET list 0 arch)
    list(GET list 1 mod)
    list(GET list 2 triple)
    
    set(NEXTCODE_SDK_embedded_ARCH_${arch}_MODULE "${mod}")
    set(NEXTCODE_SDK_embedded_LIB_SUBDIR "embedded")
    set(NEXTCODE_SDK_embedded_ARCH_${arch}_TRIPLE "${triple}")
    set(NEXTCODE_SDK_embedded_PATH ${NEXTCODE_SDK_OSX_PATH})
    set(NEXTCODE_SDK_embedded_ARCH_${arch}_PATH ${NEXTCODE_SDK_OSX_PATH})
    set(NEXTCODE_SDK_embedded_USE_ISYSROOT TRUE)
    add_nextcode_target_library_single(
      embedded-synchronization-${mod}
      nextcodeSynchronization
      ONLY_NEXTCODEMODULE
      IS_FRAGILE
      
      ${NEXTCODE_SYNCHRONIZATION_ATOMIC_SOURCES}
      GYB_SOURCES
        ${NEXTCODE_SYNCHRONIZATION_GYB_SOURCES}

      NEXTCODE_COMPILE_FLAGS
        ${NEXTCODE_SYNCHRNOIZATION_NEXTCODE_FLAGS}
        -Xcc -D__MACH__ -Xcc -D__APPLE__ -Xcc -ffreestanding -enable-experimental-feature Embedded

      MODULE_DIR "${CMAKE_BINARY_DIR}/lib/nextcode/embedded"
      SDK "embedded"
      ARCHITECTURE "${arch}"
      DEPENDS embedded-stdlib-${mod}
      INSTALL_IN_COMPONENT stdlib
      )
    add_dependencies(embedded-synchronization embedded-synchronization-${mod})
  endforeach()
endif()
