add_nextcode_host_library(nextcodeIndex STATIC
  Index.cpp
  IndexDataConsumer.cpp
  IndexRecord.cpp
  IndexSymbol.cpp)
target_link_libraries(nextcodeIndex PRIVATE
  nextcodeAST
  nextcodeIDE)
