add_nextcode_host_library(nextcodeConstExtract STATIC
  ConstExtract.cpp
  )
target_link_libraries(nextcodeConstExtract PRIVATE
  nextcodeClangImporter
  nextcodeAST)

set_nextcode_llvm_is_available(nextcodeConstExtract)
