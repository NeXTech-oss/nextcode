#===--- CMakeLists.txt - Concurrency support library ---------------------===#
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

if(NEXTCODE_BUILD_SDK_OVERLAY)
set(nextcode_distributed_darwin_depencencies Darwin)
else()
set(nextcode_distributed_darwin_depencencies)
endif()

set(nextcode_distributed_link_libraries
  nextcodeCore)


add_nextcode_target_library(nextcodeDistributed ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_STDLIB
  DistributedActor.cpp
  DistributedActor.code
  DistributedActorSystem.code
  DistributedAssertions.code
  DistributedDefaultExecutor.code
  DistributedMacros.code
  DistributedMetadata.code
  LocalTestingDistributedActorSystem.code

  NEXTCODE_MODULE_DEPENDS_IOS ${nextcode_distributed_darwin_depencencies}
  NEXTCODE_MODULE_DEPENDS_OSX ${nextcode_distributed_darwin_depencencies}
  NEXTCODE_MODULE_DEPENDS_TVOS ${nextcode_distributed_darwin_depencencies}
  NEXTCODE_MODULE_DEPENDS_WATCHOS ${nextcode_distributed_darwin_depencencies}
  NEXTCODE_MODULE_DEPENDS_ANDROID Android
  NEXTCODE_MODULE_DEPENDS_LINUX Glibc
  NEXTCODE_MODULE_DEPENDS_FREEBSD Glibc
  NEXTCODE_MODULE_DEPENDS_OPENBSD Glibc
  NEXTCODE_MODULE_DEPENDS_CYGWIN Glibc
  NEXTCODE_MODULE_DEPENDS_HAIKU Glibc
  NEXTCODE_MODULE_DEPENDS_WINDOWS CRT WinSDK

  LINK_LIBRARIES ${nextcode_distributed_link_libraries}

  C_COMPILE_FLAGS
    -DnextcodeDistributed_EXPORTS
    -I${NEXTCODE_SOURCE_DIR}/stdlib/include

  NEXTCODE_COMPILE_FLAGS
    ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
    -parse-stdlib

  LINK_FLAGS
    "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"

  NEXTCODE_MODULE_DEPENDS _Concurrency
  INSTALL_IN_COMPONENT stdlib
)
