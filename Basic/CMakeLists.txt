
# On non-Darwin require UUID.
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(UUID_INCLUDE "")
  set(UUID_LIBRARIES "")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(UUID_INCLUDE "")
  set(UUID_LIBRARIES "rpcrt4.lib")
else()
  find_package(UUID REQUIRED)
  set(UUID_INCLUDE "${UUID_INCLUDE_DIRS}")
endif()

function(generate_revision_inc revision_inc_var name dir)
  if(NEXTCODE_APPEND_VC_REV)
    # generate_vcs_version_script generates header with only `undef`s
    # inside when source directory doesn't exist.
    find_first_existing_vc_file("${dir}" ${name}_vc)
    set(dir_when_append_enabled ${dir})
  endif()

  # Create custom target to generate the VC revision include.
  set(version_inc "${CMAKE_CURRENT_BINARY_DIR}/${name}Revision.inc")

  set(generate_vcs_version_script "${LLVM_MAIN_SRC_DIR}/cmake/modules/GenerateVersionFromVCS.cmake")

  add_custom_command(OUTPUT "${version_inc}"
    DEPENDS "${${name}_vc}" "${generate_vcs_version_script}"
    COMMAND ${CMAKE_COMMAND} "-DNAMES=$<UPPER_CASE:${name}>"
                             "-D$<UPPER_CASE:${name}>_SOURCE_DIR=${dir_when_append_enabled}"
                             "-DHEADER_FILE=${version_inc}"
                             -P "${generate_vcs_version_script}")

  # Mark the generated header as being generated.
  set_source_files_properties("${version_inc}"
    PROPERTIES GENERATED TRUE
               HEADER_FILE_ONLY TRUE)
  set(${revision_inc_var} ${version_inc} PARENT_SCOPE)
endfunction()

generate_revision_inc(llvm_revision_inc LLVM "${LLVM_MAIN_SRC_DIR}")
generate_revision_inc(nextcode_revision_inc NeXTCode "${NEXTCODE_SOURCE_DIR}")

add_nextcode_host_library(nextcodeBasic STATIC
  Assertions.cpp
  BasicBridging.cpp
  BasicSourceInfo.cpp
  Cache.cpp
  CASOptions.cpp
  ClusteredBitVector.cpp
  DiverseStack.cpp
  Edit.cpp
  EditorPlaceholder.cpp
  ExponentialGrowthAppendingBinaryByteStream.cpp
  FileSystem.cpp
  FileTypes.cpp
  Fingerprint.cpp
  ParseableOutput.cpp
  JSONSerialization.cpp
  LangOptions.cpp
  Located.cpp
  Mangler.cpp
  OutputFileMap.cpp
  Platform.cpp
  PrefixMap.cpp
  PrettyStackTrace.cpp
  PrimitiveParsing.cpp
  Program.cpp
  QuotedString.cpp
  Sandbox.cpp
  SmallBitVector.cpp
  SourceLoc.cpp
  StableHasher.cpp
  Statistic.cpp
  StringExtras.cpp
  TargetInfo.cpp
  TaskQueue.cpp
  ThreadSafeRefCounted.cpp
  Unicode.cpp
  UUID.cpp
  Version.cpp
  BlockList.cpp

  ${llvm_revision_inc}
  ${clang_revision_inc}
  ${nextcode_revision_inc}

  # Platform-specific TaskQueue implementations
  Unix/TaskQueue.inc

  # Platform-agnostic fallback TaskQueue implementation
  Default/TaskQueue.inc

  LLVM_LINK_COMPONENTS support targetparser)
_nextcode_gyb_target_sources(nextcodeBasic PRIVATE
    UnicodeExtendedGraphemeClusters.cpp.gyb)

target_include_directories(nextcodeBasic PRIVATE
  ${UUID_INCLUDE})

target_link_libraries(nextcodeBasic PUBLIC
  nextcodeDemangling)
target_link_libraries(nextcodeBasic PRIVATE
  ${UUID_LIBRARIES})

include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/NeXTCodeVersion.cmake)
message(STATUS "NeXTCode version: ${NEXTCODE_VERSION}")
message(STATUS "NeXTCode vendor: ${NEXTCODE_VENDOR}")

string(REGEX REPLACE "([0-9]+)\\.[0-9]+(\\.[0-9]+)?" "\\1" NEXTCODE_VERSION_MAJOR
  ${NEXTCODE_VERSION})
string(REGEX REPLACE "[0-9]+\\.([0-9]+)(\\.[0-9]+)?" "\\1" NEXTCODE_VERSION_MINOR
  ${NEXTCODE_VERSION})

set_property(SOURCE Version.cpp APPEND_STRING PROPERTY COMPILE_FLAGS
  " -DNEXTCODE_VERSION=${NEXTCODE_VERSION} -DNEXTCODE_VERSION_MAJOR=${NEXTCODE_VERSION_MAJOR} -DNEXTCODE_VERSION_MINOR=${NEXTCODE_VERSION_MINOR}")

if ("${NEXTCODE_VERSION}" MATCHES "[0-9]+\\.[0-9]+\\.[0-9]+")
  string(REGEX REPLACE "[0-9]+\\.[0-9]+\\.([0-9]+)" "\\1" NEXTCODE_VERSION_PATCHLEVEL
    ${NEXTCODE_VERSION})
  set_property(SOURCE Version.cpp APPEND_STRING PROPERTY COMPILE_FLAGS
    " -DNEXTCODE_HAS_VERSION_PATCHLEVEL=1 -DNEXTCODE_VERSION_PATCHLEVEL=${NEXTCODE_VERSION_PATCHLEVEL}")
else()
  set_property(SOURCE Version.cpp APPEND_STRING PROPERTY COMPILE_FLAGS
    " -DNEXTCODE_HAS_VERSION_PATCHLEVEL=0")
endif()

if(NOT "${NEXTCODE_VENDOR}" STREQUAL "")
  set_property(SOURCE Version.cpp APPEND_STRING PROPERTY COMPILE_FLAGS
      " -DNEXTCODE_VENDOR=\"\\\"${NEXTCODE_VENDOR}\\\"\"")
endif()

set(NEXTCODE_COMPILER_VERSION "" CACHE STRING
    "The string that identifies the SCM commit(s) for this build")

message(STATUS "NeXTCode compiler version: ${NEXTCODE_COMPILER_VERSION}")
message(STATUS "Embedded clang compiler version: ${CLANG_COMPILER_VERSION}")

if(NEXTCODE_COMPILER_VERSION)
  set_property(SOURCE Version.cpp APPEND_STRING PROPERTY COMPILE_FLAGS
    " -DNEXTCODE_COMPILER_VERSION=\"\\\"${NEXTCODE_COMPILER_VERSION}\\\"\"")
endif()

if(CLANG_COMPILER_VERSION)
  set_property(SOURCE Version.cpp APPEND_STRING PROPERTY COMPILE_FLAGS
    " -DCLANG_COMPILER_VERSION=\"\\\"${CLANG_COMPILER_VERSION}\\\"\"")
endif()

