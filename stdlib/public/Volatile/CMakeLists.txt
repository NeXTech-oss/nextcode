#===--- CMakeLists.txt ---------------------------------------------------===#
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
#===----------------------------------------------------------------------===#

add_nextcode_target_library(nextcode_Volatile ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_STDLIB
  Volatile.code

  NEXTCODE_COMPILE_FLAGS
    ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
    -parse-stdlib
  LINK_FLAGS "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"

  INSTALL_IN_COMPONENT stdlib
)

if(NEXTCODE_SHOULD_BUILD_EMBEDDED_STDLIB)
  add_custom_target(embedded-volatile)
  add_dependencies(embedded-libraries embedded-volatile)
  foreach(entry ${EMBEDDED_STDLIB_TARGET_TRIPLES})
    string(REGEX REPLACE "[ \t]+" ";" list "${entry}")
    list(GET list 0 arch)
    list(GET list 1 mod)
    list(GET list 2 triple)

    set(NEXTCODE_SDK_embedded_ARCH_${arch}_MODULE "${mod}")
    set(NEXTCODE_SDK_embedded_LIB_SUBDIR "embedded")
    set(NEXTCODE_SDK_embedded_ARCH_${arch}_TRIPLE "${triple}")
    add_nextcode_target_library_single(
      embedded-volatile-${mod}
      nextcode_Volatile
      ONLY_NEXTCODEMODULE
      IS_SDK_OVERLAY IS_FRAGILE

      Volatile.code
      
      NEXTCODE_COMPILE_FLAGS
        -Xcc -D__MACH__ -Xcc -D__APPLE__ -Xcc -ffreestanding -enable-experimental-feature Embedded
        -parse-stdlib
      C_COMPILE_FLAGS
        -D__MACH__ -D__APPLE__ -ffreestanding
      MODULE_DIR "${CMAKE_BINARY_DIR}/lib/nextcode/embedded"
      SDK "embedded"
      ARCHITECTURE "${arch}"
      DEPENDS embedded-stdlib-${mod}
      INSTALL_IN_COMPONENT stdlib
      )
    add_dependencies(embedded-volatile embedded-volatile-${mod})
  endforeach()
endif()
