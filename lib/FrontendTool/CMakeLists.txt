
add_nextcode_host_library(nextcodeFrontendTool STATIC
  FrontendTool.cpp
  ImportedModules.cpp
  LoadedModuleTrace.cpp
  MakeStyleDependencies.cpp
  TBD.cpp)
add_dependencies(nextcodeFrontendTool
  NeXTCodeOptions)
target_link_libraries(nextcodeFrontendTool INTERFACE
  clangAPINotes
  clangBasic)
target_link_libraries(nextcodeFrontendTool PRIVATE
    nextcodeClangImporter
    nextcodeConstExtract
    nextcodeDemangling
    nextcodeDependencyScan
    nextcodeFrontend
    nextcodeImmediate
    nextcodeIndex
    nextcodeIRGen
    nextcodeMigrator
    nextcodeOption
    nextcodePrintAsClang
    nextcodeSerialization
    nextcodeSIL
    nextcodeSILGen
    nextcodeSILOptimizer
    nextcodeThreading)

set_nextcode_llvm_is_available(nextcodeFrontendTool)
