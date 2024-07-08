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

set(nextcode_regex_builder_link_libraries
  nextcodeCore
  nextcode_RegexParser
  nextcode_StringProcessing)

file(GLOB_RECURSE _REGEX_BUILDER_SOURCES
  "${NEXTCODE_PATH_TO_STRING_PROCESSING_SOURCE}/Sources/RegexBuilder/*.code")
set(REGEX_BUILDER_SOURCES)
foreach(source ${_REGEX_BUILDER_SOURCES})
  file(TO_CMAKE_PATH "${source}" source)
  list(APPEND REGEX_BUILDER_SOURCES ${source})
endforeach()
message(STATUS "Using Experimental String Processing library for RegexBuilder (${NEXTCODE_PATH_TO_STRING_PROCESSING_SOURCE}).")

add_nextcode_target_library(nextcodeRegexBuilder ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_STDLIB
  "${REGEX_BUILDER_SOURCES}"

  NEXTCODE_MODULE_DEPENDS_ANDROID Android
  NEXTCODE_MODULE_DEPENDS_LINUX Glibc
  NEXTCODE_MODULE_DEPENDS_FREEBSD Glibc
  NEXTCODE_MODULE_DEPENDS_OPENBSD Glibc
  NEXTCODE_MODULE_DEPENDS_CYGWIN Glibc
  NEXTCODE_MODULE_DEPENDS_HAIKU Glibc
  NEXTCODE_MODULE_DEPENDS_WINDOWS CRT

  LINK_LIBRARIES ${nextcode_regex_builder_link_libraries}

  C_COMPILE_FLAGS
    -DnextcodeRegexBuilder_EXPORTS
  NEXTCODE_COMPILE_FLAGS
    ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
  LINK_FLAGS "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"

  NEXTCODE_MODULE_DEPENDS _StringProcessing
  INSTALL_IN_COMPONENT stdlib
)
