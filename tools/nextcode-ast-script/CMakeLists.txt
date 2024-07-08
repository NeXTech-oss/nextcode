add_nextcode_host_tool(nextcode-ast-script
  ASTScriptConfiguration.cpp
  ASTScriptParser.cpp
  ASTScriptEvaluator.cpp
  nextcode-ast-script.cpp
  NEXTCODE_COMPONENT tools
  THINLTO_LD64_ADD_FLTO_CODEGEN_ONLY
)
target_link_libraries(nextcode-ast-script
                      PRIVATE
                        nextcodeAST
                        nextcodeFrontendTool)
