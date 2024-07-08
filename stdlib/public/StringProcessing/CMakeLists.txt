#===--- CMakeLists.txt - String processing support library -----------------===#
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

set(nextcode_string_processing_link_libraries
  nextcodeCore
  nextcode_RegexParser)

set(nextcode_string_processing_compile_flags)

# Pass a flag to the build indicating that _StringProcessing's dependencies
# will be built with library evolution.
list(APPEND nextcode_string_processing_compile_flags
  "-DRESILIENT_LIBRARIES")

if(NEXTCODE_BUILD_STATIC_STDLIB)
  # Explicitly autolink nextcode_RegexParser because it's imported with @_implementationOnly
  list(APPEND nextcode_string_processing_compile_flags
    "-Xfrontend" "-public-autolink-library" "-Xfrontend" "nextcode_RegexParser")
endif()

file(GLOB_RECURSE _STRING_PROCESSING_SOURCES
  "${NEXTCODE_PATH_TO_STRING_PROCESSING_SOURCE}/Sources/_StringProcessing/*.code"
  "${NEXTCODE_PATH_TO_STRING_PROCESSING_SOURCE}/Sources/_CUnicode/*.h"
  "${NEXTCODE_PATH_TO_STRING_PROCESSING_SOURCE}/Sources/_CUnicode/*.c")
set(STRING_PROCESSING_SOURCES)
foreach(source ${_STRING_PROCESSING_SOURCES})
  file(TO_CMAKE_PATH "${source}" source)
  list(APPEND STRING_PROCESSING_SOURCES ${source})
endforeach()
message(STATUS "Using Experimental String Processing library for _StringProcessing (${NEXTCODE_PATH_TO_STRING_PROCESSING_SOURCE}).")

add_nextcode_target_library(nextcode_StringProcessing ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_STDLIB
  "${STRING_PROCESSING_SOURCES}"

  NEXTCODE_MODULE_DEPENDS_ANDROID Android
  NEXTCODE_MODULE_DEPENDS_LINUX Glibc
  NEXTCODE_MODULE_DEPENDS_FREEBSD Glibc
  NEXTCODE_MODULE_DEPENDS_OPENBSD Glibc
  NEXTCODE_MODULE_DEPENDS_CYGWIN Glibc
  NEXTCODE_MODULE_DEPENDS_HAIKU Glibc
  NEXTCODE_MODULE_DEPENDS_WINDOWS CRT

  LINK_LIBRARIES ${nextcode_string_processing_link_libraries}

  C_COMPILE_FLAGS
    -Dnextcode_StringProcessing_EXPORTS
  NEXTCODE_COMPILE_FLAGS
    ${nextcode_string_processing_compile_flags}
    ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
  LINK_FLAGS "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"

  NEXTCODE_MODULE_DEPENDS _RegexParser
  INSTALL_IN_COMPONENT stdlib
  MACCATALYST_BUILD_FLAVOR "zippered"
)
