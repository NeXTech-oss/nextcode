add_nextcode_host_library(nextcodeMarkup STATIC
  AST.cpp
  LineList.cpp
  Markup.cpp)
target_link_libraries(nextcodeMarkup PRIVATE
  libcmark-gfm)
