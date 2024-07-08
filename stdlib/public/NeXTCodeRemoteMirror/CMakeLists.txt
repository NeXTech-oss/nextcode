# libnextcodeRemoteMirror.dylib should not have runtime dependencies; it's
# always built as a shared library.
add_nextcode_target_library(nextcodeRemoteMirror
                         SHARED DONT_EMBED_BITCODE NONEXTCODERT
                         NeXTCodeRemoteMirror.cpp
                         LINK_LIBRARIES
                           nextcodeRemoteInspection
                         C_COMPILE_FLAGS
                           ${NEXTCODE_RUNTIME_CXX_FLAGS}
                           -DnextcodeRemoteMirror_EXPORTS
                         LINK_FLAGS
                           ${NEXTCODE_RUNTIME_LINK_FLAGS}
                         PREFIX_INCLUDE_DIRS
                           ${NEXTCODE_SOURCE_DIR}/include/nextcode/RemoteInspection/RuntimeHeaders
                         INCORPORATE_OBJECT_LIBRARIES nextcodeLLVMSupport
                         NEXTCODE_COMPILE_FLAGS ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
                         DARWIN_INSTALL_NAME_DIR "${NEXTCODELIB_DARWIN_INSTALL_NAME_DIR}"
                         INSTALL_IN_COMPONENT
                           nextcode-remote-mirror

                         TARGET_SDKS NOT LINUX_STATIC)
