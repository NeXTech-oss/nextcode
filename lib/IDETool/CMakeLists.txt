
add_nextcode_host_library(nextcodeIDETool STATIC
  CompileInstance.cpp
  CompilerInvocation.cpp
  IDEInspectionInstance.cpp
  DependencyChecking.cpp
  SyntacticMacroExpansion.cpp
  )

target_link_libraries(nextcodeIDETool PRIVATE
  nextcodeAST
  nextcodeDriver
  nextcodeFrontend
  nextcodeFrontendTool
  clangAST
  clangFrontend)

set_nextcode_llvm_is_available(nextcodeIDETool)
