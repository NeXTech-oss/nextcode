add_nextcode_host_library(nextcodeRefactoring STATIC
  AddEquatableConformance.cpp
  AddExplicitCodableImplementation.cpp
  Async/AsyncConverter.cpp
  Async/AsyncHandlerDesc.cpp
  Async/AsyncRefactorings.cpp
  Async/CallbackClassifier.cpp
  Async/CallbackCondition.cpp
  Async/DeclCollector.cpp
  Async/DeclReferenceFinder.cpp
  Async/ReferenceCollector.cpp
  Async/ScopedDeclCollector.cpp
  Async/Utils.cpp
  CollapseNestedIfStmt.cpp
  ConvertGuardExprToIfLetExpr.cpp
  ConvertIfLetExprToGuardExpr.cpp
  ConvertStringConcatenationToInterpolation.cpp
  ConvertToComputedProperty.cpp
  ConvertToDoCatch.cpp
  ConvertToSwitchStmt.cpp
  ConvertToTernaryExpr.cpp
  ExpandMacro.cpp
  ExpandSwitchCases.cpp
  ExpandTernaryExpr.cpp
  ExtractExpr.cpp
  ExtractExprBase.cpp
  ExtractFunction.cpp
  ExtractRepeatedExpr.cpp
  FillProtocolStubs.cpp
  LocalizeString.cpp
  LocalRename.cpp
  MemberwiseInitLocalRefactoring.cpp
  MoveMembersToExtension.cpp
  Refactoring.cpp
  RefactoringAction.cpp
  ReplaceBodiesWithFatalError.cpp
  SimplifyNumberLiteral.cpp
  SyntacticRename.cpp
  SyntacticRenameRangeDetails.cpp
  TrailingClosure.cpp
  Utils.cpp
)

target_link_libraries(nextcodeRefactoring PRIVATE
  nextcodeAST
  nextcodeClangImporter
  nextcodeIDE
  nextcodeIndex
  nextcodeParse
  nextcodeSema)

if(NEXTCODE_BUILD_NEXTCODE_SYNTAX)
  target_link_libraries(nextcodeRefactoring PRIVATE
    nextcodeIDEUtilsBridging
  )
endif()

set_nextcode_llvm_is_available(nextcodeRefactoring)
