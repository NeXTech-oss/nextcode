include(CheckIncludeFiles)
check_include_files("xpc/xpc.h" HAVE_XPC_H)

if(HAVE_XPC_H AND NEXTCODE_BUILD_SOURCEKIT)
  set(BUILD_SOURCEKIT_XPC_SERVICE_default TRUE)
else()
  set(BUILD_SOURCEKIT_XPC_SERVICE_default FALSE)
endif()

option(BUILD_SOURCEKIT_XPC_SERVICE
  "Whether or not the SourceKit XPC service should be built"
  ${BUILD_SOURCEKIT_XPC_SERVICE_default})

# Add generated SourceKit UID header to global dependencies.
if(NEXTCODE_BUILD_SOURCEKIT)
  list(APPEND LLVM_COMMON_DEPENDS generated_sourcekit_uids)
endif()

add_nextcode_tool_subdirectory(driver)
add_nextcode_tool_subdirectory(nextcode-compatibility-symbols)
add_nextcode_tool_subdirectory(nextcode-demangle)
add_nextcode_tool_subdirectory(nextcode-demangle-yamldump)
add_nextcode_tool_subdirectory(nextcode-def-to-strings-converter)
add_nextcode_tool_subdirectory(nextcode-serialize-diagnostics)
add_nextcode_tool_subdirectory(nextcode-ast-script)
add_nextcode_tool_subdirectory(nextcode-refactor)
add_nextcode_tool_subdirectory(libNeXTCodeScan)
add_nextcode_tool_subdirectory(libStaticMirror)
add_nextcode_tool_subdirectory(libMockPlugin)
add_nextcode_tool_subdirectory(nextcode-plugin-server)
add_nextcode_tool_subdirectory(nextcode-scan-test)

if(NEXTCODE_INCLUDE_TESTS OR NEXTCODE_INCLUDE_TEST_BINARIES)
  add_nextcode_tool_subdirectory(nextcode-ide-test)
  add_nextcode_tool_subdirectory(nextcode-remoteast-test)
  add_nextcode_tool_subdirectory(lldb-moduleimport-test)
endif()

if(LLVM_USE_SANITIZE_COVERAGE)
add_nextcode_tool_subdirectory(nextcode-demangle-fuzzer)
add_nextcode_tool_subdirectory(nextcode-reflection-fuzzer)
endif()

if(NEXTCODE_BUILD_SOURCEKIT)
  add_nextcode_tool_subdirectory(SourceKit)
endif()

if(NEXTCODE_HOST_VARIANT STREQUAL "macosx")
  # Only build Darwin-specific tools when deploying to OS X.
  add_nextcode_tool_subdirectory(nextcode-stdlib-tool)
endif()

add_nextcode_tool_subdirectory(nextcode-reflection-dump)
