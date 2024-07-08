add_nextcode_host_tool(lldb-moduleimport-test
  lldb-moduleimport-test.cpp
  NEXTCODE_COMPONENT testsuite-tools
)
target_link_libraries(lldb-moduleimport-test
                      PRIVATE
                        nextcodeAST
                        nextcodeASTSectionImporter
                        nextcodeClangImporter
                        nextcodeFrontend)

