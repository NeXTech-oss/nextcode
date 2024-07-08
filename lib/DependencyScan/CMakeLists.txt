add_nextcode_host_library(nextcodeDependencyScanImpl STATIC
  DependencyScanJSON.cpp
  StringUtils.cpp)

add_nextcode_host_library(nextcodeDependencyScan STATIC
  DependencyScanningTool.cpp
  ModuleDependencyCacheSerialization.cpp
  ModuleDependencyScanner.cpp
  ScanDependencies.cpp)

target_link_libraries(nextcodeDependencyScan INTERFACE
  clangBasic)

target_link_libraries(nextcodeDependencyScan PRIVATE
  nextcodeClangImporter
  nextcodeAST
  nextcodeSerialization)

target_link_libraries(nextcodeDependencyScan PUBLIC
  nextcodeDependencyScanImpl)
