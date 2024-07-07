add_subdirectory(Runtime)

if(NEXTCODE_BUILD_REMOTE_MIRROR)
  add_subdirectory(NeXTCodeRemoteMirror)
endif()

if(NEXTCODE_INCLUDE_TOOLS)
  configure_file(Config.h.in ${CMAKE_CURRENT_BINARY_DIR}/Config.h
                ESCAPE_QUOTES @ONLY)
  add_subdirectory(AST)
  add_subdirectory(Option)
endif()

