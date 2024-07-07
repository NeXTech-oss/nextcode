

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  set(NEXTCODE_GYB_FLAGS --line-directive "^\"#line %(line)d \\\"%(file)s\\\"^\"")
else()
  set(NEXTCODE_GYB_FLAGS --line-directive "\'#line" "%(line)d" "\"%(file)s\"\'")
endif()

add_nextcode_host_library(nextcodeParse STATIC
  Confusables.cpp
  Lexer.cpp
  ParseBridging.cpp
  ParseDecl.cpp
  ParseExpr.cpp
  ParseGeneric.cpp
  ParseIfConfig.cpp
  ParsePattern.cpp
  Parser.cpp
  ParseRegex.cpp
  ParseRequests.cpp
  ParseStmt.cpp
  ParseType.cpp
  ParseVersion.cpp
  PersistentParserState.cpp)
target_link_libraries(nextcodeParse PRIVATE
  nextcodeAST
)

if (NEXTCODE_BUILD_NEXTCODE_SYNTAX)
  target_link_libraries(nextcodeParse
    PRIVATE
    nextcodeASTGen
  )

  add_dependencies(nextcodeParse
    nextcodeASTGen
  )
endif()

if(NEXTCODE_BUILD_REGEX_PARSER_IN_COMPILER)
  target_compile_definitions(nextcodeParse
    PRIVATE
    NEXTCODE_BUILD_REGEX_PARSER_IN_COMPILER
  )
endif()

if(NEXTCODE_COMPILER_VERSION)
  set_property(SOURCE ParseVersion.cpp APPEND_STRING PROPERTY COMPILE_FLAGS
    " -DNEXTCODE_COMPILER_VERSION=\"\\\"${NEXTCODE_COMPILER_VERSION}\\\"\"")
endif()

set_nextcode_llvm_is_available(nextcodeParse)
