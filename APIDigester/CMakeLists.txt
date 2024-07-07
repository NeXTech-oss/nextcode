

add_nextcode_host_library(nextcodeAPIDigester STATIC
  ModuleAnalyzerNodes.cpp
  ModuleDiagsConsumer.cpp)

target_link_libraries(nextcodeAPIDigester PRIVATE
  nextcodeIDE)

set_nextcode_llvm_is_available(nextcodeAPIDigester)
