# Detect B&I builds.
set(NEXTCODE_BNI_OS_BUILD FALSE)
set(NEXTCODE_BNI_XCODE_BUILD FALSE)
if(DEFINED ENV{RC_XBS})
  if((NOT DEFINED ENV{RC_XCODE} OR NOT "$ENV{RC_XCODE}") AND (NOT DEFINED ENV{RC_PLAYGROUNDS} OR NOT "$ENV{RC_PLAYGROUNDS}"))
    set(NEXTCODE_BNI_OS_BUILD TRUE)
  else()
    set(NEXTCODE_BNI_XCODE_BUILD TRUE)
  endif()
endif()

configure_file(CMakeConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/CMakeConfig.h
               ESCAPE_QUOTES @ONLY)
nextcode_install_in_component(FILES ${CMAKE_CURRENT_BINARY_DIR}/CMakeConfig.h
                           DESTINATION "include/nextcode/Runtime"
                           COMPONENT testsuite-tools)
