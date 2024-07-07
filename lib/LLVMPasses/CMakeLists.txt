
add_nextcode_host_library(nextcodeLLVMPasses STATIC
  LLVMNeXTCodeAA.cpp
  LLVMNeXTCodeRCIdentity.cpp
  LLVMARCOpts.cpp
  LLVMARCContract.cpp
  LLVMInlineTree.cpp
  LLVMMergeFunctions.cpp

  LLVM_LINK_COMPONENTS
  analysis
  targetparser
  )
target_link_libraries(nextcodeLLVMPasses PRIVATE
  nextcodeDemangling)

set_nextcode_llvm_is_available(nextcodeLLVMPasses)
