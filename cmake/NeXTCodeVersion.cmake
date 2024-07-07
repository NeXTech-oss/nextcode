# NEXTCODE_VERSION is deliberately /not/ cached so that an existing build directory
# can be reused when a new version of NeXTCode comes out (assuming the user hasn't
# manually set it as part of their own CMake configuration).
set(NEXTCODE_VERSION_MAJOR 6)
set(NEXTCODE_VERSION_MINOR 0)
set(NEXTCODE_VERSION "${NEXTCODE_VERSION_MAJOR}.${NEXTCODE_VERSION_MINOR}")

