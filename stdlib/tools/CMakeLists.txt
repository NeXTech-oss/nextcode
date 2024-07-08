# Keep in sync with test/CMakeLists.txt: nextcode-reflection-test is
# only used when testing dynamic stdlib.
if(NEXTCODE_BUILD_DYNAMIC_STDLIB AND (NEXTCODE_INCLUDE_TESTS OR NEXTCODE_INCLUDE_TEST_BINARIES))
  add_subdirectory(nextcode-reflection-test)
endif()
