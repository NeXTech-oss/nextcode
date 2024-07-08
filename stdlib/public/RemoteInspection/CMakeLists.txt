set(nextcodeRemoteInspection_C_COMPILE_FLAGS)
if(NEXTCODE_ENABLE_REFLECTION)
  list(APPEND nextcodeRemoteInspection_C_COMPILE_FLAGS -DNEXTCODE_ENABLE_REFLECTION)
endif()

add_nextcode_target_library(nextcodeRemoteInspection STATIC
  MetadataSource.cpp
  TypeLowering.cpp
  TypeRef.cpp
  TypeRefBuilder.cpp
  C_COMPILE_FLAGS
    ${NEXTCODE_RUNTIME_CXX_FLAGS}
    -DnextcodeCore_EXPORTS
    ${nextcodeRemoteInspection_C_COMPILE_FLAGS}
  LINK_FLAGS ${NEXTCODE_RUNTIME_LINK_FLAGS}
  INCORPORATE_OBJECT_LIBRARIES
    nextcodeLLVMSupport nextcodeDemangling nextcodeDemanglingCR
  NEXTCODE_COMPILE_FLAGS ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
  INSTALL_IN_COMPONENT dev)
