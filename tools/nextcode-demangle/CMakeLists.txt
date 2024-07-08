add_nextcode_host_tool(nextcode-demangle
  nextcode-demangle.cpp
  LLVM_LINK_COMPONENTS support
  NEXTCODE_COMPONENT compiler
  )
target_link_libraries(nextcode-demangle
                      PRIVATE
                        nextcodeDemangling)
