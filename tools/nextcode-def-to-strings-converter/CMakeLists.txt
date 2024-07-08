add_nextcode_host_tool(nextcode-def-to-strings-converter
  nextcode-def-to-strings-converter.cpp
  NEXTCODE_COMPONENT tools
  DOES_NOT_USE_NEXTCODE
)

target_link_libraries(nextcode-def-to-strings-converter PRIVATE
  nextcodeLocalization)
