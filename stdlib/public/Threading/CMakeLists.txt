# This is the counterpart to lib/Threading/CMakeLists.txt.  Any updates
# need to be reflected in both places.

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/modules")
include(AddNeXTCodeStdlib)

# This should *not* include ThreadSanitizer.cpp, as that is part of libnextcodeCore
add_nextcode_target_library(nextcodeThreading OBJECT_LIBRARY
  "${NEXTCODE_SOURCE_DIR}/lib/Threading/C11.cpp"
  "${NEXTCODE_SOURCE_DIR}/lib/Threading/Linux.cpp"
  "${NEXTCODE_SOURCE_DIR}/lib/Threading/Pthreads.cpp"
  "${NEXTCODE_SOURCE_DIR}/lib/Threading/Win32.cpp"
  INSTALL_IN_COMPONENT never_install)
