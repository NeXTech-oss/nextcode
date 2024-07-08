#===--- CMakeLists.txt - Observation support library ---------------------===#
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

list(APPEND nextcode_runtime_library_compile_flags -I${NEXTCODE_SOURCE_DIR}/stdlib/include -I${NEXTCODE_SOURCE_DIR}/include)

add_nextcode_target_library(nextcodeObservation ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_STDLIB
  Locking.cpp
  Locking.code
  Observable.code
  ObservationRegistrar.code
  ObservationTracking.code
  ThreadLocal.cpp
  ThreadLocal.code

  NEXTCODE_COMPILE_FLAGS
    ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
    "-enable-experimental-feature" "Macros"
    "-enable-experimental-feature" "ExtensionMacros"
    -Xfrontend -disable-implicit-string-processing-module-import

  C_COMPILE_FLAGS
    ${nextcode_runtime_library_compile_flags}
  LINK_FLAGS "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"

  NEXTCODE_MODULE_DEPENDS _Concurrency
  INSTALL_IN_COMPONENT stdlib

  MACCATALYST_BUILD_FLAVOR "zippered"
)
