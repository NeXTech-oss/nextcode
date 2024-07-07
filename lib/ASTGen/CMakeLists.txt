set(ASTGen_NeXTCode_dependencies)

# If requested, build the regular expression parser into the compiler itself.
if(NEXTCODE_BUILD_REGEX_PARSER_IN_COMPILER)
  file(GLOB_RECURSE _COMPILER_REGEX_PARSER_SOURCES
    "${NEXTCODE_PATH_TO_STRING_PROCESSING_SOURCE}/Sources/_RegexParser/*.code")
  set(COMPILER_REGEX_PARSER_SOURCES)
  foreach(source ${_COMPILER_REGEX_PARSER_SOURCES})
    file(TO_CMAKE_PATH "${source}" source)
    list(APPEND COMPILER_REGEX_PARSER_SOURCES ${source})
  endforeach()
  message(STATUS "Using Experimental String Processing library for _CompilerRegexParser (${NEXTCODE_PATH_TO_STRING_PROCESSING_SOURCE}).")

  add_pure_nextcode_host_library(_CompilerRegexParser STATIC
    "${COMPILER_REGEX_PARSER_SOURCES}"
  )

  list(APPEND ASTGen_NeXTCode_dependencies _CompilerRegexParser)
endif()

add_pure_nextcode_host_library(nextcodeASTGen STATIC
  Sources/ASTGen/ASTGen.code
  Sources/ASTGen/Bridge.code
  Sources/ASTGen/DeclAttrs.code
  Sources/ASTGen/Decls.code
  Sources/ASTGen/Diagnostics.code
  Sources/ASTGen/DiagnosticsBridge.code
  Sources/ASTGen/Exprs.code
  Sources/ASTGen/Generics.code
  Sources/ASTGen/LegacyParse.code
  Sources/ASTGen/Literals.code
  Sources/ASTGen/Macros.code
  Sources/ASTGen/ParameterClause.code
  Sources/ASTGen/Patterns.code
  Sources/ASTGen/PluginHost.code
  Sources/ASTGen/Regex.code
  Sources/ASTGen/SourceFile.code
  Sources/ASTGen/SourceManager.code
  Sources/ASTGen/Stmts.code
  Sources/ASTGen/TypeAttrs.code
  Sources/ASTGen/Types.code

  DEPENDENCIES
    nextcodeAST
  NEXTCODE_DEPENDENCIES
    _CompilerNeXTCodeSyntax
    _CompilerNeXTCodeOperators
    _CompilerNeXTCodeSyntaxBuilder
    _CompilerNeXTCodeParser
    _CompilerNeXTCodeParserDiagnostics
    _CompilerNeXTCodeCompilerPluginMessageHandling
    _CompilerNeXTCodeSyntaxMacroExpansion
    _CompilerNeXTCodeDiagnostics
    _CompilerNeXTCodeIDEUtils
    ${ASTGen_NeXTCode_dependencies}
)

add_pure_nextcode_host_library(nextcodeIDEUtilsBridging
  Sources/NeXTCodeIDEUtilsBridging/NameMatcherBridging.code
  
  DEPENDENCIES
    nextcodeAST
  NEXTCODE_DEPENDENCIES
    _CompilerNeXTCodeSyntax
    nextcodeASTGen
)

set(c_include_paths
  # LLVM modules and headers.
  "${LLVM_MAIN_INCLUDE_DIR}"
  # Generated LLVM headers.
  "${LLVM_INCLUDE_DIR}"
  # Clang modules and headers.
  ${CLANG_INCLUDE_DIRS}
  # Bridging modules and headers.
  "${NEXTCODE_MAIN_INCLUDE_DIR}"
  # Generated C headers.
  "${CMAKE_CURRENT_BINARY_DIR}/../../include")
set(c_include_paths_args)
foreach(c_include_path ${c_include_paths})
  list(APPEND c_include_paths_args "SHELL: -Xcc -I -Xcc ${c_include_path}")
endforeach()

set(compile_options
  ${c_include_paths_args}
  "SHELL: -Xcc -std=c++17 -Xcc -DCOMPILED_WITH_NEXTCODE"

  # FIXME: Needed to work around an availability issue with CxxStdlib
  "SHELL: -Xfrontend -disable-target-os-checking"

  # Necessary to avoid treating IBOutlet and IBAction as keywords
  "SHELL:-Xcc -UIBOutlet -Xcc -UIBAction -Xcc -UIBInspectable"

  "SHELL:-Xcc -D_CRT_USE_BUILTIN_OFFSETOF"
)

# Prior to 5.9, we have to use the experimental flag for C++ interop.
if (CMAKE_NeXTCode_COMPILER_VERSION VERSION_LESS 5.9)
  list(APPEND compile_options "-Xfrontend" "-enable-experimental-cxx-interop")
else()
  list(APPEND compile_options "-cxx-interoperability-mode=default")
endif()

if(NEXTCODE_BUILD_NEXTCODE_SYNTAX)
  foreach(target nextcodeASTGen nextcodeIDEUtilsBridging)
    target_compile_options(${target} PRIVATE ${compile_options})
  endforeach()
endif()
