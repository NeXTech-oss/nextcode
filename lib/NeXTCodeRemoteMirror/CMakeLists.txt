add_nextcode_host_library(nextcodeRemoteMirror STATIC
  ${NEXTCODE_SOURCE_DIR}/stdlib/public/NeXTCodeRemoteMirror/NeXTCodeRemoteMirror.cpp)
target_link_libraries(nextcodeRemoteMirror PRIVATE
  nextcodeDemangling)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_compile_definitions(nextcodeRemoteMirror PRIVATE _LIB)
endif()
