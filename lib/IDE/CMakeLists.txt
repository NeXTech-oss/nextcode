
add_nextcode_host_library(nextcodeIDE STATIC
  AfterPoundExprCompletion.cpp
  APIDigesterData.cpp
  ArgumentCompletion.cpp
  CodeCompletion.cpp
  CodeCompletionCache.cpp
  CodeCompletionContext.cpp
  CodeCompletionDiagnostics.cpp
  CodeCompletionResult.cpp
  CodeCompletionResultBuilder.cpp
  CodeCompletionResultPrinter.cpp
  CodeCompletionResultType.cpp
  CodeCompletionString.cpp
  CodeCompletionStringPrinter.cpp
  CommentConversion.cpp
  CompletionLookup.cpp
  CompletionOverrideLookup.cpp
  ConformingMethodList.cpp
  CursorInfo.cpp
  ExprCompletion.cpp
  ExprContextAnalysis.cpp
  Formatting.cpp
  FuzzyStringMatcher.cpp
  IDEBridging.cpp
  IDERequests.cpp
  IDETypeChecking.cpp
  ImportDepth.cpp
  KeyPathCompletion.cpp
  ModuleInterfacePrinting.cpp
  PostfixCompletion.cpp
  REPLCodeCompletion.cpp
  SelectedOverloadInfo.cpp
  SourceEntityWalker.cpp
  NeXTCodeSourceDocInfo.cpp
  SyntaxModel.cpp
  TypeCheckCompletionCallback.cpp
  TypeContextInfo.cpp
  UnresolvedMemberCompletion.cpp
  Utils.cpp
  )
target_link_libraries(nextcodeIDE PRIVATE
  nextcodeAST
  nextcodeClangImporter
  nextcodeDriver
  nextcodeParse
  nextcodeSema)

if (NEXTCODE_BUILD_NEXTCODE_SYNTAX)
  target_link_libraries(nextcodeIDE PRIVATE
    nextcodeIDEUtilsBridging
  )
endif()

set_nextcode_llvm_is_available(nextcodeIDE)
