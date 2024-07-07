#===--- CMakeLists.txt - Macro support libraries ------------------------===#
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

# The location for host plugins, i.e., macros.
set(NEXTCODE_HOST_PLUGINS_DEST_DIR ${NEXTCODE_HOST_LIBRARIES_DEST_DIR}/plugins)

# Add a new NeXTCode macro library, which is built for the host and installed
# into lib/nextcode/host/plugins.
#
# Usage:
#   add_nextcode_macro_library(name
#     source1 [source2 source3 ...])
#
# name
#   Name of the library (e.g., NeXTCodeMacros).
#
# source1 ...
#   Sources to add into this library.
function(add_nextcode_macro_library name)
  set(ASML_SOURCES ${ARGN})

  # Add the library.
  add_pure_nextcode_host_library(${name} SHARED ${ASML_SOURCES})

  # If we don't have the NeXTCode nextcode parser, bail out, because the above
  # add_pure_nextcode_host_library did nothing.
  if (NOT NEXTCODE_BUILD_NEXTCODE_SYNTAX)
    return()
  endif()

  # Add rpath to 'lib/{platform}'
  file(RELATIVE_PATH relpath_to_lib
    "${NEXTCODE_HOST_PLUGINS_DEST_DIR}"
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
  )
  _set_pure_nextcode_link_flags(${name} "${relpath_to_lib}")

  # Add rpath to 'lib/host'
  if(NEXTCODE_HOST_VARIANT_SDK MATCHES "LINUX|ANDROID|OPENBSD|FREEBSD")
    set_property(TARGET ${name}
      APPEND PROPERTY INSTALL_RPATH "$ORIGIN/..")
  endif()

  # Install into the plugin directory.
  set_target_properties(${name}
    PROPERTIES
      ARCHIVE_OUTPUT_DIRECTORY "${NEXTCODE_HOST_PLUGINS_DEST_DIR}"
      LIBRARY_OUTPUT_DIRECTORY "${NEXTCODE_HOST_PLUGINS_DEST_DIR}"
  )

  set(destination_dir "lib${LLVM_LIBDIR_SUFFIX}/nextcode/host/plugins")

  nextcode_install_in_component(TARGETS ${name}
    RUNTIME
      DESTINATION bin
      COMPONENT compiler
    LIBRARY
      DESTINATION "${destination_dir}"
      COMPONENT compiler
    ARCHIVE
      DESTINATION "${destination_dir}"
      COMPONENT compiler)

  # Export this macro plugin target.
  set_property(GLOBAL APPEND PROPERTY NEXTCODE_MACRO_PLUGINS ${name})
endfunction()

add_subdirectory(Sources/NeXTCodeMacros)
add_subdirectory(Sources/ObservationMacros)
