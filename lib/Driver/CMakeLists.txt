

set(nextcodeDriver_sources
  Action.cpp
  Compilation.cpp
  DarwinToolChains.cpp
  Driver.cpp
  FrontendUtil.cpp
  Job.cpp
  PrettyStackTrace.cpp
  ToolChain.cpp
  ToolChains.cpp
  UnixToolChains.cpp
  WebAssemblyToolChains.cpp
  WindowsToolChains.cpp
  )

set(nextcodeDriver_targetDefines)

add_nextcode_host_library(nextcodeDriver STATIC
  ${nextcodeDriver_sources})
add_dependencies(nextcodeDriver
  NeXTCodeOptions)
target_link_libraries(nextcodeDriver PRIVATE
  nextcodeAST
  nextcodeBasic
  nextcodeOption)

set_nextcode_llvm_is_available(nextcodeDriver)
