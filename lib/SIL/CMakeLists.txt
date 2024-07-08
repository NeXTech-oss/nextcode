add_nextcode_host_library(nextcodeSIL STATIC
  SIL.cpp)
target_link_libraries(nextcodeSIL PUBLIC
  nextcodeDemangling)
target_link_libraries(nextcodeSIL PRIVATE
  nextcodeAST
  nextcodeClangImporter
  nextcodeSema
  nextcodeSerialization)

add_subdirectory(IR)
add_subdirectory(Utils)
add_subdirectory(Verifier)
add_subdirectory(Parser)

# intrinsics_gen is the LLVM tablegen target that generates the include files
# where intrinsics and attributes are declared. nextcodeSIL depends on these
# headers.
# For more information see the comment at the top of lib/CMakeLists.txt.
add_dependencies(nextcodeSIL intrinsics_gen clang-tablegen-targets)

set_nextcode_llvm_is_available(nextcodeSIL)
