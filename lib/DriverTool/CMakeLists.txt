set(driver_sources_and_options
                driver.cpp
                sil_opt_main.cpp
                sil_func_extractor_main.cpp
                sil_nm_main.cpp
                sil_llvm_gen_main.cpp
                sil_passpipeline_dumper_main.cpp
                nextcode_dependency_tool_main.cpp
                nextcode_llvm_opt_main.cpp
                autolink_extract_main.cpp
                modulewrap_main.cpp
                nextcode_api_digester_main.cpp
                nextcode_cache_tool_main.cpp
                nextcode_indent_main.cpp
                nextcode_symbolgraph_extract_main.cpp
                nextcode_parse_test_main.cpp)

set(driver_common_libs
                nextcodeAPIDigester
                nextcodeDriver
                nextcodeFrontendTool
                nextcodeSymbolGraphGen
                LLVMBitstreamReader)

add_nextcode_host_library(nextcodeDriverTool STATIC
  ${driver_sources_and_options}
)
target_link_libraries(nextcodeDriverTool
                      PUBLIC
                      ${driver_common_libs})

if (NEXTCODE_BUILD_NEXTCODE_SYNTAX)
  target_link_libraries(nextcodeDriverTool
    PRIVATE
    nextcodeASTGen
  )

  add_dependencies(nextcodeDriverTool
    nextcodeASTGen
  )
endif()

# If building as part of clang, make sure the headers are installed.
if(NOT NEXTCODE_BUILT_STANDALONE)
  add_dependencies(nextcodeDriverTool clang-resource-headers)
endif()

set_nextcode_llvm_is_available(nextcodeDriverTool)

set(LLVM_TARGET_DEFINITIONS NeXTCodeCacheToolOptions.td)
nextcode_tablegen(NeXTCodeCacheToolOptions.inc -gen-opt-parser-defs)
nextcode_add_public_tablegen_target(NeXTCodeCacheToolOptions)

add_dependencies(nextcodeDriverTool NeXTCodeCacheToolOptions)
