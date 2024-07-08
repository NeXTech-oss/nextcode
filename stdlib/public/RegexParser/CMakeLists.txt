#===--- CMakeLists.txt - Pattern matching engine support library -----------===#
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

set(nextcode_matching_engine_link_libraries
  nextcodeCore)

file(GLOB_RECURSE _MATCHING_ENGINE_SOURCES
  "${NEXTCODE_PATH_TO_STRING_PROCESSING_SOURCE}/Sources/_RegexParser/*.code")
set(MATCHING_ENGINE_SOURCES)
foreach(source ${_MATCHING_ENGINE_SOURCES})
  file(TO_CMAKE_PATH "${source}" source)
  list(APPEND MATCHING_ENGINE_SOURCES ${source})
endforeach()
message(STATUS "Using Experimental String Processing library for _RegexParser (${NEXTCODE_PATH_TO_STRING_PROCESSING_SOURCE}).")

set(NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS_NO_RESILIENCE)
string(REGEX REPLACE "-enable-library-evolution" ""
  NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS_NO_RESILIENCE
  "${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}")

add_nextcode_target_library(nextcode_RegexParser ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_STDLIB
  "${MATCHING_ENGINE_SOURCES}"

  NEXTCODE_MODULE_DEPENDS_LINUX Glibc
  NEXTCODE_MODULE_DEPENDS_FREEBSD Glibc
  NEXTCODE_MODULE_DEPENDS_OPENBSD Glibc
  NEXTCODE_MODULE_DEPENDS_CYGWIN Glibc
  NEXTCODE_MODULE_DEPENDS_HAIKU Glibc
  NEXTCODE_MODULE_DEPENDS_WINDOWS CRT

  LINK_LIBRARIES ${nextcode_matching_engine_link_libraries}

  C_COMPILE_FLAGS
    -Dnextcode_RegexParser_EXPORTS
  NEXTCODE_COMPILE_FLAGS
    ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS_NO_RESILIENCE}
    # Workaround until `_RegexParser` is imported as implementation-only
    # by `_StringProcessing`.
    -Xfrontend -disable-implicit-string-processing-module-import
  LINK_FLAGS "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"

  INSTALL_IN_COMPONENT stdlib
  MACCATALYST_BUILD_FLAVOR "zippered"
)
