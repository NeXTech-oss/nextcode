add_nextcode_host_library(nextcodeStaticMirror STATIC
   BinaryScanningTool.cpp
   ObjectFileContext.cpp
   LLVM_LINK_COMPONENTS object support)

 target_link_libraries(nextcodeStaticMirror PRIVATE
     nextcodeRemoteInspection)
