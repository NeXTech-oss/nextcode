
add_nextcode_host_library(nextcodeRemoteInspection STATIC
  ${NEXTCODE_SOURCE_DIR}/stdlib/public/RemoteInspection/MetadataSource.cpp
  ${NEXTCODE_SOURCE_DIR}/stdlib/public/RemoteInspection/TypeLowering.cpp
  ${NEXTCODE_SOURCE_DIR}/stdlib/public/RemoteInspection/TypeRef.cpp
  ${NEXTCODE_SOURCE_DIR}/stdlib/public/RemoteInspection/TypeRefBuilder.cpp)
target_compile_options(nextcodeRemoteInspection PRIVATE -DNEXTCODE_ENABLE_REFLECTION)
target_link_libraries(nextcodeRemoteInspection PUBLIC
  nextcodeDemangling)
