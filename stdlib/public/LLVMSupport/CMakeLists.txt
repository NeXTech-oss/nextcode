add_nextcode_target_library(nextcodeLLVMSupport OBJECT_LIBRARY
    ErrorHandling.cpp
    Hashing.cpp
    MemAlloc.cpp
    SmallPtrSet.cpp
    SmallVector.cpp
    StringRef.cpp
  INSTALL_IN_COMPONENT
    never_install)
