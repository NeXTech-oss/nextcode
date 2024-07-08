
add_nextcode_host_library(nextcodeSerialization STATIC
  Deserialization.cpp
  DeserializeSIL.cpp
  ModuleFile.cpp
  ModuleFileSharedCore.cpp
  ScanningLoaders.cpp
  Serialization.cpp
  SerializedModuleLoader.cpp
  SerializedSILLoader.cpp
  SerializeDoc.cpp
  SerializeSIL.cpp

  LLVM_LINK_COMPONENTS
    BitstreamReader
  )
target_link_libraries(nextcodeSerialization PRIVATE
  nextcodeClangImporter
  nextcodeOption
  nextcodeAST
  nextcodeSIL
  clangIndex)

set_nextcode_llvm_is_available(nextcodeSerialization)
