add_nextcode_host_library(nextcodeSymbolGraphGen STATIC
  AvailabilityMixin.cpp
  DeclarationFragmentPrinter.cpp
  Edge.cpp
  JSON.cpp
  Symbol.cpp
  SymbolGraph.cpp
  SymbolGraphGen.cpp
  SymbolGraphASTWalker.cpp

  LLVM_LINK_COMPONENTS
  TargetParser
  )

target_link_libraries(nextcodeSymbolGraphGen PRIVATE
  nextcodeAST
  nextcodeIDE
  nextcodeMarkup)
