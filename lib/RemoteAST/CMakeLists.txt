if(XCODE)
  file(GLOB_RECURSE REMOTE_LIB_HEADERS
    ${NEXTCODE_SOURCE_DIR}/include/nextcode/Remote/*.h
    ${NEXTCODE_SOURCE_DIR}/include/nextcode/Remote/*.def)

  set_source_files_properties(${REMOTE_LIB_HEADERS}
    PROPERTIES
    HEADER_FILE_ONLY true)
  source_group("libRemote Headers" FILES ${REMOTE_LIB_HEADERS})
else()
  set(REMOTE_LIB_HEADERS)
endif()

add_nextcode_host_library(nextcodeRemoteAST STATIC
  RemoteAST.cpp
  InProcessMemoryReader.cpp
  ${REMOTE_LIB_HEADERS})
target_link_libraries(nextcodeRemoteAST PUBLIC
  nextcodeDemangling)
target_link_libraries(nextcodeRemoteAST PRIVATE
  nextcodeIRGen
  nextcodeSema)
