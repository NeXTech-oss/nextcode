add_nextcode_host_library(nextcodeImmediate STATIC
  NeXTCodeMaterializationUnit.cpp
  Immediate.cpp
  LLVM_LINK_COMPONENTS
    executionengine
    linker
    mcjit
    orcjit
    orctargetprocess
    jitlink
    transformutils)
target_link_libraries(nextcodeImmediate PRIVATE
  nextcodeFrontend
  nextcodeIDE
  nextcodeIRGen
  nextcodeSILGen
  nextcodeSILOptimizer)
