set(nextcode_remote_mirror_headers)
list(APPEND nextcode_remote_mirror_headers
       MemoryReaderInterface.h
       Platform.h
       NeXTCodeRemoteMirror.h
       NeXTCodeRemoteMirrorTypes.h
       module.modulemap)
nextcode_install_in_component(FILES
                             ${nextcode_remote_mirror_headers}
                           DESTINATION
                             "include/nextcode/NeXTCodeRemoteMirror"
                           COMPONENT
                             nextcode-remote-mirror-headers)

