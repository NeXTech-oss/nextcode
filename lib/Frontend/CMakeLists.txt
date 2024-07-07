
add_nextcode_host_library(nextcodeFrontend STATIC
  ArgsToFrontendInputsConverter.cpp
  ArgsToFrontendOptionsConverter.cpp
  ArgsToFrontendOutputsConverter.cpp
  CachedDiagnostics.cpp
  CachingUtils.cpp
  CASOutputBackends.cpp
  CompileJobCacheKey.cpp
  CompileJobCacheResult.cpp
  CompilerInvocation.cpp
  DependencyVerifier.cpp
  DiagnosticHelper.cpp
  DiagnosticVerifier.cpp
  Frontend.cpp
  FrontendInputsAndOutputs.cpp
  FrontendOptions.cpp
  ModuleInterfaceBuilder.cpp
  ModuleInterfaceLoader.cpp
  ModuleInterfaceSupport.cpp
  PrintingDiagnosticConsumer.cpp
  Serialization.cpp
  SerializedDiagnosticConsumer.cpp)
add_dependencies(nextcodeFrontend
  NeXTCodeOptions)
target_link_libraries(nextcodeFrontend PRIVATE
  nextcodeAPIDigester
  nextcodeAST
  nextcodeConstExtract
  nextcodeIDE
  nextcodeSIL
  nextcodeOption
  nextcodeSILGen
  nextcodeSILOptimizer
  nextcodeIRGen
  nextcodeLocalization
  nextcodeSema
  nextcodeSerialization
  nextcodeSymbolGraphGen)

if (NEXTCODE_BUILD_NEXTCODE_SYNTAX)
  target_link_libraries(nextcodeFrontend
    PRIVATE
    nextcodeASTGen
  )

  add_dependencies(nextcodeFrontend
    nextcodeASTGen
  )
endif()

set_nextcode_llvm_is_available(nextcodeFrontend)
