add_nextcode_host_library(nextcodeDemangle SHARED
  NeXTCodeDemangle.cpp
  MangleHack.cpp)
target_compile_definitions(nextcodeDemangle PRIVATE
  LLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING=1)
target_link_libraries(nextcodeDemangle PRIVATE
  nextcodeDemangling)

add_dependencies(compiler nextcodeDemangle)
nextcode_install_in_component(TARGETS nextcodeDemangle
  RUNTIME
    DESTINATION "bin"
    COMPONENT compiler
  LIBRARY
    DESTINATION "lib${LLVM_LIBDIR_SUFFIX}"
    COMPONENT compiler
  ARCHIVE
    DESTINATION "lib${LLVM_LIBDIR_SUFFIX}"
    COMPONENT compiler)
